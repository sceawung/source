#include "xx.h"
#include "TSSString.h"

#define MakeSubjectClass_SwitchSubjectTypeTable 0x004EC41F

__declspec(naked) TSSString * __cdecl new_TSSString()
{
	__asm
	{
		push    ebp
		mov     eax, 006354ACH
		mov     ebp, esp
		sub     esp, 136
		push    ebx
		push    esi
		mov     ecx, dword ptr ds:[MakeSubjectClass_SwitchSubjectTypeTable + stSTRING * 4]
		lea     ebx, [ebp - 124]
		push    edi
		push    ecx
		jmp     __InitExceptBlockLDTC
	}
}

