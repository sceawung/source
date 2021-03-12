#include <windows.h>

#if 0
unsigned long __cdecl Parsing61(void *SSGCtrl, void *SSGS, const void *Src, unsigned long Val);
#else
unsigned long __cdecl Parsing(void *this, void *SSGS, const void *Src, ...);
#endif

extern BOOL EnableParserFix;
extern const DWORD _TSSGCtrl_Parsing;

__declspec(naked) void __cdecl Caller_Parsing()
{
	__asm
	{
		cmp     EnableParserFix, FALSE
#pragma warning(suppress: 4414)
		jne     Parsing
		jmp     _TSSGCtrl_Parsing
		ud2
	}
}
