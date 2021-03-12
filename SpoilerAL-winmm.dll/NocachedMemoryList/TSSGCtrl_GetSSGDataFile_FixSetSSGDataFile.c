#include "xx.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "bcb6_std_allocator.h"

#pragma function(memcmp)

#pragma warning(disable:4733)

extern const DWORD F004185E8;
extern const DWORD F004185FC;
extern const DWORD F0044CA38;
extern const DWORD _STL__uninitialized_copy;
extern const DWORD F00469A5C;
extern const DWORD F004AE4AC;
extern const DWORD F004F20E4;
extern const DWORD F004F21F8;
extern const DWORD F004F2424;
extern const DWORD F004F32FC;
extern const DWORD F004F3410;

__declspec(naked) void __cdecl TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile()
{
	static const DWORD data1[] = {
		0x00402634,
		0x00000004, -8,
		0x00416274,
		0x00000004, -32,
		0x00416274,
		0x00000004, -56,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00416274,
		0x00000004, -648,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x0046ED10,
		0x00000004, -88,
		0x00469F34,
		0x00000004, -144,
		0x00000000,
	};
	static const DWORD data4[] = {
		0x004F22C8,
		0x00000004, -152,
		0x00000000,
	};
	static const DWORD data5[] = {
		0x0047111C,
		0x00000004, -160,
		0x00000000,
	};
	static const DWORD data6[] = {
		0x004F22C8,
		0x00000004, -168,
		0x00000000,
	};
	static const DWORD data7[] = {
		0x0047111C,
		0x00000004, -176,
		0x00000000,
	};
	static const DWORD data8[] = {
		0x00416510,
		0x00000004, -184,
		0x00416414,
		0x00000004, -208,
		0x00469DDC,
		0x00000004, -256,
		0x00000000,
	};
	static const DWORD data9[] = {
		0x00416510,
		0x00000004, -264,
		0x00000000,
	};
	static const DWORD data10[] = {
		0x00000000, -300,
		0x00050000, 0x00000000,
		(DWORD)data1,
		0x00050008, 0x00000005,
		(DWORD)data2,
		0x00050008, 0x00000009,
		(DWORD)data3,
		0x00050020, 0x00000000,
		(DWORD)data4,
		0x0005002C, 0x00000000,
		(DWORD)data5,
		0x00050020, 0x00000000,
		0x00000000, 0x00050020,
		0x00000000, 0x00000000,
		0x00050050, 0x00000000,
		(DWORD)data6,
		0x0005005C, 0x00000000,
		(DWORD)data7,
		0x0005005C, 0x00000000,
		0x00000000, 0x00050020,
		0x00000000, 0x00000000,
		0x00050020, 0x00000000,
		0x00000000, 0x00050020,
		0x00000000, 0x00000000,
		0x00050020, 0x00000000,
		0x00000000, 0x00050020,
		0x00000000, 0x00000000,
		0x00050020, 0x00000000,
		0x00000000, 0x00050008,
		0x00000009,
		(DWORD)data8,
		0x000500C8, 0x00000000,
		0x00000000, 0x000500C8,
		0x00000000, 0x00000000,
		0x000500C8, 0x00000004,
		(DWORD)data9,
		0x000500EC, 0x00000000,
		0x00000000, 0x000500EC,
		0x00000000, 0x00000000,
		0x000500C8, 0x00000000,
		0x00000000, 0x000500C8,
		0x00000000, 0x00000000,
		0x000500C8, 0x00000000,
		0x00000000, 0x000500C8,
		0x00000000, 0x00000000,
		0x000500C8, 0x00000000,
		0x00000000, 0x000500C8,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
		0x00050000, 0x00000000,
		0x00000000, 0x00050000,
		0x00000000, 0x00000000,
	};

	__asm
	{
		#define IsNocache (ebp - 304H)

		cmp     dword ptr [IsNocache], 0
		jne     L1
		jmp     dword ptr [TSSGCtrl_SetSSGDataFile]

	L1:
		#undef IsNocache

		#define this      (ebp + 8H)
		#define Data      (ebp + 0CH)
		#define FileName  (ebp + 10H)
		#define IsSSL     (ebp + 14H)

		push    ebp
		mov     eax, offset data10
		mov     ebp, esp
		sub     esp, 600
		push    ebx
		lea     ebx, [ebp - 90H]
		push    esi
		call    __InitExceptBlockLDTC
		mov     eax, dword ptr [Data]
		mov     edx, 00631A92H
		push    eax
		lea     ecx, [ebp - 20H]
		call    string_ctor_assign_cstr
		push    -1
		push    0
		sub     esp, 24
		mov     edx, dword ptr [FileName]
		mov     ecx, esp
		call    string_ctor_assign
		mov     ecx, dword ptr [this]
		add     ecx, 992
		push    ecx
		lea     ecx, [ebp - 38H]
		push    ecx
		call    dword ptr [F004AE4AC]
		mov     ecx, dword ptr [this]
		add     esp, 40
		add     ecx, 392
		lea     edx, [ebp - 38H]
		mov     dword ptr [ebp - 130H], edx
		mov     dword ptr [ebp - 134H], ecx
		push    edx
		push    ecx
		call    dword ptr [F004F21F8]
		mov     ecx, dword ptr [ebp - 134H]
		add     esp, 8
		mov     ecx, dword ptr [ecx + 8H]
		mov     dword ptr [ebp - 138H], eax
		mov     dword ptr [ebp - 13CH], eax
		mov     dword ptr [ebp - 140H], ecx
		mov     dword ptr [ebp - 144H], ecx
		cmp     ecx, eax
		je      L5
		mov     ecx, dword ptr [eax + 14H]
		add     eax, 16
		mov     edx, dword ptr [eax]
		mov     dword ptr [ebp - 148H], eax
		mov     dword ptr [ebp - 154H], ecx
		mov     dword ptr [ebp - 158H], edx
		sub     ecx, edx
		mov     eax, dword ptr [ebp - 134H]
		mov     dword ptr [ebp - 168H], ecx
		mov     eax, dword ptr [ebp - 130H]
		mov     ecx, dword ptr [eax + 18H]
		mov     edx, dword ptr [eax + 1CH]
		mov     dword ptr [ebp - 150H], ecx
		mov     dword ptr [ebp - 14CH], edx
		mov     ecx, dword ptr [eax + 4H]
		mov     edx, dword ptr [eax]
		mov     dword ptr [ebp - 15CH], ecx
		mov     dword ptr [ebp - 160H], edx
		sub     ecx, edx
		mov     eax, dword ptr [ebp - 168H]
		mov     dword ptr [ebp - 164H], ecx
		cmp     eax, ecx
		jge     L2
		jmp     L3

	L2:
		mov     eax, ecx
	L3:
		mov     dword ptr [ebp - 170H], eax
		push    eax
		mov     ecx, dword ptr [ebp - 158H]
		mov     edx, dword ptr [ebp - 160H]
		push    ecx
		push    edx
		call    memcmp
		add     esp, 12
		mov     dword ptr [ebp - 16CH], eax
		test    eax, eax
		jz      L4
		jl      L5
		jmp     L11

	L4:
		mov     eax, dword ptr [ebp - 164H]
		mov     ecx, dword ptr [ebp - 168H]
		cmp     eax, ecx
		jg      L11
	L5:
		mov     dword ptr [ebp - 50H], 0
		lea     eax, [ebp - 98H]
		mov     dword ptr [ebp - 17CH], eax
#if !OPTIMIZE_ALLOCATOR
		push    64
		call    dword ptr [node_alloc_allocate]
		pop     ecx
#else
		mov     ecx, 64
		call    node_alloc_allocate
#endif
		mov     dword ptr [ebp - 50H], eax
		mov     byte ptr [eax], 0
		mov     dword ptr [eax + 4H], 0
		mov     dword ptr [eax + 8H], eax
		mov     dword ptr [eax + 0CH], eax
		mov     ecx, dword ptr [ebp - 184H]
		mov     eax, dword ptr [ebp - 180H]
		mov     dword ptr [ebp - 48H], 0
		mov     dword ptr [ebp - 40H], ecx
		mov     dword ptr [ebp - 3CH], eax
		lea     eax, [ebp - 58H]
		mov     edx, dword ptr [ebp - 130H]
		mov     dword ptr [ebp - 178H], eax
		mov     ecx, ebx
		call    string_ctor_assign
		lea     eax, [ebp - 0A8H]
		mov     dword ptr [ebx + 20H], 0
		mov     dword ptr [ebp - 188H], eax
#if !OPTIMIZE_ALLOCATOR
		push    64
		call    dword ptr [node_alloc_allocate]
		pop     ecx
#else
		mov     ecx, 64
		call    node_alloc_allocate
#endif
		mov     dword ptr [ebx + 20H], eax
		mov     eax, dword ptr [ebp - 178H]
		mov     dword ptr [ebx + 28H], 0
		mov     ecx, dword ptr [eax + 18H]
		mov     edx, dword ptr [eax + 1CH]
		mov     eax, dword ptr [eax + 8H]
		mov     dword ptr [ebx + 30H], ecx
		mov     dword ptr [ebx + 34H], edx
		mov     eax, dword ptr [eax + 4H]
		test    eax, eax
		jnz     L6
		mov     eax, dword ptr [ebx + 20H]
		mov     byte ptr [eax], 0
		mov     dword ptr [eax + 4H], 0
		mov     dword ptr [eax + 8H], eax
		mov     dword ptr [eax + 0CH], eax
		jmp     L7

	L6:
		mov     ecx, dword ptr [ebp - 178H]
		mov     edx, dword ptr [ebx + 20H]
		mov     ecx, dword ptr [ecx + 8H]
		mov     byte ptr [edx], 0
		mov     ecx, dword ptr [ecx + 4H]
		push    edx
		push    ecx
		lea     edx, [ebx + 18H]
		push    edx
		call    dword ptr [F004F32FC]
		mov     ecx, dword ptr [ebx + 20H]
		add     esp, 12
		mov     dword ptr [ecx + 4H], ecx
		mov     dword ptr [ebp - 18CH], ecx
		push    ecx
		call    dword ptr [F004185E8]
		mov     ecx, dword ptr [ebx + 20H]
		mov     dword ptr [ecx + 8H], ecx
		mov     ecx, dword ptr [ecx + 4H]
		mov     dword ptr [ebp - 190H], ecx
		mov     dword ptr [esp], ecx
		call    dword ptr [F004185FC]
		pop     ecx
		mov     ecx, dword ptr [ebx + 20H]
		mov     dword ptr [ecx + 0CH], eax
	L7:
		mov     ecx, dword ptr [ebp - 178H]
		mov     edx, dword ptr [ebp - 13CH]
		mov     ecx, dword ptr [ecx + 10H]
		mov     dword ptr [ebp - 194H], edx
		mov     dword ptr [ebx + 28H], ecx
		mov     dword ptr [ebp - 174H], ebx
		push    ebx
		push    ebx
		mov     ecx, dword ptr [ebp - 194H]
		mov     edx, dword ptr [ebp - 134H]
		mov     dword ptr [esp], ecx
		lea     ecx, [ebp - 198H]
		push    edx
		push    ecx
		call    dword ptr [F004F3410]
		add     esp, 16
		lea     eax, [ebp - 198H]
		mov     ecx, dword ptr [eax]
		mov     eax, dword ptr [ebx + 28H]
		mov     dword ptr [ebp - 13CH], ecx
		test    eax, eax
		jz      L8
		mov     ecx, dword ptr [ebx + 20H]
		lea     edx, [ebx + 18H]
		mov     ecx, dword ptr [ecx + 4H]
		push    ecx
		push    edx
		call    dword ptr [F00469A5C]
		mov     eax, dword ptr [ebx + 20H]
		add     esp, 8
		mov     dword ptr [eax + 4H], 0
		mov     dword ptr [eax + 8H], eax
		mov     dword ptr [eax + 0CH], eax
		mov     dword ptr [ebx + 28H], 0
	L8:
		mov     ecx, dword ptr [ebx + 20H]
		mov     dword ptr [ebp - 19CH], ecx
		test    ecx, ecx
		jz      L9
#if !OPTIMIZE_ALLOCATOR
		push    64
		push    ecx
		call    dword ptr [node_alloc_deallocate]
		add     esp, 8
#else
		call    node_alloc_deallocate
#endif
	L9:
		mov     ecx, ebx
		call    string_dtor
		cmp     dword ptr [ebp - 48H], 0
		jz      L10
		mov     edx, dword ptr [ebp - 50H]
		mov     edx, dword ptr [edx + 4H]
		lea     ecx, [ebp - 58H]
		push    edx
		push    ecx
		call    dword ptr [F00469A5C]
		mov     eax, dword ptr [ebp - 50H]
		add     esp, 8
		mov     dword ptr [eax + 4H], 0
		mov     dword ptr [eax + 8H], eax
		mov     dword ptr [eax + 0CH], eax
		mov     dword ptr [ebp - 48H], 0
	L10:
		mov     ecx, dword ptr [ebp - 50H]
		mov     dword ptr [ebp - 1A8H], ecx
		test    ecx, ecx
		jz      L11
#if !OPTIMIZE_ALLOCATOR
		push    64
		push    ecx
		call    dword ptr [node_alloc_deallocate]
		add     esp, 8
#else
		call    node_alloc_deallocate
#endif
	L11:
		mov     edx, dword ptr [ebp - 13CH]
		add     edx, 40
		lea     ecx, [ebp - 20H]
		mov     dword ptr [ebp - 1ACH], edx
		push    ecx
		push    edx
		call    dword ptr [F004F20E4]
		add     esp, 8
		mov     ecx, dword ptr [ebp - 1ACH]
		mov     dword ptr [ebp - 1B0H], eax
		mov     ecx, dword ptr [ecx + 8H]
		mov     dword ptr [ebp - 1B4H], eax
		mov     dword ptr [ebp - 1B8H], ecx
		mov     dword ptr [ebp - 1BCH], ecx
		cmp     ecx, eax
		je      L13
		mov     eax, dword ptr [ebp - 1B4H]
		add     eax, 16
		mov     ecx, dword ptr [eax + 4H]
		mov     edx, dword ptr [eax]
		mov     dword ptr [ebp - 1C0H], eax
		mov     dword ptr [ebp - 1CCH], ecx
		mov     eax, dword ptr [ebp - 1ACH]
		mov     dword ptr [ebp - 1D0H], edx
		mov     ecx, dword ptr [eax + 18H]
		mov     eax, dword ptr [eax + 1CH]
		mov     dword ptr [ebp - 1C8H], ecx
		mov     dword ptr [ebp - 1C4H], eax
		mov     ecx, dword ptr [ebp - 1CH]
		mov     eax, dword ptr [ebp - 20H]
		mov     dword ptr [ebp - 1D4H], ecx
		mov     dword ptr [ebp - 1D8H], eax
		sub     ecx, eax
		mov     edx, dword ptr [ebp - 1D0H]
		mov     dword ptr [ebp - 1DCH], ecx
		mov     eax, dword ptr [ebp - 1CCH]
		sub     eax, edx
		mov     dword ptr [ebp - 1E0H], eax
		cmp     eax, ecx
		jl      L12
		mov     eax, ecx
	L12:
		mov     dword ptr [ebp - 1E8H], eax
		push    eax
		mov     ecx, dword ptr [ebp - 1D0H]
		mov     edx, dword ptr [ebp - 1D8H]
		push    ecx
		push    edx
		call    memcmp
		add     esp, 12
		mov     dword ptr [ebp - 1E4H], eax
		test    eax, eax
		jnz     L14
		mov     eax, dword ptr [ebp - 1DCH]
		mov     ecx, dword ptr [ebp - 1E0H]
		cmp     eax, ecx
		jl      L14
		jmp     L15

	L13:
		mov     dword ptr [ebp - 0D0H], 0
		lea     ecx, [ebp - 0B8H]
		mov     dword ptr [ebp - 1F4H], ecx
		lea     ecx, [ebp - 0D0H]
		mov     dword ptr [ebp - 1F0H], ecx
		lea     edx, [ebp - 20H]
		mov     dword ptr [ebp - 0CCH], 0
		lea     ecx, [ebp - 100H]
		mov     dword ptr [ebp - 0C0H], 0
		call    string_ctor_assign
		lea     eax, [ebp - 108H]
		mov     ecx, dword ptr [ebp - 1F0H]
		mov     dword ptr [ebp - 1F8H], eax
		mov     eax, dword ptr [ecx + 4H]
		mov     ecx, dword ptr [ecx]
		sub     eax, ecx
		mov     ecx, 0AAAAAAABH
		mul     ecx
		mov     ecx, edx
		xor     eax, eax
		shr     ecx, 4
		mov     dword ptr [ebp - 0E8H], eax
		mov     dword ptr [ebp - 0E4H], eax
		mov     dword ptr [ebp - 0D8H], eax
		mov     dword ptr [ebp - 1FCH], ecx
		jz      L14
		shl     ecx, 3
		lea     ecx, [ecx + ecx * 2]
		mov     dword ptr [ebp - 200H], ecx
		call    internal_allocate
	L14:
		mov     ecx, dword ptr [ebp - 1FCH]
		mov     dword ptr [ebp - 0E8H], eax
		shl     ecx, 3
		lea     edx, [ebp - 208H]
		mov     dword ptr [ebp - 0E4H], edx
		lea     ecx, [ecx + ecx * 2]
		push    edx
		add     ecx, eax
		mov     edx, dword ptr [ebp - 1F0H]
		mov     dword ptr [ebp - 0D8H], ecx
		push    eax
		mov     ecx, dword ptr [edx + 4H]
		mov     edx, dword ptr [edx]
		push    ecx
		push    edx
		call    dword ptr [_STL__uninitialized_copy]
		mov     dword ptr [ebp - 0E4H], eax
		lea     eax, [ebp - 100H]
		add     esp, 16
		mov     edx, dword ptr [ebp - 1B4H]
		push    eax
		push    eax
		mov     dword ptr [ebp - 1ECH], eax
		mov     dword ptr [ebp - 20CH], edx
		mov     dword ptr [esp], edx
		mov     edx, dword ptr [ebp - 1ACH]
		push    edx
		lea     ecx, [ebp - 210H]
		push    ecx
		call    dword ptr [F004F2424]
		add     esp, 16
		lea     edx, [ebp - 210H]
		mov     edx, dword ptr [edx]
		mov     dword ptr [ebp - 1B4H], edx
		lea     ecx, [ebp - 0E8H]
		call    vector_string_dtor
		lea     ecx, [ebp - 100H]
		call    string_dtor
		lea     ecx, [ebp - 0D0H]
		call    vector_string_dtor
	L15:
		mov     ecx, dword ptr [ebp - 1B4H]
		add     ecx, 40
		push    ecx
		call    dword ptr [F0044CA38]
		add     esp, 8
		lea     ecx, [ebp - 20H]
		call    string_dtor
		lea     ecx, [ebp - 38H]
		call    string_dtor
		mov     ecx, dword ptr [ebp - 300]
		mov     dword ptr fs:[0], ecx
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret

		#undef this
		#undef Data
		#undef FileName
		#undef IsSSL
	}
}
