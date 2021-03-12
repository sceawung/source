#pragma function(log10)

#ifndef _M_IX86
#include <math.h>

#ifndef M_LOG10E
#define M_LOG10E 0.434294481903251827651128918916605082
#endif

double __cdecl log10(double x)
{
	return log(x) * M_LOG10E;
}
#else
#include <errno.h>

__declspec(naked) double __cdecl log10(double x)
{
	double __cdecl _CIlog10(/*st0 x*/);

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		jmp     _CIlog10
	}
}

__declspec(naked) double __cdecl _CIlog10(/*st0 x*/)
{
	extern const double fpconst_one;
	extern const double fpconst_fyl2xp1_limit;
	extern const double fpconst_minus_inf;
	extern const double fpconst_nan_ind;
	#define _one       fpconst_one
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
		fxam
		fstsw   ax
		and     ah, 45H
		cmp     ah, 01H
		je      L2                              // x is NaN ?
		ftst
		fstsw   ax
		test    ah, 41H
		jnz     L4                              // x <= 0 ?
		fldlg2                                  // log10(2) : x
		fxch                                    // x : log10(2)
		fld     st(0)                           // x : x : log10(2)
		fsub    qword ptr [_one]                // x-1 : x : log10(2)
		fld     st(0)                           // x-1 : x-1 : x : log10(2)
		fabs                                    // |x-1| : x-1 : x : log10(2)
		fcomp   qword ptr [_limit]              // x-1 : x : log10(2)
		fstsw   ax                              // x-1 : x : log10(2)
		test    ah, 45H
		jz      L3
		ftst
		fstsw   ax
		test    ah, 40H                         // Is Zero ?
		jz      L1
		fabs                                    // log10(1) is +0 in all rounding modes.
	L1:
		fstp    st(1)                           // x-1 : log10(2)
		fyl2xp1                                 // log10(x)
	L2:
		rep ret

		align   16
	L3:
		fstp    st(0)                           // x : log10(2)
		fyl2x                                   // log10(x)
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

	#undef _one
	#undef _limit
	#undef _minus_inf
	#undef _nan_ind
	#undef set_errno
}
#endif
