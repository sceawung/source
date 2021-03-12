#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"
#include "TSSGCtrl.h"

extern void __stdcall ReplaceDefine(void *attributeSelector, void *line);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_ReplaceDefine()
{
	__asm
	{
		#define this edi
		#define VIt  esi
		#define tmpS (ebp - 68H)

#if 1
		mov     edx, [VIt]bcb6_std_string._M_finish
		sub     edx, ecx
		jz      L1
		cmp     EnableParserFix, FALSE
		je      L1

		mov     edx, VIt
		lea     ecx, [tmpS]
		call    string_ctor_assign
		push    eax
		lea     eax, [this]TSSGCtrl.attributeSelector
		push    eax
		call    ReplaceDefine
		push    0
		mov     edx, esp
		push    0
		push    edx
		push    [eax]bcb6_std_string._M_start
		call    strtoul
		add     esp, 12
		pop     edx
		lea     ecx, [tmpS]
		cmp     [ecx]bcb6_std_string._M_start, edx
		sbb     edx, edx
		push    edx
		push    eax
		call    string_dtor
		pop     eax
		pop     edx
	L1:
		rep ret
#else
		cmp     dword ptr [EnableParserFix], 0
		je      L2
		mov     edx, VIt
		lea     ecx, [tmpS]
		call    string_ctor_assign
		lea     eax, [tmpS]
		push    eax
		lea     eax, [this + TSSGCtrl.attributeSelector]
		push    eax
		call    ReplaceDefine
		push    eax
		mov     eax, dword ptr [tmpS]
		mov     ecx, esp
		push    0
		push    ecx
		push    eax
		call    strtoul
		add     esp, 12
		mov     ecx, dword ptr [tmpS]
		pop     edx
		cmp     ecx, edx
		jne     L1
		mov     dword ptr [esp + 8], 004FF399H
		add     esp, 8
	L1:
		push    eax
		lea     ecx, [tmpS]
		call    string_dtor
		pop     eax
		ret

		align   16
	L2:
		jmp     dword ptr [TStringDivision_ToULongDef]
#endif

		#undef this
		#undef VIt
		#undef tmpS
	}
}
