#include <windows.h>
#include "intrinsic.h"

#if !defined(__BORLANDC__)
#define LOCAL_MEMORY_SUPPORT 1
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"
#include "TSSGAttributeElement.h"
#include "TSSGAttributeSelector.h"
#include "TSSGSubject.h"
typedef vector vector_TSSGAttributeElement;
#else
#pragma warn -8060
#define vector_string                                                vector<string>
#define vector_TSSGAttributeElement                                  vector<TSSGAttributeElement *>
#define string                                                       string
#define string_begin(s)                                              (s)->begin()
#define string_end(s)                                                (s)->end()
#define string_length(s)                                             (s)->length()
#define TSSGAttributeElement_GetType(SSGAttributeElement)            (SSGAttributeElement)->GetType()
#define TIO_FEPAttribute_GetInputCode(IO_FEPAttribute)               (string *)((LPBYTE)(IO_FEPAttribute) + 8)
#define TIO_FEPAttribute_GetOutputCode(IO_FEPAttribute)              ((string *)((LPBYTE)(IO_FEPAttribute) + 8) + 1)
#define TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector) attributeSelector->GetNowAtteributeVec()
#define TSSGSubject_GetAttribute(SSGS)                               (SSGS)->GetAttribute()
#endif

extern HANDLE hHeap;

void __stdcall ReplaceDefineByAttributeVector(vector_TSSGAttributeElement *attributes, string *line);

size_t __stdcall ReplaceDefineByHeap(vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity)
{
	LPSTR p;
	char  c;

	p = *line;
#if LOCAL_MEMORY_SUPPORT
	if (*p == '_')
		do
			c = *(++p);
		while (__intrinsic_isspace(c));
	if (*p == 'L' && *(++p) == '{' && !__intrinsic_isspace(p[1]))
		goto INSERT_SPACE;
#endif
	while (c = *p)
	{
		if (!__intrinsic_isleadbyte(c))
		{
#if !LOCAL_MEMORY_SUPPORT
			if (c == '{')
			{
#else
			switch (c)
			{
			case '<':
				if ((c = *(++p)) != '#' && c != '@')
					continue;
				/* FALLTHROUGH */
			case '(':
			case ',':
				do
				{
					c = *(++p);
				} while (__intrinsic_isspace(c));
				if (c == '_')
					do
					{
						c = *(++p);
					} while (__intrinsic_isspace(c));
				if (c != 'L')
					continue;
				if (*(++p) != '{' || __intrinsic_isspace(p[1]))
					continue;
			INSERT_SPACE:
				if (++length == capacity)
				{
					LPSTR mem;

					if (!(capacity <<= 1))
						goto FAILED;
					mem = (LPSTR)HeapReAlloc(hHeap, 0, *line, capacity);
					if (!mem)
						goto FAILED;
					p += mem - *line;
					*line = mem;
				}
				memmove(p + 1, p, *line + length - p);
				*(p++) = ' ';
			case '{':
#endif
				if (!__intrinsic_isspace(p[1]))
				{
					LPCSTR key;
					size_t keyLength;
					LPCSTR end;
					COORD  coord = TSSGAttributeElement_GetViaCoord(atDEFINE, attributes).dwFontSize;

					end = key = p;
					while ((c = *(++end)) && c != '}');
					if (!c)
						goto NESTED_BREAK;
					keyLength = end - p + 1;

					for (TDefineAttribute **it,
						 **base = &vector_type_at(attributes, TDefineAttribute *, coord.Y),
						 **apex = base + coord.X;
						   base < apex; )
					{
						const string *outputCode;
						size_t       valueLength;
						LPCSTR       value;
						ptrdiff_t    diff;
						signed       hl;

						it = &base[apex - base >> 1];
						hl = strncmp(string_c_str(TIO_FEPAttribute_GetInputCode(*it)), key, keyLength);
						if (hl < 0)
						{
							base = it + 1;
							continue;
						}
						else if (hl > 0)
						{
							apex = it;
							continue;
						}

						outputCode = TIO_FEPAttribute_GetOutputCode(*it);
						valueLength = string_length(outputCode);
						diff = valueLength - keyLength;
						if (diff)
						{
							size_t n;
							LPCSTR src, end;

							n = length + diff;
							if (n >= capacity)
							{
								LPSTR mem;

								do
								{
									if (!(capacity <<= 1))
										goto FAILED;
								} while (n >= capacity);
								mem = (LPSTR)HeapReAlloc(hHeap, 0, *line, capacity);
								if (!mem)
									goto FAILED;
								p += mem - *line;
								*line = mem;
							}
							src = p + keyLength;
							end = *line + length;
							length = n;
							memmove(p + valueLength, src, end - src + 1);
						}
						value = string_begin(outputCode);
						memcpy(p, value, valueLength);
						goto NESTED_CONTINUE;
					}
				}
			}
		}
		else if (!*(++p))
			break;
		p++;
	NESTED_CONTINUE:
		;
	}
NESTED_BREAK:
	return p - *line;
FAILED:
	return -1;
}

string * __stdcall ReplaceDefine(TSSGAttributeSelector *const attributeSelector, string *const line)
{
	vector_TSSGAttributeElement *const attributes = TSSGAttributeSelector_GetNowAtteributeVec(attributeSelector);
	if (attributes)
		ReplaceDefineByAttributeVector(attributes, line);
	return line;
}

void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line)
{
	vector_TSSGAttributeElement *attributes;

	attributes = TSSGSubject_GetAttribute(SSGS);
	if (attributes == NULL)
		return;
	ReplaceDefineByAttributeVector(attributes, line);
}

void __stdcall ReplaceDefineByAttributeVector(vector_TSSGAttributeElement *attributes, string *line)
{
	LPCSTR        begin;
	size_t        length, capacity;
	unsigned long bits;
	LPSTR         buffer;

	begin = string_begin(line);
	length = string_end(line) - begin;
	if ((INT_PTR)length < 0)
		return;
#ifndef _WIN64
	if (_BitScanReverse(&bits, length))
#else
	if (_BitScanReverse64(&bits, length))
#endif
		capacity = (size_t)1 << (bits + 1);
	else
		capacity = 1;
	buffer = (LPSTR)HeapAlloc(hHeap, 0, capacity);
	if (buffer != NULL)
	{
		memcpy(buffer, begin, length + 1);
		length = ReplaceDefineByHeap(attributes, &buffer, length, capacity);
		if (length != -1)
		{
#if defined(__BORLANDC__)
			*line = buffer;
#else
			string_assign_cstr_with_length(line, buffer, length);
#endif
		}
		HeapFree(hHeap, 0, buffer);
	}
}

size_t __stdcall ByteArrayReplaceDefineByHeap(vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity)
{
	LPSTR p;
	char  c, prev;

	p = *line;
	prev = '\0';
	while (c = *p)
	{
		if (!__intrinsic_isleadbyte(c))
		{
			switch (c)
			{
			case '$':
				if (!*(++p))
					goto NESTED_BREAK;
				for (; ; )
				{
					if (!(c = *(++p)))
						goto NESTED_BREAK;
					if (!__intrinsic_isleadbyte(c))
					{
						if (c != '\\')
						{
							if (c != '$' || *(p + 1) != '$')
								continue;
							p++;
							break;
						}
						if (!(c = *(++p)))
							goto NESTED_BREAK;
						if (!__intrinsic_isleadbyte(c))
							continue;
					}
					if (!*(++p))
						goto NESTED_BREAK;
				}
				prev = '$';
				break;
			case '\\':
				if (!(c = *(++p)))
					goto NESTED_BREAK;
				if (__intrinsic_isleadbyte(c))
				{
					if (!*(++p))
						goto NESTED_BREAK;
				}
				prev = '\0';
				break;
			case '{':
				if (prev != '*' && !__intrinsic_isspace(p[1]))
				{
					LPCSTR key;
					size_t keyLength;
					LPCSTR end;
					COORD  coord = TSSGAttributeElement_GetViaCoord(atDEFINE, attributes).dwFontSize;

					end = key = p;
					while ((c = *(++end)) && c != '}');
					if (!c)
						goto NESTED_BREAK;
					keyLength = end - p + 1;

					for (TIO_FEPAttribute **it,
						 **base = &vector_type_at(attributes, TIO_FEPAttribute *, coord.Y),
						 **apex = base + coord.X;
						   base < apex; )
					{
						const string *outputCode;
						size_t       valueLength;
						LPCSTR       value;
						ptrdiff_t    diff;
						signed       hl;

						it = &base[apex - base >> 1];
						hl = strncmp(string_c_str(TIO_FEPAttribute_GetInputCode(*it)), key, keyLength);
						if (hl < 0)
						{
							base = it + 1;
							continue;
						}
						else if (hl > 0)
						{
							apex = it;
							continue;
						}

						outputCode = TIO_FEPAttribute_GetOutputCode(*it);
						valueLength = string_length(outputCode);
						diff = valueLength - keyLength;
						if (diff)
						{
							size_t n;
							LPCSTR src, end;

							n = length + diff;
							if (n >= capacity)
							{
								LPSTR mem;

								do
								{
									if (!(capacity <<= 1))
										goto FAILED;
								} while (n >= capacity);
								mem = (LPSTR)HeapReAlloc(hHeap, 0, *line, capacity);
								if (!mem)
									goto FAILED;
								p += mem - *line;
								*line = mem;
							}
							src = p + keyLength;
							end = *line + length;
							length = n;
							memmove(p + valueLength, src, end - src + 1);
						}
						value = string_begin(outputCode);
						memcpy(p, value, valueLength);
						goto NESTED_CONTINUE;
					}
					prev = '{';
					break;
				}
				/* FALLTHROUGH */
			default:
				prev = c;
				break;
			}
		}
		else
		{
			prev = '\0';
			if (!*(++p))
				break;
		}
		p++;
	NESTED_CONTINUE:;
	}
NESTED_BREAK:
	return p - *line;
FAILED:
	return -1;
}

void __stdcall ByteArrayReplaceDefine(TSSGSubject *SSGS, string *line)
{
	vector_TSSGAttributeElement *attributes;
	LPCSTR                      begin;
	size_t                      length, capacity;
	unsigned long               bits;
	LPSTR                       buffer;

	attributes = TSSGSubject_GetAttribute(SSGS);
	if (attributes == NULL)
		return;
	begin = string_begin(line);
	length = string_end(line) - begin;
	if ((INT_PTR)length <= 0)
		return;
#ifndef _WIN64
	_BitScanReverse(&bits, length);
#else
	_BitScanReverse64(&bits, length);
#endif
	capacity = (size_t)1 << (bits + 1);
	buffer = (LPSTR)HeapAlloc(hHeap, 0, capacity);
	if (buffer != NULL)
	{
		memcpy(buffer, begin, length + 1);
		length = ByteArrayReplaceDefineByHeap(attributes, &buffer, length, capacity);
		if (length != -1)
		{
#if defined(__BORLANDC__)
			*line = buffer;
#else
			string_assign_cstr_with_length(line, buffer, length);
#endif
		}
		HeapFree(hHeap, 0, buffer);
	}
}

#if defined(__BORLANDC__)
#undef vector_string
#undef vector_TSSGAttributeElement
#undef string
#undef string_begin
#undef string_end
#undef string_length
#undef TSSGAttributeElement_GetType
#undef TIO_FEPAttribute_GetInputCode
#undef TIO_FEPAttribute_GetOutputCode
#undef TSSGAttributeSelector_GetNowAtteributeVec
#undef TSSGSubject_GetAttribute
#endif
