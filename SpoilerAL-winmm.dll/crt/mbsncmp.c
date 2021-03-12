#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsncmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	int           ret;
	unsigned char c;

	if (!count)
		return 0;
	do
		if ((ret = *(string1++) - (c = *(string2++))) || !c)
			break;
		else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
			continue;
		else if ((ret = *(string1++) - (c = *(string2++))) || !c)
			break;
	while (--count);
	return ret;
}
#else
__declspec(naked) int __cdecl _mbsncmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [string1]
		test    eax, eax
		jz      L4
		push    ebx
		push    esi
		push    edi
		mov     esi, ecx
		mov     edi, dword ptr [string2 + 12]
		mov     ebx, eax
		sub     edi, ecx
		jmp     L2

		align   16
	L1:
		dec     ebx
		jz      L3
	L2:
		mov     cl, byte ptr [esi]
		xor     eax, eax
		mov     al, byte ptr [esi + edi]
		inc     esi
		cmp     cl, al
		jne     L5
		test    eax, eax
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi]
		xor     eax, eax
		mov     al, byte ptr [esi + edi]
		inc     esi
		cmp     cl, al
		jne     L5
		test    eax, eax
		jnz     L1
	L3:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
	L4:
		rep ret

		align   16
	L5:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
