#include <errno.h>

__declspec(naked) double __cdecl exp2(double x)
{
	extern const double fpconst_one;
	#define _one fpconst_one

#ifndef DISABLE_UCRT
	errno_t * __cdecl _errno();
	#define set_errno(x) \
		__asm   fstp    qword ptr [esp + 4]     /* Save x */ \
		__asm   call    _errno                  /* Get C errno variable pointer */ \
		__asm   fld     qword ptr [esp + 4]     /* Load x */ \
		__asm   mov     dword ptr [eax], x      /* Set error number */
#else
	extern errno_t _terrno;
	#define set_errno(x) \
		__asm   mov     dword ptr [_terrno], x  /* Set error number */
#endif

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		and     ah, 45H                         ; Isolate C0, C2 and C3
		cmp     ah, 40H                         ; Zero ?
		je      L1                              ; Re-direct if x == 0
		test    ah, 01H                         ; NaN or infinity ?
		jnz     L2                              ; Re-direct if x is NaN or infinity
		fld     st(0)                           ; Duplicate x
		frndint                                 ; Round to integer
		fsub    st(1), st(0)                    ; Subtract
		fxch                                    ; Exchange st, st(1)
		f2xm1                                   ; Compute 2 to the (x - 1)
		fadd    qword ptr [_one]                ; 2 to the x
		fscale                                  ; Scale by power of 2
		fstp    st(1)                           ; Set new stack top and pop
		fst     qword ptr [esp - 8]             ; Save x, cast to qword
		fld     qword ptr [esp - 8]             ; Load x
		fxam                                    ; Examine st
		fstsw   ax                              ; Get the FPU status word
		and     ah, 45H                         ; Isolate C0, C2 and C3
		cmp     ah, 05H                         ; Infinity ?
		je      L3                              ; Re-direct if x is infinity (overflow)
		cmp     ah, 40H                         ; Zero ?
		je      L3                              ; Re-direct if x is zero (underflow)
		fstp    st(0)                           ; Set new top of stack
	L1:
		rep ret

		align   16
	L2:
		cmp     ah, 05H                         ; Infinity ?
		je      L4                              ; Re-direct if x is infinity
		set_errno(EDOM)                         ; Set domain error (EDOM)
		ret

		align   16
	L3:
		fstp    st(1)                           ; Set new stack top and pop
	L4:
		set_errno(ERANGE)                       ; Set range error (ERANGE)
		ret
	}

	#undef _one
	#undef set_errno
}
