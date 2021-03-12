#include <memory.h>

#ifndef _M_IX86
void * __cdecl _memccpy(void *dest, const void *src, int c, size_t count)
{
	void *p;

	if (!count)
		return NULL;
	if (p = memchr(src, c, count))
		p = (char *)dest + (count = (char *)p - (char *)src + sizeof(char));
	memcpy(dest, src, count);
	return p;
}
#else
#pragma function(memchr, memcpy)

__declspec(naked) void * __cdecl _memccpy(void *dest, const void *src, int c, size_t count)
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
		call    memchr                                      // call memchr function
		mov     ecx, dword ptr [dest + 20]                  // load dest
		mov     edx, esi                                    // copy src
		mov     esi, eax                                    // p = result of memchr
		mov     dword ptr [esp], ecx                        // store the dest of memcpy
		test    eax, eax                                    // compare the result of memchr with NULL
		jz      copy                                        // jump if the result of memchr == NULL
		sub     esi, edx                                    // calculation
		inc     ecx                                         //
		lea     edi, [esi + 1]                              // count = p - src + 1
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
