#ifdef typename

#ifndef CONCAT
#define CONCATIFY(a, b) a##b
#define CONCAT(a, b)    CONCATIFY(a, b)
#endif

#define VECTOR_NAME           CONCAT(vector_, typename)
#define PVECTOR_NAME          CONCAT(p, VECTOR_NAME)
#define BCB6_STD_VECTOR_NAME  CONCAT(bcb6_std_, VECTOR_NAME)
#define PBCB6_STD_VECTOR_NAME CONCAT(p, BCB6_STD_VECTOR_NAME)

#ifndef type
#define type typename
#endif

#pragma pack(push, 1)
typedef struct CONCAT(_Vector_, typename)
{
	type *_M_start;
	type *_M_finish;
	void *allocator_type[2];
	type *_M_end_of_storage;
	INT32 lutIndex;
} BCB6_STD_VECTOR_NAME, *PBCB6_STD_VECTOR_NAME;
#pragma pack(pop)

#ifdef USING_NAMESPACE_BCB6_STD
typedef BCB6_STD_VECTOR_NAME  VECTOR_NAME;
typedef PBCB6_STD_VECTOR_NAME PVECTOR_NAME;
#endif

#undef typename
#undef type
#undef VECTOR_NAME
#undef PVECTOR_NAME
#undef BCB6_STD_VECTOR_NAME
#undef PBCB6_STD_VECTOR_NAME

#endif
