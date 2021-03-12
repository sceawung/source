#include "noinline_wchar.h"
#include <windows.h>
#include <intrin.h>

#pragma intrinsic(_byteswap_ulong)
#pragma function(memset)

#ifndef _M_IX86
unsigned char * __cdecl _mbsnbset(unsigned char *string, unsigned int c, size_t count)
{
	if (count && (count = strnlen((char *)string, count)))
	{
		if (c < 0x100)
		{
			memset(string, c, count);
		}
		else
		{
			if (c >= 0x10000 || !(unsigned char)c)
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
__declspec(naked) unsigned char * __cdecl _mbsnbset(unsigned char *string, unsigned int c, size_t count)
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
		push    ecx
		push    eax
		call    strnlen
		add     esp, 8
		mov     ecx, eax
		mov     edx, dword ptr [c]
		mov     eax, dword ptr [string]
		test    ecx, ecx
		jz      L1
		cmp     edx, 100H
		jae     L2
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
		jae     L4
		test    dl, dl
		jz      L4
		bswap   edx
		shr     ecx, 1
		shr     edx, 16
		push    ecx
		push    edx
		and     edx, 0FFH
		push    eax
		push    edx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		call    wmemset
		mov     ecx, dword ptr [count + 12]
		shr     ecx, 1
		jnc     L3
		mov     byte ptr [eax + ecx * 2], ' '
	L3:
		mov     eax, dword ptr [string + 12]
		add     esp, 12
	L4:
		rep ret

		#undef string
		#undef c
		#undef count
	}
}
#endif
