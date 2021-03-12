#include <stdlib.h>
#include "bcb6_errno.h"

#ifdef _DEBUG
_DCRTIMP errno_t * __cdecl _errno();
#else
EXTERN_C errno_t _terrno;
#endif

__declspec(naked) double __cdecl bcb6_strtod(const char *nptr, char **endptr)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)

#ifdef _DEBUG
		call    _errno
		mov     ecx, dword ptr [endptr]
		mov     edx, dword ptr [nptr]
		mov     dword ptr [eax], 0
		push    eax
		push    ecx
		push    edx
		call    strtod
		mov     eax, dword ptr [esp + 8]
		add     esp, 12
		cmp     dword ptr [eax], 0
		je      L1
#else
		mov     ecx, dword ptr [endptr]
		mov     eax, dword ptr [nptr]
		push    ecx
		push    eax
		mov     dword ptr [_terrno], 0
		call    strtod
		mov     eax, dword ptr [_terrno]
		add     esp, 8
		test    eax, eax
		jz      L1
#endif
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
	L1:
		rep ret

		#undef nptr
		#undef endptr
	}
}
