#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TCanvas.h"
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"
#include "TSSGSubject.h"
#include "SSGSubjectProperty.h"

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_FixDefaultColWidth()
{
	static void __stdcall FixDefaultColWidth(TSSGSubject *SSGS, LPVOID Canvas, int X, const string *Text);

	__asm
	{
		#define TCanvas_InternalTextOut 0055E418H
		#define Canvas                  eax
		#define X                       edx
		#define Y                       ecx
		#define SSGS                    edi
		#define DrawStr                 (ebp - 18H)

		push    TCanvas_InternalTextOut
		push    ecx
		mov     ecx, ebp
		push    eax
		sub     ecx, 24
		push    edx

		push    ecx
		push    X
		push    Canvas
		push    SSGS
		call    FixDefaultColWidth

		pop     edx
		pop     eax
		pop     ecx
		ret

		#undef TCanvas_InternalTextOut
		#undef Canvas
		#undef X
		#undef Y
		#undef SSGS
		#undef DrawStr
	}
}

static void __stdcall FixDefaultColWidth(TSSGSubject *SSGS, LPVOID Canvas, int X, const string *Text)
{
	SIZE      size;
	TMainForm *this;

	GetTextExtentPoint32A(TCanvas_GetHandle(Canvas), string_c_str(Text), string_length(Text), &size);
	size.cx += X + 6;
	if (SSGS)
	{
		assert(size.cx >= 0 && size.cx <= MAXWORD);
		SSGS->breadth = ( WORD)size.cx;
	}
	else
	{
		TitleWidth = size.cx;
	}
	this = MainForm;
	if (this->DGrid->FDefaultColWidth < size.cx)
	{
		HWND         DGridHandle;
		long         lStyle;
		unsigned int ClientHeight;
		unsigned int SubjectCount;
		unsigned int SubjectHeight;
		unsigned int FillHeight;
		int          wBar;

		DGridHandle = TWinControl_GetHandle(this->DGrid);
		lStyle = GetWindowLongA(DGridHandle, GWL_STYLE);
		TDrawGrid_SetDefaultColWidth(this->DGrid, size.cx);
		ClientHeight = TDrawGrid_GetClientHeight(this->DGrid);
		SubjectCount = this->DGrid->FRowCount - 1;
		SubjectHeight = this->DGrid->FDefaultRowHeight * SubjectCount;
		if (ClientHeight >= SubjectHeight)
		{
			FillHeight = ClientHeight - SubjectHeight;
			wBar = SB_HORZ;
		}
		else
		{
			FillHeight = this->DGrid->FDefaultRowHeight + ClientHeight % this->DGrid->FDefaultRowHeight;
			wBar = SB_BOTH;
		}
		TDrawGrid_SetRowHeights(this->DGrid, SubjectCount, FillHeight);
		ShowScrollBar(DGridHandle, wBar, TRUE);
		if (!(lStyle & WS_HSCROLL))
		{
			SCROLLINFO si;

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE;
			si.nMin = 0;
			si.nMax = this->DGrid->FDefaultColWidth - TDrawGrid_GetClientWidth(this->DGrid);
			SetScrollInfo(DGridHandle, SB_HORZ, &si, FALSE);
		}
	}
}
