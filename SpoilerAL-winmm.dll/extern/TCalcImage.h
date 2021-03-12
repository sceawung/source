#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "TSSArg.h"
#include "TControl.h"

typedef struct CalcButtonPushFunc
{
	void(__cdecl *Method)(long BtnNum);
	void   const *This;
} TCalcButtonPushFunc;

#include "TCalcValBox.h"

#pragma pack(push, 1)
typedef struct _TCalcButton
{
	RECT hitRect;       // ボタン矩形
	RECT exclusionRect; // 除外矩形
	BYTE status;        // 押／上トグル形式のボタンは、状態を記録
	__int8 : 8;
	__int16 : 16;
} TCalcButton;
#pragma pack(pop)

#define typename TCalcButton
#include "bcb6_std_vector_template.h"

#pragma pack(push, 1)
typedef struct _TCalcImage
{
	LPVOID                     *VTable;
	BYTE                        padding_TImage[380];
	int                         mouseX;
	int                         mouseY;
	TSSArg                     *naturalVal;
	TBitmap                    *pushGpc;
	TBitmap                    *freeGpc;
	TBitmap                    *sprite;
	TBitmap                    *drawImage;
	__int32 : 32;
	bcb6_std_vector_TCalcButton btnVec;
	bcb6_std_vector_TCalcValBox valBox;
	bool                        isPush;
	__int8 : 8;
	__int16 : 16;
	long                        mouseNowBtn;
	TCalcButtonPushFunc         calcButtonPushFunc;
} TCalcImage;
#pragma pack(pop)

EXTERN_C void(__cdecl * const TCalcImage_SetStatus)(TCalcImage *this, long Index, BYTE Status);
EXTERN_C BYTE(__cdecl * const TCalcImage_GetStatus)(TCalcImage *this, long Index);

void __fastcall TCalcImage_DrawBtn(TCalcImage *this, long No, BOOLEAN IsPress);

EXTERN_C void __fastcall _TCalcImage_FOnMouseUp(TCalcImage *this, LPVOID Sender, int Button, int Y, int X, WORD Shift);
#define TCalcImage_FOnMouseUp(this, Sender, Button, Shift, X, Y) _TCalcImage_FOnMouseUp(this, Sender, Button, Y, X, Shift)

EXTERN_C void __fastcall _TCalcImage_FOnMouseDown(TCalcImage *this, LPVOID Sender, int Button, int Y, int X, WORD Shift);
#define TCalcImage_FOnMouseDown(this, Sender, Button, Shift, X, Y) _TCalcImage_FOnMouseDown(this, Sender, Button, Y, X, Shift)
