#include <windows.h>
#include "TSSGSubject.h"
#include "SSGSubjectProperty.h"

void __fastcall TSSGSubject_ctor(TSSGSubject *const this, DWORD const prop)
{// prop is generally given the same value as this
	this->type = stNONE;
	this->breadth = 0;
	this->evaluateAtRead = FALSE;
	this->stable = MAXWORD;
	this->lastAddr = NULL;
#pragma region Unnamed bit-field members are not initialized.
	this->fields._M_header = NULL;
	this->fields._M_node_count = 0;
#pragma endregion
	switch (prop)
	{
		extern TSSGSubject dummySSGS;
	default: prop > MAXWORD || !prop;
		this->folder = NULL;
		this->propertyIndex = MAXDWORD;
		break;
#pragma warning(suppress: 4101)
	case 1: TSSGSubject SSGS;// TSSGCtrl::LoopSSRFile
		this->folder = dummySSGS.folder;
		this->propertyIndex = dummySSGS.propertyIndex;
		break;
	}
}

#if 0
__declspec(naked) void __cdecl Caller_TSSGSubject_ctor1()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 0ACH]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor2()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 674H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor3()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 698H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor4()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 6BCH]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor5()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 4H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor6()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 8H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor7()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 0CH]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor8()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 18H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor9()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 24H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor10()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 28H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor11()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 2CH]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor12()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 30H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor13()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 34H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor14()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 40H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor15()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 44H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor16()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 48H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor17()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 4CH]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor18()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 50H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor19()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 54H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor20()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 58H]
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor21()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 4
		mov     ecx, edi
		jmp     TSSGSubject_ctor
	}
}

__declspec(naked) void __cdecl Caller_TSSGSubject_ctor22()
{
	__asm
	{
		add     dword ptr [ebx + 1CH], 4
		lea     ecx, [ebp - 60H]
		jmp     TSSGSubject_ctor
	}
}
#endif
