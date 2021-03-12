__declspec(naked) int __cdecl InstructionSet()
{
	static int IInstrSet = -1;                              // local name

	__asm
	{
		mov     eax, dword ptr [IInstrSet]

		// Check if this function has been called before
		test    eax, eax
		js      FirstTime                                   // Negative means first time
		rep ret                                             // Early return. Has been called before

		align   16
		// Function has not been called before
	FirstTime:
		push    ebx

		// detect if CPUID instruction supported by microprocessor:
		pushfd
		pop     eax
		btc     eax, 21                                     // check if CPUID bit can toggle
		push    eax
		popfd
		pushfd
		pop     ebx
		xor     ebx, eax
		xor     eax, eax                                    // 0
		bt      ebx, 21
		jc      ISEND                                       // CPUID not supported

		cpuid                                               // get number of CPUID functions
		test    eax, eax
		je      ISEND                                       // function 1 not supported
		mov     eax, 1
		cpuid                                               // get features
		xor     eax, eax                                    // 0

		test    edx, 1                                      // floating point support
		je      ISEND
		bt      edx, 23                                     // MMX support
		jnc     ISEND
		inc     eax                                         // 1

		bt      edx, 15                                     // conditional move support
		jnc     ISEND
		inc     eax                                         // 2

		// check OS support for XMM registers (SSE)
		bt      edx, 24                                     // FXSAVE support by microprocessor
		jnc     ISEND
		push    ecx
		push    edx
		mov     ebx, esp                                    // save stack pointer
		sub     esp, 200H                                   // allocate space for FXSAVE
		and     esp, -10H                                   // align by 16

		#define TESTDATA 0D95A34BEH                         // random test value
		#define TESTPS   10CH                               // position to write TESTDATA = upper part of XMM6 image

		fxsave  [esp]                                       // save FP/MMX and XMM registers
		mov     ecx, dword ptr [esp + TESTPS]               // read part of XMM6 register
		xor     dword ptr [esp + TESTPS], TESTDATA          // change value
		fxrstor [esp]                                       // load changed value into XMM6
		mov     dword ptr [esp + TESTPS], ecx               // restore old value in buffer
		fxsave  [esp]                                       // save again
		mov     edx, dword ptr [esp + TESTPS]               // read changed XMM6 register
		mov     dword ptr [esp + TESTPS], ecx               // restore old value
		fxrstor [esp]                                       // load old value into XMM6
		xor     ecx, edx                                    // get difference between old and new value
		mov     esp, ebx                                    // restore stack pointer
		cmp     ecx, TESTDATA                               // test if XMM6 was changed correctly
		pop     edx
		pop     ecx
		jne     ISEND

		#undef TESTDATA
		#undef TESTPS

		bt      edx, 25                                     // SSE support by microprocessor
		jnc     ISEND
		inc     eax                                         // 3

		bt      edx, 26                                     // SSE2 support by microprocessor
		jnc     ISEND
		inc     eax                                         // 4

		test    ecx, 1                                      // SSE3 support by microprocessor
		je      ISEND
		inc     eax                                         // 5

		bt      ecx, 9                                      // Suppl-SSE3 support by microprocessor
		jnc     ISEND
		inc     eax                                         // 6

		bt      ecx, 19                                     // SSE4.1 support by microprocessor
		jnc     ISEND
		mov     al, 8                                       // 8

		bt      ecx, 23                                     // POPCNT support by microprocessor
		jnc     ISEND
		inc     eax                                         // 9

		bt      ecx, 20                                     // SSE4.2 support by microprocessor
		jnc     ISEND
		inc     eax                                         // 10

		// check OS support for YMM registers (AVX)
		bt      ecx, 27                                     // OSXSAVE: XGETBV supported
		jnc     ISEND
		pushad
		xor     ecx, ecx
		xgetbv                                              // db 0FH, 01H, 0D0H         ; XGETBV
		and     eax, 6
		cmp     eax, 6                                      // AVX support by OS
		popad
		jnz     ISEND

		bt      ecx, 28                                     // AVX support by microprocessor
		jnc     ISEND
		inc     eax                                         // 11

		bt      ecx, 1                                      // PCLMUL support
		jnc     ISEND
		bt      ecx, 25                                     // AES support
		jnc     ISEND
		inc     eax                                         // 12

		push    eax
		push    ecx
		mov     eax, 7
		xor     ecx, ecx
		cpuid                                               // check for AVX2
		bt      ebx, 5
		pop     ecx
		pop     eax
		jnc     ISEND
		inc     eax                                         // 13

		// 14 or above = FMA3, F16C, BMI1, BMI2, LZCNT
		bt      ecx, 12                                     // FMA3
		jnc     ISEND
		bt      ecx, 29                                     // F16C
		jnc     ISEND
		bt      ebx, 3                                      // BMI1
		jnc     ISEND
		bt      ebx, 8                                      // BMI2
		jnc     ISEND

		push    eax
		push    ebx
		push    ecx
		mov     eax, 80000001H
		cpuid
		bt      ecx, 5                                      // LZCNT
		pop     ecx
		pop     ebx
		pop     eax
		jnc     ISEND
		inc     eax                                         // 14

		bt      ebx, 16                                     // AVX512f
		jnc     ISEND
		pushad
		xor     ecx, ecx
		xgetbv
		and     al, 60H
		cmp     al, 60H                                     // AVX512 support by OS
		popad
		jnz     ISEND

		inc     eax                                         // 15

		bt      ebx, 17                                     // AVX512DQ
		jnc     ISEND
		bt      ebx, 30                                     // AVX512BW
		jnc     ISEND
		bt      ebx, 31                                     // AVX512VL
		jnc     ISEND
		inc     eax                                         // 16

	ISEND:
		mov     dword ptr [IInstrSet], eax                  // save value in public variable IInstrSet
		pop     ebx
		ret                                                 // return value is in eax
	}
}
