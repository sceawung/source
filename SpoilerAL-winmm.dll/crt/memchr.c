#pragma warning(disable:4164)
#pragma function(memchr)

#include <memory.h>
#ifndef _M_IX86

void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (*(((char *)buffer)++) == (char)c)
			return (char *)buffer - 1;
	return NULL;
}
#else
#include <immintrin.h>

void * __cdecl memchrAVX2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memchrAVX2(const void *buffer, __m256 c, size_t count);
void * __cdecl memchrSSE42(const void *buffer, int c, size_t count);
void * __vectorcall internal_memchrSSE42(const void *buffer, __m128 reserved, __m128 c, size_t count);
void * __cdecl memchrSSE2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memchrSSE2(const void *buffer, __m128 c, size_t count);
void * __cdecl memchr386(const void *buffer, int c, size_t count);
void * __fastcall internal_memchr386(const void *buffer, unsigned long c, size_t count);
#ifdef DISABLE_UCRT
static void * __cdecl memchrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl *memchrDispatch)(const void *buffer, int c, size_t count) = memchrCPUDispatch;

__declspec(naked) void * __cdecl memchr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memchrDispatch]
	}
}
#endif

// AVX2 version
__declspec(naked) void * __cdecl memchrAVX2(const void *buffer, int c, size_t count)
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
		vpbroadcastb ymm0, byte ptr [c]                     // ymm0 = search char
		jmp     internal_memchrAVX2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memchrAVX2(const void *buffer, __m256 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      ymm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx]                            // esi = end of buffer
		mov     eax, -32
		xor     edx, -1
		and     eax, ecx
		add     edx, 1                                      // edx = -count
		and     ecx, 31
		jz      loop_begin
		vpcmpeqb ymm1, ymm0, ymmword ptr [eax]
		vpmovmskb eax, ymm1
		shr     eax, cl
		test    eax, eax
		jnz     found
		sub     ecx, 32
		nop
		sub     edx, ecx
		jae     retnull

		align   16
	loop_begin:
		vpcmpeqb ymm1, ymm0, ymmword ptr [esi + edx]
		vpmovmskb eax, ymm1
		test    eax, eax
		jnz     found
		add     edx, 32
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		vzeroupper
		ret

		align   16
	found:
		bsf     eax, eax
		add     eax, edx
		jc      retnull
		add     eax, esi
		pop     esi                                         // restore esi
		vzeroupper
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE4.2 version
__declspec(naked) void * __cdecl memchrSSE42(const void *buffer, int c, size_t count)
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
		jmp     internal_memchrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memchrSSE42(const void *buffer, __m128 reserved, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm1
		#define count  edx

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		lea     esi, [ecx + edx]                            // esi = end of buffer
		mov     ebx, -16
		xor     edx, -1
		and     ebx, ecx
		lea     edi, [edx + 1]                              // edi = -count
		mov     eax, 1
		mov     edx, 16
		and     ecx, 15
		jz      loop_begin
		pcmpestrm xmm1, xmmword ptr [ebx], 00000000B
		jnc     increment
		movd    ebx, xmm0
		shr     ebx, cl
		jnz     found_at_first
	increment:
		sub     ecx, 16
		sub     edi, ecx
		jae     retnull

		align   16
	loop_begin:
		pcmpestri xmm1, xmmword ptr [esi + edi], 00000000B
		jc      found
		add     edi, 16
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret

		align   16
	found:
		mov     eax, ecx
		jmp     epilog

		align   16
	found_at_first:
		bsf     eax, ebx
	epilog:
		add     eax, edi
		jc      retnull
		add     eax, esi
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE2 version
__declspec(naked) void * __cdecl memchrSSE2(const void *buffer, int c, size_t count)
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
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		jmp     internal_memchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memchrSSE2(const void *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx]                            // esi = end of buffer
		mov     eax, -16
		xor     edx, -1
		and     eax, ecx
		add     edx, 1                                      // edx = -count
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [eax]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		test    eax, eax
		jnz     found
		sub     ecx, 16
		nop
		sub     edx, ecx
		jae     retnull

		align   16                                          // already aligned
	loop_begin:
		movdqa  xmm1, xmmword ptr [esi + edx]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     found
		add     edx, 16
		jnc     loop_begin
	retnull:
		xor     eax, eax
		pop     esi                                         // restore esi
		ret

		align   16
	found:
		bsf     eax, eax
		add     eax, edx
		jc      retnull
		add     eax, esi
		pop     esi                                         // restore esi
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// 80386 version
__declspec(naked) void * __cdecl memchr386(const void *buffer, int c, size_t count)
{
	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		xor     eax, eax
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		mov     al, byte ptr [c]                            // al = search char
		push    edx                                         // set all 4 bytes of edx to [value]
		mov     edx, eax                                    // edx = 0/0/0/c
		shl     eax, 8                                      // eax = 0/0/c/0
		mov     ecx, edx                                    // ecx = 0/0/0/c
		or      edx, eax                                    // edx = 0/0/c/c
		or      eax, ecx                                    // eax = 0/0/c/c
		shl     edx, 16                                     // edx = c/c/0/0
		mov     ecx, dword ptr [buffer + 4]                 // ecx = buffer
		or      edx, eax                                    // edx = all 4 bytes = [search char]
		call    internal_memchr386
	retnull:
		rep ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __fastcall internal_memchr386(const void *buffer, unsigned long c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      edx
		#define count  (esp + 4)

		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, dword ptr [count + 8]
		mov     esi, eax
		xor     eax, -1
		add     esi, ecx                                    // esi = end of buffer
		inc     eax                                         // eax = -count
		and     ecx, 3
		jz      loop_entry
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     ch, byte ptr [esi + eax]
		jmp     modulo3

		align   16
	modulo1:
		mov     ecx, dword ptr [esi + eax - 1]              // read 4 bytes
		mov     edi, -01010100H
		xor     ecx, edx                                    // edx is byte\byte\byte\byte
		dec     eax
		add     edi, ecx
		xor     ecx, -1
		and     edi, 80808000H
		and     ecx, edi
		jz      loop_begin
		jmp     has_char

		align   16
	modulo2:
		mov     cx, word ptr [esi + eax]
		cmp     cl, dl
		je      found
		inc     eax
		jz      epilog
	modulo3:
		cmp     ch, dl
		je      found
		inc     eax
		jnz     loop_entry
		jmp     epilog

		align   16
	loop_begin:
		add     eax, 4
		jc      retnull
	loop_entry:
		mov     ecx, dword ptr [esi + eax]                  // read 4 bytes
		mov     edi, -01010101H
		xor     ecx, edx                                    // edx is byte\byte\byte\byte
		add     edi, ecx
		xor     ecx, -1
		and     edi, 80808080H
		and     ecx, edi
		jz      loop_begin
	has_char:
		test    cx, cx
		jnz     byte_0_or_1
		and     ecx, 00800000H
		jnz     byte_2
		add     eax, 3
		jnc     found
	retnull:
		xor     eax, eax
		jmp     epilog

		align   16
	byte_2:
		add     eax, 2
		jnc     found
		xor     eax, eax
		jmp     epilog

		align   16
	byte_0_or_1:
		test    cl, cl
		jnz     found
		inc     eax
		jz      epilog
	found:
		add     eax, esi
	epilog:
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret     4

		#undef buffer
		#undef c
		#undef count
	}
}
#ifdef DISABLE_UCRT

__declspec(naked) static void * __cdecl memchrCPUDispatch(const void *buffer, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)memchr386,
		(void *)memchrSSE2,
		(void *)memchrSSE42,
		(void *)memchrSSE42,
		(void *)memchrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset memchrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [memchrDispatch], eax
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
