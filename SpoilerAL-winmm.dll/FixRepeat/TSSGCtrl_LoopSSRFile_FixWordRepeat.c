#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_list.h"

void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue();

extern BOOL EnableParserFix;
extern const DWORD _STL_string_M_append_dispatch;

const string* __fastcall TSSGCtrl_LoopSSRFile_FixWordRepeat(
	string       *const tmpS,
	list_iterator const VIt,
	unsigned long const Count,
	unsigned long const LoopVal)
{
#if 1
	string const *const Word = &((struct pair_byte_string *)VIt->_M_data)->second;
	switch (Count)
	{
	default:
		if (EnableParserFix)
		{
			string_reserve(tmpS, string_length(tmpS) + string_length(Word) * Count);
			for (unsigned long i = 0; i < Count; i++)
				string_append(tmpS, Word);
			break;
		}
		return Word;
	case 0:
		break;
	}
	return NULL;
#else
	static const DWORD X0050222B = 0x0050222B;
	static const DWORD X00502676 = 0x00502676;

	__asm
	{
		mov     eax, dword ptr [EnableParserFix]
		mov     dword ptr [ebp - 204H], ecx
		test    eax, eax
		jz      L4
		test    esi, esi
		jle     L3
		xor     edi, edi
		jmp     L2

	L1:
		mov     edx, dword ptr [ebp - 204H]
		lea     ecx, [ebp - 200H]
		mov     eax, dword ptr [edx + 4H]
		push    ecx
		push    eax
		mov     edx, dword ptr [edx]
		push    edx
		lea     ecx, [ebp - 78H]
		push    ecx
		call    dword ptr [_STL_string_M_append_dispatch]
		add     esp, 16
		inc     edi
	L2:
		cmp     esi, edi
		ja      L1
	L3:
		jmp     TSSGCtrl_LoopSSRFile_LineListLoopContinue

	L4:
		test    esi, esi
		je      L5
		jmp     dword ptr [X0050222B]

	L5:
		jmp     dword ptr [X00502676]
	}
#endif
}
