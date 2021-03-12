#include "xx.h"
#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSGScriptStruct.h"
#include "bcb6_operator.h"
#include "bcb6_std_string.h"

#pragma function(memcmp)

#pragma warning(disable:4733)

string *(__cdecl * const TSSGScriptStruct_GetDistinction)(string *Result, const TSSGScriptStruct *this, const char *ID) = (LPVOID)0x00446158;

__declspec(naked) void __cdecl TSSGScriptStruct_SetDistinction_cstr_with_length(const TSSGScriptStruct *this, const char *ID, const char *Val, size_t ValLength)
{
	extern const DWORD F00415878;
	extern const DWORD F004167F8;

	static const DWORD data1[] = {
		0x00402634,
		0x00000004, -8,
		0x004024F8,
		0x00000004, -32,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x004024F8,
		0x00000004, -56,
		0x00416304,
		0x00000004, -104,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00000000, -140,
		0x00050000, 0x00000000,
		(DWORD)data1,
		0x00050008, 0x00000005,
		(DWORD)data2,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
	};

	__asm
	{
		#define this      (ebp + 8H)
		#define ID        (ebp + 0CH)
		#define Val       (ebp + 10H)
		#define ValLength (ebp + 14H)
		#define s1        (ebp - 20H)
		#define s2        (ebp - 38H)
		#define s3        (ebp - 50H)
		#define s4        (ebp - 68H)

		push    ebp
		mov     eax, offset data3
		mov     ebp, esp
		sub     esp, 232
		push    ebx
		push    esi
		mov     ebx, dword ptr [this]
		call    __InitExceptBlockLDTC
		mov     edx, dword ptr [ID]
		lea     ecx, [s1]
		call    string_ctor_assign_cstr
		push    eax
		push    ebx
		call    dword ptr [F00415878]
		add     esp, 8
		mov     ecx, dword ptr [ebx + 8H]
		mov     dword ptr [ebp - 90H], eax
		mov     dword ptr [ebp - 94H], ecx
		cmp     eax, ecx
		je      L1
		mov     ecx, dword ptr [ebx + 18H]
		mov     edx, dword ptr [ebx + 1CH]
		mov     dword ptr [ebp - 9CH], ecx
		mov     dword ptr [ebp - 98H], edx
		mov     esi, dword ptr [eax + 10H]
		mov     eax, dword ptr [eax + 14H]
		mov     dword ptr [ebp - 0A4H], esi
		mov     dword ptr [ebp - 0A0H], eax
		mov     edx, dword ptr [s1]
		mov     ecx, dword ptr [s1 + 4H]
		mov     dword ptr [ebp - 0ACH], edx
		mov     dword ptr [ebp - 0A8H], ecx
		sub     eax, esi
		sub     ecx, edx
		mov     dword ptr [ebp - 0B4H], eax
		mov     dword ptr [ebp - 0B0H], ecx
		cmp     eax, ecx
		cmova   eax, ecx
		mov     dword ptr [ebp - 0BCH], eax
		push    eax
		push    esi
		push    edx
		call    memcmp
		add     esp, 12
		mov     dword ptr [ebp - 0B8H], eax
		test    eax, eax
		jl      L1
		jg      L2
		cmp     eax, dword ptr [ebp - 0B4H]
		jge     L2
	L1:
		lea     ecx, [s2]
		call    string_ctor
		mov     dword ptr [ebp - 0C4H], eax
		lea     ecx, [s3]
		call    string_ctor
		lea     edx, [s1]
		lea     ecx, [s4]
		call    string_ctor_assign
		mov     ecx, dword ptr [ebp - 90H]
		lea     edx, [ebp - 0CCH]
		mov     dword ptr [ebp - 0C0H], eax
		mov     dword ptr [ebp - 0C8H], ecx
		push    eax
		push    ecx
		push    ebx
		push    edx
		call    dword ptr [F004167F8]
		add     esp, 16
		mov     eax, dword ptr [ebp - 0CCH]
		mov     dword ptr [ebp - 90H], eax
		lea     ecx, [s4]
		call    string_dtor
		lea     ecx, [s3]
		call    string_dtor
		lea     ecx, [s2]
		call    string_dtor
	L2:
		mov     ecx, dword ptr [ebp - 90H]
		mov     eax, dword ptr [ValLength]
		add     ecx, 40
		push    eax
		mov     edx, dword ptr [Val]
		call    string_assign_cstr_with_length
		lea     ecx, [s1]
		call    string_dtor
		mov     eax, dword ptr [ebp - 8CH]
		mov     dword ptr fs:[0], eax
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret

		#undef this
		#undef ID
		#undef Val
		#undef ValLength
		#undef s1
		#undef s2
		#undef s3
		#undef s4
	}
}

static const DWORD data1[] = {
	0x00416274,
	0x00000004, -24,
	0x00000000,
};
const DWORD TSSGScriptStruct_GetTitleTextWidth_CtorData[] = {
	0x00000000, -60,
	0x00050000, 0x00000000,
	(DWORD)data1,
	0x00050000, 0x00000000,
	(DWORD)data1,
	0x00050014, 0x00000000,
	0x00000000, 0x00050014,
	0x00000000, 0x00000000,
};

__declspec(naked) long __stdcall TSSGScriptStruct_GetTitleTextWidth(const TSSGScriptStruct *this, HDC hDC)
{
	__asm
	{
		#define _lpszTitle 006030FFH

		push    ebp
		mov     eax, offset TSSGScriptStruct_GetTitleTextWidth_CtorData
		mov     ebp, esp
		sub     esp, 68
		call    __InitExceptBlockLDTC
		mov     edx, dword ptr [ebp + 8H]
		lea     ecx, [ebp - 18H]
		push    _lpszTitle
		push    edx
		push    ecx
		call    dword ptr [TSSGScriptStruct_GetDistinction]
		add     esp, 12
		lea     eax, [ebp - 44H]
		mov     ecx, dword ptr [ebp - 18H]
		mov     edx, dword ptr [ebp - 14H]
		sub     edx, ecx
		push    eax
		mov     eax, dword ptr [ebp + 0CH]
		push    edx
		push    ecx
		push    eax
		call    GetTextExtentPoint32A
		lea     ecx, [ebp - 18H]
		call    string_dtor
		mov     ecx, dword ptr [ebp - 60]
		mov     eax, dword ptr [ebp - 44H]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret     8

		#undef _lpszTitle
	}
}
