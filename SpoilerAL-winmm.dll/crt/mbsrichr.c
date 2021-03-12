#include <windows.h>
#include <mbstring.h>
#include <intrin.h>

#pragma intrinsic(_byteswap_ulong)

#pragma warning(disable:4414)

unsigned char * __fastcall internal_mbsrchr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbsrchr_multi_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbsrichr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbsrichr_multi_byte(const unsigned char *string, unsigned int c);

#ifndef _M_IX86
unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c)
{
	if (c < 0x100)
	{
		if ((c | ('a' - 'A')) - 'a' >= 'z' - 'a' + 1)
			return internal_mbsrchr_single_byte(string, c);
		return internal_mbsrichr_single_byte(string, c);
	}
	else
	{
		char lpCharStr[sizeof(long)];
		WORD wCharType;

		if (c >= 0x10000 || !(unsigned char)c)
			return NULL;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, (unsigned char)(*(unsigned long *)lpCharStr = _byteswap_ulong(c) >> 16)))
			return NULL;
		if (!GetStringTypeA(GetThreadLocale(), CT_CTYPE1, lpCharStr, 2, &wCharType) || !(wCharType & (C1_UPPER | C1_LOWER)))
			return internal_mbsrchr_multi_byte(string, c);
		return internal_mbsrichr_multi_byte(string, c);
	}
}

unsigned char * __fastcall internal_mbsrichr_single_byte(const unsigned char *string, unsigned int c)
{
	unsigned char *result, c1, c2;

	result = NULL;
	c1 = (unsigned char)c | ('a' - 'A');
	string--;
	do
		while (((c2 = *(++string)) | ('a' - 'A')) == c1)
			result = (unsigned char *)string;
	while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
	return result;
}

unsigned char * __fastcall internal_mbsrichr_multi_byte(const unsigned char *string, unsigned int c)
{
	unsigned char *result, c2;
	char          lpCharStr[sizeof(long)];
	LCID          Locale;

	result = NULL;
	*(unsigned long *)lpCharStr = _byteswap_ulong(c) >> 16;
	Locale = GetThreadLocale();
	string--;
	while (c2 = *(++string))
	{
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
			continue;
		if (CompareStringA(Locale, NORM_IGNORECASE, string, 2, lpCharStr, 2) == CSTR_EQUAL)
			result = (unsigned char *)string;
		if (!*(++string))
			break;
	}
	return result;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     ecx, dword ptr [string]
		cmp     eax, 100H
		jae     L1
		mov     edx, eax
		or      eax, 'a' - 'A'
		sub     eax, 'a'
		cmp     eax, 'z' - 'a' + 1
		jae     internal_mbsrchr_single_byte
		jmp     internal_mbsrichr_single_byte

		align   16
	L1:
		cmp     eax, 10000H
		jae     L3
		test    al, al
		jz      L3
		shr     eax, 8
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		call    GetThreadLocale
		mov     ecx, dword ptr [c]
		shl     ecx, 16
		push    0
		bswap   ecx
		mov     edx, esp
		push    ecx
		mov     ecx, esp
		push    edx
		push    2
		push    ecx
		push    CT_CTYPE1
		push    eax
		call    GetStringTypeA
		pop     edx
		pop     ecx
		test    eax, eax
		jz      L2
		and     ecx, C1_UPPER or C1_LOWER
		jz      L2
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [c]
		jmp     internal_mbsrichr_multi_byte

		align   16
	L2:
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [c]
		jmp     internal_mbsrchr_multi_byte

		align   16
	L3:
		xor     eax, eax
	L4:
		rep ret

		#undef string
		#undef c
	}
}

__declspec(naked) unsigned char * __fastcall internal_mbsrichr_single_byte(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string ecx
		#define c      edx

		push    ebx
		push    esi
		push    edi
		dec     ecx
		or      edx, 'a' - 'A'
		mov     esi, ecx
		mov     ebx, edx
		xor     eax, eax
		xor     edi, edi

		align   16
	L1:
		mov     cl, byte ptr [esi + 1]
		inc     esi
		mov     al, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      L3
	L2:
		test    eax, eax
		jz      L4
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L4
		mov     cl, byte ptr [esi + 2]
		add     esi, 2
		mov     al, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		jne     L2
	L3:
		mov     edi, esi
		jmp     L1

		align   16
	L4:
		mov     eax, edi
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) unsigned char * __fastcall internal_mbsrichr_multi_byte(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string ecx
		#define c      edx

		push    ebx
		push    ebp
		push    esi
		push    edi
		bswap   edx
		shr     edx, 16
		lea     esi, [ecx - 1]
		xor     ebp, ebp
		push    edx
		call    GetThreadLocale
		mov     ebx, eax
		xor     eax, eax
		mov     edi, esp

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    eax, eax
		jz      L4
	L2:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		push    2
		push    edi
		push    2
		push    esi
		push    NORM_IGNORECASE
		push    ebx
		call    CompareStringA
		cmp     eax, CSTR_EQUAL
		jne     L3
		mov     ebp, esi
	L3:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L4
		mov     al, byte ptr [esi + 2]
		add     esi, 2
		test    eax, eax
		jnz     L2
	L4:
		mov     eax, ebp
		pop     ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
