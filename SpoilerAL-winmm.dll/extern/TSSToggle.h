#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSToggle
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	bcb6_std_string addressStr;
	bcb6_std_string onCode;
	bcb6_std_string offCode;
	bcb6_std_string nowValHeadStr;
	unsigned long   size;
	__int32 : 32;
} TSSToggle;
#pragma pack(pop)

EXTERN_C TSSToggle * __cdecl new_TSSToggle();

