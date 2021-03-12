#include <windows.h>
#include "TMainForm.h"

void __declspec(naked) __cdecl TSearchForm_AddressLBoxDblClick_SubjectAccess(TMainForm* mainForm, TSSGSubject* SelectS)
{
	extern const DWORD _TMainForm_SubjectAccess;
	static const char szEmpty[] = "";

	__asm
	{
		mov     eax, dword ptr [esp + 4]
		push    TRUE
#if DRAWTREE_CONVENTION
		push    0
#else
		push    LONG_MIN
#endif
		push    0
		push    0
		push    eax
		call    TMainForm_DrawTree
#if !DRAWTREE_CONVENTION
		add     esp, 20
#endif
		mov     eax, dword ptr [esp + 8]
		cmp     [eax]TSSGSubject.type, stDIR
		je      NO_ACCESS

		mov     eax, dword ptr [esp + 4]
		cmp     [eax]TMainForm.userMode, 4
		jne     ACCESS

		push    GUIDE_IS_CLEAR | GUIDE_IS_NOT_LINE
		push    offset szEmpty
		call    TMainForm_Guide
	ACCESS:
		jmp     dword ptr [_TMainForm_SubjectAccess]

	NO_ACCESS:
		rep ret
	}
}
