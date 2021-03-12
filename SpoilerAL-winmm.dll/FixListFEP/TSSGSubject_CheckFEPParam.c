#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGSubject.h"
#include "TStringDivision.h"

void __fastcall TSSGSubject_CheckFEPParam(TSSGSubject *this, const string *s);

__declspec(naked) void __cdecl TSSBitList_Setting_CheckFEPParam()
{
	__asm
	{
		#define this ebx
		#define tmpV (ebp - 18H)

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, size string * 3
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam

	L1:
		rep ret

		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleList_Setting_CheckFEPParam()
{
	__asm
	{
		#define this ebx
		#define tmpV edi

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, size string * 6
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam

	L1:
		rep ret

		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Setting_CheckFEPParam()
{
	__asm
	{
		#define this ebx
		#define tmpV (ebp - 18H)

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, size string * 6
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam

	L1:
		rep ret

		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSList_Setting_CheckFEPParam()
{
	__asm
	{
		#define this ebx
		#define tmpV edi

		dec     dword ptr [esi + 1CH]
		dec     dword ptr [esi + 1CH]
		mov     edx, dword ptr [tmpV]
		mov     eax, dword ptr [tmpV + 4]
		add     edx, size string * 4
		mov     ecx, this
		cmp     edx, eax
		jae     L1
		jmp     TSSGSubject_CheckFEPParam

	L1:
		rep ret

		#undef this
		#undef tmpV
	}
}

void __fastcall TSSGSubject_CheckFEPParam(TSSGSubject *this, const string *s)
{
	char *first, *last;

	if ((last = string_end(s)) - (first = string_begin(s)) >= 3)
	{
		last = TrimSpace(&first, last);
		if (last - first == 3 && first[0] == 'f' && first[1] == 'e' && first[2] == 'p')
			this->withFEP = TRUE;
	}
}
