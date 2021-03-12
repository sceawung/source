#include <windows.h>
#include "TSSArg.h"
#include "TSSGCtrl.h"

__declspec(naked) struct
{
	bool CheckFunnel;
	DWORD reNO_ERROR;
} __cdecl TSSDoubleToggle_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC (ebp + 0x0C)
		#define SSGS (ebp + 0x08)
		#define Vec  (ebp - 0x3C)

		mov     edx, dword ptr [SSGS]
		mov     ecx, dword ptr [SSGC]
		push    [Vec]bcb6_std_bvector._M_start._M_p
		push    edx
		push    ecx
		call    TSSGCtrl_CheckFunnel
		add     esp, 12
		mov     edx, reNO_ERROR
		ret

		#undef Vec
		#undef SSGS
		#undef SSGC
	}
}

