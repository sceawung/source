#pragma once

#include "TWinControl.h"
#include "TCanvas.h"

#define DEFINE_TCUSTOMCONTROL                   \
    DEFINE_TWINCONTROL;                         \
    TCanvas      *FCanvas;                      \
    __int32 : 32

#pragma pack(push, 4)
typedef struct GridCoord
{
	int X;
	int Y;
} TGridCoord;
#pragma pack(pop)

typedef struct GridOptions
{
	bool goFixedVertLine     : 1;
	bool goFixedHorzLine     : 1;
	bool goVertLine          : 1;
	bool goHorzLine          : 1;
	bool goRangeSelect       : 1;
	bool goDrawFocusSelected : 1;
	bool goRowSizing         : 1;
	bool goColSizing         : 1;
	bool goRowMoving         : 1;
	bool goColMoving         : 1;
	bool goEditing           : 1;
	bool goTabs              : 1;
	bool goRowSelect         : 1;
	bool goAlwaysShowEditor  : 1;
	bool goThumbTracking     : 1;
	__int16 : 0;
} TGridOptions;

typedef char TScrollStyle, TGridState;

#define DEFINE_TCUSTOMGRID                      \
    DEFINE_TCUSTOMCONTROL;                      \
    TGridCoord       FAnchor;                   \
    TFormBorderStyle FBorderStyle;              \
    bool             FCanEditModify;            \
    __int16 : 16;                               \
    int              FColCount;                 \
    void            *FColWidths;                \
    void            *FTabStops;                 \
    TGridCoord       FCurrent;                  \
    int        const FDefaultColWidth;          \
    int        const FDefaultRowHeight;         \
    int              FFixedCols;                \
    int              FFixedRows;                \
    TColor           FFixedColor;               \
    int              FGridLineWidth;            \
    TGridOptions     FOptions;                  \
    __int16 : 16;                               \
    int        const FRowCount;                 \
    void            *FRowHeights;               \
    TScrollStyle     FScrollBars;               \
    __int8 : 8;                                 \
    __int16 : 16;                               \
    TGridCoord const FTopLeft;                  \
    int              FSizingIndex;              \
    int              FSizingPos;                \
    int              FSizingOfs;                \
    int              FMoveIndex;                \
    int              FMovePos;                  \
    TPoint           FHitTest;                  \
    TInplaceEdit    *FInplaceEdit;              \
    int              FInplaceCol;               \
    int              FInplaceRow;               \
    int              FColOffset;                \
    bool             FDefaultDrawing;           \
    bool             FEditorMode;               \
    TGridState       FGridState;                \
    bool             FSaveCellExtents;          \
    TGridOptions     DesignOptionsBoost;        \
    bool             VirtualView;               \
    __int8 : 8;                                 \
    __int32 : 32

typedef struct GridDrawState
{
	bool gdSelected : 1;
	bool gdFocused  : 1;
	bool gdFixed    : 1;
	__int8 : 0;
} TGridDrawState;

typedef struct MovedEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int FromIndex, int ToIndex);
	void      const *This;
} TMovedEvent;

typedef struct DrawCellEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int ACol, int ARow, const TRect *Rect, TGridDrawState State);
	void      const *This;
} TDrawCellEvent;

typedef struct GetEditEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int ACol, int ARow, AnsiString *Value);
	void      const *This;
} TGetEditEvent;

typedef struct SelectCellEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int ACol, int ARow, bool *CanSelect);
	void      const *This;
} TSelectCellEvent;

typedef struct SetEditEvent
{// Borland
	void(__fastcall *Method)(TObject *Sender, int ACol, int ARow, const AnsiString Value);
	void      const *This;
} TSetEditEvent;

#define DEFINE_TCUSTOMDRAWGRID                  \
    DEFINE_TCUSTOMGRID;                         \
    TMovedEvent      FOnColumnMoved;            \
    TDrawCellEvent   FOnDrawCell;               \
    TGetEditEvent    FOnGetEditMask;            \
    TGetEditEvent    FOnGetEditText;            \
    TMovedEvent      FOnRowMoved;               \
    TSelectCellEvent FOnSelectCell;             \
    TSetEditEvent    FOnSetEditText;            \
    TNotifyEvent     FOnTopLeftChanged

#define DEFINE_TDRAWGRID                        \
    DEFINE_TCUSTOMDRAWGRID

#pragma pack(push, 1)
typedef struct DrawGrid
{
	DEFINE_TDRAWGRID;
} TDrawGrid;
#pragma pack(pop)

EXTERN_C int __fastcall TDrawGrid_GetClientWidth(TDrawGrid *DrawGrid);
EXTERN_C int __fastcall TDrawGrid_GetClientHeight(TDrawGrid *DrawGrid);
EXTERN_C void __fastcall TDrawGrid_SetDefaultRowHeight(TDrawGrid *DrawGrid, const int RowHeight);
EXTERN_C void __fastcall TDrawGrid_SetDefaultColWidth(TDrawGrid *DrawGrid, const int ColWidth);
EXTERN_C void __fastcall TDrawGrid_SetRowCount(TDrawGrid *DrawGrid, const int RowCount);
EXTERN_C int __fastcall TDrawGrid_GetRowHeights(TDrawGrid *DrawGrid, const int Index);
EXTERN_C void __fastcall TDrawGrid_SetRowHeights(TDrawGrid *DrawGrid, const int Index, const int RowHeights);
EXTERN_C void __fastcall TDrawGrid_SetRow(TDrawGrid *DrawGrid, const int Row);
EXTERN_C void __fastcall TDrawGrid_SetTopRow(TDrawGrid *DrawGrid, const int TopRow);
EXTERN_C void __fastcall _TDrawGrid_MouseToCell(TDrawGrid *DrawGrid, int X, int Y, int *ARow, int *ACol);
#define TDrawGrid_MouseToCell(DrawGrid, X, Y, ACol, ARow) _TDrawGrid_MouseToCell(DrawGrid, X, Y, ARow, ACol)
EXTERN_C void __fastcall TDrawGrid_Repaint(TDrawGrid *DrawGrid);
