#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TCheckBox.h"
#include "TDrawGrid.h"
#include "TStringDivision.h"
#include "TSSGScriptStruct.h"
#include "TSSGCtrl.h"
#include "TCalcImage.h"
#include "TGuideForm.h"
#include "TSSGSubject.h"
#include "TFindNameForm.h"

typedef TForm TSearchForm;

#pragma pack(push, 1)
typedef struct _TMainForm
{
	union
	{
		LPCVOID *VTable;
		TForm    super;
	};
	TMainMenu       *MainMenu1;
	TMenuItem       *M_File;
	TMenuItem       *M_Exit;
	TSplitter       *Splitter;
	TMenuItem       *N1;
	TMenuItem       *M_TitleSelect;
	TMenuItem       *M_Memory;
	TMenuItem       *M_MemorySetting;
	TMenuItem       *M_View;
	TMenuItem       *M_Customize;
	TMenuItem       *N2;
	TMenuItem       *M_AlwaysOnTop;
	TMenuItem       *N4;
	TMenuItem       *M_FindName;
	TMenuItem       *M_SSGReFind;
	TMenuItem       *M_Help;
	TMenuItem       *M_UseHelp;
	TMenuItem       *M_SSGHelp;
	TMenuItem       *M_CLDHelp;
	TMenuItem       *N3;
	TMenuItem       *M_Version;
	TMenuItem       *M_Creator;
	TMenuItem       *M_HomePage;
	TMenuItem       *M_OnMostTop;
	TMenuItem       *M_ProcessAdd;
	TPopupMenu      *ListPMenu;
	TMenuItem       *P_AllNonCheck;
	TMenuItem       *P_AllCheck;
	TMenuItem       *M_UnLockWrite;
	TMenuItem       *P_SelectCheck;
	TMenuItem       *P_NonSelectCheck;
	TMenuItem       *P_OnlyNonSelectCheck;
	TMenuItem       *P_OnlySelectCheck;
	TMenuItem       *N5;
	TMenuItem       *N8;
	TMenuItem       *M_Address;
	TMenuItem       *M_ClearSelectAdjustAttribute;
	TMenuItem       *M_ClearAllAdjustAttribute;
	TMenuItem       *M_AutoDialogAdjustment;
	TMenuItem       *N6;
	TMenuItem       *N10;
	TMenuItem       *M_SaveSubjectStatus;
	TMenuItem       *M_LoadSubjectStatus;
	TMenuItem       *M_SearchAdjustment;
	TMenuItem       *N9;
	TDrawGrid       *DGrid;
	TMenuItem       *M_UserMode;
	TMenuItem       *M_BegginerMode;
	TMenuItem       *M_NormalMode;
	TMenuItem       *M_CreatorMode;
	TMenuItem       *M_DebaggerMode;
	TMenuItem       *M_SystemCheck;
	TEdit           *HotKeyEdit;
	TPageControl    *PageCtrl;
	TTabSheet       *CtrlTabS;
	TImage          *PanelImage;
	TCheckBox       *LockCBox;
	TEdit           *StringNowValEdit;
	TEdit           *StringNewValEdit;
	TCheckBox       *ToggleCBox;
	TButton         *StringEnterBtn;
	TListBox        *MultiLBox;
	TListBox        *ListLBox;
	TTabSheet       *InfoTabS;
	TRichEdit       *NoteREdit;
	TPanel          *ScriptInfoPanel;
	TLabel          *Label2;
	TLabel          *Label1;
	TEdit           *CreatorEdit;
	TEdit           *MakerEdit;
	TTabSheet       *CautionTabS;
	TRichEdit       *CautionREdit;
	TColor           titleColor;
	TColor           dirColor;
	TColor           calcColor;
	TColor           toggleColor;
	TColor           listColor;
	TColor           bitListColor;
	TColor           stringColor;
	TColor           backColor;
	TColor           traceColor;
	TColor           panelBackColor;
	long             shadowColor;
	long             shadowMode;
	long             shadowOffset;
	bool             isAutoSaveAdjustVal;
	bool             isNowValueDraw;
	__int16 : 16;
	long             userMode;
	TBitmap         *panelG;
	TBitmap         *backG;
	__int32 : 32;
	bcb6_std_string  backGPath;
	bcb6_std_string  panelGPath;
	bcb6_std_string  calcFreeName;
	bcb6_std_string  calcPushName;
	bcb6_std_string  splashPath;
	bool             isCalcHex;
	bool             isBackGUse;
	bool             isPanelGUse;
	bool             isSplashUse;
	byte             greatTopModeKey;
	__int8 : 8;
	__int16 : 16;
	TSSGScriptStruct selectScript;
	TBitmap         *treeBackScreen;
	TBitmap         *invertImage;
	TBitmap         *mostTopImage;
	TSSGSubject     *selectSubject;
	TCalcImage      *calcImage;
	__int32 : 32;
	bcb6_std_bvector listCheckedVec;
	long             treeFontWidth;
	int              nowSelectSubjectIndex;
	int              invertGridRow;
	long             nowPanelType;
	long             maxNameLength;
	TFindNameForm   *findNameForm;
	TSearchForm     *searchForm;
	TGuideForm      *guideForm;
	long             guideFormLeft;
	long             guideFormTop;
	long             guideFormWidth;
	long             guideFormHeight;
	long             searchFormLeft;
	long             searchFormTop;
	long             searchFormWidth;
	long             searchFormHeight;
	long             findFormLeft;
	long             findFormTop;
	long             findFormWidth;
	long             findFormHeight;
	long             lastTitleIndex;
	long             lastKeyIndex;
	long             titleFindHeight;
	long             titleSelectWindowWidth;
	long             titleSelectWindowHeight;
	bool             isTitleFindUp;
	bool             isAdjustGuideUse;
	__int16 : 16;
	bcb6_std_string  sortKey;
	bool             nowAdjusting;
	bool             canChange;
	__int16 : 16;
	long             wheelDeltaCount;
	BYTE             topManager[8];
	BYTE             topManagerListner[168];
	BYTE             ssgActionListner[16];
	BYTE             adjustmentListner[16];
	TStringDivision  strD;
	TSSGCtrl         ssgCtrl;
	bcb6_std_string  exeDir;
	byte             searchMode;
	__int8 : 8;
	__int16 : 16;
	bcb6_std_vector_string *DistractionVec;// distinction
	bcb6_std_vector  keyToIndexMap;
	bcb6_std_vector  scriptVec;
	bcb6_std_vector  toScriptVec;
	bcb6_std_vector_PTSSGSubject treeSubjectVec;
} TMainForm;
#pragma pack(pop)

#define _MainForm 0x0064CE2C
#define MainForm (*(TMainForm **)_MainForm)

EXTERN_C void __fastcall TMainForm_StringEnterBtnClick(TMainForm *this, LPVOID Sender);

EXTERN_C void(__cdecl * const TMainForm_GoCalcEnter)(TMainForm *this);

EXTERN_C void __cdecl TMainForm_CalcButtonPushFunc(TMainForm *this, long BtnNum);

EXTERN_C void(__cdecl * const TMainForm_SetLockVisible)(TMainForm *this, TSSGSubject *SSGS, bool MustVisible);
EXTERN_C void(__cdecl * const TMainForm_DrawTreeCell)(TMainForm *this, LPVOID DestCanvas, int ARow, RECT *Rect);
EXTERN_C void(__cdecl * const TMainForm_ChangeSubjectPanel)(TMainForm *this, long Type);
EXTERN_C void(__cdecl * const TMainForm_GoCalcHexChange)(TMainForm *this, bool IsCalcHex);

#define DRAWTREE_CONVENTION 0
EXTERN_C void
#if DRAWTREE_CONVENTION
__stdcall
#endif
TMainForm_DrawTree(TMainForm *this, LPVOID DestCanvas, long LeftOffset, long TopOffset
#if DRAWTREE_CONVENTION
				   , BOOL IgnoreDebug
#endif
);

EXTERN_C void __fastcall _TMainForm_FormMouseWheel(TMainForm *this, LPVOID Sender, WORD Shift, bool *Handled, POINT *MousePos, int WheelDelta);
#define TMainForm_FormMouseWheel(this, Sender, Shift, Handled, MousePos, WheelDelta) TMainForm_FormMouseWheel(this, Sender, Shift, WheelDelta, MousePos, Handled)

EXTERN_C void __cdecl TMainForm_CheckTreeSize(TMainForm *this, bool AllWidthCheck);

EXTERN_C void __fastcall TMainForm_HotKeyEditKeyDown(TMainForm *this, LPVOID Sender, WORD *Key, WORD Shift);

EXTERN_C void __stdcall TMainForm_Guide(const char *Mes, int Flags);

EXTERN_C void __fastcall _TMainForm_DGridSelectCell(TMainForm *this, LPVOID Sender, int ACol, bool *CanSelect, int ARow);
#define TMainForm_DGridSelectCell(this, Sender, ACol, ARow, CanSelect) _TMainForm_DGridSelectCell(this, Sender, ACol, CanSelect, ARow)

#define TMainForm_GetUserMode(this) (this)->userMode

