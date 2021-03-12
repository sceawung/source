#include <windows.h>
#include "TMainForm.h"
#include "ToolTip/ToolTip.h"
#include "ApplicationMessage.h"
#include "SSGSubjectProperty.h"

#define ALLOCATE_SUPPORT 1

typedef struct {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;

typedef struct {
	size_t SizeOfReplace;
	LPBYTE Replace;
	LPSTR  Source;
	size_t NumberOfMarkup;
	LPVOID Markup;
	size_t NumberOfPostfix;
	LPVOID Postfix;
	size_t Next;
} CODECACHE, *PCODECACHE;

extern HANDLE             hHeap;
extern HANDLE             hPrivateHeap;
extern size_t             nNumberOfProcessMemory;
extern PROCESSMEMORYBLOCK *lpProcessMemory;
extern FILETIME           ftProcessCreationTime;
extern size_t             nNumberOfCodeCache;
extern CODECACHE          *lpCodeCache;
extern TSSGSubject        dummySSGS;
extern void __cdecl ClearGuideBuffer();

void __cdecl OnSSGCtrlCleared(IN TSSGCtrl *SSGCtrl)
{
	ClearSubjectProperty();

#if ALLOCATE_SUPPORT
	if (lpProcessMemory)
	{
		if (nNumberOfProcessMemory)
		{
			size_t i = nNumberOfProcessMemory;
			do
				if (!lpProcessMemory[--i].Protect && lpProcessMemory[i].Address)
					HeapFree(hPrivateHeap, HEAP_NO_SERIALIZE, lpProcessMemory[i].Address);
			while (i);
			nNumberOfProcessMemory = 0;
		}
		HeapFree(hHeap, 0, lpProcessMemory);
		lpProcessMemory = NULL;
	}
#endif

	if (lpCodeCache)
	{
		if (nNumberOfCodeCache)
		{
			CODECACHE *lpCache;

			lpCache = lpCodeCache + nNumberOfCodeCache;
			nNumberOfCodeCache = 0;
			do
			{
				lpCache--;
				HeapFree(hHeap, 0, lpCache->Postfix);
				HeapFree(hHeap, 0, lpCache->Markup);
				HeapFree(hHeap, 0, lpCache->Source);
				if (lpCache->Replace)
					HeapFree(hHeap, 0, lpCache->Replace);
			} while (lpCache != lpCodeCache);
		}
		HeapFree(hHeap, 0, lpCodeCache);
		lpCodeCache = NULL;
	}

	TProcessCtrl_Clear(&SSGCtrl->processCtrl);

	{
		MSG        msg;
		HWND const hWnd = TWinControl_GetHandle(MainForm);
		while (PeekMessageA(&msg, hWnd, WM_DRAW_GUIDE_BUFFER, WM_DRAW_GUIDE_BUFFER, PM_REMOVE))
			DispatchMessageA(&msg);// quasi-DoEvent
		ClearGuideBuffer();// shrink guiding buffer
		HeapCompact(hHeap, 0);// Urge deallocation.
		if (hPrivateHeap)
		{
			LPSTR lpBuffer;
			if (HeapDestroy(hPrivateHeap))
				hPrivateHeap = NULL;
			else if (FormatMessageA(
				FORMAT_MESSAGE_MAX_WIDTH_MASK * !USE_TOOLTIP |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				0,
				(LPSTR)&lpBuffer,
				sizeof(double),
				NULL))
			{
#if USE_TOOLTIP
				ShowToolTip(lpBuffer, (HICON)TTI_ERROR);
#else
				if (TMainForm_GetUserMode(MainForm) != 1)
					TMainForm_Guide(lpBuffer, 0);
#endif
				LocalFree(lpBuffer);
			}

		}
	}
}
