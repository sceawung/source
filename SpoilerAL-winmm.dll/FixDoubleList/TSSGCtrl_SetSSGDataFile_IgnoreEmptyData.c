#include <windows.h>

__declspec(naked) void __cdecl TSSGCtrl_SetSSGDataFile_IgnoreEmptyData()
{
	static const DWORD X004F0A51 = 0x004F0A51;

	__asm
	{
		mov     eax, dword ptr [esp + 8]
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax    ]
		cmp     eax, ecx
		je      L1
		push    ebp
		mov     ebp, esp
		sub     esp, 1152
		jmp     dword ptr [X004F0A51]

	L1:
		rep ret
	}
}
