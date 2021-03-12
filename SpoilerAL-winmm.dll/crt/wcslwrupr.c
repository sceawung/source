#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if ((wchar_t)(c - L'A') < L'Z' - L'A' + 1)
				*p = c + (L'a' - L'A');
		while (c = *(++p));
	return string;
}

wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if ((wchar_t)(c - L'a') < L'z' - L'a' + 1)
				*p = c - (L'a' - L'A');
		while (c = *(++p));
	return string;
}
#else
extern const wchar_t ymmconst_upperW[16];
extern const wchar_t ymmconst_lowerW[16];
extern const wchar_t ymmconst_azrangeW[16];
extern const wchar_t ymmconst_casebitW[16];
extern const char ymmconst_maskbit[64];
#define upper   ymmconst_upperW
#define lower   ymmconst_lowerW
#define azrange ymmconst_azrangeW
#define casebit ymmconst_casebitW

__declspec(align(16)) static const wchar_t azhigh[8] = {    // define range for upper case
	L'A', L'Z', L'A', L'Z', L'A', L'Z', L'A', L'Z'
};
__declspec(align(16)) static const wchar_t azlow[8] = {     // define range for lower case
	L'a', L'z', L'a', L'z', L'a', L'z', L'a', L'z'
};

static wchar_t * __cdecl wcslwrAVX2(wchar_t *string);
static wchar_t * __cdecl wcsuprAVX2(wchar_t *string);
static wchar_t * __cdecl wcslwruprAVX2(wchar_t *string);
static wchar_t * __cdecl wcslwrSSE42(wchar_t *string);
static wchar_t * __cdecl wcsuprSSE42(wchar_t *string);
static wchar_t * __cdecl wcslwruprSSE42(wchar_t *string);
static wchar_t * __cdecl wcslwrSSE2(wchar_t *string);
static wchar_t * __cdecl wcsuprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwrGeneric(wchar_t *string);
static wchar_t * __cdecl wcsuprGeneric(wchar_t *string);
static wchar_t * __cdecl wcslwruprGeneric(wchar_t *string);
static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string);
static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string);

// Pointer to appropriate version. Initially points to dispatcher
static wchar_t *(__cdecl *wcslwrDispatch)(wchar_t *string) = wcslwrCPUDispatch;
static wchar_t *(__cdecl *wcsuprDispatch)(wchar_t *string) = wcsuprCPUDispatch;

// _wcslwr function
__declspec(naked) wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// _wcsupr function
__declspec(naked) wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcsuprDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// AVX2 version
__declspec(naked) static wchar_t * __cdecl wcslwrAVX2(wchar_t *string)
{
	__asm
	{
		vmovdqa ymm2, ymmword ptr [upper]
		jmp     wcslwruprAVX2
	}
}

// AVX2 version
__declspec(naked) static wchar_t * __cdecl wcsuprAVX2(wchar_t *string)
{
	__asm
	{
		vmovdqa ymm2, ymmword ptr [lower]
		jmp     wcslwruprAVX2
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprAVX2(wchar_t *string)
{
	#define maskbit ymmconst_maskbit

	__asm
	{
		mov     ecx, dword ptr [esp + 4]                    // string
		vpxor   ymm3, ymm3, ymm3                            // set to zero
		vmovdqa ymm4, ymmword ptr [azrange]
		vmovdqa ymm5, ymmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 31
		jz      aligned_loop_entry1
		test    eax, 1
		jnz     unaligned
		xor     ecx, 31
		and     edx, -32
		vmovdqu ymm0, ymmword ptr [maskbit + ecx + 1]       // load the non target bits mask
		vmovdqa ymm1, ymmword ptr [edx]                     // load 32 bytes
		vpor    ymm0, ymm0, ymm1                            // fill the non target bits to 1
		vpcmpeqw ymm3, ymm3, ymm0                           // compare 16 words with zero
		vpaddw  ymm0, ymm0, ymm2                            // all words greater than 'Z' if negative
		jmp     aligned_loop_entry2

		align   16
	aligned_loop:
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqa ymmword ptr [edx], ymm0                     // store 32 bytes
		add     edx, 32
	aligned_loop_entry1:
		vmovdqa ymm1, ymmword ptr [edx]                     // load 32 bytes
		vpaddw  ymm0, ymm1, ymm2                            // all words greater than 'Z' if negative
		vpcmpeqw ymm3, ymm3, ymm1                           // compare 16 words with zero
	aligned_loop_entry2:
		vpcmpgtw ymm0, ymm0, ymm4                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpmovmskb ecx, ymm3                                 // get one bit for each byte result
		vpand   ymm0, ymm0, ymm5                            // assign a mask for the appropriate words
		test    ecx, ecx
		jz      aligned_loop                                // next 32 bytes

	aligned_last:
		shr     ecx, 1
		jc      aligned_epilog
		bsf     ecx, ecx
		xor     ecx, 31
		vmovdqu ymm2, ymmword ptr [maskbit + ecx]           // load the target bits mask
		vpand   ymm0, ymm0, ymm2                            // assign a mask for casebit
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqa ymmword ptr [edx], ymm0                     // store 32 bytes
	aligned_epilog:
		vzeroupper
		ret

		align   16
	unaligned:
		xor     ecx, 31
		jz      unaligned_loop_entry
		and     edx, -32
		vmovdqa ymm1, ymmword ptr [edx]                     // load 32 bytes
		vmovdqu ymm0, ymmword ptr [maskbit + ecx]           // load the non target bits mask
		vperm2i128 ymm7, ymm1, ymm1, 00001000B              // adjust ymm value for compare
		vpslldq ymm6, ymm1, 1
		vpsrldq ymm7, ymm7, 15
		vpor    ymm6, ymm6, ymm7
		vpor    ymm0, ymm0, ymm6                            // fill the non target bits to 1
		vpcmpeqw ymm3, ymm3, ymm0                           // compare 16 words with zero
		vpaddw  ymm0, ymm0, ymm2                            // all words greater than 'Z' if negative
		vpmovmskb ecx, ymm3                                 // get one bit for each byte result
		vpcmpgtw ymm0, ymm0, ymm4                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpand   ymm0, ymm0, ymm5                            // assign a mask for the appropriate words
		vperm2i128 ymm6, ymm0, ymm0, 10000001B              // adjust mask value to 32 bytes alignment
		vpsrldq ymm0, ymm0, 1
		vpslldq ymm6, ymm6, 15
		vpor    ymm0, ymm0, ymm6
		shr     ecx, 1
		jnz     aligned_last
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqa ymmword ptr [edx], ymm0                     // store 32 bytes
		add     edx, 31
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqu ymmword ptr [edx], ymm0                     // store 32 bytes
		add     edx, 32
	unaligned_loop_entry:
		vmovdqu ymm0, ymmword ptr [edx]                     // load 32 bytes
		vmovdqa ymm1, ymm0                                  // copy
		vpcmpeqw ymm3, ymm3, ymm0                           // compare 16 words with zero
		vpaddw  ymm0, ymm0, ymm2                            // all words greater than 'Z' if negative
		vpmovmskb ecx, ymm3                                 // get one bit for each byte result
		vpcmpgtw ymm0, ymm0, ymm4                           // ymm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		vpand   ymm0, ymm0, ymm5                            // assign a mask for the appropriate words
		test    ecx, ecx
		jz      unaligned_loop                              // next 32 bytes

		shr     ecx, 1
		jc      unaligned_epilog
		bsf     ecx, ecx
		xor     ecx, 31
		vpand   ymm0, ymm0, ymmword ptr [maskbit + ecx]     // assign a mask for casebit
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqu ymmword ptr [edx], ymm0                     // store 32 bytes
	unaligned_epilog:
		vzeroupper
		ret
	}

	#undef maskbit
}

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wcslwrSSE42(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azhigh]                  // define range A-Z
		jmp     wcslwruprSSE42
	}
}

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wcsuprSSE42(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azlow]                   // define range a-z
		jmp     wcslwruprSSE42
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprSSE42(wchar_t *string)
{
	#define maskbit (ymmconst_maskbit + 16)

	__asm
	{
		// common code for wcsupr and wcslwr
		mov     ecx, dword ptr [esp + 4]                    // string
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 15
		jz      aligned_loop_entry
		test    eax, 1
		jnz     unaligned
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm4, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		por     xmm4, xmm1                                  // fill the non target bits to 1
		pcmpistrm xmm2, xmm4, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jz      aligned_last                                // string ends in this paragraph

		align   16
	aligned_loop:
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 16
	aligned_loop_entry:
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm2, xmm1, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jnz      aligned_loop                               // next 16 bytes

	aligned_last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     aligned_finish                              // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
	aligned_finish:
		rep ret

		align   16
	unaligned:
		xor     ecx, 15
		jz      unaligned_loop_entry
		and     edx, -16
		movdqa  xmm0, xmmword ptr [edx]                     // read 16 bytes from string
		movdqu  xmm4, xmmword ptr [maskbit + ecx]           // load the non target bits mask
		movdqa  xmm1, xmm0                                  // copy
		pslldq  xmm0, 1                                     // adjust xmm value for compare
		por     xmm4, xmm0                                  // fill the non target bits to 1
		pcmpistrm xmm2, xmm4, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jz      unaligned_first
		pand    xmm0, xmm3                                  // mask AND case bit
		psrldq  xmm0, 1                                     // adjust mask value to 16 bytes alignment
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 15
		jmp     unaligned_loop_entry

		align   16
	unaligned_first:
		jnc     unaligned_first_finish                      // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		psrldq  xmm0, 1                                     // adjust mask value to 16 bytes alignment
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
	unaligned_first_finish:
		rep ret

		align   16
		// loop
	unaligned_loop:
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 16
	unaligned_loop_entry:
		movdqu  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm2, xmm1, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jnz     unaligned_loop                              // next 16 bytes

		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     unaligned_finish                            // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm0                     // write changed value
	unaligned_finish:
		rep ret
	}

	#undef maskbit
}

// SSE2 version
__declspec(naked) static wchar_t * __cdecl wcslwrSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [upper]
		jmp     wcslwruprSSE2
	}
}

// SSE2 version
__declspec(naked) static wchar_t * __cdecl wcsuprSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [lower]
		jmp     wcslwruprSSE2
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string)
{
	#define maskbit (ymmconst_maskbit + 16)

	__asm
	{
		mov     ecx, dword ptr [esp + 4]                    // string
		pxor    xmm3, xmm3                                  // set to zero
		movdqa  xmm4, xmmword ptr [azrange]
		movdqa  xmm5, xmmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 15
		jz      aligned_loop_entry1
		test    eax, 1
		jnz     unaligned
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm0, xmmword ptr [maskbit + ecx + 1]       // load the non target bits mask
		movdqa  xmm1, xmmword ptr [edx]                     // load 16 bytes
		por     xmm0, xmm1                                  // fill the non target bits to 1
		jmp     aligned_loop_entry2

		align   16
	aligned_loop:
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 16
	aligned_loop_entry1:
		movdqa  xmm0, xmmword ptr [edx]                     // load 16 bytes
		movdqa  xmm1, xmm0                                  // copy
	aligned_loop_entry2:
		pcmpeqw xmm3, xmm0                                  // compare 8 words with zero
		paddw   xmm0, xmm2                                  // all words greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtw xmm0, xmm4                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm5                                  // assign a mask for the appropriate words
		test    ecx, ecx
		jz      aligned_loop                                // next 16 bytes

	aligned_last:
		shr     ecx, 1
		jc      aligned_epilog
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm2, xmmword ptr [maskbit + ecx]           // load the target bits mask
		pand    xmm0, xmm2                                  // assign a mask for casebit
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
	aligned_epilog:
		rep ret

		align   16
	unaligned:
		xor     ecx, 15
		jz      unaligned_loop_entry
		and     edx, -16
		movdqa  xmm6, xmmword ptr [edx]                     // load 16 bytes
		movdqu  xmm0, xmmword ptr [maskbit + ecx]           // load the non target bits mask
		movdqa  xmm1, xmm6                                  // copy
		pslldq  xmm6, 1                                     // adjust xmm value for compare
		por     xmm0, xmm6                                  // fill the non target bits to 1
		pcmpeqw xmm3, xmm0                                  // compare 8 words with zero
		paddw   xmm0, xmm2                                  // all words greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtw xmm0, xmm4                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm5                                  // assign a mask for the appropriate words
		psrldq  xmm0, 1                                     // adjust mask value to 16 bytes alignment
		shr     ecx, 1
		jnz     aligned_last
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 15
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqu  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 16
	unaligned_loop_entry:
		movdqu  xmm0, xmmword ptr [edx]                     // load 16 bytes
		movdqa  xmm1, xmm0                                  // copy
		pcmpeqw xmm3, xmm0                                  // compare 8 words with zero
		paddw   xmm0, xmm2                                  // all words greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtw xmm0, xmm4                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm5                                  // assign a mask for the appropriate words
		test    ecx, ecx
		jz      unaligned_loop                              // next 16 bytes

		shr     ecx, 1
		jc      unaligned_epilog
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm2, xmmword ptr [maskbit + ecx]           // load the target bits mask
		pand    xmm0, xmm2                                  // assign a mask for casebit
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqu  xmmword ptr [edx], xmm0                     // store 16 bytes
	unaligned_epilog:
		rep ret
	}

	#undef maskbit
}

// 386 version
__declspec(naked) static wchar_t * __cdecl wcslwrGeneric(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'A'
		jmp     wcslwruprGeneric
	}
}

// 386 version
__declspec(naked) static wchar_t * __cdecl wcsuprGeneric(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'a'
		jmp     wcslwruprGeneric
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprGeneric(wchar_t *string)
{
	__asm
	{
		push    ebx
		mov     edx, dword ptr [esp + 8]                    // string

		align   16
	loop_begin:
		// loop
		mov     ax, word ptr [edx]
		add     edx, 2
		test    ax, ax
		jz      return_string                               // end of string

	check_case:
		mov     bx, ax                                      // check case
		sub     ax, cx
		cmp     ax, 'Z' - 'A' + 1
		jae     loop_begin

		// convert case
		mov     ax, word ptr [edx]
		xor     bx, 'a' - 'A'
		mov     word ptr [edx - 2], bx
		add     edx, 2
		test    ax, ax
		jnz     check_case                                  // loop to next character

	return_string:
		mov     eax, dword ptr [esp + 8]                    // string
		pop     ebx
		ret
	}
}

// CPU dispatching for _wcslwr. This is executed only once
__declspec(naked) static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wcslwrGeneric,
		(void *)wcslwrSSE2,
		(void *)wcslwrSSE42,
		(void *)wcslwrSSE42,
		(void *)wcslwrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wcslwrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wcslwrDispatch], eax
		jmp     eax
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}

// CPU dispatching for _wcsupr. This is executed only once
__declspec(naked) static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)wcsuprGeneric,
		(void *)wcsuprSSE2,
		(void *)wcsuprSSE42,
		(void *)wcsuprSSE42,
		(void *)wcsuprSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset wcsuprAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [wcsuprDispatch], eax
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
