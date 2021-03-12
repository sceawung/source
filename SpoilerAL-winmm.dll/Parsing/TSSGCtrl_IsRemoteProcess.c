#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

#define IO_FEP_SUPPORT       1
#define SUBJECT_STRING_TABLE 1

EXTERN_C void __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP();
EXTERN_C void __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP();
EXTERN_C string * __fastcall SubjectStringTable_GetString(string *s);

static void __fastcall CheckAddress(LPCSTR p);
static void __fastcall CheckAddressWithStringDtor(string *s);

#ifndef _M_IX86
BOOL __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p)
{
	char c;

	do
		c = *(p++);
	while (__intrinsic_isspace(c));
	if (c == '_')
		do
			c = *(p++);
		while (__intrinsic_isspace(c));
	if (c == 'L')
	{
		c = *p;
		if (c && __intrinsic_isascii(c) && !__intrinsic_isalnum(c) && c != '_')
			return FALSE;
	}
	return TRUE;
}
#else
__declspec(naked) BOOL __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p)
{
	__asm
	{
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		mov     dl, al
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L1
		cmp     dl, '_'
		jne     L3
	L2:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		mov     dl, al
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L2
	L3:
		cmp     dl, 'L'
		jne     L4
		mov     cl, byte ptr [ecx]
		xor     eax, eax
		test    cl, cl
		jle     L4
		mov     dl, cl
		sub     cl, '0'
		cmp     cl, '9' - '0' + 1
		jb      L4
		mov     cl, dl
		or      dl, 'a' - 'A'
		sub     dl, 'a'
		cmp     dl, 'z' - 'a' + 1
		jb      L4
		cmp     cl, '_'
		jne     L5
	L4:
		mov     eax, 1
	L5:
		rep ret
	}
}
#endif

#define TSSGCtrl_Read     0x0051C3A0
#define TSSGCtrl_Write    0x0051C408
#define TSSGCtrl_Compare  0x0051C470
#define TSSGCtrl_OneRead  0x0051C4D8
#define TSSGCtrl_OneWrite 0x0051C540

__declspec(naked) void __cdecl TSSBundleCalc_Read_Read()
{
	__asm
	{
		push    TSSGCtrl_Read
		lea     ecx, [ebp - 7CH]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Write_Write()
{
	__asm
	{
		push    TSSGCtrl_Write
		lea     ecx, [ebp - 64H]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSBundleList_Read_OneRead()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 50H]
#if !IO_FEP_SUPPORT
		push    TSSGCtrl_OneRead
#else
		push    offset TSSGCtrl_OneRead_with_CheckIO_FEP
#endif
		mov     ecx, dword ptr [ecx]
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSBundleList_Write_OneWrite()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 4CH]
#if !IO_FEP_SUPPORT
		push    TSSGCtrl_OneWrite
#else
		push    offset TSSGCtrl_OneWrite_with_CheckIO_FEP
#endif
		mov     ecx, dword ptr [ecx]
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSBundleToggle_Read_Compare()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 34H]
		push    TSSGCtrl_Compare
		mov     ecx, dword ptr [ecx]
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSBundleToggle_Write_Write()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 34H]
		push    TSSGCtrl_Write
		mov     ecx, dword ptr [ecx]
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSCopy_Write_OneRead()
{
	__asm
	{
		push    TSSGCtrl_OneRead
#if !SUBJECT_STRING_TABLE
		mov     ecx, dword ptr [ebx + 98H]
#else
		lea     ecx, [ebx + 98H]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax]
#endif
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSCopy_Write_OneWrite()
{
	__asm
	{
		push    TSSGCtrl_OneWrite
#if !SUBJECT_STRING_TABLE
		mov     ecx, dword ptr [ebx + 80H]
#else
		lea     ecx, [ebx + 80H]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax]
#endif
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Read_OneRead()
{
	__asm
	{
#if !IO_FEP_SUPPORT
		push    TSSGCtrl_OneRead
#else
		push    offset TSSGCtrl_OneRead_with_CheckIO_FEP
#endif
#if !SUBJECT_STRING_TABLE
		mov     ecx, dword ptr [edi + 98H]
#else
		lea     ecx, [edi + 98H]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax]
#endif
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Read_Compare1()
{
	__asm
	{
		push    TSSGCtrl_Compare
		lea     ecx, [ebp - 74H]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Read_Compare2()
{
	__asm
	{
		push    TSSGCtrl_Compare
		lea     ecx, [ebp - 0F4H]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Write_OneWrite()
{
	__asm
	{
#if !IO_FEP_SUPPORT
		push    TSSGCtrl_OneWrite
#else
		push    offset TSSGCtrl_OneWrite_with_CheckIO_FEP
#endif
#if !SUBJECT_STRING_TABLE
		mov     ecx, dword ptr [esi + 98H]
#else
		lea     ecx, [esi + 98H]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax]
#endif
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Write_Write1()
{
	__asm
	{
		push    TSSGCtrl_Write
		lea     ecx, [ebp - 74H]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleList_Write_Write2()
{
	__asm
	{
		push    TSSGCtrl_Write
		lea     ecx, [ebp - 0F4H]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Read_Compare1()
{
	__asm
	{
		push    TSSGCtrl_Compare
		lea     ecx, [ebp - 0ECH]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Read_Compare2()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 144H]
		push    TSSGCtrl_Compare
		mov     ecx, dword ptr [ecx]
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Write_Write1()
{
	__asm
	{
		push    TSSGCtrl_Write
		lea     ecx, [ebp - 0ECH]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Write_Write2()
{
	__asm
	{
		push    TSSGCtrl_Write
		lea     ecx, [ebp - 15CH]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSDoubleToggle_Write_Write3()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 1ACH]
		push    TSSGCtrl_Write
		mov     ecx, dword ptr [ecx]
		jmp     CheckAddress
	}
}

__declspec(naked) void __cdecl TSSBundleFloatCalc_Write_OneWrite1()
{
	__asm
	{
		push    TSSGCtrl_OneWrite
		lea     ecx, [ebp - 7CH]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) void __cdecl TSSBundleFloatCalc_Write_OneWrite2()
{
	__asm
	{
		push    TSSGCtrl_OneWrite
		lea     ecx, [ebp - 4CH]
		jmp     CheckAddressWithStringDtor
	}
}

__declspec(naked) static void __fastcall CheckAddress(LPCSTR p)
{
	__asm
	{
		call    TSSGCtrl_IsRemoteProcess
		test    al, al
		jnz     L1
		call    GetCurrentProcess
		mov     dword ptr [esp + 16], eax
	L1:
		rep ret
	}
}

__declspec(naked) static void __fastcall CheckAddressWithStringDtor(string *s)
{
	__asm
	{
		push    ecx
		mov     ecx, dword ptr [ecx]
		call    TSSGCtrl_IsRemoteProcess
		test    al, al
		jnz     L1
		call    GetCurrentProcess
		mov     dword ptr [esp + 20], eax
	L1:
		pop     ecx
		jmp     string_dtor
	}
}
