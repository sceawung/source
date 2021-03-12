#pragma once

#include "TWinControl.h"

#define DEFINE_TBUTTONCONTROL                   \
    DEFINE_TWINCONTROL;                         \
    bool          FClicksDisabled;              \
    __int8 : 8;                                 \
    __int16 : 16;                               \
    __int32 : 32

#define DEFINE_TBUTTON                          \
    DEFINE_TBUTTONCONTROL;                      \
    bool FDefault;                              \
    bool FCancel;                               \
    bool FActive;                               \
    __int8 : 8;                                 \
	TModalResult FModalResult

#pragma pack(push, 1)
typedef struct Button
{
	DEFINE_TBUTTON;
} TButton;
#pragma pack(pop)

typedef char TAlignment, TCheckBoxState;

#define DEFINE_TCUSTOMCHECKBOX                  \
    DEFINE_TBUTTONCONTROL;                      \
    TAlignment     FAlignment;                  \
    bool           FAllowGrayed;                \
    TCheckBoxState FState;                      \
    __int8 : 8;                                 \
    __int32 : 32

#define DEFINE_TCHECKBOX                        \
    DEFINE_TCUSTOMCHECKBOX

#pragma pack(push, 1)
typedef struct _TCheckBox
{
	DEFINE_TCHECKBOX;
} TCheckBox;
#pragma pack(pop)

EXTERN_C void __fastcall TCheckBox_SetChecked(LPVOID this, BOOLEAN Value);
EXTERN_C BOOLEAN __fastcall TCheckBox_GetChecked(LPVOID this);
