#pragma once

#include <windows.h>

#pragma pack(push, 1)
typedef struct _Deque_iterator_base
{
	void *_M_cur;
	void *_M_first;
	void *_M_last;
	void**_M_node;
} _Deque_iterator, bcb6_std_deque_iterator, *pbcb6_std_deque_iterator;
#pragma pack(pop)

#define bcb6_std_deque_iterator_less_than(__x, __y) \
	((__x)->_M_node == (__y)->_M_node && (__x)->_M_cur < (__y)->_M_cur || (__x)->_M_node < (__y)->_M_node)

#define _Deque_iterator_set_node(this, type, __new_node) \
	((this)->_M_last = (type*)((this)->_M_first = *((this)->_M_node = (__new_node))) + 1)

#define bcb6_std_deque_iterator_increment(this, type) \
	(++(type*)(this)->_M_cur == (this)->_M_last ? (_Deque_iterator_set_node(this, type, (this)->_M_node + 1), (this)->_M_cur = (this)->_M_first) : (this)->_M_cur)

#define bcb6_std_deque_iterator_decrement(this, type) \
	((this)->_M_cur = (type*)((this)->_M_cur == (this)->_M_first ? _Deque_iterator_set_node(this, type, (this)->_M_node - 1) : (this)->_M_cur) - 1)

#pragma pack(push, 1)
typedef struct _Deque_base
{
	_Deque_iterator _M_start;
	_Deque_iterator _M_finish;
	__int32 : 32;
	__int32 : 32;
	void    const **_M_map;
	__int32 : 32;
	__int32 : 32;
	__int32 : 32;
	size_t          _M_map_size;
	__int32 : 32;
} bcb6_std_deque, *pbcb6_std_deque;
#pragma pack(pop)

#define bcb6_std_deque_empty(this) ((this)->_M_finish._M_cur == (this)->_M_start._M_cur)
#define bcb6_std_deque_begin(this) ((this)->_M_start)
#define bcb6_std_deque_end(this)   ((this)->_M_finish)

#define bcb6_std_deque_size(this, type) \
	(size_t)(((this)->_M_finish._M_node - (this)->_M_start._M_node - 1) \
		+ ((type*)(this)->_M_finish._M_cur - (type*)(this)->_M_finish._M_first) \
		+ ((type*)(this)->_M_start._M_last - (type*)(this)->_M_start._M_cur))

#define bcb6_std_deque_back(this, type) \
	((this)->_M_finish._M_cur == (this)->_M_finish._M_first \
		? &((type*)(this)->_M_finish._M_node[-1])[1-1] \
		: &((type*)(this)->_M_finish._M_cur)[-1])

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_deque_iterator  deque_iterator;
typedef pbcb6_std_deque_iterator pdeque_iterator;
typedef bcb6_std_deque           deque;
typedef pbcb6_std_deque          pdeque;
#define deque_empty              bcb6_std_deque_empty
#define deque_begin              bcb6_std_deque_begin
#define deque_end                bcb6_std_deque_end
#define deque_size               bcb6_std_deque_size
#define deque_back               bcb6_std_deque_back
#define deque_iterator_decrement bcb6_std_deque_iterator_decrement
#define deque_iterator_increment bcb6_std_deque_iterator_increment
#define deque_iterator_less_than bcb6_std_deque_iterator_less_than
#define deque_ptr_pop_back       bcb6_std_deque_ptr_pop_back
#define deque_ptr_push_back      bcb6_std_deque_ptr_push_back
#define deque_dword_erase        bcb6_std_deque_ptr_erase
#endif

EXTERN_C void __fastcall bcb6_std_deque_ptr_pop_back(bcb6_std_deque *deque);
EXTERN_C void __fastcall bcb6_std_deque_ptr_push_back(bcb6_std_deque *deque, const void *const *value);
EXTERN_C void __stdcall  bcb6_std_deque_ptr_erase(bcb6_std_deque *deque, bcb6_std_deque_iterator *it);
