#ifndef _FINDWINDOWCONTAINSMODULE_H_
#define _FINDWINDOWCONTAINSMODULE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

HWND __stdcall FindWindowExceptInvalids(
	OUT OPTIONAL LPDWORD lpdwProcessId,
	IN           BOOL    bIsRegex,
	IN  OPTIONAL LPCSTR  lpClassName,
	IN  OPTIONAL LPCSTR  lpWindowName,
	IN  OPTIONAL LPCSTR  lpModuleName,
	IN  OPTIONAL LPCSTR  lpCmdLineArg);

#ifdef __cplusplus
}
#endif

#endif	// _FINDWINDOWCONTAINSMODULE_H_
