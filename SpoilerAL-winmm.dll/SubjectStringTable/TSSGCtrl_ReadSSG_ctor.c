#include <Windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "ToolTip/ToolTip.h"
#include "SubjectStringTable.h"

EXTERN_C BOOL EnableParserFix;
EXTERN_C BOOL FixTheProcedure;
EXTERN_C UINT ExtensionTSSDir;
EXTERN_C UINT MultiStageAttrs;

HANDLE hPrivateHeap = NULL;

void __fastcall TSSGCtrl_ReadSSG_attributeSelector_StartElementCheck(
	TSSGAttributeSelector *const attributeSelector,
	va_list                      fp)
{
	LPSTR lpBuffer;
	DWORD HeapInformation = 2;// Low-fragmentation Heap.
	TSSGCtrl *const SSGC = (va_arg(fp, void *), va_arg(fp, void *), va_arg(fp, TSSGCtrl *));
	string Token, *Start = vector_begin(va_arg(fp, vector_string *));
	const char *p = string_begin(Start);

	while (__intrinsic_isblank(*p)) p++;
	if (string_end(Start) - p > 8 && *(LPDWORD)p == BSWAP32('[scr') && *(LPDWORD)&p[sizeof(DWORD)] == BSWAP32('ipt]'))
	{
		vector_string flags = { NULL };
		if (TStringDivision_List(&SSGC->strD, &(string) {
			(LPSTR)p + 8, string_end(Start), ._M_end_of_storage = p + 8
		}, *string_ctor_assign_char(&Token, ','), &flags, etTRIM))
			for (string key, *it = vector_begin(&flags); it < vector_end(&flags); it++)
			{
				if (string_at(TStringDivision_Half_WithoutTokenDtor(&key, &SSGC->strD, it, "=", 1u, 0, etTRIM), 0) != '=' &&
					string_length(&key) == 3)
				{
					const unsigned long value = strtoul(string_c_str(it), NULL, 0);
					switch (*(LPDWORD)string_c_str(&key))
					{
					case BSWAP24('fix'):
						EnableParserFix = (value & 2) != 0;
						FixTheProcedure = (value & 1) != 0;
						break;
					case BSWAP24('dir'):
						ExtensionTSSDir = value;
						break;
					case BSWAP24('mul'):
						MultiStageAttrs = (value & 3) << 4;
						break;
					}
				}
				string_dtor(&key);
			}
		vector_string_dtor(&flags);
	}

	SubjectStringTable_clear();

	if ((hPrivateHeap ||
		 !(hPrivateHeap = HeapCreate(0, 0, 0)) ||
		 !HeapSetInformation(hPrivateHeap, HeapCompatibilityInformation, &HeapInformation, sizeof(HeapInformation))) &&
		FormatMessageA(
			FORMAT_MESSAGE_MAX_WIDTH_MASK * !USE_TOOLTIP |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			0,
			(LPSTR)&lpBuffer,
			sizeof(double),
			NULL)
		)
	{
#if USE_TOOLTIP
		ShowToolTip(lpBuffer, (HICON)TTI_ERROR);
#else
		if (TMainForm_GetUserMode(MainForm) != 1)
			TMainForm_Guide(lpBuffer, 0);
#endif
		LocalFree(lpBuffer);
	}

	TSSGAttributeSelector_StartElementCheck(attributeSelector);
}
