#include <string.h>
#include <windows.h>
#include "intrinsic.h"
#include "xx.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_allocator.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TProcessAccessElementData.h"

#pragma warning(disable:4733)

extern const DWORD _STL_string_substr;
extern const DWORD F0048E0D8;
extern const DWORD F00490434;
extern const DWORD F0048FA48;
extern const DWORD F0050E758;

extern const BOOL  EnableParserFix;
extern void __stdcall ByteArrayReplaceDefine(TSSGSubject *SSGS, string *line);
extern void __stdcall ReplaceDefineDynamic  (TSSGSubject *SSGS, string *line);
extern uint64_t __cdecl InternalParsing(TSSGCtrl* SSGCtrl, TSSGSubject* SSGS, const string* Src, BOOL IsInteger, va_list ArgPtr);

void(__cdecl * const TProcessAccessElementLoop_MakeLoopSet)(
	TProcessAccessElementLoop *LoopElem,
	unsigned long              FullSize,
	vector_dword              *LoopVec,
	BOOLEAN                    IsTrueMode) = (LPVOID)0x0048FA48;

static void __fastcall TSSGCtrl_MakeLoopSet(
	TSSGCtrl     *const SSGC,
	TSSGSubject  *const SSGS,
	TProcessAccessElementLoop *const LoopElem,
	unsigned long const FullSize,
	vector_dword *const LoopVec,// not constructed, will be destructed in caller when after return.
	BOOLEAN       const IsTrueMode,
	unsigned long const ParentRel,// reserved
	string              Code)
{
	char init;
	LPCSTR dd;
	LoopElem->loopCount = 0;
	*LoopVec = (const vector_dword) { NULL };
	if (!FullSize || string_empty(&Code))
		string_dtor(&Code);
	else if (EnableParserFix && (init = string_at(&Code, 0)) != 's' && (dd = strstr(string_c_str(&Code), "$$")))
	{
		string code;
		TProcessAccessElementMaskData *LastE = NULL, *NowAE, **iter, **edge;
		unsigned long Rel = 0, Size = string_at(&Code, 1) - '0';

		if (init == '$' && Size >= 1 && Size <= 8 && dd + 2 == string_end(&Code))
		{// specialization for single code
			static char const lpszRel[] = "Rel";
			size_t ArgPtr[] = { _countof(lpszRel) - 1, (size_t)lpszRel, Rel, 0/* high dword */, 0/* sentinel */ };
			vector_byte *data;

			TProcessAccessElement_dtor(LoopElem, 0x02);
			*(NowAE = (void *)LoopElem) = (const TProcessAccessElementMaskData) { (LPVOID)0x00627294, atDATA };
			vector_byte_reserve(data = &NowAE->data, FullSize + sizeof(uint64_t));

			string_begin(&code) = string_begin(&Code) + 2;
			*(string_end(&code) = (LPSTR)dd) = '\0';
			string_end_of_storage(&code) = string_begin(&code);
			do
				*(uint64_t *)&vector_at(data, Rel) = InternalParsing(SSGC, SSGS, &code, TRUE, (va_list)ArgPtr);
			while ((ArgPtr[2] = (Rel += Size)) < FullSize);

			vector_end(data) += FullSize;
		}
		else do
		{
			vector_dtor(LoopVec);
			string_ctor_assign(&code, &Code);
			TSSGCtrl_StrToProcessAccessElementVec(LoopVec, SSGC, SSGS, code, Rel);

			if (!(Size = (edge = (void *)vector_end(LoopVec)) - (iter = (void *)vector_begin(LoopVec))))
				break;// unexpected syntax
			else if (LastE)
			{// since second loop
				AccType type;
				if ((type = TProcessAccessElement_GetType(LastE)) == TProcessAccessElement_GetType(NowAE = *iter)) switch (type)
				{// vectorized ligature
					vector_byte *src, *dst;
				case atMASK_DATA:
					src = TProcessAccessElementMaskData_GetMask(NowAE);
					if (Rel + (Size = vector_size(src)) > FullSize)
						Size = FullSize - Rel;
					dst = TProcessAccessElementMaskData_GetMask(LastE);
					vector_byte_reserve(dst, vector_size(dst) + Size);
					__movsb(vector_end(dst), vector_begin(src), Size);
					vector_end(dst) += Size;
					/* FALLTHROUGH */
				case atDATA:
					src = TProcessAccessElementData_GetData(NowAE);
					if ((Rel += Size = vector_size(src)) > FullSize)
						Size -= Rel - FullSize;
					dst = TProcessAccessElementData_GetData(LastE);
					vector_byte_reserve(dst, vector_size(dst) + Size);
					__movsb(vector_end(dst), vector_begin(src), Size);
					vector_end(dst) += Size;

					delete_TProcessAccessElement(NowAE);
					iter++;
				}
			}// else first loop
			else if (Size == 1) switch (TProcessAccessElement_GetType(NowAE = *iter))
			{// Expected to be all elements are binary data
				vector_byte *data;
			case atMASK_DATA:
				data = TProcessAccessElementMaskData_GetMask(NowAE);
				if (vector_size(data) < FullSize)
					vector_byte_reserve(data, FullSize);
				else
					vector_end(data) = vector_begin(data) + FullSize;
				/* FALLTHROUGH */
			case atDATA:
				data = TProcessAccessElementData_GetData(NowAE);
				if ((Rel = vector_size(data)) < FullSize)
					vector_byte_reserve(data, FullSize);
				else
					vector_end(data) = vector_begin(data) + FullSize;

				TProcessAccessElement_dtor(LoopElem, 0x02);
				*(LastE = (void *)LoopElem) = *NowAE;
				bcb6_operator_delete(NowAE);
				continue;
			default:
				goto FALLBACK;
			}
			else// not completely vectorizable, adjust reservation
				FALLBACK: vector_dword_reserve(
					&LoopElem->surplusVec,
					FullSize / TProcessAccessElement_GetSize(*iter, IsTrueMode) + 1
				);

			for (; iter < edge; iter++)
			{
				if (Rel < FullSize)
				{
					if ((Rel += Size = TProcessAccessElement_GetSize(LastE = *iter, IsTrueMode)) > FullSize)
						TProcessAccessElement_SetSize(LastE, Size + FullSize - Rel, IsTrueMode);
					vector_dword_push_back(&LoopElem->surplusVec, (uintptr_t)LastE);
				}
				else
					delete_TProcessAccessElement(*iter);
			}
		}
		while (Rel < FullSize);

		string_dtor(&Code);
	}
	else
	{
		TSSGCtrl_StrToProcessAccessElementVec(LoopVec, SSGC, SSGS, Code, 0);
		switch (vector_size(LoopVec))
		{
			TProcessAccessElementMaskData *NowAE;
		case 0:// unexpected syntax
			break;
		case 1:// inspects in case single element
			switch (TProcessAccessElement_GetType(NowAE = (void *)vector_at(LoopVec, 0)))
			{
				register vector_byte *data;
				register size_t size;
			case atMASK_DATA:
				data = TProcessAccessElementMaskData_GetMask(NowAE);
				size = vector_size(data);
				vector_byte_reserve(data, FullSize << 1);
				while (size < FullSize)
				{
					__movsb(vector_begin(data) + size, vector_begin(data), size);
					size <<= 1;
				}
				vector_end(data) = vector_begin(data) + FullSize;
				/* FALLTHROUGH */
			case atDATA:
				data = TProcessAccessElementData_GetData(NowAE);
				if (size = vector_size(data))
				{
					vector_byte_reserve(data, FullSize << 1);
					while (size < FullSize)
					{
						__movsb(vector_begin(data) + size, vector_begin(data), size);
						size <<= 1;
					}
					vector_end(data) = vector_begin(data) + FullSize;

					TProcessAccessElement_dtor(LoopElem, 0x02);
					*(TProcessAccessElementMaskData *)LoopElem = *NowAE;
					bcb6_operator_delete(NowAE);
				}
				else
					delete_TProcessAccessElement(NowAE);
				return;
			}
		default:
			TProcessAccessElementLoop_MakeLoopSet(LoopElem, FullSize, LoopVec, IsTrueMode);
		}
	}
}

#define MASM_BSWAP32(value) (            \
    (((value) shr 24) and 0x000000FF) or \
    (((value) shr  8) and 0x0000FF00) or \
    (((value) shl  8) and 0x00FF0000) or \
    (((value) shl 24) and 0xFF000000))

#define SSGC (ebp + 0x0C)
#define SSGS (ebp + 0x10)

__declspec(naked) void __cdecl TSSGCtrl_StrToProcessAccessElementVec_MakeLoopSet(
	TProcessAccessElementLoop *LoopElem,
	unsigned long              FillSize,
	vector_dword              *LoopVec,
	BOOLEAN                    IsTrueMode,
	unsigned long              ParentRel,
	string                     substr)
{
	__asm {
		mov  edx, [SSGS]
		mov  ecx, [SSGC]
		jmp  TSSGCtrl_MakeLoopSet
	}
}

__declspec(naked) void __cdecl TSSGCtrl_MakeDataCode_MakeLoopSet(
	TProcessAccessElementLoop *LoopElem,
	unsigned long              FullSize,
	vector_dword              *LoopVec,
	BOOLEAN                    IsTrueMode,
	unsigned long              ParentRel,// always zero
	string                     Code)
{
	#define Code (esp + 0x18)
	__asm {
		cmp  EnableParserFix, FALSE
		je   MakeLoopSet
		// Normalizes Code in advance for inspection
		lea  ecx, [Code]
		push ecx
		push dword ptr [SSGS]
		call string_trim_blank
		call ByteArrayReplaceDefine
	MakeLoopSet:
		jmp  TSSGCtrl_StrToProcessAccessElementVec_MakeLoopSet
	}
	#undef Code
}

#undef SSGS
#undef SSGC

__declspec(naked) void __stdcall FixLoopByteArray(
	TSSGCtrl      *SSGCtrl,
	TSSGSubject   *SSGS,
	string        *Code,
	void          *NowAE,
	unsigned long PosEnd,
	unsigned long PosSep,
	unsigned long FillSize,
	void          *CodeList)
{
	static const DWORD data1[] = {
		0x004667A8,
		0x00000004, -24,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x004667A8,
		0x00000004, -48,
		0x0047066C,
		0x00000004, -56,
		0x004667A8,
		0x00000004, -24,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00416274,
		0x00000004, -536,
		0x00000000,
	};
	static const DWORD data4[] = {
		0x00416274,
		0x00000004, -80,
		0x00000000,
	};
	static const DWORD data5[] = {
		0x00416274,
		0x00000004, -80,
		0x00000000,
	};
	static const DWORD data6[] = {
		0x004667A8,
		0x00000004, -104,
		0x00000000,
	};
	static const DWORD data7[] = {
		0x004667A8,
		0x00000004, -128,
		0x0047066C,
		0x00000004, -136,
		0x004667A8,
		0x00000004, -104,
		0x00000000,
	};
	static const DWORD data8[] = {
		0x00416274,
		0x00000004, -536,
		0x00000000,
	};
	static const DWORD data9[] = {
		0x00000000, -172,
		0x00050000, 0x00000000,
		(DWORD)data1,
		0x00050008, 0x00000000,
		(DWORD)data2,
		0x00050014, 0x00000003,
		(DWORD)data3,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050014, 0x00000000,
		0x00000000, 0x00050014,
		0x00000000, 0x00000000,
		0x00050008, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		(DWORD)data4,
		0x00050074, 0x00000000,
		(DWORD)data5,
		0x00050074, 0x00000004,
		(DWORD)data6,
		0x0005008C, 0x00000004,
		(DWORD)data7,
		0x00050098, 0x00000003,
		(DWORD)data8,
		0x00050098, 0x00000000,
		0x00000000, 0x00050098,
		0x00000000, 0x00000000,
		0x00050098, 0x00000000,
		0x00000000, 0x00050098,
		0x00000000, 0x00000000,
		0x0005008C, 0x00000000,
		0x00000000, 0x0005008C,
		0x00000000, 0x00000000,
		0x0005008C, 0x00000000,
		0x00000000, 0x0005008C,
		0x00000000, 0x00000000,
		0x0005008C, 0x00000000,
		0x00000000, 0x0005008C,
		0x00000000, 0x00000000,
	};
	static const DWORD X00506F81 = 0x00506F81;

	__asm
	{
		push    ebp
		mov     eax, offset data9
		mov     ebp, esp
		sub     esp, 496
		push    ebx
		push    esi
		call    __InitExceptBlockLDTC
		mov     edx, dword ptr [ebp + 10H]
		mov     ecx, dword ptr [ebp + 1CH]
		mov     edx, dword ptr [edx]
		xor     eax, eax
		lea     ecx, [ecx + edx + 6]
		add     edx, dword ptr [ebp + 18H]
		cmp     ecx, edx
		jae     L5
		sub     ecx, 4
		sub     edx, 4
	L1:
		mov     eax, dword ptr [ecx]
		inc     ecx
		cmp     eax, MASM_BSWAP32('$Rel')
		jne     L2
		mov     ah, byte ptr [ecx + 3]
		add     ecx, 3
		test    ah, ah
		js      L2
		sub     ah, '0'
		cmp     ah, '9' - '0' + 1
		jb      L4
		sub     ah, 'A' - '0'
		cmp     ah, 'Z' - 'A' + 1
		jb      L4
		cmp     ah, '_' - 'A'
		je      L4
		sub     ah, 'a' - 'A'
		cmp     ah, 'z' - 'a' + 1
		jb      L4
		jmp     L7

		align   16
	L2:
		sub     al, 81H
		cmp     al, 9FH - 81H + 1
		jb      L3
		sub     al, 0E0H - 81H
		cmp     al, 0FCH - 0E0H + 1
		jae     L4
	L3:
		inc     ecx
	L4:
		cmp     ecx, edx
		jb      L1
	L5:
		mov     ecx, dword ptr [ebp + 1CH]
		mov     eax, dword ptr [ebp + 18H]
		add     ecx, 2
		sub     esp, 24
		sub     eax, ecx
		push    0
		mov     edx, esp
		push    eax
		mov     eax, dword ptr [ebp + 10H]
		push    ecx
		push    eax
		push    edx
		call    dword ptr [_STL_string_substr]
		mov     eax, dword ptr [ebp + 0CH]
		mov     ecx, dword ptr [ebp + 8H]
		lea     edx, [ebp - 30H]
		mov     dword ptr [esp + 12], eax
		mov     dword ptr [esp +  8], ecx
		mov     dword ptr [esp +  4], edx
		mov     dword ptr [esp     ], offset L6
		push    ebp
		mov     ebp, esp
		sub     esp, 4092
		jmp     dword ptr [X00506F81]

		align   16
	L6:
		lea     ecx, [ebp - 30H]
		lea     edx, [ebp - 38H]
		mov     dword ptr [ebp - 0B8H], ecx
		mov     dword ptr [ebp - 0BCH], edx
		mov     edx, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4H]
		add     esp, 40
		sub     ecx, edx
		mov     edx, ecx
		xor     eax, eax
		sar     edx, 31
		mov     dword ptr [ebp - 18H], eax
		and     edx, 3
		mov     dword ptr [ebp - 14H], eax
		add     ecx, edx
		mov     dword ptr [ebp - 8H], eax
		sar     ecx, 2
		mov     dword ptr [ebp - 0C0H], ecx
		shl     ecx, 2
		mov     dword ptr [ebp - 0C4H], ecx
		call    internal_allocate
		mov     ecx, dword ptr [ebp - 0C0H]
		mov     dword ptr [ebp - 18H], eax
		shl     ecx, 2
		mov     dword ptr [ebp - 14H], eax
		add     ecx, eax
		mov     dword ptr [ebp - 0C8H], eax
		mov     dword ptr [ebp - 8H], ecx
		mov     eax, dword ptr [ebp - 0B8H]
		mov     ecx, dword ptr [eax + 4H]
		mov     eax, dword ptr [eax]
		mov     dword ptr [ebp - 0CCH], ecx
		mov     dword ptr [ebp - 0D0H], eax
		mov     ecx, dword ptr [ebp - 0E0H]
		mov     eax, dword ptr [ebp - 0DCH]
		mov     dword ptr [ebp - 0D8H], ecx
		mov     dword ptr [ebp - 0D4H], eax
		mov     ecx, dword ptr [ebp - 0F0H]
		mov     eax, dword ptr [ebp - 0ECH]
		mov     dword ptr [ebp - 0E8H], ecx
		lea     ecx, [ebp - 0F8H]
		mov     dword ptr [ebp - 0E4H], eax
		push    ecx
		call    dword ptr [F0048E0D8]
		mov     dword ptr [esp], 0
		lea     ecx, [ebp - 100H]
		push    ecx
		mov     eax, dword ptr [ebp - 0C8H]
		mov     ecx, dword ptr [ebp - 0CCH]
		mov     edx, dword ptr [ebp - 0D0H]
		push    eax
		push    ecx
		push    edx
		call    dword ptr [F00490434]
		mov     dword ptr [ebp - 14H], eax
		mov     eax, dword ptr [ebp - 30H]
		mov     edx, dword ptr [ebp - 20H]
		mov     dword ptr [ebp - 108H], eax
		sub     edx, eax
		mov     ecx, dword ptr [ebp - 2CH]
		mov     eax, edx
		mov     dword ptr [ebp - 104H], ecx
		sar     eax, 31
		add     esp, 20
		and     eax, 3
		mov     ecx, dword ptr [ebp - 30H]
		add     edx, eax
		mov     dword ptr [ebp - 110H], ecx
		sar     edx, 2
		mov     dword ptr [ebp - 10CH], edx
		shl     edx, 2
		mov     dword ptr [ebp - 114H], edx
		call    internal_deallocate
		push    1
		lea     eax, [ebp - 18H]
		push    eax
		mov     edx, dword ptr [ebp + 20H]
		mov     ecx, dword ptr [ebp + 14H]
		push    edx
		push    ecx
		call    dword ptr [F0048FA48]
		mov     edx, dword ptr [ebp - 8H]
		mov     eax, dword ptr [ebp - 18H]
		sub     edx, eax
		mov     dword ptr [ebp - 11CH], eax
		mov     eax, edx
		add     esp, 16
		sar     eax, 31
		mov     ecx, dword ptr [ebp - 14H]
		and     eax, 3
		mov     dword ptr [ebp - 118H], ecx
		add     edx, eax
		mov     ecx, dword ptr [ebp - 18H]
		sar     edx, 2
		mov     dword ptr [ebp - 124H], ecx
		mov     dword ptr [ebp - 120H], edx
		shl     edx, 2
		mov     dword ptr [ebp - 128H], edx
		call    internal_deallocate
		jmp     L15

		align   16
	L7:
		cmp     dword ptr [ebp + 20H], 0
		je      L15
		mov     ecx, dword ptr [ebp + 1CH]
		mov     eax, dword ptr [ebp + 18H]
		add     ecx, 2
		mov     edx, dword ptr [ebp + 10H]
		sub     eax, ecx
		lea     esi, [ebp - 50H]
		push    eax
		push    ecx
		push    edx
		push    esi
		call    dword ptr [_STL_string_substr]
		add     esp, 16
		mov     dword ptr [ebp - 12CH], 0
	L8:
		mov     eax, dword ptr [ebp - 12CH]
		lea     edx, [ebp - 50H]
		push    eax
		lea     ecx, [esp - 24]
		sub     esp, 24
		call    string_ctor_assign
		mov     eax, dword ptr [ebp + 0CH]
		mov     ecx, dword ptr [ebp + 8H]
		lea     edx, [ebp - 80H]
		push    eax
		push    ecx
		push    edx
		push    offset L9
		push    ebp
		mov     ebp, esp
		sub     esp, 4092
		jmp     dword ptr [X00506F81]

		align   16
	L9:
		lea     ebx, [ebp - 80H]
		add     esp, 40
		mov     ecx, dword ptr [ebx]
		mov     eax, dword ptr [ebx + 4H]
		sub     eax, ecx
		lea     edx, [ebp - 88H]
		mov     ecx, eax
		mov     dword ptr [ebp - 130H], edx
		sar     eax, 31
		mov     dword ptr [ebp - 68H], 0
		and     eax, 3
		mov     dword ptr [ebp - 64H], 0
		add     ecx, eax
		mov     dword ptr [ebp - 58H], 0
		sar     ecx, 2
		mov     esi, ecx
		shl     ecx, 2
		mov     dword ptr [ebp - 134H], ecx
		call    internal_allocate
		shl     esi, 2
		mov     dword ptr [ebp - 68H], eax
		add     esi, eax
		mov     dword ptr [ebp - 64H], eax
		mov     dword ptr [ebp - 58H], esi
		mov     dword ptr [ebp - 138H], eax
		mov     ecx, dword ptr [ebx + 4H]
		mov     eax, dword ptr [ebx]
		mov     dword ptr [ebp - 13CH], ecx
		mov     dword ptr [ebp - 140H], eax
		mov     ecx, dword ptr [ebp - 150H]
		mov     eax, dword ptr [ebp - 14CH]
		mov     dword ptr [ebp - 148H], ecx
		mov     dword ptr [ebp - 144H], eax
		mov     ecx, dword ptr [ebp - 160H]
		mov     eax, dword ptr [ebp - 15CH]
		mov     dword ptr [ebp - 158H], ecx
		lea     ecx, [ebp - 168H]
		mov     dword ptr [ebp - 154H], eax
		push    ecx
		call    dword ptr [F0048E0D8]
		mov     dword ptr [esp], 0
		lea     ecx, [ebp - 170H]
		push    ecx
		mov     eax, dword ptr [ebp - 138H]
		mov     ecx, dword ptr [ebp - 13CH]
		mov     edx, dword ptr [ebp - 140H]
		push    eax
		push    ecx
		push    edx
		call    dword ptr [F00490434]
		mov     dword ptr [ebp - 64H], eax
		mov     eax, dword ptr [ebp - 80H]
		mov     edx, dword ptr [ebp - 70H]
		mov     dword ptr [ebp - 178H], eax
		sub     edx, eax
		add     esp, 20
		mov     eax, edx
		mov     ecx, dword ptr [ebp - 7CH]
		sar     eax, 31
		mov     dword ptr [ebp - 174H], ecx
		and     eax, 3
		mov     ecx, dword ptr [ebp - 80H]
		add     edx, eax
		mov     dword ptr [ebp - 180H], ecx
		sar     edx, 2
		mov     dword ptr [ebp - 17CH], edx
		shl     edx, 2
		mov     dword ptr [ebp - 184H], edx
		call    internal_deallocate
		mov     ebx, dword ptr [ebp - 68H]
		jmp     L14

		align   16
	L10:
		mov     eax, dword ptr [ebx]
		push    1
		mov     ecx, dword ptr [eax]
		push    eax
		call    dword ptr [ecx + 4H]
		mov     edx, dword ptr [ebp - 12CH]
		mov     ecx, dword ptr [ebp + 20H]
		add     eax, edx
		add     esp, 8
		cmp     eax, ecx
		jae     L11
		mov     dword ptr [ebp - 12CH], eax
		mov     eax, dword ptr [ebp + 24H]
		mov     eax, dword ptr [eax + 8H]
		mov     dword ptr [ebp - 188H], ebx
		mov     dword ptr [ebp - 18CH], eax
		mov     ecx, dword ptr [ebp + 24H]
		push    ebx
		push    ecx
		call    dword ptr [F0050E758]
		mov     ecx, dword ptr [ebp - 18CH]
		add     esp, 8
		mov     dword ptr [ebp - 190H], ecx
		mov     edx, dword ptr [ecx + 4H]
		mov     dword ptr [ebp - 194H], edx
		mov     dword ptr [eax], ecx
		mov     dword ptr [eax + 4H], edx
		mov     dword ptr [edx], eax
		mov     dword ptr [ecx + 4H], eax
		mov     dword ptr [ebp - 198H], eax
		mov     dword ptr [ebp - 19CH], eax
		jmp     L13

		align   16
	L11:
		je      L12
		mov     ecx, dword ptr [ebp + 20H]
		mov     eax, dword ptr [ebp - 12CH]
		sub     ecx, eax
		mov     eax, dword ptr [ebx]
		push    1
		push    ecx
		mov     ecx, dword ptr [eax]
		push    eax
		call    dword ptr [ecx + 8H]
		add     esp, 12
	L12:
		mov     ecx, dword ptr [ebp + 24H]
		mov     esi, dword ptr [ecx + 8H]
		mov     dword ptr [ebp - 1A0H], ebx
		mov     dword ptr [ebp - 1A4H], esi
		push    ebx
		push    ecx
		call    dword ptr [F0050E758]
		mov     edx, dword ptr [esi + 4H]
		add     esp, 8
		mov     dword ptr [esi + 4H], eax
		mov     dword ptr [eax], esi
		mov     dword ptr [eax + 4H], edx
		mov     dword ptr [edx], eax
		mov     dword ptr [ebp - 1ACH], eax
		mov     ebx, dword ptr [ebp - 68H]
		mov     ecx, dword ptr [ebp - 58H]
		mov     dword ptr [ebp - 1B8H], ebx
		sub     ecx, ebx
		mov     dword ptr [ebp - 1A8H], edx
		mov     edx, ecx
		mov     eax, dword ptr [ebp - 1ACH]
		sar     ecx, 31
		mov     dword ptr [ebp - 1B0H], eax
		and     ecx, 3
		add     edx, ecx
		mov     ecx, dword ptr [ebp - 64H]
		sar     edx, 2
		mov     dword ptr [ebp - 1BCH], edx
		mov     dword ptr [ebp - 1B4H], ecx
		shl     edx, 2
		mov     dword ptr [ebp - 1C0H], edx
		mov     ecx, ebx
		call    internal_deallocate
		lea     ecx, [ebp - 50H]
		call    string_dtor
		jmp     L15

		align   16
	L13:
		add     ebx, 4
	L14:
		cmp     ebx, dword ptr [ebp - 64H]
		jne     L10
		mov     ecx, dword ptr [ebp - 68H]
		mov     eax, dword ptr [ebp - 58H]
		sub     eax, ecx
		mov     dword ptr [ebp - 1CCH], ecx
		mov     edx, eax
		mov     dword ptr [ebp - 1C8H], ebx
		sar     eax, 31
		mov     dword ptr [ebp - 1D4H], ecx
		and     eax, 3
		add     edx, eax
		sar     edx, 2
		mov     dword ptr [ebp - 1D0H], edx
		shl     edx, 2
		mov     dword ptr [ebp - 1D8H], edx
		call    internal_deallocate
		jmp     L8

		align   16
	L15:
		mov     ecx, dword ptr [ebp - 172]
		mov     dword ptr fs:[0], ecx
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret     32
	}
}
