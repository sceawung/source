#pragma once

#include <windows.h>
#include "bcb6_std_list.h"
#include "bcb6_std_vector.h"
#include "bcb6_std_set.h"

#pragma pack(push, 1)
typedef struct _TSSGAttributeSelector
{
	union {
		struct {
			bcb6_std_list   allAtteributeVecList;
			bcb6_std_list   allAtteributeList;
			bcb6_std_map    *attributeSetMap;
		};
		struct
		{
			__int32 : 32;
			bcb6_std_set    AESet;
		};
	};
	bcb6_std_list   *nowAttributeList;
	bcb6_std_vector *nowAttributeVec;
	bcb6_std_map    *stackElemMap;
} TSSGAttributeSelector;
#pragma pack(pop)

#define TSSGAttributeSelector_GetNowAtteributeVec(/*IN TSSGAttributeSelector * */this) \
	(this)->nowAttributeVec

EXTERN_C void(__cdecl * const TSSGAttributeSelector_StartElementCheck)(TSSGAttributeSelector *this);
EXTERN_C void(__cdecl * const TSSGAttributeSelector_EndElementCheck)(TSSGAttributeSelector *this);
EXTERN_C void(__cdecl * const TSSGAttributeSelector_PushStack)(TSSGAttributeSelector *this, LPVOID AElem);
EXTERN_C bcb6_std_vector *(__cdecl * const TSSGAttributeSelector_AddElement)(TSSGAttributeSelector *this, LPVOID element);

EXTERN_C void __cdecl TSSGAttributeSelector_EraseElement(TSSGAttributeSelector *this, LPVOID element);

EXTERN_C bcb6_std_vector *(__cdecl * const TSSGAttributeSelector_PushElement)(TSSGAttributeSelector *this, LPVOID element);
EXTERN_C void(__cdecl * const TSSGAttributeSelector_PopElement)(TSSGAttributeSelector *this, LPVOID element);

EXTERN_C void __cdecl TSSGAttributeSelector_PopElementByType(TSSGAttributeSelector *this, unsigned long type);
EXTERN_C void __cdecl TSSGAttributeSelector_EraseElementByType(TSSGAttributeSelector *this, unsigned long type);
EXTERN_C void __cdecl TSSGAttributeSelector_ToRootElementByType(TSSGAttributeSelector *this, unsigned long type);

EXTERN_C void(__cdecl * const TSSGAttributeSelector_ToRootElement)(TSSGAttributeSelector *this, LPVOID element);
EXTERN_C bcb6_std_vector *(__cdecl * const TSSGAttributeSelector_MakeNowAttributeVec)(TSSGAttributeSelector *this);

