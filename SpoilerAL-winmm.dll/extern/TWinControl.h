#pragma once

#include "TControl.h"
#include "TCanvas.h"

typedef struct BevelEdges
{
	bool beLeft   : 1;
	bool beTop    : 1;
	bool eRight   : 1;
	bool beBottom : 1;
	__int8 : 0;
} TBevelEdges;

typedef char TBevelCut, TBevelKind, TImeMode;
typedef int  TBevelWidth, TBorderWidth;
typedef struct DelphiInterface _di_IDockManager;

typedef struct DockDropEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TDragDockObject *Source, int X, int Y);
	void      const *This;
} TDockDropEvent;

typedef struct DockOverEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TDragDockObject *Source, int X, int Y, TDragState State, bool *Accept);
	void      const *This;
} TDockOverEvent;

typedef struct GetSiteInfoEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TControl *DockClient, TRect *InfluenceRect, const TPoint *MousePos, bool *CanDock);
	void      const *This;
} TGetSiteInfoEvent;

typedef struct KeyEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, Word *Key, TShiftState Shift);
	void      const *This;
} TKeyEvent;

typedef struct KeyPressEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, char *Key);
	void      const *This;
} TKeyPressEvent;

typedef struct UnDockEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TControl *Client, struct WinControl *NewTarget, bool *Allow);
	void      const *This;
} TUnDockEvent;

#define DEFINE_TWINCONTROL                      \
    DEFINE_TCONTROL;                            \
    Word              FAlignLevel;              \
    TBevelEdges       FBevelEdges;              \
    TBevelCut         FBevelInner;              \
    TBevelCut         FBevelOuter;              \
    TBevelKind        FBevelKind;               \
    __int16 : 16;                               \
    TBevelWidth       FBevelWidth;              \
    TBorderWidth      FBorderWidth;             \
    TBrush           *FBrush;                   \
    void             *FDefWndProc;              \
    TList            *FDockClients;             \
    _di_IDockManager  FDockManager;             \
    HWND              FHandle;                  \
    TImeMode          FImeMode;                 \
    __int8 : 8;                                 \
    __int16 : 16;                               \
    AnsiString        FImeName;                 \
    void             *FObjectInstance;          \
    HWND              FParentWindow;            \
    TList            *FTabList;                 \
    TList            *FControls;                \
    TList            *FWinControls;             \
    int               FTabOrder;                \
    bool              FTabStop;                 \
    bool              FCtl3D;                   \
    bool              FShowing;                 \
    bool              FUseDockManager;          \
    bool              FDockSite;                \
    bool              FParentCtl3D;             \
    __int16 : 16;                               \
    __int32 : 32;                               \
    TDockDropEvent    FOnDockDrop;              \
    TDockOverEvent    FOnDockOver;              \
    TNotifyEvent      FOnEnter;                 \
    TNotifyEvent      FOnExit;                  \
    TGetSiteInfoEvent FOnGetSiteInfo;           \
    TKeyEvent         FOnKeyDown;               \
    TKeyPressEvent    FOnKeyPress;              \
    TKeyEvent         FOnKeyUp;                 \
    TUnDockEvent      FOnUnDock;                \
    bool              FDoubleBuffered;          \
    bool              FInImeComposition;        \
    __int16 : 16;                               \
    TPoint            FDesignSize;              \
    __int32 : 32

#pragma pack(push, 1)
typedef struct WinControl
{
	DEFINE_TWINCONTROL;
} TWinControl, TPanel, TListBox, TComboBox, TEdit, TRichEdit, TInplaceEdit, TPageControl, TTabSheet, TRadioGroup;
#pragma pack(pop)

typedef Word TScrollBarInc;
typedef char TScrollBarKind, TScrollBarStyle;

#pragma pack(push, 1)
typedef struct ControlScrollBar
{
	LPCVOID                    *VTable;
	struct ScrollingWinControl *FControl;
	TScrollBarInc               FIncrement;
	TScrollBarInc               FPageIncrement;
	int                         FPosition;
	int                         FRange;
	int                         FCalcRange;
	TScrollBarKind              FKind;
	__int8 : 8;
	Word                        FMargin;
	bool                        FVisible;
	bool                        FTracking;
	bool                        FScaled;
	bool                        FSmooth;
	int                         FDelay;
	int                         FButtonSize;
	TColor                      FColor;
	bool                        FParentColor;
	__int8 : 8;
	__int16 : 16;
	int                         FSize;
	TScrollBarStyle             FStyle;
	__int8 : 8;
	__int16 : 16;
	int                         FThumbSize;
	int                         FPageDiv;
	int                         FLineDiv;
	bool                        FUpdateNeeded;
	__int8 : 8;
	__int16 : 16;
} TControlScrollBar;
#pragma pack(pop)

#define DEFINE_TSCROLLINGWINCONTROL             \
    DEFINE_TWINCONTROL;                         \
    TControlScrollBar* FHorzScrollBar;          \
    TControlScrollBar* FVertScrollBar;          \
    bool               FAutoScroll;             \
    __int8 : 8;                                 \
    __int16 : 16;                               \
    int                FAutoRangeCount;         \
    bool               FUpdatingScrollBars;     \
    __int8 : 8;                                 \
    __int16 : 16;                               \
    __int32 : 32

#pragma pack(push, 1)
typedef struct ScrollingWinControl
{
	DEFINE_TSCROLLINGWINCONTROL;
} TScrollingWinControl;
;
#pragma pack(pop)

typedef struct BorderIcons
{
	bool biSystemMenu : 1;
	bool biMinimize   : 1;
	bool biMaximize   : 1;
	bool biHelp       : 1;
	__int8 : 0;
} TBorderIcons;

typedef char TFormBorderStyle, TWindowState, TShowAction, TFormStyle, TPosition, TDefaultMonitor, TTileMode, TPrintScale, TCloseAction;
typedef int TModalResult;
typedef TObject TIcon, TWMKey;
typedef struct DelphiInterface _di_IDesignerHook, _di_IOleForm;
typedef TComponent TMainMenu, TMenuItem;

typedef struct CloseEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TCloseAction *Action);
	void      const *This;
} TCloseEvent;

typedef struct CloseQueryEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, bool *CanClose);
	void      const *This;
} TCloseQueryEvent;

typedef struct HelpEvent
{// Borland
    void(__fastcall *Method)(Word Command, int Data, bool *CallHelp);
    void      const *This;
} THelpEvent;

typedef struct ShortCutEvent
{// Borland
    void(__fastcall *Method)(TWMKey *Msg, bool *Handled);
    void      const *This;
} TShortCutEvent;

typedef struct FormState
{
	bool fsCreating        : 1;
	bool fsVisible         : 1;
	bool fsShowing         : 1;
	bool fsModal           : 1;
	bool fsCreatedMDIChild : 1;
	bool fsActivated       : 1;
	__int8 : 0;
} TFormState;

#define DEFINE_TCUSTOMFORM                          \
    DEFINE_TSCROLLINGWINCONTROL;                    \
    TWinControl       *FActiveControl;              \
    TWinControl       *FFocusedControl;             \
    TBorderIcons      FBorderIcons;                 \
    TFormBorderStyle  FBorderStyle;                 \
    bool              FSizeChanging;                \
    TWindowState      FWindowState;                 \
    TShowAction       FShowAction;                  \
    bool              FKeyPreview;                  \
    bool              FActive;                      \
    TFormStyle        FFormStyle;                   \
    TPosition         FPosition;                    \
    TDefaultMonitor   FDefaultMonitor;              \
    TTileMode         FTileMode;                    \
    bool              FDropTarget;                  \
    bool              FOldCreateOrder;              \
    TPrintScale       FPrintScale;                  \
    __int16 : 16;                                   \
    TControlCanvas   *FCanvas;                      \
    AnsiString        FHelpFile;                    \
    TIcon            *FIcon;                        \
    bool              FInCMParentBiDiModeChanged;   \
    __int8 : 8;                                     \
    __int16 : 16;                                   \
    TMainMenu        *FMenu;                        \
    TModalResult      FModalResult;                 \
    _di_IDesignerHook FDesigner;                    \
    HWND              FClientHandle;                \
    TMenuItem        *FWindowMenu;                  \
    int               FPixelsPerInch;               \
    TMenuItem        *FObjectMenuItem;              \
    _di_IOleForm      FOleForm;                     \
    int               FClientWidth;                 \
    int               FClientHeight;                \
    int               FTextHeight;                  \
    void             *FDefClientProc;               \
    void             *FClientInstance;              \
    TWinControl      *FActiveOleControl;            \
    TFormBorderStyle  FSavedBorderStyle;            \
    __int8 : 8;                                     \
    __int16 : 16;                                   \
    __int32 : 32;                                   \
    TNotifyEvent      FOnActivate;                  \
    TCloseEvent       FOnClose;                     \
    TCloseQueryEvent  FOnCloseQuery;                \
    TNotifyEvent      FOnDeactivate;                \
    THelpEvent        FOnHelp;                      \
    TNotifyEvent      FOnHide;                      \
    TNotifyEvent      FOnPaint;                     \
    TShortCutEvent    FOnShortCut;                  \
    TNotifyEvent      FOnShow;                      \
    TNotifyEvent      FOnCreate;                    \
    TNotifyEvent      FOnDestroy;                   \
    bool              FAlphaBlend;                  \
    Byte              FAlphaBlendValue;             \
    bool              FTransparentColor;            \
    __int8 : 8;                                     \
    TColor            FTransparentColorValue;       \
    TList            *FActionLists;                 \
    TFormState        FFormState;                   \
    __int8 : 8;                                     \
    __int16 : 16

#define DEFINE_TFORM    \
    DEFINE_TCUSTOMFORM

#pragma pack(push, 1)
typedef struct Form
{
	DEFINE_TFORM;
} TForm;
#pragma pack(pop)

EXTERN_C HWND __fastcall TWinControl_GetHandle(LPCVOID WinControl);
