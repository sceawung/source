#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TShiftState.h"
#include "TControl.h"
#include "TWinControl.h"
#include "TCheckBox.h"
#include "TMainForm.h"
#include "TSSDir.h"

void __fastcall TMainForm_HotKeyEditKeyDown(TMainForm *this, LPVOID Sender, WORD *Key, WORD Shift)
{
	LRESULT __fastcall TMainForm_NewVal_OnPaste(TMainForm *this);
	LRESULT __fastcall TMainForm_NewVal_OnBinaryPaste(TMainForm *this, BOOLEAN isBigEndian);

	static __inline void TMainForm_HotKeyEditKeyDown_PageUp(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_PageDown(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_End(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_Home(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_Up(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_Right(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_Down(TMainForm *this);
	static __inline void TMainForm_HotKeyEditKeyDown_Delete(TMainForm *this);

	BYTE   KeyState[256];
	BYTE   Ascii;
	HKL    KeyLayout;
	WORD   Char;
	long   Num;
	LPRECT lprc;

	if (Shift & (ssAlt | ssCtrl))
	{
		if (Shift & ssCtrl)
			if (*Key == 'V')
				if (!(Shift & (ssShift | ssAlt)))
					TMainForm_NewVal_OnPaste(this);
				else
					TMainForm_NewVal_OnBinaryPaste(this, (unsigned char)Shift & ssAlt);
			else if (*Key == 'A')
				if (!(Shift & ssAlt))
					SendMessageA(TWinControl_GetHandle(vector_at(&this->calcImage->valBox, 1).edit), EM_SETSEL, 0, ULONG_MAX);
		return;
	}
	if (!GetKeyboardState(KeyState))
		return;
	Ascii = 0;
	KeyLayout = GetKeyboardLayout(0);
	if (ToAsciiEx(*Key, MapVirtualKeyExA(*Key, 2, KeyLayout), KeyState, &Char, 0, KeyLayout) == 1)
		Ascii = *(BYTE *)&Char;
	switch (Ascii)
	{
	case ' ':
		if (this->LockCBox->FVisible)
			TCheckBox_SetChecked(this->LockCBox, !TCheckBox_GetChecked(this->LockCBox));
		return;
	case '*':
		// * (16/10)
		Num = 18;
		break;
	case '+':
		// + (max)
		Num = 21;
		break;
	case '-':
		// - (+/-)
		Num = 16;
		break;
	case '.':
		// .
		Num = 22;
		break;
	case '/':
		// / (min)
		Num = 20;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		// 0 - 9
		Num = Ascii - '0';
		break;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		// A - F
		Num = Ascii - ('A' - 0xA);
		break;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		Num = Ascii - ('a' - 0xA);
		break;
	default:
		switch (*Key)
		{
		case VK_BACK:
			// BackSpace
			Num = 17;
			break;
		case VK_RETURN:
			// Enter
			if (!this->selectSubject)
				return;
			switch (TSSGSubject_GetArgType(this->selectSubject))
			{
			case atLONG: case atDOUBLE: case atDIR:
				Num = 19;
				break;
			case atBOOL:
				TCheckBox_SetChecked(this->ToggleCBox, !TCheckBox_GetChecked(this->ToggleCBox));
				return;
			case atSTRING:
				TMainForm_StringEnterBtnClick(this, this->StringEnterBtn);
				return;
			default:
				return;
			}
			break;
		case VK_PRIOR:
			TMainForm_HotKeyEditKeyDown_PageUp(this);
			return;
		case VK_NEXT:
			TMainForm_HotKeyEditKeyDown_PageDown(this);
			return;
		case VK_END:
			TMainForm_HotKeyEditKeyDown_End(this);
			return;
		case VK_HOME:
			TMainForm_HotKeyEditKeyDown_Home(this);
			return;
		case VK_UP:
			TMainForm_HotKeyEditKeyDown_Up(this);
			return;
		case VK_RIGHT:
			TMainForm_HotKeyEditKeyDown_Right(this);
			return;
		case VK_DOWN:
			TMainForm_HotKeyEditKeyDown_Down(this);
			return;
		case VK_DELETE:
			TMainForm_HotKeyEditKeyDown_Delete(this);
			return;
		default:
			return;
		}
		break;
	}

	// 残りは、電卓系項目用のキー処理
	if (!this->selectSubject)
		return;
	switch (TSSGSubject_GetArgType(this->selectSubject))
	{
	case atLONG: case atDOUBLE:
		break;
	case atDIR:
		if (!vector_empty(&((TSSDir *)this->selectSubject)->childVec))
			break;
		/* FALLTHROUGH */
	default:
		return;
	}

	lprc = &vector_at(&this->calcImage->btnVec, Num).hitRect;
	TCalcImage_FOnMouseDown(this->calcImage, this->calcImage, 0, 0, lprc->left, lprc->top);
	Sleep(30);
	TCalcImage_FOnMouseUp(this->calcImage, this->calcImage, 0, 0, lprc->left, lprc->top);
}

static __inline void TMainForm_HotKeyEditKeyDown_PageUp(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        pos, range, topRow, row, clientRows, bottomRow;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	rect.left = 0;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
		if (pos = si.nPos - si.nMin)
			if (range = si.nMax - si.nMin)
				rect.left = -MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
	rect.right = rect.left + this->DGrid->FDefaultColWidth;
	if (this->invertGridRow < 0)
		this->invertGridRow = 0;
	topRow = this->DGrid->FTopLeft.Y >= 0 ? this->DGrid->FTopLeft.Y : 0;
	rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
	row = this->invertGridRow;
	clientRows = clientHeight / this->DGrid->FDefaultRowHeight;
	this->invertGridRow = this->invertGridRow > clientRows ? this->invertGridRow - clientRows : 0;
	TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, row, &rect);
	bottomRow = topRow - 1;
	if (bottomRow < clientRows)
		bottomRow = clientRows;
	if (this->invertGridRow > bottomRow)
		this->invertGridRow = bottomRow;
	rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow, &rect);
	if (topRow)
	{
		topRow = topRow > clientRows ? topRow - clientRows : 0;
		if (this->invertGridRow < topRow)
			this->invertGridRow = topRow;
		TDrawGrid_SetTopRow(this->DGrid, topRow);
	}
}

static __inline void TMainForm_HotKeyEditKeyDown_PageDown(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        pos, range, topRow, clientRows, bottomRow;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	rect.left = 0;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
		if (pos = si.nPos - si.nMin)
			if (range = si.nMax - si.nMin)
				rect.left = -MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
	rect.right = rect.left + this->DGrid->FDefaultColWidth;
	if (this->invertGridRow < 0)
		this->invertGridRow = 0;
	topRow = this->DGrid->FTopLeft.Y >= 0 ? this->DGrid->FTopLeft.Y : 0;
	clientRows = clientHeight / this->DGrid->FDefaultRowHeight;
	bottomRow = topRow + clientRows;
	if (this->invertGridRow < topRow)
	{
		this->invertGridRow = bottomRow;
		rect.top = clientRows * this->DGrid->FDefaultRowHeight;
	}
	else
	{
		int row;

		rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
		rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
		row = this->invertGridRow;
		if (this->invertGridRow < bottomRow)
		{
			int maxRow;

			this->invertGridRow += clientRows;
			maxRow = this->DGrid->FRowCount - 2;
			if (this->invertGridRow > maxRow)
				this->invertGridRow = maxRow;
		}
		else
		{
			this->invertGridRow = bottomRow + clientRows - 1;
		}
		TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, row, &rect);
		rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
	}
	rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow, &rect);
	if (topRow < (this->DGrid->FRowCount > clientRows ? this->DGrid->FRowCount - clientRows : 0))
	{
		int maxTopRow;

		topRow = bottomRow;
		maxTopRow = this->DGrid->FRowCount - clientRows;
		if (topRow > maxTopRow)
			topRow = maxTopRow;
		TDrawGrid_SetTopRow(this->DGrid, topRow);
	}
}

static __inline void TMainForm_HotKeyEditKeyDown_End(TMainForm *this)
{
	HWND DGridHandle;
	RECT rect;
	int  clientWidth, clientHeight;
	int  topRow, maxRow, clientRows;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	topRow = this->DGrid->FTopLeft.Y >= 0 ? this->DGrid->FTopLeft.Y : 0;
	maxRow = this->DGrid->FRowCount > 2 ? this->DGrid->FRowCount - 2 : 0;
	if (this->invertGridRow != maxRow)
	{
		RECT       rect;
		SCROLLINFO si;
		int        pos, range, row;

		rect.left = 0;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_POS;
		if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
			if (pos = si.nPos - si.nMin)
				if (range = si.nMax - si.nMin)
					rect.left = -MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
		rect.right = rect.left + this->DGrid->FDefaultColWidth;
		row = this->invertGridRow >= 0 ? this->invertGridRow : 0;
		this->invertGridRow = maxRow;
		rect.top = (row - topRow) * this->DGrid->FDefaultRowHeight;
		rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
		TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, row, &rect);
		rect.top = (maxRow - topRow) * this->DGrid->FDefaultRowHeight;
		rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
		TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, maxRow, &rect);
	}
	clientRows = clientHeight / this->DGrid->FDefaultRowHeight;
	TDrawGrid_SetTopRow(this->DGrid, this->DGrid->FRowCount > clientRows ? this->DGrid->FRowCount - clientRows : 0);
}

static __inline void TMainForm_HotKeyEditKeyDown_Home(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth;
	SCROLLINFO si;
	int        pos, range, row, topRow;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	rect.left = 0;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
		if (pos = si.nPos - si.nMin)
			if (range = si.nMax - si.nMin)
				rect.left = -MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
	rect.right = rect.left + this->DGrid->FDefaultColWidth;
	row = this->invertGridRow >= 0 ? this->invertGridRow : 0;
	this->invertGridRow = 0;
	topRow = this->DGrid->FTopLeft.Y >= 0 ? this->DGrid->FTopLeft.Y : 0;
	rect.top = (row - topRow) * this->DGrid->FDefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, row, &rect);
	rect.top = -topRow * this->DGrid->FDefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, 0, &rect);
	TDrawGrid_SetTopRow(this->DGrid, 0);
}

static __inline void TMainForm_HotKeyEditKeyDown_Up(TMainForm *this)
{
	int topRow;

	topRow = this->DGrid->FTopLeft.Y >= 0 ? this->DGrid->FTopLeft.Y : 0;
	if (this->invertGridRow)
	{
		HWND       DGridHandle;
		RECT       rect;
		int        clientWidth, clientHeight;
		SCROLLINFO si;
		int        pos, range;

		DGridHandle = TWinControl_GetHandle(this->DGrid);
		GetClientRect(DGridHandle, &rect);
		clientWidth = rect.right - rect.left;
		clientHeight = rect.bottom - rect.top;
		rect.left = 0;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_POS;
		if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
			if (pos = si.nPos - si.nMin)
				if (range = si.nMax - si.nMin)
					rect.left = -MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
		rect.right = rect.left + this->DGrid->FDefaultColWidth;
		if (this->invertGridRow >= 0)
		{
			int bottomRow;

			rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
			TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow--, &rect);
			bottomRow = clientHeight / this->DGrid->FDefaultRowHeight + topRow - 2;
			if (this->invertGridRow <= bottomRow)
			{
				rect.bottom = rect.top;
				rect.top -= this->DGrid->FDefaultRowHeight;
			}
			else
			{
				this->invertGridRow = bottomRow;
				rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
				rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
			}
		}
		else
		{
			this->invertGridRow = 0;
			rect.top = -topRow * this->DGrid->FDefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
		}
		TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow, &rect);
	}
	if (topRow)
	{
		topRow--;
		if (this->invertGridRow < topRow)
			this->invertGridRow = topRow;
		TDrawGrid_SetTopRow(this->DGrid, topRow);
	}
}

static __inline void TMainForm_HotKeyEditKeyDown_Right(TMainForm *this)
{
	HWND    hWnd;
	char    lpClassName[5 + 1 + 1];
	BOOLEAN tmpB;

	if (hWnd = GetFocus())
		if (GetClassNameA(hWnd, lpClassName, _countof(lpClassName)) == 5)
			if (*(LPDWORD)lpClassName == BSWAP32('TEdi') && *(LPWORD)&lpClassName[4] == BSWAP16('t\0'))
				return;
	TMainForm_DGridSelectCell(this, this->DGrid, 0, this->invertGridRow, &tmpB);
}

static __inline void TMainForm_HotKeyEditKeyDown_Down(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        pos, range, topRow, clientRows;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	rect.left = 0;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
		if (pos = si.nPos - si.nMin)
			if (range = si.nMax - si.nMin)
				rect.left = -MulDiv(this->DGrid->FDefaultColWidth - clientWidth, pos, range);
	rect.right = rect.left + this->DGrid->FDefaultColWidth;
	if (this->invertGridRow < 0)
		this->invertGridRow = 0;
	topRow = this->DGrid->FTopLeft.Y >= 0 ? this->DGrid->FTopLeft.Y : 0;
	do	/* do { ... } while (0); */
	{
		if (this->invertGridRow < topRow)
		{
			this->invertGridRow = topRow + 1;
			rect.top = this->DGrid->FDefaultRowHeight;
			rect.bottom = this->DGrid->FDefaultRowHeight * 2;
		}
		else
		{
			if (this->invertGridRow >= this->DGrid->FRowCount - 2)
				break;
			rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
			TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow++, &rect);
			rect.top = rect.bottom;
			rect.bottom += this->DGrid->FDefaultRowHeight;
		}
		TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow, &rect);
	} while (0);
	clientRows = clientHeight / this->DGrid->FDefaultRowHeight;
	if (topRow < this->DGrid->FRowCount - clientRows)
	{
		int bottomRow;

		bottomRow = topRow + clientRows;
		if (this->invertGridRow > bottomRow)
		{
			this->invertGridRow = bottomRow;
			rect.top = (this->invertGridRow - topRow) * this->DGrid->FDefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->FDefaultRowHeight;
			TMainForm_DrawTreeCell(this, this->DGrid->FCanvas, this->invertGridRow, &rect);
		}
		TDrawGrid_SetTopRow(this->DGrid, topRow + 1);
	}
}

static __inline void TMainForm_HotKeyEditKeyDown_Delete(TMainForm *this)
{
	HWND          edit;
	unsigned long start, end;

	edit = TWinControl_GetHandle(vector_at(&this->calcImage->valBox, 1).edit);
	SendMessageA(edit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
	if (start == GetWindowTextLength(edit))
	{
		MessageBeep(-1);
		return;
	}
	if (start == end)
		SendMessageA(edit, EM_SETSEL, start, start + 1);
	SendMessageA(edit, EM_REPLACESEL, FALSE, (WPARAM)"");
}
