#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) unsigned long __cdecl TSSDoubleList_WriteOne_CheckFunnel()
{
	__asm
	{
		#define SSGC          (ebp + 0x0C)
		#define SSGS          (ebp + 0x08)
		#define Index         (ebp - 0x0160)

		mov     edx, dword ptr [SSGS]
		mov     ecx, dword ptr [SSGC]
		push    dword ptr [Index]
		push    edx
		push    ecx
		call    TSSGCtrl_CheckFunnel
		add     esp, 12
		mov     eax, reNO_ERROR
		ret

		#undef Index
		#undef SSGS
		#undef SSGC
	}
}

