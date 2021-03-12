#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"
#include "SSGSubjectProperty.h"
#include "TMainForm.h"

extern HANDLE hHeap;

TSSGSubjectProperty *SubjectProperty = NULL;
DWORD               SubjectPropertyCount = 0;
DWORD static        Allocation;
DWORD               TitleWidth = 0;

void __cdecl ClearSubjectProperty()
{
	SubjectPropertyCount = 0;
	if (SubjectProperty != NULL)
	{
		HeapFree(hHeap, 0, SubjectProperty);
		SubjectProperty = NULL;
	}
	TitleWidth = 0;
}

__declspec(noinline) TSSGSubjectProperty * __fastcall GrowSubjectProperty(DWORD *lpdwIndex)
{
	TSSGSubjectProperty *prop;
	DWORD index;

	do	/* do { ... } while (0); */
	{
		if (SubjectPropertyCount)
		{
			if (MainForm->ssgCtrl.script.sPos)
			{
				DWORD i = SubjectPropertyCount;
				do if (!SubjectProperty[--i].DirectChild)
				{
					 prop = &SubjectProperty[index = i];
					*prop = (const TSSGSubjectProperty) { .OuterRepeat = MAXDWORD };
					goto RESOLVED;
				}
				while (i);
			}
			if (SubjectPropertyCount < Allocation)
				prop = SubjectProperty;
			else
			{
				prop = (TSSGSubjectProperty *)HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, SubjectProperty, sizeof(TSSGSubjectProperty) * (Allocation << 1));
				if (prop == NULL)
					break;
				SubjectProperty = prop;
				Allocation <<= 1;
			}
		}
		else
		{
			prop = (TSSGSubjectProperty *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(TSSGSubjectProperty) * (Allocation = 0x10));
			if (prop == NULL)
				break;
			SubjectProperty = prop;
		}
		prop += index = SubjectPropertyCount++;
		prop->OuterRepeat = MAXDWORD;
	} while (0);
RESOLVED:
	*lpdwIndex = prop == NULL ? MAXDWORD : index;
	return prop;
}
