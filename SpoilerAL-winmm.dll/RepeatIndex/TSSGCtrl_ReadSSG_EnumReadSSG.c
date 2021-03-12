#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"

extern DWORD  RepeatDepth;
extern BOOL   IsProcessAttached;
extern string ProcessAttachCode;
extern vector *ProcessAttachAttribute;
extern string ProcessDetachCode;
extern vector *ProcessDetachAttribute;
extern TSSGSubject dummySSGS;

void __cdecl TSSGCtrl_ReadSSG_EnumReadSSG(TSSGCtrl *this, vector_string *SSGFile, stack_ptr *ParentStack, TDialogAdjustmentAttribute *ADJElem)
{
	RepeatDepth = 0;
	IsProcessAttached = FALSE;
	string_ctor_null(&ProcessAttachCode);
	ProcessAttachAttribute = NULL;
	string_ctor_null(&ProcessDetachCode);
	ProcessDetachAttribute = NULL;
	TSSGCtrl_EnumReadSSG(this, SSGFile, ParentStack, NULL, 0, MAXDWORD);
}
