#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

void __stdcall ReplaceDefineDynamic(void *SSGS, void *line);

extern BOOL EnableParserFix;
extern const DWORD _TSSGCtrl_GetNameString;

#if 0
__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_ReplaceDefineDynamic1()
{
	__asm
	{
		#define CallAddress 00504284H
		#define SSGS        (ebp + 10H)
		#define NameStr     edx

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, dword ptr [SSGS]
		test    eax, eax
		jz      L1
		push    NameStr
		push    ecx
		push    CallAddress
		jmp     ReplaceDefineDynamic

	L1:
		jmp     dword ptr [_TSSGCtrl_GetNameString]
		ud2

		#undef CallAddress
		#undef SSGS
		#undef NameStr
	}
}
#endif

__declspec(naked) void __cdecl TSSGCtrl_AddressNaming_ReplaceDefineDynamic2()
{
	__asm
	{
		#define SSGS         (ebp + 10H)
		#define ReturnString (ebp + 08H)

		mov     edx, eax
		call    string_ctor_assign
		mov     ecx, dword ptr [EnableParserFix]
		mov     edx, dword ptr [SSGS]
		test    ecx, ecx
		jz      L1
		push    dword ptr [ReturnString]
		push    edx
		call    ReplaceDefineDynamic
	L1:
		rep ret

		#undef SSGS
		#undef ReturnString
	}
}
