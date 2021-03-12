#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

extern void __stdcall ReplaceDefine(void *attributeSelector, void *line);

extern BOOL EnableParserFix;

#define _Str  (ebp - 110H)

__declspec(naked) string * __cdecl TSSGCtrl_LoopSSRFile_ReplaceDefine(
	string const *const File_begin,
	size_t        const __n)
{
	__asm
	{
		#define this  (ebp + 8H)

#if 1
		mov     ecx, [esp + 4]
		mov     edx, [esp + 8]
		lea     eax, [edx + edx * 2]
		lea     eax, [ecx + eax * 8]
		cmp     EnableParserFix, FALSE
		je      L1
		
		mov     edx, eax
		lea     ecx, [_Str]
		call    string_ctor_assign
		push    eax
		mov     ecx, [this]
		lea     edx, [ecx]TSSGCtrl.attributeSelector
		push    edx
		call    ReplaceDefine
#else
		#define begin (ebp - 1F8H)
		#define _end  (ebp - 1F4H)

		mov     ecx, dword ptr [EnableParserFix]
		mov     eax, dword ptr [_end]
		test    ecx, ecx
		jz      L1
		push    eax
		lea     ecx, [_Str]
		call    string_ctor_assign_range
		mov     eax, dword ptr [this]
		lea     ecx, [_Str]
		add     eax, offset TSSGCtrl.attributeSelector
		push    ecx
		push    eax
		call    ReplaceDefine
		mov     ecx, dword ptr [_Str]
		mov     eax, dword ptr [_Str + 4]
		mov     dword ptr [begin], ecx
		
		#undef _end
		#undef begin
#endif
	L1:
		rep ret

		#undef this
	}
}

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_ReplaceDefine_Release()
{
	__asm
	{
		cmp     EnableParserFix, FALSE
		jne     L1
		rep ret

		align   16
	L1:
		lea     ecx, [_Str]
		jmp     string_dtor
	}
}

#undef _Str
