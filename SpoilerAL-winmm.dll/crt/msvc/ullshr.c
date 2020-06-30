//***
//ullshr.c - long shift right
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       define unsigned long shift right routine
//           __aullshr
//
//*******************************************************************************

//***
//ullshr - long shift right
//
//Purpose:
//       Does a unsigned Long Shift Right
//       Shifts a long right any number of bits.
//
//Entry:
//       EDX:EAX - long value to be shifted
//       CL    - number of bits to shift by
//
//Exit:
//       EDX:EAX - shifted value
//
//Uses:
//       CL is destroyed.
//
//Exceptions:
//
//*******************************************************************************

#if 0
static
#endif
__declspec(naked) void __cdecl _aullshr()
{
#if 0
	__asm
	{
		//
		// Handle shifts of 64 bits or more (if shifting 64 bits or more, the result
		// depends only on the high order bit of edx).
		//
		cmp     cl, 64
		jae     short RETZERO

		//
		// Handle shifts of between 0 and 31 bits
		//
		cmp     cl, 32
		jae     short MORE32
		shrd    eax, edx, cl
		shr     edx, cl
		ret

		//
		// Handle shifts of between 32 and 63 bits
		//
	MORE32:
		mov     eax, edx
		xor     edx, edx
		and     cl, 31
		shr     eax, cl
		ret

		//
		// return 0 in edx:eax
		//
	RETZERO:
		xor     eax, eax
		xor     edx, edx
		ret
	}
#else
	__asm
	{
		//
		// Handle shifts of 64 bits or more (if shifting 64 bits or more, the result
		// depends only on the high order bit of edx).
		//
		cmp     cl, 64
		jae     retzero

		//
		// Handle shifts of between 0 and 31 bits
		//
		cmp     cl, 32
		jae     more32
		shrd    eax, edx, cl
		shr     edx, cl
		ret

		//
		// Handle shifts of between 32 and 63 bits
		//
		align   16
	more32:
		mov     eax, edx
		je      equal32
		and     cl, 31
		xor     edx, edx
		shr     eax, cl
		ret

		//
		// return 0 in edx:eax
		//
		align   16
	retzero:
		xor     eax, eax
	equal32:
		xor     edx, edx
		ret
	}
#endif
}

#if 0
__declspec(naked) unsigned __int64 __stdcall _ullshr(unsigned __int64 value, unsigned char shift)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 12]
		call    _aullshr
		ret     12
	}
}
#endif