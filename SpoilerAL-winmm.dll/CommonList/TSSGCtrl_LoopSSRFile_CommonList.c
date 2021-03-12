#include <windows.h>
#include "TStringDivision.h"

extern const DWORD _STL_string_M_append_dispatch;

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_CommonList()
{
	__asm
	{
		#define tmpS    (esp +  4)
		#define begin   (esp +  8)
		#define end     (esp + 12)
		#define unknown (esp + 16)
		#define prefix  (esp + 20)

		mov     al, byte ptr [prefix]
		mov     ecx, dword ptr [begin]
		cmp     al, '+'
		je      L1
		cmp     al, '*'
		je      L2
		jmp     dword ptr [_STL_string_M_append_dispatch]

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jz      L3
		cmp     al, '='
		jne     L1
		call    TrimLeftSpace
		mov     dword ptr [begin], eax
		jmp     dword ptr [_STL_string_M_append_dispatch]

		align   16
	L2:
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jz      L3
		cmp     al, '='
		jne     L2
		mov     edx, ecx
		jmp     L4

		align   16
	L3:
		lea     eax, [ecx - 1]
		jmp     L5

		align   16
	L4:
		mov     ecx, dword ptr [begin]
		dec     edx
		call    TrimRightSpace
	L5:
		mov     dword ptr [end], eax
		jmp     dword ptr [_STL_string_M_append_dispatch]
	}
}
