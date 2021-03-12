#ifndef _M_IX86
char * __cdecl _strlwr(char *string)
{
	char *p, c;

	if (c = *(p = string))
		do
			if ((unsigned char)(c - 'A') < 'Z' - 'A' + 1)
				*p = c + ('a' - 'A');
		while (c = *(++p));
	return string;
}

char * __cdecl _strupr(char *string)
{
	char *p, c;

	if (c = *(p = string))
		do
			if ((unsigned char)(c - 'a') < 'z' - 'a' + 1)
				*p = c - ('a' - 'A');
		while (c = *(++p));
	return string;
}
#else
extern const char ymmconst_upperA[32];
extern const char ymmconst_lowerA[32];
extern const char ymmconst_azrangeA[32];
extern const char ymmconst_casebitA[32];
extern const char ymmconst_maskbit[64];
#define upper   ymmconst_upperA
#define lower   ymmconst_lowerA
#define azrange ymmconst_azrangeA
#define casebit ymmconst_casebitA

__declspec(align(16)) static const char azhigh[16] = {      // define range for upper case
	'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z'
};
__declspec(align(16)) static const char azlow[16] = {       // define range for lower case
	'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z'
};

static char * __cdecl strlwrAVX2(char *string);
static char * __cdecl struprAVX2(char *string);
static char * __cdecl strlwruprAVX2(char *string);
static char * __cdecl strlwrSSE42(char *string);
static char * __cdecl struprSSE42(char *string);
static char * __cdecl strlwruprSSE42(char *string);
static char * __cdecl strlwrSSE2(char *string);
static char * __cdecl struprSSE2(char *string);
static char * __cdecl strlwruprSSE2(char *string);
static char * __cdecl strlwrGeneric(char *string);
static char * __cdecl struprGeneric(char *string);
static char * __cdecl strlwruprGeneric(char *string);
static char * __cdecl strlwrCPUDispatch(char *string);
static char * __cdecl struprCPUDispatch(char *string);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl *strlwrDispatch)(char *string) = strlwrCPUDispatch;
static char *(__cdecl *struprDispatch)(char *string) = struprCPUDispatch;

// _strlwr function
__declspec(naked) char * __cdecl _strlwr(char *string)
{
	__asm
	{
		jmp     dword ptr [strlwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// _strupr function
__declspec(naked) char * __cdecl _strupr(char *string)
{
	__asm
	{
		jmp     dword ptr [struprDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// AVX2 version
__declspec(naked) static char * __cdecl strlwrAVX2(char *string)
{
	__asm
	{
		vmovdqa ymm2, ymmword ptr [upper]
		jmp     strlwruprAVX2
	}
}

// AVX2 version
__declspec(naked) static char * __cdecl struprAVX2(char *string)
{
	__asm
	{
		vmovdqa ymm2, ymmword ptr [lower]
		jmp     strlwruprAVX2
	}
}

__declspec(naked) static char * __cdecl strlwruprAVX2(char *string)
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
		jz      loop_entry1
		xor     ecx, 31
		and     edx, -32
		vmovdqu ymm0, ymmword ptr [maskbit + ecx + 1]       // load the non target bits mask
		vmovdqa ymm1, ymmword ptr [edx]                     // load 32 bytes
		vpor    ymm0, ymm0, ymm1                            // fill the non target bits to 1
		vpcmpeqb ymm3, ymm3, ymm0                           // compare 32 bytes with zero
		vpaddb  ymm0, ymm0, ymm2                            // all bytes greater than 'Z' if negative
		jmp     loop_entry2

		align   16
	loop_begin:
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqa ymmword ptr [edx], ymm0                     // store 32 bytes
		add     edx, 32
	loop_entry1:
		vmovdqa ymm1, ymmword ptr [edx]                     // load 32 bytes
		vpaddb  ymm0, ymm1, ymm2                            // all bytes greater than 'Z' if negative
		vpcmpeqb ymm3, ymm3, ymm1                           // compare 32 bytes with zero
	loop_entry2:
		vpcmpgtb ymm0, ymm0, ymm4                           // ymm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		vpmovmskb ecx, ymm3                                 // get one bit for each byte result
		vpand   ymm0, ymm0, ymm5                            // assign a mask for the appropriate bytes
		test    ecx, ecx
		jz      loop_begin                                  // next 32 bytes

		shr     ecx, 1
		jc      epilog
		bsf     ecx, ecx
		xor     ecx, 31
		vpand   ymm0, ymm0, ymmword ptr [maskbit + ecx]     // assign a mask for casebit
		vpxor   ymm0, ymm0, ymm1                            // negation of the 5th bit - lowercase letters
		vmovdqa ymmword ptr [edx], ymm0                     // store 32 bytes
	epilog:
		vzeroupper
		ret
	}

	#undef maskbit
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strlwrSSE42(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azhigh]                  // define range A-Z
		jmp     strlwruprSSE42
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl struprSSE42(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azlow]                   // define range a-z
		jmp     strlwruprSSE42
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE42(char *string)
{
	#define maskbit (ymmconst_maskbit + 16)

	__asm
	{
		// common code for strupr and strlwr
		mov     ecx, dword ptr [esp + 4]                    // string
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 15
		jz      loop_entry
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm4, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		por     xmm4, xmm1                                  // fill the non target bits to 1
		pcmpistrm xmm2, xmm4, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jz      last                                        // string ends in this paragraph

		align   16
	loop_begin:
		// loop
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 16
	loop_entry:
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm2, xmm1, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jnz     loop_begin                                  // next 16 bytes

	last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     finish                                      // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
	finish:
		rep ret
	}

	#undef maskbit
}

// SSE2 version
__declspec(naked) static char * __cdecl strlwrSSE2(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [upper]
		jmp     strlwruprSSE2
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl struprSSE2(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [lower]
		jmp     strlwruprSSE2
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE2(char *string)
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
		jz      loop_entry1
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm0, xmmword ptr [maskbit + ecx + 1]       // load the non target bits mask
		movdqa  xmm1, xmmword ptr [edx]                     // load 16 bytes
		por     xmm0, xmm1                                  // fill the non target bits to 1
		jmp     loop_entry2

		align   16
	loop_begin:
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 16
	loop_entry1:
		movdqa  xmm0, xmmword ptr [edx]                     // load 16 bytes
		movdqa  xmm1, xmm0                                  // copy
	loop_entry2:
		pcmpeqb xmm3, xmm0                                  // compare 16 bytes with zero
		paddb   xmm0, xmm2                                  // all bytes greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtb xmm0, xmm4                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pand    xmm0, xmm5                                  // assign a mask for the appropriate bytes
		test    ecx, ecx
		jz      loop_begin                                  // next 16 bytes

		shr     ecx, 1
		jc      epilog
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm2, xmmword ptr [maskbit + ecx]           // load the target bits mask
		pand    xmm0, xmm2                                  // assign a mask for casebit
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
	epilog:
		rep ret
	}

	#undef maskbit
}

// 386 version
__declspec(naked) static char * __cdecl strlwrGeneric(char *string)
{
	__asm
	{
		mov     ecx, 'A'
		jmp     strlwruprGeneric
	}
}

// 386 version
__declspec(naked) static char * __cdecl struprGeneric(char *string)
{
	__asm
	{
		mov     ecx, 'a'
		jmp     strlwruprGeneric
	}
}

__declspec(naked) static char * __cdecl strlwruprGeneric(char *string)
{
	__asm
	{
		push    ebx
		mov     edx, dword ptr [esp + 8]                    // string

		// loop
		align   16
	loop_begin:
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jz      return_string                               // end of string

	check_case:
		mov     bl, al                                      // check case
		sub     al, cl
		cmp     al, 'Z' - 'A' + 1
		jae     loop_begin

		// convert case
		mov     al, byte ptr [edx]
		xor     bl, 'a' - 'A'
		mov     byte ptr [edx - 1], bl
		inc     edx
		test    al, al
		jnz     check_case                                  // loop to next character

	return_string:
		mov     eax, dword ptr [esp + 8]                    // string
		pop     ebx
		ret
	}
}

// CPU dispatching for _strlwr. This is executed only once
__declspec(naked) static char * __cdecl strlwrCPUDispatch(char *string)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)strlwrGeneric,
		(void *)strlwrSSE2,
		(void *)strlwrSSE42,
		(void *)strlwrSSE42,
		(void *)strlwrSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset strlwrAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [strlwrDispatch], eax
		jmp     eax
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}

// CPU dispatching for _strupr. This is executed only once
__declspec(naked) static char * __cdecl struprCPUDispatch(char *string)
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)struprGeneric,
		(void *)struprSSE2,
		(void *)struprSSE42,
		(void *)struprSSE42,
		(void *)struprSSE42,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset struprAVX2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [struprDispatch], eax
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
