#include "HintWindow.h"
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TApplication.h"
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "commctrl.h"

static HWND      hToolTip = NULL;
static TOOLINFOA ti = { sizeof(TOOLINFOA), TTF_IDISHWND | TTF_SUBCLASS };
static HHOOK     hHook = NULL;
static DWORD     dwTrackPos;
static TDrawGrid *DrawGrid;

static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);

void __cdecl CreateHintWindow()
{
	if (hToolTip)
		return;
	hToolTip = CreateWindowExA(
		0,
		TOOLTIPS_CLASSA,
		NULL,
		TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		ti.hinst = GetModuleHandleA(NULL),
		NULL);
	if (!hToolTip)
		return;
    SendMessageA(hToolTip, TTM_SETMAXTIPWIDTH, 0, USHRT_MAX);
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM(2500, 0));
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_INITIAL, MAKELPARAM(0, 0));
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_RESHOW, MAKELPARAM(0, 0));
}

void __cdecl DestroyHintWindow()
{
	if (hHook)
	{
		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
	if (hToolTip)
	{
		DestroyWindow(hToolTip);
		hToolTip = NULL;
	}
	ti.uId = 0;
}

static BOOL __fastcall MouseToCell(LPDWORD lpdwPos)
{
	POINT pt;

	pt.x = ((LPPOINTS)lpdwPos)->x;
	pt.y = ((LPPOINTS)lpdwPos)->y;
	if (!ScreenToClient((HWND)ti.uId, &pt))
		return FALSE;
	TDrawGrid_MouseToCell(DrawGrid, pt.x, pt.y, &pt.x, &pt.y);
	((LPPOINTS)lpdwPos)->x = *(short *)&pt.x;
	((LPPOINTS)lpdwPos)->y = *(short *)&pt.y;
	return TRUE;
}

void __fastcall TApplication_ActivateHint(TApplication *this, LPPOINT CursorPos)
{
	HWND hWnd;
	char lpClassName[16];

	if (hHook || !this->FHintControl || !hToolTip)
		return;
	dwTrackPos = GetMessagePos();
	ti.lpszText = (LPSTR)this->FHintControl->FHint;
	if ((hWnd = TWinControl_GetHandle(this->FHintControl)) == (HWND)ti.uId)
	{
		SendMessageA(hToolTip, TTM_UPDATETIPTEXTA, 0, (LPARAM)&ti);
	}
	else
	{
		if (ti.uId)
			SendMessageA(hToolTip, TTM_DELTOOLA, 0, (LPARAM)&ti);
		ti.uId = (UINT_PTR)hWnd;
		SendMessageA(hToolTip, TTM_ADDTOOLA, 0, (LPARAM)&ti);
	}
	DrawGrid = NULL;
	if (GetClassNameA((HWND)ti.uId, lpClassName, _countof(lpClassName)) == 9 &&
		*(LPDWORD)&lpClassName[0] == BSWAP32('TDra') &&
		*(LPDWORD)&lpClassName[4] == BSWAP32('wGri') &&
		*(LPWORD )&lpClassName[8] == BSWAP16('d\0'))
	{
		DrawGrid = (TDrawGrid *)this->FHintControl;
		MouseToCell(&dwTrackPos);
	}
	hHook = SetWindowsHookExA(WH_CALLWNDPROCRET, CallWndRetProc, NULL, GetCurrentThreadId());
}

static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
	if (nCode >= 0)
	{
		DWORD dwPos;

		dwPos = GetMessagePos();
		if (DrawGrid && !MouseToCell(&dwPos) || dwPos != dwTrackPos || !IsWindowVisible((HWND)ti.uId))
		{
			UnhookWindowsHookEx(hHook);
			SendMessageA(hToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);
			hHook = NULL;
		}
	}
	return lResult;
}

