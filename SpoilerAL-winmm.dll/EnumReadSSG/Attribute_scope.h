#include <errno.h>
#include <stdint.h>
#include <windows.h>
#include <dhcpsapi.h>
#include "intrinsic.h"
#include "HashBytes.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "SubjectStringTable.h"

EXTERN_C void __stdcall ReplaceDefine(TSSGAttributeSelector *attributeSelector, string *line);

__inline TScopeAttribute *new_TScopeAttribute()
{
	static const char tag[] = "heap";
	static const string Tag = { (LPSTR)tag, (LPSTR)tag + _countof(tag) - 1, ._M_end_of_storage = tag };

	THeapAdjustmentAttribute *const NewAElem = TSSGCtrl_MakeAdjustmentClass(&Tag);
	assert(NewAElem && map_end(&NewAElem->heapMap)->_M_parent == NULL);// required when using different node type
	NewAElem->type = atSCOPE;
	return (TScopeAttribute *)NewAElem;
}

DWORD_DWORD __fastcall TSSGCtrl_ReadSSG_rootSubject_SetAttribute(
	TSSGCtrl const *const  this,
	vector                *nowAttributeVec,
	TSSGAttributeSelector *attributeSelector,
	TDirAttribute   const *NewAElem)
{
	TScopeAttribute *scope = new_TScopeAttribute();
	scope->super.adjustVal = 0;// global scope setup
	nowAttributeVec = TSSGAttributeSelector_AddElement(attributeSelector, scope);
	nowAttributeVec->allocator_type[0] = NULL;
	return (DWORD_DWORD) { (DWORD)nowAttributeVec, (DWORD)this->rootSubject };
}

__inline void Attribute_scope_open(TSSGCtrl *const this, string *const code)
{
	string label, Token;
	vector_string tmpV = { NULL };

	TScopeAttribute *scope = new_TScopeAttribute();
	scope->super.adjustVal = -(intptr_t)scope;// guarantee unique

	ReplaceDefine(&this->attributeSelector, code);
	TStringDivision_Half_WithoutTokenDtor(&label, &this->strD, code, ":", 1u, 0, etTRIM);
	if (string_at(&label, 0) != ':')
	{
		LPSTR last;
		ULONG val = strtoul(string_c_str(&label), &last, 0);
		scope->super.adjustVal = last != string_end(&label) ? SubjectStringTable_insert(&label) : val;
	}
	string_dtor(&label);

	TSSGAttributeSelector_AddElement(&this->attributeSelector, scope);
	scope = *(TScopeAttribute **)list_back(this->attributeSelector.nowAttributeList);

	TStringDivision_List(&this->strD, code, *string_ctor_assign_char(&Token, ','), &tmpV, etTRIM);
	for (string *tmpS = vector_begin(&tmpV); tmpS < vector_end(&tmpV); ++tmpS)
	{
		TStringDivision_Half_WithoutTokenDtor(&label, &this->strD, tmpS, "=", 1u, 0, etTRIM);
		if (!string_empty(&label) && !string_empty(tmpS))
		{
			BOOL assign = string_at(&label, 0) != '=';
			string *var = assign ? &label : tmpS;
			LPSTR start = &string_at(var, string_at(var, 0) == SCOPE_PREFIX);
			ScopeVariant sv = { { start, string_end(var),  ._M_end_of_storage = start }, 0, 0 };
			map_iterator it = map_lower_bound(&scope->heapMap, &sv.Identifier);
			if (it == map_end(&scope->heapMap) || !string_equals(&((ScopeVariant *)pair_first(it))->Identifier, &sv.Identifier))
				map_string_quad_insert(&it, &scope->heapMap, it, &sv);
			((ScopeVariant *)pair_first(it))->Identifier.sstIndex = MAXDWORD;
			if (assign)
			{
				char *endptr;

				errno = 0;
				((ScopeVariant *)pair_first(it))->Quad = _strtoui64(string_c_str(tmpS), &endptr, 0);
				do	/* do { ... } while (0); */
				{
					if (errno != ERANGE)
						switch (*endptr)
						{
						case '\0':
						case '\t':
						case '\n':
						case '\r':
						case ' ':
						case '#':
						case '/':
						case ';':
							if (((ScopeVariant *)pair_first(it))->Quad < 0 && ((ScopeVariant *)pair_first(it))->Quad >= LONG_MIN)
								((ScopeVariant *)pair_first(it))->High = 0;
							continue;
						}
					((ScopeVariant *)pair_first(it))->Real = strtod(string_c_str(tmpS), NULL);
				} while (0);
			}
		}
		string_dtor(&label);
	}
	vector_string_dtor(&tmpV);
}

__inline void Attribute_scope_close(TSSGCtrl *this)
{
	TSSGAttributeSelector_EraseElementByType(&this->attributeSelector, atSCOPE);
}
