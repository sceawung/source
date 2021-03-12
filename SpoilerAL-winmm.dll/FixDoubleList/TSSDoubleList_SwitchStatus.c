#include <windows.h>
#include "TSSGSubject.h"

static __declspec(naked) void __fastcall TSSDoubleList_SwitchStatus(unsigned long Index, void* ListFile)
{
	__asm
	{// Borland's specification
		#define AddressTable eax
		#define ListFile     edx
		#define Index        ecx

		mov     edx, dword ptr [ListFile]
		lea     ecx, [Index + Index * 2]
		mov     edx, dword ptr [edx + ecx * 8]
		movzx   ecx, byte  ptr [edx]
		mov     edx, dword ptr [AddressTable]
		cmp     cl, ','
		cmove   edx, dword ptr [AddressTable + 0x04]
		cmp     cl, '@'
		cmove   edx, dword ptr [AddressTable + 0x08]
		test    cl, cl
		cmovz   edx, dword ptr [AddressTable + 0x0C]
		mov     eax, reNO_ERROR
		jmp     edx
		ud2

		#undef Index
		#undef ListFile
		#undef AddressTable
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Read_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C4390,
		0x004C445A,
		0x004C4B4D,
		0x004C5209
	};

	__asm
	{
		#define ListFile (ebp - 160H)
		#define i        (ebp - 16CH)

		mov     ecx, dword ptr [i]
		mov     edx, dword ptr [ListFile]
		mov     eax, offset AddressTable
		jmp     TSSDoubleList_SwitchStatus

		#undef ListFile
		#undef i
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Write_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C5477,
		0x004C555A,
		0x004C5BF4,
		0x004C5512
	};

	__asm
	{
		#define ListFile (ebp - 15CH)
		#define Index    (ebp - 160H)

		mov     ecx, dword ptr [Index]
		mov     edx, dword ptr [ListFile]
		mov     eax, offset AddressTable
		jmp     TSSDoubleList_SwitchStatus

		#undef ListFile
		#undef Index
	}
}

__declspec(naked) void __cdecl TSSDoubleList_ToByteCode_SwitchStatus()
{
	static const DWORD AddressTable[] = {
		0x004C6755,
		0x004C6A01,
		0x004C721F,
		0x004C7A1E
	};

	__asm
	{
		#define ListFile (ebp - 27CH)
		#define Index    (ebp - 288H)

		mov     ecx, dword ptr [Index]
		mov     edx, dword ptr [ListFile]
		mov     eax, offset AddressTable
		jmp     TSSDoubleList_SwitchStatus

		#undef ListFile
		#undef Index
	}
}
