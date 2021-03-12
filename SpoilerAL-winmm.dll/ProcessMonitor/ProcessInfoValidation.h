#ifndef _PROCESSCONTAINSMODULE_H_
#define _PROCESSCONTAINSMODULE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>
#include <regex.h>
#include "ToolTip/ToolTip.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall ProcessInfoValidation(
	IN DWORD   dwProcessId,
	IN BOOL    bIsRegex,
	IN LPCVOID lpModuleName,
	IN LPCVOID lpCmdLineArg);

#if USE_TOOLTIP
static __forceinline int regcompTip(
	regex_t    *_Restrict_ const preg,
	const char *_Restrict_ const pattern,
	int                    const cflags)
{
	extern BOOL bActive;

	char lpTitle[0x0100];
	int const code = regcomp(preg, pattern, cflags);
	if (!bActive && code && regerror(code, preg, lpTitle, sizeof(lpTitle)))
		ShowToolTip(pattern, (HICON)lpTitle);
	return code;
}
#define regcomp regcompTip
#endif

#ifdef __cplusplus
}
#endif

#endif	// _PROCESSCONTAINSMODULE_H_
