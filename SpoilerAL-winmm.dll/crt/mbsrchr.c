#include <windows.h>

#pragma warning(disable:4414)

unsigned char * __fastcall internal_mbsrchr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbsrchr_multi_byte(const unsigned char *string, unsigned int c);

#ifndef _M_IX86
#ifdef DISABLE_UCRT
unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	if (c < 0x100)
	{
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
			return NULL;
		return internal_mbsrchr_single_byte(string, c);
	}
	else
	{
		if (c >= 0x10000 || !(unsigned char)c)
			return NULL;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, (unsigned char)(c >> 8)))
			return NULL;
		return internal_mbsrchr_multi_byte(string, c);
	}
}

#endif
unsigned char * __fastcall internal_mbsrchr_single_byte(const unsigned char *string, unsigned int c)
{
	unsigned char *result, c2;

	result = NULL;
	string--;
	do
		while ((c2 = *(++string)) == (unsigned char)c)
			result = (unsigned char *)string;
	while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
	return result;
}

unsigned char * __fastcall internal_mbsrchr_multi_byte(const unsigned char *string, unsigned int c)
{
	unsigned char *result, c2;

	result = NULL;
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
			result = (unsigned char *)string;
		else if (!c2)
			break;
		string++;
	}
	return result;
}
#else
#ifdef DISABLE_UCRT
__declspec(naked) unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
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
		jmp     internal_mbsrchr_multi_byte

		align   16
	L1:
		push    edx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     ecx, dword ptr [string]
		mov     edx, dword ptr [c]
		test    eax, eax
		jz      internal_mbsrchr_single_byte
		xor     eax, eax
	L2:
		rep ret

		#undef string
		#undef c
	}
}

#endif
__declspec(naked) unsigned char * __fastcall internal_mbsrchr_single_byte(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string ecx
		#define c      edx

		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		xor     edi, edi
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

__declspec(naked) unsigned char * __fastcall internal_mbsrchr_multi_byte(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		xor     edi, edi
		lea     esi, [ecx - 1]
		mov     ebx, edx

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bh
		je      L5
	L2:
		test    eax, eax
		jz      L6
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
	L3:
		test    cl, cl
		jz      L6
	L4:
		mov     al, byte ptr [esi + 2]
		add     esi, 2
		cmp     al, bh
		jne     L2
	L5:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		jne     L3
		mov     edi, esi
		jmp     L4

		align   16
	L6:
		mov     eax, edi
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
