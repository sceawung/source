#ifndef _M_IX86
#include <string.h>

char * __cdecl _strrichr(const char *string, int c)
{
	char *p, c1, c2;

	c1 = (char)c | ('a' - 'A');
	if ((unsigned char)(c1 - 'a') >= 'z' - 'a' + 1)
		return strrchr(string, c);
	string--;
	p = NULL;
	while (c2 = *(++string))
		if ((c2 | ('a' - 'A')) == c1)
			p = (char *)string;
	return p;
}
#else
#include <stddef.h>

#pragma warning(disable:4414)

extern const char    ymmconst_casebitA[32];
extern const wchar_t ymmconst_casebitW[16];
extern const char    ymmconst_maskbit[64];
#define casebit     ymmconst_casebitA
#define insensitive ymmconst_casebitW

static char * __cdecl strrichrAVX2(const char *string, int c);
static char * __cdecl strrichrSSE42(const char *string, int c);
static char * __cdecl strrichrSSE2(const char *string, int c);
static char * __cdecl strrichr386(const char *string, int c);
static char * __cdecl strrichrCPUDispatch(const char *string, int c);

static char *(__cdecl *strrichrDispatch)(const char *string, int c) = strrichrCPUDispatch;

__declspec(naked) char * __cdecl _strrichr(const char *string, int c)
{
	__asm
	{
		jmp     dword ptr [strrichrDispatch]
	}
}

// AVX2 version
__declspec(naked) static char * __cdecl strrichrAVX2(const char *string, int c)
{
	extern char * __cdecl strrchrAVX2(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     edx, dword ptr [string]
		or      eax, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, al
		sub     eax, 'a'
		cmp     al, 'z' - 'a' + 1
		jae     strrchrAVX2
		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		movd    xmm2, ecx
		vmovdqa ymm3, ymmword ptr [casebit]
		vpbroadcastb ymm2, xmm2
		mov     ecx, edx
		or      edi, -1
		and     ecx, 31
		jz      loop_entry
		shl     edi, cl
		sub     edx, ecx
		jmp     loop_entry

		align   16
	loop_begin:
		and     ebx, edi
		jz      loop_increment
		mov     eax, edx
		mov     esi, ebx
	loop_increment:
		add     edx, 32
		or      edi, -1
	loop_entry:
		vmovdqa ymm0, ymmword ptr [edx]
		vpxor   ymm1, ymm1, ymm1
		vpcmpeqb ymm1, ymm1, ymm0
		vpor    ymm0, ymm0, ymm3
		vpcmpeqb ymm0, ymm0, ymm2
		vpmovmskb ecx, ymm1
		vpmovmskb ebx, ymm0
		and     ecx, edi
		jz      loop_begin
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
		add     eax, edx
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
__declspec(naked) char * __cdecl strrichrSSE42(const char *string, int c)
{
	#define maskbit (ymmconst_maskbit + 16)

	extern char * __cdecl strrchrSSE42(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     edx, dword ptr [string]
		or      eax, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, al
		sub     eax, 'a'
		cmp     al, 'z' - 'a' + 1
		jae     strrchrSSE42
		movd    xmm0, ecx
		movdqa  xmm1, xmmword ptr [insensitive]
		punpcklbw xmm0, xmm0
		pshuflw xmm0, xmm0, 0
		movlhps xmm0, xmm0
		pxor    xmm0, xmm1
		mov     eax, edx
		sub     edx, 16
		and     eax, 15
		jz      loop_begin
		sub     edx, eax
		xor     eax, 15
		movdqa  xmm1, xmmword ptr [edx + 16]
		movdqu  xmm2, xmmword ptr [maskbit + eax + 1]
		pcmpeqb xmm3, xmm3
		movdqa  xmm4, xmm2
		pxor    xmm3, xmm2
		pand    xmm4, xmm0
		pand    xmm1, xmm3
		por     xmm1, xmm4
		paddb   xmm1, xmm2
		add     edx, 16
		xor     eax, eax
		pcmpistri xmm0, xmm1, 01000000B
		jbe     loop_found

		align   16
	loop_begin:
		pcmpistri xmm0, xmmword ptr [edx + 16], 01000000B
		lea     edx, [edx + 16]
		jnbe    loop_begin
		jnc     epilog
	loop_found:
		lea     eax, [edx + ecx]
		jnz     loop_begin
	epilog:
		rep ret

		#undef string
		#undef c
	}

	#undef maskbit
}

// SSE2 version
__declspec(naked) static char * __cdecl strrichrSSE2(const char *string, int c)
{
	extern char * __cdecl strrchrSSE2(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     edx, dword ptr [string]
		or      eax, 'a' - 'A'
		xor     ecx, ecx
		mov     cl, al
		sub     eax, 'a'
		cmp     al, 'z' - 'a' + 1
		jae     strrchrSSE2
		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		movd    xmm2, ecx
		punpcklbw xmm2, xmm2
		pshuflw xmm2, xmm2, 0
		movlhps xmm2, xmm2
		movdqa  xmm3, xmmword ptr [casebit]
		mov     ecx, edx
		or      edi, -1
		and     ecx, 15
		jz      loop_entry
		shl     edi, cl
		sub     edx, ecx
		jmp     loop_entry

		align   16
	loop_begin:
		and     ebx, edi
		jz      loop_increment
		mov     eax, edx
		mov     esi, ebx
	loop_increment:
		add     edx, 16
		or      edi, -1
	loop_entry:
		movdqa  xmm0, xmmword ptr [edx]
		pxor    xmm1, xmm1
		pcmpeqb xmm1, xmm0
		por     xmm0, xmm3
		pcmpeqb xmm0, xmm2
		pmovmskb ecx, xmm1
		pmovmskb ebx, xmm0
		and     ecx, edi
		jz      loop_begin
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
		add     eax, edx
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
__declspec(naked) static char * __cdecl strrichr386(const char *string, int c)
{
	extern char * __cdecl strrchr386(const char *string, int c);

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     edx, dword ptr [c]                          // dl = search char
		mov     ecx, dword ptr [string]                     // ecx = string
		or      edx, 'a' - 'A'
		xor     eax, eax
		mov     al, dl
		sub     edx, 'a'
		cmp     dl, 'z' - 'a' + 1
		jae     strrchr386
		                                                    // set all 4 bytes of ebx to [value]
		mov     edx, eax                                    // u edx = 0/0/0/c
		push    ebx                                         // v preserve ebx
		shl     eax, 8                                      // u eax = 0/0/c/0
		push    ebp                                         // v preserve ebp
		or      eax, edx                                    // u eax = 0/0/c/c
		push    esi                                         // v preserve esi
		mov     ebx, eax                                    // u ebx = 0/0/c/c
		push    edi                                         // v preserve edi
		shl     eax, 16                                     // u eax = c/c/0/0
		xor     ebp, ebp                                    // v ebp = NULL
		or      ebx, eax                                    // u ebx = all 4 bytes = [search char]
		mov     eax, ecx                                    // v eax = string
		and     ecx, 3
		jz      loop_begin
		dec     ecx
		jz      modulo1
		dec     ecx
		jz      modulo2
		mov     ch, byte ptr [eax]
		inc     eax
		mov     dh, ch
		or      ch, 'a' - 'A'
		jmp     modulo3

		align   16
	modulo1:
		mov     cl, byte ptr [eax]
		inc     eax
		test    cl, cl
		jz      process_stored_pointer
		or      cl, 'a' - 'A'
		cmp     cl, bl
		jne     modulo2
		lea     ebp, [eax - 1]
	modulo2:
		mov     cx, word ptr [eax]
		add     eax, 2
		test    cl, cl
		jz      process_stored_pointer
		mov     edx, ecx
		or      ecx, 00002020H
		cmp     cl, bl
		jne     modulo3
		lea     ebp, [eax - 2]
	modulo3:
		test    dh, dh
		jz      process_stored_pointer
		cmp     ch, bl
		jne     loop_begin
		lea     ebp, [eax - 1]
		jmp     loop_begin

		align   16
	loop_begin:
		mov     ecx, dword ptr [eax]
		add     eax, 4
		mov     esi, ecx
		or      ecx, 20202020H
		mov     edx, ecx
		xor     ecx, ebx
		sub     esi, 01010101H
		lea     edi, [ecx - 01010101H]
		xor     edx, -1
		xor     ecx, -1
		and     esi, edx
		and     ecx, edi
		and     esi, 80808080H
		jnz     null_is_found
		and     ecx, 80808080H
		jz      loop_begin
		mov     ebp, eax
		jmp     loop_begin

		align   16
	null_is_found:
		and     ecx, 00808080H
		jz      process_stored_pointer
		not     edx
		bswap   edx
		add     esi, esi
		jz      compare_byte_2
		and     esi, 00010100H
		jz      compare_byte_1
		shr     edx, 16
		jmp     compare_byte_0

		align   16
	compare_byte_2:
		cmp     dh, bl
		je      byte_2
	compare_byte_1:
		shr     edx, 16
		nop                                                 // padding 1 byte
		cmp     dl, bl
		je      byte_1
	compare_byte_0:
		cmp     dh, bl
		je      byte_0

		align   16                                          // already aligned
	process_stored_pointer:
		mov     eax, ebp
		test    ebp, ebp
		jz      epilog
		test    eax, 3                                      // append 3 byte (and ebp,imm -> test eax,imm)
		jnz     epilog
		mov     ecx, dword ptr [eax - 4]
		bswap   ecx
		or      ecx, 20202020H
		align   16                                          // padding 8 byte
		cmp     cl, bl
		je      byte_3
		cmp     ch, bl
		je      byte_2
		shr     ecx, 16
		nop                                                 // padding 1 byte
		cmp     cl, bl
		je      byte_1

		align   16                                          // already aligned
	byte_0:
		sub     eax, 4
		jmp     epilog

	byte_1:
		sub     eax, 3
		jmp     epilog

	byte_2:
		sub     eax, 2
		jmp     epilog

	byte_3:
		dec     eax

		align   16                                          // already aligned
	epilog:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
		#undef c
	}
}

__declspec(naked) static char * __cdecl strrichrCPUDispatch(const char *string, int c)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)strrichr386,
		(void *)strrichrSSE2,
		(void *)strrichrSSE42,
		(void *)strrichrSSE42,
		(void *)strrichrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset strrichrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [strrichrDispatch], eax
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
