#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SSGSubjectProperty.h"

extern DWORD RepeatDepth;
extern TSSGSubject dummySSGS;

void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	stack        *ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        OuterRepeat,
	TSSGSubject  *SSGS)
{
	vector_string       tmpV;
	vector_dword        indices;
	TSSGSubjectProperty *prop, *erty;// Don't get property at here because will be moved by GrowSubjectProperty

	vector_ctor(&indices);
	dummySSGS.folder = *stack_top(ParentStack, TSSDir *);
	dummySSGS.propertyIndex = OuterRepeat;
	TSSGCtrl_ReadSSRFile(&tmpV, this, LineS, &indices, SSGS);
	dummySSGS.propertyIndex = MAXDWORD;
	dummySSGS.folder = NULL;
	if (!vector_empty(&tmpV))
	{
		if (vector_size(&indices) > 1)
		{
			DWORD   outer;
			string  *it = vector_begin(&tmpV);
			LPDWORD repeat = vector_begin(&indices);
			size_t  const elementSize = *(repeat++);

			assert(vector_size(&tmpV) == (vector_size(&indices) - 1) * elementSize);
			RepeatDepth++;
			do
			{
				vector_string constElem = { it, it, ._M_end_of_storage = it };
				DWORD const repeatIndex = *(repeat++);

				if (erty = GrowSubjectProperty(&outer))
				{
					erty->OuterRepeat = OuterRepeat;
					erty->DirectChild = 1;
					erty->RepeatDepth = RepeatDepth;
					erty->RepeatIndex = repeatIndex;
				}
				vector_end(&constElem) = it += elementSize;
				TSSGCtrl_EnumReadSSG(this, &constElem, ParentStack, ADJElem, repeatIndex, outer);
				if ((erty = GetProperty(outer)) && --erty->DirectChild && (prop = GetProperty(OuterRepeat)))
					prop->DirectChild++;
			}
			while (it < vector_end(&tmpV));
			RepeatDepth--;
		}
		else
			TSSGCtrl_EnumReadSSG(this, &tmpV, ParentStack, ADJElem, RepeatIndex, OuterRepeat);
	}
	vector_dtor(&indices);
	vector_string_dtor(&tmpV);
}

#if 0
__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReadSSRFile()
{
	static TSSGSubject SSGS = { TSSGSubject_VTable, 0 };
	__asm
	{
		#define ReturnAddress   004EB508H
		#define this            (ebp +   8H)
		#define ParentStack     (ebp +  10H)
		#define ADJElem         (ebp +  14H)
		#define RepeatIndex     (ebp +  18H)
		#define OuterRepeat     (ebp +  1CH)
		#define LineS           (ebp -  38H)

		mov     edx, dword ptr [OuterRepeat]
		mov     ecx, dword ptr [RepeatIndex]
		push    offset SSGS
		lea     eax, [LineS]
		push    edx
		push    ecx
		push    eax
		mov     edx, dword ptr [ADJElem]
		mov     ecx, dword ptr [ParentStack]
		mov     eax, dword ptr [this]
		push    edx
		push    ecx
		push    eax
		push    ReturnAddress
		jmp     repeat_ReadSSRFile

		#undef ReturnAddress
		#undef this
		#undef ParentStack
		#undef ADJElem
		#undef RepeatIndex
		#undef OuterRepeat
		#undef LineS
	}
}
#endif
