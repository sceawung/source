#include "TSSGCtrl.h"
#include "TSSGAttributeElement.h"

#if 0
__declspec(naked) void __cdecl TSSBundleList_Write_ErrorSkip()
{
	__asm
	{
		#define ReturnAddress 004BF89EH
		#define SSGC          (ebp + 12)
		#define this          esi

		mov     dword ptr [esp], offset L1
		jmp     dword ptr [TSSGCtrl_OneWrite]

	L1:
		push    ReturnAddress
		test    al, al
		jnz     L2
		mov     ecx, dword ptr [SSGC]
		push    atON_ERROR
		push    this
		push    ecx
		call    dword ptr [TSSGCtrl_GetAttribute]
		add     esp, 12
		test    eax, eax
		setnz   al
	L2:
		rep ret

		#undef ReturnAddress
		#undef SSGC
		#undef this
	}
}
#endif
