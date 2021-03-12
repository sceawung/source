#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGSubject.h"

vector* __stdcall TSSGCtrl_IsEnabled_GetAttribute(va_list register fp)
{
	LPCVOID      const ReturnAddress = (va_arg(fp, LPCVOID), va_arg(fp, LPCVOID));
	TSSGSubject *const SSGS = (va_arg(fp, LPVOID), va_arg(fp, LPVOID));
	switch ((uintptr_t)ReturnAddress)
	{
	case 0x004B90B1u:// TSSBitList::Read
	case 0x004BD6F2u:// TSSBundleCalc::Read
	case 0x004BF3D0u:// TSSBundleList::Read
	case 0x004C01BDu:// TSSBundleToggle::Read
	case 0x004C1E6Du:// TSSCalc::Read
	case 0x004C41D3u:// TSSDoubleList::Read
	case 0x004C862Bu:// TSSDoubleToggle::Read
	case 0x004CE381u:// TSSFloatCalc::Read
	case 0x0052A0D1u:// TSSList::Read
	case 0x0052AF69u:// TSSString::Read
	case 0x0052C47Fu:// TSSToggle::Read
	case 0x0052DD26u:// TSSBundleFloatCalc::Read
		SSGS->evaluateAtRead = TRUE;
		break;
	default:
		SSGS->evaluateAtRead = FALSE;
		break;
	}
	return SSGS->attribute;
}
