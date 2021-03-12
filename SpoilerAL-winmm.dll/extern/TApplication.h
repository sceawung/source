#pragma once

#include <windows.h>
#include "TWinControl.h"

typedef struct DelphiInterface _di_IHelpSystem;
typedef TObject TBasicAction, Exception;
typedef TWinControl THintWindow;
typedef char TTimerMode;

#pragma pack(push, 4)
typedef struct HintInfo
{
	TControl   *HintControl;
	TMetaClass *HintWindowClass;
	TPoint      HintPos;
	int         HintMaxWidth;
	TColor      HintColor;
	TRect       CursorRect;
	TPoint      CursorPos;
	int         ReshowTimeout;
	int         HideTimeout;
	AnsiString  HintStr;
	void       *HintData;
} THintInfo;
#pragma pack(pop)

typedef struct ActionEvent
{// Borland
    void(__fastcall *Method)(TBasicAction *Action, bool *Handled);
    void      const *This;
} TActionEvent;

typedef struct ExceptionEvent
{// Borland
    void(__fastcall *Method)(TObject *Sender, Exception* E);
    void      const *This;
} TExceptionEvent;

typedef struct MessageEvent
{// Borland
    void(__fastcall *Method)(struct tagMSG *Msg, bool *Handled);
    void      const *This;
} TMessageEvent;

typedef struct IdleEvent
{// Borland
    void(__fastcall *Method)(TObject *Sender, bool *Done);
    void      const *This;
} TIdleEvent;

typedef struct ShowHintEvent
{// Borland
    void(__fastcall *Method)(AnsiString *HintStr, bool *CanShow, THintInfo *HintInfo);
    void      const *This;
} TShowHintEvent;

typedef struct SettingChangeEvent
{// Borland
    void(__fastcall *Method)(TObject *Sender, int Flag, const AnsiString Section, int *Result);
    void      const *This;
} TSettingChangeEvent;

#pragma pack(push, 1)
typedef struct Application
{
	DEFINE_TCOMPONENT;
	HWND                FHandle;
	TBiDiMode           FBiDiMode;
	__int8 : 8;
	__int16 : 16;
	AnsiString          FBiDiKeyboard;
	AnsiString          FNonBiDiKeyboard;
	void               *FObjectInstance;
	TForm              *FMainForm;
	TControl           *FMouseControl;
	_di_IHelpSystem     FHelpSystem;
	AnsiString          FHelpFile;
	AnsiString          FHint;
	bool                FHintActive;
	bool                FUpdateFormatSettings;
	bool                FUpdateMetricSettings;
	bool                FShowMainForm;
	TColor              FHintColor;
	TControl           *FHintControl;
	TRect               FHintCursorRect;
	int                 FHintHidePause;
	int                 FHintPause;
	bool                FHintShortCuts;
	__int8 : 8;
	__int16 : 16;
	int                 FHintShortPause;
	THintWindow        *FHintWindow;
	bool                FShowHint;
	TTimerMode          FTimerMode;
	Word                FTimerHandle;
	AnsiString          FTitle;
	TList              *FTopMostList;
	int                 FTopMostLevel;
	TIcon              *FIcon;
	bool                FTerminate;
	bool                FActive;
	bool                FAllowTesting;
	__int8 : 8;
	unsigned            FTestLib;
	bool                FHandleCreated;
	bool                FRunning;
	__int16 : 16;
	TList              *FWindowHooks;
	void               *FWindowList;
	HWND                FDialogHandle;
	bool                FAutoDragDocking;
	__int8 : 8;
	__int16 : 16;
	TActionEvent        FOnActionExecute;
	TActionEvent        FOnActionUpdate;
	TExceptionEvent     FOnException;
	TMessageEvent       FOnMessage;
	THelpEvent          FOnHelp;
	TNotifyEvent        FOnHint;
	TIdleEvent          FOnIdle;
	TNotifyEvent        FOnDeactivate;
	TNotifyEvent        FOnActivate;
	TNotifyEvent        FOnMinimize;
	TNotifyEvent        FOnRestore;
	TShortCutEvent      FOnShortCut;
	TShowHintEvent      FOnShowHint;
	TSettingChangeEvent FOnSettingChange;
} TApplication;
#pragma pack(pop)

#define Application (*(LPVOID *)0x0064D0AC)

EXTERN_C void __fastcall TApplication_ProcessMessages(LPVOID _Application);
EXTERN_C void __fastcall TApplication_ActivateHint(TApplication *this, LPPOINT CursorPos);
