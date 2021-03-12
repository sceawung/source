#ifndef _M_IX86
#include <memory.h>

void * __cdecl _memichr(const void *buffer, int c, size_t count)
{
	char c2;

	c2 = (char)c | ('a' - 'A');
	if ((unsigned char)(c2 - 'a') >= 'z' - 'a' + 1)
		return memchr(buffer, c, count);
	while (count--)
		if ((*(((char *)buffer)++) | ('a' - 'A')) == c2)
			return (char *)buffer - 1;
	return NULL;
}
#else
#include <immintrin.h>

#pragma warning(disable:4414)

extern const char ymmconst_casebitA[32];
#define casebit ymmconst_casebitA

static void * __cdecl memichrAVX2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memichrAVX2(const void *buffer, __m256 c, size_t count);
static void * __cdecl memichrSSE42(const void *buffer, int c, size_t count);
void * __vectorcall internal_memichrSSE42(const void *buffer, __m128 reserved, __m128 c, size_t count);
static void * __cdecl memichrSSE2(const void *buffer, int c, size_t count);
void * __vectorcall internal_memichrSSE2(const void *buffer, __m128 c, size_t count);
static void * __cdecl memichr386(const void *buffer, int c, size_t count);
void * __fastcall internal_memichr386(const void *buffer, unsigned long c, size_t count);
static void * __cdecl memichrCPUDispatch(const void *buffer, int c, size_t count);

static void *(__cdecl *memichrDispatch)(const void *buffer, int c, size_t count) = memichrCPUDispatch;

__declspec(naked) void * __cdecl _memichr(const void *buffer, int c, size_t count)
{
	__asm
	{
		jmp     dword ptr [memichrDispatch]
	}
}

// AVX2 version
__declspec(naked) static void * __cdecl memichrAVX2(const void *buffer, int c, size_t count)
{
	extern void * __cdecl memchrAVX2(const void *buffer, int c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]                          // cl = search char
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     al, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a' + 1
		jae     memchrAVX2
		movd    xmm0, eax
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		vpbroadcastb ymm0, xmm0                             // ymm0 = search char
		jmp     internal_memichrAVX2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memichrAVX2(const void *buffer, __m256 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      ymm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx]                            // esi = end of buffer
		mov     eax, ecx
		xor     edx, -1
		and     eax, -32
		inc     edx                                         // edx = -count
		vmovdqa ymm2, ymmword ptr [casebit]
		and     ecx, 31
		jz      loop_begin
		vpor    ymm1, ymm2, ymmword ptr [eax]
		vpcmpeqb ymm1, ymm1, ymm0
		vpmovmskb eax, ymm1
		shr     eax, cl
		test    eax, eax
		jnz     found
		sub     ecx, 32
		sub     edx, ecx
		jb      loop_begin
		pop     esi                                         // restore esi
		vzeroupper
		ret

		align   16
	loop_begin:
		vpor    ymm1, ymm2, ymmword ptr [esi + edx]
		vpcmpeqb ymm1, ymm1, ymm0
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
__declspec(naked) void * __cdecl memichrSSE42(const void *buffer, int c, size_t count)
{
	extern void * __cdecl memchrSSE42(const void *buffer, int c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]                          // cl = search char
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     al, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a' + 1
		jae     memchrSSE42
		mov     ecx, eax
		xor     eax, 'a' - 'A'
		shl     ecx, 8
		or      eax, ecx
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		movd    xmm1, eax                                   // xmm1 = search char
		jmp     internal_memichrSSE42

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memichrSSE42(const void *buffer, __m128 reserved, __m128 c, size_t count)
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
		mov     eax, 2
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
__declspec(naked) static void * __cdecl memichrSSE2(const void *buffer, int c, size_t count)
{
	extern void * __cdecl memchrSSE2(const void *buffer, int c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     edx, dword ptr [count]                      // edx = count
		mov     ecx, dword ptr [c]                          // cl = search char
		test    edx, edx                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     eax, eax
		mov     al, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a' + 1
		jae     memchrSSE2
		movd    xmm0, eax                                   // xmm0 = search char
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		mov     ecx, dword ptr [buffer]                     // ecx = buffer
		jmp     internal_memichrSSE2

		align   16
	retnull:
		xor     eax, eax
		ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __vectorcall internal_memichrSSE2(const void *buffer, __m128 c, size_t count)
{
	__asm
	{
		#define buffer ecx
		#define c      xmm0
		#define count  edx

		push    esi                                         // preserve esi
		lea     esi, [ecx + edx]                            // esi = end of buffer
		mov     eax, ecx
		xor     edx, -1
		and     eax, -16
		inc     edx                                         // edx = -count
		movdqa  xmm2, xmmword ptr [casebit]
		and     ecx, 15
		jz      loop_begin
		movdqa  xmm1, xmmword ptr [eax]
		por     xmm1, xmm2
		pcmpeqb xmm1, xmm0
		pmovmskb eax, xmm1
		shr     eax, cl
		test    eax, eax
		jnz     found
		sub     ecx, 16
		sub     edx, ecx
		jb      loop_begin
		pop     esi                                         // restore esi
		ret

		align   16
	loop_begin:
		movdqa  xmm1, xmmword ptr [esi + edx]
		por     xmm1, xmm2
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
__declspec(naked) static void * __cdecl memichr386(const void *buffer, int c, size_t count)
{
	extern void * __cdecl memchr386(const void *buffer, int c, size_t count);

	__asm
	{
		#define buffer (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     eax, dword ptr [count]                      // eax = count
		mov     ecx, dword ptr [c]                          // cl = search char
		test    eax, eax                                    // check if count == 0
		jz      retnull                                     // if count == 0, leave
		or      ecx, 'a' - 'A'
		xor     edx, edx
		mov     dl, cl
		sub     ecx, 'a'
		cmp     cl, 'z' - 'a' + 1
		jae     memchr386
		push    eax                                         // set all 4 bytes of edx to [value]
		mov     eax, edx                                    // eax = 0/0/0/c
		shl     edx, 8                                      // edx = 0/0/c/0
		mov     ecx, eax                                    // ecx = 0/0/0/c
		or      eax, edx                                    // eax = 0/0/c/c
		or      edx, ecx                                    // edx = 0/0/c/c
		shl     eax, 16                                     // eax = c/c/0/0
		mov     ecx, dword ptr [buffer + 4]                 // ecx = buffer
		or      edx, eax                                    // edx = all 4 bytes = [search char]
		call    internal_memichr386
	retnull:
		rep ret

		#undef buffer
		#undef c
		#undef count
	}
}

__declspec(naked) void * __fastcall internal_memichr386(const void *buffer, unsigned long c, size_t count)
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
		or      ch, 'a' - 'A'
		jmp     modulo3

		align   16
	modulo1:
		mov     ecx, dword ptr [esi + eax - 1]              // read 4 bytes
		mov     edi, -01010100H
		or      ecx, 20202000H
		dec     eax
		xor     ecx, edx                                    // edx is byte\byte\byte\byte
		add     edi, ecx
		xor     ecx, -1
		and     edi, 80808080H
		and     ecx, edi
		jz      loop_begin
		jmp     has_char

		align   16
	modulo2:
		mov     cx, word ptr [esi + eax]
		or      ecx, 00002020H
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
		or      ecx, 20202020H
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

__declspec(naked) static void * __cdecl memichrCPUDispatch(const void *buffer, int c, size_t count)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)memichr386,
		(void *)memichrSSE2,
		(void *)memichrSSE42,
		(void *)memichrSSE42,
		(void *)memichrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset memichrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [memichrDispatch], eax
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
