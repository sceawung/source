#include "noinline_wchar.h"
#include <windows.h>
#include <intrin.h>

#ifndef _WIN64
#pragma intrinsic(_addcarry_u32)
#define _add_uintptr(a, b, out) _addcarry_u32(0, a, b, out)
#else
#pragma intrinsic(_addcarry_u64)
#define _add_uintptr(a, b, out) _addcarry_u64(0, a, b, out)
#endif

#pragma intrinsic(_byteswap_ulong)
#pragma function(memset)

#ifndef _M_IX86
unsigned char * __cdecl _mbsnset(unsigned char *string, unsigned int c, size_t count)
{
	if (count)
	{
		if (c < 0x100)
		{
			if (!(count = strnlen((char *)string, count)))
				goto DONE;
			memset(string, c, strnlen((char *)string, count));
		}
		else
		{
			if (c >= 0x10000 || !(unsigned char)c)
				goto DONE;
			if (_add_uintptr(count, count, &count))
				count = -1;
			if (!(count = strnlen((char *)string, count)))
				goto DONE;
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, (unsigned char)(c = _byteswap_ulong(c) >> 16)))
				goto DONE;
			wmemset(string, (wchar_t)c, count / 2);
			if (count & 1)
				string[count - 1] = ' ';
		}
	}
DONE:
	return string;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsnset(unsigned char *string, unsigned int c, size_t count)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)
		#define count  (esp + 12)

		mov     ecx, dword ptr [count]
		mov     eax, dword ptr [string]
		test    ecx, ecx
		jz      L1
		cmp     edx, 100H
		jae     L2
		push    ecx
		push    eax
		call    strnlen
		add     esp, 8
		mov     ecx, eax
		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    ecx, ecx
		jz      L1
		push    ecx
		push    edx
		push    eax
		call    memset
		add     esp, 12
	L1:
		rep ret

		align   16
	L2:
		cmp     edx, 10000H
		jae     L5
		test    dl, dl
		jz      L5
		add     ecx, ecx
		jnc     L3
		or      ecx, -1
	L3:
		push    ecx
		push    eax
		call    strnlen
		add     esp, 8
		mov     ecx, eax
		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    ecx, ecx
		jz      L5
		bswap   edx
		shr     edx, 16
		push    ecx
		shr     ecx, 1
		push    ecx
		push    edx
		and     edx, 0FFH
		push    eax
		push    edx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		call    wmemset
		mov     ecx, dword ptr [esp + 12]
		shr     ecx, 1
		jnc     L4
		mov     byte ptr [eax + ecx * 2], ' '
	L4:
		mov     eax, dword ptr [string + 16]
		add     esp, 16
	L5:
		rep ret

		#undef string
		#undef c
		#undef count
	}
}
#endif
