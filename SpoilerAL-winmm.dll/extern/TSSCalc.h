#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSCalc
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	BYTE            size;
	__int8 : 8;
	__int16 : 16;
	long            min;
	long            max;
	bool            isUnsigned;
	bool            isBigEndian;
	__int16 : 16;
	bcb6_std_string addressStr;
	bcb6_std_string nowValHeadStr;
	bcb6_std_string nowValFootStr;
} TSSCalc;
#pragma pack(pop)

EXTERN_C TSSCalc * __cdecl new_TSSCalc();

