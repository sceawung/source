#include "TMainForm.h"
#include "TSSDir.h"

__declspec(naked) void __cdecl TMainForm_SubjectAccess_FixDirSameChildren()
{
	__asm
	{
		#define this          ebx
		#define SelectS       edi
		#define ArgType       eax
		#define ssgCtrl_atDIR 8

		cmp     ArgType, ssgCtrl_atDIR
		jne     L1
		push    SelectS
		call    dword ptr [TSSDir_IsSameChildren]
		test    al, al
		lea     eax, [this + TMainForm.ssgCtrl]
		pop     ecx
		jz      L1
		push    SelectS
		push    eax
		call    dword ptr [TSSGCtrl_IsChildRWProhibit]
		add     esp, 8
		test    al, al
		jz      L2
	L1:
		jmp     dword ptr [TMainForm_ChangeSubjectPanel]

	L2:
		rep ret

		#undef this
		#undef SelectS
		#undef ArgType
		#undef ssgCtrl_atDIR
	}
}
