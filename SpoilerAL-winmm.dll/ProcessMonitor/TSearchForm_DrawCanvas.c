#include <windows.h>
#include "TBrush.h"
#include "TCanvas.h"
#include "TProcessSearchReportListnerBase.h"

void __fastcall TSearchForm_DrawCanvas(TProcessSearchReportListnerBase *reportListner, long ImageWidth, unsigned long Pos, TCanvas *Canv)
{
	RECT rect  = { 0, 0, ImageWidth, 24 };
	long denom = (reportListner->max - reportListner->min) / ImageWidth;
	TBrush_SetColor(Canv->FBrush, 0x000000);
	if (Pos >= reportListner->start) {
		TCanvas_FillRect(Canv, &rect);
		rect.left = (reportListner->start - reportListner->min) / denom;
		rect.right = (Pos - reportListner->min) / denom;
	} else {
		rect.right = (reportListner->start - reportListner->min) / denom;
		TCanvas_FillRect(Canv, &rect);
		rect.left = (Pos - reportListner->min) / denom;
	}
	TBrush_SetColor(Canv->FBrush, 0x008000);
	TCanvas_FillRect(Canv, &rect);
}
