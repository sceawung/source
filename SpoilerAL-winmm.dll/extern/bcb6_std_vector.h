#pragma once

#include <windows.h>
#define typename byte
#define type     BYTE
#include "bcb6_std_vector_template.h"
#define typename dword
#define type     DWORD
#include "bcb6_std_vector_template.h"
#define typename size_t
#include "bcb6_std_vector_template.h"

#pragma pack(push, 1)
typedef struct _Vector_base
{
	LPVOID _M_start;
	LPVOID _M_finish;
	LPVOID allocator_type[2];
	LPVOID _M_end_of_storage;
	signed lutIndex;
} bcb6_std_vector, *pbcb6_std_vector;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _Bit_iterator_base
{
	unsigned int *_M_p;
	unsigned int  _M_offset;
} _Bit_iter;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _Bvector_base
{
	_Bit_iter     _M_start;
	_Bit_iter     _M_finish;
	LPCVOID const __chunk_allocator_type;
	ptrdiff_t     tblIndex;
	unsigned int *_M_end_of_storage;
    __int32 : 32;
} bcb6_std_bvector, *pbcb6_std_bvector;
#pragma pack(pop)

#define bcb6_std_bvector_size(Bvec) \
    (size_t)((Bvec._M_finish._M_p - Bvec._M_start._M_p << 5) + Bvec._M_finish._M_offset - Bvec._M_start._M_offset)

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_vector  vector;
typedef pbcb6_std_vector pvector;
typedef bcb6_std_bvector bvector;
#define bvector_size              bcb6_std_bvector_size
#define vector_ctor               bcb6_std_vector_ctor
#define vector_size               bcb6_std_vector_size
#define vector_size_by_type       bcb6_std_vector_size_by_type
#define vector_byte_size          bcb6_std_vector_byte_size
#define vector_capacity           bcb6_std_vector_capacity
#define vector_capacity_by_type   bcb6_std_vector_capacity_by_type
#define vector_byte_capacity      bcb6_std_vector_byte_capacity
#define vector_begin              bcb6_std_vector_begin
#define vector_end                bcb6_std_vector_end
#define vector_end_of_storage     bcb6_std_vector_end_of_storage
#define vector_empty              bcb6_std_vector_empty
#define vector_at                 bcb6_std_vector_at
#define vector_type_at            bcb6_std_vector_type_at
#define vector_offset             bcb6_std_vector_offset
#define vector_dtor               bcb6_std_vector_dtor
#define vector_shrink_to_fit      bcb6_std_vector_shrink_to_fit
#define vector_byte_reserve       bcb6_std_vector_byte_reserve
#define vector_byte_resize        bcb6_std_vector_byte_resize
#define vector_byte_resize_nofill bcb6_std_vector_byte_resize_nofill
#define vector_byte_push_back     bcb6_std_vector_byte_push_back
#define vector_dword_reserve      bcb6_std_vector_dword_reserve
#define vector_dword_push_back    bcb6_std_vector_dword_push_back
#define vector_reserve            bcb6_std_vector_reserve
#define vector_resize             bcb6_std_vector_resize
#define vector_resize_nofill      bcb6_std_vector_resize_nofill
#define vector_push_back          bcb6_std_vector_push_back
#define vector_pop_back           bcb6_std_vector_pop_back
#define vector_insert             bcb6_std_vector_insert
#define vector_insert_by_type     bcb6_std_vector_insert_by_type
#define vector_insert_range       bcb6_std_vector_insert_range
#define vector_erase              bcb6_std_vector_erase
#define vector_clear              bcb6_std_vector_clear
#endif

#define bcb6_std_vector_ctor(v)                   ((v)->_M_end_of_storage = (v)->_M_finish = (v)->_M_start = NULL)
#define bcb6_std_vector_size(v)                   (size_t)((v)->_M_finish - (v)->_M_start)
#define bcb6_std_vector_size_by_type(v, type)     (size_t)((type *)(v)->_M_finish - (type *)(v)->_M_start)
#define bcb6_std_vector_byte_size(v)              bcb6_std_vector_size_by_type(v, char)
#define bcb6_std_vector_capacity(v)               (size_t)((v)->_M_end_of_storage - (v)->_M_start)
#define bcb6_std_vector_capacity_by_type(v, type) (size_t)((type *)(v)->_M_end_of_storage - (type *)(v)->_M_start)
#define bcb6_std_vector_byte_capacity(v)          bcb6_std_vector_capacity_by_type(v, char)
#define bcb6_std_vector_begin(v)                  (v)->_M_start
#define bcb6_std_vector_end(v)                    (v)->_M_finish
#define bcb6_std_vector_end_of_storage(v)         (v)->_M_end_of_storage
#define bcb6_std_vector_empty(v)                  ((v)->_M_finish == (v)->_M_start)
#define bcb6_std_vector_at(v, index)              (v)->_M_start[index]
#define bcb6_std_vector_type_at(v, type, index)   ((type *)(v)->_M_start)[index]
#define bcb6_std_vector_offset(v, index)          (void *)&bcb6_std_vector_type_at(v, char, index)
#define bcb6_std_vector_clear(v)                  ((v)->_M_finish = (v)->_M_start)

EXTERN_C void __fastcall bcb6_std_vector_dtor(void *v);
EXTERN_C void __fastcall bcb6_std_vector_shrink_to_fit(void *v);
EXTERN_C void __fastcall bcb6_std_vector_byte_reserve(void *v, size_t n);
EXTERN_C void __fastcall bcb6_std_vector_byte_resize(void *v, size_t n);
EXTERN_C void __fastcall bcb6_std_vector_byte_resize_nofill(void *v, size_t n);
EXTERN_C void __fastcall bcb6_std_vector_byte_push_back(void *v, BYTE value);
EXTERN_C void __fastcall bcb6_std_vector_dword_reserve(void *s, size_t n);
EXTERN_C void __fastcall bcb6_std_vector_dword_push_back(void *v, DWORD value);

#define bcb6_std_vector_reserve(v, n) \
	bcb6_std_vector_byte_reserve(v, (n) * sizeof(*(v)->_M_start))

#define bcb6_std_vector_resize(v, n) \
	bcb6_std_vector_byte_resize(v, (n) * sizeof(*(v)->_M_start))

#define bcb6_std_vector_resize_nofill(v, n) \
	bcb6_std_vector_byte_resize_nofill(v, (n) * sizeof(*(v)->_M_start))

#define bcb6_std_vector_push_back(v, x)                                                           \
do                                                                                                \
{                                                                                                 \
    bcb6_std_vector_byte_resize_nofill(v, bcb6_std_vector_byte_size(v) + sizeof(*(v)->_M_start)); \
    *((v)->_M_finish - 1) = x;                                                                    \
} while (0)

#define bcb6_std_vector_pop_back(v) \
do                                  \
{                                   \
    if (!bcb6_std_vector_empty(v))  \
        (v)->_M_finish--;           \
} while (0)

#define bcb6_std_vector_insert(v, position, x)                                                    \
do                                                                                                \
{                                                                                                 \
    size_t __index = (position) - (v)->_M_start;                                                  \
    size_t __backward = (char *)(v)->_M_finish - (char *)(position);                              \
    bcb6_std_vector_byte_resize_nofill(v, bcb6_std_vector_byte_size(v) + sizeof(*(v)->_M_start)); \
    memmove((v)->_M_start + __index + 1, (v)->_M_start + __index, __backward);                    \
    (v)->_M_start[__index] = x;                                                                   \
} while (0)

#define bcb6_std_vector_insert_by_type(v, type, position, x)                              \
do                                                                                        \
{                                                                                         \
    bcb6_std_vector * __restrict __v = (bcb6_std_vector *)(v);                            \
    void * __restrict __position = position;                                              \
    size_t __forward = (char *)__position - (char *)__v->_M_start;                        \
    size_t __backward = (char *)__v->_M_finish - (char *)__position;                      \
    bcb6_std_vector_byte_resize_nofill(v, bcb6_std_vector_byte_size(__v) + sizeof(type)); \
    type *__element = (type *)((char *)__v->_M_start + __forward);                        \
    memmove(__element + 1, __element, __backward);                                        \
    *__element = x;                                                                       \
} while (0)

#define bcb6_std_vector_insert_range(v, position, first, last)                    \
do                                                                                \
{                                                                                 \
    size_t __forward = (char *)(position) - (char *)(v)->_M_start;                \
    size_t __backward = (char *)(v)->_M_finish - (char *)(position);              \
    size_t __size = (char *)(last) - (char *)(first);                             \
    bcb6_std_vector_byte_resize_nofill(v, bcb6_std_vector_byte_size(v) + __size); \
    char *__position = (char *)(v)->_M_start + __forward;                         \
    memmove(__position + __size, __position, __backward);                         \
    memcpy(__position, first, __size);                                            \
} while (0)

#define bcb6_std_vector_erase(v, position) \
    memcpy(position, (position) + 1, (size_t)--(v)->_M_finish - (size_t)(position))
