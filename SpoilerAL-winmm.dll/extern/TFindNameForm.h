#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TWinControl.h"
#include "TCheckBox.h"

#pragma pack(push, 1)
typedef struct _TFindNameForm
{
	union
	{
		LPCVOID *VTable;
		TForm    super;
	};
	TPanel         *Panel_T;
	TEdit          *FindEdit;
	TButton        *FindStopBtn;
	TListBox       *FindLBox;
	TButton        *FindStartBtn;
	TRadioGroup    *FindRGroup;
	TCheckBox      *IsWordConvertCBox;
	LCID            findLCID;
	long            findMode;
	__int32 : 32;
	bcb6_std_vector findIndexVec;
	bcb6_std_vector findSubjectVec;
	bool            isWordConvert;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
} TFindNameForm;
#pragma pack(pop)

EXTERN_C bcb6_std_string * __cdecl TFindNameForm_CnvString(bcb6_std_string *Result, TFindNameForm *this, const bcb6_std_string *Src);
