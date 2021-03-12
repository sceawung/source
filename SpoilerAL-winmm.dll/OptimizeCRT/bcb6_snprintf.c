#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include "bcb6_errno.h"

__declspec(naked) int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...)
{
	int __fastcall internal_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr, const va_list endarg);

	__asm
	{
		#define buffer  (esp + 4)
		#define count   (esp + 8)
		#define format  (esp + 12)
		#define va_args (esp + 16)

		mov     eax, dword ptr [format]
		lea     ecx, [va_args]
		push    0
		push    ecx
		mov     ecx, dword ptr [buffer + 8]
		mov     edx, dword ptr [count + 8]
		push    eax
		call    internal_vsnprintf
		cmp     eax, dword ptr [count]
		jb      L1
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		or      eax, -1
	L1:
		rep ret

		#undef buffer
		#undef count
		#undef format
		#undef va_args
	}
}
