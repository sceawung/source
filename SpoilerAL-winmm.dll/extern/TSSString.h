#pragma once

#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSString
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	unsigned long   size;
	WORD            codePage;// reserved for fully code page
	__int8 : 8;
	bool            cautious;
	bcb6_std_string addressStr;
	bcb6_std_string endWord;
	bcb6_std_string nowValHeadStr;
} TSSString;
#pragma pack(pop)

EXTERN_C TSSString * __cdecl new_TSSString();

