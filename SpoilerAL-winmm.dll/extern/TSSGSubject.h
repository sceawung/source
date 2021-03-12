#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "SubjectStringTable.h"

typedef enum Result
{
	reNO_ERROR      = 0,
	reACCESS_ERROR     ,
	rePOINTER_ERROR    ,
	reOPTION_ERROR     ,
	reINDEX_ERROR      ,
	reNOT_ENABLED      ,
	reOPEN_ERROR       ,
} ResErr;

typedef enum Subject
{
	stNONE         = 0,
	stDIR             ,
	stCALC            ,
	stTOGGLE          ,
	stLIST            ,
	stSTRING          ,
	stBITLIST         ,
	stB_TOGGLE        ,
	stB_CALC          ,
	stB_LIST          ,
	stD_LIST          ,
	stTRACE           ,
	stD_TOGGLE        ,
	stCOPY            ,
	stFLOAT_CALC      ,
	stB_FLOAT_CALC    ,
	stSPLIT           ,
} SubType;

typedef enum Status
{
	ssOPEN        = 1,
	ssLOCK        = 2,
	ssCAN_UNKNOWN = 4,
} SubState;

typedef struct TSSGSubjectVtbl SubjectVtbl;

#pragma pack(push, 1)
typedef struct _TSSGSubject
{
	SubjectVtbl     *VTable;
	union {
		struct {
			bool    isSeted;
			BYTE    type;
			WORD    breadth;
		};
		struct {
			unsigned      : 8;
			SubType stype : 8;
		};// for debugger
	};
	bcb6_std_vector *attribute;
	union
	{
		struct
		{
			BYTE    status;
			bool    evaluateAtRead;
			WORD    stable;
		};
		SubState    states : 8;
		struct
		{
			bool    isOpen       : 1;
			bool    isLocked     : 1;
			bool    isCanUnknown : 1;
			bool    isAdjustment : 1;
			bool    withFEP      : 1;
		};
	};
	LPCVOID         lastAddr;
	union {
		struct {
			bcb6_std_string name;
			bcb6_std_string code;
		};
		struct {
			__int32 : 32;
			bcb6_std_map    fields;
			struct _TSSDir *folder;
		};
	};
	bcb6_std_string subjectName;
	ptrdiff_t       propertyIndex;
} TSSGSubject;
#pragma pack(pop)

#define typename PTSSGSubject
#define type     TSSGSubject *
#include "bcb6_std_vector_template.h"
#include "TSSGCtrl.h"

struct TSSGSubjectVtbl
{
	void(__cdecl *const dtor)(void *this, enum flagsClean);
	void(__cdecl *const Setting)(void *this, TSSGCtrl *SSGC);
	unsigned long(__cdecl *const Read)(void *this, TSSGCtrl *SSGC, void *Arg);
	unsigned long(__cdecl *const Write)(void *this, TSSGCtrl *SSGC, void *Arg);
	bcb6_std_string *(__cdecl *const ToString)(bcb6_std_string *, void *this, TSSGCtrl *SSGC);
	bcb6_std_string *(__cdecl *const ToByteCode)(bcb6_std_string *, void *this, TSSGCtrl *SSGC, void *Val);
	bcb6_std_string *(__cdecl *const GetLockName)(bcb6_std_string *, void *this);
	unsigned long(__cdecl *const GetSize)(void *this);
	bcb6_std_string *(__cdecl *const GetAddressStr)(bcb6_std_string *, void *this);
	unsigned long(__cdecl *const GetArgType)(void *this);
	bool(__cdecl *const IsSameSubject)(void *this, void *SSGS);

	bcb6_std_string *(__cdecl *const GetIndexFileName)(bcb6_std_string *, void *this);
};

EXTERN_C void __fastcall delete_TSSGSubject(TSSGSubject *SSGS);

EXTERN_C long __stdcall TSSGSubject_GetSubjectNameTextWidth(TSSGSubject *this, HDC hDC, TSSGCtrl *SSGCtrl);

EXTERN_C void(__cdecl * const TSSGSubject_GetSubjectName)(bcb6_std_string *Result, TSSGSubject *this, TSSGCtrl *SSGC);

__inline void TSSGSubject_SetCode_stdstr(TSSGSubject *this, const bcb6_std_string *Val)
{
	this->code.sstIndex = SubjectStringTable_insert(Val);
}
__inline void TSSGSubject_SetName_stdstr(TSSGSubject *this, const bcb6_std_string *Val)
{
	this->name.sstIndex = SubjectStringTable_insert(Val);
}
__inline void TSSGSubject_SetSubjectName_stdstr(TSSGSubject *this, const bcb6_std_string *Val)
{
	this->subjectName.sstIndex = SubjectStringTable_insert(Val);
}
__inline void TSSGSubject_SetCode(TSSGSubject *this, const char *Val)
{
	LPCSTR s[] = { Val, Val + strlen(Val) };
	this->code.sstIndex = SubjectStringTable_insert((bcb6_std_string *)s);
}
__inline void TSSGSubject_SetName(TSSGSubject *this, const char *Val)
{
	LPCSTR s[] = { Val, Val + strlen(Val) };
	this->name.sstIndex = SubjectStringTable_insert((bcb6_std_string *)s);
}
__inline void TSSGSubject_SetSubjectName(TSSGSubject *this, const char *Val)
{
	LPCSTR s[] = { Val, Val + strlen(Val) };
	this->subjectName.sstIndex = SubjectStringTable_insert((bcb6_std_string *)s);
}
__inline void TSSGSubject_SetCanUnknown(TSSGSubject *this, bool CanUnknown)
{
	this->isCanUnknown = !!CanUnknown;
}

#define _TSSGSubject_VTable             0x00617C20
#define _TSSSplit_VTable                0x0063FED8
#define _TSSBundleFloatCalc_VTable      0x0063FF10
#define _TSSFloatCalc_VTable            0x0063FF48
#define _TSSCopy_VTable                 0x0063FF80
#define _TSSTrace_VTable                0x0063FFB8
#define _TSSBitList_VTable              0x0063FFF0
#define _TSSArgBoolVectorSubject_VTable 0x0064002C
#define _TSSDoubleToggle_VTable         0x00640068
#define _TSSBundleCalc_VTable           0x006400A4
#define _TSSBundleToggle_VTable         0x00640118
#define _TSSDoubleList_VTable           0x0064018C
#define _TSSList_VTable                 0x006401C8
#define _TSSString_VTable               0x00640204
#define _TSSToggle_VTable               0x0064023C
#define _TSSCalc_VTable                 0x00640274
#define _TSSDir_VTable                  0x006403C0
#define TSSGSubject_VTable              ((void *)_TSSGSubject_VTable            )
#define TSSSplit_VTable                 ((void *)_TSSSplit_VTable               )
#define TSSBundleFloatCalc_VTable       ((void *)_TSSBundleFloatCalc_VTable     )
#define TSSFloatCalc_VTable             ((void *)_TSSFloatCalc_VTable           )
#define TSSCopy_VTable                  ((void *)_TSSCopy_VTable                )
#define TSSTrace_VTable                 ((void *)_TSSTrace_VTable               )
#define TSSBitList_VTable               ((void *)_TSSBitList_VTable             )
#define TSSArgBoolVectorSubject_VTable  ((void *)_TSSArgBoolVectorSubject_VTable)
#define TSSDoubleToggle_VTable          ((void *)_TSSDoubleToggle_VTable        )
#define TSSBundleCalc_VTable            ((void *)_TSSBundleCalc_VTable          )
#define TSSBundleToggle_VTable          ((void *)_TSSBundleToggle_VTable        )
#define TSSDoubleList_VTable            ((void *)_TSSDoubleList_VTable          )
#define TSSList_VTable                  ((void *)_TSSList_VTable                )
#define TSSString_VTable                ((void *)_TSSString_VTable              )
#define TSSToggle_VTable                ((void *)_TSSToggle_VTable              )
#define TSSCalc_VTable                  ((void *)_TSSCalc_VTable                )
#define TSSDir_VTable                   ((void *)_TSSDir_VTable                 )


#define TSSGSubject_GetAttribute(/*IN TSSGSubject * */this) \
	((this)->attribute)
#define TSSGSubject_SetAttribute(/*IN TSSGSubject * */this, /* bcb6_std_vector * */Val) \
	((Val) && ((Val)->allocator_type[0] = NULL), (this)->attribute = Val)
// virtual
#define TSSGSubject_Setting(this, SSGC) \
	(this)->VTable->Setting(this, SSGC)
#define TSSGSubject_Read(this, SSGC, Arg) \
	(this)->VTable->Read(this, SSGC, Arg)
#define TSSGSubject_Write(this, SSGC, Arg) \
	(this)->VTable->Write(this, SSGC, Arg)
#define TSSGSubject_ToString(Result, this, SSGC) \
	(this)->VTable->ToString(Result, this, SSGC)
#define TSSGSubject_ToByteCode(Result, this, SSGC, Val) \
	(this)->VTable->ToByteCode(Result, this, SSGC, Val)
#define TSSGSubject_GetLockName(Result, this) \
	(this)->VTable->GetLockName(Result, this)
#define TSSGSubject_GetSize(this) \
	(this)->VTable->GetSize(this)
#define TSSGSubject_GetAddressStr(Result, this) \
	(this)->VTable->GetAddressStr(Result, this)
#define TSSGSubject_GetArgType(this) \
	(this)->VTable->GetArgType(this)
#define TSSGSubject_IsSameSubject(this, SSGS) \
	(this)->VTable->IsSameSubject(this, SSGS)
#define TSSArgLongIndexSubject_GetIndexFileName(Result, this) \
	(this)->VTable->GetIndexFileName(Result, this)
#define TSSArgBoolVectorSubject_GetIndexFileName TSSArgLongIndexSubject_GetIndexFileName

#pragma pack(push, 1)
typedef struct _TSSBitList
{
	TSSGSubject     super;
	bcb6_std_string lockStr;
	BYTE            size;
	BYTE            padBitList[7];
	bcb6_std_string addressStr;
	bcb6_std_string indexFileName;
	bcb6_std_string realFileName;
	unsigned long   maskByte;
} TSSBitList;
#pragma pack(pop)
