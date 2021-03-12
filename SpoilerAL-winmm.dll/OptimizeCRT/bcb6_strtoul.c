#include <stdlib.h>
#include "bcb6_errno.h"

EXTERN_C unsigned __int64 __cdecl internal_strtoi64(BOOL is_unsigned, errno_t *errnoptr, errno_t reserved1, BOOL is_int64, void *reserved2, const TCHAR *nptr, TCHAR **endptr, int base);

__declspec(naked) unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base)
{
	__asm
	{
		#define IS_UNSIGNED TRUE
		#define IS_INT64    FALSE

		push    IS_INT64
		push    0
		push    esp
		push    IS_UNSIGNED
		call    internal_strtoi64
		mov     ecx, dword ptr [esp + 8]
		add     esp, 16
		test    ecx, ecx
		jz      L2
		cmp     ecx, EINVAL
		jne     L1
		mov     ecx, BCB6_EINVAL
	L1:
		push    eax
		push    ecx
		call    dword ptr [_bcb6_errno]
		pop     ecx
		mov     dword ptr [eax], ecx
		pop     eax
	L2:
		rep ret

		#undef IS_UNSIGNED
		#undef IS_INT64
	}
}
