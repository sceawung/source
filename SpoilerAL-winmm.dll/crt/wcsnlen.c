#include <wchar.h>

#ifndef _M_IX86
size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
extern const char ymmconst_maskbit[64];

static size_t __cdecl wcsnlenAVX2(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlenSSE42(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlenCPUDispatch(const wchar_t *string, size_t maxlen);

static size_t(__cdecl * wcsnlenDispatch)(const wchar_t *string, size_t maxlen) = wcsnlenCPUDispatch;

__declspec(naked) size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [wcsnlenDispatch]
	}
}

// AVX2 version
__declspec(naked) static size_t __cdecl wcsnlenAVX2(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]                     // eax = maxlen
		mov     edx, dword ptr [string]                     // edx = string
		test    eax, eax                                    // check if maxlen=0
		jz      retzero                                     // if maxlen=0, leave
		vpxor   ymm1, ymm1, ymm1                            // ymm1 = zero clear
		push    esi                                         // preserve esi
		lea     esi, [edx + eax * 2]                        // esi = end of string
		test    edx, 1
		jnz     unaligned
		mov     ecx, edx
		and     edx, -32
		and     ecx, 31
		jz      negate_count
		vpcmpeqw ymm0, ymm1, ymmword ptr [edx]
		jmp     compare

		align   16
	unaligned:
		lea     ecx, [edx + 1]
		and     edx, -32
		and     ecx, 31
		jz      negate_count
		vmovdqa ymm0, ymmword ptr [edx]
		vperm2i128 ymm2, ymm0, ymm0, 00001000B              // shift 1 byte for words compare
		vpslldq ymm0, ymm0, 1
		vpsrldq ymm2, ymm2, 15
		vpor    ymm0, ymm0, ymm2
		vpcmpeqw ymm0, ymm0, ymm1
	compare:
		vpmovmskb edx, ymm0
		shr     edx, cl
		xor     ecx, 31
		test    edx, edx
		lea     ecx, [ecx + 1]
		jnz     found_at_first
		shr     ecx, 1
	negate_count:
		sub     ecx, eax                                    // ecx = negative count
		jae     epilog

		align   16
	loop_begin:
		vpcmpeqw ymm0, ymm1, ymmword ptr [esi + ecx * 2]
		vpmovmskb edx, ymm0
		test    edx, edx
		jnz     found
		add     ecx, 16
		jnc     loop_begin
		jmp     epilog

		align   8
		nop __asm nop __asm nop __asm nop                   // padding 4 byte
	found_at_first:
		xor     ecx, ecx
		sub     ecx, eax                                    // ecx = negative count

		align   16                                          // already aligned
	found:
		bsf     edx, edx
		shr     edx, 1
		add     ecx, edx
		jc      epilog
		add     eax, ecx
	epilog:
		pop     esi                                         // restore esi
		vzeroupper
	retzero:
		rep ret

		#undef string
		#undef maxlen
	}
}

// SSE4.2 version
__declspec(naked) static size_t __cdecl wcsnlenSSE42(const wchar_t *string, size_t maxlen)
{
	#define maskbit (ymmconst_maskbit + 16)

	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]                     // eax = maxlen
		mov     ecx, dword ptr [string]                     // ecx = string
		test    eax, eax                                    // check if maxlen=0
		jz      retzero                                     // if maxlen=0, leave
		push    esi                                         // preserve esi
		lea     esi, [ecx + eax * 2]                        // esi = end of string
		pxor    xmm0, xmm0                                  // xmm0 = zero clear
		mov     edx, ecx
		and     ecx, -16
		and     edx, 15
		jz      negate_count
		shr     edx, 1
		jc      unaligned
		xor     edx, 7
		movdqa  xmm1, xmmword ptr [ecx]                     // read 16 bytes from string
		movdqu  xmm2, xmmword ptr [(maskbit + 2) + edx * 2] // load the non target bits mask
		inc     edx
		jmp     compare

		align   16
	unaligned:
		xor     edx, 7
		jz      negate_count
		movdqa  xmm1, xmmword ptr [ecx]                     // read 16 bytes from string
		movdqu  xmm2, xmmword ptr [maskbit + edx * 2]       // load the non target bits mask
		pslldq  xmm1, 1
	compare:
		por     xmm1, xmm2                                  // fill the non target bits to 1
		pcmpistri xmm0, xmm1, 00001001B                     // find null. returns index in ecx
		jz      found_at_first
	negate_count:
		sub     edx, eax                                    // edx = negative count
		jb      loop_entry
		jmp     epilog

		align   16
	found_at_first:
		add     ecx, edx
		xor     edx, edx
		sub     ecx, 8
		sub     edx, eax                                    // edx = negative count
		jmp     found

		align   16
	loop_begin:
		add     edx, 8
		jc      epilog
	loop_entry:
		pcmpistri xmm0, \
		          xmmword ptr [esi + edx * 2], 00001001B    // find null. returns index in ecx
		jnz     loop_begin

	found:
		add     edx, ecx
		jc      epilog
		add     eax, edx
	epilog:
		pop     esi                                         // restore esi
	retzero:
		rep ret

		#undef string
		#undef maxlen
	}

	#undef maskbit
}

// SSE2 version
__declspec(naked) static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]                     // eax = maxlen
		mov     edx, dword ptr [string]                     // edx = string
		test    eax, eax                                    // check if maxlen=0
		jz      retzero                                     // if maxlen=0, leave
		pxor    xmm1, xmm1                                  // xmm1 = zero clear
		push    esi                                         // preserve esi
		lea     esi, [edx + eax * 2]                        // esi = end of string
		test    edx, 1
		jnz     unaligned
		mov     ecx, edx
		and     edx, -16
		and     ecx, 15
		jz      negate_count_at_aligned
		movdqa  xmm0, xmmword ptr [edx]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		xor     ecx, 15
		test    edx, edx
		lea     ecx, [ecx + 1]
		jnz     found_at_first
		shr     ecx, 1
	negate_count_at_aligned:
		sub     ecx, eax                                    // ecx = negative count
		jb      aligned_loop
		pop     esi                                         // restore esi
	retzero:
		rep ret

		align   16
	aligned_loop:
		movdqa  xmm0, xmmword ptr [esi + ecx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 8
		jnc     aligned_loop
		pop     esi                                         // restore esi
		ret

		align   16
	unaligned:
		lea     ecx, [edx + 1]
		and     edx, -16
		and     ecx, 15
		jz      negate_count_at_unaligned
		movdqa  xmm0, xmmword ptr [edx]
		pslldq  xmm0, 1
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		shr     edx, cl
		xor     ecx, 15
		test    edx, edx
		lea     ecx, [ecx + 1]
		jnz     found_at_first
		shr     ecx, 1
	negate_count_at_unaligned:
		sub     ecx, eax                                    // ecx = negative count
		jb      unaligned_loop
		pop     esi                                         // restore esi
		ret

		align   16
	unaligned_loop:
		movdqu  xmm0, xmmword ptr [esi + ecx * 2]
		pcmpeqw xmm0, xmm1
		pmovmskb edx, xmm0
		test    edx, edx
		jnz     found
		add     ecx, 8
		jnc     unaligned_loop
		pop     esi                                         // restore esi
		ret

		align   8
		nop __asm nop __asm nop __asm nop                   // padding 4 byte
	found_at_first:
		xor     ecx, ecx
		sub     ecx, eax                                    // ecx = negative count

		align   16                                          // already aligned
	found:
		bsf     edx, edx
		shr     edx, 1
		add     ecx, edx
		jc      epilog
		add     eax, ecx
	epilog:
		pop     esi                                         // restore esi
		ret

		#undef string
		#undef maxlen
	}
}

// 80386 version
__declspec(naked) static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      retzero

		align   16
	loop_begin:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      found
		dec     eax
		jnz     loop_begin
	found:
		mov     ecx, eax
		mov     eax, dword ptr [maxlen]
		sub     eax, ecx
	retzero:
		rep ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl wcsnlenCPUDispatch(const wchar_t *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wcsnlen386,
		(void *)wcsnlenSSE2,
		(void *)wcsnlenSSE42,
		(void *)wcsnlenSSE42,
		(void *)wcsnlenSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wcsnlenAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wcsnlenDispatch], eax
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
