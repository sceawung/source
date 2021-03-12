#include "noinline_wchar.h"
#include <memory.h>

#ifndef _M_IX86
wchar_t * __cdecl _wmemccpy(wchar_t *dest, const wchar_t *src, wchar_t c, size_t count)
{
	void *p;

	if (!count)
		return NULL;
	if (p = wmemchr(src, c, count))
		p = (char *)dest + (count = (char *)p - (char *)src + sizeof(wchar_t));
	else
		count *= sizeof(wchar_t);
	memcpy(dest, src, count);
	return p;
}
#else
#pragma function(memcpy)

__declspec(naked) wchar_t * __cdecl _wmemccpy(wchar_t *dest, const wchar_t *src, wchar_t c, size_t count)
{
	__asm
	{
		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define c     (esp + 12)
		#define count (esp + 16)

		mov     eax, dword ptr [count]                      // load count
		mov     ecx, dword ptr [c]                          // load c
		test    eax, eax                                    // compare count with 0
		jz      retnull                                     // jump if count == 0
		push    esi                                         // store register
		push    edi                                         //
		mov     esi, dword ptr [src + 8]                    // load src
		mov     edi, eax                                    // copy count
		push    eax                                         // push parameters
		push    ecx                                         //
		push    esi                                         //
		call    wmemchr                                     // call wmemchr function
		mov     ecx, dword ptr [dest + 20]                  // load dest
		mov     edx, esi                                    // copy src
		mov     esi, eax                                    // p = result of memchr
		mov     dword ptr [esp], ecx                        // store the dest of memcpy
		test    eax, eax                                    // compare the result of memchr with NULL
		jnz     found                                       // jump if the result of memchr != NULL
		add     edi, edi                                    // count *= 2
		jmp     copy

	found:
		sub     esi, edx                                    // calculation
		add     ecx, 2                                      //
		lea     edi, [esi + 2]                              // count = p - src + 2
		add     esi, ecx                                    // p = dest + count
	copy:
		mov     dword ptr [esp + 4], edx                    // store the src of memcpy
		mov     dword ptr [esp + 8], edi                    // store the count of memcpy
		call    memcpy                                      // call memcpy function
		add     esp, 12                                     // restore esp register
		mov     eax, esi                                    // set return value
		pop     edi                                         // restore register
		pop     esi                                         //
	retnull:
		rep ret                                             // __cdecl return

		#undef dest
		#undef src
		#undef c
		#undef count
	}
}
#endif
