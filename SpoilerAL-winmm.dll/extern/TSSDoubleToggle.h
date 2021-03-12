#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSDoubleToggle
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	bool            isOffset;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
	bcb6_std_vector statusVec;
	bcb6_std_string addressStr;
	bcb6_std_string indexFileName;
	bcb6_std_string dataFileName;
} TSSDoubleToggle;
#pragma pack(pop)

