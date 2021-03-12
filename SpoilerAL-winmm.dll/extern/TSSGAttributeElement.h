#pragma once

#include <windows.h>
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TStringDivision.h"

typedef enum Attribute
{
//	atUNKNOWN,
	atDIR_LEVEL    = 1,
	atADJUST       = 2,
	atREPLACE      = 4,
	atSIZE         = 8,
//	atALL          = atDIR_LEVEL | atADJUST | atREPLACE | atSIZE,
	atFUNNEL       = 16,
	atIO_FEP       = 32,
	atE_WITH       = 64,
	atENABLED      = 128,
	atADJUST_CHECK = 256,
	atCHILD_RW     = 512,
	atCAUTION      = 1024,
	atPROLOGUE     = 0x0800,
	atDEFINE       = 0x1000,
	atON_ERROR     = 0x2000,
	atSCOPE        = 0x4000,
	atFORMAT       = 0x8000,
} AeType;// for debugger

#define atALL (atDIR_LEVEL | atADJUST | atREPLACE | atSIZE)

typedef enum Adjuster
{
	asADJUSTED  = 1,
	asCHECK_SUM = 2,
	asHEAP      = 4,
	asSEARCHING = 8,
} AdjState;

typedef enum Accuracy
{
	acHEAP   = 1,
	acCRC       ,
	acSEARCH    ,
	acDIALOG    ,
} AdjCheck;

EXTERN_C CONSOLE_FONT_INFO __fastcall TSSGAttributeElement_GetViaCoord(AeType Type, const bcb6_std_vector *AttrV);

typedef struct TSSGAttributeElementVtbl
{
	void(__cdecl *const dtor)(void *this, enum flagsClean);
	bool(__cdecl *const IsEqual)(void *this, const void *X);
	bool(__cdecl *const IsUpper)(void *this, const void *X);

	void(__cdecl *const Setting)(void *this, struct _TSSGCtrl *SSGCtrl, const char *Code);
	void(__cdecl *const SetAdjusted)(void *this, bool Flag, unsigned long Val);

	unsigned long(__cdecl *const Adjustment)(void *this, struct _TSSGCtrl *SSGCtrl);
} AeVtbl;

#define TSSGAttributeElement_IsEqual(elem1, elem2) (elem1)->VTable->IsEqual(elem1, elem2)

//---------------------------------------------------------------------
//「クラス宣言」
//---------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TSSGAttributeElement {
	AeVtbl *VTable;
	AeType type;
} TSSGAttributeElement;
#pragma pack(pop)

#define _TSSGAttributeElement_VTable 0x006151C0
#define TSSGAttributeElement_VTable  ((void *)_TSSGAttributeElement_VTable)
__inline TSSGAttributeElement * TSSGAttributeElement_ctor(TSSGAttributeElement *this)
{
	this->VTable = TSSGAttributeElement_VTable;
	this->type = 0;
	return this;
}
__inline void TSSGAttributeElement_dtor(TSSGAttributeElement *this)
{
}
EXTERN_C TSSGAttributeElement * __cdecl new_TSSGAttributeElement();
EXTERN_C void __fastcall delete_TSSGAttributeElement(TSSGAttributeElement *this);
#define TSSGAttributeElement_GetType(/*TSSGAttributeElement * */this) \
	(this)->type
//----------------------------------------------------------------------------
//「階層属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TDirAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long level;
	size_t        identity;
	void          *arbitrary;
} TDirAttribute;
#pragma pack(pop)

#define _TDirAttribute_VTable 0x006403A8
#define TDirAttribute_VTable  ((void *)_TDirAttribute_VTable)
__inline TDirAttribute * TDirAttribute_ctor(TDirAttribute *this)
{
	this->VTable = TDirAttribute_VTable;
	this->type = atDIR_LEVEL;
	this->level = 0;
	this->identity = 0;
	this->arbitrary = NULL;
	return this;
}
__inline void TDirAttribute_dtor(TDirAttribute *this)
{
}
EXTERN_C TDirAttribute * __cdecl new_TDirAttribute();
EXTERN_C void __fastcall delete_TDirAttribute(TDirAttribute *this);
#define TDirAttribute_Setting(/*TDirAttribute * */this, /*unsigned long */Level) \
	((this)->level = Level)
//----------------------------------------------------------------------------
//「読み書きサイズ指定属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TSizeAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	unsigned long size;
} TSizeAttribute;
#pragma pack(pop)

#define _TSizeAttribute_VTable 0x00640378
#define TSizeAttribute_VTable  ((void *)_TSizeAttribute_VTable)
__inline TSizeAttribute * TSizeAttribute_ctor(TSizeAttribute *this)
{
	this->VTable = TSizeAttribute_VTable;
	this->type = atSIZE;
	this->size = 0;
	return this;
}
__inline void TSizeAttribute_dtor(TSizeAttribute *this)
{
}
EXTERN_C TSizeAttribute * __cdecl new_TSizeAttribute();
EXTERN_C void __fastcall delete_TSizeAttribute(TSizeAttribute *this);
#define TSizeAttribute_Setting(/*TSizeAttribute * */this, /*unsigned long */Size) \
	((this)->size = Size)
//----------------------------------------------------------------------------
//「アドレス置き換え属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TReplaceAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string offsetCode;
	bcb6_std_string fileName;
	unsigned long   offsetNum;
	BOOL            displace;
} TReplaceAttribute, TFormatAttribute;
#pragma pack(pop)

#define _TReplaceAttribute_VTable 0x00640390
#define TReplaceAttribute_VTable  ((void *)_TReplaceAttribute_VTable)
__inline TReplaceAttribute * TReplaceAttribute_ctor(TReplaceAttribute *this)
{
	this->VTable = TReplaceAttribute_VTable;
	this->type = atREPLACE;
	bcb6_std_string_ctor(&this->offsetCode);
	bcb6_std_string_ctor(&this->fileName);
	this->offsetNum = 0;
	this->displace = FALSE;
	return this;
}
__inline void TReplaceAttribute_dtor(TReplaceAttribute *this)
{
	bcb6_std_string_dtor(&this->fileName);
	bcb6_std_string_dtor(&this->offsetCode);
}
EXTERN_C TReplaceAttribute * __cdecl new_TReplaceAttribute();
EXTERN_C void __fastcall delete_TReplaceAttribute(TReplaceAttribute *this);
EXTERN_C void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *);
//----------------------------------------------------------------------------
//「ファンネル（随行書き込み）属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TFunnelAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string fileName;
} TFunnelAttribute;
#pragma pack(pop)

#define _TFunnelAttribute_VTable 0x0064033C
#define TFunnelAttribute_VTable  ((void *)_TFunnelAttribute_VTable)
__inline TFunnelAttribute * TFunnelAttribute_ctor(TFunnelAttribute *this)
{
	this->VTable = TFunnelAttribute_VTable;
	this->type = atFUNNEL;
	bcb6_std_string_ctor(&this->fileName);
	return this;
}
__inline void TFunnelAttribute_dtor(TFunnelAttribute *this)
{
	bcb6_std_string_dtor(&this->fileName);
}
EXTERN_C TFunnelAttribute * __cdecl new_TFunnelAttribute();
EXTERN_C void __fastcall delete_TFunnelAttribute(TFunnelAttribute *this);
#define TFunnelAttribute_Setting(/*TFunnelAttribute * */this, /*const bcb6_std_string * */FileName) \
	bcb6_std_string_assign(&(this)->fileName, FileName)
//----------------------------------------------------------------------------
//「入出力時変換属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TIO_FEPAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string inputCode;
	bcb6_std_string outputCode;
} TIO_FEPAttribute, TDefineAttribute;
#pragma pack(pop)

#define _TIO_FEPAttribute_VTable 0x00640324
#define TIO_FEPAttribute_VTable  ((void *)_TIO_FEPAttribute_VTable)
__inline TIO_FEPAttribute * TIO_FEPAttribute_ctor(TIO_FEPAttribute *this)
{
	this->VTable = TIO_FEPAttribute_VTable;
	this->type = atIO_FEP;
	bcb6_std_string_ctor(&this->inputCode);
	bcb6_std_string_ctor(&this->outputCode);
	return this;
}
__inline void TIO_FEPAttribute_dtor(TIO_FEPAttribute *this)
{
	bcb6_std_string_dtor(&this->outputCode);
	bcb6_std_string_dtor(&this->inputCode);
}
EXTERN_C TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute();
EXTERN_C void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *this);
EXTERN_C void(__cdecl * const TIO_FEPAttribute_Setting)(TIO_FEPAttribute *this, TStringDivision *StrD, const char *Code);
#define TIO_FEPAttribute_GetInputCode(/*TIO_FEPAttribute * */this) \
	(&(this)->inputCode)
#define TIO_FEPAttribute_GetOutputCode(/*TIO_FEPAttribute * */this) \
	(&(this)->outputCode)
//----------------------------------------------------------------------------
//「コード付加属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TEndWithAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string code;
} TEndWithAttribute, TPrologueAttribute;
#pragma pack(pop)

#define _TEndWithAttribute_VTable 0x0064030C
#define TEndWithAttribute_VTable  ((void *)_TEndWithAttribute_VTable)
__inline TEndWithAttribute * TEndWithAttribute_ctor(TEndWithAttribute *this)
{
	this->VTable = TEndWithAttribute_VTable;
	this->type = atE_WITH;
	bcb6_std_string_ctor(&this->code);
	return this;
}
__inline void TEndWithAttribute_dtor(TEndWithAttribute *this)
{
	bcb6_std_string_dtor(&this->code);
}
EXTERN_C TEndWithAttribute * __cdecl new_TEndWithAttribute();
EXTERN_C void __fastcall delete_TEndWithAttribute(TEndWithAttribute *this);
#define TEndWithAttribute_GetCode(/*TEndWithAttribute * */this) \
	(&(this)->code)
#define TEndWithAttribute_Setting(/*TEndWithAttribute * */this, /*LPCSTR */Code, /*size_t */Length) \
	bcb6_std_string_assign_cstr_with_length(&(this)->code, Code, Length)
#define TEndWithAttribute_Setting_stdstr(/*TEndWithAttribute * */this, /*const bcb6_std_string * */Code) \
	bcb6_std_string_assign(&(this)->code, Code)
//----------------------------------------------------------------------------
//「有効状態属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TEnabledAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string code;
} TEnabledAttribute;
#pragma pack(pop)

#define _TEnabledAttribute_VTable 0x006402F4
#define TEnabledAttribute_VTable  ((void *)_TEnabledAttribute_VTable)
__inline TEnabledAttribute * TEnabledAttribute_ctor(TEnabledAttribute *this)
{
	this->VTable = TEnabledAttribute_VTable;
	this->type = atENABLED;
	bcb6_std_string_ctor(&this->code);
	return this;
}
__inline void TEnabledAttribute_dtor(TEnabledAttribute *this)
{
	bcb6_std_string_dtor(&this->code);
}
EXTERN_C TEnabledAttribute * __cdecl new_TEnabledAttribute();
EXTERN_C void __fastcall delete_TEnabledAttribute(TEnabledAttribute *this);
#define TEnabledAttribute_Setting(/*TEnabledAttribute * */this, /*const bcb6_std_string * */Code) \
	bcb6_std_string_assign(&(this)->code, Code)
//----------------------------------------------------------------------------
//「下位項目一括可否属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TChildRWAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	unsigned char prohibit;
	bool          spAccept;
	bool          spIgnore;
	__int8 : 8;
} TChildRWAttribute, TOnErrorAttribute;
#pragma pack(pop)

#define _TChildRWAttribute_VTable 0x006402C4
#define TChildRWAttribute_VTable  ((void *)_TChildRWAttribute_VTable)
__inline TChildRWAttribute * TChildRWAttribute_ctor(TChildRWAttribute *this)
{
	extern UINT ExtensionTSSDir;

	this->VTable = TChildRWAttribute_VTable;
	this->type = atCHILD_RW;
	*(BOOL *)&this->prohibit = FALSE;
	this->spAccept = !!ExtensionTSSDir;
	this->spIgnore = TRUE;
	return this;
}
__inline void TChildRWAttribute_dtor(TChildRWAttribute *this)
{
}
EXTERN_C TChildRWAttribute * __cdecl new_TChildRWAttribute();
EXTERN_C void __fastcall delete_TChildRWAttribute(TChildRWAttribute *this);
EXTERN_C void __cdecl TChildRWAttribute_Setting(TChildRWAttribute *this, TStringDivision *StrD, const char *Code);
//----------------------------------------------------------------------------
//「注意事項指定属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TCautionAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bcb6_std_string fileName;
} TCautionAttribute;
#pragma pack(pop)

#define _TCautionAttribute_VTable 0x006402AC
#define TCautionAttribute_VTable  ((void *)_TCautionAttribute_VTable)
__inline TCautionAttribute * TCautionAttribute_ctor(TCautionAttribute *this)
{
	this->VTable = TCautionAttribute_VTable;
	this->type = atCAUTION;
	bcb6_std_string_ctor(&this->fileName);
	return this;
}
__inline void TCautionAttribute_dtor(TCautionAttribute *this)
{
	bcb6_std_string_dtor(&this->fileName);
}
EXTERN_C TCautionAttribute * __cdecl new_TCautionAttribute();
EXTERN_C void __fastcall delete_TCautionAttribute(TCautionAttribute *this);
#define TCautionAttribute_Setting(/*TCautionAttribute * */this, /*const bcb6_std_string * */FileName) \
	bcb6_std_string_assign(&(this)->fileName, FileName)
//----------------------------------------------------------------------------
//「アドレス補正の試作などで用いる、補正試作時に優先使用するかなどの属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TAdjustCheckAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	bool    check;
	bool    mustCheck;
	__int16 : 16;
} TAdjustCheckAttribute;
#pragma pack(pop)

#define _TAdjustCheckAttribute_VTable 0x006402DC
#define TAdjustCheckAttribute_VTable  ((void *)_TAdjustCheckAttribute_VTable)
__inline TAdjustCheckAttribute * TAdjustCheckAttribute_ctor(TAdjustCheckAttribute *this)
{
	this->VTable = TAdjustCheckAttribute_VTable;
	this->type = atADJUST_CHECK;
	this->check = TRUE;
	this->mustCheck = FALSE;
	return this;
}
__inline void TAdjustCheckAttribute_dtor(TAdjustCheckAttribute *this)
{
}
EXTERN_C TAdjustCheckAttribute * __cdecl new_TAdjustCheckAttribute();
EXTERN_C void __fastcall delete_TAdjustCheckAttribute(TAdjustCheckAttribute *this);
EXTERN_C void(__cdecl * const TAdjustCheckAttribute_Setting)(TAdjustCheckAttribute *this, TStringDivision *StrD, const char *Code);
//----------------------------------------------------------------------------
//「基本補正属性抽象クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TAdjustmentAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSSGAttributeElement super;
	};
	union
	{
		AdjState  status;
		LPCSTR    c_str;
	};
	AdjCheck      checkType;
	unsigned long adjustVal;
	unsigned      seqElement;// define here for convenience' sake.
} TAdjustmentAttribute, TSimpleAdjustmentAttribute;
#pragma pack(pop)

#define _TAdjustmentAttribute_VTable 0x006151D8
#define TAdjustmentAttribute_VTable  ((void *)_TAdjustmentAttribute_VTable)
__inline TAdjustmentAttribute *TRepeatAttribute_ctor(TAdjustmentAttribute *this)
{
	this->VTable = TAdjustmentAttribute_VTable;
	this->type = atADJUST;
	this->status = 0;
	this->checkType = 0;
	return this;
}
#define TAdjustmentAttribute_Setting(Attr, SSGC, Code) \
	(Attr)->VTable->Setting(Attr, SSGC, Code)

//----------------------------------------------------------------------------
//「ヒープ補正属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _THeapAdjustmentAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TAdjustmentAttribute super;
	};
	bcb6_std_map heapMap;
} THeapAdjustmentAttribute, TScopeAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
typedef struct value_type
{
	// const
	bcb6_std_string  Identifier;
	union data_type
	{
		struct mapped_type
		{
			unsigned Low;
			unsigned High;
		};
		long    long Quad;
		long  double Real;
		const LPCSTR Text;
	};
} ScopeVariant;
//----------------------------------------------------------------------------
//「CRC補正属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TCRCAdjustmentAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSimpleAdjustmentAttribute super;
	};
	bcb6_std_map crcMap;
} TCRCAdjustmentAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
//「サーチ補正属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TSearchAdjustmentAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSimpleAdjustmentAttribute super;
	};
	bcb6_std_string startAddressStr;
	bcb6_std_string minAddressStr;
	bcb6_std_string maxAddressStr;
	unsigned long   step;
	__int32 : 32;
	bcb6_std_string searchCode;
	bcb6_std_map    diffMap;
} TSearchAdjustmentAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------
//「ダイアログ補正属性クラス」
//----------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct _TDialogAdjustmentAttribute {
	union {
		struct {
			AeVtbl *VTable;
			AeType type;
		};
		TSearchAdjustmentAttribute super;
	};
	bcb6_std_string        fileName;
	bcb6_std_string        valStr;
	bool                   isMemoWordWrap;
	__int8 : 8;
	__int16 : 16;
	__int32 : 32;
	bcb6_std_vector_string memo;
	bcb6_std_vector_string answerVec;
	bcb6_std_vector_byte   flagVec;
	bcb6_std_vector        subjectVec;
} TDialogAdjustmentAttribute;
#pragma pack(pop)
//----------------------------------------------------------------------------

