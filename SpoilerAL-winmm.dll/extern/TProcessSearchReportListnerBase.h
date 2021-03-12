#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "TWinControl.h"

#pragma pack(push, 1)
typedef struct ProcessSearchReportListnerBase
{
	LPCVOID              *VTable;
	unsigned long         reserveMode;
	TForm                *thisPointer;
	unsigned long         reserveVal;
	unsigned long         reportSpan;
	unsigned long         start, min, max, step;
	__int32 : 32;
	bcb6_std_vector_dword aeVec;
	HANDLE                processHandle;

} TProcessSearchReportListnerBase;
#pragma pack(pop)
