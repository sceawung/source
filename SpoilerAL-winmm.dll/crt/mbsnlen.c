#include <windows.h>
#include <mbstring.h>

#ifndef _M_IX86
size_t __cdecl _mbsnlen(const unsigned char *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		unsigned char c;

		while ((c = *(p++)) && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || *(p++)) && --length);
		length = maxlen - length;
	}
	return length;
}
#elif defined(DISABLE_UCRT)
__declspec(naked) size_t __cdecl _mbsnlen(const unsigned char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     ecx, dword ptr [string]
		test    eax, eax
		jz      L5
		push    esi
		push    edi
		mov     esi, eax
		xor     eax, eax
		mov     edi, ecx
		jmp     L2

		align   16
	L1:
		dec     esi
		jz      L4
	L2:
		mov     al, byte ptr [edi]
		inc     edi
		test    al, al
		jz      L4
	L3:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [edi]
		xor     eax, eax
		test    cl, cl
		jz      L4
		dec     esi
		jz      L4
		mov     al, byte ptr [edi + 1]
		add     edi, 2
		test    al, al
		jnz     L3
	L4:
		mov     eax, dword ptr [maxlen + 8]
		pop     edi
		sub     eax, esi
		pop     esi
	L5:
		rep ret

		#undef string
		#undef maxlen
	}
}
#endif
