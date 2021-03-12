#include <windows.h>

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl FixTraceAndCopyWrite()
{
	__asm
	{
		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, dword ptr [ebx]
		test    eax, eax
		jz      L1
		rep ret
	L1:
		jmp     dword ptr [ecx + 4]
	}
}
