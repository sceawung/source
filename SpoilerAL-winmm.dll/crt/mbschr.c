#include <windows.h>

#pragma warning(disable:4414)

unsigned char * __fastcall internal_mbschr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbschr_multi_byte(const unsigned char *string, unsigned int c);

#ifndef _M_IX86
#ifdef DISABLE_UCRT
unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	if (c < 0x100)
	{
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
			return NULL;
		return internal_mbschr_single_byte(string, c);
	}
	else
	{
		if (c >= 0x10000 || !(unsigned char)c)
			return NULL;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, (unsigned char)(c >> 8)))
			return NULL;
		return internal_mbschr_multi_byte(string, c);
	}
}

#endif
unsigned char * __fastcall internal_mbschr_single_byte(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	string--;
	do
		if ((c2 = *(++string)) == (unsigned char)c)
			return (unsigned char *)string;
	while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
	return NULL;
}

unsigned char * __fastcall internal_mbschr_multi_byte(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	for (string--; ; )
	{
		if ((c2 = *(++string)) != (unsigned char)(c >> 8))
		{
			if (!c2)
				break;
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
				continue;
			if (!string[1])
				break;
		}
		else if ((c2 = string[1]) == (unsigned char)c)
			return (unsigned char *)string;
		else if (!c2)
			break;
		string++;
	}
	return NULL;
}
#else
#ifdef DISABLE_UCRT
__declspec(naked) unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		xor     eax, eax
		cmp     edx, 100H
		jb      L1
		cmp     edx, 10000H
		jae     L2
		test    dl, dl
		jz      L2
		shr     edx, 8
		push    edx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [c]
		jmp     internal_mbschr_multi_byte

		align   16
	L1:
		push    edx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [c]
		test    eax, eax
		jz      internal_mbschr_single_byte
		xor     eax, eax
	L2:
		rep ret

		#undef string
		#undef c
	}
}

#endif
__declspec(naked) unsigned char * __fastcall internal_mbschr_single_byte(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string ecx
		#define c      edx

		push    ebx
		push    esi
		xor     eax, eax
		lea     esi, [ecx - 1]
		mov     ebx, edx

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
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
		mov     al, byte ptr [esi + 2]
		add     esi, 2
		cmp     al, bl
		jne     L2
	L3:
		mov     eax, esi
	L4:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) unsigned char * __fastcall internal_mbschr_multi_byte(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string ecx
		#define c      edx

		push    ebx
		push    esi
		xor     eax, eax
		lea     esi, [ecx - 1]
		mov     ebx, edx

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bh
		je      L4
	L2:
		test    eax, eax
		jz      L5
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
	L3:
		test    cl, cl
		jz      L5
		mov     al, byte ptr [esi + 2]
		add     esi, 2
		cmp     al, bh
		jne     L2
	L4:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		jne     L3
		mov     eax, esi
	L5:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
