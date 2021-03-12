/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using srandom(seed).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include "random.h"
#if defined(_M_IX86) || defined(_M_X64)
#include <immintrin.h>
#endif
#include <assert.h>
#include "endianness.h"

/*------
  ENDIAN
  ------*/
#if !defined(__BYTE_ORDER__) || __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__ && __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
#error "Current byte order is not supported."
#endif

/*-----------------
  BASIC DEFINITIONS
  -----------------*/
#define SFMT_MEXP       19937                   // Mersenne Exponent. The period of the sequence is a multiple of 2^MEXP-1.
#define SFMT_N          (SFMT_MEXP / 128 + 1)   // SFMT generator has an internal state array of 128-bit integers, and N is its size.
#define SFMT_N32        (SFMT_N * 4)            // N32 is the size of internal state array when regarded as an array of 32-bit integers.

/*----------------------
  the parameters of SFMT
  ----------------------*/
#define SFMT_POS1       122                     // the pick up position of the array.
#define SFMT_SL1        18                      // the parameter of shift left as four 32-bit registers.
#define SFMT_SL2        1                       // the parameter of shift left as one 128-bit register. The 128-bit integer is shifted by (SFMT_SL2 * 8) bits.
#define SFMT_SR1        11                      // the parameter of shift right as four 32-bit registers.
#define SFMT_SR2        1                       // the parameter of shift right as one 128-bit register. The 128-bit integer is shifted by (SFMT_SR2 * 8) bits.
#define SFMT_MSK1       0xDFFFFFEFU             // A bitmask, used in the recursion.  These parameters are introduced to break symmetry of SIMD.
#define SFMT_MSK2       0xDDFECB7FU             //
#define SFMT_MSK3       0xBFFAFFFFU             //
#define SFMT_MSK4       0xBFFFFFF6U             //
#define SFMT_PARITY1    0x00000001U             // These definitions are part of a 128-bit period certification vector.
#define SFMT_PARITY2    0x00000000U             //
#define SFMT_PARITY3    0x00000000U             //
#define SFMT_PARITY4    0x13C9E684U             //

#if defined(__LITTLE_ENDIAN__)
#define SFMT_MSK(index) (           \
    (index) == 0 ? SFMT_MSK1 :      \
    (index) == 1 ? SFMT_MSK2 :      \
    (index) == 2 ? SFMT_MSK3 :      \
    (index) == 3 ? SFMT_MSK4 : 0)
#else
#define SFMT_MSK(index) (           \
    (index) == 0 ? SFMT_MSK4 :      \
    (index) == 1 ? SFMT_MSK3 :      \
    (index) == 2 ? SFMT_MSK2 :      \
    (index) == 3 ? SFMT_MSK1 : 0)
#endif

/*------------------------------------------
  128-bit SIMD like data type for standard C
  ------------------------------------------*/
/* 128-bit data structure */
typedef __declspec(align(16)) union {
	uint32_t    u[4];
	uint64_t    u64[2];
#if defined(_M_IX86) || defined(_M_X64)
	__m128i     si;
#endif
} w128_t;

/*---------
  VARIABLES
  ---------*/
/* SFMT internal state */
static w128_t __declspec(align(32)) state[SFMT_N];  // the 128-bit internal state array
static size_t                       idx;            // index counter to the 32-bit internal state array

#define sfmt   ((__m128i *)state)
#define sfmt32 ((uint32_t *)state)

/*------
  MACROS
  ------*/
#define _BSF8(x, default) ( \
    ((x) & 0x01) ? 0 :      \
    ((x) & 0x02) ? 1 :      \
    ((x) & 0x04) ? 2 :      \
    ((x) & 0x08) ? 3 :      \
    ((x) & 0x10) ? 4 :      \
    ((x) & 0x20) ? 5 :      \
    ((x) & 0x40) ? 6 :      \
    ((x) & 0x80) ? 7 :      \
    (default))
#define _BSF16(x, default) _BSF8(x, _BSF8((x) >> 8, (default) - 8) + 8)
#define _BSF32(x, default) _BSF16(x, _BSF16((x) >> 16, (default) - 16) + 16)
#define BSF8(x) _BSF8(x, -1)
#define BSF16(x) _BSF16(x, -1)
#define BSF32(x) _BSF32(x, -1)

#define MASM_BSF32(x) (                                      -1 + \
    (  (x)         and 1                                 )      + \
    ((((x) shr  1) and 1) and (((x) and 0x00000001) eq 0)) *  2 + \
    ((((x) shr  2) and 1) and (((x) and 0x00000003) eq 0)) *  3 + \
    ((((x) shr  3) and 1) and (((x) and 0x00000007) eq 0)) *  4 + \
    ((((x) shr  4) and 1) and (((x) and 0x0000000F) eq 0)) *  5 + \
    ((((x) shr  5) and 1) and (((x) and 0x0000001F) eq 0)) *  6 + \
    ((((x) shr  6) and 1) and (((x) and 0x0000003F) eq 0)) *  7 + \
    ((((x) shr  7) and 1) and (((x) and 0x0000007F) eq 0)) *  8 + \
    ((((x) shr  8) and 1) and (((x) and 0x000000FF) eq 0)) *  9 + \
    ((((x) shr  9) and 1) and (((x) and 0x000001FF) eq 0)) * 10 + \
    ((((x) shr 10) and 1) and (((x) and 0x000003FF) eq 0)) * 11 + \
    ((((x) shr 11) and 1) and (((x) and 0x000007FF) eq 0)) * 12 + \
    ((((x) shr 12) and 1) and (((x) and 0x00000FFF) eq 0)) * 13 + \
    ((((x) shr 13) and 1) and (((x) and 0x00001FFF) eq 0)) * 14 + \
    ((((x) shr 14) and 1) and (((x) and 0x00003FFF) eq 0)) * 15 + \
    ((((x) shr 15) and 1) and (((x) and 0x00007FFF) eq 0)) * 16 + \
    ((((x) shr 16) and 1) and (((x) and 0x0000FFFF) eq 0)) * 17 + \
    ((((x) shr 17) and 1) and (((x) and 0x0001FFFF) eq 0)) * 18 + \
    ((((x) shr 18) and 1) and (((x) and 0x0003FFFF) eq 0)) * 19 + \
    ((((x) shr 19) and 1) and (((x) and 0x0007FFFF) eq 0)) * 20 + \
    ((((x) shr 20) and 1) and (((x) and 0x000FFFFF) eq 0)) * 21 + \
    ((((x) shr 21) and 1) and (((x) and 0x001FFFFF) eq 0)) * 22 + \
    ((((x) shr 22) and 1) and (((x) and 0x003FFFFF) eq 0)) * 23 + \
    ((((x) shr 23) and 1) and (((x) and 0x007FFFFF) eq 0)) * 24 + \
    ((((x) shr 24) and 1) and (((x) and 0x00FFFFFF) eq 0)) * 25 + \
    ((((x) shr 25) and 1) and (((x) and 0x01FFFFFF) eq 0)) * 26 + \
    ((((x) shr 26) and 1) and (((x) and 0x03FFFFFF) eq 0)) * 27 + \
    ((((x) shr 27) and 1) and (((x) and 0x07FFFFFF) eq 0)) * 28 + \
    ((((x) shr 28) and 1) and (((x) and 0x0FFFFFFF) eq 0)) * 29 + \
    ((((x) shr 29) and 1) and (((x) and 0x1FFFFFFF) eq 0)) * 30 + \
    ((((x) shr 30) and 1) and (((x) and 0x3FFFFFFF) eq 0)) * 31 + \
    ((((x) shr 31) and 1) and (((x) and 0x7FFFFFFF) eq 0)) * 32)

#if defined(__LITTLE_ENDIAN__)
#define IDX_LO          0
#define IDX_HI          1
#define IDX32(x)        (x)
#define IDX128(x)       (x)
#define LE_PLUS(x)      (x)
#define LE_MINUS(x)     (-(x))
#define LE_POST_INC(x)  ((x)++)
#define LE_ADD          add
#define LE_INC          inc
#else
#define IDX_LO          1
#define IDX_HI          0
#define IDX32(x)        (SFMT_N32 - 1 - (x))
#define IDX128(x)       (SFMT_N - 1 - (x))
#define LE_PLUS(x)      (-(x))
#define LE_MINUS(x)     (x)
#define LE_POST_INC(x)  ((x)--)
#define LE_ADD          sub
#define LE_INC          dec
#endif

/*----------------
  STATIC FUNCTIONS
  ----------------*/
#if defined(_M_IX86)
static void sfmt_gen_rand_all_avx2();
static void sfmt_gen_rand_all_sse2();
static void sfmt_gen_rand_all_generic();
static void sfmt_gen_rand_all_cpu_dispatch();

static void (*sfmt_gen_rand_all)() = sfmt_gen_rand_all_cpu_dispatch;
#endif

#if defined(_M_IX86) || defined(_M_X64)
/**
 * parameters used by sse2.
 */
static const w128_t __declspec(align(16)) simd_param_mask = { {
	SFMT_MSK(0) & (UINT32_MAX >> SFMT_SR1),
	SFMT_MSK(1) & (UINT32_MAX >> SFMT_SR1),
	SFMT_MSK(2) & (UINT32_MAX >> SFMT_SR1),
	SFMT_MSK(3) & (UINT32_MAX >> SFMT_SR1) } };
#endif

#if defined(_M_X64)
/* This function represents the recursion formula. */
#define mm256_recursion(r4, r5, p1, p2)             \
do {                                                \
    __m256i r0, r1, r2, r3;                         \
                                                    \
    r0 = _mm256_load_si256(p2);                     \
    r2 = _mm256_load_si256(p1);                     \
    r0 = _mm256_srli_epi32(r0, SFMT_SR1);           \
    r0 = _mm256_and_si256(r0, r5);                  \
    r1 = _mm256_slli_si256(r2, SFMT_SL2);           \
    r0 = _mm256_xor_si256(r0, r1);                  \
    r0 = _mm256_xor_si256(r0, r2);                  \
    r3 = _mm256_srli_si256(r4, SFMT_SR2);           \
    r1 = _mm256_xor_si256(r0, r3);                  \
    r0 = _mm256_permute2x128_si256(r4, r1, 0x21);   \
    r0 = _mm256_slli_epi32(r0, SFMT_SL1);           \
    r4 = _mm256_xor_si256(r0, r1);                  \
    _mm256_store_si256(p1, r4);                     \
} while (0)

/**
 * This function fills the internal state array with pseudorandom
 * integers.
 * @param sfmt SFMT internal state
 */
static void sfmt_gen_rand_all_avx2()
{
	__m256i r4, r5;
	ptrdiff_t offset;

	assert(SFMT_N % 2 == 0);
	assert(SFMT_POS1 % 2 == 0);
	assert((size_t)sfmt % 32 == 0);
	assert((size_t)&simd_param_mask % 16 == 0);

	r5 = _mm256_broadcastsi128_si256(simd_param_mask.si);
	r4 = _mm256_loadu_si256((__m256i *)(sfmt + SFMT_N) - 1);
	offset = LE_MINUS(SFMT_N - SFMT_POS1) * 16;
	do
		mm256_recursion(r4, r5,
			(__m256i *)((char *)(sfmt + IDX128(SFMT_N - SFMT_POS1)) + offset),
			(__m256i *)((char *)(sfmt + IDX128(SFMT_N)) + offset));
	while (offset += LE_PLUS(32));
	offset = LE_MINUS(SFMT_POS1) * 16;
	do
		mm256_recursion(r4, r5,
			(__m256i *)((char *)(sfmt + IDX128(SFMT_N)) + offset),
			(__m256i *)((char *)(sfmt + IDX128(SFMT_POS1)) + offset));
	while (offset += LE_PLUS(32));
}

/* This function represents the recursion formula. */
#define mm_recursion(r2, r3, r4, p1, p2)    \
do {                                        \
    __m128i r0, r1;                         \
                                            \
    r1 = _mm_load_si128(p1);                \
    r0 = r2;                                \
    r3 = _mm_srli_si128(r3, SFMT_SR2);      \
    r0 = _mm_slli_epi32(r0, SFMT_SL1);      \
    r3 = _mm_xor_si128(r3, r1);             \
    r1 = _mm_slli_si128(r1, SFMT_SL2);      \
    r0 = _mm_xor_si128(r0, r3);             \
    r3 = r2;                                \
    r2 = _mm_load_si128(p2);                \
    r1 = _mm_xor_si128(r1, r0);             \
    r2 = _mm_srli_epi32(r2, SFMT_SR1);      \
    r2 = _mm_and_si128(r2, r4);             \
    r2 = _mm_xor_si128(r2, r1);             \
    _mm_store_si128(p1, r2);                \
} while (0)

/* This function fills the internal state array with pseudorandom
   integers. */
static void sfmt_gen_rand_all_sse2()
{
	__m128i r2, r3, r4;
	ptrdiff_t offset;

	assert((size_t)sfmt % 16 == 0);
	assert((size_t)&simd_param_mask % 16 == 0);

	r2 = _mm_load_si128(sfmt + IDX128(SFMT_N - 1));
	r3 = _mm_load_si128(sfmt + IDX128(SFMT_N - 2));
	r4 = _mm_load_si128(&simd_param_mask.si);
	offset = LE_MINUS(SFMT_N - SFMT_POS1) * 16;
	do
		mm_recursion(r2, r3, r4,
			(__m128i *)((char *)(sfmt + IDX128(SFMT_N - SFMT_POS1)) + offset),
			(__m128i *)((char *)(sfmt + IDX128(SFMT_N)) + offset));
	while (offset += LE_PLUS(16));
	offset = LE_MINUS(SFMT_POS1) * 16;
	do
		mm_recursion(r2, r3, r4,
			(__m128i *)((char *)(sfmt + IDX128(SFMT_N)) + offset),
			(__m128i *)((char *)(sfmt + IDX128(SFMT_POS1)) + offset));
	while (offset += LE_PLUS(16));
}

static void sfmt_gen_rand_all()
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	if (__isa_available >= __ISA_AVAILABLE_AVX2)
		sfmt_gen_rand_all_avx2();
	else
		sfmt_gen_rand_all_sse2();

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
	#undef __ISA_AVAILABLE_AVX
	#undef __ISA_AVAILABLE_ENFSTRG
	#undef __ISA_AVAILABLE_AVX2
}
#elif defined(_M_IX86)
/* This function fills the internal state array with pseudorandom
   integers. */
__declspec(naked) static void __cdecl sfmt_gen_rand_all_avx2()
{
	__asm
	{
		vbroadcasti128 ymm5, xmmword ptr [simd_param_mask]
		vmovdqa ymm4, ymmword ptr [state + IDX128(SFMT_N - 2) * 16]
		mov     eax, LE_MINUS(SFMT_N - SFMT_POS1) * 16

		align   16
	loop1:
		vmovdqa ymm0, ymmword ptr [state + IDX128(SFMT_N) * 16 + eax]
		vmovdqa ymm2, ymmword ptr [state + IDX128(SFMT_N - SFMT_POS1) * 16 + eax]
		vpsrld  ymm0, ymm0, SFMT_SR1
		vpand   ymm0, ymm0, ymm5
		vpslldq ymm1, ymm2, SFMT_SL2
		vpxor   ymm0, ymm0, ymm1
		vpxor   ymm0, ymm0, ymm2
		vpsrldq ymm3, ymm4, SFMT_SR2
		vpxor   ymm1, ymm0, ymm3
		vperm2i128 ymm0, ymm4, ymm1, 21H
		vpslld  ymm0, ymm0, SFMT_SL1
		vpxor   ymm4, ymm0, ymm1
		vmovdqa ymmword ptr [state + IDX128(SFMT_N - SFMT_POS1) * 16 + eax], ymm4
		LE_ADD  eax, 32
		jnz     loop1

		mov     eax, LE_MINUS(SFMT_POS1) * 16

		align   16
	loop2:
		vmovdqa ymm0, ymmword ptr [state + IDX128(SFMT_POS1) * 16 + eax]
		vmovdqa ymm2, ymmword ptr [state + IDX128(SFMT_N) * 16 + eax]
		vpsrld  ymm0, ymm0, SFMT_SR1
		vpand   ymm0, ymm0, ymm5
		vpslldq ymm1, ymm2, SFMT_SL2
		vpxor   ymm0, ymm0, ymm1
		vpxor   ymm0, ymm0, ymm2
		vpsrldq ymm3, ymm4, SFMT_SR2
		vpxor   ymm1, ymm0, ymm3
		vperm2i128 ymm0, ymm4, ymm1, 21H
		vpslld  ymm0, ymm0, SFMT_SL1
		vpxor   ymm4, ymm0, ymm1
		vmovdqa ymmword ptr [state + IDX128(SFMT_N) * 16 + eax], ymm4
		LE_ADD  eax, 32
		jnz     loop2

		vzeroupper
		ret
	}
}

/* This function fills the internal state array with pseudorandom
   integers. */
__declspec(naked) static void __cdecl sfmt_gen_rand_all_sse2()
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [state + IDX128(SFMT_N - 1) * 16]
		movdqa  xmm3, xmmword ptr [state + IDX128(SFMT_N - 2) * 16]
		movdqa  xmm4, xmmword ptr [simd_param_mask]
		mov     eax, LE_MINUS(SFMT_N - SFMT_POS1) * 16

		align   16
	loop1:
		movdqa  xmm1, xmmword ptr [state + IDX128(SFMT_N - SFMT_POS1) * 16 + eax]
		movdqa  xmm0, xmm2
		psrldq  xmm3, SFMT_SR2
		pslld   xmm0, SFMT_SL1
		pxor    xmm3, xmm1
		pslldq  xmm1, SFMT_SL2
		pxor    xmm0, xmm3
		movdqa  xmm3, xmm2
		movdqa  xmm2, xmmword ptr [state + IDX128(SFMT_N) * 16 + eax]
		pxor    xmm1, xmm0
		psrld   xmm2, SFMT_SR1
		pand    xmm2, xmm4
		pxor    xmm2, xmm1
		movdqa  xmmword ptr [state + IDX128(SFMT_N - SFMT_POS1) * 16 + eax], xmm2
		LE_ADD  eax, 16
		jnz     loop1

		mov     eax, LE_MINUS(SFMT_POS1) * 16

		align   16
	loop2:
		movdqa  xmm1, xmmword ptr [state + IDX128(SFMT_N) * 16 + eax]
		movdqa  xmm0, xmm2
		psrldq  xmm3, SFMT_SR2
		pslld   xmm0, SFMT_SL1
		pxor    xmm3, xmm1
		pslldq  xmm1, SFMT_SL2
		pxor    xmm0, xmm3
		movdqa  xmm3, xmm2
		movdqa  xmm2, xmmword ptr [state + IDX128(SFMT_POS1) * 16 + eax]
		pxor    xmm1, xmm0
		psrld   xmm2, SFMT_SR1
		pand    xmm2, xmm4
		pxor    xmm2, xmm1
		movdqa  xmmword ptr [state + IDX128(SFMT_N) * 16 + eax], xmm2
		LE_ADD  eax, 16
		jnz     loop2

		rep ret
	}
}
#endif

#if !defined(_M_X64)
/* This function represents the recursion formula. */
#if !defined(_M_IX86)
#define do_recursion(a, b, c, d)                                                    \
do {                                                                                \
    w128_t __x;                                                                     \
                                                                                    \
    __x.u64[IDX_HI] = ((a)->u64[IDX_HI] << (SFMT_SL2 * 8)) |                        \
                      ((a)->u64[IDX_LO] >> (64 - SFMT_SL2 * 8));                    \
    __x.u64[IDX_LO] =  (a)->u64[IDX_LO] << (SFMT_SL2 * 8);                          \
    __x.u[0] ^= ((b)->u[0] >> SFMT_SR1) & (SFMT_MSK(0) & (UINT32_MAX >> SFMT_SR1)); \
    __x.u[1] ^= ((b)->u[1] >> SFMT_SR1) & (SFMT_MSK(1) & (UINT32_MAX >> SFMT_SR1)); \
    __x.u[2] ^= ((b)->u[2] >> SFMT_SR1) & (SFMT_MSK(2) & (UINT32_MAX >> SFMT_SR1)); \
    __x.u[3] ^= ((b)->u[3] >> SFMT_SR1) & (SFMT_MSK(3) & (UINT32_MAX >> SFMT_SR1)); \
    __x.u64[IDX_LO] ^= ((c)->u64[IDX_LO] >> (SFMT_SR2 * 8)) |                       \
                       ((c)->u64[IDX_HI] << (64 - SFMT_SR2 * 8));                   \
    __x.u64[IDX_HI] ^=  (c)->u64[IDX_HI] >> (SFMT_SR2 * 8);                         \
    __x.u[0] ^= (d)->u[0] << SFMT_SL1;                                              \
    __x.u[1] ^= (d)->u[1] << SFMT_SL1;                                              \
    __x.u[2] ^= (d)->u[2] << SFMT_SL1;                                              \
    __x.u[3] ^= (d)->u[3] << SFMT_SL1;                                              \
    (a)->u64[0] ^= __x.u64[0];                                                      \
    (a)->u64[1] ^= __x.u64[1];                                                      \
} while (0)
#else
__declspec(naked) static void __cdecl do_recursion(w128_t *a, w128_t *b, w128_t *c, w128_t *d)
{
#if defined(__LITTLE_ENDIAN__)
	#define IDX0  0
	#define IDX1  4
	#define IDX2  8
	#define IDX3 12
#else
	#define IDX0 12
	#define IDX1  8
	#define IDX2  4
	#define IDX3  0
#endif

	__asm
	{
		#define a (esp + 4)
		#define b (esp + 8)
		#define c (esp + 12)
		#define d (esp + 16)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebp, dword ptr [a   +   16]

		// x.u[3] = (a->u[3] << (SFMT_SL2 * 8)) | (a->u[2] >> (32 - SFMT_SL2 * 8));
		// x.u[2] = (a->u[2] << (SFMT_SL2 * 8)) | (a->u[1] >> (32 - SFMT_SL2 * 8));
		// x.u[1] = (a->u[1] << (SFMT_SL2 * 8)) | (a->u[0] >> (32 - SFMT_SL2 * 8));
		// x.u[0] =  a->u[0] << (SFMT_SL2 * 8);
		mov     eax, dword ptr [ebp + IDX0]
		mov     ecx, dword ptr [ebp + IDX1]
		mov     edx, dword ptr [ebp + IDX2]
		mov     ebx, dword ptr [ebp + IDX3]
		shl     ebx, SFMT_SL2 * 8
		mov     esi, edx
		shr     esi, 32 - SFMT_SL2 * 8
		mov     edi, ecx
		shl     edx, SFMT_SL2 * 8
		or      ebx, esi
		shr     edi, 32 - SFMT_SL2 * 8
		mov     esi, eax
		shl     ecx, SFMT_SL2 * 8
		or      edx, edi
		shr     esi, 32 - SFMT_SL2 * 8
		mov     ebp, dword ptr [b   +   16]
		shl     eax, SFMT_SL2 * 8
		or      ecx, esi

		// x.u[0] ^= (b->u[0] >> SFMT_SR1) & (SFMT_MSK1 & (UINT32_MAX >> SFMT_SR1));
		// x.u[1] ^= (b->u[1] >> SFMT_SR1) & (SFMT_MSK2 & (UINT32_MAX >> SFMT_SR1));
		// x.u[2] ^= (b->u[2] >> SFMT_SR1) & (SFMT_MSK3 & (UINT32_MAX >> SFMT_SR1));
		// x.u[3] ^= (b->u[3] >> SFMT_SR1) & (SFMT_MSK4 & (UINT32_MAX >> SFMT_SR1));
		mov     esi, dword ptr [ebp + IDX0]
		mov     edi, dword ptr [ebp + IDX1]
		shr     esi, SFMT_SR1
		shr     edi, SFMT_SR1
		and     esi, SFMT_MSK1 and (INT32_MAX shr (SFMT_SR1 - 1))
		and     edi, SFMT_MSK2 and (INT32_MAX shr (SFMT_SR1 - 1))
		xor     eax, esi
		mov     esi, dword ptr [ebp + IDX2]
		xor     ecx, edi
		shr     esi, SFMT_SR1
		mov     edi, dword ptr [ebp + IDX3]
		shr     edi, SFMT_SR1
		mov     ebp, dword ptr [c   +   16]
		and     esi, SFMT_MSK3 and (INT32_MAX shr (SFMT_SR1 - 1))
		and     edi, SFMT_MSK4 and (INT32_MAX shr (SFMT_SR1 - 1))
		xor     edx, esi
		xor     ebx, edi

		// x.u[0] ^= (c->u[0] >> (SFMT_SR2 * 8)) | (c->u[1] << (32 - SFMT_SR2 * 8));
		// x.u[1] ^= (c->u[1] >> (SFMT_SR2 * 8)) | (c->u[2] << (32 - SFMT_SR2 * 8));
		// x.u[2] ^= (c->u[2] >> (SFMT_SR2 * 8)) | (c->u[3] << (32 - SFMT_SR2 * 8));
		// x.u[3] ^=  c->u[3] >> (SFMT_SR2 * 8);
		mov     esi, dword ptr [ebp + IDX0]
		mov     edi, dword ptr [ebp + IDX1]
		ror     edi, SFMT_SR2 * 8
		shr     esi, SFMT_SR2 * 8
		xor     eax, edi
		and     edi, INT32_MAX shr (SFMT_SR2 * 8 - 1)
		xor     eax, esi
		xor     ecx, edi
		xor     eax, edi
		mov     esi, dword ptr [ebp + IDX2]
		mov     edi, dword ptr [ebp + IDX3]
		ror     esi, SFMT_SR2 * 8
		ror     edi, SFMT_SR2 * 8
		mov     ebp, dword ptr [d   +   16]
		xor     ecx, esi
		xor     edx, edi
		and     esi, INT32_MAX shr (SFMT_SR2 * 8 - 1)
		and     edi, INT32_MAX shr (SFMT_SR2 * 8 - 1)
		xor     edx, esi
		xor     ecx, esi
		mov     esi, dword ptr [ebp + IDX0]
		xor     edx, edi
		xor     ebx, edi

		// x.u[0] ^= d->u[0] << SFMT_SL1;
		// x.u[1] ^= d->u[1] << SFMT_SL1;
		// x.u[2] ^= d->u[2] << SFMT_SL1;
		// x.u[3] ^= d->u[3] << SFMT_SL1;
		shl     esi, SFMT_SL1
		mov     edi, dword ptr [ebp + IDX1]
		shl     edi, SFMT_SL1
		xor     eax, esi
		mov     esi, dword ptr [ebp + IDX2]
		xor     ecx, edi
		shl     esi, SFMT_SL1
		mov     edi, dword ptr [ebp + IDX3]
		shl     edi, SFMT_SL1
		mov     ebp, dword ptr [a   +   16]
		xor     edx, esi
		xor     ebx, edi

		// a->u[0] ^= x.u[0];
		// a->u[1] ^= x.u[1];
		// a->u[2] ^= x.u[2];
		// a->u[3] ^= x.u[3];
		xor     dword ptr [ebp + IDX0], eax
		xor     dword ptr [ebp + IDX1], ecx
		xor     dword ptr [ebp + IDX2], edx
		xor     dword ptr [ebp + IDX3], ebx

		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef a
		#undef b
		#undef c
		#undef d
	}

	#undef IDX0
	#undef IDX1
	#undef IDX2
	#undef IDX3
}
#endif

/* This function fills the internal state array with pseudorandom
   integers. */
#if !defined(_M_IX86)
static void sfmt_gen_rand_all()
{
	w128_t *a, *b, *c, *d;

	a = state + IDX128(0);
	b = state + IDX128(SFMT_POS1);
	c = state + IDX128(SFMT_N - 2);
	d = state + IDX128(SFMT_N - 1);
	do {
		do_recursion(a, b, c, d);
		c = d;
		d = a;
		LE_POST_INC(a);
		LE_POST_INC(b);
	} while (a != state + IDX128(SFMT_N - SFMT_POS1));
	b = state + IDX128(0);
	do {
		do_recursion(a, b, c, d);
		c = d;
		d = a;
		LE_POST_INC(a);
		LE_POST_INC(b);
	} while (a != state + IDX128(SFMT_N));
}
#else
__declspec(naked) static void sfmt_gen_rand_all_generic()
{
	__asm
	{
		push    ebx
		push    esi
		push    edi
		mov     ebx, offset state + IDX128(0) * 16
		mov     esi, offset state + IDX128(SFMT_POS1) * 16
		mov     eax, offset state + IDX128(SFMT_N - 2) * 16
		mov     edi, offset state + IDX128(SFMT_N - 1) * 16
		sub     esp, 16

		align   16
	loop1:
		mov     dword ptr [esp     ], ebx
		mov     dword ptr [esp +  4], esi
		mov     dword ptr [esp +  8], eax
		mov     dword ptr [esp + 12], edi
		call    do_recursion
		mov     eax, edi
		mov     edi, ebx
		LE_ADD  ebx, 16
		LE_ADD  esi, 16
		cmp     ebx, offset state + IDX128(SFMT_N - SFMT_POS1) * 16
		jne     loop1

		mov     esi, offset state + IDX128(0) * 16

		align   16
	loop2:
		mov     dword ptr [esp     ], ebx
		mov     dword ptr [esp +  4], esi
		mov     dword ptr [esp +  8], eax
		mov     dword ptr [esp + 12], edi
		call    do_recursion
		mov     eax, edi
		mov     edi, ebx
		LE_ADD  ebx, 16
		LE_ADD  esi, 16
		cmp     ebx, offset state + IDX128(SFMT_N) * 16
		jne     loop2

		mov     edi, dword ptr [esp + 16]
		add     esp, 20
		pop     esi
		pop     ebx
		ret
	}
}
#endif
#endif

#if defined(_M_IX86)
__declspec(naked) static void sfmt_gen_rand_all_cpu_dispatch()
{
	#define __ISA_AVAILABLE_X86     0
	#define __ISA_AVAILABLE_SSE2    1
	#define __ISA_AVAILABLE_SSE42   2
	#define __ISA_AVAILABLE_AVX     3
	#define __ISA_AVAILABLE_ENFSTRG 4
	#define __ISA_AVAILABLE_AVX2    5

	extern unsigned int __isa_available;

	static void *table[] = {
		(void *)sfmt_gen_rand_all_generic,
		(void *)sfmt_gen_rand_all_sse2,
		(void *)sfmt_gen_rand_all_sse2,
		(void *)sfmt_gen_rand_all_sse2,
		(void *)sfmt_gen_rand_all_sse2,
	};

	__asm
	{
		mov     ecx, dword ptr [__isa_available]
		mov     eax, offset sfmt_gen_rand_all_avx2
		cmp     ecx, __ISA_AVAILABLE_AVX2
		jae     L1
		mov     eax, dword ptr [table + ecx * 4]
	L1:
		mov     dword ptr [sfmt_gen_rand_all], eax
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

/*----------------
  PUBLIC FUNCTIONS
  ----------------*/
/* This function initializes the internal state array with a 32-bit
   integer seed. */
#if !defined(_M_IX86)
void __cdecl srandom(unsigned int seed)
{
	uint32_t x;
	size_t i;
#if defined(__BIG_ENDIAN__)
	uint32_t j;
#endif

	x = seed;
	i = IDX32(0);
#if defined(__LITTLE_ENDIAN__)
	do {
		sfmt32[i++] = x;
		x = ((x >> 30) ^ x) * 1812433253UL + (uint32_t)i;
	} while (i < SFMT_N32 - 1);
#else
	j = 0;
	do {
		sfmt32[i] = x;
		x = ((x >> 30) ^ x) * 1812433253UL + ++j;
	} while (--i);
#endif
	sfmt32[IDX32(SFMT_N32 - 1)] = x;
	idx = IDX32(SFMT_N32);
#if SFMT_PARITY1 || SFMT_PARITY2 || SFMT_PARITY3 || SFMT_PARITY4
	/* certificate the period of 2^{MEXP} */
	x =  sfmt32[IDX32(0)] & SFMT_PARITY1;
	x ^= sfmt32[IDX32(1)] & SFMT_PARITY2;
	x ^= sfmt32[IDX32(2)] & SFMT_PARITY3;
	x ^= sfmt32[IDX32(3)] & SFMT_PARITY4;
	x ^= x << 16;
	x ^= x << 8;
	x ^= x << 4;
	x ^= x << 2;
	x ^= x << 1;
	if ((int32_t)x >= 0)
		/* check NG, and modification */
#if SFMT_PARITY1
		sfmt32[IDX32(0)] ^= 1 << BSF32(SFMT_PARITY1);
#elif SFMT_PARITY2
		sfmt32[IDX32(1)] ^= 1 << BSF32(SFMT_PARITY2);
#elif SFMT_PARITY3
		sfmt32[IDX32(2)] ^= 1 << BSF32(SFMT_PARITY3);
#elif SFMT_PARITY4
		sfmt32[IDX32(3)] ^= 1 << BSF32(SFMT_PARITY4);
#endif
#endif
}
#else
__declspec(naked) void __cdecl srandom(unsigned int seed)
{
#if SFMT_PARITY1
	#define PARITY_WORD  SFMT_PARITY1
	#define PARITY_INDEX 0
#elif SFMT_PARITY2
	#define PARITY_WORD  SFMT_PARITY2
	#define PARITY_INDEX 1
#elif SFMT_PARITY3
	#define PARITY_WORD  SFMT_PARITY3
	#define PARITY_INDEX 2
#elif SFMT_PARITY4
	#define PARITY_WORD  SFMT_PARITY4
	#define PARITY_INDEX 3
#else
	#define PARITY_WORD  0
	#define PARITY_INDEX -1
#endif

	__asm
	{
		#define seed (esp + 4)

#if defined(__LITTLE_ENDIAN__)
		mov     eax, dword ptr [seed]
		xor     ecx, ecx
		mov     edx, eax

		align   16
	loop1:
		shr     eax, 30
		mov     dword ptr [state + ecx * 4], edx
		xor     eax, edx
		inc     ecx
		imul    eax, 1812433253
		add     eax, ecx
		cmp     ecx, SFMT_N32 - 1
		mov     edx, eax
		jb      loop1
#else
		mov     eax, dword ptr [seed]
		mov     ecx, SFMT_N32 - 1
		mov     edx, eax
		push    esi
		xor     esi, esi

		align   16
	loop1:
		shr     eax, 30
		mov     dword ptr [state + ecx * 4], edx
		xor     eax, edx
		inc     esi
		imul    eax, 1812433253
		add     eax, esi
		dec     ecx
		mov     edx, eax
		jnz     loop1
#endif

#if PARITY_WORD
#if SFMT_PARITY1 && !SFMT_PARITY2 && !SFMT_PARITY3 && SFMT_PARITY4
		mov     eax, dword ptr [state + IDX32(0) * 4]
		mov     ecx, dword ptr [state + IDX32(3) * 4]
		and     eax, SFMT_PARITY1
		and     ecx, SFMT_PARITY4
		mov     dword ptr [state + IDX32(SFMT_N32 - 1) * 4], edx
		xor     eax, ecx
		mov     dword ptr [idx], IDX32(SFMT_N32)
		mov     ecx, eax
#else
		mov     eax, dword ptr [state + IDX32(0) * 4]
		mov     ecx, dword ptr [state + IDX32(1) * 4]
		and     eax, SFMT_PARITY1
		and     ecx, SFMT_PARITY2
		mov     dword ptr [state + IDX32(SFMT_N32 - 1) * 4], edx
		xor     eax, ecx
		mov     ecx, dword ptr [state + IDX32(2) * 4]
		mov     edx, dword ptr [state + IDX32(3) * 4]
		and     ecx, SFMT_PARITY3
		and     edx, SFMT_PARITY4
		xor     eax, ecx
		mov     dword ptr [idx], IDX32(SFMT_N32)
		xor     eax, edx
		mov     ecx, eax
#endif
		shr     eax, 16
		xor     edx, edx
		xor     eax, ecx
		xor     ecx, ecx
		xor     al, ah
#if defined(__BIG_ENDIAN__)
		pop     esi
#endif
		mov     dl, al
		mov     cl, al
		shl     edx, 4
		xor     ecx, edx
		lea     eax, [ecx * 4]
		xor     ecx, eax
		mov     edx, dword ptr [state + IDX32(PARITY_INDEX) * 4]
		mov     eax, ecx
		add     ecx, ecx
		xor     eax, ecx
		xor     edx, 1 shl MASM_BSF32(PARITY_WORD)
		test    al, al
		js      epilog
		mov     dword ptr [state + IDX32(PARITY_INDEX) * 4], edx
	epilog:
#elif defined(__BIG_ENDIAN__)
		pop     esi
#endif
		ret

		#undef seed
	}

	#undef PARITY_WORD
	#undef PARITY_INDEX
}
#endif

/* This function generates and returns 32-bit pseudorandom number.
   srandom must be called before this function. */
#if !defined(_M_IX86)
uint32_t __cdecl rand32()
{
	if (idx == IDX32(SFMT_N32)) {
		sfmt_gen_rand_all();
		idx = IDX32(0);
	}
	return sfmt32[LE_POST_INC(idx)];
}
#else
__declspec(naked) uint32_t __cdecl rand32()
{
	__asm
	{
		mov     ecx, dword ptr [idx]
#if defined(__LITTLE_ENDIAN__)
		cmp     ecx, SFMT_N32
		jb      get_dword
		call    dword ptr [sfmt_gen_rand_all]
		xor     ecx, ecx
		jmp     get_dword
#else
		test    ecx, ecx
		jns     get_dword
		call    dword ptr [sfmt_gen_rand_all]
		mov     ecx, IDX32(0)
#endif

		align   16
	get_dword:
		mov     eax, dword ptr [state + ecx * 4]
		LE_INC  ecx
		mov     dword ptr [idx], ecx
		ret
	}
}
#endif

/* This function generates and returns 64-bit pseudorandom number.
   srandom must be called before this function. */
#if !defined(_M_IX86)
uint64_t __msreturn __cdecl rand64()
{
	uint64_t x;

#if defined(__LITTLE_ENDIAN__)
	if (idx >= SFMT_N32 - 1) {
		if (idx == SFMT_N32 - 1) {
#else
	if ((ptrdiff_t)idx <= 0) {
		if (idx == 0) {
#endif
			((uint32_t *)&x)[IDX_LO] = sfmt32[IDX32(SFMT_N32 - 1)];
			sfmt_gen_rand_all();
			((uint32_t *)&x)[IDX_HI] = sfmt32[IDX32(0)];
			idx = IDX32(1);
			return x;
		}
		sfmt_gen_rand_all();
		idx = IDX32(0);
	}
	x = *(uint64_t *)(sfmt32 - IDX_LO + idx);
	idx += LE_PLUS(2);
	return x;
}
#else
__declspec(naked) uint64_t __msreturn __cdecl rand64()
{
	__asm
	{
		mov     ecx, dword ptr [idx]
#if defined(__LITTLE_ENDIAN__)
		cmp     ecx, SFMT_N32 - 1
		jb      get_qword
		mov     eax, dword ptr [state + (SFMT_N32 - 1) * 4]
		jne     generate
		push    eax
		call    dword ptr [sfmt_gen_rand_all]
		xor     ecx, ecx
		pop     eax
		jmp     get_high

		align   16
	generate:
		call    dword ptr [sfmt_gen_rand_all]
		xor     ecx, ecx
#else
		test    ecx, ecx
		jg      get_qword
		mov     eax, dword ptr [state]
		jnz     generate
		push    eax
		call    dword ptr [sfmt_gen_rand_all]
		mov     ecx, IDX32(0)
		pop     eax
		jmp     get_high

		align   16
	generate:
		call    dword ptr [sfmt_gen_rand_all]
		mov     ecx, IDX32(0)
#endif
	get_qword:
		mov     eax, dword ptr [state + ecx * 4]
		LE_INC  ecx
	get_high:
		mov     edx, dword ptr [state + ecx * 4]
		LE_INC  ecx
		mov     dword ptr [idx], ecx
		ret
	}
}
#endif

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range [-FLT_MAX, FLT_MAX).
 */
#if !defined(_M_IX86)
uint32_t __cdecl internal_randf32()
{
	uint32_t x;

	while ((x = rand32()) * 2 >= 0x7F800000 * 2);
	return x;
}
#else
__declspec(naked) uint32_t __cdecl internal_randf32()
{
	__asm
	{
	loop1:
		call    rand32
		mov     ecx, eax
		add     eax, eax
		cmp     eax, 0x7F800000 * 2
		jae     loop1

		mov     eax, ecx
		ret
	}
}
#endif

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [-DBL_MAX, DBL_MAX).
 */
#if !defined(_M_IX86)
uint64_t __msreturn __cdecl internal_randf64()
{
	uint64_t x;

	x = rand64();
	while (x * 2 >= 0x7FF0000000000000 * 2)
		x = ((uint64_t)rand32() << 32) | (x >> 32);
	return x;
}
#else
__declspec(naked) uint64_t __msreturn __cdecl internal_randf64()
{
	__asm
	{
		call    rand64
		push    edx
		add     edx, edx
		cmp     edx, 0x7FF00000 * 2
		jb      epilog

		align   16
	loop1:
		call    rand32
		mov     edx, eax
		pop     eax
		push    edx
		add     edx, edx
		cmp     edx, 0x7FF00000 * 2
		jae     loop1

	epilog:
		pop     edx
		ret
	}
}
#endif

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range [0, 1).
 */
#if !defined(_M_IX86)
uint32_t __cdecl internal_randf32ge0lt1()
{
	uint32_t x;

	while ((x = rand32()) >= 0x3F800000 * 4);
	return x >> 2;
}
#else
__declspec(naked) uint32_t __cdecl internal_randf32ge0lt1()
{
	__asm
	{
	loop1:
		call    rand32
		cmp     eax, 0x3F800000 * 4
		jae     loop1

		shr     eax, 2
		ret
	}
}
#endif

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range [0, 1].
 */
#if !defined(_M_IX86)
uint32_t __cdecl internal_randf32ge0le1()
{
	uint32_t x;

	while ((x = rand32()) >= (0x3F800000 + 1) * 4);
	return x >> 2;
}
#else
__declspec(naked) uint32_t __cdecl internal_randf32ge0le1()
{
	__asm
	{
	loop1:
		call    rand32
		cmp     eax, (0x3F800000 + 1) * 4
		jae     loop1

		shr     eax, 2
		ret
	}
}
#endif

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range (0, 1].
 */
#if !defined(_M_IX86)
uint32_t __cdecl internal_randf32gt0le1()
{
	uint32_t x;

	while ((x = rand32()) >= 0x3F800000 * 4);
	return (x >> 2) + 1;
}
#else
__declspec(naked) uint32_t __cdecl internal_randf32gt0le1()
{
	__asm
	{
	loop1:
		call    rand32
		cmp     eax, 0x3F800000 * 4
		jae     loop1

		shr     eax, 2
		inc     eax
		ret
	}
}
#endif

/**
 * This function generates and returns single precision pseudorandom
 * number which distributes uniformly in the range (0, 1).
 */
#if !defined(_M_IX86)
uint32_t __cdecl internal_randf32gt0lt1()
{
	uint32_t x;

	while ((x = rand32()) >= (0x3F800000 - 1) * 4);
	return (x >> 2) + 1;
}
#else
__declspec(naked) uint32_t __cdecl internal_randf32gt0lt1()
{
	__asm
	{
	loop1:
		call    rand32
		cmp     eax, (0x3F800000 - 1) * 4
		jae     loop1

		shr     eax, 2
		inc     eax
		ret
	}
}
#endif

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [0, 1).
 */
#if !defined(_M_IX86)
uint64_t __msreturn __cdecl internal_randf64ge0lt1()
{
	uint64_t x;
	uint32_t y, z;

	y = (uint32_t)((x = rand64()) >> 32);
	while ((x &= 0x3FFFFFFFFFFFFFFF) >= 0x3FF0000000000000)
		x = ((uint64_t)(z = rand32()) << 32) | y, y = z;
	return x;
}
#else
__declspec(naked) uint64_t __msreturn __cdecl internal_randf64ge0lt1()
{
	__asm
	{
		call    rand64
		mov     ecx, edx
		and     edx, 0x3FFFFFFF
		cmp     edx, 0x3FF00000
		jb      done
		push    esi
		mov     esi, ecx

	loop1:
		call    rand32
		mov     edx, eax
		mov     eax, esi
		mov     esi, edx
		and     edx, 0x3FFFFFFF
		cmp     edx, 0x3FF00000
		jae     loop1

		pop     esi
	done:
		rep ret
	}
}
#endif

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [0, 1].
 */
#if !defined(_M_IX86)
uint64_t __msreturn __cdecl internal_randf64ge0le1()
{
	uint64_t x;
	uint32_t y, z;

	y = (uint32_t)((x = rand64()) >> 32);
	while ((x &= 0x3FFFFFFFFFFFFFFF) >= 0x3FF0000000000000 + 1)
		x = ((uint64_t)(z = rand32()) << 32) | y, y = z;
	return x;
}
#else
__declspec(naked) uint64_t __msreturn __cdecl internal_randf64ge0le1()
{
	__asm
	{
		call    rand64
		mov     ecx, edx
		push    esi
		and     ecx, 0x3FFFFFFF
		cmp     eax, 1
		sbb     ecx, 0x3FF00000
		jb      done
		mov     esi, edx

	loop1:
		call    rand32
		mov     ecx, esi
		mov     esi, eax
		and     eax, 0x3FFFFFFF
		cmp     ecx, 1
		sbb     eax, 0x3FF00000
		jae     loop1

		mov     edx, esi
		mov     eax, ecx
	done:
		and     edx, 0x3FFFFFFF
		pop     esi
		ret
	}
}
#endif

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range (0, 1].
 */
#if !defined(_M_IX86)
uint64_t __msreturn __cdecl internal_randf64gt0le1()
{
	uint64_t x;
	uint32_t y, z;

	y = (uint32_t)((x = rand64()) >> 32);
	while ((x &= 0x3FFFFFFFFFFFFFFF) >= 0x3FF0000000000000)
		x = ((uint64_t)(z = rand32()) << 32) | y, y = z;
	return x + 1;
}
#else
__declspec(naked) uint64_t __msreturn __cdecl internal_randf64gt0le1()
{
	__asm
	{
		call    rand64
		mov     ecx, edx
		and     edx, 0x3FFFFFFF
		cmp     edx, 0x3FF00000
		jb      done
		push    esi
		mov     esi, ecx

	loop1:
		call    rand32
		mov     edx, eax
		mov     eax, esi
		mov     esi, edx
		and     edx, 0x3FFFFFFF
		cmp     edx, 0x3FF00000
		jae     loop1

		pop     esi
	done:
		add     eax, 1
		adc     edx, 0
		ret
	}
}
#endif

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range (0, 1).
 */
#if !defined(_M_IX86)
uint64_t __msreturn __cdecl internal_randf64gt0lt1()
{
	uint64_t x;
	uint32_t y, z;

	y = (uint32_t)((x = rand64()) >> 32);
	while ((x &= 0x3FFFFFFFFFFFFFFF) >= 0x3FF0000000000000 - 1)
		x = ((uint64_t)(z = rand32()) << 32) | y, y = z;
	return x + 1;
}
#else
__declspec(naked) uint64_t __msreturn __cdecl internal_randf64gt0lt1()
{
	__asm
	{
		call    rand64
		push    esi
		mov     esi, edx
		and     edx, 0x3FFFFFFF
		sub     eax, 0xFFFFFFFF	// (0x3FF0000000000000 - 1) & UINT32_MAX
		sbb     edx, 0x3FEFFFFF	// (0x3FF0000000000000 - 1) >> 32
		jb      done

	loop1:
		call    rand32
		mov     ecx, esi
		mov     esi, eax
		and     eax, 0x3FFFFFFF
		sub     ecx, 0xFFFFFFFF	// (0x3FF0000000000000 - 1) & UINT32_MAX
		sbb     eax, 0x3FEFFFFF	// (0x3FF0000000000000 - 1) >> 32
		jae     loop1

		mov     edx, eax
		mov     eax, ecx
	done:
		add     edx, 0x3FF00000	// 0x3FF0000000000000 >> 32
		pop     esi
		ret
	}
}
#endif
