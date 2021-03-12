#pragma once

#include <windows.h>
#include "bcb6_std_string.h"

#pragma pack(push, 1)
typedef struct _Rb_tree_node
{
	bool                  _M_color;
	__int8 : 8;
	__int16 : 16;
	struct _Rb_tree_node *_M_parent;
	struct _Rb_tree_node *_M_left;
	struct _Rb_tree_node *_M_right;
	BYTE                  _M_value_field[];
} *_Rb_tree_iterator, bcb6_std_map_node, *bcb6_std_map_iterator, **pbcb6_std_map_iterator;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _Rb_tree
{
	__int32 : 32;
	__int32 : 32;
	struct _Rb_tree_node *_M_header;
	__int32 : 32;
	size_t                _M_node_count;
	__int32 : 32;
	const void     *const _M_key_compare[2];
} bcb6_std_map, *pbcb6_std_map;
#pragma pack(pop)

EXTERN_C void(__cdecl *const _Rb_global_Rebalance)(_Rb_tree_iterator __x, struct _Rb_tree_node **__root);
EXTERN_C _Rb_tree_iterator(__cdecl *const _Rb_global_Rebalance_for_erase)(
	_Rb_tree_iterator      __z,
	struct _Rb_tree_node **__root,
	struct _Rb_tree_node **__leftmost,
	struct _Rb_tree_node **__rightmost);
EXTERN_C _Rb_tree_iterator(__cdecl * const _Rb_global_M_increment)(_Rb_tree_iterator);
EXTERN_C _Rb_tree_iterator(__cdecl * const _Rb_global_M_decrement)(_Rb_tree_iterator);

#define tree_erase_sans_delete(this, it) \
	((this)->_M_node_count--, _Rb_global_Rebalance_for_erase(it, &(this)->_M_header->_M_parent, &(this)->_M_header->_M_left, &(this)->_M_header->_M_right))

EXTERN_C bcb6_std_map_iterator(__cdecl *const bcb6_std_map_find)(bcb6_std_map *this, const unsigned long *key);
EXTERN_C bcb6_std_map_iterator(__cdecl *const bcb6_std_map_lower_bound)(bcb6_std_map *this, const unsigned long *key);

EXTERN_C bcb6_std_map_iterator(__cdecl *const bcb6_std_map_string_find)(bcb6_std_map *this, const bcb6_std_string *key);
EXTERN_C bcb6_std_map_iterator(__cdecl *const bcb6_std_map_string_lower_bound)(bcb6_std_map *this, const bcb6_std_string *key);

EXTERN_C void(__cdecl *const bcb6_std_ulong_pair_uu_insert)(bcb6_std_map_iterator *it, bcb6_std_map *this, bcb6_std_map_iterator pos, const void *pair);
EXTERN_C void(__cdecl *const bcb6_std_string_double_insert)(bcb6_std_map_iterator *it, bcb6_std_map *this, bcb6_std_map_iterator pos, const void *pair);

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_map_iterator  map_iterator;
typedef pbcb6_std_map_iterator pmap_iterator;
typedef bcb6_std_map           map;
typedef pbcb6_std_map          pmap;
#define map_find(map,k)        _Generic(k, const struct _String_base *: bcb6_std_map_string_find, default: bcb6_std_map_find)(map, k)
#define map_lower_bound(map,k) _Generic(k, struct _String_base *: bcb6_std_map_string_lower_bound, const unsigned long *: bcb6_std_map_lower_bound)(map, k)
#define map_dword_dw_dw_insert bcb6_std_ulong_pair_uu_insert
#define map_string_quad_insert bcb6_std_string_double_insert
#define map_begin              bcb6_std_map_begin
#define map_end                bcb6_std_map_end
#define pair_first             bcb6_std_pair_first
#define pair_second            bcb6_std_pair_second
#define pair_second_aligned    bcb6_std_pair_second_aligned
#define map_iterator_increment bcb6_std_map_iterator_increment
#define map_iterator_decrement bcb6_std_map_iterator_decrement
#define map_erase              bcb6_std_map_erase
#endif

#define bcb6_std_map_begin(this) (this)->_M_header->_M_left
#define bcb6_std_map_end(this)   (this)->_M_header

#define bcb6_std_pair_first(it) (LPVOID)(it)->_M_value_field
#define bcb6_std_pair_second(it, first) (LPVOID)((it)->_M_value_field + sizeof(first))
#define bcb6_std_pair_second_aligned(it, first) (LPVOID)((it)->_M_value_field + ((sizeof(first) + __alignof(double) - 1) & -(signed)__alignof(double)))

#define bcb6_std_map_iterator_increment(it) ((it) = _Rb_global_M_increment(it))
#define bcb6_std_map_iterator_decrement(it) ((it) = _Rb_global_M_decrement(it))

EXTERN_C void __stdcall bcb6_std_map_erase(bcb6_std_map *map, bcb6_std_map_iterator it);
