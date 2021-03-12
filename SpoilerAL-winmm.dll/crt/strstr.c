#ifndef _M_IX86
#include <stddef.h>

char * __cdecl strstr(const char *haystack, const char *needle)
{
	size_t i;

	if (!*needle)
		return (char *)haystack;
	for (; *haystack; haystack++)
		for (i = 0; ; i++)
			if (!needle[i])
				return (char *)haystack;
			else if (needle[i] != haystack[i])
				break;
	return NULL;
}
#else
#include "page.h"

static char * __cdecl strstrAVX2(const char *string1, const char *string2);
static char * __cdecl strstrSSE42(const char *string1, const char *string2);
static char * __cdecl strstrSSE2(const char *string1, const char *string2);
static char * __cdecl strstr386(const char *string1, const char *string2);
static char * __cdecl strstrCPUDispatch(const char *string1, const char *string2);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl *strstrDispatch)(const char *string1, const char *string2) = strstrCPUDispatch;

// function dispatching
__declspec(naked) char * __cdecl strstr(const char *string1, const char *string2)
{
	__asm
	{
		jmp     dword ptr [strstrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// AVX2 version
__declspec(naked) static char * __cdecl strstrAVX2(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     edx, dword ptr [needle]                     // needle (the string to be searched for)
		xor     ecx, ecx
		mov     cl, byte ptr [edx]                          // ecx contains first char from needle
		mov     eax, dword ptr [haystack]                   // haystack (the string to be searched)
		test    ecx, ecx                                    // is needle empty?
		jz      empty_needle                                // if so, return haystack (ANSI mandated)
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		vpbroadcastb ymm2, byte ptr [edx]                   // set all bytes of ymm2 to first char
		vpxor   ymm3, ymm3, ymm3                            // set to zero
		lea     esi, [eax - 1]                              // haystack - 1

		// find the first character of needle in the haystack by doing linear scan
		align   16
	find_first_char:
		inc     esi
		or      edx, -1
		mov     ecx, esi
		and     esi, -16
		and     ecx, 15
		jz      xmmword_find_loop_entry
		shl     edx, cl
		jmp     xmmword_find_loop_entry

		align   16
	xmmword_find_loop:
		add     esi, 16
		or      edx, -1
	xmmword_find_loop_entry:
		vmovdqa xmm0, xmmword ptr [esi]
		vpcmpeqb xmm1, xmm0, xmm3
		vpcmpeqb xmm0, xmm0, xmm2
		vpor    xmm0, xmm0, xmm1
		vpmovmskb eax, xmm0
		and     eax, edx
		jz      xmmword_find_loop
		bsf     eax, eax
		mov     cl, byte ptr [esi + eax]                    // cl is char from haystack
		add     esi, eax                                    // increment pointer into haystack
		test    cl, cl                                      // end of haystack?
		jz      not_found                                   // yes, and no match has been found

		// check if remaining consecutive characters match continuously
		mov     eax, dword ptr [needle + 8]
		mov     edi, esi
		test    eax, 15
		jz      xmmword_compare

		align   16
	byte_compare_loop:
		inc     eax
		inc     edi
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_compare
	byte_compare_loop_entry:
		mov     cl, byte ptr [eax]
		mov     dl, byte ptr [edi]
		test    cl, cl
		jz      found
		cmp     cl, dl
		je      byte_compare_loop
		jmp     find_first_char

		align   16
	xmmword_compare:
		test    eax, 31
		jz      ymmword_compare_loop_entry
		mov     ecx, edi
		and     ecx, PAGE_SIZE - 1
	xmmword_check_cross_pages:
		cmp     ecx, PAGE_SIZE - 15
		jae     byte_compare_loop_entry                     // jump if cross pages
		vmovdqu xmm0, xmmword ptr [edi]
		vmovdqa xmm1, xmmword ptr [eax]
		vpcmpeqb xmm0, xmm0, xmm1
		vpcmpeqb xmm1, xmm1, xmm3
		vpmovmskb ecx, xmm0
		vpmovmskb edx, xmm1
		xor     ecx, 0FFFFH
		or      ecx, edx
		jnz     ymmword_found
		add     edi, 16
		add     eax, 16
	ymmword_compare_loop_entry:
		sub     edi, 32
		sub     eax, 32

		align   16
	ymmword_compare_loop:
		mov     ecx, PAGE_SIZE - 1
		add     edi, 32
		and     ecx, edi
		add     eax, 32
		cmp     ecx, PAGE_SIZE - 31
		jae     xmmword_check_cross_pages                   // jump if cross pages
		vmovdqu ymm0, ymmword ptr [edi]
		vmovdqa ymm1, ymmword ptr [eax]
		vpcmpeqb ymm0, ymm0, ymm1
		vpcmpeqb ymm1, ymm1, ymm3
		vpmovmskb ecx, ymm0
		vpmovmskb edx, ymm1
		xor     ecx, -1
		or      ecx, edx
		jz      ymmword_compare_loop
	ymmword_found:
		bsf     ecx, ecx
		cmp     byte ptr [eax + ecx], '\0'
		jne     find_first_char
		jmp     found

		align   16
	not_found:
		xor     esi, esi
	found:
		pop     edi
		mov     eax, esi
		pop     esi
		vzeroupper
	empty_needle:
		rep ret

		#undef haystack
		#undef needle
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strstrSSE42(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     edx, dword ptr [needle]                     // needle (the string to be searched for)
		xor     ecx, ecx
		mov     cl, byte ptr [edx]                          // ecx contains first char from needle
		mov     eax, dword ptr [haystack]                   // haystack (the string to be searched)
		mov     edx, ecx                                    // set 2 bytes of ecx to first char
		push    esi                                         // preserve esi
		shl     ecx, 8
		lea     esi, [eax - 1]                              // haystack - 1
		or      ecx, edx                                    // is needle empty?
		jz      empty_needle                                // if so, return haystack (ANSI mandated)
		push    edi                                         // preserve edi
		movd    xmm1, ecx                                   // set all bytes of xmm1 to first char
		movd    xmm2, edx
		pshuflw xmm1, xmm1, 0
		movlhps xmm1, xmm1

		// find the first character of needle in the haystack by doing linear scan
		align   16
	find_first_char:
		sub     esi, 15
		mov     eax, 2
		mov     ecx, esi
		and     esi, -16
		and     ecx, 15
		jz      xmmword_find_loop
		mov     edx, 16
		pcmpestrm xmm2, xmmword ptr [esi + 16], 00000000B
		lea     esi, [esi + 16]
		jnc     xmmword_find_loop
		movd    eax, xmm0
		shr     eax, cl
		jz      xmmword_find_loop
		bsf     eax, eax
		add     ecx, eax
		cmp     byte ptr [esi + ecx], 0
		jne     match_first_char
		jmp     not_found

		align   16
	xmmword_find_loop:
		pcmpistri xmm1, xmmword ptr [esi + 16], 00000000B
		lea     esi, [esi + 16]
		jnbe    xmmword_find_loop
		jnc     not_found                                   // no match has been found
	match_first_char:
		add     esi, ecx                                    // increment pointer into haystack

		// check if remaining consecutive characters match continuously
		mov     eax, dword ptr [needle + 8]
		mov     edi, esi
		test    eax, 15
		jz      xmmword_compare_loop_entry

		align   16
	byte_compare_loop:
		inc     eax
		inc     edi
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_compare_loop_entry
	byte_compare_loop_entry:
		mov     cl, byte ptr [eax]
		mov     dl, byte ptr [edi]
		test    cl, cl
		jz      found
		cmp     cl, dl
		je      byte_compare_loop
		jmp     find_first_char

		align   8
		xchg    ax, ax                                      // padding 2 byte
	xmmword_compare_loop_entry:
		sub     edi, 16
		sub     eax, 16

		align   16                                          // already aligned
	xmmword_compare_loop:
		mov     ecx, PAGE_SIZE - 1
		add     edi, 16
		and     ecx, edi
		add     eax, 16
		cmp     ecx, PAGE_SIZE - 15
		jae     byte_compare_loop_entry                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [edi]                     // read 16 bytes of haystack
		pcmpistri xmm0, xmmword ptr [eax], 00011000B        // unsigned bytes, equal each, invert. returns index in ecx
		jnbe    xmmword_compare_loop                        // jump if not carry flag and not zero flag
		jc      find_first_char
		jmp     found

		align   16
	not_found:
		xor     esi, esi
	found:
		pop     edi
		mov     eax, esi
	empty_needle:
		pop     esi
		ret

		#undef haystack
		#undef needle
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl strstrSSE2(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     edx, dword ptr [needle]                     // needle (the string to be searched for)
		xor     ecx, ecx
		mov     cl, byte ptr [edx]                          // ecx contains first char from needle
		mov     eax, dword ptr [haystack]                   // haystack (the string to be searched)
		mov     edx, ecx                                    // set 2 bytes of ecx to first char
		push    esi                                         // preserve esi
		shl     ecx, 8
		lea     esi, [eax - 1]                              // haystack - 1
		or      ecx, edx                                    // is needle empty?
		jz      empty_needle                                // if so, return haystack (ANSI mandated)
		push    edi                                         // preserve edi
		movd    xmm2, ecx                                   // set all bytes of xmm2 to first char
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		pxor    xmm3, xmm3                                  // set to zero

		// find the first character of needle in the haystack by doing linear scan
		align   16
	find_first_char:
		inc     esi
		or      edx, -1
		mov     ecx, esi
		and     esi, -16
		and     ecx, 15
		jz      xmmword_find_loop_entry
		shl     edx, cl
		jmp     xmmword_find_loop_entry

		align   16
	xmmword_find_loop:
		add     esi, 16
		or      edx, -1
	xmmword_find_loop_entry:
		movdqa  xmm0, xmmword ptr [esi]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		pcmpeqb xmm0, xmm2
		por     xmm0, xmm1
		pmovmskb eax, xmm0
		and     eax, edx
		jz      xmmword_find_loop
		bsf     eax, eax
		mov     cl, byte ptr [esi + eax]                    // cl is char from haystack
		add     esi, eax                                    // increment pointer into haystack
		cmp     cl, 0                                       // end of haystack?
		je      not_found                                   // yes, and no match has been found

		// check if remaining consecutive characters match continuously
		mov     eax, dword ptr [needle + 8]
		mov     edi, esi
		test    eax, 15
		jz      xmmword_compare_loop_entry

		align   16                                          // already aligned
	byte_compare_loop:
		inc     eax
		inc     edi
		test    eax, 15                                     // use only eax for 'test reg, imm'
		jz      xmmword_compare_loop_entry
	byte_compare_loop_entry:
		mov     cl, byte ptr [eax]
		mov     dl, byte ptr [edi]
		test    cl, cl
		jz      found
		cmp     cl, dl
		je      byte_compare_loop
		jmp     find_first_char

		align   8
		xchg    ax, ax                                      // padding 2 byte
	xmmword_compare_loop_entry:
		sub     edi, 16
		sub     eax, 16

		align   16                                          // already aligned
	xmmword_compare_loop:
		mov     ecx, PAGE_SIZE - 1
		add     edi, 16
		and     ecx, edi
		add     eax, 16
		cmp     ecx, PAGE_SIZE - 15
		jae     byte_compare_loop_entry                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [edi]
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm3
		pmovmskb ecx, xmm0
		pmovmskb edx, xmm1
		xor     ecx, 0FFFFH
		or      ecx, edx
		jz      xmmword_compare_loop
		bsf     ecx, ecx
		cmp     byte ptr [eax + ecx], '\0'
		jne     find_first_char
		jmp     found

		align   16
	not_found:
		xor     esi, esi
	found:
		pop     edi
		mov     eax, esi
	empty_needle:
		pop     esi
		ret

		#undef haystack
		#undef needle
	}
}

// 80386 version
__declspec(naked) static char * __cdecl strstr386(const char *string1, const char *string2)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		mov     eax, dword ptr [haystack]                   // haystack
		mov     edx, dword ptr [needle]                     // needle
		mov     cl, byte ptr [edx]
		inc     edx
		test    cl, cl
		jz      EmptyNeedle                                 // a zero-length needle is always found
		push    ebx
		push    esi
		push    edi
		jmp     SearchLoop

		align   16
	SearchLoop:
		// search for first character match
		mov     bl, byte ptr [eax]
		inc     eax
		test    bl, bl
		jz      NotFound                                    // end of haystack reached without finding
		cmp     bl, cl
		jne     SearchLoop                                  // loop through haystack

		// first character match
		dec     eax                                         // begin of match position
		mov     edi, edx
		mov     esi, eax
		sub     edi, eax

	MatchLoop:
		mov     bl, byte ptr [edi + esi]
		inc     esi
		test    bl, bl
		jz      Found                                       // end of needle. match ok
		cmp     bl, byte ptr [esi]
		je      MatchLoop

		// match failed, recover and continue
		inc     eax
		jmp     SearchLoop

		align   16
	NotFound:
		// needle not found. return NULL
		xor     eax, eax
	Found:
		// needle found. return pointer to position in haystack
		pop     edi
		pop     esi
		pop     ebx
	EmptyNeedle:
		rep ret

		#undef haystack
		#undef needle
	}
}

// CPU dispatching for strstr. This is executed only once
__declspec(naked) static char * __cdecl strstrCPUDispatch(const char *string1, const char *string2)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)strstr386,
		(void *)strstrSSE2,
		(void *)strstrSSE42,
		(void *)strstrSSE42,
		(void *)strstrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset strstrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [strstrDispatch], eax
		jmp     eax
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#endif
