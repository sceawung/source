#pragma once

#include <windows.h>
#include "TWinControl.h"

#pragma pack(push, 1)
typedef struct _TCalcValBox
{
	TEdit *edit;
	DWORD  frontTColor;
	DWORD  backTColor;
	BYTE   backXOffset;
	BYTE   backYOffset;
	BYTE   frontXOffset;
	BYTE   frontYOffset;
	DWORD  mode;
} TCalcValBox;
#pragma pack(pop)

#define typename TCalcValBox
#include "bcb6_std_vector_template.h"
