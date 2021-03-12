#include <windows.h>
#include "bcb6_errno.h"

extern HANDLE hHeap;

#ifndef _M_IX86
void * __cdecl bcb6_calloc(size_t num, size_t size)
{
	void *memblock;

	if (!(size *= num))
		size++;
	memblock = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, size);
	if (!memblock)
		bcb6_errno = BCB6_ENOMEM;
	return memblock;
}
#else
__declspec(naked) void * __cdecl bcb6_calloc(size_t num, size_t size)
{
	__asm
	{
		#define num  (esp + 4)
		#define size (esp + 8)

		mov     eax, dword ptr [num]
		mov     ecx, dword ptr [size]
		imul    eax, ecx
		cmp     eax, 1
		adc     eax, 0
		mov     ecx, dword ptr [hHeap]
		push    eax
		push    HEAP_ZERO_MEMORY
		push    ecx
		call    HeapAlloc
		test    eax, eax
		jnz     L1
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ENOMEM
		xor     eax, eax
	L1:
		rep ret

		#undef num
		#undef size
	}
}
#endif

#ifndef _M_IX86
void * __cdecl bcb6_malloc(size_t size)
{
	void *memblock;

	if (!size)
		size++;
	memblock = HeapAlloc(hHeap, 0, size);
	if (!memblock)
		bcb6_errno = BCB6_ENOMEM;
	return memblock;
}
#else
__declspec(naked) void * __cdecl bcb6_malloc(size_t size)
{
	__asm
	{
		#define size (esp + 4)

		mov     eax, dword ptr [size]
		cmp     eax, 1
		adc     eax, 0
		mov     ecx, dword ptr [hHeap]
		push    eax
		push    0
		push    ecx
		call    HeapAlloc
		test    eax, eax
		jnz     L1
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ENOMEM
		xor     eax, eax
	L1:
		rep ret

		#undef size
	}
}
#endif

#ifndef _M_IX86
void __cdecl bcb6_free(void *memblock)
{
	if (memblock)
		HeapFree(hHeap, 0, memblock);
}
#else
__declspec(naked) void __cdecl bcb6_free(void *memblock)
{
	__asm
	{
		#define memblock (esp + 4)

		mov     eax, dword ptr [memblock]
		mov     ecx, dword ptr [hHeap]
		test    eax, eax
		jz      L1
		push    eax
		push    0
		push    ecx
		call    HeapFree
	L1:
		rep ret

		#undef memblock
	}
}
#endif

#ifndef _M_IX86
void * __cdecl bcb6_realloc(void *memblock, size_t size)
{
	if (size)
	{
		memblock = memblock ?
			HeapReAlloc(hHeap, 0, memblock, size) :
			HeapAlloc(hHeap, 0, size);
		if (!memblock)
			bcb6_errno = BCB6_ENOMEM;
	}
	else
	{
		if (memblock)
			HeapFree(hHeap, 0, memblock);
		memblock = NULL;
	}
	return memblock;
}
#else
__declspec(naked) void * __cdecl bcb6_realloc(void *memblock, size_t size)
{
	__asm
	{
		#define memblock (esp + 4)
		#define size     (esp + 8)

		mov     eax, dword ptr [memblock]
		mov     ecx, dword ptr [size]
		test    ecx, ecx
		jz      L3
		test    eax, eax
		push    ecx
		mov     ecx, dword ptr [hHeap]
		jnz     L1
		mov     edx, HeapAlloc
		jmp     L2

	L1:
		mov     edx, HeapReAlloc
		push    eax
	L2:
		push    0
		push    ecx
		call    edx
		test    eax, eax
		jnz     L5
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ENOMEM
		jmp     L4

	L3:
		test    eax, eax
		jz      L5
		mov     ecx, dword ptr [hHeap]
		push    eax
		push    0
		push    ecx
		call    HeapFree
	L4:
		xor     eax, eax
	L5:
		rep ret

		#undef memblock
		#undef size
	}
}
#endif
