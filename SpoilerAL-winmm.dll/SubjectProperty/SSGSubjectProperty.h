#ifndef _SSGSUBJECTPROPERTY_H_
#define _SSGSUBJECTPROPERTY_H_

#include <windows.h>
#define EMBED_BREADTH 0
#include "TSSDir.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TSSGSubjectProperty {
	INT     OuterRepeat;
	UINT    DirectChild;
	DWORD   RepeatDepth;
	ULONG   RepeatIndex;
} TSSGSubjectProperty;

extern TSSGSubjectProperty *SubjectProperty;
extern DWORD               SubjectPropertyCount;
extern DWORD               TitleWidth;

EXTERN_C void __cdecl ClearSubjectProperty();
EXTERN_C TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex);
#define AppendSubjectProperty(SSGS) \
	GrowSubjectProperty((LPDWORD)&(SSGS)->propertyIndex)
#define GetProperty(Index) \
	(Index == MAXDWORD ? NULL : SubjectProperty + Index)
#define GetSubjectProperty(SSGS) \
	((SSGS) && (SSGS)->propertyIndex != MAXDWORD ? SubjectProperty + (SSGS)->propertyIndex : NULL)
#define GetOuterRepeat(Property) \
	(((Property)->OuterRepeat) != MAXDWORD ? SubjectProperty + (Property)->OuterRepeat : NULL)

#ifdef __cplusplus
}
#endif

#endif	// _SSGSUBJECTPROPERTY_H_
