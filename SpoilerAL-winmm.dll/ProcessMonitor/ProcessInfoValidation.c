#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
/*
https://docs.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_unicode_string
 * Length         The length, in bytes, of the string stored in Buffer.
 * MaximumLength  The length, in bytes, of Buffer.
 * Buffer         Pointer to a buffer used to contain a string of wide characters.
 If the string is null-terminated, Length does not include the trailing null character.

https://docs.microsoft.com/en-us/windows/win32/api/subauth/ns-subauth-unicode_string
 Note that the strings returned by the various LSA functions might not be null-terminated.
 */
#ifndef __BORLANDC__
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")
#define USING_REGEX 1
#else
#pragma warn -8060
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define ProcessBasicInformation 0

typedef /*__success(return >= 0) */LONG NTSTATUS;

typedef int PROCESSINFOCLASS;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {
	BYTE                                   Reserved1[2];
	BYTE                                   BeingDebugged;
	BYTE                                   Reserved2[1];
	PVOID                                  Reserved3[2];
	PPEB_LDR_DATA                          Ldr;
	PVOID/*PRTL_USER_PROCESS_PARAMETERS*/  ProcessParameters;
	BYTE                                   Reserved4[104];
	PVOID                                  Reserved5[52];
	PVOID/*PPS_POST_PROCESS_INIT_ROUTINE*/ PostProcessInitRoutine;
	BYTE                                   Reserved6[128];
	PVOID                                  Reserved7[1];
	ULONG                                  SessionId;
} PEB, *PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID     Reserved1;
	PPEB      PebBaseAddress;
	PVOID     Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID     Reserved3;
} PROCESS_BASIC_INFORMATION;

typedef NTSTATUS(WINAPI *PFN_NTQUERYINFORMATIONPROCESS)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
#endif

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <mbstring.h>

#if USING_REGEX
#include <regex.h>
#endif

#include "GetFileTitlePointer.h"
#include "TMainForm.h"

extern HANDLE hHeap;

BOOL __stdcall ProcessInfoValidation(
	IN DWORD   dwProcessId,
	IN BOOL    bIsRegex,
	IN LPCVOID lpModuleName,
	IN LPCVOID lpCmdLineArg)
{
	BOOL   bFound, iError;
	HANDLE hProcess;

#ifdef __BORLANDC__
	PFN_NTQUERYINFORMATIONPROCESS NtQueryInformationProcess =
		(PFN_NTQUERYINFORMATIONPROCESS)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	if (NtQueryInformationProcess == NULL)
		return FALSE;
#endif

	if (bFound = !(iError = !!lpCmdLineArg) && !lpModuleName)
		return TRUE;
	else if (hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId))
	{
		do	/* do { ... } while (0); */
		{
			PROCESS_BASIC_INFORMATION   pbi;
			struct _PEB                 peb;
			RTL_USER_PROCESS_PARAMETERS upp;
			PLIST_ENTRY                 Flink, EndOfFlink;

			if (!NT_SUCCESS(NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL)))
				break;
			if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL))
				break;
			if (lpCmdLineArg)
			{
				LPSTR cmd;
				if (ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), NULL)
					&& (cmd = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, upp.CommandLine.MaximumLength << 1)))
				{
					if (ReadProcessMemory(
						hProcess,
						upp.CommandLine.Buffer,
						&cmd[upp.CommandLine.MaximumLength],
						upp.CommandLine.Length,
						NULL
					) && WideCharToMultiByte(CP_THREAD_ACP, 0, (LPCWCH)&cmd[upp.CommandLine.MaximumLength], upp.CommandLine.Length >> 1,
											 cmd, upp.CommandLine.MaximumLength, NULL, NULL))
					{
						if (TMainForm_GetUserMode(MainForm) >= 3 &&
							TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl))
							TMainForm_Guide(cmd, 0);
						iError = bIsRegex ?
#if USING_REGEX
							regexec(lpCmdLineArg, cmd, 0, NULL, 0)
#else
							TRUE
#endif
							: !_mbsstr(_mbslwr(cmd), lpCmdLineArg);
					}
					HeapFree(hHeap, 0, cmd);
				}
				else
					break;
				if (iError) break;
			}
			if (bFound = !lpModuleName)
				break;
			if (!ReadProcessMemory(hProcess, EndOfFlink = &peb.Ldr->InMemoryOrderModuleList, &Flink, sizeof(Flink), NULL))
				break;
			do
			{
				LDR_DATA_TABLE_ENTRY LdrData;

				if (!ReadProcessMemory(
					hProcess,
					Flink,
					&LdrData.InMemoryOrderLinks,
					offsetof(LDR_DATA_TABLE_ENTRY, FullDllName) + sizeof(LdrData.FullDllName) - offsetof(LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks),
					NULL))
					break;
				if (LdrData.FullDllName.Length)
				{
					wchar_t lpBuffer[MAX_PATH];
					PWSTR   lpFullDllName;
					LPCWSTR lpFileTitle;
					char    lpMultiByteStr[MAX_PATH];
#if USING_REGEX
					BOOL    bUsedDefaultChar;
#endif

					if (LdrData.FullDllName.Length >> 1 < _countof(lpBuffer))
						lpFullDllName = lpBuffer;
					else if (!(lpFullDllName = (PWSTR)HeapAlloc(hHeap, 0, LdrData.FullDllName.MaximumLength)))
						break;
					if (!ReadProcessMemory(hProcess, LdrData.FullDllName.Buffer, lpFullDllName, LdrData.FullDllName.Length, NULL))
						break;
					lpFullDllName[LdrData.FullDllName.Length >> 1] = L'\0';
					lpFileTitle = GetFileTitlePointerW(lpFullDllName);
					bFound = !bIsRegex ?
						_wcsicmp(lpModuleName, lpFileTitle) == 0 :
#if USING_REGEX
						WideCharToMultiByte(CP_THREAD_ACP, 0, lpFileTitle, -1, lpMultiByteStr, _countof(lpMultiByteStr), NULL, &bUsedDefaultChar) &&
						!bUsedDefaultChar &&
						regexec(lpModuleName, lpMultiByteStr, 0, NULL, 0) == 0;
#else
						FALSE;
#endif
					if (lpFullDllName != lpBuffer)
						HeapFree(hHeap, 0, lpFullDllName);
					if (bFound)
						break;
				}
				Flink = LdrData.InMemoryOrderLinks.Flink;
			} while (Flink != EndOfFlink);
		} while (0);
		CloseHandle(hProcess);
	}
	return bFound;
}
