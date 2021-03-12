#include <windows.h>

#ifndef _M_IX86
unsigned int __cdecl _mbsnextc(const unsigned char *string)
{
	unsigned int c, trail;

	if (IsDBCSLeadByteEx(CP_THREAD_ACP, (BYTE)(c = *(string++))) && (trail = *string))
		c = (c << 8) | trail;
	return c;
}
#else
__declspec(naked) unsigned int __cdecl _mbsnextc(const unsigned char *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     edx, dword ptr [string]
		xor     eax, eax
		mov     al, byte ptr [edx]
		inc     edx
		push    eax
		push    edx
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		pop     edx
		pop     eax
		jz      L1
		shl     eax, 8
		xor     ecx, ecx
		mov     cl, byte ptr [edx]
		or      eax, ecx
	L1:
		rep ret

		#undef string
	}
}
#endif
