#include <stdint.h>
#include <windows.h>
#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std.h"
#include "bcb6_std_stack.h"
#include "bcb6_std_string.h"
#include "TStringDivision.h"
#include "TSSGCtrl.h"
#include "TSSDir.h"
#include "TSSCalc.h"
#include "TSSToggle.h"
#include "TSSString.h"
#include "SSGSubjectProperty.h"
#include "SubjectStringOperator.h"
#include "Attribute_variable.h"
#include "Attribute_expr.h"
#include "Attribute_define.h"
#include "Attribute_undef.h"
#include "Attribute_allcate.h"
#include "Attribute_error_skip.h"
#include "Attribute_scope.h"
#include "Attribute_offset.h"
#include "TSSArg.h"

extern void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);
extern void __stdcall repeat_ReadSSRFile(
	TSSGCtrl     *this,
	stack        *ParentStack,
	LPVOID       ADJElem,
	const string *LineS,
	DWORD        RepeatIndex,
	DWORD        OuterRepeat,
	TSSGSubject  *SSGS);

extern UINT   MultiStageAttrs;
extern DWORD  RepeatDepth;
extern string ProcessAttachCode;
extern vector *ProcessAttachAttribute;
extern string ProcessDetachCode;
extern vector *ProcessDetachAttribute;
extern DWORD  IndexRoot, IndexTemp;

TSSGSubject dummySSGS = {
	TSSGSubject_VTable,
	.stable = MAXWORD,
	.fields = { NULL },
	.folder = NULL,
	.propertyIndex = MAXDWORD,
};// the remaining members or elements of the aggregate type are initialized to 0.

void __cdecl TSSGCtrl_EnumReadSSG(
	TSSGCtrl                   *this,
	vector_string              *SSGFile,
	stack                      *ParentStack,
	TDialogAdjustmentAttribute *ADJElem,
	DWORD                       RepeatIndex/* = 0*/,
	DWORD                       OuterRepeat/* = MAXDWORD*/)
{
	extern unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...);

	#define stack_PTSSDir_size(Stack)        stack_size((stack *)(Stack), void *)
	#define stack_PTSSDir_top(Stack)         stack_top((stack *)(Stack), TSSDir *)[0]
	#define stack_PTSSDir_push(Stack, Value) stack_ptr_push((stack_ptr *)(Stack), Value)
	#define stack_PTSSDir_pop(Stack)         stack_ptr_pop((stack_ptr *)(Stack))

	size_t invalid = FALSE, condition = TRUE;
	for (string *it = vector_begin(SSGFile); it != vector_end(SSGFile); ++it)
	{
		typedef enum {
			OPEN  = 0,
			CLOSE = 1,
		} TAG_TYPE;

		typedef enum {
			IF,
			ELIF,
			ELSE,
			ENDIF,
			SUBJECT,
			INPUT,
			BACK,
			ROOT,
			REPLACE,
			SIZE_OPEN,
			SIZE_CLOSE,
			ADJUSTMENT_OPEN,
			ADJUSTMENT_CLOSE,
			ADJUST_CHECK_OPEN,
			ADJUST_CHECK_CLOSE,
			FUNNEL_OPEN,
			FUNNEL_CLOSE,
			REPEAT,
			IO_FEP_OPEN,
			IO_FEP_CLOSE,
			E_WITH_OPEN,
			E_WITH_CLOSE,
			ENABLED_OPEN,
			ENABLED_CLOSE,
			CHILD_RW_OPEN,
			CHILD_RW_CLOSE,
			CAUTION_OPEN,
			CAUTION_CLOSE,
			INVOLVE,
			NOTE,
			PROCESS,
			TITLE,
			MAKER,
			CREATOR,
			DISTINCTION,
			CALC,
			TOGGLE,
			STRING,
			VAL,
			SCRIPT_CLOSE,
			ATTACH,
			DETACH,
			VARIABLE_OPEN,
			VARIABLE_CLOSE,
			EXPR,
			DEFINE,
			UNDEF,
			ALLOCATE,
			ERROR_SKIP_OPEN,
			ERROR_SKIP_CLOSE,
			SCOPE_OPEN,
			SCOPE_CLOSE,
			OFFSET_OPEN,
			OFFSET_CLOSE,
			FORMAT_OPEN,
			FORMAT_CLOSE,
		} TAG;

		char     *p, c;
		size_t   length;
		DWORD    dw;
		TAG_TYPE close;
		TAG      tag;

		p = string_begin(it);
		while ((c = *(p++)) == ' ' || c == '\t');
		if (c != '[')
			continue;
		p += close = (*p == '/');
		if ((length = (string_end(it) - p)) < 3)
			continue;

		switch ((BYTE)(dw = *(LPDWORD)p))	// jump by table
		{
		case 'a':
			if (invalid)
				continue;
			switch (dw)
			{
			case BSWAP32('adju'):
				if (length < 11)
					continue;
				switch (*(LPDWORD)(p + 4))
				{
				// [adjustment], [/adjustment]
				case BSWAP32('stme'):
					if (*(LPWORD)(p + 8) != BSWAP16('nt'))
						continue;
					p += 10;
					tag = ADJUSTMENT_OPEN + close;
					break;
				// [adjust_check], [/adjust_check]
				case BSWAP32('st_c'):
					if (length < 13 || *(LPDWORD)(p + 8) != BSWAP32('heck'))
						continue;
					p += 12;
					tag = ADJUST_CHECK_OPEN + close;
					break;
				default:
					continue;
				}
				break;
			// [allocate]
			case BSWAP32('allo'):
				if (close || length < 9 || *(LPDWORD)(p + 4) != BSWAP32('cate'))
					continue;
				p += 8;
				tag = ALLOCATE;
				break;
			// [attach]
			case BSWAP32('atta'):
				if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ch'))
					continue;
				p += 6;
				tag = ATTACH;
				break;
			default:
				continue;
			}
			break;
		// [back]
		case 'b':
			if (invalid || dw != BSWAP32('back') || close || length < 5)
				continue;
			p += 4;
			tag = BACK;
			break;
		case 'c':
			if (invalid)
				continue;
			switch (dw)
			{
			// [calc]
			case BSWAP32('calc'):
				if (close || length < 5)
					continue;
				p += 4;
				tag = CALC;
				break;
			// [caution], [/caution]
			case BSWAP32('caut'):
				if (length < 8 || *(LPDWORD)(p + 3) != BSWAP32('tion'))
					continue;
				p += 7;
				tag = CAUTION_OPEN + close;
				break;
			// [child_rw], [/child_rw]
			case BSWAP32('chil'):
				if (length < 9 || *(LPDWORD)(p + 4) != BSWAP32('d_rw'))
					continue;
				p += 8;
				tag = CHILD_RW_OPEN + close;
				break;
			// [creator]
			case BSWAP32('crea'):
				if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('ator'))
					continue;
				p += 7;
				tag = CREATOR;
				break;
			default:
				continue;
			}
			break;
		case 'd':
			if (invalid)
				continue;
			switch (dw)
			{
			// [define ...]
			case BSWAP32('defi'):
				if (close || length < 9 || *(LPWORD)(p + 4) != BSWAP16('ne') || ((c = p[6]) != ' ' && c != '\t'))
					continue;
				p += 7;
				tag = DEFINE;
				break;
			// [detach]
			case BSWAP32('deta'):
				if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ch'))
					continue;
				p += 6;
				tag = DETACH;
				break;
			// [distinction]
			case BSWAP32('dist'):
				if (close || length < 12 || *(LPDWORD)(p + 4) != BSWAP32('inct') || *(LPDWORD)(p + 7) != BSWAP32('tion'))
					continue;
				p += 11;
				tag = DISTINCTION;
				break;
			default:
				continue;
			}
			break;
		case 'e':
			switch (dw)
			{
			// [e_with], [/e_with]
			case BSWAP32('e_wi'):
				if (invalid || length < 7 || *(LPWORD)(p + 4) != BSWAP16('th'))
					continue;
				p += 6;
				tag = E_WITH_OPEN + close;
				break;
			// [elif]
			case BSWAP32('elif'):
				if (close || length < 5)
					continue;
				p += 4;
				tag = ELIF;
				break;
			// [else]
			case BSWAP32('else'):
				if (close || length < 5)
					continue;
				p += 4;
				tag = ELSE;
				break;
			// [enabled], [/enabled]
			case BSWAP32('enab'):
				if (invalid || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('bled'))
					continue;
				p += 7;
				tag = ENABLED_OPEN + close;
				break;
			// [endif]
			case BSWAP32('endi'):
				if (close || length < 6 || p[4] != 'f')
					continue;
				p += 5;
				tag = ENDIF;
				break;
			// [error_skip], [/error_skip]
			case BSWAP32('erro'):
				if (invalid || length < 11 || *(LPDWORD)(p + 4) != BSWAP32('r_sk') || *(LPWORD)(p + 8) != BSWAP16('ip'))
					continue;
				p += 10;
				tag = ERROR_SKIP_OPEN + close;
				break;
			// [expr]
			case BSWAP32('expr'):
				if (invalid || close || length < 5)
					continue;
				p += 4;
				tag = EXPR;
				break;
			default:
				continue;
			}
			break;
		case 'f':
			if (invalid)
				continue;
			switch (dw)
			{
			// [format], [/format]
			case BSWAP32('form'):
				if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('at'))
					continue;
				p += 6;
				tag = FORMAT_OPEN + close;
				break;
			// [funnel], [/funnel]
			case BSWAP32('funn'):
				if (length < 7 || *(LPWORD)(p + 4) != BSWAP16('el'))
					continue;
				p += 6;
				tag = FUNNEL_OPEN + close;
				break;
			default:
				continue;
			}
			break;
		case 'i':
			switch (dw)
			{
			// [input]
			case BSWAP32('inpu'):
				if (invalid || close || length < 6 || p[4] != 't')
					continue;
				p += 5;
				tag = INPUT;
				break;
			// [involve]
			case BSWAP32('invo'):
				if (invalid || close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('olve'))
					continue;
				p += 7;
				tag = INVOLVE;
				break;
			// [io_fep], [/io_fep]
			case BSWAP32('io_f'):
				if (invalid || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ep'))
					continue;
				p += 6;
				tag = IO_FEP_OPEN + close;
				break;
			// [if]
			default:
				if (close || (WORD)dw != BSWAP16('if'))
					continue;
				p += 2;
				tag = IF;
				break;
			}
			break;
		// [maker]
		case 'm':
			if (invalid || dw != BSWAP32('make') || close || length < 6 || p[4] != 'r')
				continue;
			p += 5;
			tag = MAKER;
			break;
		// [note]
		case 'n':
			if (invalid || dw != BSWAP32('note') || close || length < 5)
				continue;
			p += 4;
			tag = NOTE;
			break;
		// [offset], [/offset]
		case 'o':
			if (invalid || dw != BSWAP32('offs') || length < 7 || *(LPWORD)(p + 4) != BSWAP16('et'))
				continue;
			p += 6;
			tag = OFFSET_OPEN + close;
			break;
		// [process]
		case 'p':
			if (invalid || dw != BSWAP32('proc') || close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('cess'))
				continue;
			p += 7;
			tag = PROCESS;
			break;
		case 'r':
			if (invalid)
				continue;
			switch (dw)
			{
			// [repeat]
			case BSWAP32('repe'):
				if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('at'))
					continue;
				p += 6;
				tag = REPEAT;
				break;
			// [replace]
			case BSWAP32('repl'):
				if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('lace'))
					continue;
				p += 7;
				tag = REPLACE;
				break;
			// [root]
			case BSWAP32('root'):
				if (close || length < 5)
					continue;
				p += 4;
				tag = ROOT;
				break;
			default:
				continue;
			}
			break;
		case 's':
			if (invalid)
				continue;
			switch (dw)
			{
			// [scope], [/scope]
			case BSWAP32('scop'):
				if (length < 6 || p[4] != 'e')
					continue;
				p += 5;
				tag = SCOPE_OPEN + close;
				break;
			// [/script]
			case BSWAP32('scri'):
				if (!close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('pt'))
					continue;
				p += 6;
				tag = SCRIPT_CLOSE;
				break;
			// [size], [/size]
			case BSWAP32('size'):
				if (length < 5)
					continue;
				p += 4;
				tag = SIZE_OPEN + close;
				break;
			// [string]
			case BSWAP32('stri'):
				if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('ng'))
					continue;
				p += 6;
				tag = STRING;
				break;
			// [subject]
			case BSWAP32('subj'):
				if (close || length < 8 || *(LPDWORD)(p + 3) != BSWAP32('ject'))
					continue;
				p += 7;
				tag = SUBJECT;
				break;
			default:
				continue;
			}
			break;
		case 't':
			if (invalid)
				continue;
			switch (dw)
			{
			// [title]
			case BSWAP32('titl'):
				if (close || length < 6 || p[4] != 'e')
					continue;
				p += 5;
				tag = TITLE;
				break;
			// [toggle]
			case BSWAP32('togg'):
				if (close || length < 7 || *(LPWORD)(p + 4) != BSWAP16('le'))
					continue;
				p += 6;
				tag = TOGGLE;
				break;
			default:
				continue;
			}
			break;
		// [undef ...]
		case 'u':
			if (invalid || dw != BSWAP32('unde') || close || length < 8 || p[4] != 'f' || ((c = p[5]) != ' ' && c != '\t'))
				continue;
			p += 6;
			tag = UNDEF;
			break;
		case 'v':
			if (invalid)
				continue;
			switch (dw)
			{
			// [val]
			case BSWAP32('val]'):
				if (close)
					continue;
				p += 3;
				tag = VAL;
				break;
			case BSWAP32('val '):
			case BSWAP32('val\t'):
				if (close || length < 5)
					continue;
				p += 4;
				tag = VAL;
				break;
			// [variable], [/variable]
			case BSWAP32('vari'):
				if (length < 9 || *(LPDWORD)(p + 4) != BSWAP32('able'))
					continue;
				p += 8;
				tag = VARIABLE_OPEN + close;
				break;
			default:
				continue;
			}
			break;
		default:
			continue;
		}

		while ((c = *(p++)) == ' ' || c == '\t');
		if (!c || ((c != ']') ^ (tag == DEFINE || tag == UNDEF)))
			continue;

		switch (tag)	// jump by table
		{
		// [elif], [else]
		case ELIF:
		case ELSE:
			if (invalid > TRUE || (invalid = condition) || tag == ELSE)
				break;
			/* FALLTHROUGH */

		// [if]
		case IF:
			if (!invalid)
			{
				dummySSGS.folder = stack_PTSSDir_top(ParentStack);
				dummySSGS.propertyIndex = OuterRepeat;
				invalid = !(condition = !!Parsing(this, &dummySSGS, &(string) { p, string_end(it), ._M_end_of_storage = p }, 0));
				dummySSGS.propertyIndex = MAXDWORD;
				dummySSGS.folder = NULL;
			}
			else
				invalid++;
			break;

		// [endif]
		case ENDIF:
			if (invalid <= TRUE)
				invalid = FALSE, condition = TRUE;
			else
				invalid--;
			break;

		// [subject], [input]
		case SUBJECT:
		case INPUT:
			{
				string              Name, Code, Tag;
				TSSGSubject         *SSGS; // 項目名、コード部分、項目種別の取得
				TSSGSubjectProperty *const prop = GetProperty(OuterRepeat);

				string_ctor_assign_cstr_with_length(&Code, p, string_end(it) - p);
				TStringDivision_Half_WithoutTokenDtor(&Name, &this->strD, &Code, ":", 1, 0, dtESCAPE);
				TStringDivision_Half_WithoutTokenDtor(&Tag, &this->strD, &Code, ",", 1, 0, dtESCAPE);
				if ((p = string_begin(&Tag))[0] == ',' && p[1] == '\0')
				{
					std_swap(&Tag, &Code);
					string_clear(&Code);
				}

				// この時点でクラス分け
				SSGS = TSSGCtrl_MakeSubjectClass(string_trim_blank(&Tag));
				string_dtor(&Tag);
				if (SSGS)
				{
					if (SSGS->isAdjustment = tag == INPUT)
					{
						string  tmpS;
						BOOLEAN CanUnknown;

						TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &Code, ",", 1, 0, etTRIM);
						CanUnknown = (p = string_begin(&tmpS))[0] == 'o' && p[1] == 'k' && p[2] == '\0';
						TSSGSubject_SetCanUnknown(SSGS, CanUnknown);
						string_dtor(&tmpS);
						// アドレスは存在しない(その位置に必須入力判定がある)
						string_storage_append(&Code, 2);
						memmove(string_begin(&Code) + 2, string_begin(&Code), string_length(&Code) + 1);
						*(LPWORD)string_begin(&Code) = BSWAP16('0,');
						string_end(&Code) += 2;
					}
					TSSGSubject_SetCode_stdstr(SSGS, &Code);
					TSSGSubject_SetName_stdstr(SSGS, &Name);
				}
				string_dtor(&Code);
				string_dtor(&Name);
				if (!SSGS)
					// 未サポートのタグ
					break;

				vector_push_back(&stack_PTSSDir_top(ParentStack)->childVec, SSGS);
				TSSGSubject_SetAttribute(SSGS, TSSGAttributeSelector_GetNowAtteributeVec(&this->attributeSelector));

				if (!ADJElem)
				{
					SSGS->folder = stack_PTSSDir_top(ParentStack);
					SSGS->propertyIndex = OuterRepeat;
					if (prop) prop->DirectChild++;
				}

				if (SSGS->type == stDIR)
				{// dir
					TDirAttribute *const  NewAElem = new_TDirAttribute();
					TDirAttribute_Setting(NewAElem, TSSGCtrl_GetDirLevel(this, SSGS) + 1);
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
					stack_PTSSDir_push(ParentStack, &SSGS);
				}// change parent

				TSSGSubject_Setting(SSGS, this);
			}
			break;

		// [back]
		case BACK:
			if (stack_PTSSDir_size(ParentStack) > 1)
			{
				TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atDIR_LEVEL);
				stack_PTSSDir_pop(ParentStack);
			}
			break;

		// [root]
		case ROOT:
			{
				unsigned long StackSize;

				StackSize = stack_PTSSDir_size(ParentStack);
				if (StackSize > 1)
				{
					unsigned long i;

					TSSGAttributeSelector_ToRootElementByType(&this->attributeSelector, atDIR_LEVEL);
					i = StackSize - 1;
					do
						stack_PTSSDir_pop(ParentStack);
					while (--i);
				}
			}
			break;

		// [replace]
		case REPLACE:
			{
				string            LineS;
				TReplaceAttribute *NewAElem;
				vector_string     *tmpL;
				string            FName, DefaultExt;
				LPSTR             end;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TReplaceAttribute();
				TReplaceAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);

				string_ctor_assign(&FName, &NewAElem->fileName);
				string_ctor_assign_cstr_with_length(&DefaultExt, ".SSC", 4);
				dummySSGS.folder = stack_PTSSDir_top(ParentStack);
				dummySSGS.propertyIndex = OuterRepeat;
				tmpL = TSSGCtrl_GetSSGDataFile(this, &dummySSGS, FName, DefaultExt, NULL);
				dummySSGS.propertyIndex = MAXDWORD;
				dummySSGS.folder = NULL;
				if (tmpL == NULL)
				{
					delete_TReplaceAttribute(NewAElem);
					break;
				}
				// [replace]属性を追加
				if (NewAElem->offsetNum || (strtoul(string_c_str(&NewAElem->offsetCode), &end, 0), end) != string_end(&NewAElem->offsetCode))
					TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);
				else
					delete_TReplaceAttribute(NewAElem), NewAElem = NULL;

				// 再帰
				TSSGCtrl_EnumReadSSG(this, tmpL, ParentStack, ADJElem, RepeatIndex, OuterRepeat);
				// 帰ってきたので、[replace]属性を外す
				if (NewAElem)
					TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atREPLACE);
			}
			break;

		// [size]
		case SIZE_OPEN:
			{
				// アクセスサイズ強制指定命令
				string LineS;
				long   Size;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				Size = TStringDivision_ToLongDef(&LineS, -1);
				string_dtor(&LineS);
				if (Size >= 0)
				{
					TSizeAttribute *NewAElem;

					// [size]属性を付加
					NewAElem = new_TSizeAttribute(); // size
					TSizeAttribute_Setting(NewAElem, Size);
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
				}
			}
			break;

		// [/size]
		case SIZE_CLOSE:
			// [size]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atSIZE);
			break;

		// [adjustment]
		case ADJUSTMENT_OPEN:
			{
				string               LineS, tmpS;
				TAdjustmentAttribute *NewAElem;

				// [adjustment]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, ",", 1, 0, FALSE);
				NewAElem = TSSGCtrl_MakeAdjustmentClass(&tmpS);
				string_dtor(&tmpS);
				if (NewAElem != NULL)
				{
					TAdjustmentAttribute_Setting(NewAElem, this, string_c_str(&LineS));
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
				}
				string_dtor(&LineS);
			}
			break;

		// [/adjustment]
		case ADJUSTMENT_CLOSE:
			// [adjustment]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atADJUST);
			break;

		// [adjust_check]
		case ADJUST_CHECK_OPEN:
			{
				string                LineS;
				TAdjustCheckAttribute *NewAElem;

				// [adjust_check]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TAdjustCheckAttribute();
				TAdjustCheckAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/adjust_check]
		case ADJUST_CHECK_CLOSE:
			// [adjust_check]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atADJUST_CHECK);
			break;

		// [funnel]
		case FUNNEL_OPEN:
			{
				string           LineS;
				TFunnelAttribute *NewAElem;

				// [funnel]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TFunnelAttribute();
				TFunnelAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				if (MultiStageAttrs & atFUNNEL)
					TSSGAttributeSelector_AddElement (&this->attributeSelector, NewAElem);
				else
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/funnel]
		case FUNNEL_CLOSE:
			// [funnel]属性を除去
			if (MultiStageAttrs & atFUNNEL)
				TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atFUNNEL);
			else
				TSSGAttributeSelector_PopElementByType  (&this->attributeSelector, atFUNNEL);
			break;

		// [repeat]
		case REPEAT:
			{
				string LineS;

				// 繰り返し書式発動！
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				repeat_ReadSSRFile(this, ParentStack, ADJElem, &LineS, RepeatIndex, OuterRepeat, &dummySSGS);
				string_dtor(&LineS);
			}
			break;

		// [io_fep]
		case IO_FEP_OPEN:
			{
				string           LineS;
				TIO_FEPAttribute *NewAElem;

				// [io_fep]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TIO_FEPAttribute(); // io_fep
				TIO_FEPAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				if (MultiStageAttrs & atIO_FEP)
					TSSGAttributeSelector_AddElement (&this->attributeSelector, NewAElem);
				else
					TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/io_fep]
		case IO_FEP_CLOSE:
			// [io_fep]属性を除去
			if (MultiStageAttrs & atIO_FEP)
				TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atIO_FEP);
			else
				TSSGAttributeSelector_PopElementByType  (&this->attributeSelector, atIO_FEP);
			break;

		// [e_with]
		case E_WITH_OPEN:
			{
				string            LineS;
				TEndWithAttribute *NewAElem;

				// [e_with]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TEndWithAttribute(); // e_with
				TEndWithAttribute_Setting_stdstr(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/e_with]
		case E_WITH_CLOSE:
			// [e_with]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atE_WITH);
			break;

		// [enabled]
		case ENABLED_OPEN:
			{
				string            LineS;
				TEnabledAttribute *NewAElem;

				// [enabled]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TEnabledAttribute(); // enabled
				TEnabledAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/enabled]
		case ENABLED_CLOSE:
			// [enabled]属性を除去
			TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atENABLED);
			break;

		// [child_rw]
		case CHILD_RW_OPEN:
			{
				string            LineS;
				TChildRWAttribute *NewAElem;

				// [child_rw]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TChildRWAttribute(); // child_rw
				TChildRWAttribute_Setting(NewAElem, &this->strD, string_c_str(&LineS));
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/child_rw]
		case CHILD_RW_CLOSE:
			// [child_rw]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atCHILD_RW);
			break;

		// [caution]
		case CAUTION_OPEN:
			{
				string            LineS;
				TCautionAttribute *NewAElem;

				// [caution]属性を付加
				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				NewAElem = new_TCautionAttribute(); // caution
				TCautionAttribute_Setting(NewAElem, &LineS);
				string_dtor(&LineS);
				TSSGAttributeSelector_PushElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/caution]
		case CAUTION_CLOSE:
			// [caution]属性を除去
			TSSGAttributeSelector_PopElementByType(&this->attributeSelector, atCAUTION);
			break;

		// [involve]
		case INVOLVE:
			{
				string        FName;
				const char    *end;
				vector_string tmpV;

				// 埋め込みファイル
				string_ctor_assign(&FName, &this->scriptDir);
				end = TrimBlank(&p, string_end(it));
				string_append_cstr_with_length(&FName, p, end - p);
				ReplaceDefine(&this->attributeSelector, &FName);
				string_append_cstr_with_length(&FName, ".ssl", 4);
				vector_ctor(&tmpV);
				vector_string_reserve(&tmpV, vector_end(SSGFile) - it);
				while (++it != vector_end(SSGFile))
				{
					p = string_begin(it) - 1;
					do
						c = *(++p);
					while (c == ' ' || c == '\t');
					if ((string_end(it) - p) >= 10 &&
						*(LPDWORD) p      == BSWAP32('[/in') &&
						*(LPDWORD)(p + 4) == BSWAP32('volv') &&
						*         (p + 8) ==         'e'    )
					{
						p += 9;
						do
							if ((c = *(p++)) == ']')
								goto INVOLVE_NESTED_BREAK;
						while (c == ' ' || c == '\t');
					}
					vector_string_push_back(&tmpV, it);
				}
			INVOLVE_NESTED_BREAK:
				vector_shrink_to_fit(&tmpV);
				TSSGCtrl_SetSSGDataFile(this, &tmpV, &FName, TRUE);
				vector_string_dtor(&tmpV);
				string_dtor(&FName);
				if (it == vector_end(SSGFile))
					return;  // [/involve]が存在しない
			}
			break;

		// [note]
		case NOTE:
			{
				const char    *end;
				string        LineS;
				BOOLEAN       IsMemoWordWrap;
				vector_string *memo;

				end = TrimBlank(&p, string_end(it));
				string_ctor_assign_cstr_with_length(&LineS, p, end - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				IsMemoWordWrap = (string_length(&LineS) == 8 && *(LPDWORD)(p = string_begin(&LineS)) == BSWAP32('word') && *(LPDWORD)(p + 4) == BSWAP32('wrap'));
				string_dtor(&LineS);
				if (!ADJElem)
				{
					// SSGモード
					this->isMemoWordWrap = IsMemoWordWrap;
					memo = &this->memo;
				}
				else
				{
					// ADJモード
					ADJElem->isMemoWordWrap = IsMemoWordWrap;
					memo = &ADJElem->memo;
				}
				vector_string_dtor(memo);
				vector_ctor(memo);
				vector_string_reserve(memo, vector_end(SSGFile) - it);
				while (++it != vector_end(SSGFile))
				{
					p = string_begin(it) - 1;
					do
						c = *(++p);
					while (c == ' ' || c == '\t');
					if ((string_end(it) - p) >= 7 &&
						*(LPDWORD) p      == BSWAP32('[/no') &&
						*(LPWORD )(p + 4) == BSWAP16('te'  ))
					{
						p += 6;
						do
							if ((c = *(p++)) == ']')
								// [/note]
								goto NOTE_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					vector_string_push_back(memo, it);
				}
			NOTE_NESTED_BREAK:
				vector_shrink_to_fit(memo);
				if (it == vector_end(SSGFile))
					return;  // [/note]が存在しない
			}
			break;

		// [process]
		case PROCESS:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				vector_string_push_back(&this->processNameVec, &LineS);
				string_dtor(&LineS);
			}
			break;

		// [title]
		case TITLE:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "title", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [maker]
		case MAKER:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "maker", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [creator]
		case CREATOR:
			{
				string LineS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TSSGScriptStruct_SetDistinction_stdstr(&this->script, "creator", &LineS);
				string_dtor(&LineS);
			}
			break;

		// [distinction]
		case DISTINCTION:
			{
				string LineS, tmpS;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, "=", 1, 0, dtESCAPE | etREPLACE | etTRIM);
				if (*(p = string_begin(&tmpS)) != '=' || *(p + 1) != '\0')
					TSSGScriptStruct_SetDistinction_stdstr(&this->script, string_c_str(&tmpS), &LineS);
				string_dtor(&tmpS);
				string_dtor(&LineS);
			}
			break;

		// [calc], [toggle], [string]
		case CALC:
		case TOGGLE:
		case STRING:
			{
				string              LineS, tmpS, Name;
				TSSGSubject         *SSGS;
				BOOLEAN             CanUnknown;

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_Half_WithoutTokenDtor(&tmpS, &this->strD, &LineS, ",", 1, tag != TOGGLE ? 1 : 3, FALSE);
				if (tag == CALC)
					SSGS = &new_TSSCalc()->super;
				else if (tag == TOGGLE)
					SSGS = &new_TSSToggle()->super;
				else/* if (tag == STRING)*/
					SSGS = &new_TSSString()->super;
				TStringDivision_Half_WithoutTokenDtor(&Name, &this->strD, &tmpS, ",", 1, 0, etTRIM);
				CanUnknown = (p = string_begin(&tmpS))[0] == 'o' && p[1] == 'k' && p[2] == '\0';
				TSSGSubject_SetCanUnknown(SSGS, CanUnknown);
				string_dtor(&tmpS);
				TSSGSubject_SetName_stdstr(SSGS, &Name);
				string_dtor(&Name);
				string_storage_append(&LineS, 2);
				memmove(string_begin(&LineS) + 2, string_begin(&LineS), string_length(&LineS) + 1);
				*(LPWORD)string_begin(&LineS) = BSWAP16('0,');
				string_end(&LineS) += 2;
				TSSGSubject_SetCode_stdstr(SSGS, &LineS);
				string_dtor(&LineS);
				SSGS->isAdjustment = TRUE;
				TSSGSubject_Setting(SSGS, this);
				vector_push_back(&stack_PTSSDir_top(ParentStack)->childVec, SSGS);
				TSSGSubject_SetAttribute(SSGS, TSSGAttributeSelector_GetNowAtteributeVec(&this->attributeSelector));
			}
			break;

		// [val]
		case VAL:
			if (ADJElem)
			{
				string_assign_cstr_with_length(&ADJElem->valStr, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &ADJElem->valStr);
				string_shrink_to_fit(&ADJElem->valStr);
			}
			break;

		// [/script]
		case SCRIPT_CLOSE:
			return;

		// [attach]
		case ATTACH:
			{
				string_dtor(&ProcessAttachCode);
				string_ctor_null(&ProcessAttachCode);
				while (++it != vector_end(SSGFile))
				{
					char *first, *last;

					p = string_begin(it) - 1;
					do
						c = *(++p);
					while (c == ' ' || c == '\t');
					if ((string_end(it) - p) >= 9 &&
						*(LPDWORD) p      == BSWAP32('[/at') &&
						*(LPDWORD)(p + 4) == BSWAP32('tach'))
					{
						p += 8;
						do
							if ((c = *(p++)) == ']')
								// [/attach]
								goto ATTACH_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					first = string_begin(it);
					last = TrimSpace(&first, string_end(it));
					string_append_range(&ProcessAttachCode, first, last);
					string_push_back(&ProcessAttachCode, '\n');
				}
			ATTACH_NESTED_BREAK:
				if (!string_empty(&ProcessAttachCode))
				{
					*(--string_end(&ProcessAttachCode)) = '\0';
					string_shrink_to_fit(&ProcessAttachCode);
					ProcessAttachAttribute = TSSGAttributeSelector_GetNowAtteributeVec(&this->attributeSelector);
					ProcessAttachAttribute->allocator_type[0] = NULL;
				}
				if (it == vector_end(SSGFile))
					return;  // [/attach]が存在しない
			}
			break;

		// [detach]
		case DETACH:
			{
				string_dtor(&ProcessDetachCode);
				string_ctor_null(&ProcessDetachCode);
				while (++it != vector_end(SSGFile))
				{
					char *first, *last;

					p = string_begin(it) - 1;
					do
						c = *(++p);
					while (c == ' ' || c == '\t');
					if ((string_end(it) - p) >= 9 &&
						*(LPDWORD) p      == BSWAP32('[/de') &&
						*(LPDWORD)(p + 4) == BSWAP32('tach'))
					{
						p += 8;
						do
							if ((c = *(p++)) == ']')
								// [/detach]
								goto DETACH_NESTED_BREAK;
						while (c == ' ' && c == '\t');
					}
					first = string_begin(it);
					last = TrimSpace(&first, string_end(it));
					string_append_range(&ProcessDetachCode, first, last);
					string_push_back(&ProcessDetachCode, '\n');
				}
			DETACH_NESTED_BREAK:
				if (!string_empty(&ProcessDetachCode))
				{
					*(--string_end(&ProcessDetachCode)) = '\0';
					string_shrink_to_fit(&ProcessDetachCode);
					ProcessDetachAttribute = TSSGAttributeSelector_GetNowAtteributeVec(&this->attributeSelector);
					ProcessDetachAttribute->allocator_type[0] = NULL;
				}
				if (it == vector_end(SSGFile))
					return;  // [/detach]が存在しない
			}
			break;

		// [variable]
		case VARIABLE_OPEN:
			Attribute_variable_open(this, ParentStack, p, string_end(it));
			break;

		// [/variable]
		case VARIABLE_CLOSE:
			// [variable]属性を除去
			Attribute_variable_close(this);
			break;

		// [expr]
		case EXPR:
			Attribute_expr(this, p, string_end(it));
			break;

		// [define ...]
		case DEFINE:
			Attribute_define(this, ParentStack, --p, string_end(it));
			break;

		// [undef ...]
		case UNDEF:
			Attribute_undef(this, --p);
			break;

		// [allocate]
		case ALLOCATE:
			Attribute_allcate(p, string_end(it));
			break;

		// [error_skip]
		case ERROR_SKIP_OPEN:
			Attribute_error_skip_open(this);
			break;

		// [/error_skip]
		case ERROR_SKIP_CLOSE:
			// [error_skip]属性を除去
			Attribute_error_skip_close(this);
			break;

		// [scope]
		case SCOPE_OPEN:
			{
				string code;

				string_ctor_assign_cstr_with_length(&code, p, string_end(it) - p);
				Attribute_scope_open(this, &code);
				string_dtor(&code);
			}
			break;

		// [/scope]
		case SCOPE_CLOSE:
			Attribute_scope_close(this);
			break;

		// [offset]
		case OFFSET_OPEN:
			{
				string code;

				string_ctor_assign_cstr_with_length(&code, p, string_end(it) - p);
				Attribute_offset_open(this, &code);
				string_dtor(&code);
			}
			break;

		// [/offset]
		case OFFSET_CLOSE:
			Attribute_offset_close(this);
			break;

		// [format]
		case FORMAT_OPEN:
			{
				static struct ArgTp {
					struct Constant {
						size_t   const Length;
						LPCSTR   const String;
						uint64_t const Number;
					} const Constants[10];
					const size_t Sentinel;
				} const specifier = { {
					{ 7, "UNKNOWN", 1 << atUNKNOWN     },
					{ 4, "LONG"   , 1 << atLONG        },
					{ 5, "INDEX"  , 1 << atLONG_INDEX  },
					{ 4, "BOOL"   , 1 << atBOOL        },
					{ 6, "STRING" , 1 << atSTRING      },
					{ 6, "VECTOR" , 1 << atBOOL_VECTOR },
					{ 6, "DOUBLE" , 1 << atDOUBLE      },
					{ 3, "ANY"    , ~(-1 << atDIR)     },
					{ 3, "DIR"    , 1 << atDIR         },
					{ 3, "ALT"    , (unsigned)INT_MIN  },
				}, 0 };

				string           LineS, Token;
				TFormatAttribute *NewAElem;
				vector_string    tmpV = { NULL };

				string_ctor_assign_cstr_with_length(&LineS, p, string_end(it) - p);
				ReplaceDefine(&this->attributeSelector, &LineS);
				TStringDivision_List(&this->strD, &LineS, *string_ctor_assign_char(&Token, ','), &tmpV, dtESCAPE | etREPLACE);
				vector_string_resize(&tmpV, 3);
				string_dtor(&LineS);

				NewAElem = operator_new(sizeof(TFormatAttribute));
				NewAElem->VTable    = TReplaceAttribute_VTable;
				NewAElem->type      = atFORMAT;
				NewAElem->displace  = FALSE;
				dummySSGS.folder = stack_PTSSDir_top(ParentStack);
				dummySSGS.propertyIndex = OuterRepeat;
				NewAElem->offsetNum = Parsing(this, &dummySSGS, &vector_at(&tmpV, 0), specifier);
				dummySSGS.propertyIndex = MAXDWORD;
				dummySSGS.folder = NULL;
				string_ctor_assign(&NewAElem->offsetCode , &vector_at(&tmpV, 1));
				string_ctor_assign(&NewAElem->fileName   , &vector_at(&tmpV, 2));
				vector_dtor(&tmpV);

				TSSGAttributeSelector_AddElement(&this->attributeSelector, NewAElem);
			}
			break;

		// [/format]
		case FORMAT_CLOSE:
			TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atFORMAT);
			break;
		}
	}

	#undef stack_PTSSDir_size
	#undef stack_PTSSDir_top
	#undef stack_PTSSDir_push
	#undef stack_PTSSDir_pop
}
