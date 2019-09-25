#include <stddef.h>
#include <ctype.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	ret = 0;
	while (++count)
		if (ret = tolower(string1[count]) - (c = tolower(string2[count])))
			break;
		else if (!c)
			break;
	return ret;
}
#else
__declspec(naked) int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 16]           // esi = string1
		mov     edi, dword ptr [string2 + 16]           // edi = string2
		mov     ecx, dword ptr [count + 16]             // ecx = count
		xor     eax, eax
		add     esi, ecx                                // esi = end of string1
		add     edi, ecx                                // edi = end of string2
		xor     ecx, -1                                 // ecx = -count - 1
		xor     edx, edx
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		inc     ecx
		jz      return_equal
		lea     ebx, [edi + ecx]
		lea     ebp, [esi + ecx]
		and     ebx, 3
		jnz     byte_loop
		shl     ebp, 32 - BSF_PAGE_SIZE

		align   16
	dword_loop:
		cmp     ebp, (PAGE_SIZE - 4) shl (32 - BSF_PAGE_SIZE)
		ja      byte_loop                               // cross pages
		mov     ebx, dword ptr [esi + ecx]
		mov     ebp, dword ptr [edi + ecx]
		cmp     ebx, ebp
		jne     byte_loop                               // not equal
		add     ecx, 4
		jc      return_equal
		mov     edx, ebx
		lea     ebp, [esi + ecx]
		sub     ebx, 01010101H
		xor     edx, -1
		shl     ebp, 32 - BSF_PAGE_SIZE
		and     ebx, 80808080H
		and     edx, ebx
		jz      dword_loop
	return_equal:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      difference
		add     eax, 'a' - 'A'
	difference:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		sub     eax, edx
		ret

		#undef string1
		#undef string2
	}
}
#endif
