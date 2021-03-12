#include "xx.h"
#include "TMainForm.h"

#pragma warning(disable:4733)

void(__cdecl * const TMainForm_GoCalcEnter)(TMainForm *this) = (LPVOID)0x0043F454;
void(__cdecl * const TMainForm_SetLockVisible)(TMainForm *this, TSSGSubject *SSGS, BOOLEAN MustVisible) = (LPVOID)0x00444490;
void(__cdecl * const TMainForm_DrawTreeCell)(TMainForm *this, LPVOID DestCanvas, int ARow, RECT *Rect) = (LPVOID)0x00444848;
void(__cdecl * const TMainForm_ChangeSubjectPanel)(TMainForm *this, long Type) = (LPVOID)0x004465AC;
void(__cdecl * const TMainForm_GoCalcHexChange)(TMainForm *this, bool IsCalcHex) = (LPVOID)0x00447194;

__declspec(naked) void __fastcall TMainForm_StringEnterBtnClick(TMainForm *this, LPVOID Sender)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0043E98CH
		jmp     ecx
	}
}

__declspec(naked) void __stdcall TMainForm_Guide(const char *Mes, int Flags)
{
	extern const DWORD F0048C1F8;
	extern const DWORD F0056DDBC;

	__asm
	{
		push    ebp
		mov     eax, 0060E730H
		mov     ebp, esp
		sub     esp, 36
		push    ebx
		mov     ebx, dword ptr ds:[_MainForm]
		call    __InitExceptBlockLDTC
		mov     ecx, [ebx]TMainForm.userMode
		mov     eax, [ebx]TMainForm.guideForm
#ifdef GUIDE_LIMITED
		dec     ecx
		jz      L2
#endif
		test    eax, eax
		jnz     L1
		mov     eax, dword ptr ds:[0062089CH]
		mov     ecx, ebx
		mov     edx, 1// refer dl only
		call    dword ptr [F0048C1F8]
		mov     [ebx]TMainForm.guideForm, eax
	L1:
		mov     ecx, dword ptr [ebp + 0CH]
		mov     edx, dword ptr [ebp + 8H]
		push    ecx
		push    edx
		push    eax
		call    TGuideForm_Guide
		mov     eax, [ebx]TMainForm.guideForm
		add     esp, 12
		cmp     byte ptr [eax + 57H], 0
		jne     L2
		call    dword ptr [F0056DDBC]
	L2:
		mov     ecx, dword ptr [ebp - 36]
		mov     dword ptr fs:[0], ecx
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret     8
	}
}
