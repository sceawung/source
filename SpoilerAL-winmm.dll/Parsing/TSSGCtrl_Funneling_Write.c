#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSGCtrl_Funneling_Write()
{
	__asm
	{
		#define SHandle (ebp - size PVOID)

		mov     ecx, dword ptr [ecx + size bcb6_std_string]
		call    TSSGCtrl_IsRemoteProcess
		mov     edx, dword ptr [SHandle]
		test    al, al
		jnz     L1
		call    GetCurrentProcess
		mov     edx, eax
	L1:
		rep ret

		#undef SHandle
	}
}
