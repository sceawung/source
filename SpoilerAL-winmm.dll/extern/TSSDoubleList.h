#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSDoubleList
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	bool            isOffset;
	BYTE            size;
	__int16 : 16;
	__int32 : 32;
	bcb6_std_vector statusVec;
	bcb6_std_string addressStr;
	bcb6_std_string indexFileName;
	bcb6_std_string dataFileName;
} TSSDoubleList;
#pragma pack(pop)

