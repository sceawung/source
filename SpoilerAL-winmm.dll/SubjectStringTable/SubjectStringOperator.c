#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "SubjectStringTable.h"
#include "TStringDivision.h"
#include "TSSGSubject.h"
#include "TSSString.h"
#include "TSSGCtrl.h"
#include "TMainForm.h"

void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s);
void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

#define array SubjectStringTable_array

void __cdecl SubjectStringTable_StringCtor(string *s)
{
	*s = (const string) { NULL };
}

const string * __fastcall SubjectStringTable_GetString(const string *s)
{
	return &vector_at(&array, s->sstIndex);
}

void __fastcall SubjectStringTable_SetString(string *dest, string *src)
{
	dest->sstIndex = SubjectStringTable_insert(src);
}

static void __fastcall SetName(TSSGSubject *SSGS, string *s)
{
	SSGS->name.sstIndex = SubjectStringTable_insert(s);
}

static void __fastcall SetCode(TSSGSubject *SSGS, string *s)
{
	SSGS->code.sstIndex = SubjectStringTable_insert(s);
}

__declspec(naked)
static void __fastcall TMainForm_FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS
#ifdef _M_IX86
												  , string *retVal, const string* nowValHeadStr
#endif
)
{
	__asm
	{
		mov     eax,      [esp + 8]// nowValHeadStr
		push    dword ptr [esp + 4]// retVal
		push    edx
		push    ecx
		mov     ecx, eax
		call    SubjectStringTable_GetString
		mov     edx, eax
		mov     ecx, dword ptr [esp + 8]
		call    string_ctor_assign
		call    FormatNameString
		ret     0// don't discard stack
	}
}

__declspec(naked) void __cdecl TMainForm_SubjectAccess_TSSToggle_GetNowValHeadStr()
{
	__asm
	{
		#define this ebx
		#define SSGS (ebp - 2BCH)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [this]TMainForm.ssgCtrl
		jmp     TMainForm_FormatNameString

		#undef this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_SubjectAccess_TSSString_GetNowValHeadStr()
{
	__asm
	{
		#define this ebx
		#define SSGS (ebp - 2FCH)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [this]TMainForm.ssgCtrl
		jmp     TMainForm_FormatNameString

		#undef this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_StringEnterBtnClick_TSSString_GetNowValHeadStr()
{
	__asm
	{
		#define this esi
		#define SSGS edi

		mov     edx, SSGS
		lea     ecx, [this]TMainForm.ssgCtrl
		jmp     TMainForm_FormatNameString

		#undef this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_SetCalcNowValue_TSSCalc_GetNowValHeadStr()
{
	__asm
	{
		#define this esi
		#define SSGS (ebp - 3B0H)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [this]TMainForm.ssgCtrl
		jmp     TMainForm_FormatNameString

		#undef this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TMainForm_SetCalcNowValue_TSSFloatCalc_GetNowValHeadStr()
{
	__asm
	{
		#define this esi
		#define SSGS (ebp - 47CH)

		mov     edx, dword ptr [SSGS]
		lea     ecx, [this]TMainForm.ssgCtrl
		jmp     TMainForm_FormatNameString

		#undef this
		#undef SSGS
	}
}

static void __fastcall ModifySplit(
	TMainForm    *const this,
	TSSGSubject  *const SSGS,
	string       *const dest,
	string const *const src)
{
	extern BOOL EnableParserFix;

	if (EnableParserFix && string_empty(src))
		TSSGSubject_GetSubjectName(dest, SSGS, &this->ssgCtrl);
	else
		FormatNameString(&this->ssgCtrl, SSGS, string_ctor_assign(dest, src));
}

__declspec(naked) void __stdcall TMainForm_DrawTreeCell_GetStrParam(string *const tmpS, string const *strParam)
{
	__asm
	{
		#define this  ebx
		#define SSGS (ebp - 0x01E0)

		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 8], eax
		mov     edx, dword ptr [SSGS]
		mov     ecx, this
		jmp     ModifySplit

		#undef this
		#undef SSGS
	}
}

__declspec(naked) void __cdecl TFindNameForm_EnumSubjectNameFind_GetName()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     edx, eax
		mov     ecx, dword ptr [esp + 4]
		jmp     string_ctor_assign
	}
}

__declspec(naked) void __cdecl TSSBitList_Setting_GetCode()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 12]
		pop     eax
		push    ecx
		push    eax
		mov     ecx, dword ptr [esp + 12]
		call    SubjectStringTable_GetString
		mov     edx, eax
		mov     ecx, dword ptr [esp +  8]
		jmp     string_concat
	}
}

__declspec(naked) void __cdecl TSSBitList_Setting_GetName()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 8]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 8], eax
		jmp     TStringDivision_List
	}
}

__declspec(naked) void __cdecl TSSBitList_Read_GetAddressStr()
{
	__asm
	{
		mov     ecx, dword ptr [esp + 12]
		call    SubjectStringTable_GetString
		mov     dword ptr [esp + 12], eax
		jmp     dword ptr [TSSGCtrl_GetAddress]
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCodeAndName()
{
	__asm
	{
		#define SSGS (ebp - 7C8H)
		#define Code (ebp - 110H)
		#define Name (ebp - 128H)

		mov     ecx, dword ptr [SSGS]
		lea     edx, [Code]
		call    SetCode
		mov     ecx, dword ptr [SSGS]
		lea     edx, [Name]
		call    SetName
		ret

		#undef SSGS
		#undef Code
		#undef Name
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetCode()
{
	__asm
	{
		#define SSGS (ebp - 0EF4H)
		#define Src  eax

		mov     edx, Src
		mov     ecx, dword ptr [SSGS]
		jmp     SetCode

		#undef SSGS
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_SetName()
{
	__asm
	{
		#define SSGS (ebp - 0EF4H)
		#define Src  eax

		mov     edx, Src
		mov     ecx, dword ptr [SSGS]
		jmp     SetName

		#undef SSGS
		#undef Src
	}
}

__declspec(naked) void __cdecl TSSGCtrl_MakeADJFile_GetAddressStr()
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]
		mov     eax, dword ptr [eax]
		jmp     dword ptr [eax + 8 * 4]
	}
}

static __inline void TSSString_Setting_CheckCodePage(TSSString *this, string *s)
{
	this->codePage = CP_ACP;// must be 0
	assert(!this->codePage);
	switch (string_length(s))
	{
	case 4:
		if (*(LPDWORD)string_begin(s) == BSWAP32('utf8'))
		{
			this->codePage = CP_UTF8;
			*(LPWORD )string_begin(s) = '00';
			*(string_end(s) = string_begin(s) + 2) = '\0';
		}
		break;
	case 7:
		if (*(LPDWORD)string_begin(s) == BSWAP32('unic') && *(LPDWORD)&string_at(s, sizeof(DWORD)) == BSWAP32('ode\0'))
		{
			this->codePage = MAXWORD;
			*(LPDWORD)string_begin(s) = '0000';
			*(string_end(s) = string_begin(s) + 4) = '\0';
			this->size &= -(signed)sizeof(wchar_t);
		}
		break;
	}
}

void __fastcall TSSString_Setting_SetEndWord(TSSString *this, string *s)
{
	TSSString_Setting_CheckCodePage(this, s);
	SubjectStringTable_SetString(&this->endWord, s);
}

__declspec(naked) void __cdecl TSSString_Read_GetEndWord()
{
	__asm
	{
		#define this ebx

		lea     ecx, [this]TSSString.endWord
		jmp     SubjectStringTable_GetString

		#undef this
	}
}

__declspec(naked) void __cdecl TSSString_Write_GetEndWord()
{
	__asm
	{
		#define this (ebp + 8H)

		mov     ecx, dword ptr [this]
		lea     ecx, [ecx]TSSString.endWord
		jmp     SubjectStringTable_GetString

		#undef this
	}
}

__declspec(naked) void __cdecl TSSToggle_Read_GetOnCode1()
{
	__asm
	{
		#define this ebx

		lea     ecx, [this + 90H]
		jmp     SubjectStringTable_GetString

		#undef this
	}
}

__declspec(naked) void __cdecl TSSToggle_Read_GetOffCode()
{
	__asm
	{
		#define this ebx

		lea     ecx, [this + 0A8H]
		jmp     SubjectStringTable_GetString

		#undef this
	}
}

__declspec(naked) void __cdecl TSSTrace_Write_GetFileName()
{
	__asm
	{
		#define this ebx

		lea     ecx, [this + 78H]
		call    SubjectStringTable_GetString
		mov     ecx, eax
		pop     eax
		push    1
		jmp     eax

		#undef this
	}
}

__declspec(naked) void __cdecl TSSGSubject_GetSubjectName_GetSubjectName()
{
	__asm
	{
		#define this ebx

		lea     ecx, [this]TSSGSubject.subjectName
		call    SubjectStringTable_GetString
		pop     ecx
		push    eax
		push    ebx
		jmp     ecx

		#undef this
	}
}
