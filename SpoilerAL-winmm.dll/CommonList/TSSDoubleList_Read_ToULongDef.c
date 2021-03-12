#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSDoubleList_Read_ToULongDef()
{
	static const DWORD JumpTable[] = {
		0x004C5209,
		0x004C5512,
		0x004C7A1E,
	};

	__asm
	{
		#define Default (esp + 8)

		lea     edx, [Default]
		movzx   ecx, cl
		push    ecx
		push    0
		push    edx
		push    eax
		call    strtoul
		pop     edx
		add     esp, 8
		pop     ecx
		cmp     edx, dword ptr [Default]
		sbb     edx, edx
		jz      L1
		rep ret

		align   16
	L1:
		add     esp, 12
		mov     eax, reINDEX_ERROR
		jmp     dword ptr [offset JumpTable + ecx]
		ud2

		#undef Default
	}
}
