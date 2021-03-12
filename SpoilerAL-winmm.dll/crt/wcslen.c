#include <wchar.h>

#pragma function(wcslen)

#ifndef _M_IX86
size_t __cdecl wcslen(const wchar_t *string)
{
	size_t length;

	length = -1;
	string++;
	while (string[length++]);
	return length;
}
#else
extern const char ymmconst_maskbit[64];

static size_t __cdecl wcslenAVX2(const wchar_t *string);
static size_t __cdecl wcslenSSE42(const wchar_t *string);
static size_t __cdecl wcslenSSE2(const wchar_t *string);
static size_t __cdecl wcslen386(const wchar_t *string);
static size_t __cdecl wcslenCPUDispatch(const wchar_t *string);

static size_t(__cdecl * wcslenDispatch)(const wchar_t *string) = wcslenCPUDispatch;

__declspec(naked) size_t __cdecl wcslen(const wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslenDispatch]
	}
}

// AVX2 version
__declspec(naked) static size_t __cdecl wcslenAVX2(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     eax, dword ptr [string]                     // edx = string
		vpxor   ymm1, ymm1, ymm1                            // set to zero
		or      edx, -1                                     // fill mask bits
		mov     ecx, eax                                    // copy pointer
		test    eax, 1                                      // is aligned to word?
		jnz     unaligned                                   // jump if not aligned to word
		and     ecx, 31                                     // get lower 8 bits indicate misalignment
		jz      loop_entry1                                 // jump if aligned to wmmword
		shl     edx, cl                                     // shift out false bits
		sub     eax, ecx                                    // align pointer by 32
		jmp     loop_entry1

		align   16
	unaligned:
		inc     ecx                                         // add 1 byte
		or      eax, 31                                     // (align pointer by 32) + 31 byte
		and     ecx, 31                                     // compute (pointer + 1 byte) % 32
		jz      loop_entry1                                 // jump if pointer % 32 == 31
		vmovdqa ymm0, ymmword ptr [eax - 31]                // read 32 bytes aligned
		vperm2i128 ymm2, ymm0, ymm0, 00001000B              // shift 1 byte for words compare
		vpslldq ymm0, ymm0, 1
		vpsrldq ymm2, ymm2, 15
		vpor    ymm0, ymm0, ymm2
		shl     edx, cl                                     // shift out false bits
		sub     eax, 32                                     // decrement pointer by 32
		vpcmpeqw ymm0, ymm0, ymm1                           // compare 16 words with zero
		jmp     loop_entry2

		align   16
	loop_begin:
		add     eax, 32                                     // increment pointer by 32
		or      edx, -1                                     // fill mask bits
	loop_entry1:
		vpcmpeqw ymm0, ymm1, ymmword ptr [eax]              // compare 16 words with zero
	loop_entry2:
		vpmovmskb ecx, ymm0                                 // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      loop_begin                                  // loop if not found

		bsf     ecx, ecx                                    // get first bit index of result
		mov     edx, dword ptr [string]                     // get pointer to string
		add     eax, ecx                                    // add byte index
		sub     eax, edx                                    // subtract start address
		shr     eax, 1                                      // get number of words
		vzeroupper
		ret

		#undef string
	}
}

// SSE4.2 version
__declspec(naked) static size_t __cdecl wcslenSSE42(const wchar_t *string)
{
	#define maskbit (ymmconst_maskbit + 16)

	__asm
	{
		#define string (esp + 4)

		mov     eax, dword ptr [string]                     // get pointer to string
		pxor    xmm0, xmm0                                  // set to zero
		mov     ecx, eax
		mov     edx, eax
		and     eax, 15
		jz      loop_begin
		shr     eax, 1
		jc      unaligned
		and     ecx, -16
		xor     eax, 7
		movdqa  xmm1, xmmword ptr [ecx]                     // read 16 bytes from string
		movdqu  xmm2, xmmword ptr [(maskbit + 2) + eax * 2] // load the non target bits mask
		inc     eax
		jmp     compare

		align   16
	unaligned:
		xor     eax, 7
		jz      loop_begin
		and     ecx, -16
		movdqa  xmm1, xmmword ptr [ecx]                     // read 16 bytes from string
		movdqu  xmm2, xmmword ptr [maskbit + eax * 2]       // load the non target bits mask
		pslldq  xmm1, 1
	compare:
		por     xmm1, xmm2                                  // fill the non target bits to 1
		pcmpistri xmm0, xmm1, 00001001B                     // find null. returns index in ecx
		jz      last

		align   16
	loop_begin:
		pcmpistri xmm0, \
		          xmmword ptr [edx + eax * 2], 00001001B    // find null. returns index in ecx
		lea     eax, [eax + 8]                              // increment pointer by 16
		jnz     loop_begin                                  // next 16 bytes

	last:
		lea     eax, [eax + ecx - 8]
		ret

		#undef string
	}

	#undef maskbit
}

// SSE2 version
__declspec(naked) static size_t __cdecl wcslenSSE2(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     eax, dword ptr [string]                     // edx = string
		pxor    xmm1, xmm1                                  // set to zero
		or      edx, -1                                     // fill mask bits
		mov     ecx, eax                                    // copy pointer
		test    eax, 1                                      // is aligned to word?
		jnz     unaligned                                   // jump if not aligned to word
		and     ecx, 15                                     // get lower 4 bits indicate misalignment
		jz      aligned_loop_entry                          // jump if aligned to wmmword
		shl     edx, cl                                     // shift out false bits
		sub     eax, ecx                                    // align pointer by 16
		jmp     aligned_loop_entry

		align   16
	aligned_loop:
		add     eax, 16                                     // increment pointer by 16
		or      edx, -1                                     // fill mask bits
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [eax]                     // read 16 bytes aligned
		pcmpeqw xmm0, xmm1                                  // compare 8 words with zero
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      aligned_loop                                // loop if not found
		jmp     found

		align   16
	unaligned:
		inc     ecx                                         // add 1 byte
		or      eax, 15                                     // (align pointer by 16) + 15 byte
		and     ecx, 15                                     // compute (pointer + 1 byte) % 16
		jz      unaligned_loop_entry1                       // jump if pointer % 16 == 15
		movdqa  xmm0, xmmword ptr [eax - 15]                // read 16 bytes aligned
		pslldq  xmm0, 1                                     // shift 1 byte for words compare
		shl     edx, cl                                     // shift out false bits
		sub     eax, 16                                     // decrement pointer by 16
		jmp     unaligned_loop_entry2

		align   16
	unaligned_loop:
		add     eax, 16                                     // increment pointer by 16
		or      edx, -1                                     // fill mask bits
	unaligned_loop_entry1:
		movdqu  xmm0, xmmword ptr [eax]                     // read 16 bytes unaligned
	unaligned_loop_entry2:
		pcmpeqw xmm0, xmm1                                  // compare 8 words with zero
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		and     ecx, edx                                    // mask result
		jz      unaligned_loop                              // loop if not found

		align   16
	found:
		bsf     ecx, ecx                                    // get first bit index of result
		mov     edx, dword ptr [string]                     // get pointer to string
		add     eax, ecx                                    // add byte index
		sub     eax, edx                                    // subtract start address
		shr     eax, 1                                      // get number of words
		ret

		#undef string
	}
}

// 80386 version
__declspec(naked) static size_t __cdecl wcslen386(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     ecx, dword ptr [string]
		mov     eax, -1
		add     ecx, 2

		align   16
	loop_begin:
		mov     dx, word ptr [ecx + eax * 2]
		inc     eax
		test    dx, dx
		jnz     loop_begin
		rep ret

		#undef string
	}
}

__declspec(naked) static size_t __cdecl wcslenCPUDispatch(const wchar_t *string)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wcslen386,
		(void *)wcslenSSE2,
		(void *)wcslenSSE42,
		(void *)wcslenSSE42,
		(void *)wcslenSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wcslenAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wcslenDispatch], eax
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
