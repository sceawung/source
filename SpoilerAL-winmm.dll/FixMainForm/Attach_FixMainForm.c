#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "TSSString.h"
#include "TMainForm.h"

EXTERN_C void __cdecl TMainForm_ctor();
EXTERN_C void __cdecl TMainForm_FormClose_Header();
EXTERN_C void __cdecl TMainForm_dtor();
EXTERN_C void __cdecl TMainForm_LoadSetting_NoteREdit_Font_setName();
EXTERN_C void __cdecl TMainForm_LoadSetting_treeBackScreen_Canvas_Font_setName();
EXTERN_C void __cdecl TMainForm_LoadSetting_SetUserMode();
EXTERN_C void __cdecl TMainForm_SubjectAccess_FixDirSameChildren();
EXTERN_C UINT __stdcall TSSGSubject_Write_WithDrawTree(LPVOID, LPVOID, LPVOID);
EXTERN_C void __stdcall Caller_TMainForm_SetLockVisible_ModifyLockName(void *, void *);
EXTERN_C void __cdecl TMainForm_DrawTreeCell_DrawHover();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_FixLabelDrawX();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifySplitRoll();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifySplitLabel();
EXTERN_C void __fastcall TMainForm_DrawTreeCell_DrawStr(LPVOID, LPVOID, LPVOID, LPVOID, LPVOID);
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifyNowValueBoolVector();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifyNowValueFloatCalc();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_FixDefaultColWidth();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_DrawFocusRect();
EXTERN_C void __cdecl Caller_TMainForm_DrawTree();
EXTERN_C void __cdecl TMainForm_LockCBoxClick_CellRectStub();
EXTERN_C void __cdecl TMainForm_DGridMouseMove_DrawTree();
EXTERN_C void __cdecl TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible();
EXTERN_C void __cdecl TMainForm_FormResize_CheckTreeSize();
EXTERN_C void __cdecl TMainForm_CheckTreeSize();
EXTERN_C void __cdecl TMainForm_M_TitleSelectClick_OpenSSG();
EXTERN_C void __cdecl TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage();
EXTERN_C void __fastcall TMainForm_M_CustomizeClick_RedrawCalcImage(void *this);
EXTERN_C void __cdecl TMainForm_LoadCLD_Footer();
EXTERN_C void __cdecl TFindNameForm_ctor();
EXTERN_C void __cdecl TGuideForm_ctor();
EXTERN_C void __cdecl UpdateUserModeMenu();
EXTERN_C void __cdecl TSearchForm_ctor();

extern const DWORD vcl_System_AnsiString_dtor;

static __declspec(naked) void __cdecl TMainForm_SubjectAccess_break_ListLBox()
{
	__asm {// ListLBox->Items->Clear();
		call dword ptr [vcl_System_AnsiString_dtor]
		push FALSE
		push edi
		push ebx
		call TMainForm_SetLockVisible
		add  esp, 12
		mov  ecx, [ebx]TMainForm.ListLBox
		mov  eax, [ecx + 0x0218]
		mov  edx, [eax]
		jmp  dword ptr [edx + 0x44]
		ud2
	}
}

static HWND __fastcall TMainForm_SubjectAccess_GetCautionHandle(TMainForm *const this, TSSString *const SSGS)
{
	return SSGS->cautious ? TWinControl_GetHandle(this->CautionREdit) : NULL;
}

static __declspec(naked) void __cdecl TMainForm_SubjectAccess_CautiousString() {
	__asm {// TSSString *SSGS
		mov  edx, dword ptr [ebp - 0x02FC]
		mov  ecx, ebx
		call TMainForm_SubjectAccess_GetCautionHandle
		test eax, eax
		jnz  CAUTION
		rep ret

		align 16
	CAUTION:// TSSArgString.value.c_str()
		push 0
		push SB_TOP
		push WM_VSCROLL
		push eax
		push dword ptr [ebp - 0xE4]
		push 0
		push WM_SETTEXT
		push eax
		call SendMessageA
		call PostMessageA
		// selectSubject = NULL;
		mov  dword ptr [ebx]TMainForm.selectSubject, 0
		// CautionTabS->TabVisible = true;
		mov  edx, TRUE
		mov  eax, [ebx]TMainForm.CautionTabS
		mov  ecx, 0x00594684
		call ecx
		// PageCtrl->ActivePage = CautionTabS;
		mov  edx, [ebx]TMainForm.CautionTabS
		mov  eax, [ebx]TMainForm.PageCtrl
		mov  ecx, 0x00594E6C
		jmp  ecx
		ud2
	}
}

static __declspec(naked) void __cdecl TMainForm_SubjectAccess_break_MultiLBox()
{
	__asm {// MultiLBox->Items->Clear();
		call dword ptr [vcl_System_AnsiString_dtor]
		push FALSE
		push edi
		push ebx
		call TMainForm_SetLockVisible
		add  esp, 12
		mov  ecx, [ebx]TMainForm.MultiLBox
		mov  eax, [ecx + 0x0218]
		mov  edx, [eax]
		jmp  dword ptr [edx + 0x44]
		ud2
	}
}

static __declspec(naked) void __cdecl TMainForm_StringEnterBtnClick_GetSubjectName(string* retval, TSSGSubject* this, TSSGCtrl* SSGC) {
	__asm {// this = TMainForm*->selectSubject
		mov  eax, [esi]TMainForm.selectSubject
		xchg eax, [esp + 8]
		jmp  TSSGSubject_GetSubjectName
		ud2
	}
}

static void __fastcall TMainForm_GoCalcEnter_selectAll(TMainForm* mainForm) {
	HWND edit = TWinControl_GetHandle(vector_at(&mainForm->calcImage->valBox, 1).edit);
	SendMessageA(edit, EM_SETSEL, 0, -1);
	PostMessageA(edit, WM_SETFOCUS, (WPARAM)NULL, 0);
}

static void __declspec(naked) __fastcall TMainForm_GoCalcEnter_destroyMessage(string* this, DWORD two) {
	__asm {// Borland's fastcall
		call vcl_System_AnsiString_dtor
		mov  ecx, ebx
#if 0
		mov  dword ptr [esp], 0x0043FB70// goto calcImage->SetNewVal("");
#endif
		jmp  TMainForm_GoCalcEnter_selectAll
	}
}

static __declspec(naked) void __cdecl TMainForm_SetLockVisible_IsLocked(TSSGCtrl* SSGC, TSSGSubject* SSGS) {
	__asm {// SSGS = TMainForm*->selectSubject
		mov  eax, [ebx]TMainForm.selectSubject
		xchg eax, [esp + 8]
		jmp  TSSGCtrl_IsLocked
		ud2
	}
}

#ifdef FORMAT_NOT_IMPLEMENTED
static __declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueString() {
	__asm {
		mov   edx, edi
		mov   ecx, ebx
		call  TMainForm_SubjectAccess_GetCautionHandle
		mov   ecx, 0x00445931
		mov   edx, 0x00445B4A
		test  eax, eax
		cmovz edx, ecx
		jmp   edx
		ud2
	}
}
#endif

static enum TFontStyles __fastcall TMainForm_DrawTreeCell_LetStyle(const TSSGSubject *const SSGS, enum TFontStyles Style)
{
	return Style & ~fsUnderline | (SSGS && SSGS->type == stDIR && SSGS->isOpen) << 2;
}

static __declspec(naked) long __cdecl TMainForm_DrawTreeCell_shadowModeStub() {
	static const DWORD Vcl_Graphics_TFont_GetStyle = 0x055D610;
	static const DWORD Vcl_Graphics_TFont_SetStyle = 0x055D61C;

	__asm {
		mov   eax, [esi]TCanvas.FFont
		call  Vcl_Graphics_TFont_GetStyle

		movzx edx, al
		mov   ecx, edi
		call  TMainForm_DrawTreeCell_LetStyle

		movzx edx, al
		mov   eax, [esi]TCanvas.FFont
		call  Vcl_Graphics_TFont_SetStyle

		mov   eax, [ebx]TMainForm.shadowMode
		ret
	}
}

#define JB_REL32              (WORD )0x820F
#define JB_REL8               (BYTE )0x72
#define ADD_ESP               (WORD )0xC483
#define MOV_ECX_DWORD_PTR_EAX (WORD )0x088B
#define MOV_ECX_DWORD_PTR_ECX (WORD )0x098B
#define MOV_ECX_EBX           (WORD )0xCB8B
#define CALL_REL32            (BYTE )0xE8
#define JMP_REL32             (BYTE )0xE9
#define JMP_REL8              (BYTE )0xEB
#define XCHG_EAX_EDX          (BYTE )0x92
#define NOP                   (BYTE )0x90
#define NOP_X2                (WORD )0x9066
#define NOP_X4                (DWORD)0x00401F0F

EXTERN_C void __cdecl Attach_FixMainForm()
{
	// TMainForm::TMainForm
	*(LPBYTE )0x00401FA5 = CALL_REL32;
	*(LPDWORD)0x00401FA6 = (DWORD)TMainForm_ctor - (0x00401FA6 + sizeof(DWORD));
	*(LPBYTE )0x00401FAA = NOP;

	// TMainForm::FormClose
	*(LPBYTE )0x004026C8 = CALL_REL32;
	*(LPDWORD)0x004026C9 = (DWORD)TMainForm_FormClose_Header - (0x004026C9 + sizeof(DWORD));
	*(LPBYTE )0x004026CD = NOP;
#if 0
	// TMainForm::FormCreate
	//   StringNowValEdit->SetBounds(8, 32, 200, 20);
	*(LPWORD )0x00417D9E = BSWAP16(0x6A64);
	*(LPBYTE )0x00417DA0 =         0x68   ;
	*(LPDWORD)0x00417DA1 =         320    ;
	//   StringNewValEdit->SetBounds(8, 54, 200, 20);
	*(LPWORD )0x00417DBD = BSWAP16(0x6A64);
	*(LPBYTE )0x00417DBF =         0x68   ;
	*(LPDWORD)0x00417DC0 =         320    ;
	*(LPDWORD)0x00417DCB =         360    ;
	//   StringEnterBtn	->SetBounds(8, 76,  64, 24);
	*(LPDWORD)0x00417DE7 =         700    ;
	// TMainForm::PageCtrlResize
	//   LockCBox   ->Width = W;
	*(LPDWORD)0x00446CAD = offsetof(TMainForm, StringNewValEdit);
#endif
	// TMainForm::~TMainForm
	*(LPBYTE )0x0045FDE9 = JMP_REL32;
	*(LPDWORD)0x0045FDEA = (DWORD)TMainForm_dtor - (0x0045FDEA + sizeof(DWORD));
	*(LPDWORD)0x0045FDEE = NOP_X4;

	// TMainForm::LoadSetting
	*(LPDWORD)(0x004307A0 + 1) = (DWORD)TMainForm_LoadSetting_NoteREdit_Font_setName - (0x004307A0 + 1 + sizeof(DWORD));

	*(LPDWORD)(0x004351B0 + 1) = (DWORD)TMainForm_LoadSetting_treeBackScreen_Canvas_Font_setName - (0x004351B0 + 1 + sizeof(DWORD));

	*(LPBYTE )0x00437E28 = JMP_REL32;
	*(LPDWORD)0x00437E29 = (DWORD)TMainForm_LoadSetting_SetUserMode - (0x00437E29 + sizeof(DWORD));
	*(LPBYTE )0x00437E2D = NOP;

	// TMainForm::DGridSelectCell
	/*
		mov     dword ptr [esp - 8], eax                ; 00439E90 _ 89. 44 24, F8
		mov     eax, dword ptr [esp]                    ; 00439E94 _ 8B. 04 24
		mov     dword ptr [esp], ecx                    ; 00439E97 _ 89. 0C 24
		push    eax                                     ; 00439E9A _ 50
		mov     ecx, dword ptr [esp - 4]                ; 00439E9B _ 8B. 4C 24, FC
		jmp     TMainForm_DGridSelectCell               ; 00439E9F _ E9, ????????
	*/
	*(LPDWORD)0x00439E90 = BSWAP32(0x894424F8);
	*(LPDWORD)0x00439E94 = BSWAP32(0x8B042489);
	*(LPDWORD)0x00439E98 = BSWAP32(0x0C24508B);
	*(LPDWORD)0x00439E9C = BSWAP32(0x4C24FCE9);
	*(LPDWORD)0x00439EA0 = (DWORD)_TMainForm_DGridSelectCell - (0x00439EA0 + sizeof(DWORD));

	// TMainForm::SubjectAccess
	*(LPDWORD)(0x00439F42 + 1) = (DWORD)TMainForm_SubjectAccess_FixDirSameChildren - (0x00439F42 + 1 + sizeof(DWORD));

	*(LPWORD )0x0043A2E6 = BSWAP16(0x8B93);// mov edx, dword ptr [ebx + ...
	*(LPDWORD)0x0043A2E8 = offsetof(TMainForm, selectSubject);

	*(LPDWORD)(0x0043A59D + 1) = (DWORD)TMainForm_SubjectAccess_break_ListLBox - (0x0043A59D + 1 + sizeof(DWORD));

	*(LPWORD )0x0043A88E = BSWAP16(0x8B8B);// mov ecx, dword ptr [ebx + ...
	*(LPDWORD)0x0043A890 = offsetof(TMainForm, selectSubject);

	*(LPBYTE )0x0043B075 =         0x66       ;
	*(LPWORD )0x0043B076 = BSWAP16(0xC746    );// mov  word ptr [esi+10h], 14Ch
	*(LPDWORD)0x0043B078 = BSWAP32(0x104C01 << 8 | CALL_REL32);
	*(LPDWORD)0x0043B07C = (DWORD)TMainForm_SubjectAccess_CautiousString - (0x0043B07C + sizeof(DWORD));
	*(LPDWORD)0x0043B080 = BSWAP32(0x836E1C03);// sub dword ptr [esi+1Ch], 3

	*(LPDWORD)(0x0043B1F5 + 1) = (DWORD)TMainForm_SubjectAccess_break_MultiLBox - (0x0043B1F5 + 1 + sizeof(DWORD));

	// TMainForm::ToggleCBoxClick
	*(LPBYTE )0x0043D52D = CALL_REL32;
	*(LPDWORD)0x0043D52E = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043D52E + sizeof(DWORD));
	*(LPBYTE )0x0043D532 = NOP;

	// TMainForm::ListLBoxClick
	*(LPBYTE )0x0043D774 = CALL_REL32;
	*(LPDWORD)0x0043D775 = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043D775 + sizeof(DWORD));
	*(LPBYTE )0x0043D779 = NOP;

	// TMainForm::MultiListUpdate
	*(LPWORD )0x0043E148 = MOV_ECX_DWORD_PTR_EAX;
	*(LPBYTE )0x0043E14A = CALL_REL32;
	*(LPDWORD)0x0043E14B = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043E14B + sizeof(DWORD));
	*(LPBYTE )0x0043E14F = NOP;

	// TMainForm::StringEnterBtnClick
	*(LPBYTE )0x0043EC5B = CALL_REL32;
	*(LPDWORD)0x0043EC5C = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043EC5C + sizeof(DWORD));
	*(LPBYTE )0x0043EC60 = NOP;

	*(LPDWORD)0x0043EEE7 = (DWORD)TMainForm_StringEnterBtnClick_GetSubjectName - (0x0043EEE7 + sizeof(DWORD));

	// TMainForm::GoCalcEnter
	*(LPDWORD)0x0043F692 = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F692 + sizeof(DWORD));
	*(LPDWORD)0x0043F6D1 = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F6D1 + sizeof(DWORD));
	*(LPDWORD)0x0043F965 = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F965 + sizeof(DWORD));
	*(LPDWORD)0x0043F9AA = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F9AA + sizeof(DWORD));

	*(LPWORD )0x0043FA6E = MOV_ECX_DWORD_PTR_ECX;
	*(LPBYTE )0x0043FA70 = CALL_REL32;
	*(LPDWORD)0x0043FA71 = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043FA71 + sizeof(DWORD));
	*(LPBYTE )0x0043FA75 = NOP;

	// TMainForm::LockCBoxClick
	//  nowSelectSubjectIndex => nowSelectSubjectIndex + 1
	*(LPBYTE )0x00442FF0 = 0xB5;
	*(LPBYTE )0x00442FF5 = 0x56;
	*(LPWORD )0x00443004 = BSWAP16(0x41 << 8 | CALL_REL32);
	*(LPDWORD)0x00443006 = 0x00559D94 - (0x00443006 + sizeof(DWORD));
	*(LPBYTE )0x0044300A = CALL_REL32;
	*(LPDWORD)0x0044300B = (DWORD)TMainForm_LockCBoxClick_CellRectStub - (0x0044300B + sizeof(DWORD));

	*(LPDWORD)0x00443028 = BSWAP32(0x405052FF);
	*(LPDWORD)0x0044302C = BSWAP32(0xB547FDFF);
	*(LPWORD )0x00443030 = BSWAP16(0xFF8B);
	*(LPBYTE )0x00443032 =         0xC3;

	// TMainForm::HotKeyEditKeyDown
	/*
		mov     dword ptr [esp - 8], eax                ; 00443054 _ 89. 44 24, F8
		mov     eax, dword ptr [esp]                    ; 00443058 _ 8B. 04 24
		mov     dword ptr [esp], ecx                    ; 0044305B _ 89. 0C 24
		push    eax                                     ; 0044305E _ 50
		mov     ecx, dword ptr [esp - 4]                ; 0044305F _ 8B. 4C 24, FC
		jmp     TMainForm_HotKeyEditKeyDown             ; 00443063 _ E9, ????????
		nop                                             ; 00443068 _ 90
	*/
	*(LPDWORD)0x00443054 = BSWAP32(0x894424F8);
	*(LPDWORD)0x00443058 = BSWAP32(0x8B042489);
	*(LPDWORD)0x0044305C = BSWAP32(0x0C24508B);
	*(LPDWORD)0x00443060 = BSWAP32(0x4C24FCE9);
	*(LPDWORD)0x00443064 = (DWORD)TMainForm_HotKeyEditKeyDown - (0x00443064 + sizeof(DWORD));
	*(LPWORD )0x00443068 = NOP;

	// TMainForm::SetCalcNowValue
	*(LPBYTE )0x00440608 = NOP;
	*(LPBYTE )0x00440609 = JMP_REL32;

	*(LPBYTE )0x00440ECE = JB_REL8;

	// TMainForm::CalcButtonPushFunc
	*(LPBYTE )0x00443490 = JMP_REL32;
	*(LPDWORD)0x00443491 = (DWORD)TMainForm_CalcButtonPushFunc - (0x00443491 + sizeof(DWORD));
	*(LPDWORD)0x00443495 = NOP_X4;

	// TMainForm::SetLockVisible
	*(LPDWORD)(0x004444DB + 1) = (DWORD)Caller_TMainForm_SetLockVisible_ModifyLockName - (0x004444DB + 1 + sizeof(DWORD));
	*(LPBYTE )(0x004444EA + 2) = 0;// stack size to discard
	*(LPDWORD)(0x00444619 + 1) = (DWORD)Caller_TMainForm_SetLockVisible_ModifyLockName - (0x00444619 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x00444628 + 2) = 0;// stack size to discard
	*(LPDWORD)(0x004447E9 + 1) = (DWORD)TMainForm_SetLockVisible_IsLocked - (0x004447E9 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00444C09 = JMP_REL32;
	*(LPDWORD)0x00444C0A = (DWORD)TMainForm_DrawTreeCell_DrawHover - (0x00444C0A + sizeof(DWORD));
	*(LPDWORD)0x00444C0E = NOP_X4;

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00444E61 = JMP_REL32;
	*(LPDWORD)0x00444E62 = (DWORD)TMainForm_DrawTreeCell_FixLabelDrawX - (0x00444E62 + sizeof(DWORD));
	*(LPBYTE )0x00444E66 = NOP;

	// replaced at "SubjectStringTable\SubjectStringOperator.c" - TMainForm_DrawTreeCell_GetStrParam
#if 0
	// TMainForm::DrawTreeCell
	//   string tmpS( SS->GetStrParam() );
	*(LPDWORD)(0x00444FBB + 1) = (DWORD)TMainForm_DrawTreeCell_ModifySplitRoll - (0x00444FBB + 1 + sizeof(DWORD));

	//   DrawStr = SS->GetStrParam();
	*(LPDWORD)(0x004451C3 + 1) = (DWORD)TMainForm_DrawTreeCell_ModifySplitLabel - (0x004451C3 + 1 + sizeof(DWORD));
#endif

#ifndef FORMAT_NOT_IMPLEMENTED
	// TMainForm::DrawTreeCell
	*(LPBYTE )0x004452BD =         0x8D       ;// lea  edx,[DrawStr]
	*(LPWORD )0x004452BE = BSWAP16(0x55E8    );// mov  ecx, this
	*(LPDWORD)0x004452C0 = BSWAP32(0x89D9E800);// call ...
	*(LPDWORD)0x004452C3 = (DWORD)TMainForm_DrawTreeCell_DrawStr - (0x004452C3 + sizeof(DWORD));
	*(LPBYTE )0x004452C7 = JMP_REL32;
	*(LPDWORD)0x004452C8 = 0x00445B80 - (0x004452C8 + sizeof(DWORD));
	*(LPBYTE )0x004452CC = NOP;
#else
	// TMainForm::DrawTreeCell
	*(LPDWORD)0x00445402;// = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueString;

	// TMainForm::DrawTreeCell
	*(LPDWORD)0x00445406 = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueBoolVector;

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445452 + 1) = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueCalc - (0x00445452 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	/*
		sub     esp, 8                                  ; 004457F0 _ 83. EC, 08
		fstp    qword ptr [esp]                         ; 004457F3 _ DD. 1C 24
		lea     edx, [ebp - 0FCH]                       ; 004457F6 _ 8D. 95, FFFFFF04
		push    edx                                     ; 004457FC _ 52
		call    TStringDivision::ToStringDouble         ; 004457FD _ E8, <offset TMainForm_DrawTreeCell_ModifyNowValueFloatCalc - 00445802H>
		add     esp, 16                                 ; 00445802 _ 83. C4, 10
	*/
	*(LPDWORD)0x004457F0 = BSWAP32(0x83EC08DD);
	*(LPWORD )0x004457F4 = BSWAP16(0x1C24    );
	*(LPDWORD)0x004457FE = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueFloatCalc - (0x004457FE + sizeof(DWORD));
	*(LPBYTE )0x00445804 = 0x10;
#endif

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00445B8C = CALL_REL32;
	*(LPDWORD)0x00445B8D = (DWORD)TMainForm_DrawTreeCell_shadowModeStub - (0x00445B8D + sizeof(DWORD));
	*(LPBYTE )0x00445B91 = XCHG_EAX_EDX;

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445C82 + 1) = (DWORD)TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445C82 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445CD9 + 1) = (DWORD)TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445CD9 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445DDE + 1) = (DWORD)TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445DDE + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x0044602C = JMP_REL8;
	*(LPBYTE )0x0044602D = 0x6D;
	*(LPBYTE )0x0044602E = NOP;

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x0044609B = JMP_REL32;
	*(LPDWORD)0x0044609C = (DWORD)TMainForm_DrawTreeCell_DrawFocusRect - (0x0044609C + sizeof(DWORD));

	// TMainForm::DrawTree
	*(LPBYTE )0x00446448 = JMP_REL32;
	*(LPDWORD)0x00446449 = (DWORD)Caller_TMainForm_DrawTree - (0x00446449 + sizeof(DWORD));
	*(LPBYTE )0x0044644D = NOP;

	// TMainForm::GetFontColor
	((LPVOID*)0x0044652D)[atSPLIT  ] = ((LPVOID*)0x0044652D)[atUNKNOWN];
	((LPVOID*)0x0044652D)[atUNKNOWN] = ((LPVOID*)0x0044652D)[atNONE];

	// TMainForm::ChangeSubjectPanel
	*(LPDWORD)(0x004465BD + 2) = 0;// je $

	*(LPDWORD)(0x00446716 + 1) = (DWORD)TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible - (0x00446716 + 1 + sizeof(DWORD));

	// TMainForm::FillPanelImage
	*(LPBYTE )0x0044695A = JMP_REL32;
	*(LPDWORD)0x0044695B = 0x00446A32 - (0x0044695B + sizeof(DWORD));
	*(LPWORD )0x0044695F = NOP_X2;

	// TMainForm::DGridMouseMove
#if 0// replaced at "OptimizeGuide\OptimizeGuide.c" - TMainForm_DGridDrawCell_DrawTreeCell
	*(LPBYTE )0x00446C67 = CALL_REL32;
	*(LPDWORD)0x00446C68 = (DWORD)TMainForm_DGridMouseMove_DrawTree - (0x00446C68 + sizeof(DWORD));
	*(LPBYTE )0x00446C6C = NOP;
#endif

	// TMainForm::FormResize
	*(LPDWORD)(0x00446DB2 + 1) = (DWORD)TMainForm_FormResize_CheckTreeSize - (0x00446DB2 + 1 + sizeof(DWORD));

	// TMainForm::FormMouseWheel
	/*
		mov     dword ptr [esp - 8], eax                ; 00446DC0 _ 89. 44 24, F8
		mov     eax, dword ptr [esp]                    ; 00446DC4 _ 8B. 04 24
		mov     dword ptr [esp], ecx                    ; 00446DC7 _ 89. 0C 24
		push    eax                                     ; 00446DCA _ 50
		mov     ecx, dword ptr [esp - 4]                ; 00446DCB _ 8B. 4C 24, FC
		jmp     _TMainForm_FormMouseWheel               ; 00446DCF _ E9, ????????
		nop                                             ; 00446DD4 _ 90
	*/
	*(LPDWORD)0x00446DC0 = BSWAP32(0x894424F8);
	*(LPDWORD)0x00446DC4 = BSWAP32(0x8B042489);
	*(LPDWORD)0x00446DC8 = BSWAP32(0x0C24508B);
	*(LPDWORD)0x00446DCC = BSWAP32(0x4C24FCE9);
	*(LPDWORD)0x00446DD0 = (DWORD)_TMainForm_FormMouseWheel - (0x00446DD0 + sizeof(DWORD));
	*(LPWORD )0x00446DD4 = NOP;

	// TMainForm::CheckTreeSize
	*(LPBYTE )0x00446E88 = JMP_REL32;
	*(LPDWORD)0x00446E89 = (DWORD)TMainForm_CheckTreeSize - (0x00446E89 + sizeof(DWORD));
	*(LPBYTE )0x00446E8D = NOP;

	// TMainForm::M_TitleSelectClick
	*(LPDWORD)(0x00449220 + 2) = 0x004497BE - (0x00449220 + 2 + sizeof(DWORD));

	*(LPDWORD)(0x0044993B + 1) = (DWORD)TMainForm_M_TitleSelectClick_OpenSSG - (0x0044993B + 1 + sizeof(DWORD));

	// TMainForm::M_CustomizeClick
	/*
		test    eax, eax                                ; 0044B24F _ 85. C0
		jnz     0044B258H                               ; 0044B251 _ 75, 05
		jmp     0044B131H                               ; 0044B253 _ E9, FFFFFED9
	*/
	*(LPBYTE )0x0044B24F =               0x85 ;
	*(LPDWORD)0x0044B250 = BSWAP32(0xC07505E9);
	*(LPDWORD)0x0044B254 =         0xFFFFFED9 ;

	// TMainForm::M_CustomizeClick
	/*
		mov     eax, edx                                ; 0044B26C _ 8B. C2
		mov     edx, 3                                  ; 0044B26E _ BA, 00000003
		call    dword ptr [ecx - 4]                     ; 0044B273 _ FF. 51, FC
		mov     word ptr [edi + 16], 332                ; 0044B276 _ 66: C7. 47, 10, 014C
		jmp     0044B131H                               ; 0044B27C _ E9, FFFFFEB0
		nop                                             ; 0044B281 _ 90
	*/
	*(LPDWORD)0x0044B26C = BSWAP32(0x8BC2BA03);
	*(LPDWORD)0x0044B270 = BSWAP32(0x000000FF);
	*(LPDWORD)0x0044B274 = BSWAP32(0x51FC66C7);
	*(LPDWORD)0x0044B278 = BSWAP32(0x47104C01);
	*(LPDWORD)0x0044B27C = BSWAP32(0xE9B0FEFF);
	*(LPWORD )0x0044B280 = BSWAP16(0xFF90    );

	// TMainForm::M_CustomizeClick
	*(LPBYTE )(0x0044B2E4 + 1) = 0x2F;
	*(LPBYTE )0x0044B30D = JMP_REL32;
	*(LPDWORD)0x0044B30E = (DWORD)TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage - (0x0044B30E + sizeof(DWORD));
	*(LPWORD )0x0044B312 = ADD_ESP;
	*(LPBYTE )0x0044B314 = 0x10;
	*(LPWORD )0x0044B315 = MOV_ECX_EBX;
	*(LPBYTE )0x0044B317 = CALL_REL32;
	*(LPDWORD)0x0044B318 = (DWORD)TMainForm_M_CustomizeClick_RedrawCalcImage - (0x0044B318 + sizeof(DWORD));

	// TMainForm::M_CustomizeClick
	*(LPBYTE )0x0044B327 = JMP_REL8;
	*(LPBYTE )0x0044B328 = 0x0D;

	// TMainForm::LoadCLD
	/*
		mov     eax, dword ptr [esi]                    ; 00454C91 _ 8B. 06
		add     eax, 360                                ; 00454C93 _ 05, 00000168
		mov     ecx, esp                                ; 00454C98 _ 8B. CC
		push    eax                                     ; 00454C9A _ 50
		push    ecx                                     ; 00454C9B _ 51
	*/
	*(LPWORD )0x00454C92 = BSWAP16(    0x0605);
	*(LPDWORD)0x00454C94 =         0x00000168 ;
	*(LPDWORD)0x00454C98 = BSWAP32(0x8BCC5051);

	// TMainForm::LoadCLD
	*(LPBYTE )0x0045621C = CALL_REL32;
	*(LPDWORD)0x0045621D = (DWORD)TMainForm_LoadCLD_Footer - (0x0045621D + sizeof(DWORD));
	*(LPBYTE )0x00456221 = NOP;

	// TFindNameForm::TFindNameForm
	*(LPBYTE )0x0048397C = CALL_REL32;
	*(LPDWORD)0x0048397D = (DWORD)TFindNameForm_ctor - (0x0048397D + sizeof(DWORD));
	*(LPBYTE )0x00483981 = NOP;

	// TGuideForm::TGuideForm
	*(LPDWORD)(0x0048C23D + 1) = (DWORD)TGuideForm_ctor - (0x0048C23D + 1 + sizeof(DWORD));

	// TGuideForm::UserModeCmbBoxChange
	/*
		mov     ecx, dword ptr ds:[_MainForm]           ; 0048D15C _ 8B. 0D, 0064CE2C(d)
		dec     eax                                     ; 0048D162 _ 48
		mov     dword ptr [ecx + 448H], eax             ; 0048D163 _ 89. 81, 00000448
		call    UpdateUserModeMenu                      ; 0048D169 _ E8, ????????
		jmp     0048D178H                               ; 0048D16E _ EB, 08
	*/
	*(LPDWORD)0x0048D15C = BSWAP32(0x8B0D2CCE);
	*(LPDWORD)0x0048D160 = BSWAP32(0x64004889);
	*(LPDWORD)0x0048D164 = BSWAP32(0x81480400);
	*(LPWORD )0x0048D168 = BSWAP16(0x00E8);
	*(LPDWORD)0x0048D16A = (DWORD)UpdateUserModeMenu - (0x0048D16A + sizeof(DWORD));
	*(LPWORD )0x0048D16E = BSWAP16(0xEB08);

	// TSearchForm::TSearchForm
	/*
		call    TSearchForm_ctor                        ; 004919AF _ E8, ????????
		jmp     004919B8H                               ; 004919B4 _ EB, 02
	*/
	*(LPBYTE )0x004919AF = CALL_REL32;
	*(LPDWORD)0x004919B0 = (DWORD)TSearchForm_ctor - (0x004919B0 + sizeof(DWORD));
	*(LPDWORD)0x004919B4 = BSWAP32(0xEB029090);
	*(LPWORD )0x004919B6 = NOP_X2;

	// TSSGActionListner::OnProcessOpen
	*(LPBYTE )0x0052EB4B = JB_REL8;

	// TSSGActionListner::OnSubjectDisabled
	*(LPBYTE )(0x0052EBFD + 2) = 0x04;

	// TSSGActionListner::OnProcessOpenError
	*(LPBYTE )(0x0052EEA0 + 2) = 0x04;

	// TSSGActionListner::OnSubjectReadError
	*(LPBYTE )(0x0052EFB2 + 2) = 0x04;

	// TSSGActionListner::OnSubjectWriteError
	*(LPBYTE )(0x0052F164 + 2) = 0x04;

	// TSSGActionListner::OnSubjectReadSuccess
	*(LPBYTE )(0x0052F345 + 2) = 0x04;

	// TSSGActionListner::OnSubjectWriteSuccess
	*(LPBYTE )(0x0052F6F9 + 2) = 0x04;

	// TSSGActionListner::OnParsingError
	*(LPBYTE )(0x0052F870 + 2) = 0x04;

	// TSSGActionListner::OnParsingProcess
	*(LPBYTE ) 0x0052FAD1 =         0x48   ;// dec  eax
	*(LPWORD ) 0x0052FAD2 = BSWAP16(0xA80A);// test al, 0Ah
	*(LPWORD ) 0x0052FAD4 = BSWAP16(0x0F8A);// jnz  => jp

	// TSSGActionListner::OnParsingDoubleProcess
	*(LPBYTE ) 0x0052FCDD =         0x48   ;// dec  eax
	*(LPWORD ) 0x0052FCDE = BSWAP16(0xA80A);// test al, 0Ah
	*(LPWORD ) 0x0052FCE0 = BSWAP16(0x0F8A);// jnz  => jp

	*(LPDWORD)(0x0052FCF4 + 0) = BSWAP32(0x83EC08DD);// sub  esp, 8 
	*(LPWORD )(0x0052FCF7 + 1) = BSWAP16(0x1C24    );// fstp qword ptr [esp]
	*(LPDWORD)(0x0052FCFE + 1) = (DWORD)TStringDivision_ToStringDouble - (0x0052FCFE + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0052FD03 + 2) = 0x10;// stack size to discard

	// TMainForm::LoadSetting
	*(LPDWORD)0x00602888 = BSWAP32('Fixe');
	*(LPDWORD)0x0060288C = BSWAP32('dSys');
	*(LPWORD )0x00602890 = BSWAP16('\0\0');

	*(LPBYTE )0x00602945 =         'M'    ;
	*(LPWORD )0x00602946 = BSWAP16('S '  );
	*(LPDWORD)0x00602948 = BSWAP32('Goth');
	*(LPDWORD)0x0060294C = BSWAP24('ic\0');

	// TMainForm::LoadCLD
	*(LPBYTE )0x0060348D =         'M'    ;
	*(LPWORD )0x0060348E = BSWAP16('S '  );
	*(LPDWORD)0x00603490 = BSWAP32('Goth');
	*(LPDWORD)0x00603494 = BSWAP24('ic\0');
}
