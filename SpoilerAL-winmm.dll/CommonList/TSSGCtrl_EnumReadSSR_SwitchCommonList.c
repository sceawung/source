#include <windows.h>

#if 0
__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSR_SwitchCommonList()
{
	static const DWORD X005009AF = 0x005009AF;
	static const DWORD X00500C13 = 0x00500C13;

	__asm
	{
		cmp     al, '+' - 'R'
		je      L1
		cmp     al, '*' - 'R'
		je      L1
		jmp     dword ptr [X00500C13]
		ud2

		align   16
	L1:
		push    -1
		push    0
		jmp     dword ptr [X005009AF]
		ud2
	}
}
#endif
