#include <wchar.h>
#ifndef _M_IX86

wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	while (count--)
		if (buffer[count] == c)
			return (wchar_t *)buffer + count;
	return NULL;
}
#else
#include <immintrin.h>

wchar_t * __cdecl wmemrchrAVX2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrchrAVX2(const wchar_t *buffer, __m256 c, size_t count);
wchar_t * __cdecl wmemrchrSSE42(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrchrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count);
wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrchrSSE2(const wchar_t *buffer, __m128 c, size_t count);
static wchar_t * __cdecl wmemrchr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemrchrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemrchrDispatch]
	}
}

// AVX2 version
__declspec(naked) wchar_t * __cdecl wmemrchrAVX2(const wchar_t *buffer, wchar_t c, size_t count)
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
		jmp     internal_wmemrchrAVX2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrchrAVX2(const wchar_t *buffer, __m256 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      ymm0
		#define count  edx

		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, edx                                    // eax = count
		lea     esi, [ecx + edx * 2 - 2]                    // esi = last word of buffer
		add     edx, edx                                    // edx = count * 2
		and     esi, -32 or 1                               // esi = last ymmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     esi, edx                                    // esi = last ymmword of buffer - count
		and     ecx, 31
		jz      loop_begin
		test    ecx, 1
		jnz     unaligned
		vpcmpeqw ymm1, ymm0, ymmword ptr [esi + eax * 2]
		jmp     compare

		align   16
	unaligned:
		dec     ecx
		jz      loop_begin
		vmovdqa ymm1, ymmword ptr [esi + eax * 2 - 1]
		vperm2i128 ymm2, ymm1, ymm1, 10000001B
		vpsrldq ymm1, ymm1, 1
		vpslldq ymm2, ymm2, 15
		vpor    ymm1, ymm1, ymm2
		vpcmpeqw ymm1, ymm1, ymm0
	compare:
		vpmovmskb edx, ymm1
		mov     edi, 3FFFFFFFH
		xor     ecx, 30
		shr     edi, cl
		and     edx, edi
		jnz     has_char_at_last
		sub     esi, ecx
		xor     ecx, 30
		shr     ecx, 1
		sub     esi, 2
		sub     eax, ecx
		jbe     retnull

		align   16
	loop_begin:
		vpcmpeqw ymm1, ymm0, ymmword ptr [esi + eax * 2]
		vpmovmskb edx, ymm1
		test    edx, edx
		jnz     has_char
		sub     eax, 16
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		vzeroupper
		ret

		align   16
	has_char_at_last:
		xor     ecx, 30
		lea     edi, [eax + eax]
		sub     ecx, edi
		jbe     found
		or      edi, -1
		jmp     mask_ymmword

		align   16
	has_char:
		cmp     eax, 16
		jae     found
		mov     ecx, esi
		or      edi, -1
		and     ecx, 30
		jz      found
	mask_ymmword:
		shl     edi, cl
		and     edx, edi
		jz      retnull
	found:
		bsr     edx, edx
		pop     edi                                         // restore edi
		lea     eax, [esi + eax * 2 - 1]
		pop     esi                                         // restore esi
		add     eax, edx
		vzeroupper
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE4.2 version
__declspec(naked) wchar_t * __cdecl wmemrchrSSE42(const wchar_t *buffer, wchar_t c, size_t count)
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
		jmp     internal_wmemrchrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrchrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm1
		#define count  edx

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		lea     esi, [ecx + edx * 2]                        // esi = end of buffer
		mov     edi, edx                                    // edi = count
		sub     esi, 2                                      // esi = last word of buffer
		add     edx, edx                                    // edx = count * 2
		and     esi, -16 or 1                               // esi = last xmmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     esi, edx                                    // esi = last xmmword of buffer - count
		mov     eax, 1
		mov     edx, 8
		and     ecx, 15
		jz      loop_begin
		shr     ecx, 1
		jc      unaligned
		movdqa  xmm0, xmmword ptr [esi + edi * 2]
		jmp     compare

		align   16
	unaligned:
		jz      loop_begin
		movdqa  xmm0, xmmword ptr [esi + edi * 2 - 1]
		psrldq  xmm0, 1
	compare:
		xor     ecx, 7
		pcmpestrm xmm1, xmm0, 00000001B
		mov     eax, 7FH
		jnc     increment
		movd    ebx, xmm0
		shr     eax, cl
		and     ebx, eax
		jnz     has_char_at_last
	increment:
		lea     eax, [ecx * 2 + 2]
		xor     ecx, 7
		sub     esi, eax
		mov     eax, 1
		sub     edi, ecx
		jbe     retnull

		align   16
	loop_begin:
		pcmpestrm xmm1, xmmword ptr [esi + edi * 2], 00000001B
		jc      has_char
		sub     edi, 8
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret

		align   16
	has_char_at_last:
		xor     ecx, 7
		sub     ecx, edi
		jbe     found
		or      eax, -1
		jmp     mask_xmmword

		align   16
	has_char:
		movd    ebx, xmm0
		cmp     edi, 8
		jae     found
		mov     ecx, esi
		or      eax, -1
		and     ecx, 14
		jz      found
		shr     ecx, 1
	mask_xmmword:
		shl     eax, cl
		and     ebx, eax
		jz      retnull
	found:
		bsr     edx, ebx
		mov     eax, esi
		add     edx, edi
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		lea     eax, [eax + edx * 2]
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE2 version
__declspec(naked) wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
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
		jmp     internal_wmemrchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrchrSSE2(const wchar_t *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, edx                                    // eax = count
		lea     esi, [ecx + edx * 2 - 2]                    // esi = last word of buffer
		add     edx, edx                                    // edx = count * 2
		and     esi, -16 or 1                               // esi = last xmmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     esi, edx                                    // esi = last xmmword of buffer - count
		and     ecx, 15
		jz      aligned_loop
		test    ecx, 1
		jnz     unaligned
		movdqa  xmm1, xmmword ptr [esi + eax * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		mov     edi, 3FFFH
		xor     ecx, 14
		shr     edi, cl
		and     edx, edi
		jnz     has_char_at_last
		sub     esi, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     esi, 2
		sub     eax, ecx
		ja      aligned_loop
		jmp     retnull

		align   16
	aligned_loop:
		movdqa  xmm1, xmmword ptr [esi + eax * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      aligned_loop
		jmp     retnull

		align   16
	unaligned:
		dec     ecx
		jz      unaligned_loop
		movdqa  xmm1, xmmword ptr [esi + eax * 2 - 1]
		psrldq  xmm1, 1
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		mov     edi, 3FFFH
		xor     ecx, 14
		shr     edi, cl
		and     edx, edi
		jnz     has_char_at_last
		sub     esi, ecx
		xor     ecx, 14
		shr     ecx, 1
		sub     esi, 2
		sub     eax, ecx
		ja      unaligned_loop
		jmp     retnull

		align   16
	unaligned_loop:
		movdqu  xmm1, xmmword ptr [esi + eax * 2]
		pcmpeqw xmm1, xmm0
		pmovmskb edx, xmm1
		test    edx, edx
		jnz     has_char
		sub     eax, 8
		ja      unaligned_loop
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret

		align   16
	has_char_at_last:
		xor     ecx, 14
		lea     edi, [eax + eax]
		sub     ecx, edi
		jbe     found
		or      edi, -1
		jmp     mask_xmmword

		align   16
	has_char:
		cmp     eax, 8
		jae     found
		mov     ecx, esi
		or      edi, -1
		and     ecx, 14
		jz      found
	mask_xmmword:
		shl     edi, cl
		and     edx, edi
		jz      retnull
	found:
		bsr     edx, edx
		pop     edi                                         // restore edi
		lea     eax, [esi + eax * 2 - 1]
		pop     esi                                         // restore esi
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// 80386 version
__declspec(naked) static wchar_t * __cdecl wmemrchr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                      // eax = count
		mov     edx, dword ptr [buffer]                     // edx = buffer
		test    eax, eax
		jz      retnull
		push    ebx                                         // preserve ebx
		mov     ecx, eax
		add     eax, eax
		mov     bx, word ptr [c + 4]                        // cx = search char
		add     eax, edx

		align   16
	loop_begin:
		mov     dx, word ptr [eax - 2]
		sub     eax, 2
		cmp     dx, bx
		je      found
		dec     ecx
		jnz     loop_begin
		xor     eax, eax
	found:
		pop     ebx                                         // restore ebx
	retnull:
		rep ret                                             // __cdecl return

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static wchar_t * __cdecl wmemrchrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wmemrchr386,
		(void *)wmemrchrSSE2,
		(void *)wmemrchrSSE42,
		(void *)wmemrchrSSE42,
		(void *)wmemrchrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wmemrchrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wmemrchrDispatch], eax
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
