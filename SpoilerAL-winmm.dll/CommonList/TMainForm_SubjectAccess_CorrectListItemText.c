#include <windows.h>
#include "TStringDivision.h"

__declspec(naked) void __cdecl TMainForm_SubjectAccess_CorrectListItemText()
{
	__asm
	{
		mov     cl, byte ptr [esp + 4]
		push    005E0D50H
		cmp     cl, '+'
		je      L1
		rep ret

		align   16
	L1:
		mov     cl, byte ptr [edx]
		inc     edx
		test    cl, cl
		jz      L2
		cmp     cl, '='
		jne     L1
		mov     ecx, edx
		call    TrimLeftSpace
		mov     edx, eax
		lea     eax, [ebp - 176]
		ret

		align   16
	L2:
		dec     edx
		ret
	}
}
