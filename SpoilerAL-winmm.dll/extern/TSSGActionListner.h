#pragma once

#include <windows.h>
#include "TSSGSubject.h"
#include "bcb6_std_string.h"

EXTERN_C void __stdcall TSSGActionListner_OnParsingProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, long TopVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingDoubleProcess61(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code, double TopVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingProcess(LPCSTR code, size_t codeLength, unsigned __int64 topVal);
EXTERN_C void __stdcall TSSGActionListner_OnParsingError(LPVOID ssgActionListner, TSSGSubject *SSGS, LPCSTR Code);
EXTERN_C void __stdcall TSSGActionListner_OnParsingDoubleProcess(LPCSTR code, size_t codeLength, double topVal);

EXTERN_C void(__cdecl *TSSGActionListner_OnProcessOpen)(LPVOID _this, TSSGSubject *SSGS, DWORD Mode);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectDisabled)(LPVOID _this, bcb6_std_string Code);
EXTERN_C void(__cdecl *TSSGActionListner_OnProcessOpenError)(LPVOID _this, TSSGSubject *SSGS);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectReadError)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectWriteError)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectReadSuccess)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address);
EXTERN_C void(__cdecl *TSSGActionListner_OnSubjectWriteSuccess)(LPVOID _this, TSSGSubject *SSGS, unsigned long Address);
