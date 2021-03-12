#pragma once

#include <windows.h>
#include "intrinsic.h"

#pragma pack(push, 1)
typedef struct _String_base
{
	LPSTR  _M_start;
	LPSTR  _M_finish;
	signed sstIndex;
	__int32 : 32;// using as order @ TSSGAttributeElement
	LPCSTR _M_end_of_storage;
	__int32 : 32;
	__int32 : 0;// fix wronged size aggregate initializer
} bcb6_std_string, *pbcb6_std_string;
#pragma pack(pop)

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_string  string;
typedef pbcb6_std_string pstring;
#define string_c_str                        bcb6_std_string_c_str
#define string_begin                        bcb6_std_string_begin
#define string_end                          bcb6_std_string_end
#define string_end_of_storage               bcb6_std_string_end_of_storage
#define string_length                       bcb6_std_string_length
#define string_size                         bcb6_std_string_size
#define string_capacity                     bcb6_std_string_capacity
#define string_storage_size                 bcb6_std_string_storage_size
#define string_storage_capacity             bcb6_std_string_storage_capacity
#define string_empty                        bcb6_std_string_empty
#define string_at                           bcb6_std_string_at
#define string_clear                        bcb6_std_string_clear
#define new_string                          new_bcb6_std_string
#define delete_string                       delete_bcb6_std_string
#define string_dtor                         bcb6_std_string_dtor
#define string_ctor                         bcb6_std_string_ctor
#define string_ctor_null                    bcb6_std_string_ctor_null
#define string_ctor_assign_range            bcb6_std_string_ctor_assign_range
#define string_ctor_assign                  bcb6_std_string_ctor_assign
#define string_ctor_assign_cstr             bcb6_std_string_ctor_assign_cstr
#define string_ctor_assign_cstr_with_length bcb6_std_string_ctor_assign_cstr_with_length
#define string_ctor_assign_char             bcb6_std_string_ctor_assign_char
#define string_ctor_assign_wchar            bcb6_std_string_ctor_assign_wchar
#define string_assign                       bcb6_std_string_assign
#define string_assign_range                 bcb6_std_string_assign_range
#define string_assign_cstr                  bcb6_std_string_assign_cstr
#define string_assign_cstr_with_length      bcb6_std_string_assign_cstr_with_length
#define string_assign_char                  bcb6_std_string_assign_char
#define string_assign_wchar                 bcb6_std_string_assign_wchar
#define string_append_repeat_char           bcb6_std_string_append_repeat_char
#define string_append                       bcb6_std_string_append
#define string_append_range                 bcb6_std_string_append_range
#define string_append_cstr_with_length      bcb6_std_string_append_cstr_with_length
#define string_concat                       bcb6_std_string_concat
#define string_reserve                      bcb6_std_string_reserve
#define string_resize                       bcb6_std_string_resize
#define string_storage_append               bcb6_std_string_storage_append
#define string_storage_resize               bcb6_std_string_storage_resize
#define string_shrink_to_fit                bcb6_std_string_shrink_to_fit
#define string_trim_left                    bcb6_std_string_trim_left
#define string_trim_right                   bcb6_std_string_trim_right
#define string_trim                         bcb6_std_string_trim
#define string_trim_left_blank              bcb6_std_string_trim_left_blank
#define string_trim_right_blank             bcb6_std_string_trim_right_blank
#define string_trim_blank                   bcb6_std_string_trim_blank
#define string_equals                       bcb6_std_string_equals
#define string_compare                      bcb6_std_string_compare
#define string_push_back                    bcb6_std_string_push_back
#define string_insert_char                  bcb6_std_string_insert_char
#define string_swap                         bcb6_std_string_swap
#define string_append_wchar                 bcb6_std_string_append_wchar
#endif

#define bcb6_std_string_ctor_null(s)        ((s)->_M_end_of_storage = (s)->_M_finish = (s)->_M_start = NULL)
#define bcb6_std_string_c_str(s)            (LPCSTR)(s)->_M_start
#define bcb6_std_string_begin(s)            (s)->_M_start
#define bcb6_std_string_end(s)              (s)->_M_finish
#define bcb6_std_string_end_of_storage(s)   (s)->_M_end_of_storage
#define bcb6_std_string_length(s)           (size_t)((s)->_M_finish - (s)->_M_start)
#define bcb6_std_string_size                bcb6_std_string_length
#define bcb6_std_string_capacity(s)         (bcb6_std_string_storage_capacity(s) - 1)
#define bcb6_std_string_storage_size(s)     (bcb6_std_string_size(s) + 1)
#define bcb6_std_string_storage_capacity(s) (size_t)((s)->_M_end_of_storage - (s)->_M_start)
#define bcb6_std_string_empty(s)            ((s)->_M_finish == (s)->_M_start)
#define bcb6_std_string_at(s, index)        (s)->_M_start[index]
#define bcb6_std_string_clear(s)            (*((s)->_M_finish = (s)->_M_start) = '\0')

EXTERN_C bcb6_std_string * __cdecl new_bcb6_std_string();
EXTERN_C void __fastcall delete_bcb6_std_string(bcb6_std_string *s);
EXTERN_C void __fastcall bcb6_std_string_dtor(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign(bcb6_std_string *s, const bcb6_std_string *src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_range(bcb6_std_string *s, LPCSTR first, LPCSTR last);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_cstr(bcb6_std_string *s, LPCSTR src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_cstr_with_length(bcb6_std_string *s, LPCSTR src, size_t length);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_char(bcb6_std_string *s, const char c);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_ctor_assign_wchar(bcb6_std_string *s, const wchar_t c);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign(bcb6_std_string *s, const bcb6_std_string *src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_range(bcb6_std_string *s, LPCSTR first, LPCSTR last);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_cstr(bcb6_std_string *s, LPCSTR src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_cstr_with_length(bcb6_std_string *s, LPCSTR src, size_t length);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_char(bcb6_std_string *s, const char c);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_assign_wchar(bcb6_std_string *s, const wchar_t c);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_append_repeat_char(bcb6_std_string *s, size_t n, char c);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_append(bcb6_std_string *s, const bcb6_std_string *src);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_append_range(bcb6_std_string *s, LPCSTR first, LPCSTR last);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_append_cstr_with_length(bcb6_std_string *s, LPCSTR src, size_t length);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_concat(bcb6_std_string *s, const bcb6_std_string *left, const bcb6_std_string *right);
EXTERN_C void __fastcall bcb6_std_string_reserve(bcb6_std_string *s, size_t n);
EXTERN_C void __fastcall bcb6_std_string_resize(bcb6_std_string *s, size_t n);
EXTERN_C void __fastcall bcb6_std_string_storage_append(bcb6_std_string *s, size_t n);
EXTERN_C void __fastcall bcb6_std_string_storage_resize(bcb6_std_string *s, size_t n);
EXTERN_C void __fastcall bcb6_std_string_shrink_to_fit(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_trim_left(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_trim_right(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_trim(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_trim_left_blank(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_trim_right_blank(bcb6_std_string *s);
EXTERN_C bcb6_std_string * __fastcall bcb6_std_string_trim_blank(bcb6_std_string *s);

__inline BOOLEAN bcb6_std_string_equals(const bcb6_std_string* const self, const bcb6_std_string* const other)
{
	size_t length =  bcb6_std_string_length(self);
	return length == bcb6_std_string_length(other) && !memcmp(
		bcb6_std_string_begin(self),
		bcb6_std_string_begin(other),
		length);
}

__inline int bcb6_std_string_compare(bcb6_std_string *s1, bcb6_std_string *s2)
{
	size_t length1 = bcb6_std_string_length(s1);
	size_t length2 = bcb6_std_string_length(s2);
	return memcmp(
		bcb6_std_string_begin(s1),
		bcb6_std_string_begin(s2),
		min(length1, length2) + 1);
}

__inline void bcb6_std_string_push_back(bcb6_std_string *s, char c)
{
	bcb6_std_string_storage_append(s, 1);
	*(LPWORD)(bcb6_std_string_end(s)++) = (BYTE)c;
}

__inline void bcb6_std_string_insert_char(bcb6_std_string *s, size_t pos, char c)
{
	if (pos <= bcb6_std_string_length(s))
	{
		bcb6_std_string_storage_append(s, 1);
		bcb6_std_string_end(s)++;
		memmove(bcb6_std_string_begin(s) + pos + 1, bcb6_std_string_begin(s) + pos, bcb6_std_string_length(s) - pos);
		bcb6_std_string_at(s, pos) = c;
	}
}

__inline void bcb6_std_string_swap(bcb6_std_string *a, bcb6_std_string *b)
{
	bcb6_std_string swap = *a;
	*a = *b;
	*b = swap;
}

__inline void bcb6_std_string_append_wchar(bcb6_std_string *s, wchar_t c)
{
	bcb6_std_string_storage_append(s, 2);
	*(wchar_t *)bcb6_std_string_end(s) = c;
	*(bcb6_std_string_end(s) += 2) = '\0';
}
