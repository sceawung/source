#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

extern const DWORD F00415A60;

#ifdef FORMAT_NOT_IMPLEMENTED
__declspec(naked) void __cdecl TMainForm_DrawTreeCell_CorrectDrawText()
{
	__asm
	{
		mov     al, byte ptr [esp + 20]
		mov     ecx, dword ptr [esp + 12]
		cmp     al, '+'
		je      L1
		jmp     dword ptr [F00415A60]

		align   16
	L1:
		mov     edx, dword ptr [ecx]

		align   16
	L2:
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jz      L3
		cmp     al, '='
		jne     L2
		mov     ecx, edx
		jmp     L4

		align   16
	L3:
		dec     edx
		jmp     L5

		align   16
	L4:
		call    TrimLeftSpace
		mov     ecx, dword ptr [esp + 12]
		mov     edx, eax
	L5:
		mov     eax, dword ptr [ecx + 4]
		sub     esp, 24
		mov     ecx, esp
		push    eax
		call    string_ctor_assign_range
		mov     ecx, dword ptr [esp + 24 + 4]
		mov     edx, dword ptr [esp + 24 + 8]
		push    esp
		push    edx
		push    ecx
		call    dword ptr [F00415A60]
		add     esp, 12
		mov     ecx, esp
		call    string_dtor
		add     esp, 24
		ret
	}
}
#endif
