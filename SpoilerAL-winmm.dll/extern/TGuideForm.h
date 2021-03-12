#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "TWinControl.h"

#pragma pack(push, 1)
typedef struct _TGuideForm
{
	union
	{
		LPCVOID *VTable;
		TForm    super;
	};
	TPanel         *Panel_T;
	TRichEdit      *REdit;
	TPanel         *Panel_B;
	TComboBox      *UserModeCmbBox;
	bcb6_std_string helpPath;
} TGuideForm;
#pragma pack(pop)

#define GUIDE_IS_CLEAR    0x01
#define GUIDE_IS_NOT_LINE 0x02

EXTERN_C void __cdecl TGuideForm_Guide(TGuideForm *this, const char *Mes, int Flags);
