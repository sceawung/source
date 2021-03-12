#include <windows.h>
#include <mbstring.h>

#ifndef _M_IX86
size_t __cdecl _mbsnccnt(const unsigned char *string, size_t number)
{
	size_t        count;
	unsigned char c;

	if (!number)
		return 0;
	string++;
	count = -1;
	do
		if (!(c = string[count++]))
			return count;
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && (!--number || !(string++)[count]))
			return count;
	while (--number);
	return count + 1;
}
#else
__declspec(naked) size_t __cdecl _mbsnccnt(const unsigned char *string, size_t number)
{
	__asm
	{
		#define string (esp + 4)
		#define number (esp + 8)

		mov     eax, dword ptr [number]
		mov     ecx, dword ptr [string]
		test    eax, eax
		jz      L5
		push    ebx
		push    esi
		push    edi
		lea     esi, [ecx + 1]
		or      edi, -1
		mov     ebx, eax
		xor     eax, eax
		jmp     L2

		align   16
	L1:
		dec     ebx
		jz      L3
	L2:
		mov     al, byte ptr [esi + edi]
		inc     edi
		test    al, al
		jz      L4
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		jz      L1
		dec     ebx
		jz      L4
		mov     cl, byte ptr [esi + edi]
		xor     eax, eax
		test    cl, cl
		jz      L4
		inc     esi
		jmp     L1

		align   16
	L3:
		inc     edi
	L4:
		mov     eax, edi
		pop     edi
		pop     esi
		pop     ebx
	L5:
		rep ret

		#undef string
		#undef number
	}
}
#endif
