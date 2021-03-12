#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"

__declspec(naked) void __cdecl TSSGCtrl_StrToProcessAccessElementVec_AppendNoEscapedChar()
{
#if CODEPAGE_SUPPORT
	__asm
	{
		movzx   eax, byte ptr [esi]
		push    005072DCH
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     dl, byte ptr [esi]
		inc     esi
		jmp     vector_byte_push_back

		align   16
	L1:
		rep ret
	}
#else
	static const DWORD X005072DC = 0x005072DC;

	__asm
	{
		mov     al, byte ptr [esi]
		mov     ecx, dword ptr [ebp - 618H]
		mov     dl, al
		sub     al, 81H
		cmp     al, 9FH - 81H + 1
		jb      L1
		sub     al, 0E0H - 81H
		cmp     al, 0FCH - 0E0H + 1
		jae     L2
	L1:
		push    005072DCH
		inc     esi
		jmp     vector_byte_push_back

		align   16
	L2:
		jmp     dword ptr [X005072DC]
	}
#endif
}
