#include <windows.h>
#include "TControl.h"
#include "TMainForm.h"

extern const DWORD _TFont_SetName;

__declspec(naked) void __cdecl TMainForm_LoadSetting_NoteREdit_Font_setName()
{
	__asm// Borland register
	{
		#define this edi

		push    eax
		push    edx
		mov     eax, [this]TMainForm.MultiLBox
		mov     eax, [eax]TControl.FFont
		call    _TFont_SetName
		mov     edx, [esp]
		mov     eax, [this]TMainForm.ListLBox
		mov     eax, [eax]TControl.FFont
		call    _TFont_SetName
		mov     edx, [esp]
		mov     eax, [this]TMainForm.CautionREdit
		mov     eax, [eax]TControl.FFont
		call    _TFont_SetName
		pop     edx
		pop     eax
		jmp     _TFont_SetName
		ud2

		#undef this
	}
}

__declspec(naked) void __cdecl TMainForm_LoadSetting_treeBackScreen_Canvas_Font_setName()
{
	__asm// Borland register
	{
#define this edi

		push    eax
		push    edx
		mov     eax, [this]TMainForm.LockCBox
		mov     eax, [eax]TControl.FFont
		call    _TFont_SetName
		mov     edx, [esp]
		mov     eax, [this]TMainForm.ToggleCBox
		mov     eax, [eax]TControl.FFont
		call    _TFont_SetName
		pop     edx
		pop     eax
		jmp     _TFont_SetName
		ud2

#undef this
	}
}
