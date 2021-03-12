#include <errno.h>

__declspec(naked) double __cdecl log2(double x)
{
	extern const double fpconst_fyl2xp1_limit;
	extern const double fpconst_minus_inf;
	extern const double fpconst_nan_ind;
	#define _limit     fpconst_fyl2xp1_limit
	#define _minus_inf fpconst_minus_inf
	#define _nan_ind   fpconst_nan_ind

#ifndef DISABLE_UCRT
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fld     qword ptr [esp + 4]             // x
		fxam
		fstsw   ax
		and     ah, 45H
		cmp     ah, 01H
		je      L2                              // x is NaN ?
		ftst
		fstsw   ax
		test    ah, 41H
		jnz     L4                              // x <= 0 ?
		fld1                                    // 1 : x
		fxch                                    // x : 1
		fld     st(0)                           // x : x : 1
		fsub    st(0), st(2)                    // x-1 : x : 1
		fld     st(0)                           // x-1 : x-1 : x : 1
		fabs                                    // |x-1| : x-1 : x : 1
		fcomp   qword ptr [_limit]              // x-1 : x : 1
		fstsw   ax                              // x-1 : x : 1
		test    ah, 45H
		jz      L3
		ftst
		fstsw   ax
		test    ah, 40H                         // Is Zero ?
		jz      L1
		fabs                                    // log10(1) is +0 in all rounding modes.
	L1:
		fstp    st(1)                           // x-1 : 1
		fyl2xp1                                 // log2(x)
	L2:
		rep ret

		align   16
	L3:
		fstp    st(0)                           // x : 1
		fyl2x                                   // log2(x)
		ret

		align   16
	L4:
		fstp    st(0)
		test    ah, 40H
		jnz     L5                              // x == 0 ?
		set_errno(EDOM)                         // Set domain error (EDOM)
		fld     qword ptr [_nan_ind]            // Load NaN(indeterminate)
		ret

		align   16
	L5:
		set_errno(ERANGE)                       // Set range error (ERANGE)
		fld     qword ptr [_minus_inf]          // Load -Inf
		ret
	}

	#undef _limit
	#undef _minus_inf
	#undef _nan_ind
	#undef set_errno
}
