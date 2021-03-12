#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

__declspec(naked) void __cdecl Caller_TMainForm_DrawTree(TMainForm *this, LPVOID DestCanvas, long LeftOffset, long TopOffset)
{
	__asm
	{
		#define this       (esp +  4)
		#define DestCanvas (esp +  8)
		#define LeftOffset (esp + 12)
		#define TopOffset  (esp + 16)

#if DRAWTREE_CONVENTION
		mov     eax, dword ptr [TopOffset      ]
		pop     edx
		push    FALSE
		push    eax
		mov     ecx, dword ptr [LeftOffset +  4]
		mov     eax, dword ptr [DestCanvas +  4]
		push    ecx
		push    eax
		mov     ecx, dword ptr [this       + 12]
		push    ecx
		push    edx
#endif
		jmp     TMainForm_DrawTree

		#undef this
		#undef DestCanvas
		#undef LeftOffset
		#undef TopOffset
	}
}

void
#if DRAWTREE_CONVENTION
__stdcall
#endif
TMainForm_DrawTree(TMainForm *this, LPVOID DestCanvas, long LeftOffset, long TopOffset
#if DRAWTREE_CONVENTION
								  , BOOL IgnoreDebugString
#endif
)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        span;
	int        i, end;
#if !DRAWTREE_CONVENTION
	BOOL       IgnoreDebugString = TopOffset < 0;
#endif

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
	{
		int pos;

		pos = si.nPos - si.nMin;
		if (pos)
		{
			int range;

			range = si.nMax - si.nMin;
			if (range)
			{
				LeftOffset -= MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
			}
		}
	}

	span = this->DGrid->FDefaultRowHeight;
	rect.left = LeftOffset;
	rect.right = this->DGrid->FDefaultColWidth + LeftOffset;
	if (DestCanvas)
	{
		rect.top = TopOffset & LONG_MAX;
	}
	else
	{
		rect.top = 0;
		DestCanvas = this->DGrid->FCanvas;
	}
	rect.bottom = rect.top + span;

	i = this->DGrid->FTopLeft.Y;
	end = clientHeight / this->DGrid->FDefaultRowHeight;
	if (clientHeight % this->DGrid->FDefaultRowHeight)
		end++;
	end += i;
	if (end > this->DGrid->FRowCount)
		end = this->DGrid->FRowCount;
	if (IgnoreDebugString)
		this->ssgCtrl.ssgActionListner = NULL;// quiet mode
	for (end--; i < end; i++)
	{
		TMainForm_DrawTreeCell(this, DestCanvas, i, &rect);
		rect.top += span;
		rect.bottom += span;
	}
	rect.bottom = rect.top + TDrawGrid_GetRowHeights(this->DGrid, end);
	TMainForm_DrawTreeCell(this, DestCanvas, end, &rect);
	if (IgnoreDebugString)
		this->ssgCtrl.ssgActionListner = &this->ssgActionListner;
}

void __fastcall TMainForm_LockCBoxClick_CellRectAfter(TMainForm *this, RECT *R)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
	{
		int pos;

		pos = si.nPos - si.nMin;
		if (pos)
		{
			int range;

			range = si.nMax - si.nMin;
			if (range)
			{
				R->left -= MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
			}
		}
	}
}

__declspec(naked) void __cdecl TMainForm_LockCBoxClick_CellRectStub()
{
	__asm {
		mov edx, esi
		mov ecx, ebx
		jmp TMainForm_LockCBoxClick_CellRectAfter
	}
}
