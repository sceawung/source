#pragma once

#include "TControl.h"

typedef struct CanvasState
{
	bool csHandleValid : 1;
	bool csFontValid   : 1;
	bool csPenValid    : 1;
	bool csBrushValid  : 1;
	__int8 : 0;
} TCanvasState;

typedef TObject TPen, TBrush;

#pragma pack(push, 1)
typedef struct Canvas
{
	LPCVOID         *VTable;
	HDC              FHandle;
	TCanvasState     State;
	__int8 : 8;
	__int16 : 16;
	TFont           *FFont;
	TPen            *FPen;
	TBrush          *FBrush;
	TPoint           FPenPos;
	int              FCopyMode;
	__int32 : 32;
	TNotifyEvent     FOnChange;
	TNotifyEvent     FOnChanging;
	CRITICAL_SECTION FLock;
	int              FLockCount;
	int              FTextFlags;
} TCanvas;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct ControlCanvas
{
	TCanvas   super;
	TControl *FControl;
	HDC       FDeviceContext;
	HWND      FWindowHandle;
	__int32 : 32;
} TControlCanvas;
#pragma pack(pop)

EXTERN_C void __fastcall TCanvas_FillRect(LPVOID this, const RECT *rect);
EXTERN_C void __fastcall TCanvas_Lock(LPVOID this);
EXTERN_C void __fastcall TCanvas_Unlock(LPVOID this);
EXTERN_C HDC  __fastcall TCanvas_GetHandle(LPVOID this);
EXTERN_C void __fastcall TCanvas_TextOut(LPVOID *this, int X, int Y, LPCSTR Text);
