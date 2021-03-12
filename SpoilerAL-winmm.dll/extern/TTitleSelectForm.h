#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "TDrawGrid.h"
#include "TSSGScriptStruct.h"
#include "TCheckBox.h"

#pragma pack(push, 1)
typedef struct _TTitleSelectForm
{
	union
	{
		LPCVOID *VTable;
		TForm    super;
	};
	TPanel           *Panel_T;
	TPanel           *Panel_C;
	TSplitter        *Splitter1;
	TPanel           *Panel_TR;
	TButton          *CancelBtn;
	TButton          *OKBtn;
	TPanel           *Panel_B;
	TSplitter        *Splitter2;
	TPanel           *Panel_TC;
	TComboBox        *SortCmbBox;
	TLabel           *Label3;
	TLabel           *Label2;
	TPanel           *Panel_BT;
	TButton          *FindUDBtn;
	TLabel           *Label1;
	TEdit            *FindEdit;
	TListBox         *FindLBox;
	TButton          *FindStopBtn;
	TDrawGrid        *TitleDGrid;
	TDrawGrid        *KeyDGrid;
	TButton          *BegginerBtn;
	TPanel           *Panel_BB;
	TEdit            *SSGPathEdit;
	TButton          *SSGPathBtn;
	bcb6_std_string   sortKey;
	LCID              sortLCID;
	bcb6_std_vector  *keyToIndexMap;
	bcb6_std_vector  *scriptVec;
	bcb6_std_vector  *toScriptVec;
	long              findMode;
	__int32 : 32;	 
	bcb6_std_vector   findIndexVec;
	TSSGScriptStruct *nowOnMouseScript;
	TSSGScriptStruct *selectScript;
} TTitleSelectForm;
#pragma pack(pop)

EXTERN_C bcb6_std_string * __cdecl TTitleSelectForm_CnvString(bcb6_std_string *Result, TTitleSelectForm *this, const bcb6_std_string *Src);
EXTERN_C BOOLEAN __cdecl TTitleSelectForm_SortFunc(TTitleSelectForm *this, const TSSGScriptStruct *SS1, const TSSGScriptStruct *SS2);
