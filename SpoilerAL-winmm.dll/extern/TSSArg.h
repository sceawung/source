#pragma once

#include <windows.h>
#include "xx.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"

typedef enum Argument
{
	atUNKNOWN,
	atLONG,
	atLONG_INDEX,
	atBOOL,
	atSTRING,
	atBOOL_VECTOR,
	atDOUBLE,
	atNONE,
	atDIR,
	atSPLIT
} ArgType;

typedef struct _TSSArgVTbl
{
	void(__cdecl *const dtor)(void *this, enum flagsClean);
	void(__cdecl *const SetValue)(void *this, const char *Val);
	bcb6_std_string *(__cdecl *const ToString)(bcb6_std_string *, void *this);
} TSSArgVTbl;

#pragma pack(push, 1)
typedef struct _TSSArg
{
	TSSArgVTbl   *VTable;
	ArgType       type;
} TSSArg;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgLong
{
	TSSArgVTbl   *VTable;
	ArgType       type;
	long          value;
} TSSArgLong, TSSArgLongIndex;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgBool
{
	TSSArgVTbl   *VTable;
	ArgType       type;
	bool          value;
} TSSArgBool;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgString
{
	TSSArgVTbl     *VTable;
	ArgType         type;
	bcb6_std_string value;
} TSSArgString;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgBoolVector{
	TSSArgVTbl      *VTable;
	ArgType          type;
	bcb6_std_bvector value;
} TSSArgBoolVector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TSSArgDouble
{
	TSSArgVTbl   *VTable;
	ArgType       type;
	double        value;
} TSSArgDouble;
#pragma pack(pop)

#define delete_TSSArg(Arg)        (Arg)->VTable->dtor(Arg, DTCVF_PTRVAL | DTCVF_DELPTR)
#define TSSArg_ToString(ret, Arg) (Arg)->VTable->ToString(ret, Arg)

