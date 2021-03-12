#include <assert.h>
#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_allocator.h"

#pragma warning(disable:4414)

#pragma function(strlen)

string * __cdecl new_string()
{
	return string_ctor((string *)operator_new(sizeof(string)));
}

void __fastcall delete_string(string *s)
{
	if (s)
	{
		string_dtor(s);
		operator_delete(s);
	}
}

void __fastcall string_dtor(string *s)
{
	assert(!string_begin(s) || string_end_of_storage(s) - string_begin(s));
	allocator_deallocate(string_begin(s), string_storage_capacity(s));
}

string * __fastcall string_ctor(string *s)
{
	char *p = (char *)node_alloc_allocate(8);
	string_begin(s) = p;
	*(string_end(s) = p) = '\0';
	string_end_of_storage(s) = p + 8;
	return s;
}

__declspec(naked) string * __fastcall string_ctor_assign(string *s, const string *src)
{
	__asm
	{
		pop     eax
		push    eax
		push    eax
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		sub     eax, edx
		mov     dword ptr [esp + 4], eax
		jmp     string_ctor_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_ctor_assign_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jae     L1
		mov     dword ptr [esp + 4], 0
	L1:
		jmp     string_ctor_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_ctor_assign_cstr(string *s, LPCSTR src)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    eax
		push    edx
		push    ecx
		push    edx
		call    strlen
		mov     dword ptr [esp + 16], eax
		pop     eax
		pop     ecx
		pop     edx
		jmp     string_ctor_assign_cstr_with_length
	}
}

string * __fastcall string_ctor_assign_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	char *p = (char *)internal_allocate(length + 1);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + length + 1;
	memcpy(p, src, length);
	*string_end(s) = '\0';
	return s;
}

string * __fastcall string_ctor_assign_char(string *s, const char c)
{
	char *p = (char *)node_alloc_allocate(2);
	*(string_begin(s) = p) = c;
	*(string_end(s) = p + 1) = '\0';
	string_end_of_storage(s) = p + 2;
	return s;
}

string * __fastcall string_ctor_assign_wchar(string *s, const wchar_t c)
{
	char *p = (char *)node_alloc_allocate(3);
	*(wchar_t *)(string_begin(s) = p) = c;
	*(string_end(s) = p + 2) = '\0';
	string_end_of_storage(s) = p + 3;
	return s;
}

__declspec(naked) string * __fastcall string_assign(string *s, const string *src)
{
	__asm
	{
		pop     eax
		push    eax
		push    eax
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		sub     eax, edx
		mov     dword ptr [esp + 4], eax
		jmp     string_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_assign_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jae     L1
		mov     dword ptr [esp + 4], 0
	L1:
		jmp     string_assign_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_assign_cstr(string *s, LPCSTR src)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    eax
		push    edx
		push    ecx
		push    edx
		call    strlen
		mov     dword ptr [esp + 16], eax
		pop     eax
		pop     ecx
		pop     edx
		jmp     string_assign_cstr_with_length
	}
}

string * __fastcall string_assign_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	size_t capacity = string_storage_capacity(s);
	if (length >= capacity)
	{
		bool is_internal;
		if (is_internal = (src >= string_begin(s) && src <= string_end(s)))
			src -= (size_t)string_begin(s);
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, length + 1);
		string_begin(s) = p;
		string_end_of_storage(s) = p + length + 1;
		if (is_internal)
			src += (size_t)string_begin(s);
	}
	string_end(s) = string_begin(s) + length;
	memmove(string_begin(s), src, length);
	*string_end(s) = '\0';
	return s;
}

string * __fastcall string_assign_char(string *s, const char c)
{
	size_t capacity = string_storage_capacity(s);
	if (capacity < 2)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, 2);
		string_begin(s) = p;
		string_end_of_storage(s) = p + 2;
	}
	*string_begin(s) = c;
	*(string_end(s) = string_begin(s) + 1) = '\0';
	return s;
}

string * __fastcall string_assign_wchar(string *s, const wchar_t c)
{
	size_t capacity = string_storage_capacity(s);
	if (capacity < 3)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, 3);
		string_begin(s) = p;
		string_end_of_storage(s) = p + 3;
	}
	*(wchar_t *)string_begin(s) = c;
	*(string_end(s) = string_begin(s) + 2) = '\0';
	return s;
}

static void __fastcall storage_append(string *s, size_t n)
{
	size_t length = string_length(s);
	size_t capacity = length + 1;
	capacity += max(capacity, n);
	char *p = (char *)allocator_reallocate(string_begin(s), string_storage_capacity(s), capacity);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + capacity;
}

string * __fastcall string_append_repeat_char(string *s, size_t n, char c)
{
	if (string_end(s) + n >= string_end_of_storage(s))
		storage_append(s, n);
	char *p = string_end(s);
	string_end(s) = p + n;
	memset(p, c, n);
	*string_end(s) = '\0';
	return s;
}

__declspec(naked) string * __fastcall string_append(string *s, const string *src)
{
	__asm
	{
		pop     eax
		push    eax
		push    eax
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		sub     eax, edx
		mov     dword ptr [esp + 4], eax
		jmp     string_append_cstr_with_length
	}
}

__declspec(naked) string * __fastcall string_append_range(string *s, LPCSTR first, LPCSTR last)
{
	__asm
	{
		sub     dword ptr [esp + 4], edx
		jnz     string_append_cstr_with_length
		mov     eax, ecx
		ret     4
	}
}

string * __fastcall string_append_cstr_with_length(string *s, LPCSTR src, size_t length)
{
	if (string_end(s) + length >= string_end_of_storage(s))
	{
		bool is_internal;
		if (is_internal = (src >= string_begin(s) && src <= string_end(s)))
			src -= (size_t)string_begin(s);
		storage_append(s, length);
		if (is_internal)
			src += (size_t)string_begin(s);
	}
	char *p = string_end(s);
	string_end(s) = p + length;
	memmove(p, src, length);
	*string_end(s) = '\0';
	return s;
}

string * __fastcall string_concat(string *s, const string *left, const string *right)
{
	size_t leftLength = string_length(left);
	size_t rightLength = string_length(right);
	size_t length = leftLength + rightLength;
	char *p = (char *)internal_allocate(length + 1);
	string_begin(s) = p;
	string_end(s) = p + length;
	string_end_of_storage(s) = p + length + 1;
	memcpy(p, string_begin(left), leftLength);
	memcpy(p + leftLength, string_begin(right), rightLength);
	*string_end(s) = '\0';
	return s;
}

void __fastcall string_reserve(string *s, size_t n)
{
	size_t capacity = string_storage_capacity(s);
	size_t require = n + 1;
	if (require != capacity)
	{
		size_t length = string_length(s);
		if (length > n)
			*(string_begin(s) + (length = n)) = '\0';
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, require);
		string_begin(s) = p;
		string_end(s) = p + length;
		string_end_of_storage(s) = p + require;
	}
}

void __fastcall string_resize(string *s, size_t n)
{
	char *end = string_begin(s) + n;
	if (end > string_end(s))
		string_append_repeat_char(s, end - string_end(s), '\0');
	else
		*(string_end(s) = end) = '\0';
}

void __fastcall string_storage_append(string *s, size_t n)
{
	if (string_end(s) + n >= string_end_of_storage(s))
		storage_append(s, n);
}

void __fastcall string_storage_resize(string *s, size_t n)
{
	char *p = string_begin(s) + n;
	if (p >= string_end_of_storage(s))
		storage_append(s, p - string_end(s));
}

void __fastcall string_shrink_to_fit(string *s)
{
	size_t capacity = string_storage_capacity(s);
	size_t size = string_length(s) + 1;
	if (size != capacity)
	{
		char *p = (char *)allocator_reallocate(string_begin(s), capacity, size--);
		string_begin(s) = p;
		string_end(s) = p += size;
		string_end_of_storage(s) = p + 1;
	}
}

#ifndef _M_IX86
string * __fastcall string_trim_left(string *s)
{
	char *first, *last, c;

	if ((first = string_begin(s)) < (last = string_end(s)))
	{
		while ((c = *(first++)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
		if (string_begin(s) >= --first)
			return s;
		if (last > first)
		{
			last -= (size_t)first;
			last += (size_t)memcpy(string_begin(s), first, (size_t)last);
		}
		else
		{
			last = string_begin(s);
		}
		*last = '\0';
	}
	else
	{
		if (last = first)
			*last = '\0';
	}
	string_end(s) = last;
	return s;
}
#else
#pragma function(memcpy)
__declspec(naked) string * __fastcall string_trim_left(string *s)
{
	__asm
	{
		#define s ecx

		push    esi
		mov     edx, dword ptr [ecx]
		mov     esi, dword ptr [ecx + 4]
		cmp     edx, esi
		jb      L1
		test    edx, edx
		jnz     L2
		jmp     L3

		align   16
	L1:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L1
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L1
		mov     eax, dword ptr [ecx]
		dec     edx
		cmp     eax, edx
		jae     L4
		sub     esi, edx
		jbe     L5
		push    ecx
		push    esi
		push    edx
		push    eax
		call    memcpy
		add     esi, eax
		mov     eax, dword ptr [esp + 12]
		mov     byte ptr [esi], '\0'
		mov     dword ptr [eax + 4], esi
		mov     esi, dword ptr [esp + 16]
		add     esp, 20
		ret

		align   16
	L2:
		mov     byte ptr [edx], '\0'
	L3:
		mov     dword ptr [ecx + 4], edx
	L4:
		mov     eax, ecx
		pop     esi
		ret

		align   16
	L5:
		mov     byte ptr [eax], '\0'
		mov     dword ptr [ecx + 4], eax
		mov     eax, ecx
		pop     esi
		ret

		#undef s
	}
}
#endif

#ifndef _M_IX86
string * __fastcall string_trim_right(string *s)
{
	char *first, *last, c;

	if ((first = string_begin(s)) < (last = string_end(s)))
	{
		while ((c = *(--last)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1)
			if (first >= last)
				goto TERMINATE_AT_FIRST;
		*(++last) = '\0';
	}
	else
	{
	TERMINATE_AT_FIRST:
		if (last = first)
			*last = '\0';
	}
	string_end(s) = last;
	return s;
}
#else
__declspec(naked) string * __fastcall string_trim_right(string *s)
{
	__asm
	{
		#define s ecx

		push    ecx
		mov     edx, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		cmp     edx, ecx
		jb      L2
		test    edx, edx
		jnz     L3
		jmp     L4

		align   16
	L1:
		cmp     edx, ecx
		jae     L3
	L2:
		mov     al, byte ptr [ecx - 1]
		dec     ecx
		cmp     al, ' '
		je      L1
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L1
		pop     eax
		inc     ecx
		mov     byte ptr [ecx], '\0'
		mov     dword ptr [eax + 4], ecx
		ret

		align   16
	L3:
		pop     eax
		mov     byte ptr [edx], '\0'
		mov     dword ptr [eax + 4], edx
		ret

		align   16
	L4:
		pop     eax
		mov     dword ptr [eax + 4], edx
		ret

		#undef s
	}
}
#endif

#ifndef _M_IX86
string * __fastcall string_trim(string *s)
{
	char *first, *last, c;

	if ((first = string_begin(s)) < (last = string_end(s)))
	{
		while ((c = *(first++)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
		if (last >= first)
		{
			while ((c = *(--last)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
			--first;
			++last;
			if (string_begin(s) < first)
			{
				last -= (size_t)first;
				last += (size_t)memcpy(string_begin(s), first, (size_t)last);
			}
		}
		else
		{
			last = string_begin(s);
		}
		*last = '\0';
	}
	else
	{
		if (last = first)
			*last = '\0';
	}
	string_end(s) = last;
	return s;
}
#else
#pragma function(memcpy)
__declspec(naked) string * __fastcall string_trim(string *s)
{
	__asm
	{
		#define s ecx

		push    esi
		mov     edx, dword ptr [ecx]
		mov     esi, dword ptr [ecx + 4]
		cmp     edx, esi
		jb      L1
		test    edx, edx
		jnz     L5
		jmp     L6

		align   16
	L1:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L1
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L1
		cmp     esi, edx
		jb      L4
		dec     edx
		jmp     L2

		align   16
	L2:
		mov     al, byte ptr [esi - 1]
		dec     esi
		cmp     al, ' '
		je      L2
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L2
		mov     eax, dword ptr [ecx]
		inc     esi
		cmp     eax, edx
		jae     L3
		sub     esi, edx
		push    ecx
		push    esi
		push    edx
		push    eax
		add     esi, eax
		call    memcpy
		mov     ecx, dword ptr [esp + 12]
		add     esp, 16
	L3:
		mov     byte ptr [esi], '\0'
		mov     dword ptr [ecx + 4], esi
		mov     eax, ecx
		pop     esi
		ret

		align   16
	L4:
		mov     edx, dword ptr [ecx]
	L5:
		mov     byte ptr [edx], '\0'
	L6:
		mov     dword ptr [ecx + 4], edx
		mov     eax, ecx
		pop     esi
		ret

		#undef s
	}
}
#endif

#ifndef _M_IX86
string * __fastcall string_trim_left_blank(string *s)
{
	char *first, *last, c;

	if ((first = string_begin(s)) < (last = string_end(s)))
	{
		while ((c = *(first++)) == ' ' || c == '\t');
		if (string_begin(s) >= --first)
			return s;
		if (last > first)
		{
			last -= (size_t)first;
			last += (size_t)memcpy(string_begin(s), first, (size_t)last);
		}
		else
		{
			last = string_begin(s);
		}
		*last = '\0';
	}
	else
	{
		if (last = first)
			*last = '\0';
	}
	string_end(s) = last;
	return s;
}
#else
#pragma function(memcpy)
__declspec(naked) string * __fastcall string_trim_left_blank(string *s)
{
	__asm
	{
		#define s ecx

		push    esi
		mov     edx, dword ptr [ecx]
		mov     esi, dword ptr [ecx + 4]
		cmp     edx, esi
		jb      L1
		test    edx, edx
		jnz     L2
		jmp     L3

		align   16
	L1:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		mov     eax, dword ptr [ecx]
		dec     edx
		cmp     eax, edx
		jae     L4
		sub     esi, edx
		jbe     L5
		push    ecx
		push    esi
		push    edx
		push    eax
		call    memcpy
		add     esi, eax
		mov     eax, dword ptr [esp + 12]
		mov     byte ptr [esi], '\0'
		mov     dword ptr [eax + 4], esi
		mov     esi, dword ptr [esp + 16]
		add     esp, 20
		ret

		align   16
	L2:
		mov     byte ptr [edx], '\0'
	L3:
		mov     dword ptr [ecx + 4], edx
	L4:
		mov     eax, ecx
		pop     esi
		ret

		align   16
	L5:
		mov     byte ptr [eax], '\0'
		mov     dword ptr [ecx + 4], eax
		mov     eax, ecx
		pop     esi
		ret

		#undef s
	}
}
#endif

#ifndef _M_IX86
string * __fastcall string_trim_right_blank(string *s)
{
	char *first, *last, c;

	if ((first = string_begin(s)) < (last = string_end(s)))
	{
		while ((c = *(--last)) == ' ' || c == '\t')
			if (first >= last)
				goto TERMINATE_AT_FIRST;
		*(++last) = '\0';
	}
	else
	{
	TERMINATE_AT_FIRST:
		if (last = first)
			*last = '\0';
	}
	string_end(s) = last;
	return s;
}
#else
__declspec(naked) string * __fastcall string_trim_right_blank(string *s)
{
	__asm
	{
		#define s ecx

		push    ecx
		mov     edx, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
		cmp     edx, ecx
		jb      L2
		test    edx, edx
		jnz     L3
		jmp     L4

		align   16
	L1:
		cmp     edx, ecx
		jae     L3
	L2:
		mov     al, byte ptr [ecx - 1]
		dec     ecx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		pop     eax
		inc     ecx
		mov     byte ptr [ecx], '\0'
		mov     dword ptr [eax + 4], ecx
		ret

		align   16
	L3:
		pop     eax
		mov     byte ptr [edx], '\0'
		mov     dword ptr [eax + 4], edx
		ret

		align   16
	L4:
		pop     eax
		mov     dword ptr [eax + 4], edx
		ret

		#undef s
	}
}
#endif

#ifndef _M_IX86
string * __fastcall string_trim_blank(string *s)
{
	char *first, *last, c;

	if ((first = string_begin(s)) < (last = string_end(s)))
	{
		while ((c = *(first++)) == ' ' || c == '\t');
		if (last >= first)
		{
			while ((c = *(--last)) == ' ' || c == '\t');
			--first;
			++last;
			if (string_begin(s) < first)
			{
				last -= (size_t)first;
				last += (size_t)memcpy(string_begin(s), first, (size_t)last);
			}
		}
		else
		{
			last = string_begin(s);
		}
		*last = '\0';
	}
	else
	{
		if (last = first)
			*last = '\0';
	}
	string_end(s) = last;
	return s;
}
#else
#pragma function(memcpy)
__declspec(naked) string * __fastcall string_trim_blank(string *s)
{
	__asm
	{
		#define s ecx

		push    esi
		mov     edx, dword ptr [ecx]
		mov     esi, dword ptr [ecx + 4]
		cmp     edx, esi
		jb      L1
		test    edx, edx
		jnz     L5
		jmp     L6

		align   16
	L1:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		cmp     esi, edx
		jb      L4
		dec     edx

		align   16
	L2:
		mov     al, byte ptr [esi - 1]
		dec     esi
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		mov     eax, dword ptr [ecx]
		inc     esi
		cmp     eax, edx
		jae     L3
		sub     esi, edx
		push    ecx
		push    esi
		push    edx
		push    eax
		add     esi, eax
		call    memcpy
		mov     ecx, dword ptr [esp + 12]
		add     esp, 16
	L3:
		mov     byte ptr [esi], '\0'
		mov     dword ptr [ecx + 4], esi
		mov     eax, ecx
		pop     esi
		ret

		align   16
	L4:
		mov     edx, dword ptr [ecx]
	L5:
		mov     byte ptr [edx], '\0'
	L6:
		mov     dword ptr [ecx + 4], edx
		mov     eax, ecx
		pop     esi
		ret

		#undef s
	}
}
#endif
