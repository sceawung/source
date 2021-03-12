#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSFloatCalc
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	BYTE            size;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
	double          min;
	double          max;
	bool            isBigEndian;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
	bcb6_std_string addressStr;
	bcb6_std_string nowValHeadStr;
	bcb6_std_string nowValFootStr;
} TSSFloatCalc;
#pragma pack(pop)

