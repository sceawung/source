#pragma once

#include <windows.h>
#include "bcb6_std_stack.h"
#include "bcb6_std_vector_string.h"
#include "TStringDivision.h"
#include "TProcessCtrl.h"
#include "TSSGAttributeElement.h"
#include "TSSGAttributeSelector.h"
#include "TSSGScriptStruct.h"

enum Repeat
{
	rtSTRING         = 0,
	rtNUMBER         = 1,
	rtREVERSE_ENDIAN = 2,
	rtFILE           = 4,
	rtDECIMAL        = 8,
	rtWORD_REPEAT    = 16,
	rtFORMAT         = 0x20,
};

#pragma pack(push, 1)
typedef struct _TSSGCtrl
{
	struct _TFunctionTimer {
		unsigned long (*nowTimerFunc)(unsigned long);
		MMRESULT timerID;
		TIMECAPS devCaps;
		unsigned long oldTime, nowTime, interval;
	} funcTimer;
	__int32 : 32;
	TSSGAttributeSelector  attributeSelector;
	LPVOID                 adjustmentListner;
	LPVOID                 ssgActionListner;
	unsigned long          lockTimerInterval;
	__int32 : 32;
	bcb6_std_string        ssgDir;
	bcb6_std_string        scriptDir;
	bcb6_std_string        exeDir;
	bool                   canLockWrite;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
	bcb6_std_map           lockMap;
	bcb6_std_map           operatorMap;
	bcb6_std_map           stringOperatorMap;
	TSSGScriptStruct       script;
	bcb6_std_vector_string memo;
	bcb6_std_vector_string processNameVec;
	bool                   isMemoWordWrap;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
	bcb6_std_map           dataFileMap;
	struct _TSSDir         *rootSubject;
	__int32 : 32;
	TProcessCtrl           processCtrl;
	TStringDivision        strD;
} TSSGCtrl;
#pragma pack(pop)

#include "TSSDir.h"

#define TSSGCtrl_GetAttributeSelector(/*IN TSSGCtrl * */this) \
	(&(this)->attributeSelector)

#define TSSGCtrl_GetSSGActionListner(/*IN TSSGCtrl * */this) \
	(this)->ssgActionListner

EXTERN_C void(__cdecl * const TSSGCtrl_LockClear)(TSSGCtrl *this);
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSG(
	TSSGCtrl *this, bcb6_std_vector_string *SSGFile, bcb6_std_stack *ParentStack, TDialogAdjustmentAttribute *ADJElem, DWORD RepeatIndex, DWORD OuterRepeat);
EXTERN_C bool(__cdecl * const TSSGCtrl_IsEnabled)(TSSGCtrl *this, TSSGSubject *SSGS);
EXTERN_C unsigned long __cdecl TSSGCtrl_MakeSubjectType(const bcb6_std_string *Tag);
EXTERN_C TSSGSubject *(__cdecl * const TSSGCtrl_MakeSubjectClass)(const bcb6_std_string *Tag);
EXTERN_C LPVOID(__cdecl * const TSSGCtrl_MakeAdjustmentClass)(const bcb6_std_string *);
EXTERN_C TSSGSubject *(__cdecl * const TSSGCtrl_GetTargetSubject)(TSSGSubject *SSGS);
EXTERN_C void(__cdecl * const TSSGCtrl_SetSSGDataFile)(TSSGCtrl *this, bcb6_std_vector_string *Data, bcb6_std_string *FileName, bool IsSSL);
EXTERN_C bool(__cdecl * const TSSGCtrl_IsLocked)(TSSGCtrl *this, TSSGSubject *SSGS);
EXTERN_C bcb6_std_vector_string *(__cdecl * const TSSGCtrl_ReadSSRFile)(bcb6_std_vector_string *Result, TSSGCtrl *this, const bcb6_std_string *Code, bcb6_std_vector_dword *Indices, TSSGSubject *SSGS);
EXTERN_C void(__cdecl * const TSSGCtrl_LoopSSRFile)(TSSGCtrl *this, bcb6_std_vector *FormatVec, bcb6_std_vector *Dest, unsigned long LoopVal);
EXTERN_C LPVOID(__cdecl * const TSSGCtrl_GetAttribute)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Type);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_GetAddress)(TSSGCtrl *this, TSSGSubject *SSGS, const bcb6_std_string *AddressStr, unsigned long Mode);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_GetDirLevel)(TSSGCtrl *this, TSSGSubject *SSGS);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_AddressAttributeFilter)(TSSGCtrl *this, TSSGSubject *SSGS, DWORD *Address, unsigned long Mode);
EXTERN_C bool(__cdecl * const TSSGCtrl_CheckFunnel)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Val);
EXTERN_C bool(__cdecl * const TSSGCtrl_Funneling)(TSSGCtrl *this, TSSGSubject *SSGS, const bcb6_std_string *FileName, unsigned long Val);
EXTERN_C unsigned long(__cdecl * const TSSGCtrl_CheckIO_FEP)(TSSGCtrl *this, TSSGSubject *SSGS, unsigned long Val, bool IsInput);
EXTERN_C double(__cdecl * const TSSGCtrl_CheckIO_FEPDouble)(TSSGCtrl *this, TSSGSubject *SSGS, double Val, bool IsInput);

EXTERN_C BOOL __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

EXTERN_C bcb6_std_vector_string *(__cdecl * const TSSGCtrl_GetSSGDataFile)(TSSGCtrl *this, TSSGSubject *SSGS, bcb6_std_string FName, bcb6_std_string DefaultExt, bcb6_std_string *CurrentDir);

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_GetNameString(bcb6_std_string *Result, TSSGCtrl *this, TSSGSubject *SSGS, const bcb6_std_string *NameStr);

EXTERN_C bcb6_std_string *(__cdecl * const TSSGCtrl_GetSimpleByteCode)(bcb6_std_string *Result, TSSGCtrl *this, TSSGSubject *SSGS, bcb6_std_string CodeStr);
EXTERN_C bcb6_std_vector_dword *(__cdecl * const TSSGCtrl_StrToProcessAccessElementVec)(bcb6_std_vector_dword *CodeVec, TSSGCtrl *this, TSSGSubject *SSGS, bcb6_std_string Code, unsigned long Rel);
EXTERN_C bool(__cdecl * const TSSGCtrl_IsChildRWProhibit)(TSSGCtrl *this, TSSGSubject *SSGS);
HANDLE __cdecl TSSGCtrl_Open(TSSGCtrl *this, TSSGSubject *SSGS, DWORD Mode);
EXTERN_C bool(__cdecl * const TSSGCtrl_OneRead)(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);
EXTERN_C bool(__cdecl * const TSSGCtrl_OneWrite)(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size);

EXTERN_C void(__cdecl * const TSSGCtrl_SetLock)(TSSGCtrl *this, bool IsLock, TSSGSubject *Target, void *Value);
