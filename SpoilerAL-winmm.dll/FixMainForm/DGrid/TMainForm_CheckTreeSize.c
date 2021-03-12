#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TApplication.h"
#include "TBitmap.h"
#include "TCanvas.h"
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"
#include "TSSGSubject.h"
#include "TSSGCtrl.h"
#include "TSSGScriptStruct.h"
#include "TSSSplit.h"
#include "WaitCursor.h"
#include "SSGSubjectProperty.h"

void __cdecl TMainForm_CheckTreeSize(TMainForm *this, BOOLEAN AllWidthCheck)
{
	static BOOL  InProcessing = FALSE;
	HWND         DGridHandle;
	LPVOID       BSCanvas;
	SIZE         TextSize;
	unsigned int RowHeight;
	unsigned int SubjectCount;
	unsigned int SubjectWidth;
	unsigned int SubjectHeight;
	unsigned int cx;
	unsigned int cy;
	unsigned int uShow, uHide;
	unsigned int FillHeight;

	#define lpszDagger (LPCSTR)0x0060313B

	// cancel recursive call
	if (InProcessing)
		return;
	InProcessing = TRUE;

	// set wait cursor
	BeginWaitCursor();

	// suspend redraw
	DGridHandle = TWinControl_GetHandle(this->DGrid);
	SendMessageA(DGridHandle, WM_SETREDRAW, FALSE, 0);

	// 文字サイズ基準の調整
	BSCanvas = TBitmap_GetCanvas(this->treeBackScreen);
	GetTextExtentPoint32A(TCanvas_GetHandle(BSCanvas), lpszDagger, 2, &TextSize);
	this->treeFontWidth = TextSize.cx;
	RowHeight = TextSize.cy + 8;
	TDrawGrid_SetDefaultRowHeight(this->DGrid, RowHeight);

	// 横方向のサイズ調整
	SubjectCount = vector_size(&this->treeSubjectVec) + 1;
	if (SubjectCount + 1 != (unsigned int)this->DGrid->FRowCount)
		TDrawGrid_SetRowCount(this->DGrid, SubjectCount + 1);

	// calculate max width from all rows
	SubjectWidth = TitleWidth;
	for (TSSGSubject **it = vector_begin(&this->treeSubjectVec); it != vector_end(&this->treeSubjectVec); it++)
	{
		if (SubjectWidth < (*it)->breadth)
			SubjectWidth = (*it)->breadth;
	}

	// calculate height of all rows
	SubjectHeight = SubjectCount * RowHeight;

	// calculate client area without scrollbar
	cx = this->DGrid->FWidth;
	cy = this->DGrid->FHeight;
	cx -= (unsigned int)GetSystemMetrics(SM_CXEDGE) * 2;
	cy -= (unsigned int)GetSystemMetrics(SM_CYEDGE) * 2;

	// subtract scrollbar size
	if (cx >= SubjectWidth)
	{
		if (cy < SubjectHeight)
		{
			if ((int)(cx -= GetSystemMetrics(SM_CXVSCROLL)) < 0)
				cx = 0;
			if (cx < SubjectWidth)
				goto SUBTRACT_CYHSCROLL;
		}
		uShow = 0;
	}
	else
	{
	SUBTRACT_CYHSCROLL:
		cx = SubjectWidth;
		if ((int)(cy -= GetSystemMetrics(SM_CYHSCROLL)) < 0)
			cy = 0;
		uShow = SB_HORZ + 1;
	}
	TDrawGrid_SetDefaultColWidth(this->DGrid, cx);

	// filling free space by last row
	if ((int)(FillHeight = cy - SubjectHeight) >= 0)
	{
		TDrawGrid_SetTopRow(this->DGrid, 0);
	}
	else
	{
		FillHeight = RowHeight + cy % RowHeight;
		uShow |= SB_VERT + 1;
	}
	TDrawGrid_SetRowHeights(this->DGrid, SubjectCount, FillHeight);

	assert(SB_HORZ + 1 == 1);
	assert(SB_VERT + 1 == 2);
	assert(SB_BOTH     == 3);

	// update scroll bar
	uHide = ~uShow;
	if (uHide &= SB_BOTH)
		ShowScrollBar(DGridHandle, uHide - (uHide != SB_BOTH), FALSE);
	if (uShow)
		ShowScrollBar(DGridHandle, uShow - (uShow != SB_BOTH), TRUE);

	// resume redraw
	SendMessageA(DGridHandle, WM_SETREDRAW, TRUE, 0);
	InvalidateRect(DGridHandle, NULL, FALSE);

	// restore cursor
	EndWaitCursor();

	// restore static flag
	InProcessing = FALSE;
}
