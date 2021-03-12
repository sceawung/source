#include <stdint.h>
#include <math.h>

__declspec(naked) double __cdecl expm1(double x)
{
	double __cdecl _CIexp(/*st0 x*/);

	extern const double fpconst_one;
	extern const double fpconst_minus_one;
	#define _one       fpconst_one
	#define _minus_one fpconst_minus_one

	static const uint64_t a = 0x3EC5CA73DFF66EDE;   //  2.5976694357341699e-06
	static const uint64_t b = 0x3F1DA2E7E515EA71;   //  1.1305370687423614e-04
	static const uint64_t c = 0x3F51DE97C5B063B2;   //  1.0906679297077834e-03
	static const uint64_t d = 0x3F9C8391E0C43166;   //  2.7845649093295143e-02
	static const uint64_t e = 0x3FA41A22F5C5081F;   //  3.9261906148127183e-02
	static const uint64_t f = 0x3FED9093FE30EFD3;   //  9.2389869353018594e-01
	static const uint64_t g = 0x3EF26C937B07E563;   //  1.7570615172578750e-05
	static const uint64_t h = 0x3F43D31708605728;   //  6.0499786991627825e-04
	static const uint64_t i = 0x3F83945DEB77150F;   //  9.5603311045900068e-03
	static const uint64_t j = 0x3FB5D013B98B14B0;   //  8.5206253813413069e-02
	static const uint64_t k = 0x3FDB0D4F9F784ED1;   //  4.2268744061696589e-01

	__asm
	{
		#define x (esp + 4)

		; x > log(0.5) ... -0.69314718055994529 ... 0xBFE62E42FEFA39EF
		; x < log(1.5) ...  0.40546510810816438 ... 0x3FD9F323ECBF984C

		mov     eax, dword ptr [x]
		mov     edx, dword ptr [x + 4]
		mov     ecx, edx
		and     edx, 7FF00000h
		cmp     edx, 7FF00000h
		je      L4                              ; Re-direct if x is NaN or infinity
		fld     qword ptr [x]
		mov     edx, ecx
		and     ecx, 7FFFFFFFh
		or      ecx, eax
		jz      L3                              ; Re-direct if x == 0
		test    edx, edx
		js      L1                              ; Re-direct if x < 0
		cmp     eax, 0ECBF984Ch
		sbb     edx, 3FD9F323h
		jb      L2
		jmp     L6                              ; Re-direct if x >= log(1.5)

		align   16
	L1:
		cmp     eax, 0FEFA39EFh
		sbb     edx, 0BFE62E42h
		jae     L6                              ; Re-direct if x <= log(0.5)
	L2:
		fld     qword ptr [a]                   ; Computes the exponent, minus one
		fmul    st(0), st(1)
		fadd    qword ptr [b]
		fmul    st(0), st(1)
		fadd    qword ptr [c]
		fmul    st(0), st(1)
		fadd    qword ptr [d]
		fmul    st(0), st(1)
		fadd    qword ptr [e]
		fmul    st(0), st(1)
		fld     qword ptr [f]
		fadd    st(1), st(0)
		fld     st(2)
		fmul    qword ptr [g]
		fsubr   qword ptr [h]
		fmul    st(0), st(3)
		fsub    qword ptr [i]
		fmul    st(0), st(3)
		fadd    qword ptr [j]
		fmul    st(0), st(3)
		fsub    qword ptr [k]
		fmul    st(0), st(3)
		fadd
		fdiv
		fmul
	L3:
		rep ret

		align   16
	L4:
		xor     ecx, 0FFF00000h
		or      eax, ecx
		jz      L5                              ; Re-direct if x == -INFINITY
		fld     qword ptr [x]                   ; Load x
		ret

		align   16
	L5:
		fld     qword ptr [_minus_one]          ; Load -1.0
		ret

		align   16
	L6:
		call    _CIexp                          ; Computes the exponent by exp function
		fsub    qword ptr [_one]
		ret

		#undef x
	}

	#undef _one
	#undef _minus_one
}
