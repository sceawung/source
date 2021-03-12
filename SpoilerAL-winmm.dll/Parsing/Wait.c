#include <windows.h>
#include "intrinsic.h"

#ifndef __BORLANDC__
#define __msfastcall __fastcall
#endif

#ifndef _M_IX86
void __msfastcall Wait(DWORD dwMilliseconds)
{
	ULONGLONG qwNow, qwEnd;
	MSG       msg;

	if (!dwMilliseconds)
		return;
	if (dwMilliseconds != INFINITE)
	{
		GetSystemTimeAsFileTime((LPFILETIME)&qwNow);
		qwEnd = qwNow + __emulu(dwMilliseconds, 10000);
	}
	while (MsgWaitForMultipleObjects(0, NULL, FALSE, dwMilliseconds, QS_ALLEVENTS) != WAIT_TIMEOUT)
	{
		unsigned int remainder;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				goto QUIT;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (dwMilliseconds == INFINITE)
			continue;
		GetSystemTimeAsFileTime((LPFILETIME)&qwNow);
		if (qwEnd <= qwNow || !(dwMilliseconds = _udiv64(qwEnd - qwNow, 10000, &remainder)))
			break;
	}
	return;

QUIT:
	PostQuitMessage(msg.wParam);
}
#else
__declspec(naked) void __msfastcall Wait(DWORD dwMilliseconds)
{
	__asm
	{
		test    ecx, ecx
		jz      L7
		push    ebx
		push    esi
		push    edi
		mov     ebx, ecx
		sub     esp, 28
		cmp     ecx, INFINITE
		je      L4
		push    esp
		call    GetSystemTimeAsFileTime
		mov     esi, dword ptr [esp]
		mov     edi, dword ptr [esp + 4]
		mov     eax, ebx
		mov     ecx, 10000
		mul     ecx
		add     esi, eax
		adc     edi, edx
		jmp     L4

		align   16
	L1:
		mov     eax, dword ptr [esp + 4]
		mov     ecx, dword ptr [esp + 8]
		cmp     eax, WM_QUIT
		je      L5
		push    esp
		call    TranslateMessage
		push    esp
		call    DispatchMessage
	L2:
		mov     eax, esp
		push    PM_REMOVE
		push    0
		push    0
		push    0
		push    eax
		call    PeekMessage
		test    eax, eax
		jnz     L1
		cmp     ebx, INFINITE
		je      L4
		push    esp
		call    GetSystemTimeAsFileTime
		mov     ecx, dword ptr [esp]
		mov     ebx, dword ptr [esp + 4]
		mov     eax, esi
		mov     edx, edi
		sub     eax, ecx
		mov     ecx, 10000
		sbb     edx, ebx
		ja      L3
		jb      L6
		test    eax, eax
		jz      L6
	L3:
		div     ecx
		test    eax, eax
		jz      L6
		mov     ebx, eax
	L4:
		push    QS_ALLEVENTS
		push    ebx
		push    FALSE
		push    0
		push    0
		call    MsgWaitForMultipleObjects
		cmp     eax, WAIT_TIMEOUT
		jne     L2
		jmp     L6

		align   16
	L5:
		push    ecx
		call    PostQuitMessage
	L6:
		mov     edi, dword ptr [esp + 28]
		add     esp, 32
		pop     esi
		pop     ebx
	L7:
		rep ret
	}
}
#endif
