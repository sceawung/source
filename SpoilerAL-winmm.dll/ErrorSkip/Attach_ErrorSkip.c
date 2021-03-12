#include <windows.h>
#include <dhcpsapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

EXTERN_C void __cdecl TSSBundleList_Write_ErrorSkip();
EXTERN_C unsigned long __stdcall TSSDir_WriteChildren_ErrorSkip(LPVOID, LPVOID, LPCVOID);
EXTERN_C void __cdecl TSSGCtrl_Write_ErrorSkip();
EXTERN_C void __cdecl TSSGActionListner_OnSubjectWriteError_ErrorSkip();

static __declspec(naked) void TSSBundleCalc_Read_AE_reset()
{
	__asm {
		movzx ecx, byte ptr [ebx + 0x78]
		jecxz REVERT
		xor   eax, eax
		push  edi
		mov   edi, dword ptr [ebp - 0x24]
		rep   stosb
		pop   edi
	REVERT:
		rep ret
	}
}

static DWORD_DWORD __fastcall TSSGSubject_ErrorOverride(TSSGSubject *this, unsigned long error)
{
	return (DWORD_DWORD) { error && !TSSGCtrl_GetAttribute(NULL, this, atON_ERROR), error };
}

static __declspec(naked) ResErr TSSGCtrl_ErrorOverride()
{
	__asm {
		mov edx, eax
		mov ecx, dword ptr [ebp + 8]
		jmp TSSGSubject_ErrorOverride
	}
}

static __declspec(naked) ResErr __cdecl TSSGCtrl_One_ErrorOverride()
{
	__asm {
		movzx edx, al
		xor   edx, TRUE
		mov   ecx, dword ptr [ebp + 8]
		jmp   TSSGSubject_ErrorOverride
	}
}

static __declspec(naked) ResErr __cdecl TSSGCtrl_Compare_ErrorOverride()
{
	__asm {
		mov edx, reACCESS_ERROR
		mov ecx, dword ptr [ebp + 8]
		jmp TSSGSubject_ErrorOverride
	}
}

__declspec(naked)
static bool __cdecl TSSDir_IsSameChildren_IsSameSubject(TSSGSubject* const this, TSSGSubject* const FirstSubject) {
	__asm {
		mov   ecx, [esp + 4]
		cmp   [ecx]TSSGSubject.type, stSPLIT
		je    SPLIT

		mov   eax, [ecx]TSSGSubject.VTable
		jmp   [eax]SubjectVtbl.IsSameSubject
		int   3

		align 16
	SPLIT:
		push  atCHILD_RW
		push  ecx
		push  0
		call  TSSGCtrl_GetAttribute
		add   esp, 12
		test  eax, eax
		jz    REVERT

		movzx eax, [eax]TChildRWAttribute.spAccept
	REVERT:
		rep ret
	}
}

#define PUSH_ECX   (BYTE)0x51
#define JZ_SHORT   (BYTE)0x74
#define JNZ_SHORT  (BYTE)0x75
#define NOP        (BYTE)0x90
#define JECXZ      (BYTE)0xE3
#define CALL_REL32 (BYTE)0xE8
#define JMP_REL32  (BYTE)0xE9

EXTERN_C void __cdecl Attach_ErrorSkip()
{
	// TSSBundleCalc::Read
#if 0
	*(LPBYTE )0x004BDCB7 = 0x83;
	*(LPBYTE )0x004BDCB8 = 0x6E;// dec => sub
	*(LPBYTE )0x004BDCBA = 0x03;
	*(LPBYTE )0x004BDCBB = CALL_REL32;
	*(LPDWORD)0x004BDCBC = (DWORD)TSSBundleCalc_Read_AE_reset - (0x004BDCBC + sizeof(DWORD));

#endif
	//   primary interrupt
	*(LPBYTE )0x004BDCDC = CALL_REL32;
	*(LPDWORD)0x004BDCDD = (DWORD)TSSGCtrl_ErrorOverride - (0x004BDCDD + sizeof(DWORD));
	*(LPBYTE )0x004BDCE1 =         0x85;// test eax, eax
	*(LPWORD )0x004BDCE2 = BSWAP16(0xC00F);
	*(LPBYTE )0x004BDCE4 =         0x84;// jz
	*(LPDWORD)0x004BDCE5 = 0x004BDDD7 - (0x004BDCE5 + sizeof(DWORD));

	//   secondary, goto next on error
	*(LPBYTE )0x004BDDCD =         0x33   ;
	*(LPWORD )0x004BDDCE = BSWAP16(0xC964);// xor   ecx, ecx
	*(LPWORD )0x004BDDD0 = BSWAP16(0x8911);// mov   dword ptr fs:[ecx], edx
	*(LPBYTE )0x004BDDD2 = JMP_REL32;
	*(LPDWORD)0x004BDDD3 = 0x004BDF69 - (0x004BDDD3 + sizeof(DWORD));
	*(LPWORD )0x004BDDD7 =         0x85   ;// test  edx, edx
	*(LPWORD )0x004BDDD8 = BSWAP16(0xD2 << 8 | JNZ_SHORT);
	*(LPBYTE )0x004BDDDA = 0x004BDE1C - (0x004BDDDA + sizeof(BYTE));
	*(LPBYTE )0x004BDDDB =         0x0F   ;// movzx ecx, byte ptr [ebx + 0x78]
	*(LPDWORD)0x004BDDDC = BSWAP32(0xB64B78 << 8 | PUSH_ECX);
	*(LPWORD )0x004BDDE0 = BSWAP16(0xFF75);// push  dword ptr [ebp - 0x24] 
	*(LPBYTE )0x004BDDE2 =         0xDC   ;

	// TSSBundleCalc::Write
	*(LPBYTE )0x004BE580 = CALL_REL32;
	*(LPDWORD)0x004BE581 = (DWORD)TSSGCtrl_ErrorOverride - (0x004BE581 + sizeof(DWORD));
	*(LPBYTE )0x004BE585 =         0x85;// test eax, eax
	*(LPWORD )0x004BE586 = BSWAP16(0xC00F);
	*(LPBYTE )0x004BE588 =         0x84;// jz
	*(LPDWORD)0x004BE589 = 0x004BE61A - (0x004BE589 + sizeof(DWORD));

	// TSSBundleList::Read
	//   Val &= 0;
	*(LPBYTE )0x004BF591 =         0x0F   ;
	*(LPWORD )0x004BF592 = BSWAP16(0xB64E);// movzx ecx, byte ptr [esi + 0x78]
	*(LPWORD )0x004BF594 = BSWAP16(0x7851);// push  ecx
	*(LPBYTE )0x004BF599 =         0x83   ;
	*(LPWORD )0x004BF59A = BSWAP16(0x2000);// and   dword ptr [eax], 0
	*(LPDWORD)0x004BF59C = BSWAP32(0x50FF75C0);// push eax
	*(LPWORD )0x004BF5A0 = BSWAP16(0xFF75);// push  dword ptr [ebp - 0x40]
	*(LPBYTE )0x004BF5A2 =         0xFC   ;// push  dword ptr [ebp - 0x04]

	*(LPBYTE )0x004BF5B0 = CALL_REL32;
	*(LPDWORD)0x004BF5B1 = (DWORD)TSSGCtrl_One_ErrorOverride - (0x004BF5B1 + sizeof(DWORD));
	*(LPBYTE )0x004BF5B5 =         0x85;// test eax, eax
	*(LPWORD )0x004BF5B6 = BSWAP16(0xC0 << 8 | JZ_SHORT);
	*(LPBYTE )0x004BF5B8 = 0x004BF5DF - (0x004BF5B8 + sizeof(BYTE));

	// TSSBundleList::Write
#if 0
	*(LPDWORD)(0x004BF899 + 1) = (DWORD)TSSBundleList_Write_ErrorSkip - (0x004BF899 + 1 + sizeof(DWORD));
#else
	*(LPBYTE )0x004BF8A1 = CALL_REL32;
	*(LPDWORD)0x004BF8A2 = (DWORD)TSSGCtrl_One_ErrorOverride - (0x004BF8A2 + sizeof(DWORD));
	*(LPWORD )0x004BF8A6 = BSWAP16(0x85C0);// test eax, eax
	*(LPBYTE )0x004BF8A8 = JZ_SHORT;
	*(LPBYTE )0x004BF8A9 = 0x004BF8D0 - (0x004BF8A9 + sizeof(BYTE));
#endif

	// TSSBundleToggle::Read
	*(LPBYTE )0x004C0774 = CALL_REL32;
	*(LPDWORD)0x004C0775 = (DWORD)TSSGCtrl_Compare_ErrorOverride - (0x004C0775 + sizeof(DWORD));

	// TSSBundleToggle::Write
	*(LPBYTE )0x004C10DB = CALL_REL32;
	*(LPDWORD)0x004C10DC = (DWORD)TSSGCtrl_ErrorOverride - (0x004C10DC + sizeof(DWORD));
	*(LPDWORD)0x004C10E0 = BSWAP32(0x85C00F84);// test eax, eax; jz
	*(LPDWORD)0x004C10E4 = 0x004C1213 - (0x004C10E4 + sizeof(DWORD));

	// TSSDir::IsSameChildren
	*(LPBYTE )0x004C3217 = CALL_REL32;
	*(LPDWORD)0x004C3218 = (DWORD)TSSDir_IsSameChildren_IsSameSubject - (0x004C3218 + sizeof(DWORD));

	// TSSDir::WriteChildren
	*(LPBYTE )0x004C32A1 = CALL_REL32;
	*(LPDWORD)0x004C32A2 = (DWORD)TSSDir_WriteChildren_ErrorSkip - (0x004C32A2 + sizeof(DWORD));
	*(LPBYTE )0x004C32A6 = JECXZ;
#if 0

	// TSSGCtrl::Write
	*(LPBYTE )0x0051C455 = JMP_REL32;
	*(LPDWORD)0x0051C456 = (DWORD)TSSGCtrl_Write_ErrorSkip - (0x0051C456 + sizeof(DWORD));
#endif

	// TSSBundleFloatCalc::Read
#if 0
	*(LPBYTE )0x0052E2F7 = 0x83;
	*(LPBYTE )0x0052E2F8 = 0x6E;
	*(LPBYTE )0x0052E2FA = 0x03;
	*(LPBYTE )0x0052E2FB = CALL_REL32;
	*(LPDWORD)0x0052E2FC = (DWORD)TSSBundleCalc_Read_AE_reset - (0x0052E2FC + sizeof(DWORD));

#endif
	//   primary interrupt
	*(LPBYTE )0x0052E31C = CALL_REL32;
	*(LPDWORD)0x0052E31D = (DWORD)TSSGCtrl_ErrorOverride - (0x0052E31D + sizeof(DWORD));
	*(LPBYTE )0x0052E321 =         0x85;// test eax, eax
	*(LPWORD )0x0052E322 = BSWAP16(0xC00F);
	*(LPBYTE )0x0052E324 =         0x84;// jz
	*(LPDWORD)0x0052E325 = 0x0052E417 - (0x0052E325 + sizeof(DWORD));

	//   secondary, goto next on error
	*(LPBYTE )0x0052E40D =         0x33   ;
	*(LPWORD )0x0052E40E = BSWAP16(0xC964);// xor   ecx, ecx
	*(LPWORD )0x0052E410 = BSWAP16(0x8911);// mov   dword ptr fs:[ecx], edx
	*(LPBYTE )0x0052E412 = JMP_REL32;
	*(LPDWORD)0x0052E413 = 0x0052E61F - (0x0052E413 + sizeof(DWORD));
	*(LPWORD )0x0052E417 =         0x85   ;// test  edx, edx
	*(LPWORD )0x0052E418 = BSWAP16(0xD2 << 8 | JNZ_SHORT);
	*(LPBYTE )0x0052E41A = 0x0052E494 - (0x0052E41A + sizeof(BYTE));
	*(LPBYTE )0x0052E41B =         0x0F   ;// movzx ecx, byte ptr [ebx + 0x78]
	*(LPDWORD)0x0052E41C = BSWAP32(0xB64B78 << 8 | PUSH_ECX);
	*(LPWORD )0x0052E420 = BSWAP16(0xFF75);// push  dword ptr [ebp - 0x24] 
	*(LPBYTE )0x0052E422 =         0xDC   ;
	
	// TSSBundleFloatCalc::Write
	*(LPBYTE )0x0052E93D = CALL_REL32;
	*(LPDWORD)0x0052E93E = (DWORD)TSSGCtrl_One_ErrorOverride - (0x0052E93E + sizeof(DWORD));
	*(LPWORD )0x0052E942 = BSWAP16(0x85C0);// test eax, eax
	*(LPBYTE )0x0052E944 = JZ_SHORT;
	*(LPBYTE )0x0052E945 = 0x0052E969 - (0x0052E945 + sizeof(BYTE));

	*(LPBYTE )0x0052EABF = CALL_REL32;
	*(LPDWORD)0x0052EAC0 = (DWORD)TSSGCtrl_One_ErrorOverride - (0x0052EAC0 + sizeof(DWORD));
	*(LPWORD )0x0052EAC4 = BSWAP16(0x85C0);// test eax, eax
	*(LPBYTE )0x0052EAC6 = JZ_SHORT;
	*(LPBYTE )0x0052EAC7 = 0x0052EAE8 - (0x0052EAC7 + sizeof(BYTE));
#if 0

	// TSSGActionListner::OnSubjectWriteError
	*(LPDWORD)(0x0052F1E0 + 1) = (DWORD)TSSGActionListner_OnSubjectWriteError_ErrorSkip - (0x0052F1E0 + 1 + sizeof(DWORD));
#endif
}
