#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

static __declspec(naked) TMainForm* __fastcall TMainForm_DGridDrawCell_DrawTreeCell(int ARow, TDrawGrid *DGrid)
{
	static const LPCVOID listener = NULL;
	__asm {// Don't change eax!
		lea    ecx, [eax]TMainForm.ssgActionListner
		cmp    [eax]TMainForm.userMode, 4
		cmovae ecx, listener// quiet mode
		mov    [eax]TMainForm.ssgCtrl.ssgActionListner, ecx
		mov    ecx, [edx]TDrawGrid.FCanvas
		ret
	}// Must make ecx into TCanvas*
}

static void __fastcall TMainForm_DGridDrawCell_epilog(
	TMainForm       *const this,
	TCanvas   const *const DestCanvas,
	BYTE/*TGridDrawState*/ State,
	RECT      const *const Rect,
	int              const ARow)
{// restore listener pointer
	this->ssgCtrl.ssgActionListner = &this->ssgActionListner;
}

static void __fastcall TSSGCtrl_GoLockWrite_forBegin(
	TSSGCtrl *const This,
	string   *const tmpS)
{
	string_ctor_null(tmpS);
	switch (TMainForm_GetUserMode(MainForm))
	{// quiet mode
	case 4:
	case 5:
		This->ssgActionListner = NULL;
		break;
	}
}

static void __fastcall TSSGCtrl_GoLockWrite_forEnd(
	TSSGCtrl *const This)
{// restore listener pointer
	This->ssgActionListner = &MainForm->ssgActionListner;
}

#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9
#define JNE_SHORT (BYTE)0x75
#define NOP       (BYTE)0x90

EXTERN_C void __cdecl OptimizeGuide()
{
	// TMainForm::DGridDrawCell
	*(LPBYTE )0x00444831 = CALL_REL;
	*(LPDWORD)0x00444832 = (DWORD)TMainForm_DGridDrawCell_DrawTreeCell - (0x00444832 + sizeof(DWORD));
	*(LPBYTE )0x00444836 = NOP;

	*(LPWORD )0x0044483E = BSWAP16(0x595A);// pop ecx; pop edx
	*(LPDWORD)0x00444840 = BSWAP32(0x58585D << 8 | JMP_REL32);
	*(LPDWORD)0x00444844 = (DWORD)TMainForm_DGridDrawCell_epilog - (0x00444844 + sizeof(DWORD));

	// TGuideForm::Guide
	*(LPBYTE )0x0048CF94 = JMP_REL32;
	*(LPDWORD)0x0048CF95 = (DWORD)TGuideForm_Guide - (0x0048CF95 + sizeof(DWORD));
	*(LPBYTE )0x0048CF99 = NOP;

	// TSSGCtrl::GoLockWrite - static
	*(LPBYTE )0x004FEA42 =         0x55   ;// ecx => edx
	*(LPWORD )0x004FEA44 = BSWAP16(0x8BCB);// mov ecx, ebx
	*(LPBYTE )0x004FEA46 = CALL_REL;
	*(LPDWORD)0x004FEA47 = (DWORD)TSSGCtrl_GoLockWrite_forBegin - (0x004FEA47 + sizeof(DWORD));

	*(LPBYTE )0x004FEA91 = JNE_SHORT;
	*(LPBYTE )0x004FEA92 = (BYTE)(0x004FEA67 - (0x004FEA92 + sizeof(BYTE)));
	*(LPBYTE )0x004FEA93 = NOP;
	*(LPWORD )0x004FEA94 = BSWAP16(0x8BCB);// mov ecx, ebx
	*(LPBYTE )0x004FEA96 = CALL_REL;
	*(LPDWORD)0x004FEA97 = (DWORD)TSSGCtrl_GoLockWrite_forEnd - (0x004FEA97 + sizeof(DWORD));
}
