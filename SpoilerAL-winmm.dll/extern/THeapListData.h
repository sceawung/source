#pragma once

#include <windows.h>
#include "tlhelp32fix.h"

#pragma pack(push, 1)
typedef struct _THeapListData
{
	HEAPLIST32 heapList;
	DWORD      heapListAddress;
	DWORD      heapListSize;
} THeapListData;
#pragma pack(pop)

#define typename THeapListData
#include "bcb6_std_vector_template.h"

