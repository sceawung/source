#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) unsigned long __cdecl TSSBitList_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC         (ebp + 0x0C)
		#define SSGS          ebx
		#define Val          (ebp - 0x90)

		mov     ecx, dword ptr [Val]
		mov     eax, dword ptr [SSGC]
		push    ecx
		push    SSGS
		push    eax
		call    TSSGCtrl_CheckFunnel
		add     esp, 12
		mov     eax, reNO_ERROR
		ret

		#undef Val
		#undef SSGS
		#undef SSGC
	}
}

