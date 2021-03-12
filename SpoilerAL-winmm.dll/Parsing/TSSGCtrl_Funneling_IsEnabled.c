#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

extern BOOL FixTheProcedure;
extern const DWORD _TSSGCtrl_MakeDataCode;

__declspec(naked) bool __cdecl TSSGCtrl_Funneling_IsEnabled(TSSGCtrl *SSGC, TSSGSubject *SSGS)
{
	__asm
	{
		mov     eax, FixTheProcedure
		test     al, al
		jz      IsEnabled
		rep ret

	IsEnabled:
		jmp     TSSGCtrl_IsEnabled
		ud2
	}
}

__declspec(naked) unsigned long __cdecl TSSGCtrl_Funneling_GetAddress(TSSGCtrl *SSGC, TSSGSubject *SSGS, const string *AddressStr, unsigned long Mode)
{
	__asm
	{
		mov     eax, FixTheProcedure
		test    eax, eax
		jz      GetAddress
		rep ret

	GetAddress:
		jmp     TSSGCtrl_GetAddress
		ud2
	}
}

__declspec(naked) vector* __cdecl TSSGCtrl_Funneling_MakeDataCode(
	struct TSmartVector {
		vector   data;
	}           *CompareVec,
	TSSGCtrl    *this,
	TSSGSubject *SSGS,
	string       OnCode,
	string      *OffCode,
	bool         IsOn,
	LPCVOID      StartAddress,
	ptrdiff_t    OffsetAddress)
{
	__asm
	{
		cmp     FixTheProcedure, 0
		je      MakeDataCode
		mov     ecx, dword ptr [ebp - 0x34]     // tmpV
		lea     edx, [ecx + size string]        // tmpV[1]
		push    atALL                           // Mode
		push    edx                             // AddressStr = tmpV[1]
		push    dword ptr [ebp + 0x0C]          // SSGS
		push    edi                             // this
		call    TSSGCtrl_GetAddress
		add     esp, 0x10
		mov     dword ptr [ebp - 0x0148], eax   // Address
		mov     dword ptr [esp + 0x30  ], eax   // StartAddress
	MakeDataCode:
		jmp     dword ptr [_TSSGCtrl_MakeDataCode]
		ud2
	}
}
