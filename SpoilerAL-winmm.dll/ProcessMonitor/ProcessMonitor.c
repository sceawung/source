#include <windows.h>
#include <winternl.h>
#include <dhcpsapi.h>
#include <WbemIdl.h>
#pragma comment(lib, "wbemuuid.lib")

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <mbstring.h>

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(_InterlockedCompareExchange)
#else
#define _InterlockedCompareExchange InterlockedCompareExchange
#endif

#include <pdh.h>
#pragma comment(lib, "pdh.lib")
#ifndef PDH_MAX_COUNTER_PATH
#define PDH_MAX_COUNTER_PATH 2048
#endif

#define PSAPI_VERSION 1
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#include <Shlwapi.h>

#ifndef __BORLANDC__
#define USING_REGEX 1
#endif

#if USING_REGEX
#include <regex.h>
#endif

#include "TDrawGrid.h"
#include "ProcessMonitor.h"
#include "GetFileTitlePointer.h"
#include "ProcessInfoValidation.h"
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#define typename PROCESSENTRY32A
#include "bcb6_std_vector_template.h"
#undef  typename

#pragma warning(disable:4996)

#ifdef __BORLANDC__
#pragma warn -8065
#endif

extern HANDLE hHeap;

static CRITICAL_SECTION          cs;
static BOOL             volatile bInitialized          = FALSE;
static HANDLE           volatile hObjectToSignal       = NULL;
static HANDLE           volatile hMonitorThread        = NULL;
static size_t           volatile nMonitorPIDsCapacity  = 0;
static LPDWORD          volatile lpdwMonitorPIDs       = NULL;
static LPDWORD          volatile lpdwMonitorEndOfPIDs  = NULL;
static size_t           volatile nMonitorNamesCapacity = 0;
static LPBYTE           volatile lpMonitorNames        = NULL;

void __cdecl InitializeProcessMonitor()
{
	InitializeCriticalSection(&cs);
	hObjectToSignal = CreateEventW(NULL, FALSE, FALSE, NULL);
	if (SUCCEEDED(CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		CoInitializeSecurity(
			NULL,
			-1,                          // COM negotiates service
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities
			NULL                         // Reserved
		);
		CoUninitialize();
	}
	bInitialized = TRUE;
}

void __cdecl DeleteProcessMonitor()
{
	StopProcessMonitor();
	CloseHandle(hObjectToSignal);
	DeleteCriticalSection(&cs);
	bInitialized = FALSE;
}

void __cdecl StopProcessMonitor()
{
	HANDLE hThread;

	hThread = hMonitorThread;
	hMonitorThread = NULL;
	EnterCriticalSection(&cs);
	nMonitorPIDsCapacity = 0;
	if (lpdwMonitorPIDs)
	{
		HeapFree(hHeap, 0, lpdwMonitorPIDs);
		lpdwMonitorPIDs = NULL;
	}
	lpdwMonitorEndOfPIDs = NULL;
	nMonitorNamesCapacity = 0;
	if (lpMonitorNames)
	{
		HeapFree(hHeap, 0, lpMonitorNames);
		lpMonitorNames = NULL;
	}
	LeaveCriticalSection(&cs);
	if (hThread)
	{
		QueueUserAPC(SetLastError, hThread, 0);
		WaitForSingleObject(hThread, INFINITE);
	}
}

static BOOL __cdecl EnumProcessId()
{
	size_t  nPIDsCapacity;
	LPDWORD lpdwPIDs;
	size_t  nNamesCapacity;
	LPBYTE  lpNames;
	DWORD   dwPIDsSize;

	if (lpdwMonitorPIDs)
	{
		nPIDsCapacity  = nMonitorPIDsCapacity;
		lpdwPIDs       = lpdwMonitorPIDs;
		nNamesCapacity = nMonitorNamesCapacity;
		lpNames        = lpMonitorNames;
		nMonitorPIDsCapacity  = 0;
		lpdwMonitorPIDs       = NULL;
		lpdwMonitorEndOfPIDs  = NULL;
		nMonitorNamesCapacity = 0;
		lpMonitorNames        = NULL;
	}
	else
	{
		lpNames = (LPBYTE)HeapAlloc(hHeap, 0, nNamesCapacity = 4096);
		if (!lpNames)
			goto FAILED1;
		lpdwPIDs = (LPDWORD)HeapAlloc(hHeap, 0, nPIDsCapacity = 1024 * sizeof(DWORD));
		if (!lpdwPIDs)
			goto FAILED2;
	}
	while (EnumProcesses(lpdwPIDs, nPIDsCapacity, &dwPIDsSize))
	{
		size_t  nNamesSize;
		LPBYTE  lpBaseName;
		LPDWORD lpdwEndOfPIDs, lpdwProcessId;

		if (nPIDsCapacity == dwPIDsSize)
		{
			HeapFree(hHeap, 0, lpdwPIDs);
			lpdwPIDs = (LPDWORD)HeapAlloc(hHeap, 0, nPIDsCapacity *= 2);
			if (lpdwPIDs)
				continue;
			else
				goto FAILED2;
		}
		nNamesSize = 0;
		lpBaseName = lpNames;
		lpdwEndOfPIDs = (LPDWORD)((LPBYTE)lpdwPIDs + (dwPIDsSize & -(signed)sizeof(DWORD)));
		for (lpdwProcessId = lpdwPIDs; lpdwProcessId != lpdwEndOfPIDs; lpdwProcessId++)
		{
			DWORD        dwLength = 0;
			HANDLE const hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, *lpdwProcessId);
			if (hProcess)
			{
				WCHAR lpFilename[MAX_PATH];
				dwLength = GetModuleBaseNameW(hProcess, NULL, lpFilename, _countof(lpFilename));
				if (dwLength && (dwLength = WideCharToMultiByte(CP_ACP, 0, lpFilename, dwLength + 1, lpBaseName + sizeof(DWORD), MAX_PATH, NULL, NULL)))
					dwLength--;
				CloseHandle(hProcess);
			}
			*(LPDWORD)lpBaseName = dwLength;
			nNamesSize += sizeof(DWORD) + dwLength + __alignof(DWORD)/* included \0 */ & -(signed)__alignof(DWORD);
			if (nNamesSize > nNamesCapacity)
			{
				LPVOID lpMem;

				lpBaseName -= (size_t)lpNames;
				lpMem = HeapReAlloc(hHeap, 0, lpNames, nNamesCapacity *= 2);
				if (!lpMem)
					goto FAILED3;
				lpNames = (LPBYTE)lpMem;
				lpBaseName += (size_t)lpMem;
			}
			lpBaseName += sizeof(DWORD) + dwLength + __alignof(DWORD)/* included \0 */ & -(signed)__alignof(DWORD);
		}
		nMonitorPIDsCapacity = nPIDsCapacity;
		lpdwMonitorPIDs = lpdwPIDs;
		lpdwMonitorEndOfPIDs = lpdwEndOfPIDs;
		nMonitorNamesCapacity = nNamesCapacity;
		lpMonitorNames = lpNames;
		return TRUE;
	}
FAILED3:
	HeapFree(hHeap, 0, lpdwPIDs);
FAILED2:
	HeapFree(hHeap, 0, lpNames);
FAILED1:
	return FALSE;
}

struct EventSink
{
	const IWbemObjectSink super;
	volatile LONG m_lRef;
	volatile BOOL bDone;
};

static HRESULT STDMETHODCALLTYPE QueryInterface(
	__RPC__in IWbemObjectSink * This,
	/* [in] */ __RPC__in REFIID riid,
	/* [annotation][iid_is][out] */
	_COM_Outptr_  void **ppvObject)
{
	if (InlineIsEqualGUID(riid, &IID_IUnknown) || InlineIsEqualGUID(riid, &IID_IWbemObjectSink))
	{
		*ppvObject = This;
		This->lpVtbl->AddRef(This);
		return WBEM_S_NO_ERROR;
	}
	return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE AddRef(
	__RPC__in IWbemObjectSink * This)
{
	return _InterlockedIncrement(&((struct EventSink *)This)->m_lRef);
}

static ULONG STDMETHODCALLTYPE Release(
	__RPC__in IWbemObjectSink * This)
{
	return _InterlockedDecrement(&((struct EventSink *)This)->m_lRef);
}

static HRESULT STDMETHODCALLTYPE Indicate(
	__RPC__in IWbemObjectSink * This,
	/* [in] */ long lObjectCount,
	/* [size_is][in] */ __RPC__in_ecount_full(lObjectCount) IWbemClassObject **apObjArray)
{
	EnterCriticalSection(&cs);
	if (hMonitorThread) EnumProcessId();
	LeaveCriticalSection(&cs);
	return WBEM_S_NO_ERROR;
}

static HRESULT STDMETHODCALLTYPE SetStatus(
	__RPC__in IWbemObjectSink * This,
	/* [in] */ long lFlags,
	/* [in] */ HRESULT hResult,
	/* [unique][in] */ __RPC__in_opt BSTR strParam,
	/* [unique][in] */ __RPC__in_opt IWbemClassObject *pObjParam)
{
	if (lFlags == WBEM_STATUS_COMPLETE)
		((struct EventSink *)This)->bDone = TRUE;
	return WBEM_S_NO_ERROR;
}

static IWbemObjectSinkVtbl SinkVtbl = {
	QueryInterface,
	AddRef,
	Release,
	Indicate,
	SetStatus
};

static DWORD WINAPI ProcessMonitor(HANDLE hObjectToSignal)
{
#if 1
	if (SUCCEEDED(CoInitializeEx(0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		IWbemLocator *pLoc = NULL;
		BSTR const bsRES = SysAllocString(OLESTR("ROOT/CIMV2"));
		if (bsRES && SUCCEEDED(CoCreateInstance(
			&CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			&IID_IWbemLocator,
			&pLoc
		)))
		{
			IWbemServices *pSvc = NULL;
			if (SUCCEEDED(pLoc->lpVtbl->ConnectServer(
				pLoc,
				bsRES,
				NULL,
				NULL,
				NULL,
				0,
				NULL,
				NULL,
				&pSvc
			)))
			{
				IUnsecuredApartment *pUnsecApp = NULL;
				if (SUCCEEDED(CoSetProxyBlanket(
					(void *)pSvc,                // Indicates the proxy to set
					RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
					RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
					NULL,                        // Server principal name
					RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
					RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
					NULL,                        // client identity
					EOAC_NONE                    // proxy capabilities
				))
#if 0
					&& SUCCEEDED(CoCreateInstance(
						&CLSID_UnsecuredApartment,
						NULL,
						CLSCTX_LOCAL_SERVER,
						&IID_IUnsecuredApartment,
						&pUnsecApp
					))
#endif
					)
				{
					struct EventSink esNew = { &SinkVtbl, 1, FALSE };
					struct EventSink esDel = { &SinkVtbl, 1, FALSE };
					BSTR const bsWQL = SysAllocString(OLESTR("WQL"));
					BSTR const bsNew = SysAllocString(OLESTR(
						"SELECT * FROM __InstanceCreationEvent WITHIN 1"
						" WHERE TargetInstance ISA 'Win32_Process'"
					));
					BSTR const bsDel = SysAllocString(OLESTR(
						"SELECT * FROM __InstanceDeletionEvent WITHIN 1"
						" WHERE TargetInstance ISA 'Win32_Process'"
					));
					if (bsWQL && bsNew && bsDel && SUCCEEDED(pSvc->lpVtbl->ExecNotificationQueryAsync(
						pSvc,
						bsWQL,
						bsNew,
						WBEM_FLAG_SEND_STATUS,
						NULL,
						(void *)&esNew
					)) && SUCCEEDED(pSvc->lpVtbl->ExecNotificationQueryAsync(
						pSvc,
						bsWQL,
						bsDel,
						WBEM_FLAG_SEND_STATUS,
						NULL,
						(void *)&esDel
					)))
					{
						SignalObjectAndWait(hObjectToSignal, GetCurrentProcess(), INFINITE, TRUE);
						pSvc->lpVtbl->CancelAsyncCall(pSvc, (void *)&esDel);
						pSvc->lpVtbl->CancelAsyncCall(pSvc, (void *)&esNew);
					}
					SysFreeString(bsDel);
					SysFreeString(bsNew);
					SysFreeString(bsWQL);
					if (pUnsecApp) pUnsecApp->lpVtbl->Release(pUnsecApp);
				}
				pSvc->lpVtbl->Release(pSvc);
			}
			pLoc->lpVtbl->Release(pLoc);
		}
		SysFreeString(bsRES);
		CoUninitialize();
	}
	PulseEvent(hObjectToSignal);
#else
	HQUERY hQuery;

	if (PdhOpenQueryW(NULL, 0, &hQuery) == ERROR_SUCCESS)
	{
		wchar_t szInstanceName[MAX_PATH];

		DWORD                       dwProcessId;
		PDH_COUNTER_PATH_ELEMENTS_W cpe;

		GetModuleFileNameW(GetModuleHandleW(NULL), szInstanceName, _countof(szInstanceName));
		PathRemoveExtensionW(szInstanceName);
		dwProcessId = GetCurrentProcessId();
		cpe.szMachineName    = NULL;
		cpe.szObjectName     = L"Process";
		cpe.szInstanceName   = PathFindFileNameW(szInstanceName);
		cpe.szParentInstance = NULL;
		cpe.dwInstanceIndex  = 0;
		cpe.szCounterName    = L"ID Process";
		do
		{
			wchar_t              szCounterPath[PDH_MAX_COUNTER_PATH];
			DWORD                cchCounterPathLength;
			HANDLE               hCounter;
			PDH_STATUS           Status;
			PDH_FMT_COUNTERVALUE fmtValue;
			DWORD                dwThreadId;
			DWORD                dwThreadIndex;
			wchar_t              szNumberString[11];

			cchCounterPathLength = _countof(szCounterPath);
			if (PdhMakeCounterPathW(&cpe, szCounterPath, &cchCounterPathLength, 0) != ERROR_SUCCESS)
				break;
			if (PdhAddCounterW(hQuery, szCounterPath, 0, &hCounter) != ERROR_SUCCESS)
				break;
			if ((Status = PdhCollectQueryData(hQuery)) == ERROR_SUCCESS)
				Status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, NULL, &fmtValue);
			PdhRemoveCounter(hCounter);
			if (Status != ERROR_SUCCESS)
				break;
			if ((DWORD)fmtValue.longValue != dwProcessId)
				continue;
			dwThreadId = GetCurrentThreadId();
			dwThreadIndex = 0;
			cpe.szObjectName     = L"Thread";
			cpe.szInstanceName   = szNumberString;
			cpe.szParentInstance = PathFindFileNameW(szInstanceName);
			cpe.szCounterName    = L"ID Thread";
			do
			{
				DWORD dwSpan;

				_ultow(dwThreadIndex, szNumberString, 10);
				cchCounterPathLength = _countof(szCounterPath);
				if (PdhMakeCounterPathW(&cpe, szCounterPath, &cchCounterPathLength, 0) != ERROR_SUCCESS)
					break;
				if (PdhAddCounterW(hQuery, szCounterPath, 0, &hCounter) != ERROR_SUCCESS)
					break;
				if ((Status = PdhCollectQueryData(hQuery)) == ERROR_SUCCESS)
					Status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG, NULL, &fmtValue);
				PdhRemoveCounter(hCounter);
				if (Status != ERROR_SUCCESS)
					break;
				if ((DWORD)fmtValue.longValue != dwThreadId)
					continue;
				cpe.szObjectName  = L"Processor";
				cpe.szCounterName = L"% Processor Time";
				cchCounterPathLength = _countof(szCounterPath);
				if (PdhMakeCounterPathW(&cpe, szCounterPath, &cchCounterPathLength, 0) != ERROR_SUCCESS)
					break;
				if (PdhAddCounterW(hQuery, szCounterPath, 0, &hCounter) != ERROR_SUCCESS)
					break;
				dwSpan = 500;
				while (hMonitorThread)
				{
					EnterCriticalSection(&cs);
					if (hMonitorThread)
					{
						if (dwSpan < 4000)
							Status = PdhCollectQueryData(hQuery);
						EnumProcessId();
						LeaveCriticalSection(&cs);
						Sleep(dwSpan);
						if (dwSpan >= 4000)
							continue;
						if (Status != ERROR_SUCCESS)
							continue;
						if (PdhCollectQueryData(hQuery) != ERROR_SUCCESS)
							continue;
						if (PdhGetFormattedCounterValue(hCounter, PDH_FMT_LONG | PDH_FMT_1000, NULL, &fmtValue) != ERROR_SUCCESS)
							continue;
						if (fmtValue.longValue >= 500)
							dwSpan *= 2;
					}
					else
					{
						LeaveCriticalSection(&cs);
						break;
					}
				}
				PdhRemoveCounter(hCounter);
				break;
			} while (++dwThreadIndex);
			break;
		} while (++cpe.dwInstanceIndex);
		PdhCloseQuery(hQuery);
	}
#endif
	hMonitorThread = NULL;
	return 0;
}

DWORD __stdcall FindProcessId(
	IN          BOOL   bIsRegex,
	IN          LPCSTR lpProcessName,
	IN          size_t nProcessNameLength,
	IN OPTIONAL LPCSTR lpModuleName,
	IN OPTIONAL LPCSTR lpCmdLineArg)
{
	static BOOL InProcessing = FALSE;
	DWORD       dwProcessId;
	wchar_t     lpWideCharStr[MAX_PATH];
	LPDWORD     lpdwProcessId;
#if USING_REGEX
	size_t      nModuleNameLength;
	LPSTR       lpBuffer;
#endif

	if (!lpProcessName && !lpModuleName && !lpCmdLineArg)
		return 0;
	if (!bInitialized || (BOOL)_InterlockedCompareExchange((long *)&InProcessing, TRUE, FALSE))
		return 0;
	dwProcessId = 0;
	if (!hMonitorThread && !EnumProcessId())
		goto FINALLY;
	if (!bIsRegex && lpModuleName && !MultiByteToWideChar(CP_THREAD_ACP, 0, lpModuleName, -1, lpWideCharStr, _countof(lpWideCharStr)))
		goto FINALLY;
	EnterCriticalSection(&cs);
	if (!bIsRegex)
	{
		LPCSTR lpBaseName = (void *)lpMonitorNames;
		for (lpdwProcessId = lpdwMonitorPIDs; lpdwProcessId != lpdwMonitorEndOfPIDs; lpdwProcessId++)
		{
			DWORD dwLength = *(LPDWORD)lpBaseName;
			lpBaseName += sizeof(DWORD);
			if (dwLength && (!lpProcessName || dwLength == nProcessNameLength && _mbsicmp(lpBaseName, lpProcessName) == 0))
			{
				if (ProcessInfoValidation(*lpdwProcessId, FALSE, lpModuleName ? lpWideCharStr : NULL, lpCmdLineArg))
				{
					dwProcessId = *lpdwProcessId;
					break;
				}
			}
			lpBaseName += dwLength + __alignof(DWORD)/* included \0 */ & -(signed)__alignof(DWORD);
		}
	}
#if USING_REGEX
	else if (lpBuffer = HeapAlloc(hHeap, 0,
		nProcessNameLength + (lpProcessName ? 3 : 0) +
		(nModuleNameLength = lpModuleName ? strlen(lpModuleName) : 0) + (lpModuleName ? 3 : 0)))
	{
		regex_t reProcessName;
		regex_t reModuleName;
		regex_t reCmdLineArg;
		LPSTR   dest;
		LPCSTR  src;

		dest = lpBuffer;
		if (lpProcessName)
		{
			src = lpProcessName;
			lpProcessName = dest;
			*(dest++) = '^';
			memcpy(dest, src, nProcessNameLength);
			dest += nProcessNameLength;
			*(((LPWORD)dest)++) = (BYTE)'$';
		}
		if (!lpProcessName || regcomp(&reProcessName, lpProcessName, REG_EXTENDED | REG_ICASE | REG_NOSUB) == 0)
		{
			if (lpModuleName)
			{
				src = lpModuleName;
				lpModuleName = dest;
				*(dest++) = '^';
				memcpy(dest, src, nModuleNameLength);
				dest += nModuleNameLength;
				*(LPWORD)dest = (BYTE)'$';
			}
			if (!lpModuleName || regcomp(&reModuleName, lpModuleName, REG_EXTENDED | REG_ICASE | REG_NOSUB) == 0)
			{
				if (!lpCmdLineArg || regcomp(&reCmdLineArg, lpCmdLineArg, REG_EXTENDED | REG_NOSUB) == 0)
				{
					LPCSTR lpBaseName = (void *)lpMonitorNames;
					for (lpdwProcessId = lpdwMonitorPIDs; lpdwProcessId != lpdwMonitorEndOfPIDs; lpdwProcessId++)
					{
						DWORD dwLength = *(LPDWORD)lpBaseName;
						lpBaseName += sizeof(DWORD);
						if (dwLength && (!lpProcessName || regexec(&reProcessName, lpBaseName, 0, NULL, 0) == 0))
						{
							if (ProcessInfoValidation(
								*lpdwProcessId,
								TRUE,
								lpModuleName ? &reModuleName : NULL,
								lpCmdLineArg ? &reCmdLineArg : NULL
							))
							{
								dwProcessId = *lpdwProcessId;
								break;
							}
						}
						lpBaseName += dwLength + __alignof(DWORD)/* included \0 */ & -(signed)__alignof(DWORD);
					}
					if (lpCmdLineArg)
						regfree(&reCmdLineArg);
				}
				if (lpModuleName)
					regfree(&reModuleName);
			}
			if (lpProcessName)
				regfree(&reProcessName);
		}
		HeapFree(hHeap, 0, lpBuffer);
	}
#endif
	LeaveCriticalSection(&cs);
	if (!dwProcessId)
	{
		if (!hMonitorThread)
		{
			DWORD dwThreadId;

			hMonitorThread = CreateThread(NULL, 0, ProcessMonitor, hObjectToSignal, 0, &dwThreadId);
			WaitForSingleObject(hObjectToSignal, INFINITE);
		}
	}
	else
	{
		StopProcessMonitor();
	}
FINALLY:
	InProcessing = FALSE;
	return dwProcessId;
}

DWORD_DWORD __fastcall TProcessAddForm_ReLoadBtnClick_GetFirstModule(
	vector_PROCESSENTRY32A *const processVec,
	MODULEENTRY32A         *const lpME,
	PROCESSENTRY32A        *const VIt)
{
	WCHAR lpFilename[MAX_PATH];
	DWORD        dwLength = 0;
	HANDLE const hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, VIt->th32ProcessID);
	if (hProcess)
	{
		dwLength = GetModuleFileNameExW(hProcess, NULL, lpFilename, _countof(lpFilename));
		CloseHandle(hProcess);
	}
	if (!dwLength)
		memmove(VIt, VIt + 1, (size_t)--vector_end(processVec) - (size_t)VIt);
	else if (dwLength = WideCharToMultiByte(CP_ACP, 0, lpFilename, dwLength + 1, lpME->szExePath, sizeof(lpME->szExePath), NULL, NULL))
		dwLength--;
	return (DWORD_DWORD) { dwLength, (DWORD)vector_end(processVec) };
}

DWORD_DWORD __fastcall TProcessAddForm_ReLoadBtnClick_ProcessDGrid_Repaint(
	vector_PROCESSENTRY32A *const processVec,
	TWinControl      const *const ProcessAddForm)
{
	void *const lpProcessDGrid = (LPBYTE)ProcessAddForm + 0x0304;
	int   const pid = MainForm->ssgCtrl.processCtrl.entry.th32ProcessID;
	if (pid > 0) for (ptrdiff_t i = vector_size(processVec); --i >= 0; )
	{
		if (vector_at(processVec, i).th32ProcessID == pid)
		{
			TDrawGrid_SetRow(*(TDrawGrid **)lpProcessDGrid, vector_size(processVec) - i);
			break;
		}
	}
	return (DWORD_DWORD) { (DWORD)ProcessAddForm, (DWORD)ProcessAddForm->VTable };
}
