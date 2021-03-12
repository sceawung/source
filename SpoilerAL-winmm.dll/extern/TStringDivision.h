#pragma once

#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "bcb6_std_map.h"
#include "bcb6_std_set.h"

#ifndef __BORLANDC__
#define __msreturn
#define __msfastcall __fastcall
#endif

typedef enum Dealt
{
	dtNEST      = 0x00000001,
	dtESCAPE    = 0x00000002,
	etTRIM_L    = 0x00000004,
	etTRIM_R    = 0x00000008,
	etREPLACE   = 0x00000010,
	etREVERSE_REPLACE = 0x20,
	dtBYTEARRAY = 0x00000040,
} DealType;

#define etTRIM      (etTRIM_L | etTRIM_R )
#define etSOME_EDIT (etTRIM   | etREPLACE)

#pragma pack(push, 1)
typedef struct _TStringDivision
{
	LPVOID          *VTable;
	bcb6_std_string nestStartTag;
	bcb6_std_string nestEndTag;
	bcb6_std_string escapeTag;
	bcb6_std_map    replaceMap;
	bcb6_std_set    trimSet;
	__int32 : 32;

} TStringDivision;
#pragma pack(pop)

EXTERN_C unsigned long __stdcall TStringDivision_Find_WithoutTokenDtor(
	IN     TStringDivision       *this,
	IN     const bcb6_std_string *Src,
	IN     const char            *Token,
	IN     size_t                TokenLength,
	IN     unsigned long         FromIndex,
	IN     unsigned long         ToIndex,
	IN     unsigned long         Option);

EXTERN_C unsigned long __cdecl TStringDivision_Find(
	IN     TStringDivision       *this,
	IN     const bcb6_std_string *Src,
	IN     bcb6_std_string       Token,
	IN     unsigned long         FromIndex,
	IN     unsigned long         ToIndex,
	IN     unsigned long         Option);

EXTERN_C unsigned long __cdecl TStringDivision_FindBack(
	IN     TStringDivision       *this,
	IN     const bcb6_std_string *Src,
	IN     bcb6_std_string       Token,
	IN     unsigned long         FromIndex,
	IN     unsigned long         ToIndex,
	IN     unsigned long         Option);

EXTERN_C unsigned long __cdecl TStringDivision_FindByVector(
	IN     TStringDivision              *this,
	IN     const bcb6_std_string        *Src,
	IN     const bcb6_std_vector_string *Token,
	OUT    unsigned long                *FindElementIndex,
	IN     unsigned long                FromIndex,
	IN     unsigned long                ToIndex,
	IN     unsigned long                Option);

EXTERN_C bcb6_std_string * __stdcall TStringDivision_Half_WithoutTokenDtor(
	OUT    bcb6_std_string *Result,
	IN     TStringDivision *this,
	IN OUT bcb6_std_string *Src,
	IN     const char      *Token,
	IN     size_t          TokenLength,
	IN     unsigned long   Index,
	IN     unsigned long   Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Half(
	OUT    bcb6_std_string *Result,
	IN     TStringDivision *this,
	IN OUT bcb6_std_string *Src,
	IN     bcb6_std_string Token,
	IN     unsigned long   Index,
	IN     unsigned long   Option);

EXTERN_C unsigned long __cdecl TStringDivision_List(
	IN     TStringDivision        *this,
	IN     const bcb6_std_string  *Src,
	IN     bcb6_std_string        Token,
	OUT    bcb6_std_vector_string *List,
	IN     unsigned long          Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Editing(
	IN     bcb6_std_string       *Result,
	IN     TStringDivision       *this,
	IN     const bcb6_std_string *Src,
	IN     unsigned long         Option);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Remove(
	OUT    bcb6_std_string       *Result,
	IN     TStringDivision       *Reserved1,
	IN     const bcb6_std_string *Src,
	IN     bcb6_std_string       Token,
	IN     const char            *Dest,
	IN     unsigned long         Reserved2);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_RemoveByMap(
	OUT    bcb6_std_string       *Result,
	IN     TStringDivision       *this,
	IN     const bcb6_std_string *Src,
	IN     const bcb6_std_map    *ReplaceMap,
	IN     unsigned long         Reserved);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_TrimDefault(
	OUT    bcb6_std_string       *Result,
	IN     TStringDivision       *Reserved1,
	IN     const bcb6_std_string *Src,
	IN     const void            *Reserved2,
	IN     unsigned long         Reserved3);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_TrimFull(
	OUT    bcb6_std_string       *Result,
	IN     TStringDivision       *Reserved1,
	IN     const bcb6_std_string *Src,
	IN     const void            *Reserved2,
	IN     unsigned long         Reserved3);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_Trim(
	OUT    bcb6_std_string       *Result,
	IN     TStringDivision       *Reserved,
	IN     const bcb6_std_string *Src,
	IN     const bcb6_std_set    *TrimSet,
	IN     unsigned long         Option);

EXTERN_C char * __msfastcall TrimLeftSpace(const char *first);
EXTERN_C char * __msfastcall TrimRightSpace(const char *first, const char *last);
EXTERN_C unsigned __int64 __msreturn __msfastcall __reg64return_TrimSpace(const char *first, const char *last);
static __forceinline char *TrimSpace(char **pfirst, const char *last)
{
	unsigned __int64 result;

	result = __reg64return_TrimSpace(*pfirst, last);
	*pfirst = (char *)result;
	return (char *)(result >> 32);
}
EXTERN_C char * __msfastcall TrimLeftBlank(const char *first);
EXTERN_C char * __msfastcall TrimRightBlank(const char *first, const char *last);
EXTERN_C unsigned __int64 __msreturn __msfastcall __reg64return_TrimBlank(const char *first, const char *last);
static __forceinline char *TrimBlank(char **pfirst, const char *last)
{
	unsigned __int64 result;

	result = __reg64return_TrimBlank(*pfirst, last);
	*pfirst = (char *)result;
	return (char *)(result >> 32);
}

EXTERN_C bcb6_std_string * __cdecl TStringDivision_ToString(
	OUT    bcb6_std_string *Result,
	IN     unsigned long   Src,
	IN     const char      *Format);

EXTERN_C bcb6_std_string * __cdecl TStringDivision_ToStringDouble(
	OUT    bcb6_std_string *Result,
	IN     double          Src,
	IN     const char      *Format);

EXTERN_C BOOLEAN __cdecl TStringDivision_WCList(
	IN     TStringDivision        *this,
	IN     const bcb6_std_string  *Val1,
	IN     const bcb6_std_string  *Val2,
	OUT    bcb6_std_vector_string *List1,
	OUT    bcb6_std_vector_string *List2);

EXTERN_C long(__cdecl * const TStringDivision_ToLongDef)(const bcb6_std_string* Src, long Default);
EXTERN_C unsigned long(__cdecl * const TStringDivision_ToULongDef)(const bcb6_std_string* Src, unsigned long Default);

