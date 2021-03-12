#include <windows.h>
#include "TSSGCtrl.h"

extern const BOOL EnableParserFix;

__declspec(naked) void __cdecl Caller_StrToProcessAccessElementVecWithoutRel()
{
	__asm
	{
		cmp     EnableParserFix, FALSE
		je      L1
		mov     ecx,  [esp    ]
		mov     edx,  [esp + 4]
		movups  xmm0, [esp + 8]
		movups  xmm1, [esp + 8 + 0x10]
		push    edx
		push    offset L2// ReturnAddress
		movups  xmmword ptr [esp + 8       ], xmm0
		movups  xmmword ptr [esp + 8 + 0x10], xmm1
		mov     dword   ptr [esp + 8 + 0x20], 0
		mov     dword   ptr [esp + 8 + 0x24], ecx
	L1:
		jmp     TSSGCtrl_StrToProcessAccessElementVec
		ud2

		align   16
	L2:// discard extra stack
		add     esp, 8
		jmp     dword ptr [esp + 0x20]
		ud2
	}
}
