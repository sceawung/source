#ifndef _RTL_H_
#define _RTL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

extern
#ifdef _DLL
__declspec(dllimport) void
#else
DWORD
#endif
__InitExceptBlockLDTC
#ifdef _DLL
()
#endif
;

#ifndef __BORLANDC__
/*
 *      C/C++ Run Time Library - Version 2.0
 *
 *      Copyright (c) 1993, 1996 by Borland International
 *      All Rights Reserved.
 *
 */

// We want "C" exception handling
#define EXCEPT_C

#define _ss
#define __far

// We want the exception handling structures to be 1 byte aligned,
// the compiler excepts this.
#pragma pack(push, 1)

#define TPDST  __far                // type-id's and such are always 'far'
struct  TPDST  tpid;                // type descriptor structure
typedef struct tpid  TPDST *tpidPtr;
// tpMask flags
enum flagsType
{
	TM_IS_STRUCT   = 0x0001,
	TM_IS_CLASS    = 0x0002,
	TM_IS_PTR      = 0x0010,
	TM_IS_REF      = 0x0020,
	TM_IS_VOIDPTR  = 0x0040,
	TM_LOCALTYPE   = 0x0080,
	TM_IS_CONST    = 0x0100,
	TM_IS_VOLATILE = 0x0200,
	TM_IS_ARRAY    = 0x0400,
};
// Class flags ('tpcFlags')
enum flagsClass
{
	CF_HAS_CTOR    = 0x00000001,
	CF_HAS_DTOR    = 0x00000002,
	CF_HAS_BASES   = 0x00000004,
	CF_HAS_VBASES  = 0x00000008,
	CF_HAS_VTABPTR = 0x00000010,
	CF_HAS_VIRTDT  = 0x00000020,
	CF_HAS_RTTI    = 0x00000040,
	CF_DELPHICLASS = 0x00000080,
	CF_HAS_FARVPTR = 0x00001000,
	CF_HAS_GUID    = 0x00002000,// The tpcGuid field is valid and filled in
};
// Variable cleanup descriptor flags
enum flagsClean
{
	DTCVF_PTRVAL   = 0x0001,
	DTCVF_DELPTR   = 0x0002,
	DTCVF_STACKVAR = 0x0004,
	DTCVF_DELETE   = 0x0008,
	DTCVF_RETVAL   = 0x0010,
	DTCVF_RETCTX   = 0x0020,
	DTCVF_VECCNT   = 0x0040,
	DTCVF_DTCADJ   = 0x0100,
	DTCVF_THROWCTX = 0x0200,// Used internally by the compiler?
	DTCVF_THISCTX  = 0x0400,
	DTCVF_WITHVT   = 0x1000,
};
struct tpid                         // type descriptor
{
	unsigned                tpSize; // size of type in bytes
	union
	{
		struct
		{
			unsigned short  tpMask; // attribute bits
			unsigned short  tpName; // offset of start of the zero terminated name
									// where offset is relative from tpid base
		};
		enum flagsType      _tpMask : 16;
	};
	union
	{
		struct tpPtr
		{
			tpidPtr         tppBaseType;    // pointed-to type
		} tpPtr;

		struct tpArr
		{
			tpidPtr         tpaElemType;    // element type
			unsigned long   tpaElemCount;   // element count
		} tpArr;

		struct tpClass
		{
			/*un*/signed    tpcVptrOffs;    // offset of vtable pointer
			enum flagsClass tpcFlags;       // more flags

			// Only valid if (tpMask & TM_IS_CLASS)

			unsigned short  tpcBaseList;    // offset of non-virt base list,
											// where offset is relative from tpid base
			unsigned short  tpcVbasList;    // offset of virtual  base list
											// where offset is relative from tpid base

			void    __far * tpcDlOpAddr;    // operator delete   addr
			unsigned short  tpcDlOpMask;    // operator delete   convention

			unsigned short  tpcDaOpMask;    // operator delete[] convention
			void    __far * tpcDaOpAddr;    // operator delete[] addr


			// Only valid if (tpcFlags & CF_HAS_DTOR), implies also (tpMask & TM_IS_CLASS)

			unsigned        tpcDtorCount;   // dtor count - total
			unsigned        tpcNVdtCount;   // dtor count - non-virtual

			void    __far * tpcDtorAddr;    // destructor addr
			unsigned short  tpcDtorMask;    // destructor convention

			unsigned short  tpcDtMembers;   // offset of list of destructible members
											// where offset is relative from tpid base

			// The following field is only there is the CF_HAS_GUID flag
			// is turned on.   To make it worse, you can NOT just access
			// this field by name, you HAVE to go through the type_info::guid
			// function in xxtype.cpp, which does all the magic to determine
			// where this field is located exactly.

			GUID            guid;
		} tpClass;
	};
	// Following is the zero terminated name, padded with zero's to
	// the next dword boundary.

	// Optionally (if tpMask & TM_IS_CLASS), we have next:
	//  - non-virtual base list, terminated by a null pointer
	//  -     virtual base list, terminated by a null pointer

	// Optionally (if tpcFlags & CF_HAS_DTOR), we have next:
	//  - list of destructible members, terminated by a null pointer
};
typedef EXCEPTION_ROUTINE   *   catchPtrTP;
typedef struct  ERRbc   _ss *   PREGISTRATION_RECORD;
#define CPP_EXCEPT_CODE 0x0EEFFACEul// use something better, OK?
#define PAS_EXCEPT_CODE 0x0EEDFADE
typedef struct  _EXCEPTION_POINTERS EXCEPTION_PTRS, *PEXCEPTION_PTRS;
struct  DTT
{
	tpidPtr         dttType;
	enum flagsClean dttFlags;
	union
	{
		/*un*/signed    dttFrameOffs;
		void    __far * dttAddress;
	};
};
typedef enum excBlock
{
	XB_FINALLY,                     /* 0 try/finally        ("C") */
	XB_EXCEXP,                      /* 1 try/except(expr )  ("C") */
	XB_EXCCNS,                      /* 2 try/except(const)  ("C") */
	XB_TRYCPP,                      /* 3 try                (C++) */
	XB_CATCH,                       /* 4 catch              (C++) */
	XB_DEST,                        /* 5 destructor cleanup (C++) */
} excBlockKind;
typedef unsigned int            dtorCntType;
typedef struct
{
	tpidPtr TPDST * xtThrowLst;             // throw-list address or NULL
	/*un*/signed    xtBPoffs;               // BP frame offset of ERR
	union
	{
		unsigned    xtTable[];              // context table follows ...
		struct xtTable
		{
			unsigned     outer : 16;
			excBlockKind kind  : 16;
			union
			{
				struct XB_DEST
				{
					unsigned     dtcMin;
					struct DTT * dttAdr;
				};
			};
		} _ctxTbl[];
	};
} EXCTAB, TPDST * EXCTABPTR;
enum flagsError
{
	EF_DELETETHIS = 0x01,// set if we need to delete 'this'
};
typedef struct  ERRbc
{
	PREGISTRATION_RECORD    ERRcNext;       // next registration record
	catchPtrTP              ERRcCatcher;    // address of handler
	EXCTABPTR               ERRcXtab;       // addr of table of handlers
	void     _ss *          ERRcSPsv;       // saved (E)SP value
	unsigned short          ERRcCCtx;       // current context
#ifdef  EXCEPT_C
	struct
	{
		unsigned short  ERRcUnwind;     // unwinding flag
		NTSTATUS        ERRcExcCode;    // exception code
		PEXCEPTION_PTRS ERRcExcInfo;    // exception info
	} ERRcInfo;
#endif
	dtorCntType     ERRcInitDtc;            // initial destructor count
	enum flagsError ERRflags : 8;           // flags for hacks - see EF_*
} REGREC_BC, _ss *PREGREC_BC;

#undef TPDST

#pragma pack(pop)

#undef __far
#undef _ss

#undef EXCEPT_C

#endif

#endif	// _RTL_H_
