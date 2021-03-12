#include <math.h>
#include <stdint.h>

#ifndef fpclassify
#ifdef _M_IX86
__declspec(naked)
#endif
int __cdecl fpclassify(double x)
{
#else
short __cdecl _dtest(double *px)
{
	extern short __cdecl _dclass(double x);
	return _dclass(*px);
}
#ifdef _M_IX86
__declspec(naked)
#endif
short __cdecl _dclass(double x)
{
#endif
#if INTPTR_MAX > INT32_MAX
	#define U64(x) (*(uint64_t *)&(x))

	return
		(U64(x) *= 2) ?
			U64(x) < UINT64_C(0x7FF0000000000000) * 2 ?
				U64(x) >= UINT64_C(0x0010000000000000) * 2 ?
					FP_NORMAL :
					FP_SUBNORMAL :
				U64(x) == UINT64_C(0x7FF0000000000000) * 2 ?
					FP_INFINITE :
					FP_NAN :
			FP_ZERO;

	#undef U64
#elif !defined(_M_IX86)
	#define LSW(x) ((uint32_t *)&(x))[0]
	#define MSW(x) ((uint32_t *)&(x))[1]

	uint32_t i;

	return
		(i = MSW(x) * 2) ?
			i < UINT32_C(0x7FF00000) * 2 ?
				i >= UINT32_C(0x00100000) * 2 ?
					FP_NORMAL :
					FP_SUBNORMAL :
			!((i ^ (UINT32_C(0x7FF00000) * 2)) | LSW(x)) ?
				FP_INFINITE :
				FP_NAN :
		!LSW(x) ?
			FP_ZERO :
			FP_SUBNORMAL;

	#undef LSW
	#undef MSW
#else
	__asm
	{
		mov     ecx, dword ptr [esp + 8]        ; Load MSW
		or      eax, -1                         ; Set result to FP_NORMAL(-1)
		add     ecx, ecx                        ; Take the absolute value
		jz      zero_or_subnormal               ; Re-direct if (MSW of x) * 2 == 0
		cmp     ecx, 7FF00000H * 2              ; x is infinity or NaN ?
		jae     infinite_or_nan                 ; Re-direct if x is infinity or NaN
		cmp     ecx, 00100000H * 2              ; x < DBL_MIN ?
		jb      subnormal                       ; Re-direct if x < DBL_MIN
		rep ret

		align   16
	zero_or_subnormal:
		cmp     dword ptr [esp + 4], 0          ; x == 0 ?
		je      zero                            ; Re-direct if x == 0
	subnormal:
		dec     eax                             ; Set result to FP_SUBNORMAL(-2)
		ret

		align   16
	infinite_or_nan:
		mov     edx, dword ptr [esp + 4]        ; Load LSW
		xor     ecx, 7FF00000H * 2              ; Compare |x| with HUGE_VAL
		xor     eax, eax                        ;
		or      ecx, edx                        ;
		setnz   al                              ;
	zero:
		inc     eax                             ; Set result to FP_ZERO(0) or FP_INFINITE(1) or FP_NAN(2)
		ret
	}
#endif
}
