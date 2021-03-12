#include <windows.h>
#include "TSSGCtrl.h"

extern void __stdcall ReplaceDefine(void *attributeSelector, void *line);

extern BOOL EnableParserFix;

static void __cdecl TSSGCtrl_EnumReadSSG_ReplaceDefine();

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_replace_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 44
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_size_ReplaceDefine()
{
	__asm
	{
		#define LineS (ebp - 38H)

		call    TSSGCtrl_EnumReadSSG_ReplaceDefine
		pop     ecx
		lea     eax, [LineS]
		push    -1
		jmp     ecx

		#undef LineS
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_adjustment_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 848
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_repeat_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 1076
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_involve_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 1688
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_note_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 1904
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_process_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 2156
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_title_ReplaceDefine()
{
	__asm
	{
		#define LineS (ebp - 38H)

		call    TSSGCtrl_EnumReadSSG_ReplaceDefine
		mov     eax, dword ptr [LineS]
		lea     edx, [ebp - 498H]
		ret

		#undef LineS
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_maker_ReplaceDefine()
{
	__asm
	{
		#define LineS (ebp - 38H)

		call    TSSGCtrl_EnumReadSSG_ReplaceDefine
		mov     edx, dword ptr [LineS]
		lea     ecx, [ebp - 500H]
		ret

		#undef LineS
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_creator_ReplaceDefine()
{
	__asm
	{
		#define LineS (ebp - 38H)

		call    TSSGCtrl_EnumReadSSG_ReplaceDefine
		mov     edx, dword ptr [LineS]
		lea     ecx, [ebp - 568H]
		ret

		#undef LineS
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_distinction_ReplaceDefine()
{
	__asm
	{
		mov     word ptr [ebx + 10H], 2636
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_ADJSubjectSet_ReplaceDefine()
{
	__asm
	{
		#define CallAddress 005D5258H

		push    CallAddress
		jmp     TSSGCtrl_EnumReadSSG_ReplaceDefine

		#undef CallAddress
	}
}

__declspec(naked) void __cdecl TSSGCtrl_EnumReadSSG_val_ReplaceDefine()
{
	__asm
	{
		#define LineS (ebp - 38H)

		call    TSSGCtrl_EnumReadSSG_ReplaceDefine
		mov     edx, dword ptr [LineS]
		mov     dword ptr [ebp - 1028H], edx
		ret

		#undef LineS
	}
}

__declspec(naked) static void __cdecl TSSGCtrl_EnumReadSSG_ReplaceDefine()
{
	__asm
	{
		#define this  (ebp + 8H)
		#define LineS (ebp - 38H)

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, dword ptr [this]
		test    eax, eax
		jz      L1
		add     ecx, offset TSSGCtrl.attributeSelector
		lea     eax, [LineS]
		push    eax
		push    ecx
		call    ReplaceDefine
	L1:
		rep ret

		#undef this
		#undef LineS
	}
}
