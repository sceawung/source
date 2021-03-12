#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl wcsrchr(const wchar_t *string, wchar_t c)
{
	wchar_t *p, c2;

	if (!c)
		return (wchar_t *)string + wcslen(string);
	string--;
	p = NULL;
	while (c2 = *(++string))
		if (c2 == c)
			p = (wchar_t *)string;
	return p;
}
#else
#pragma function(wcslen)

extern const char ymmconst_maskbit[64];

wchar_t * __cdecl wcsrchrAVX2(const wchar_t *string, wchar_t c);
wchar_t * __cdecl wcsrchrSSE42(const wchar_t *string, wchar_t c);
wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wchar_t c);
wchar_t * __cdecl wcsrchr386(const wchar_t *string, wchar_t c);
static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wchar_t c);

static wchar_t *(__cdecl *wcsrchrDispatch)(const wchar_t *string, wchar_t c) = wcsrchrCPUDispatch;

__declspec(naked) wchar_t * __cdecl wcsrchr(const wchar_t *string, wchar_t c)
{
	__asm
	{
		jmp     dword ptr [wcsrchrDispatch]
	}
}

// AVX2 version
__declspec(naked) wchar_t * __cdecl wcsrchrAVX2(const wchar_t *string, wchar_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    cx, cx
		jnz     char_is_not_null
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	char_is_not_null:
		push    ebx
		push    esi
		push    edi
		mov     edx, eax
		vpbroadcastw ymm2, word ptr [c + 12]
		mov     ecx, eax
		or      edi, -1
		and     eax, 1
		jnz     unaligned
		and     ecx, 31
		jz      aligned_loop_entry
		shl     edi, cl
		sub     edx, ecx
		jmp     aligned_loop_entry

		align   16
	aligned_loop:
		and     ebx, edi
		jz      aligned_loop_increment
		mov     eax, edx
		mov     esi, ebx
	aligned_loop_increment:
		add     edx, 32
		or      edi, -1
	aligned_loop_entry:
		vmovdqa ymm0, ymmword ptr [edx]
		vpxor   ymm1, ymm1, ymm1
		vpcmpeqw ymm1, ymm1, ymm0
		vpcmpeqw ymm0, ymm0, ymm2
		vpmovmskb ecx, ymm1
		vpmovmskb ebx, ymm0
		and     ecx, edi
		jz      aligned_loop
		jmp     null_is_found

		align   16
	unaligned:
		inc     ecx
		and     edx, -32
		dec     eax
		dec     edx
		and     ecx, 31
		jz      unaligned_loop_increment
		vmovdqa ymm0, ymmword ptr [edx + 1]
		vperm2i128 ymm3, ymm0, ymm0, 00001000B
		vpslldq ymm0, ymm0, 1
		vpsrldq ymm3, ymm3, 15
		vpor    ymm0, ymm0, ymm3
		shl     edi, cl
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		and     ebx, edi
		jz      unaligned_loop_increment
		mov     eax, edx
		mov     esi, ebx
	unaligned_loop_increment:
		add     edx, 32
		or      edi, -1
		vmovdqu ymm0, ymmword ptr [edx]
	unaligned_loop_entry:
		vpxor   ymm1, ymm1, ymm1
		vpcmpeqw ymm1, ymm1, ymm0
		vpcmpeqw ymm0, ymm0, ymm2
		vpmovmskb ecx, ymm1
		vpmovmskb ebx, ymm0
		and     ecx, edi
		jz      unaligned_loop

		align   16
	null_is_found:
		bsf     ecx, ecx
		and     ebx, edi
		xor     ecx, 31
		shl     ebx, cl
		and     ebx, 7FFFFFFFH
		jz      process_stored_pointer
		bsr     eax, ebx
		sub     edx, ecx
		jmp     return_pointer

		align   16
	process_stored_pointer:
		test    eax, eax
		jz      epilog
		bsr     edx, esi
	return_pointer:
		lea     eax, [eax + edx - 1]
	epilog:
		pop     edi
		pop     esi
		pop     ebx
		vzeroupper
		ret

		#undef string
		#undef c
	}
}

// SSE4.2 version
__declspec(naked) wchar_t * __cdecl wcsrchrSSE42(const wchar_t *string, wchar_t c)
{
	#define maskbit (ymmconst_maskbit + 16)

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     edx, dword ptr [string]
		test    cx, cx
		jz      char_is_null
		movd    xmm0, ecx
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     eax, edx
		sub     edx, 16
		and     eax, 15
		jz      loop_begin
		test    eax, 1
		jnz     unaligned
		sub     edx, eax
		xor     eax, 15
		movdqa  xmm1, xmmword ptr [edx + 16]
		movdqu  xmm2, xmmword ptr [maskbit + eax + 1]
		add     edx, 16
		jmp     compare

		align   16
	char_is_null:
		push    edx
		push    edx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	unaligned:
		xor     eax, 15
		jz      loop_begin
		and     edx, -16
		movdqa  xmm1, xmmword ptr [edx + 16]
		movdqu  xmm2, xmmword ptr [maskbit + eax]
		pslldq  xmm1, 1
		add     edx, 15
	compare:
		pcmpeqb xmm3, xmm3
		movdqa  xmm4, xmm2
		pxor    xmm3, xmm2
		pand    xmm4, xmm0
		pand    xmm1, xmm3
		por     xmm1, xmm4
		cmp     cx, 1
		je      increment
		paddb   xmm1, xmm2
		jmp     loop_entry

		align   16
	increment:
		psubb   xmm1, xmm2
	loop_entry:
		xor     eax, eax
		xor     ecx, ecx                                    // padding 2 byte
		pcmpistri xmm0, xmm1, 01000001B
		jbe     loop_found

		align   16                                          // already aligned
	loop_begin:
		pcmpistri xmm0, xmmword ptr [edx + 16], 01000001B
		lea     edx, [edx + 16]
		jnbe    loop_begin
		jnc     epilog
	loop_found:
		lea     eax, [edx + ecx * 2]
		jnz     loop_begin
	epilog:
		rep ret

		#undef string
		#undef c
	}

	#undef maskbit
}

// SSE2 version
__declspec(naked) wchar_t * __cdecl wcsrchrSSE2(const wchar_t *string, wchar_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    cx, cx
		jnz     char_is_not_null
		push    eax
		push    eax
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		align   16
	char_is_not_null:
		push    ebx
		push    esi
		push    edi
		mov     edx, eax
		movd    xmm2, ecx
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		mov     ecx, eax
		or      edi, -1
		and     eax, 1
		jnz     unaligned
		and     ecx, 15
		jz      aligned_loop_entry
		shl     edi, cl
		sub     edx, ecx
		jmp     aligned_loop_entry

		align   16
	aligned_loop:
		and     ebx, edi
		jz      aligned_loop_increment
		mov     eax, edx
		mov     esi, ebx
	aligned_loop_increment:
		add     edx, 16
		or      edi, -1
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		and     ecx, edi
		jz      aligned_loop
		jmp     null_is_found

		align   16
	unaligned:
		inc     ecx
		and     edx, -16
		dec     eax
		dec     edx
		and     ecx, 15
		jz      unaligned_loop_increment
		movdqa  xmm0, xmmword ptr [edx + 1]
		pslldq  xmm0, 1
		shl     edi, cl
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		and     ebx, edi
		jz      unaligned_loop_increment
		mov     eax, edx
		mov     esi, ebx
	unaligned_loop_increment:
		add     edx, 16
		or      edi, -1
		movdqu  xmm0, xmmword ptr [edx]
	unaligned_loop_entry:
		pxor    xmm1, xmm1
		pcmpeqw xmm1, xmm0
		pcmpeqw xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		and     ecx, edi
		jz      unaligned_loop

		align   16
	null_is_found:
		bsf     ecx, ecx
		and     ebx, edi
		xor     ecx, 15
		shl     ebx, cl
		and     ebx, 7FFFH
		jz      process_stored_pointer
		bsr     eax, ebx
		sub     edx, ecx
		jmp     return_pointer

		align   16
	process_stored_pointer:
		test    eax, eax
		jz      epilog
		bsr     edx, esi
	return_pointer:
		lea     eax, [eax + edx - 1]
	epilog:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

// 80386 version
__declspec(naked) wchar_t * __cdecl wcsrchr386(const wchar_t *string, wchar_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]
		mov     ecx, dword ptr [string]
		test    dx, dx
		jz      char_is_null
		push    esi
		sub     ecx, 2
		xor     esi, esi

		align   16
	main_loop:
		mov     ax, word ptr [ecx + 2]
		add     ecx, 2
		test    ax, ax
		jz      null_found
		cmp     ax, dx
		jne     main_loop
		mov     esi, ecx
		jmp     main_loop

		align   16
	null_found:
		mov     eax, esi
		pop     esi
		ret

		align   16
	char_is_null:
		push    ecx
		push    ecx
		call    wcslen
		pop     edx
		pop     ecx
		lea     eax, [ecx + eax * 2]
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static wchar_t * __cdecl wcsrchrCPUDispatch(const wchar_t *string, wchar_t c)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wcsrchr386,
		(void *)wcsrchrSSE2,
		(void *)wcsrchrSSE42,
		(void *)wcsrchrSSE42,
		(void *)wcsrchrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wcsrchrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wcsrchrDispatch], eax
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
