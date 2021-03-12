#include <windows.h>

unsigned long __cdecl Parsing(void *this, void *SSGS, const void *Src, ...);

extern BOOL EnableParserFix;
extern const DWORD _TSSGCtrl_Parsing;

__declspec(naked) void __cdecl Caller_ParsingWithVal()
{
	static const char lpszVal[] = "Val";

	__asm
	{
		cmp     EnableParserFix, FALSE
		je      L1
		mov     ecx, dword ptr [esp + 0x04]
		mov     edx, dword ptr [esp + 0x08]
		mov     eax, dword ptr [esp + 0x0C]
		push    0
		push    0
		push    dword ptr [esp + 0x08 + 16]
		push    offset lpszVal
		push    length lpszVal - 1
		push    eax
		push    edx
		push    ecx
		call    Parsing
		add     esp, 32
		ret

	L1:
		jmp     _TSSGCtrl_Parsing
		ud2
	}
}
