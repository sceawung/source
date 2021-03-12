#define wmemchr inline_wmemchr
#include <wchar.h>
#undef wmemchr
#ifndef _M_IX86

wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	while (count--)
		if (*(buffer++) == c)
			return buffer - 1;
	return NULL;
}
#else
#include <immintrin.h>

wchar_t * __cdecl wmemchrAVX2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemchrAVX2(const wchar_t *buffer, __m256 c, size_t count);
wchar_t * __cdecl wmemchrSSE42(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemchrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count);
wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemchrSSE2(const wchar_t *buffer, __m128 c, size_t count);
#ifdef DISABLE_UCRT
static wchar_t * __cdecl wmemchr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemchrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wmemchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemchrDispatch]
	}
}
#endif

// AVX2 version
__declspec(naked) wchar_t * __cdecl wmemchrAVX2(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		vpbroadcastw ymm0, word ptr [c]                     // ymm0 = search char
		jmp     internal_wmemchrAVX2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemchrAVX2(const wchar_t *buffer, __m256 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      ymm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx * 2]                        // esi = end of buffer
		mov     eax, ecx                                    // eax = buffer
		neg     edx                                         // edx = -count
		test    eax, 1
		jnz     unaligned
		and     ecx, 31
		jz      loop_begin
		sub     eax, ecx
		vpcmpeqw ymm1, ymm0, ymmword ptr [eax]
		jmp     compare

		align   16
	unaligned:
		inc     ecx
		and     eax, -32
		and     ecx, 31
		jz      loop_begin
		vmovdqa ymm1, ymmword ptr [eax]
		vperm2i128 ymm2, ymm1, ymm1, 00001000B
		vpslldq ymm1, ymm1, 1
		vpsrldq ymm2, ymm2, 15
		vpor    ymm1, ymm1, ymm2
		vpcmpeqw ymm1, ymm1, ymm0
	compare:
		vpmovmskb eax, ymm1
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 16]
		jnz     found
		sub     edx, ecx
		jae     retnull

		align   16
	loop_begin:
		vpcmpeqw ymm1, ymm0, ymmword ptr [esi + edx * 2]
		vpmovmskb eax, ymm1
		test    eax, eax
		jnz     found
		add     edx, 16
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		vzeroupper
		ret                                                 // __cdecl return

		align   16
	found:
		bsf     eax, eax
		shr     eax, 1
		add     eax, edx
		jc      retnull
		lea     eax, [esi + eax * 2]
		pop     esi                                         // restore esi
		vzeroupper
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE4.2 version
__declspec(naked) wchar_t * __cdecl wmemchrSSE42(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		movd    xmm1, dword ptr [c]                         // xmm1 = search char
		jmp     internal_wmemchrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemchrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm1
		#define count  edx

		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     edi, edx                                    // edi = count
		mov     eax, ecx                                    // eax = buffer
		xor     edi, -1
		lea     esi, [ecx + edx * 2]                        // esi = end of buffer
		inc     edi                                         // edi = -count
		mov     edx, 8
		test    eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      loop_entry
		sub     eax, ecx
		movdqa  xmm0, xmmword ptr [eax]
		jmp     compare

		align   16
	unaligned:
		inc     ecx
		and     eax, -16
		and     ecx, 15
		jz      loop_entry
		movdqa  xmm0, xmmword ptr [eax]
		pslldq  xmm0, 1
	compare:
		shr     ecx, 1
		mov     eax, 1
		pcmpestrm xmm1, xmm0, 00000001B
		jnc     increment
		movd    eax, xmm0
		shr     eax, cl
		jnz     found_at_first
	increment:
		sub     ecx, 8
		sub     edi, ecx
		jae     retnull
	loop_entry:
		mov     eax, 1

		align   16
	loop_begin:
		pcmpestri xmm1, xmmword ptr [esi + edi * 2], 00000001B
		jc      found
		add     edi, 8
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	found_at_first:
		bsf     ecx, eax
	found:
		add     ecx, edi
		jc      retnull
		lea     eax, [esi + ecx * 2]
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE2 version
__declspec(naked) wchar_t * __cdecl wmemchrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		movd    xmm0, dword ptr [c]                         // xmm0 = search char
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		jmp     internal_wmemchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemchrSSE2(const wchar_t *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx * 2]                        // esi = end of buffer
		mov     eax, ecx                                    // eax = buffer
		neg     edx                                         // edx = -count
		test    eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop
		sub     eax, ecx
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jae     retnull

		align   16
	aligned_loop:
		movdqa  xmm1, xmmword ptr [esi + edx * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     aligned_loop
		jmp     retnull

		align   16
	unaligned:
		inc     ecx
		and     eax, -16
		and     ecx, 15
		jz      unaligned_loop
		movdqa  xmm1, xmmword ptr [eax]
		pslldq  xmm1, 1
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		shr     ecx, 1
		test    eax, eax
		lea     ecx, [ecx - 8]
		jnz     found
		sub     edx, ecx
		jae     retnull

		align   16
	unaligned_loop:
		movdqu  xmm1, xmmword ptr [esi + edx * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     found
		add     edx, 8
		jnc     unaligned_loop
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		align   16
	found:
		bsf     eax, eax
		shr     eax, 1
		add     eax, edx
		jc      retnull
		lea     eax, [esi + eax * 2]
		pop     esi                                         // restore esi
		ret                                                 // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}
#ifdef DISABLE_UCRT

// 80386 version
__declspec(naked) static wchar_t * __cdecl wmemchr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [buffer]                     // edx = buffer
		mov     eax, dword ptr [count]                      // eax = count
		mov     cx, word ptr [c]                            // cx = search char
		lea     edx, [edx + eax * 2]                        // edx = end of buffer
		xor     eax, -1                                     // eax = -count - 1

		align   16
	loop_begin:
		inc     eax
		jz      retnull
		cmp     word ptr [edx + eax * 2], cx
		jne     loop_begin
		lea     eax, [edx + eax * 2]
	retnull:
		rep ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wmemchr386,
		(void *)wmemchrSSE2,
		(void *)wmemchrSSE42,
		(void *)wmemchrSSE42,
		(void *)wmemchrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wmemchrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wmemchrDispatch], eax
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
#endif
