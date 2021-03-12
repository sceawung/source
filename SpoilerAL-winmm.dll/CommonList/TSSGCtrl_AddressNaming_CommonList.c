#include <stdlib.h>
#include "bcb6_std_string.h"

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_CommonList()
{
	__asm
	{
		#define Src     (esp + 4)
		#define Default (esp + 8)

		mov     ecx, [Src]
		lea     eax, [Default]
		push    0
		push    eax
		push    [ecx]bcb6_std_string._M_start
		call    strtoul
		pop     edx
		add     esp, 8
		cmp     edx, [Default]
		sbb     edx, edx
		ret     8
		
		#undef Default
		#undef Src
	}
}
