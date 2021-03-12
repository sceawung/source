#pragma once

#include <windows.h>
#include "TShiftState.h"

typedef unsigned char        Byte;             // 0..255
typedef unsigned short       Word;             // 0..65535
typedef unsigned long        DWord;            // 0..4294967295
typedef char *AnsiString;
typedef CRGB TColor;
typedef RECT TRect;

typedef struct Object
{
	LPCVOID *VTable;
} TObject, TList, TMessage, TBitmap;

typedef struct DelphiInterface
{
	void *intf;
} _di_IChangeNotifier;

typedef struct ComponentState
{
	bool csLoading          : 1;
	bool csReading          : 1;
	bool csWriting          : 1;
	bool csDestroying       : 1;
	bool csDesigning        : 1;
	bool csAncestor         : 1;
	bool csUpdating         : 1;
	bool csFixups           : 1;
	bool csFreeNotification : 1;
	bool csInline           : 1;
	bool csDesignInstance   : 1;
	__int16 : 0;
} TComponentState;

typedef struct ComponentStyle
{
	bool csInheritable    : 1;
	bool csCheckPropAvail : 1;
	bool csSubComponent   : 1;
	bool csTransient      : 1;
	__int8 : 0;
} TComponentStyle;

#define DEFINE_TCOMPONENT                           \
    LPCVOID          *VTable;                       \
    struct Component *FOwner;                       \
    AnsiString        FName;                        \
    int               FTag;                         \
    TList            *FComponents;                  \
    TList            *FFreeNotifies;                \
    int               FDesignInfo;                  \
    TComponentState   FComponentState;              \
    __int16 : 16;                                   \
    void             *FVCLComObject;                \
    TComponentStyle   FComponentStyle;              \
    __int8 : 8;                                     \
    __int16 : 16;                                   \
    void             *__IInterface;                 \
    void             *__IInterfaceComponentReference

#pragma pack(push, 1)
typedef struct Component
{
	DEFINE_TCOMPONENT;
} TComponent, TPopupMenu;
#pragma pack(pop)

typedef struct WndMethod
{// Borland
    void(__fastcall *Method)(TMessage *Message);
    void      const *This;
} TWndMethod;

typedef struct ControlStyle
{ 
	bool csAcceptsControls   : 1;
	bool csCaptureMouse      : 1;
	bool csDesignInteractive : 1;
	bool csClickEvents       : 1;
	bool csFramed            : 1;
	bool csSetCaption        : 1;
	bool csOpaque            : 1;
	bool csDoubleClicks      : 1;
	bool csFixedWidth        : 1;
	bool csFixedHeight       : 1;
	bool csNoDesignVisible   : 1;
	bool csReplicatable      : 1;
	bool csNoStdEvents       : 1;
	bool csDisplayDragImage  : 1;
	bool csReflector         : 1;
	bool csActionClient      : 1;
	bool csMenuEvents        : 1;
	__int32 : 0;
} TControlStyle;
static_assert(sizeof(TControlStyle) == sizeof(__int32), "modified pack size");

typedef struct ControlState
{
	bool csLButtonDown      : 1;
	bool csClicked          : 1;
	bool csPalette          : 1;
	bool csReadingState     : 1;
	bool csAlignmentNeeded  : 1;
	bool csFocusing         : 1;
	bool csCreating         : 1;
	bool csPaintCopy        : 1;
	bool csCustomPaint      : 1;
	bool csDestroyingHandle : 1;
	bool csDocking          : 1;
	__int16 : 0;
} TControlState;

typedef char TAlign, TDragMode, TBiDiMode;

typedef struct Anchors
{
	bool akLeft   : 1;
	bool akTop    : 1;
	bool akRight  : 1;
	bool akBottom : 1;
	__int8 : 0;
} TAnchors;

typedef struct NotifyEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender);
	void      const *This;
} TNotifyEvent;

typedef char TFontPitch;

typedef enum FontStyles// : unsigned char
{
	fsBold      = 0x01,
	fsItalic    = 0x02,
	fsUnderline = 0x04,
	fsStrikeOut = 0x08,
} TFontStyles;

typedef struct FontStylesBase
{
	bool fsBold      : 1;
	bool fsItalic    : 1;
	bool fsUnderline : 1;
	bool fsStrikeOut : 1;
	__int8 : 0;
} TFontStylesBase;

typedef Byte TFontCharset;

typedef struct SmallStringBase
{
    char unsigned Len;
    char          Data[31];
} TFontDataName;

#pragma pack(push, 4)
typedef struct Resource {
	struct Resource *Next;
	int              RefCount;
	unsigned         Handle;
	Word             HashCode;
	union {
		struct TFontData {
			HFONT           Handle;
			int             Height;
			TFontPitch      Pitch;
			TFontStylesBase Style;
			TFontCharset    Charset;
			TFontDataName   Name;
		}            Font;
	};
} TResource;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TFont {
#pragma region TGraphicsObject
	LPCVOID            *VTable;
	__int32 : 32;      
	TNotifyEvent        FOnChange;
	TResource          *FResource;
	CRITICAL_SECTION   *FOwnerLock;
#pragma endregion
	TColor              FColor;
	int                 FPixelsPerInch;
	_di_IChangeNotifier FNotify;
	__int32 : 32;
} TFont;
#pragma pack(pop)

typedef TObject TControlActionLink;
typedef int  TConstraintSize;

typedef struct SizeConstraints
{
	LPCVOID        *VTable;
	struct Control *FControl;
	TConstraintSize FMaxHeight;
	TConstraintSize FMaxWidth;
	TConstraintSize FMinHeight;
	TConstraintSize FMinWidth;
	TNotifyEvent    FOnChange;
} TSizeConstraints;

typedef short TCursor;
typedef POINT TPoint;

typedef struct ScalingFlags
{
	bool sfLeft       : 1;
	bool sfTop        : 1;
	bool sfWidth      : 1;
	bool sfHeight     : 1;
	bool sfFont       : 1;
	bool sfDesignSize : 1;
	__int8 : 0;
} TScalingFlags;

typedef char TDragKind, TDockOrientation;
typedef void TMetaClass;
typedef TObject TDragObject, TDragDockObject;
typedef char TMouseButton, TDragState;

typedef struct CanResizeEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int *NewWidth, int *NewHeight, bool *Resize);
	void      const *This;
} TCanResizeEvent;

typedef struct ConstrainedResizeEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int *MinWidth, int *MinHeight, int *MaxWidth, int *MaxHeight);
	void      const *This;
} TConstrainedResizeEvent;

typedef struct MouseEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void      const *This;
} TMouseEvent;

typedef struct MouseMoveEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TShiftState Shift, int X, int Y);
	void      const *This;
} TMouseMoveEvent;

typedef struct DragDropEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TObject *Source, int X, int Y);
	void      const *This;
} TDragDropEvent;

typedef struct DragOverEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool *Accept);
	void      const *This;
} TDragOverEvent;

typedef struct StartDockEvent
{// Borland
	void(__fastcall *Method)(TObject* Sender, TDragDockObject **DragObject);
	void      const *This;
} TStartDockEvent;

typedef struct EndDragEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TObject *Target, int X, int Y);
	void      const *This;
} TEndDragEvent;

typedef struct StartDragEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TDragObject **DragObject);
	void      const *This;
} TStartDragEvent;

typedef struct ContextPopupEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, TPoint *MousePos, bool *Handled);
	void      const *This;
} TContextPopupEvent;

typedef struct MouseWheelEvent
{// Borland
	void(__fastcall *Method)(TObject* Sender, TShiftState Shift, int WheelDelta, const TPoint *MousePos, bool *Handled);
	void      const *This;
} TMouseWheelEvent;

typedef struct MouseWheelUpDownEvent
{// Borland
	void(__fastcall *Method)(TObject* Sender, TShiftState Shift, const TPoint *MousePos, bool *Handled);
	void      const *This;
} TMouseWheelUpDownEvent;

typedef char THelpType;
typedef int  THelpContext;

#define DEFINE_TCONTROL                             \
    DEFINE_TCOMPONENT;                              \
    struct WinControl      *FParent;                \
    __int32 : 32;                                   \
    TWndMethod              FWindowProc;            \
    int                     FLeft;                  \
    int                     FTop;                   \
    int               const FWidth;                 \
    int               const FHeight;                \
    TControlStyle           FControlStyle;          \
    TControlState           FControlState;          \
    bool                    FDesktopFont;           \
    bool              const FVisible;               \
    bool                    FEnabled;               \
    bool                    FParentFont;            \
    bool                    FParentColor;           \
    TAlign                  FAlign;                 \
    bool                    FAutoSize;              \
    TDragMode               FDragMode;              \
    bool                    FIsControl;             \
    TBiDiMode               FBiDiMode;              \
    bool                    FParentBiDiMode;        \
    TAnchors                FAnchors;               \
    bool                    FAnchorMove;            \
    __int8 : 8;                                     \
    char                   *FText;                  \
    TFont                  *FFont;                  \
    TControlActionLink     *FActionLink;            \
    TColor                  FColor;                 \
    TSizeConstraints       *FConstraints;           \
    TCursor                 FCursor;                \
    TCursor                 FDragCursor;            \
    TPopupMenu             *FPopupMenu;             \
    AnsiString              FHint;                  \
    int                     FFontHeight;            \
    TPoint                  FAnchorRules;           \
    TPoint                  FOriginalParentSize;    \
    TScalingFlags           FScalingFlags;          \
    bool                    FShowHint;              \
    bool                    FParentShowHint;        \
    TDragKind               FDragKind;              \
    TDockOrientation        FDockOrientation;       \
    __int8 : 8;                                     \
    __int16 : 16;                                   \
    struct WinControl      *FHostDockSite;          \
    int                     FWheelAccumulator;      \
    int                     FUndockWidth;           \
    int                     FUndockHeight;          \
    int                     FLRDockWidth;           \
    int                     FTBDockHeight;          \
    TMetaClass             *FFloatingDockSiteClass; \
    __int32 : 32;                                   \
    TCanResizeEvent         FOnCanResize;           \
    TConstrainedResizeEvent FOnConstrainedResize;   \
    TMouseEvent             FOnMouseDown;           \
    TMouseMoveEvent         FOnMouseMove;           \
    TMouseEvent             FOnMouseUp;             \
    TDragDropEvent          FOnDragDrop;            \
    TDragOverEvent          FOnDragOver;            \
    TNotifyEvent            FOnResize;              \
    TStartDockEvent         FOnStartDock;           \
    TEndDragEvent           FOnEndDock;             \
    TStartDragEvent         FOnStartDrag;           \
    TEndDragEvent           FOnEndDrag;             \
    TNotifyEvent            FOnClick;               \
    TNotifyEvent            FOnDblClick;            \
    TContextPopupEvent      FOnContextPopup;        \
    TMouseWheelEvent        FOnMouseWheel;          \
    TMouseWheelUpDownEvent  FOnMouseWheelDown;      \
    TMouseWheelUpDownEvent  FOnMouseWheelUp;        \
    THelpType               FHelpType;              \
    __int8 : 8;                                     \
    __int16 : 16;                                   \
    AnsiString              FHelpKeyword;           \
    THelpContext            FHelpContext;           \
    __int32 : 32

#pragma pack(push, 1)
typedef struct Control
{
	DEFINE_TCONTROL;
} TControl, TLabel, TSplitter, TImage;
#pragma pack(pop)

#define TControl_GetWidth(this) ((TControl *)(this))->FWidth
#define TControl_GetHeight(this) ((TControl *)(this))->FHeight
#define TControl_GetVisible(this) ((TControl *)(this))->FVisible
