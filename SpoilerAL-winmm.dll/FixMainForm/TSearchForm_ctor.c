#include <windows.h>
#include "TWinControl.h"
#include "SnapWindow.h"

#pragma warning(disable:4733)

static void __fastcall ctor(LPVOID this);

__declspec(naked) void __cdecl TSearchForm_ctor()
{
	__asm
	{
		#define this (ebp - 4H)

		mov     ecx, dword ptr [this]
		call    ctor
		mov     eax, dword ptr [ebp - 40H]
		mov     dword ptr fs:[0], eax
		ret

		#undef this
	}
}

static void __fastcall ctor(LPVOID this)
{
	HWND hWnd;

	hWnd = TWinControl_GetHandle(this);
	SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);

#if SNAPWINDOW
	AttachSnapWindow(hWnd);
#endif
}
