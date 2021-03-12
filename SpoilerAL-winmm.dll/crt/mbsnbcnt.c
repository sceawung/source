#include <windows.h>
#include <mbstring.h>

#ifndef _M_IX86
size_t __cdecl _mbsnbcnt(const unsigned char *string, size_t number)
{
	const unsigned char *p;
	unsigned char       c;

	if (!number)
		return 0;
	p = string;
	do
		if (!(c = *(p++)))
			return p - string - 1;
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && !*(p++))
			return p - string - 2;
	while (--number);
	return p - string;
}
#else
__declspec(naked) size_t __cdecl _mbsnbcnt(const unsigned char *string, size_t number)
{
	__asm
	{
		#define string (esp + 4)
		#define number (esp + 8)

		mov     eax, dword ptr [number]
		mov     ecx, dword ptr [string]
		test    eax, eax
		jz      L5
		push    esi
		push    edi
		mov     esi, ecx
		mov     edi, eax
		xor     eax, eax
		jmp     L2

		align   16
	L1:
		dec     edi
		jz      L7
	L2:
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L4
	L3:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		jz      L1
		mov     cl, byte ptr [esi]
		xor     eax, eax
		test    cl, cl
		jz      L4
		dec     edi
		jz      L6
		mov     al, byte ptr [esi + 1]
		add     esi, 2
		test    al, al
		jnz     L3
	L4:
		mov     eax, dword ptr [string + 8]
		pop     edi
		sub     eax, esi
		pop     esi
		xor     eax, -1
	L5:
		rep ret

		align   16
	L6:
		inc     esi
	L7:
		mov     ecx, dword ptr [string + 8]
		mov     eax, esi
		pop     edi
		pop     esi
		sub     eax, ecx
		ret

		#undef string
		#undef number
	}
}
#endif
