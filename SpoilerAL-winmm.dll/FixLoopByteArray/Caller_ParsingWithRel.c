#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "TProcessAccessElementData.h"
#include "TSSGCtrl.h"

extern const BOOL EnableParserFix;
extern const DWORD _STL_string_substr;

uint64_t __cdecl InternalParsing(TSSGCtrl* SSGCtrl, TSSGSubject* SSGS, const string* Src, BOOL IsInteger, va_list ArgPtr);

static unsigned long __fastcall OffsetRel(
	list*                      const CodeList,
	unsigned long           register Rel,
	TProcessAccessElementBase* const NowAE)
{
	for (register list_iterator it = list_begin(CodeList);
		 it != list_end(CodeList);
		 list_iterator_increment(it))
	{
		register TProcessAccessElementBase* CurAE = *(TProcessAccessElementBase**)it->_M_data;
		if (TProcessAccessElement_GetType(CurAE) != atJUMP)
			Rel += TProcessAccessElement_GetSize(CurAE, TRUE);
		else
			Rel  = 0;
	}
	if (TProcessAccessElement_GetType(NowAE) != atJUMP)
		Rel += TProcessAccessElement_GetSize(NowAE, TRUE);
	else
		Rel  = 0;
	return Rel;
}

static TProcessAccessElementMaskData* __fastcall TSSGCtrl_StrToProcessAccessElementVec_switch_CodeSize(
	list*                    const CodeList,
	int                   register Size,// sign extended char
	uint64_t                 const Val,
	TProcessAccessElementMaskData* NowAE)
{
	switch (Size -= '0')
	{// The code was parsed, but doesn't inserting if 0 byte
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		switch (TProcessAccessElement_GetType(NowAE))
		{
			vector_byte *mask, *data;
		case atMASK_DATA:
			mask = TProcessAccessElementMaskData_GetMask(NowAE);
			if (vector_end_of_storage(mask) - vector_end(mask) < sizeof(uint64_t))
				vector_byte_reserve(mask, max(sizeof(uint64_t), vector_capacity(mask)) << 1);
			vector_byte_resize(mask, vector_size(mask) + Size);
			goto L_PUSH_DATA;
		default:
			list_push_back(CodeList, &NowAE);
			 NowAE = bcb6_operator_new(sizeof(TProcessAccessElementMaskData));
			*NowAE = (const TProcessAccessElementMaskData) { (LPVOID)0x00627294, atDATA };
			vector_byte_reserve(TProcessAccessElementData_GetData(NowAE), sizeof(uint64_t));
			/* FALLTHROUGH */
		case atDATA:
		L_PUSH_DATA:
			data = TProcessAccessElementData_GetData(NowAE);
			if (vector_end_of_storage(data) - vector_end(data) < sizeof(uint64_t))
				vector_byte_reserve(data, max(sizeof(uint64_t), vector_capacity(data)) << 1);
			*(uint64_t*)vector_end(data) = Val;
			vector_end(data) += Size;
		}
	}
	return NowAE;
}

__declspec(naked) void __cdecl Caller_ParsingWithRel()
{
	extern BOOL FixTheProcedure;

	static const char  lpszRel[] = "Rel";
	static const DWORD X0050B4D9 = 0x0050B4D9;
	static const DWORD X0050BC21 = 0x0050BC21;

	__asm
	{
		cmp     EnableParserFix, 0
		je      L1

		#define this          (ebp + 0x0C)
		#define SSGS          (ebp + 0x10)
		#define Code          (ebp + 0x14)
		#define Rel           (ebp + 0x2C)
		#define CodeList      (ebp - 0x10)
		#define NowAE         (ebp - 0x085C)
		#define i              esi
		#define PosEnd        (ebp - 0x0D60)
		#define Data          (ebp - 0x03C8)

		mov     word ptr [ebx + 0x10], 0x08B4
#pragma region ArgPtr
		push    0// sentinel
		push    0// high dword
		mov     eax, dword ptr [Rel]
		cmp     FixTheProcedure, 0
		je      L2
		push    dword ptr [NowAE]
		mov     edx, eax
		lea     ecx, [CodeList]
		call    OffsetRel
	L2:
		push    eax
		push    offset lpszRel
		push    length lpszRel - 1
#pragma endregion
		mov     edx, esp
#pragma region Src
		mov     eax, dword ptr [Code]
		lea     ecx, [eax + i + 2]
		add     eax, dword ptr [PosEnd]
		push    0
		push    ecx
		push    0
		push    0
		push    eax
		push    ecx
#pragma endregion
		mov     ecx, esp
		push    edx
		push    TRUE
		push    ecx
		push    dword ptr [SSGS]
		push    dword ptr [this]
		call    InternalParsing
		add     esp, 20 + size string + 20
#pragma region Data_ctor
		xor     ecx, ecx
		mov     dword ptr [Data + 0x00], ecx
		mov     dword ptr [Data + 0x04], ecx
		mov     dword ptr [Data + 0x10], ecx
		add     dword ptr [ebx + 0x1C], 4
		mov      word ptr [ebx + 0x10], 0x0890
#pragma endregion
		push    dword ptr [NowAE]
		push    edx
		push    eax
		mov     eax, dword ptr [Code]
		movsx   edx, byte  ptr [eax + i + 1]
		lea     ecx, [CodeList]
		call    TSSGCtrl_StrToProcessAccessElementVec_switch_CodeSize
		mov     dword ptr [NowAE], eax
		jmp     dword ptr [X0050BC21]	// goto default:
		ud2

		#undef Data
		#undef PosEnd
		#undef i
		#undef NowAE
		#undef CodeList
		#undef Rel
		#undef Code
		#undef SSGS
		#undef this

		align   16
	L1:
		push    0
		lea     eax, [esi + 2]
		jmp     dword ptr [X0050B4D9]
	}
}
