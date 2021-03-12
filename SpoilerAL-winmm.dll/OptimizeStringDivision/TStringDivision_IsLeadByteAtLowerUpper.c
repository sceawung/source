#include <windows.h>
#include "intrinsic.h"

__declspec(naked) void __cdecl TStringDivision_IsLeadByteAtLowerUpper()
{
	__asm
	{
#if CODEPAGE_SUPPORT
		movzx   edx, byte ptr [ebx]
		pop     eax
		push    edx
		push    CP_THREAD_ACP
		push    eax
		jmp     IsDBCSLeadByteEx
#else
		mov     dl, byte ptr [ebx]
		xor     eax, eax
		sub     dl, 81H
		cmp     dl, 9FH - 81H + 1
		jb      L1
		sub     dl, 0E0H - 81H
		cmp     dl, 0FCH - 0E0H + 1
		jae     L2
	L1:
		inc     eax
	L2:
		rep ret
#endif
	}
}
