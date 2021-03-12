#if !defined(__BORLANDC__) || defined(__cplusplus)

#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#ifdef __BORLANDC__
#include "MainUnit.h"
extern TMainForm *MainForm;
#else
#include <commctrl.h>
#include "xx.h"
#include "ToolTip\ToolTip.h"
#include "TWinControl.h"
#include "TMainForm.h"
#endif

volatile DWORD dwErrorMessageId = ERROR_SUCCESS;

void __cdecl ShowErrorMessage()
{
#ifdef __BORLANDC__
	#define hMainFormHandle    MainForm->Handle
	#define lpApplicationTitle Application->Title.c_str()
	LPCSTR lpNotDefinedError = "Not Defined Error!";
#else
	#define hMainFormHandle    TWinControl_GetHandle(MainForm)
	#define lpApplicationTitle (LPCSTR)0x006020D8
	#define lpNotDefinedError  (LPCSTR)0x00602DE0
#endif

	char   lpBuffer[0x0200];
	LPCSTR lpText;
	DWORD  dwError;

	dwError = dwErrorMessageId;
	dwErrorMessageId = ERROR_SUCCESS;
	lpText = dwError != ERROR_SUCCESS && FormatMessageA(
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		0,
		lpBuffer,
		_countof(lpBuffer),
		NULL) ? lpBuffer : lpNotDefinedError;
#if USE_TOOLTIP
	ShowToolTip(lpText, (HICON)TTI_ERROR);
#else
	MessageBoxA(hMainFormHandle, lpText, lpApplicationTitle, MB_ICONERROR);
#endif

#ifndef __BORLANDC__
	#undef lpNotDefinedError
#endif
	#undef lpApplicationTitle
	#undef hMainFormHandle
}

__declspec(naked) HANDLE __stdcall OpenProcessWithSaveError(
	IN DWORD dwDesiredAccess,
	IN BOOL  bInheritHandle,
	IN DWORD dwProcessId)
{
	__asm
	{
		cmp     dword ptr [dwErrorMessageId], ERROR_SUCCESS
		jne     L2
		mov     edx, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp +  8]
		mov     eax, dword ptr [esp +  4]
		push    edx
		push    ecx
		push    eax
		call    OpenProcess
		test    eax, eax
		jnz     L1
		call    GetLastError
		mov     dword ptr [dwErrorMessageId], eax
		xor     eax, eax
	L1:
		ret     12
	L2:
		jmp     OpenProcess
	}
}

__declspec(naked) BOOL __stdcall WriteProcessMemoryWithSaveError(
	IN           HANDLE  hProcess,
	IN           LPVOID  lpBaseAddress,
	IN           LPCVOID lpBuffer,
	IN           SIZE_T  nSize,
	OUT OPTIONAL SIZE_T  *lpNumberOfBytesWritten)
{
	__asm
	{
		cmp     dword ptr [dwErrorMessageId], ERROR_SUCCESS
		jne     L2
		mov     ecx, dword ptr [esp + 20]
		mov     eax, dword ptr [esp + 16]
		push    ecx
		push    eax
		mov     edx, dword ptr [esp + 20]
		mov     ecx, dword ptr [esp + 16]
		mov     eax, dword ptr [esp + 12]
		push    edx
		push    ecx
		push    eax
		call    WriteProcessMemory
		test    eax, eax
		jnz     L1
		call    GetLastError
		mov     dword ptr [dwErrorMessageId], eax
		xor     eax, eax
	L1:
		ret     20
	L2:
		jmp     WriteProcessMemory
	}
}

#ifndef __BORLANDC__
__declspec(naked) void __cdecl TSSGActionListner_OnSubjectDisabled_SetErrorMessage()
{
	__asm
	{
		mov     dword ptr [dwErrorMessageId], ERROR_DS_ILLEGAL_MOD_OPERATION
		jmp     __InitExceptBlockLDTC
	}
}
#endif

#endif	// #if !defined(__BORLANDC__) || defined(__cplusplus)

