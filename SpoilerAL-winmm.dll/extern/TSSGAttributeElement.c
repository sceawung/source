#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeElement.h"
#include "bcb6_operator.h"

#pragma function(memcmp, strlen)

#pragma warning(disable:4733)

void(__cdecl * const TReplaceAttribute_Setting)(TReplaceAttribute *, TStringDivision *, const char *) = (LPVOID)0x004CEA24;
void(__cdecl * const TIO_FEPAttribute_Setting)(TIO_FEPAttribute *this, TStringDivision *StrD, const char *Code) = (LPVOID)0x004CEEE4;
void(__cdecl * const TAdjustCheckAttribute_Setting)(TAdjustCheckAttribute *this, TStringDivision *StrD, const char *Code) = (LPVOID)0x004CF6A0;

TSSGAttributeElement * __cdecl new_TSSGAttributeElement()
{
	return TSSGAttributeElement_ctor(operator_new(sizeof(TSSGAttributeElement)));
}

void __fastcall delete_TSSGAttributeElement(TSSGAttributeElement *this)
{
	TSSGAttributeElement_dtor(this);
	operator_delete(this);
}

TDirAttribute * __cdecl new_TDirAttribute()
{
	return TDirAttribute_ctor(operator_new(sizeof(TDirAttribute)));
}

void __fastcall delete_TDirAttribute(TDirAttribute *this)
{
	TDirAttribute_dtor(this);
	operator_delete(this);
}

TSizeAttribute * __cdecl new_TSizeAttribute()
{
	return TSizeAttribute_ctor(operator_new(sizeof(TSizeAttribute)));
}

void __fastcall delete_TSizeAttribute(TSizeAttribute *this)
{
	TSizeAttribute_dtor(this);
	operator_delete(this);
}

TReplaceAttribute * __cdecl new_TReplaceAttribute()
{
	return TReplaceAttribute_ctor(operator_new(sizeof(TReplaceAttribute)));
}

void __fastcall delete_TReplaceAttribute(TReplaceAttribute *this)
{
	TReplaceAttribute_dtor(this);
	operator_delete(this);
}

TFunnelAttribute * __cdecl new_TFunnelAttribute()
{
	return TFunnelAttribute_ctor(operator_new(sizeof(TFunnelAttribute)));
}

void __fastcall delete_TFunnelAttribute(TFunnelAttribute *this)
{
	TFunnelAttribute_dtor(this);
	operator_delete(this);
}

TIO_FEPAttribute * __cdecl new_TIO_FEPAttribute()
{
	return TIO_FEPAttribute_ctor(operator_new(sizeof(TIO_FEPAttribute)));
}

void __fastcall delete_TIO_FEPAttribute(TIO_FEPAttribute *this)
{
	TIO_FEPAttribute_dtor(this);
	operator_delete(this);
}

TEndWithAttribute * __cdecl new_TEndWithAttribute()
{
	return TEndWithAttribute_ctor(operator_new(sizeof(TEndWithAttribute)));
}

void __fastcall delete_TEndWithAttribute(TEndWithAttribute *this)
{
	TEndWithAttribute_dtor(this);
	operator_delete(this);
}

TEnabledAttribute * __cdecl new_TEnabledAttribute()
{
	return TEnabledAttribute_ctor(operator_new(sizeof(TEnabledAttribute)));
}

void __fastcall delete_TEnabledAttribute(TEnabledAttribute *this)
{
	TEnabledAttribute_dtor(this);
	operator_delete(this);
}

TChildRWAttribute * __cdecl new_TChildRWAttribute()
{
	return TChildRWAttribute_ctor(operator_new(sizeof(TChildRWAttribute)));
}

void __fastcall delete_TChildRWAttribute(TChildRWAttribute *this)
{
	TChildRWAttribute_dtor(this);
	operator_delete(this);
}

void __cdecl TChildRWAttribute_Setting(TChildRWAttribute *this, TStringDivision *StrD, const char *Code)
{
	string Token, Src = { (LPSTR)Code, (LPSTR)Code + strlen(Code), ._M_end_of_storage = Code };
	vector_string tmpV = { NULL };
	string_ctor_assign_char(&Token, ',');
	{
		const size_t argc = TStringDivision_List(StrD, &Src, Token, &tmpV, etTRIM);
		if (argc > 0)
			this->prohibit = !strncmp(string_c_str(&vector_at(&tmpV, 0)), "false", 6);
		if (argc > 1)
			this->spAccept = !strncmp(string_c_str(&vector_at(&tmpV, 1)), "true", 5);
		if (argc > 2)
			this->spIgnore = !strncmp(string_c_str(&vector_at(&tmpV, 2)), "true", 5);
		vector_string_dtor(&tmpV);
	}
}

TCautionAttribute * __cdecl new_TCautionAttribute()
{
	return TCautionAttribute_ctor(operator_new(sizeof(TCautionAttribute)));
}

void __fastcall delete_TCautionAttribute(TCautionAttribute *this)
{
	TCautionAttribute_dtor(this);
	operator_delete(this);
}

TAdjustCheckAttribute * __cdecl new_TAdjustCheckAttribute()
{
	return TAdjustCheckAttribute_ctor(operator_new(sizeof(TAdjustCheckAttribute)));
}

void __fastcall delete_TAdjustCheckAttribute(TAdjustCheckAttribute *this)
{
	TAdjustCheckAttribute_dtor(this);
	operator_delete(this);
}
