#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"

#pragma pack(push, 1)
typedef struct _TSSGScriptStruct
{
	bcb6_std_map    distinctionMap;
	bcb6_std_string filePath;
	unsigned long   sPos;
	unsigned long   ePos;
} TSSGScriptStruct;
#pragma pack(pop)

EXTERN_C long __stdcall TSSGScriptStruct_GetTitleTextWidth(const TSSGScriptStruct *this, HDC hDC);

EXTERN_C void __cdecl TSSGScriptStruct_SetDistinction_cstr_with_length(const TSSGScriptStruct *this, const char *ID, const char *Val, size_t ValLength);
__inline void TSSGScriptStruct_SetDistinction_stdstr(const TSSGScriptStruct *this, const char *ID, const bcb6_std_string *Val) { TSSGScriptStruct_SetDistinction_cstr_with_length(this, ID, bcb6_std_string_c_str(Val), bcb6_std_string_length(Val)); }
__inline void TSSGScriptStruct_SetDistinction(const TSSGScriptStruct *this, const char *ID, const char *Val) { TSSGScriptStruct_SetDistinction_cstr_with_length(this, ID, Val, strlen(Val)); }

EXTERN_C bcb6_std_string *(__cdecl * const TSSGScriptStruct_GetDistinction)(bcb6_std_string *Result, const TSSGScriptStruct *this, const char *ID);

#define TSSGScriptStruct_GetTitle(Result, this) TSSGScriptStruct_GetDistinction(Result, this, /* "title" */(LPCSTR)0x006030FF)

