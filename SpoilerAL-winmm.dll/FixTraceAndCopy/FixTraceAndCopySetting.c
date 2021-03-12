#include "xx.h"
#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std_vector_string.h"

#pragma warning(disable:4733)

extern BOOL EnableParserFix;
extern const DWORD F004FE200;

__declspec(naked) void __cdecl FixTraceAndCopySetting()
{
	static void __stdcall InternalFixTraceAndCopySetting(void *this, void *SSGC);

	__asm
	{
		#define SSGC (ebp + 0CH)
		#define this ebx

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, dword ptr [SSGC]
		test    eax, eax
		jz      L1
		push    ecx
		push    this
		call    InternalFixTraceAndCopySetting
	L1:
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		jmp     vector_string_deallocate

		#undef SSGC
		#undef this
	}
}

__declspec(naked) static void __stdcall InternalFixTraceAndCopySetting(void *this, void *SSGC)
{
	__asm
	{
		push    ebp
		mov     eax, 006164C4H
		mov     ebp, esp
		sub     esp, 40
		call    __InitExceptBlockLDTC
#if !OPTIMIZE_ALLOCATOR
		push    12
		call    dword ptr [operator_new]
		pop     ecx
#else
		mov     ecx, 12
		call    operator_new
#endif
		test    eax, eax
		jz      L1
		mov     dword ptr [eax], 00615314H
		mov     dword ptr [eax + 4H], 3
		mov     byte ptr [eax + 8H], 1
	L1:
		mov     ecx, dword ptr [ebp + 8H]
		push    eax
		push    ecx
		mov     ecx, dword ptr [ebp + 0CH]
		push    1
		push    ecx
		call    dword ptr [F004FE200]
		mov     ecx, dword ptr [ebp - 40]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret     8
	}
}
