#pragma once

#include <windows.h>
#include <intrin.h>
#include "bcb6_std_string.h"

#pragma pack(push, 1)
typedef struct _List_node
{
	struct _List_node *_M_next;
	struct _List_node *_M_prev;
	BYTE               _M_data[];
} *_List_iterator, bcb6_std_list_node, *pbcb6_std_list_node, *bcb6_std_list_iterator, **pbcb6_std_list_iterator;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _List_base
{
	__int32 : 32;
	__int32 : 32;
	struct _List_node *_M_node;
	__int32 : 32;
} bcb6_std_list, *pbcb6_std_list;
#pragma pack(pop)

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_list_node      list_node;
typedef pbcb6_std_list_node     plist_node;
typedef bcb6_std_list_iterator  list_iterator;
typedef pbcb6_std_list_iterator plist_iterator;
typedef bcb6_std_list           list;
typedef pbcb6_std_list          plist;
#define list_begin              bcb6_std_list_begin
#define list_end                bcb6_std_list_end
#define list_front              bcb6_std_list_front
#define list_back               bcb6_std_list_back
#define list_iterator_increment bcb6_std_list_iterator_increment
#define list_iterator_decrement bcb6_std_list_iterator_decrement
#define list_erase              bcb6_std_list_erase
#endif

#define bcb6_std_list_begin(list) (list)->_M_node->_M_next
#define bcb6_std_list_end(list)   (list)->_M_node

#define bcb6_std_list_front(list) ((void *)bcb6_std_list_begin(list)->_M_data)
#define bcb6_std_list_back(list)  ((void *)bcb6_std_list_end(list)->_M_prev->_M_data)

#define bcb6_std_list_iterator_increment(it) ((it) = (it)->_M_next)
#define bcb6_std_list_iterator_decrement(it) ((it) = (it)->_M_prev)

EXTERN_C bcb6_std_list_iterator __fastcall bcb6_std_list_erase(bcb6_std_list_iterator it);

typedef struct _List_node *(__cdecl *const _STL_list_M_create_node)(struct _List_base *self, const void *__x);

static _List_iterator __forceinline _STL_list_insert(struct _List_node *const __n, struct _List_node *const __tmp)
{
	struct _List_node *const __p = __n->_M_prev;
	__tmp->_M_next = __n;
	__tmp->_M_prev = __p;
	__p->_M_next = __tmp;
	__n->_M_prev = __tmp;
	return __tmp;
}

#define list_push_back(self, __x) _STL_list_insert(bcb6_std_list_end(self), ((_STL_list_M_create_node)_Generic(__x\
	, struct _TProcessAccessElementMaskData **:0x0050E758\
	, struct pair_byte_string                *:0x0050123C\
	, struct _TAdjustmentAttribute    *const *:0x0044E3EC\
	, unsigned long                          *:0x0044E3EC\
))(self, __x))

struct pair_byte_string
{
	enum Repeat first : 0x08;
	unsigned  __int32 : 0x18;
	unsigned  __int32 : 0x20;
	bcb6_std_string   second;
};
