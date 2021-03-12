#include <windows.h>
#include "TMainForm.h"
#include "TWinControl.h"

void __stdcall TMainForm_OnShowWindow(HWND hWnd, BOOL bShow, UINT nStatus)
{
	TMainForm *lpForm;
	int       nCmdShow;

	lpForm = MainForm;
	nCmdShow = bShow ? SW_SHOWNA : SW_HIDE;
	if (lpForm->findNameForm)
		ShowWindow(TWinControl_GetHandle(lpForm->findNameForm), nCmdShow);
	if (lpForm->searchForm && ((TWinControl *)lpForm->searchForm)->FVisible)
		ShowWindow(TWinControl_GetHandle(lpForm->searchForm), nCmdShow);
	if (lpForm->guideForm)
		ShowWindow(TWinControl_GetHandle(lpForm->guideForm), nCmdShow);
}
