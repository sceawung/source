#ifndef _M_IX86
#include <math.h>
#include <stdbool.h>

double __cdecl cbrt(double x)
{
	static const double one_third =  1.0 / 3.0;

	bool sign;

	if (x == 0.0)
		return x;
	if (sign = x < 0.0)
		x = -x;
	x = exp(log(x) * one_third);
	if (sign)
		x = -x;
	return x;
}
#else
__declspec(naked) double __cdecl cbrt(double x)
{
	extern double __cdecl _CIlog(/*st0 x*/);
	extern double __cdecl _CIexp(/*st0 x*/);

	static const unsigned int one_third[] = { 0xAAAAAAAB, 0xAAAAAAAA, 0x00003FFD };

	__asm
	{
		fld     qword ptr [esp + 4]             ; Load real from stack
		ftst                                    ; Compare x with zero
		fstsw   ax                              ; Get the FPU status word
		test    ah, 40H                         ; x == 0 ?
		jnz     L1                              ; Re-direct if x == 0
		fstcw   word ptr [esp - 4]              ; Save control word
		mov     cx, word ptr [esp - 4]          ;
		sub     esp, 8                          ; Allocate temporary space
		or      cx, 0300H                       ;
		mov     byte ptr [esp], ah              ; Save result of compare x with zero
		mov     word ptr [esp - 4], cx          ;
		fldcw   word ptr [esp - 4]              ; Set new precision control
		fabs                                    ; Take the absolute value
		call    _CIlog                          ; Compute the natural log(st)
		fld     tbyte ptr [one_third]           ; Load real number 1.0 / 3.0
		fmul                                    ; Compute st / 3
		call    _CIexp                          ; Compute the exponent
		fldcw   word ptr [esp + 4]              ; Restore control word
		mov     al, byte ptr [esp]              ; Load result of compare x with zero
		add     esp, 8                          ; Deallocate temporary space
		test    al, 01H                         ; x >= 0 ?
		jz      L1                              ; Re-direct if x >= 0
		fchs                                    ; Negate the result
	L1:
		rep ret
	}
}
#endif
