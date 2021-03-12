#include <memory.h>
#ifndef _M_IX86

void * __cdecl _memrchr(const void *buffer, int c, size_t count)
{
	while (count--)
		if (((char *)buffer)[count] == (char)c)
			return (char *)buffer + count;
	return NULL;
}
#else
#include <immintrin.h>

void * __cdecl memrchrAVX2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memrchrAVX2(const void *buffer, __m256 c, size_t count);
void * __cdecl memrchrSSE42(const void *buffer, int c, size_t count);
void * __vectorcall internal_memrchrSSE42(const void *buffer, __m128 reserved, __m128 c, size_t count);
void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memrchrSSE2(const void *buffer, __m128 c, size_t count);
void * __cdecl memrchr386(const void *buffer, int c, size_t count);
void * __fastcall internal_memrchr386(const void *buffer, unsigned long c, size_t count);
static void * __cdecl memrchrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl *memrchrDispatch)(const void *buffer, int c, size_t count) = memrchrCPUDispatch;

__declspec(naked) void * __cdecl _memrchr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memrchrDispatch]
	}
}

// AVX2 version
__declspec(naked) void * __cdecl memrchrAVX2(const void *buffer, int c, size_t count)
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
		jmp     internal_memrchrAVX2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memrchrAVX2(const void *buffer, __m256 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      ymm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx - 1]                        // esi = last byte of buffer
		push    edi                                         // preserve edi
		and     esi, -32                                    // esi = last ymmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     esi, edx                                    // esi = last ymmword of buffer - count
		and     ecx, 31
		jz      loop_begin
		vpcmpeqb ymm1, ymm0, ymmword ptr [esi + edx]
		vpmovmskb eax, ymm1
		mov     edi, 7FFFFFFFH
		xor     ecx, 31
		shr     edi, cl
		and     eax, edi
		jnz     has_char_at_last_ymmword
		sub     esi, ecx
		xor     ecx, 31
		sub     edx, ecx
		jbe     retnull
		dec     esi

		align   16
	loop_begin:
		vpcmpeqb ymm1, ymm0, ymmword ptr [esi + edx]
		vpmovmskb eax, ymm1
		test    eax, eax
		jnz     has_char
		sub     edx, 32
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		vzeroupper
		ret

		align   16
	has_char_at_last_ymmword:
		xor     ecx, 31
		or      edi, -1
		sub     ecx, edx
		ja      mask_result
		jmp     found

		align   16
	has_char:
		cmp     edx, 32
		jae     found
		mov     ecx, esi
		or      edi, -1
		and     ecx, 31
		jz      found
	mask_result:
		shl     edi, cl
		and     eax, edi
		jz      retnull
	found:
		bsr     eax, eax
		add     edx, esi
		pop     edi                                         // restore edi
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
__declspec(naked) void * __cdecl memrchrSSE42(const void *buffer, int c, size_t count)
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
		jmp     internal_memrchrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memrchrSSE42(const void *buffer, __m128 reserved, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm1
		#define count  edx

		add     ecx, edx                                    // ecx = end of buffer
		push    esi                                         // preserve esi
		mov     esi, ecx                                    // esi = end of buffer
		push    edi                                         // preserve edi
		dec     esi                                         // esi = last byte of buffer
		mov     edi, edx                                    // edi = count
		and     esi, -16                                    // esi = last xmmword of buffer
		mov     eax, 1
		sub     esi, edx                                    // esi = last xmmword of buffer - count
		mov     edx, 16
		and     ecx, 15
		jz      loop_begin
		xor     ecx, 15
		pcmpestrm xmm1, xmmword ptr [esi + edi], 00000000B
		jnc     increment
		mov     edx, 7FFFH
		movd    eax, xmm0
		shr     edx, cl
		and     eax, edx
		jnz     has_char_at_last_xmmword
		inc     eax
		mov     edx, 16
	increment:
		sub     esi, ecx
		xor     ecx, 15
		sub     edi, ecx
		jbe     retnull
		dec     esi

		align   16
	loop_begin:
		pcmpestrm xmm1, xmmword ptr [esi + edi], 00000000B
		jc      has_char
		sub     edi, 16
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret

		align   16
	has_char_at_last_xmmword:
		xor     ecx, 15
		or      edx, -1
		sub     ecx, edi
		ja      mask_result
		jmp     found

		align   16
	has_char:
		movd    eax, xmm0
		cmp     edi, 16
		jae     found
		mov     ecx, esi
		or      edx, -1
		and     ecx, 15
		jz      found
	mask_result:
		shl     edx, cl
		and     eax, edx
		jz      retnull
	found:
		bsr     eax, eax
		add     eax, edi
		pop     edi                                         // restore edi
		add     eax, esi
		pop     esi                                         // restore esi
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// SSE2 version
__declspec(naked) void * __cdecl memrchrSSE2(const void *buffer, int c, size_t count)
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
		jmp     internal_memrchrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memrchrSSE2(const void *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx - 1]                        // esi = last byte of buffer
		push    edi                                         // preserve edi
		and     esi, -16                                    // esi = last xmmword of buffer
		add     ecx, edx                                    // ecx = end of buffer
		sub     esi, edx                                    // esi = last xmmword of buffer - count
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [esi + edx]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		mov     edi, 7FFFH
		xor     ecx, 15
		shr     edi, cl
		and     eax, edi
		jnz     has_char_at_last_xmmword
		sub     esi, ecx
		xor     ecx, 15
		sub     edx, ecx
		jbe     retnull
		dec     esi

		align   16
	loop_begin:
		movdqa  xmm1, xmmword ptr [esi + edx]
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		test    eax, eax
		jnz     has_char
		sub     edx, 16
		ja      loop_begin
	retnull:
		xor     eax, eax
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		ret

		align   16
	has_char_at_last_xmmword:
		xor     ecx, 15
		or      edi, -1
		sub     ecx, edx
		ja      mask_result
		jmp     found

		align   16
	has_char:
		cmp     edx, 16
		jae     found
		mov     ecx, esi
		or      edi, -1
		and     ecx, 15
		jz      found
	mask_result:
		shl     edi, cl
		and     eax, edi
		jz      retnull
	found:
		bsr     eax, eax
		add     edx, esi
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		add     eax, edx
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

// 80386 version
__declspec(naked) void * __cdecl memrchr386(const void *buffer, int c, size_t count)
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
		call    internal_memrchr386
	retnull:
		rep ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __fastcall internal_memrchr386(const void *buffer, unsigned long c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      edx
		#define count  (esp + 4)

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		mov     eax, dword ptr [count + 8]                  // eax = count
		lea     esi, [ecx - 4]                              // esi = buffer - 4
		add     ecx, eax                                    // ecx = end of buffer
		push    edi                                         // preserve edi
		and     ecx, 3
		jz      loop_entry
		xor     ecx, 3
		jz      modulo3
		dec     ecx
		jz      modulo2
		mov     cl, byte ptr [esi + eax + 3]
		jmp     modulo1

		align   16
	modulo3:
		mov     ecx, dword ptr [esi + eax + 1]              // read 4 bytes
		mov     edi, -00010101H
		xor     ecx, edx                                    // edx is byte\byte\byte\byte
		or      ebx, -1
		add     edi, ecx
		xor     ebx, ecx
		and     edi, 00808080H
		and     ecx, 00FFFFFFH
		and     ebx, edi
		jnz     has_char
		sub     eax, 3
		ja      loop_entry
		xor     eax, eax
		jmp     epilog

		align   16
	modulo2:
		mov     cx, word ptr [esi + eax + 2]
		cmp     ch, dl
		je      found
		dec     eax
		jz      epilog
	modulo1:
		cmp     cl, dl
		je      found
		dec     eax
		jnz     loop_entry
		jmp     epilog

		align   16
	loop_begin:
		sub     eax, 4
		jbe     retnull
	loop_entry:
		mov     ecx, dword ptr [esi + eax]                  // read 4 bytes
		mov     edi, -01010101H
		xor     ecx, edx                                    // edx is byte\byte\byte\byte
		or      ebx, -1
		add     edi, ecx
		xor     ebx, ecx
		shr     ecx, 8
		and     edi, 80808080H
		and     ebx, edi
		jz      loop_begin
	has_char:
		cmp     ecx, 00010000H
		jb      found
		test    ch, ch
		jz      byte_2
		test    cl, cl
		jz      byte_1
		sub     eax, 3
		ja      found
	retnull:
		xor     eax, eax
		jmp     epilog

		align   16
	byte_1:
		sub     eax, 2
		ja      found
		xor     eax, eax
		jmp     epilog

		align   16
	byte_2:
		dec     eax
		jz      epilog
	found:
		lea     eax, [esi + eax + 3]
	epilog:
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret     4

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) static void * __cdecl memrchrCPUDispatch(const void *buffer, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)memrchr386,
		(void *)memrchrSSE2,
		(void *)memrchrSSE42,
		(void *)memrchrSSE42,
		(void *)memrchrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset memrchrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [memrchrDispatch], eax
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
