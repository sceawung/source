#define USING_NAMESPACE_BCB6_STD
#include "TTitleSelectForm.h"
#include "TFindNameForm.h"
#include "bcb6_std_allocator.h"

EXTERN_C int __stdcall LCMapStringJISX0213(
	IN           LCID   Locale,
	IN           DWORD  dwMapFlags,
	IN           LPCSTR lpSrcStr,
	IN           int    cchSrc,
	OUT OPTIONAL LPSTR  lpDestStr,
	IN           int    cchDest);

static string * __cdecl CnvString(string *Result, LCID Locale, const string *Src);

__declspec(naked) string * __cdecl TTitleSelectForm_CnvString(string *Result, TTitleSelectForm *this, const string *Src)
{
	__asm
	{
		#define this   (esp + 8)
		#define Locale (esp + 8)

		mov     eax, dword ptr [this]
		mov     eax, [eax]TTitleSelectForm.sortLCID
		mov     dword ptr [Locale], eax
		jmp     CnvString

		#undef this
		#undef Locale
	}
}

__declspec(naked) string * __cdecl TFindNameForm_CnvString(string *Result, TFindNameForm *this, const string *Src)
{
	__asm
	{
		#define this   (esp + 8)
		#define Locale (esp + 8)

		mov     eax, dword ptr [this]
		mov     eax, [eax]TFindNameForm.findLCID
		mov     dword ptr [Locale], eax
		jmp     CnvString

		#undef this
		#undef Locale
	}
}

static string * __cdecl CnvString(string *Result, LCID Locale, const string *Src)
{
	#define dwMapFlags (DWORD)(LCMAP_FULLWIDTH | LCMAP_KATAKANA | LCMAP_LOWERCASE)

	size_t cchSrc = string_length(Src);
	size_t cchDest = cchSrc * 2;
	string_end_of_storage(Result) = (string_begin(Result) = (LPSTR)allocator_allocate(cchDest + 1)) + cchDest + 1;
	cchDest = LCMapStringJISX0213(Locale, dwMapFlags, string_c_str(Src), cchSrc, string_begin(Result), cchDest);
	*(string_end(Result) = string_begin(Result) + cchDest) = '\0';
	string_shrink_to_fit(Result);
	return Result;

	#undef dwMapFlags
}

#ifndef _M_IX86
static void __fastcall ToSortKey(char *s, LCID lcid)
{
	static const unsigned char jisx0213_8346_83FF[] = {
		                                    0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x09, 0x0A, 0x0D, 0x0E,
		0x11, 0x12, 0x14, 0x15, 0x17, 0x18, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 0x20, 0x22, 0x23, 0x24, 0x25,
		0x26, 0x27, 0x28, 0x29, 0x2A, 0x2C, 0x2D, 0x2F, 0x30, 0x32, 0x33, 0x35, 0x36, 0x37, 0x3A, 0x3B,
		0x3C, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x45, 0x47, 0x48, 0x49, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x39,
		0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5B, 0x5D, 0x5F, 0x61, 0x63, 0x64, 0x65,
		0x67, 0x69, 0x6B, 0x6D, 0x00, 0x05, 0x10, 0x08, 0x0B, 0x0F, 0x13, 0x16, 0x21, 0x2B, 0x31, 0x6E,
		0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,
		0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,
		0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E,
		0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0x9F, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE,
		0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xFA, 0xFB, 0xFC, 0xFD, 0xB6, 0x0C, 0x19, 0x1C, 0x2E,
		0x34, 0x38, 0x3D, 0x41, 0x46, 0x4A, 0x44, 0x50, 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0xB7, 0xB8, 0xB9
	};
	static const unsigned char jisx0213_8440_8495[] = {
		0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
		0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
		0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
		0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD4, 0xD5, 0xD6, 0xD7, 0xD3,
		0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
		0xE8, 0xE9, 0x00, 0x02, 0x04, 0x06
	};
	unsigned char c1, c2;
	if (PRIMARYLANGID(LANGIDFROMLCID(lcid)) == LANG_JAPANESE)
		while (c1 = *(s++)) {
			unsigned short c3;
			if ((unsigned char)(c1 - 0x81) >= 0x9F - 0x81 + 1 && (unsigned char)(c1 - 0xE0) >= 0xFC - 0xE0 + 1)
				continue;
			else if (!(c2 = *(s++)))
				break;
			else if (c1 -= 0x84)
				if (++c1 || c2 < 0x46)
					continue;
				else
					c3 = jisx0213_8346_83FF[c2 - 0x46] + 0x8346;
			else
				if ((unsigned char)(c2 - 0x40) >= 0x95 - 0x40 + 1)
					continue;
				else
					c3 = jisx0213_8440_8495[c2 - 0x40] + 0x83AC;
			s[-2] = (unsigned char)(c3 >> 8);
			s[-1] = (unsigned char)c3;
		}
}
#else
__declspec(naked) static void __fastcall ToSortKey(char *s, LCID lcid)
{
	static const unsigned char jisx0213_8346_83FF[] = {
		                                    0x01, 0x02, 0x03, 0x04, 0x06, 0x07, 0x09, 0x0A, 0x0D, 0x0E,
		0x11, 0x12, 0x14, 0x15, 0x17, 0x18, 0x1A, 0x1B, 0x1D, 0x1E, 0x1F, 0x20, 0x22, 0x23, 0x24, 0x25,
		0x26, 0x27, 0x28, 0x29, 0x2A, 0x2C, 0x2D, 0x2F, 0x30, 0x32, 0x33, 0x35, 0x36, 0x37, 0x3A, 0x3B,
		0x3C, 0x3E, 0x3F, 0x40, 0x42, 0x43, 0x45, 0x47, 0x48, 0x49, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x39,
		0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5B, 0x5D, 0x5F, 0x61, 0x63, 0x64, 0x65,
		0x67, 0x69, 0x6B, 0x6D, 0x00, 0x05, 0x10, 0x08, 0x0B, 0x0F, 0x13, 0x16, 0x21, 0x2B, 0x31, 0x6E,
		0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,
		0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,
		0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E,
		0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0x9F, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE,
		0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xFA, 0xFB, 0xFC, 0xFD, 0xB6, 0x0C, 0x19, 0x1C, 0x2E,
		0x34, 0x38, 0x3D, 0x41, 0x46, 0x4A, 0x44, 0x50, 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0xB7, 0xB8, 0xB9
	};
	static const unsigned char jisx0213_8440_8495[] = {
		0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
		0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
		0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
		0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD4, 0xD5, 0xD6, 0xD7, 0xD3,
		0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
		0xE8, 0xE9, 0x00, 0x02, 0x04, 0x06
	};
	__asm
	{
		and     edx, 0x03FF
		xor     eax, eax
		xor     edx, LANG_JAPANESE
		jz      L5
		jmp     L6

		align   16
	L1:
		mov     dl, al
		sub     al, 0x81
		cmp     al, 0x9F - 0x81 + 1
		jb      L2
		sub     al, 0xE0 - 0x81
		cmp     al, 0xFC - 0xE0 + 1
		jae     L5
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jnz     L5
		jmp     L6

	L2:
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jz      L6
		sub     dl, 0x84
		jz      L3
		inc     dl
		jnz     L5
		sub     al, 0x46
		jb      L5
		mov     al, byte ptr [jisx0213_8346_83FF + eax]
		add     al, 0x46
		jmp     L4

	L3:
		sub     al, 0x40
		cmp     al, 0x95 - 0x40 + 1
		jae     L5
		mov     al, byte ptr [jisx0213_8440_8495 + eax]
		add     al, 0xAC
	L4:
		adc     dl, 0x83
		mov     byte ptr [ecx - 1], al
		mov     byte ptr [ecx - 2], dl
	L5:
		mov     al, byte ptr [ecx]
		inc     ecx
		test    al, al
		jnz     L1
	L6:
		ret
	}
}
#endif

static __inline void GetDistinction(string *dest, const TSSGScriptStruct *ss, const char *id, LCID lcid)
{
	string s;

	TSSGScriptStruct_GetDistinction(&s, ss, id);
	if (!string_empty(&s)) {
		CnvString(dest, lcid, &s);
		ToSortKey(string_begin(dest), lcid);
	} else {
		char *p = (char *)node_alloc_allocate(1);
		*(string_end(dest) = string_begin(dest) = p) = '\0';
		string_end_of_storage(dest) = p + 1;
	}
	string_dtor(&s);
}

BOOLEAN __cdecl TTitleSelectForm_SortFunc(TTitleSelectForm *this, const TSSGScriptStruct *SS1, const TSSGScriptStruct *SS2)
{
	#define lpszTitle ((LPCSTR)0x006182E3)

	int    ret;
	string s1, s2;

	GetDistinction(&s1, SS1, string_c_str(&this->sortKey), this->sortLCID);
	GetDistinction(&s2, SS2, string_c_str(&this->sortKey), this->sortLCID);
	ret = strcmp(string_c_str(&s1), string_c_str(&s2));
	string_dtor(&s2);
	string_dtor(&s1);
	if (ret == 0 && (
		string_length(&this->sortKey) != 5 ||
		*(LPDWORD)string_begin(&this->sortKey) != BSWAP32('titl') ||
		string_at(&this->sortKey, 4) != 'e'))
	{
		GetDistinction(&s1, SS1, lpszTitle, this->sortLCID);
		GetDistinction(&s2, SS2, lpszTitle, this->sortLCID);
		ret = strcmp(string_c_str(&s1), string_c_str(&s2));
		string_dtor(&s2);
		string_dtor(&s1);
	}
	return ret < 0;

	#undef lpszTitle
}
