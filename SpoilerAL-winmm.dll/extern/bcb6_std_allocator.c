#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"
#include "TWinControl.h"
#include "TMainForm.h"

#undef internal_allocate
#undef internal_deallocate
#undef internal_reallocate
#define internal_allocate   bcb6_std_internal_allocate
#define internal_deallocate bcb6_std_internal_deallocate
#define internal_reallocate bcb6_std_internal_reallocate

#if !OPTIMIZE_ALLOCATOR
void *(__cdecl * const node_alloc_allocate)(size_t n) = (LPVOID)0x005F43F0;
void (__cdecl * const node_alloc_deallocate)(void *p, size_t n) = (LPVOID)0x005F47A0;
#endif

#if !OPTIMIZE_ALLOCATOR
#define MAX_BYTES 128
void * __cdecl bcb6_realloc(void *memblock, size_t size);
#else
extern HANDLE hHeap;
#endif

#define lpApplicationTitle (LPCSTR)0x006020D8

#if !OPTIMIZE_ALLOCATOR
__declspec(naked) void * __fastcall internal_allocate(size_t n)
{
	__asm
	{
		cmp     ecx, MAX_BYTES
		push    ecx
		mov     eax, ecx
		jbe     L1
		call    dword ptr [operator_new]
		jmp     L2

	L1:
		test    ecx, ecx
		jz      L3
		call    dword ptr [node_alloc_allocate]
	L2:
		test    eax, eax
		jz      L4
	L3:
		pop     ecx
		ret

		align   16
	L4:
		pop     ecx
		call    bad_alloc
		xor     eax, eax
		ret
	}
}
#elif !defined(_M_IX86)
void * __fastcall internal_allocate(size_t n)
{
	void *p;

	if (n)
	{
		p = HeapAlloc(hHeap, 0, n);
		if (!p)
			bad_alloc();
	}
	else
	{
		p = NULL;
	}
	return p;
}
#else
__declspec(naked) void * __fastcall internal_allocate(size_t n)
{
	__asm
	{
		test    ecx, ecx
		jz      L1
		mov     eax, dword ptr [hHeap]
		push    ecx
		push    0
		push    eax
		call    HeapAlloc
		test    eax, eax
		jnz     L2
		call    bad_alloc
	L1:
		xor     eax, eax
	L2:
		rep ret
	}
}
#endif

#if !OPTIMIZE_ALLOCATOR
__declspec(naked) void __fastcall internal_deallocate(void *p, size_t n)
{
	__asm
	{
		test    ecx, ecx
		jz      L3
		cmp     edx, MAX_BYTES
		push    edx
		push    ecx
		jbe     L1
		call    dword ptr [operator_delete]
		jmp     L2

	L1:
		call    dword ptr [node_alloc_deallocate]
	L2:
		add     esp, 8
	L3:
		rep ret
	}
}
#elif !defined(_M_IX86)
void __fastcall internal_deallocate(void *p)
{
	if (p)
		HeapFree(hHeap, 0, p);
}
#else
__declspec(naked) void __fastcall internal_deallocate(void *p)
{
	__asm
	{
		test    ecx, ecx
		jz      L1
		mov     eax, dword ptr [hHeap]
		push    ecx
		push    0
		push    eax
		call    HeapFree
	L1:
		rep ret
	}
}
#endif

#if !OPTIMIZE_ALLOCATOR
void * __fastcall internal_reallocate(void *p, size_t from, size_t to)
{
	if (to)
	{
		if (from != to)
		{
			if (from > MAX_BYTES && to > MAX_BYTES)
			{
				p = bcb6_realloc(p, to);
				if (!p)
					bad_alloc();
			}
			else
			{
				void *src = p;
				p = internal_allocate(to);
				if (from)
				{
					memcpy(p, src, min(from, to));
					internal_deallocate(src, from);
				}
			}
		}
	}
	else
	{
		if (from)
			internal_deallocate(p, from);
		p = NULL;
	}
	return p;
}
#elif !defined(_M_IX86)
void * __fastcall internal_reallocate(void *p, size_t n)
{
	if (n)
	{
		p = p ? HeapReAlloc(hHeap, 0, p, n) : HeapAlloc(hHeap, 0, n);
		if (!p)
			bad_alloc();
	}
	else
	{
		if (p)
			HeapFree(hHeap, 0, p);
		p = NULL;
	}
	return p;
}
#else
__declspec(naked) void * __fastcall internal_reallocate(void *p, size_t n)
{
	__asm
	{
		test    edx, edx
		jz      L3
		test    ecx, ecx
		mov     eax, dword ptr [hHeap]
		push    edx
		jnz     L1
		mov     edx, HeapAlloc
		jmp     L2

	L1:
		mov     edx, HeapReAlloc
		push    ecx
	L2:
		push    0
		push    eax
		call    edx
		test    eax, eax
		jnz     L5
		call    bad_alloc
		jmp     L4

	L3:
		test    ecx, ecx
		jz      L4
		mov     eax, dword ptr [hHeap]
		push    ecx
		push    0
		push    eax
		call    HeapFree
	L4:
		xor     eax, eax
	L5:
		rep ret
	}
}
#endif

void __cdecl bad_alloc()
{
	char lpText[0x80];

	if (FormatMessageA(
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		ERROR_IPSEC_IKE_OUT_OF_MEMORY,
		0,
		lpText,
		_countof(lpText),
		NULL))
		MessageBoxA(TWinControl_GetHandle(MainForm), lpText, lpApplicationTitle, MB_ICONERROR);
	ExitProcess(ERROR_OUTOFMEMORY);
}
