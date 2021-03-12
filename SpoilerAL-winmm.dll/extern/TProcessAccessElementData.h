#pragma once

#include "bcb6_std_vector.h"
#include "TProcessAccessElementBase.h"

typedef enum Comparative
{
	ctEQUOL       = 0,
	ctNOT         = 1,
	ctLOWER       = 2,
	ctUPPER       = 3,
	ctLOWER_EQUOL = 4,
	ctUPPER_EQUOL = 5,
} CmpType;

typedef enum Valuation
{
	voNONE           = 0,
	voREVERSE_ENDIAN = 1,
} ValueOpt;

#pragma pack(push, 1)
typedef struct _TProcessAccessElementDataBase {
	TProcessAccessElementBase super;
	CmpType                   compareType;
	ValueOpt                  valueOption;
} TProcessAccessElementDataBase;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _TProcessAccessElementData {
	union {
		struct {
			LPVOID *VTable;
			AccType type;
		};
		TProcessAccessElementDataBase super;
	};
	bcb6_std_vector_byte data;
} TProcessAccessElementData;
#pragma pack(pop)

#define TProcessAccessElementData_GetData(PAED) &(PAED)->data

#pragma pack(push, 1)
typedef struct _TProcessAccessElementMaskData {
	union {
		struct {
			LPVOID *VTable;
			AccType type;
		};// Real super class is TProcessAccessElementData
		TProcessAccessElementDataBase super;
	};
	bcb6_std_vector_byte data;
	bcb6_std_vector_byte mask;
} TProcessAccessElementMaskData;
#pragma pack(pop)

#define TProcessAccessElementMaskData_GetMask(PAEM) &(PAEM)->mask
