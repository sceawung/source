#include <wchar.h>
extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wchar_t c, size_t count);
#ifndef _M_IX86

wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	wchar_t c2;

	c2 = c | ('a' - 'A');
	if (c2 - 'a' >= 'z' - 'a' + 1)
		return _wmemrchr(buffer, c, count);
	while (count--)
		if ((buffer[count] | ('a' - 'A')) == c2)
			return buffer + count;
	return NULL;
}
#else
#include <immintrin.h>

#pragma warning(disable:4414)

extern const wchar_t ymmconst_casebitW[16];
#define casebit ymmconst_casebitW

static wchar_t * __cdecl wmemrichrAVX2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrichrAVX2(const wchar_t *buffer, __m256 c, size_t count);
static wchar_t * __cdecl wmemrichrSSE42(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrichrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count);
static wchar_t * __cdecl wmemrichrSSE2(const wchar_t *buffer, wchar_t c, size_t count);
wchar_t * __vectorcall internal_wmemrichrSSE2(const wchar_t *buffer, __m128 c, size_t count);
static wchar_t * __cdecl wmemrichr386(const wchar_t *buffer, wchar_t c, size_t count);
static wchar_t * __cdecl wmemrichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count);

static wchar_t *(__cdecl *wmemrichrDispatch)(const wchar_t *buffer, wchar_t c, size_t count) = wmemrichrCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		jmp     dword ptr [wmemrichrDispatch]
	}
}

// AVX2 version
__declspec(naked) static wchar_t * __cdecl wmemrichrAVX2(const wchar_t *buffer, wchar_t c, size_t count)
{
	extern wchar_t * __cdecl wmemrchrAVX2(const wchar_t *buffer, wchar_t c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemrchrAVX2
		movd    xmm0, eax
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		vpbroadcastw ymm0, xmm0                             // ymm0 = search char
		jmp     internal_wmemrichrAVX2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrichrAVX2(const wchar_t *buffer, __m256 c, size_t count)
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
		vmovdqa ymm2, ymmword ptr [casebit]
		and     ecx, 31
		jz      loop_begin
		test    ecx, 1
		jnz     unaligned
		vpor    ymm1, ymm2, ymmword ptr [esi + eax * 2]
		jmp     compare

		align   16
	unaligned:
		dec     ecx
		jz      loop_begin
		vmovdqa ymm1, ymmword ptr [esi + eax * 2 - 1]
		vperm2i128 ymm3, ymm1, ymm1, 10000001B
		vpsrldq ymm1, ymm1, 1
		vpslldq ymm3, ymm3, 15
		vpor    ymm1, ymm1, ymm2
		vpor    ymm1, ymm1, ymm3
	compare:
		vpcmpeqw ymm1, ymm1, ymm0
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
		vpor    ymm1, ymm2, ymmword ptr [esi + eax * 2]
		vpcmpeqw ymm1, ymm1, ymm0
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
__declspec(naked) static wchar_t * __cdecl wmemrichrSSE42(const wchar_t *buffer, wchar_t c, size_t count)
{
	extern wchar_t * __cdecl wmemrchrSSE42(const wchar_t *buffer, wchar_t c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemrchrSSE42
		mov     ecx, eax
		xor     eax, 'a' - 'A'
		shl     ecx, 16
		or      eax, ecx
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		movd    xmm1, eax                                   // xmm1 = search char
		jmp     internal_wmemrichrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrichrSSE42(const wchar_t *buffer, __m128 reserved, __m128 c, size_t count)
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
		mov     eax, 2
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
		mov     eax, 2
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
__declspec(naked) static wchar_t * __cdecl wmemrichrSSE2(const wchar_t *buffer, wchar_t c, size_t count)
{
	extern wchar_t * __cdecl wmemrchrSSE2(const wchar_t *buffer, wchar_t c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     ax, cx
		sub     ecx, 'a'
		cmp     cx, 'z' - 'a' + 1
		jae     wmemrchrSSE2
		movd    xmm0, eax                                   // xmm0 = search char
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		jmp     internal_wmemrichrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) wchar_t * __vectorcall internal_wmemrichrSSE2(const wchar_t *buffer, __m128 c, size_t count)
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
		movdqa  xmm2, xmmword ptr [casebit]
		and     ecx, 15
		jz      aligned_loop
		test    ecx, 1
		jnz     unaligned
		movdqa  xmm1, xmmword ptr [esi + eax * 2]
		por     xmm1, xmm2
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
		por     xmm1, xmm2
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
		por     xmm1, xmm2
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
		por     xmm1, xmm2
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
__declspec(naked) static wchar_t * __cdecl wmemrichr386(const wchar_t *buffer, wchar_t c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                      // eax = count
		mov     edx, dword ptr [c]                          // dx = search char
		test    eax, eax                                    // check if count=0
		jz      retnull                                     // if count=0, leave
		or      edx, 'a' - 'A'
		xor     ecx, ecx
		mov     cx, dx
		sub     edx, 'a'
		cmp     dx, 'z' - 'a' + 1
		jae     _wmemrchr
		push    ebx                                         // preserve ebx
		mov     ebx, ecx
		mov     ecx, eax
		add     eax, eax
		add     eax, dword ptr [buffer + 4]                 // edx = buffer

		align   16
	loop_begin:
		mov     dx, word ptr [eax - 2]
		sub     eax, 2
		or      dx, 'a' - 'A'
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

__declspec(naked) static wchar_t * __cdecl wmemrichrCPUDispatch(const wchar_t *buffer, wchar_t c, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wmemrichr386,
		(void *)wmemrichrSSE2,
		(void *)wmemrichrSSE42,
		(void *)wmemrichrSSE42,
		(void *)wmemrichrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wmemrichrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wmemrichrDispatch], eax
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
