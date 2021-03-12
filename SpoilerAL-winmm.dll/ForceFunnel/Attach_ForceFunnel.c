#include <windows.h>
#include "intrinsic.h"
#include "TSSArg.h"
#include "TSSGCtrl.h"

EXTERN_C void __cdecl TSSBitList_Write_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleList_WriteOne_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleList_Write_CheckFunnel();
EXTERN_C void __cdecl TSSDoubleToggle_Write_CheckFunnel();
EXTERN_C void __cdecl TSSFloatCalc_Write_CheckFunnel();
EXTERN_C void __cdecl TSSString_Write_CheckFunnel();
EXTERN_C void __cdecl TSSBundleFloatCalc_Write_CheckFunnel();

static __declspec(naked) bool __cdecl TSSCalc_Write_CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, unsigned long Val) {
	extern BOOL FixTheProcedure;
	__asm {
		cmp FixTheProcedure, FALSE
		je  PASS
		mov edx, [ebp + 0x10]// Arg
		mov edx, [edx]TSSArgLong.value
		mov [esp + 0x0C], edx// Val
	PASS:
		jmp TSSGCtrl_CheckFunnel
	}
}

#define PUSH_EDX   (BYTE)0x52
#define PUSH_IMM8  (BYTE)0x6A
#define JZ_SHORT   (BYTE)0x74
#define JNZ_SHORT  (BYTE)0x75
#define JZ_REL32   (WORD)0x840F
#define JNZ_REL32  (WORD)0x850F
#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void __cdecl Attach_ForceFunnel()
{
	// TSSBitList::Write - CheckFunnel
	*(LPBYTE )0x004BB703 = JNZ_SHORT;
	*(LPBYTE )0x004BB705 = PUSH_IMM8;
	*(LPBYTE )0x004BB706 = reACCESS_ERROR;
	*(LPBYTE )0x004BB708 = 0x004BB70F - (0x004BB708 + sizeof(BYTE));
	*(LPBYTE )0x004BB709 = CALL_REL32;
	*(LPDWORD)0x004BB70A = (DWORD)TSSBitList_Write_CheckFunnel - (0x004BB70A + sizeof(DWORD));

	// TSSBundleCalc::Write - CheckFunnel
	*(LPDWORD)0x004BE6B5 = (DWORD)TSSCalc_Write_CheckFunnel - (0x004BE6B5 + sizeof(DWORD));

	// TSSCalc::Write - CheckFunnel
	*(LPDWORD)0x004C20F2 = (DWORD)TSSCalc_Write_CheckFunnel - (0x004C20F2 + sizeof(DWORD));

	// TSSDoubleList::Write - CheckFunnel
	*(LPBYTE )0x004C5507 = JNZ_SHORT;
	*(LPBYTE )0x004C5509 = PUSH_IMM8;
	*(LPBYTE )0x004C550A = reACCESS_ERROR;
	*(LPBYTE )0x004C550C = 0x004C5513 - (0x004C550C + sizeof(BYTE));
	*(LPBYTE )0x004C550D = CALL_REL32;
	*(LPDWORD)0x004C550E = (DWORD)TSSDoubleList_WriteOne_CheckFunnel - (0x004C550E + sizeof(DWORD));

	*(LPBYTE )0x004C6283 = CALL_REL32;
	*(LPDWORD)0x004C6284 = (DWORD)TSSDoubleList_Write_CheckFunnel - (0x004C6284 + sizeof(DWORD));
	*(LPWORD )0x004C6288 = BSWAP16(0x85D2);// test edx, edx
	*(LPBYTE )0x004C628A = JZ_SHORT;
	*(LPBYTE )0x004C628B = 0x004C6292 - (0x004C628B + sizeof(BYTE));

	// TSSDoubleToggle::Write - CheckFunnel
	*(LPBYTE )0x004CD908 = CALL_REL32;
	*(LPDWORD)0x004CD909 = (DWORD)TSSDoubleToggle_Write_CheckFunnel - (0x004CD909 + sizeof(DWORD));
	*(LPBYTE )0x004CD90D = PUSH_EDX;
	*(LPWORD )0x004CD90E = BSWAP16(0x836B);// sub dword ptr [ebx + ...], 2
	*(LPBYTE )0x004CD911 =         0x02   ;
	*(LPWORD )0x004CD912 = BSWAP16(0x3955);// cmp dword ptr [ebp - 3Ch], edx
	*(LPBYTE )0x004CD914 =         0xC4   ;

	// TSSFloatCalc::Write - CheckFunnel
	*(LPBYTE )0x004CE69A = CALL_REL32;
	*(LPDWORD)0x004CE69B = (DWORD)TSSFloatCalc_Write_CheckFunnel - (0x004CE69B + sizeof(DWORD));
	*(LPBYTE )0x004CE69F =         0x85;// test edx, edx
	*(LPWORD )0x004CE6A0 = BSWAP16(0xD2 << 8 | JZ_SHORT);
	*(LPBYTE )0x004CE6A2 = 0x004CE6A9 - (0x004CE6A2 + sizeof(BYTE));

	// TSSString::Write - CheckFunnel
	*(LPBYTE )0x0052B558 = JZ_SHORT;
	*(LPBYTE )0x0052B55A = PUSH_IMM8;
	*(LPBYTE )0x0052B55B = reACCESS_ERROR;
	*(LPBYTE )0x0052B55D = 0x0052B564 - (0x0052B55D + sizeof(BYTE));
	*(LPBYTE )0x0052B55E = CALL_REL32;
	*(LPDWORD)0x0052B55F = (DWORD)TSSString_Write_CheckFunnel - (0x0052B55F + sizeof(DWORD));

	// TSSBundleFloatCalc::Write - CheckFunnel
	*(LPBYTE )0x0052EAFD = CALL_REL32;
	*(LPDWORD)0x0052EAFE = (DWORD)TSSBundleFloatCalc_Write_CheckFunnel - (0x0052EAFE + sizeof(DWORD));
	*(LPWORD )0x0052EB02 = BSWAP16(0x85D2);// test edx, edx
	*(LPBYTE )0x0052EB04 = JZ_SHORT;
	*(LPBYTE )0x0052EB05 = 0x0052EB0C - (0x0052EB05 + sizeof(BYTE));
}
