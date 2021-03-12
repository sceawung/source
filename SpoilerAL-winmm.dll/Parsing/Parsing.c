#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE

#define _strspnp inline_strspnp
#define _wcsspnp inline_wcsspnp
#include <tchar.h>
#undef _strspnp
#undef _wcsspnp
#ifdef __cplusplus
extern "C" {
#endif
extern char * __cdecl _strspnp(const char *string, const char *control);
extern wchar_t * _wcsspnp(const wchar_t *string, const wchar_t *control);
#ifdef __cplusplus
}
#endif

#include <windows.h>

#include <stdint.h>
#ifdef __BORLANDC__
#undef INTPTR_MIN
#undef INTPTR_MAX
#undef UINTPTR_MAX
#undef PTRDIFF_MIN
#undef PTRDIFF_MAX
#undef SIZE_MAX
#ifdef _WIN64
#define INTPTR_MIN  _I64_MIN
#define INTPTR_MAX  _I64_MAX
#define UINTPTR_MAX _UI64_MAX
#else
#define INTPTR_MIN  INT_MIN
#define INTPTR_MAX  INT_MAX
#define UINTPTR_MAX UINT_MAX
#endif
#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX
#define SIZE_MAX    UINTPTR_MAX
#endif

#include <float.h>
#include <math.h>
#ifdef __BORLANDC__
#define trunc(x) ((x) >= 0 ? floor(x) : ceil(x))
#define round(x) floor((x) + 0.5)
#endif

#include <stdio.h>
#include <mbstring.h>
#include <fbit.h>
#include <assert.h>
#include "tlhelp32fix.h"
#include "intrinsic.h"
#include "IsBadPtr.h"
#include "page.h"
#include "atoitbl.h"
#include "CompareProcessMemory.h"
#include "MoveProcessMemory.h"
#include "FillProcessMemory.h"
#include "FindProcessMemory.h"
#include "FindReverseProcessMemory.h"
#include "TranscodeMultiByte.h"
#include "ToolTip/ToolTip.h"

#ifdef __BORLANDC__
#define USE_PLUGIN 0
#else
#define USE_PLUGIN 1
#endif

#if USE_PLUGIN
#include "Plugin.h"
#endif

#ifndef __BORLANDC__
#define __msreturn
#define __msfastcall __fastcall
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma function(strlen, wcslen, memcmp, memcpy)
#endif

#define IMPLEMENTED 0

#define ULL2DBL_LOST_BIT (64 - DBL_MANT_DIG)
#define ULL2DBL_LOST_MAX ((1 << ULL2DBL_LOST_BIT) - 1)

#if defined(__BORLANDC__)
EXTERN_C uint64_t __msreturn __cdecl _strtoui64(const char *nptr, char **endptr, int base);
#pragma warn -8004
#pragma warn -8008
#pragma warn -8060
#pragma warn -8066
#ifndef PROCESS_QUERY_LIMITED_INFORMATION
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#endif
#define static_assert(expression, string)                                              assert(expression)
#define _ultoa                                                                         ultoa
#define string                                                                         string
#define string_c_str(s)                                                                (s)->c_str()
#define string_begin(s)                                                                (s)->begin()
#define string_end(s)                                                                  (s)->end()
#define string_length(s)                                                               (s)->length()
#define vector_begin(v)                                                                (v)->begin()
#define vector_end(v)                                                                  (v)->end()
#define vector_TSSGAttributeElement                                                    vector<TSSGAttributeElement *>
#define TMainForm_GetUserMode(MainForm)                                                (MainForm)->GetUserMode()
#define TMainForm_Guide(Mes, Flags)                                                    (MainForm)->Guide(Mes, Flags)
#define TSSGCtrl_GetAttribute(SSGCtrl, SSGS, Type)                                     (SSGCtrl)->GetAttribute(SSGS, Type)
#define TSSGCtrl_GetSSGActionListner(SSGCtrl)                                          (SSGCtrl)->GetSSGActionListner()
#define TSSGCtrl_AddressAttributeFilter(SSGCtrl, SSGS, Address, Mode)                  (SSGCtrl)->AddressAttributeFilter(SSGS, Address, Mode)
#define TSSGSubject_GetAttribute(SSGS)                                                 (SSGS)->GetAttribute()
#define TSSGSubject_GetSize(SSGS)                                                      (SSGS)->GetSize()
#define TSSGActionListner_OnProcessOpenError(SSGActionListner, SSGS)                   (SSGActionListner)->OnProcessOpenError(SSGS)
#define TSSGActionListner_OnSubjectReadError(SSGActionListner, SSGS, Address)          (SSGActionListner)->OnSubjectReadError(SSGS, Address)
#define TSSGActionListner_OnSubjectWriteError(SSGActionListner, SSGS, Address)         (SSGActionListner)->OnSubjectWriteError(SSGS, Address)
#define TSSGActionListner_OnParsingError(SSGActionListner, SSGS, Code)                 (SSGActionListner)->OnParsingError(SSGS, Code)
#define TSSGActionListner_OnParsingProcess(SSGActionListner, SSGS, Code, TopVal)       (SSGActionListner)->OnParsingProcess(SSGS, Code, TopVal)
#define TSSGActionListner_OnParsingDoubleProcess(SSGActionListner, SSGS, Code, TopVal) (SSGActionListner)->OnParsingDoubleProcess(SSGS, Code, TopVal)
#define TSSGAttributeElement_GetType(SSGAttributeElement)                              (SSGAttributeElement)->GetType()
#define TEndWithAttribute_GetCode(EndWithAttribute)                                    (string *)((LPBYTE)(EndWithAttribute) + 8)
#define TIO_FEPAttribute_GetInputCode(IO_FEPAttribute)                                 (string *)((LPBYTE)(IO_FEPAttribute) + 8)
#define TIO_FEPAttribute_GetOutputCode(IO_FEPAttribute)                                ((string *)((LPBYTE)(IO_FEPAttribute) + 8) + 1)
#define TProcessCtrl_Open(ProcessCtrl, Mode)                                           (ProcessCtrl)->Open(Mode)
#define TProcessCtrl_GetModuleFromName(ProcessCtrl, Name)                              (ProcessCtrl)->GetModuleFromName(Name)
#define TProcessCtrl_GetHeapList(ProcessCtrl, ListNo)                                  (ProcessCtrl)->GetHeapList(ListNo)
#else
#define ADDITIONAL_TAGS      1
#define ALLOCATE_SUPPORT     1
#define LOCAL_MEMORY_SUPPORT 1
#define REPEAT_INDEX         1
#define SUBJECT_STATUS       1
#define SCOPE_SUPPORT        1
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_allocator.h"
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TSSGActionListner.h"
#include "THeapListData.h"
#include "TProcessCtrl.h"
#include "TSSGAttributeElement.h"
#include "TMainForm.h"
#define vector_TSSGAttributeElement vector
EXTERN_C size_t __stdcall ReplaceDefineByHeap(vector_TSSGAttributeElement *attributes, LPSTR *line, size_t length, size_t capacity);
#endif
#include "TStringDivision.h"
#include "Unescape.h"
#include "SubjectStringOperator.h"
#include "random.h"

#ifdef _MSC_VER
EXTERN_C int __fastcall internal_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr, const va_list endarg);
EXTERN_C int __fastcall internal_vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr, const va_list endarg);
#else
#define internal_vsnprintf(buffer, count, format, argptr, endarg) _vsnprintf(buffer, count, format, argptr)
#define internal_vsnwprintf(buffer, count, format, argptr, endarg) _vsnwprintf(buffer, count, format, argptr)
#endif
EXTERN_C unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
EXTERN_C unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c);
EXTERN_C unsigned char * __cdecl _mbsistr(const unsigned char *string1, const unsigned char *string2);
EXTERN_C unsigned char * __cdecl _mbsrstr(const unsigned char *string1, const unsigned char *string2);
EXTERN_C unsigned char * __cdecl _mbsristr(const unsigned char *string1, const unsigned char *string2);
EXTERN_C char *__fastcall internal_strtok(char *string, const char *delimiter, char **context);
EXTERN_C wchar_t *__fastcall internal_wcstok(wchar_t *string, const wchar_t *delimiter, wchar_t **context);
EXTERN_C unsigned char *__fastcall internal_mbstok(unsigned char *string, const unsigned char *delimiter, unsigned char **context);

EXTERN_C void __msfastcall Wait(DWORD dwMilliseconds);
EXTERN_C int __cdecl GuidePrint(const char *format, ...);
EXTERN_C int __fastcall GuidePrintV(const char *format, va_list argptr, const va_list endarg);
EXTERN_C int __cdecl DebugPrint(const char *format, ...);
EXTERN_C int __fastcall DebugPrintV(const char *format, va_list argptr, const va_list endarg);
EXTERN_C FARPROC __stdcall GetExportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpProcName);
EXTERN_C FARPROC * __stdcall GetImportFunction(HANDLE hProcess, HMODULE hModule, LPCSTR lpModuleName, LPCSTR lpProcName);
EXTERN_C LPVOID __stdcall GetSectionAddress(HANDLE hProcess, HMODULE hModule, LPCSTR lpSectionName, LPDWORD lpdwSectionSize);

#if defined(_MSC_VER)
#if REPEAT_INDEX
#include "SSGSubjectProperty.h"
#endif
extern HANDLE hHeap;
#endif
extern HANDLE hReadOnlyHeap;
extern HANDLE hPrivateHeap;

extern BOOL FixTheProcedure;

#include "HashBytes.h"

enum OS
{
	OS_PUSH          = 0x00000001,
	OS_OPEN          = 0x00000002,
	OS_CLOSE         = 0x00000004,
	OS_SPLIT         = 0x00000008,
	OS_DELIMITER     = 0x00000010,
	OS_ANTE          = 0x00000020,
	OS_POST          = 0x00000040,
	OS_LEFT_ASSIGN   = 0x00000080,
	OS_STRING        = 0x00000100,
	OS_HAS_EXPR      = 0x00000200,
	OS_PARENTHESES   = 0x00000400,
	OS_BRACES        = 0x00000800,
	OS_LOOP_BEGIN    = 0x00001000,
	OS_LOOP_END      = 0x00002000,
	OS_TERNARIES     = 0x00004000,
	OS_TERNARY_END   = 0x00008000,
	OS_VERIFY        = 0x00010000,
	// Declared below flags are not referenced alone.

	OS_SHORT_CIRCUIT = 0x00020000,
	OS_YIELD_OPERAND = 0x00040000,
};

/*
 [Wikipedia] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/%E6%BC%94%E7%AE%97%E5%AD%90%E3%81%AE%E5%84%AA%E5%85%88%E9%A0%86%E4%BD%8D#.E3.83.97.E3.83.AD.E3.82.B0.E3.83.A9.E3.83.9F.E3.83.B3.E3.82.B0.E8.A8.80.E8.AA.9E

 [Microsoft Docs] - [優先順位と評価順序] - [C 演算子の優先順位と結合規則]
 https://docs.microsoft.com/ja-jp/cpp/c-language/precedence-and-order-of-evaluation

 [Wikipedia] - [CとC++の演算子] - [演算子の優先順位]
 https://ja.wikipedia.org/wiki/C%E3%81%A8C%2B%2B%E3%81%AE%E6%BC%94%E7%AE%97%E5%AD%90#.E6.BC.94.E7.AE.97.E5.AD.90.E3.81.AE.E5.84.AA.E5.85.88.E9.A0.86.E4.BD.8D

 127 if                                                          OS_PUSH | OS_HAS_EXPR
 127 else                                                        OS_PUSH
 127 do                                                          OS_PUSH | OS_LOOP_BEGIN
 127 while                                                       OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_HAS_EXPR | OS_POST)
 127 for                                                         OS_PUSH | OS_HAS_EXPR
 127 break                                                       OS_PUSH
 127 continue                                                    OS_PUSH
#if IMPLEMENTED
 127 co_await                                                    OS_PUSH
 127 co_yield                                                    OS_PUSH
 127 co_return                                                   OS_PUSH
#endif
 127 switch                                                      OS_PUSH | OS_HAS_EXPR
 127 case                                                        OS_PUSH
 127 default                                                     OS_PUSH
 127 parse_int       parse_real      parse_reset                 OS_PUSH
  64 {                                                           OS_OPEN | OS_BRACES
  64 (                                                           OS_OPEN | OS_PARENTHESES
  64 [_                                                          OS_OPEN
  64 [.                                                          OS_OPEN
  64 [~                                                          OS_OPEN
  64 [:                                                          OS_OPEN
  64 ++ --                                                       OS_PUSH | OS_POST 後置インクリメント 後置デクリメント
  60 MName                                                       OS_PUSH | OS_ANTE
     ProcessId
     HNumber
     Memory
     IsBadCodePtr
     IsBadReadPtr    IsBadWritePtr
     IsBadStringPtrA IsBadStringPtrW
     Cast32          Cast64
     I1toI4          I2toI4          I4toI8
     cbd             cwd             cdq
     utof            itof            ftoi
     trunc           round
     isfinite        isinf           isnan
     BitScanForward  BitScanReverse
     bsf             bsr
     rotl8           rotl16          rotl32          rotl64
                                     rotl
     rotr8           rotr16          rotr32          rotr64
                                     rotr
     A2U             A2W
     U2A             U2W
     W2A             W2U
     assert
     wait
     sleep
#if ALLOCATE_SUPPORT
     realloc
#endif
     alloca
     atoi            wtoi
     atof            wtof
     tick
     rand32          rand64
     min             max             imin            imax
     memdup          wmemdup
     memccpy         wmemccpy
     memcmp          wmemcmp
     memicmp         wmemicmp
     memcpy          wmemcpy
     mempcpy         wmempcpy
     memmove         wmemmove
     memset8         memset16        memset32        memset64
     memset          wmemset
     memchr          wmemchr
     memichr         wmemichr
     memrchr         wmemrchr
     memrichr        wmemrichr
     memmem          wmemmem
     memimem         wmemimem
     memrmem         wmemrmem
     memrimem        wmemrimem
     printf          dprintf
     snprintf        snwprintf
     strdup          wcsdup
     strinc          wcsinc          mbsinc
     strdec          wcsdec          mbsdec
     strninc         wcsninc         mbsninc
     strnextc        wcsnextc        mbsnextc
     strlen          wcslen          mbslen
     strnlen         wcsnlen         mbsnlen
                                     mbsnccnt        mbsnbcnt
     strcmp          wcscmp
     strncmp         wcsncmp         mbsncmp
     stricmp         wcsicmp         mbsicmp
     strnicmp        wcsnicmp        mbsnicmp        mbsnbicmp
     strcpy          wcscpy
     stpcpy          wcpcpy
     strcat          wcscat
     strncpy         wcsncpy         mbsncpy         mbsnbcpy
     stpncpy         wcpncpy
     strncat         wcsncat         mbsncat         mbsnbcat
     strlcpy         wcslcpy
     strlcat         wcslcat
     strchr          wcschr          mbschr
     strichr         wcsichr         mbsichr
     strrchr         wcsrchr         mbsrchr
     strrichr        wcsrichr        mbsrichr
     strstr          wcsstr          mbsstr
     stristr         wcsistr         mbsistr
     strrstr         wcsrstr         mbsrstr
     strristr        wcsristr        mbsristr
     strspn          wcsspn          mbsspn
     strspnp         wcsspnp         mbsspnp
     strcspn         wcscspn         mbscspn
     strpbrk         wcspbrk         mbspbrk
     strset          wcsset          mbsset
     strnset         wcsnset         mbsnset         mbsnbset
     strtok          wcstok          mbstok
     strlwr          wcslwr          mbslwr
     strupr          wcsupr          mbsupr
     strrev          wcsrev          mbsrev
     isalnum         isalpha         isascii         isblank
     iscntrl         iscsym          iscsymf         isdigit
     isgraph         iskana          isleadbyte      islower
     isprint         ispunct         isspace         istrailbyte
     isupper         isxdigit
     ismbbalnum      ismbbalpha      ismbbgraph      ismbbprint
     ismbcalnum      ismbcalpha      ismbcdigit      ismbcgraph
     ismbchira       ismbckata       ismbcl0         ismbcl1
     ismbcl2         ismbclegal      ismbclower      ismbcprint
     ismbcpunct      ismbcspace      ismbcsymbol     ismbcupper
     iswalnum        iswalpha        iswascii        iswblank
     iswcntrl        iswcsym         iswcsymf        iswdigit
     iswgraph        iswlower        iswprint        iswpunct
     iswspace        iswupper        iswxdigit
     toascii         tolower         toupper
  56 ++ -- - ! ~ * &                                             OS_PUSH | OS_ANTE              前置インクリメント 前置デクリメント 単項マイナス 論理否定 ビットごとの論理否定 間接演算子 アドレス演算子
  52 * / % idiv imod                                             OS_PUSH                        乗算 除算 剰余算 符号付除算 符号付剰余算
  48 + -                                                         OS_PUSH                        加算 減算
  44 << >> sar rol ror                                           OS_PUSH                        左論理シフト 右論理シフト 右算術シフト 左ローテート 右ローテート
  40 < > <= >= lt gt le ge                                       OS_PUSH                        小なり 大なり 以下 以上 符号付小なり 符号付大なり 符号付以下 符号付以上
  36 == !=                                                       OS_PUSH                        等価 非等価
  32 &                                                           OS_PUSH                        ビットごとの論理積
  28 ^                                                           OS_PUSH                        ビットごとの排他的論理和
  24 |                                                           OS_PUSH                        ビットごとの論理和
  20 && and                                                      OS_PUSH | OS_SHORT_CIRCUIT     論理積
  16 || or                                                       OS_PUSH | OS_SHORT_CIRCUIT     論理和
  12 ? :                                                         OS_PUSH | OS_TERNARIES         条件演算子
   8 =>                                                          OS_PUSH                        右辺代入
   8 = += -= *= /= %= &= |= ^= <<= >>=                           OS_PUSH | OS_LEFT_ASSIGN       左辺代入 加算代入 減算代入 乗算代入 除算代入 剰余代入 ビット積代入 ビット排他的論理和代入 ビット和代入 左論理シフト代入 右論理シフト代入
   4 ,                                                           OS_DELIMITER
   4 :]   :8]   :7]   :6]   :5]   :4]   :3]   :2]   :1]          OS_PUSH | OS_CLOSE
     :I]  :I8]  :I7]  :I6]  :I5]  :I4]  :I3]  :I2]  :I1]
     :R]  :R4]  :R8]
     :F]  :F4]  :F8]
     :L]  :L8]  :L7]  :L6]  :L5]
     :L4] :L3]  :L2]  :L1]
     :LI] :LI8] :LI7] :LI6] :LI5] :LI4] :LI3] :LI2] :LI1]
     :LR] :LR4] :LR8]
     :LF] :LF4] :LF8]
   4 ~]   ~8]   ~7]   ~6]   ~5]   ~4]   ~3]   ~2]                OS_PUSH | OS_CLOSE
   4 .]                                                          OS_PUSH | OS_CLOSE
   4 _]                                                          OS_PUSH | OS_CLOSE
   4 )                                                           OS_CLOSE | OS_PARENTHESES
   4 }                                                           OS_CLOSE | OS_BRACES
   4 ;                                                           OS_SPLIT
   0 goto                                                        OS_PUSH
   0 return                                                      OS_PUSH
*/

typedef enum TAG {
	TAG_NOT_OPERATOR     ,  // 127                 OS_PUSH
	TAG_IF               ,  // 127 if              OS_PUSH | OS_HAS_EXPR
	TAG_ELSE             ,  // 127 else            OS_PUSH
	TAG_DO               ,  // 127 do              OS_PUSH | OS_LOOP_BEGIN
	TAG_WHILE            ,  // 127 while           OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_HAS_EXPR | OS_POST)
	TAG_FOR              ,  // 127 for             OS_PUSH | OS_HAS_EXPR
	TAG_BREAK            ,  // 127 break           OS_PUSH
	TAG_CONTINUE         ,  // 127 continue        OS_PUSH
	TAG_PARAM_LOCAL      ,  // 127 L               OS_PUSH
#if IMPLEMENTED
	TAG_CO_AWAIT         ,  // 127 co_await        OS_PUSH
	TAG_CO_YIELD         ,  // 127 co_yield        OS_PUSH
	TAG_CO_RETURN        ,  // 127 co_return       OS_PUSH
#endif
	TAG_SWITCH           ,  // 127 switch          OS_PUSH | OS_HAS_EXPR
	TAG_CASE             ,  // 127 case            OS_PUSH
	TAG_DEFAULT          ,  // 127 default         OS_PUSH
	TAG_LABEL            ,  // 127 :               OS_PUSH
	TAG_PARSE_INT        ,  // 127 parse_int       OS_PUSH
	TAG_PARSE_REAL       ,  // 127 parse_real      OS_PUSH
	TAG_PARSE_RESET      ,  // 127 parse_reset     OS_PUSH
	TAG_BRACE_OPEN       ,  //  64 {               OS_OPEN | OS_BRACES
	TAG_PARENTHESIS_OPEN ,  //  64 (               OS_OPEN | OS_PARENTHESES
	TAG_ADDR_ADJUST_OPEN ,  //  64 [_              OS_OPEN
	TAG_ADDR_REPLACE_OPEN,  //  64 [.              OS_OPEN
	TAG_REV_ENDIAN_OPEN  ,  //  64 [~              OS_OPEN
	TAG_REMOTE_OPEN      ,  //  64 [:              OS_OPEN
	TAG_INC              ,  //  64 N++  (52 ++N)   OS_PUSH | OS_POST (OS_PUSH | OS_ANTE)
	TAG_DEC              ,  //  64 N--  (52 --N)   OS_PUSH | OS_POST (OS_PUSH | OS_ANTE)
	TAG_PROCEDURE        ,  //  60 ::              OS_PUSH
	TAG_IMPORT_FUNCTION  ,  //  60 :!              OS_PUSH
	TAG_IMPORT_REFERENCE ,  //  60 :&              OS_PUSH
	TAG_MODULENAME       ,  //  60                 OS_PUSH
	TAG_SECTION          ,  //  60 := :+           OS_PUSH
	TAG_PROCESSID        ,  //  60 ProcessId       OS_PUSH | OS_ANTE
	TAG_MNAME            ,  //  60 MName           OS_PUSH | OS_ANTE
	TAG_HNUMBER          ,  //  60 HNumber         OS_PUSH | OS_ANTE
	TAG_MEMORY           ,  //  60 Memory          OS_PUSH | OS_ANTE
	TAG_ISBADCODEPTR     ,  //  60 IsBadCodePtr    OS_PUSH | OS_ANTE
	TAG_ISBADREADPTR     ,  //  60 IsBadReadPtr    OS_PUSH | OS_ANTE
	TAG_ISBADWRITEPTR    ,  //  60 IsBadWritePtr   OS_PUSH | OS_ANTE
	TAG_ISBADSTRINGPTRA  ,  //  60 IsBadStringPtrA OS_PUSH | OS_ANTE
	TAG_ISBADSTRINGPTRW  ,  //  60 IsBadStringPtrW OS_PUSH | OS_ANTE
	TAG_CAST32           ,  //  60 Cast32          OS_PUSH | OS_ANTE
	TAG_CAST64           ,  //  60 Cast64          OS_PUSH | OS_ANTE
	TAG_I1TOI4           ,  //  60 I1toI4          OS_PUSH | OS_ANTE
	                        //  60 cbd             OS_PUSH | OS_ANTE
	TAG_I2TOI4           ,  //  60 I2toI4          OS_PUSH | OS_ANTE
	                        //  60 cwd             OS_PUSH | OS_ANTE
	TAG_I4TOI8           ,  //  60 I4toI8          OS_PUSH | OS_ANTE
	                        //  60 cdq             OS_PUSH | OS_ANTE
	TAG_UTOF             ,  //  60 utof            OS_PUSH | OS_ANTE
	TAG_ITOF             ,  //  60 itof            OS_PUSH | OS_ANTE
	TAG_FTOI             ,  //  60 ftoi            OS_PUSH | OS_ANTE
	TAG_TRUNC            ,  //  60 trunc           OS_PUSH | OS_ANTE
	TAG_ROUND            ,  //  60 round           OS_PUSH | OS_ANTE
	TAG_ISFINITE         ,  //  60 isfinite        OS_PUSH | OS_ANTE
	TAG_ISINF            ,  //  60 isinf           OS_PUSH | OS_ANTE
	TAG_ISNAN            ,  //  60 isnan           OS_PUSH | OS_ANTE
	TAG_BSF              ,  //  60 BitScanForward  OS_PUSH | OS_ANTE
	                        //  60 bsf             OS_PUSH | OS_ANTE
	TAG_BSR              ,  //  60 BitScanReverse  OS_PUSH | OS_ANTE
	                        //  60 bsr             OS_PUSH | OS_ANTE
	TAG_ROTL8            ,  //  60 rotl8           OS_PUSH | OS_ANTE
	TAG_ROTL16           ,  //  60 rotl16          OS_PUSH | OS_ANTE
	TAG_ROTL32           ,  //  60 rotl32          OS_PUSH | OS_ANTE
	                        //  60 rotl            OS_PUSH | OS_ANTE
	TAG_ROTL64           ,  //  60 rotl64          OS_PUSH | OS_ANTE
	TAG_ROTR8            ,  //  60 rotr8           OS_PUSH | OS_ANTE
	TAG_ROTR16           ,  //  60 rotr16          OS_PUSH | OS_ANTE
	TAG_ROTR32           ,  //  60 rotr32          OS_PUSH | OS_ANTE
	                        //  60 rotr            OS_PUSH | OS_ANTE
	TAG_ROTR64           ,  //  60 rotr64          OS_PUSH | OS_ANTE
	TAG_A2U              ,  //  60 A2U             OS_PUSH | OS_ANTE
	TAG_A2W              ,  //  60 A2W             OS_PUSH | OS_ANTE
	TAG_U2A              ,  //  60 U2A             OS_PUSH | OS_ANTE
	TAG_U2W              ,  //  60 U2W             OS_PUSH | OS_ANTE
	TAG_W2A              ,  //  60 W2A             OS_PUSH | OS_ANTE
	TAG_W2U              ,  //  60 W2U             OS_PUSH | OS_ANTE
	TAG_ASSERT           ,  //  60 assert          OS_PUSH | OS_ANTE
	TAG_WAIT             ,  //  60 wait            OS_PUSH | OS_ANTE
	TAG_SLEEP            ,  //  60 sleep           OS_PUSH | OS_ANTE
#if ALLOCATE_SUPPORT
	TAG_REALLOC          ,  //  60 realloc         OS_PUSH | OS_ANTE
#endif
	TAG_ALLOCA           ,  //  60 alloca          OS_PUSH | OS_ANTE
	TAG_ATOI             ,  //  60 atoi            OS_PUSH | OS_ANTE
	TAG_WTOI             ,  //  60 wtoi            OS_PUSH | OS_ANTE
	TAG_ATOF             ,  //  60 atof            OS_PUSH | OS_ANTE
	TAG_WTOF             ,  //  60 wtof            OS_PUSH | OS_ANTE
	TAG_TICK             ,  //  60 tick            OS_PUSH | OS_ANTE
	TAG_RAND32           ,  //  60 rand32          OS_PUSH | OS_ANTE
	TAG_RAND64           ,  //  60 rand64          OS_PUSH | OS_ANTE
	TAG_MIN              ,  //  60 min             OS_PUSH | OS_ANTE
	TAG_MAX              ,  //  60 max             OS_PUSH | OS_ANTE
	TAG_IMIN             ,  //  60 imin            OS_PUSH | OS_ANTE
	TAG_IMAX             ,  //  60 imax            OS_PUSH | OS_ANTE
	TAG_MEMDUP           ,  //  60 memdup          OS_PUSH | OS_ANTE
	TAG_WMEMDUP          ,  //  60 wmemdup         OS_PUSH | OS_ANTE
	TAG_MEMCCPY          ,  //  60 memccpy         OS_PUSH | OS_ANTE
	TAG_WMEMCCPY         ,  //  60 wmemccpy        OS_PUSH | OS_ANTE
	TAG_MEMCMP           ,  //  60 memcmp          OS_PUSH | OS_ANTE
	TAG_WMEMCMP          ,  //  60 wmemcmp         OS_PUSH | OS_ANTE
	TAG_MEMICMP          ,  //  60 memicmp         OS_PUSH | OS_ANTE
	TAG_WMEMICMP         ,  //  60 wmemicmp        OS_PUSH | OS_ANTE
	TAG_MEMCPY           ,  //  60 memcpy          OS_PUSH | OS_ANTE
	TAG_WMEMCPY          ,  //  60 wmemcpy         OS_PUSH | OS_ANTE
	TAG_MEMPCPY          ,  //  60 mempcpy         OS_PUSH | OS_ANTE
	TAG_WMEMPCPY         ,  //  60 wmempcpy        OS_PUSH | OS_ANTE
	TAG_MEMMOVE          ,  //  60 memmove         OS_PUSH | OS_ANTE
	TAG_WMEMMOVE         ,  //  60 wmemmove        OS_PUSH | OS_ANTE
	TAG_MEMSET8          ,  //  60 memset8         OS_PUSH | OS_ANTE
	                        //  60 memset          OS_PUSH | OS_ANTE
	TAG_MEMSET16         ,  //  60 memset16        OS_PUSH | OS_ANTE
	                        //  60 wmemset         OS_PUSH | OS_ANTE
	TAG_MEMSET32         ,  //  60 memset32        OS_PUSH | OS_ANTE
	TAG_MEMSET64         ,  //  60 memset64        OS_PUSH | OS_ANTE
	TAG_MEMCHR           ,  //  60 memchr          OS_PUSH | OS_ANTE
	TAG_WMEMCHR          ,  //  60 wmemchr         OS_PUSH | OS_ANTE
	TAG_MEMICHR          ,  //  60 memichr         OS_PUSH | OS_ANTE
	TAG_WMEMICHR         ,  //  60 wmemichr        OS_PUSH | OS_ANTE
	TAG_MEMRCHR          ,  //  60 memrchr         OS_PUSH | OS_ANTE
	TAG_WMEMRCHR         ,  //  60 wmemrchr        OS_PUSH | OS_ANTE
	TAG_MEMRICHR         ,  //  60 memrichr        OS_PUSH | OS_ANTE
	TAG_WMEMRICHR        ,  //  60 wmemrichr       OS_PUSH | OS_ANTE
	TAG_MEMMEM           ,  //  60 memmem          OS_PUSH | OS_ANTE
	TAG_WMEMMEM          ,  //  60 wmemmem         OS_PUSH | OS_ANTE
	TAG_MEMIMEM          ,  //  60 memimem         OS_PUSH | OS_ANTE
	TAG_WMEMIMEM         ,  //  60 wmemimem        OS_PUSH | OS_ANTE
	TAG_MEMRMEM          ,  //  60 memrmem         OS_PUSH | OS_ANTE
	TAG_WMEMRMEM         ,  //  60 wmemrmem        OS_PUSH | OS_ANTE
	TAG_MEMRIMEM         ,  //  60 memrimem        OS_PUSH | OS_ANTE
	TAG_WMEMRIMEM        ,  //  60 wmemrimem       OS_PUSH | OS_ANTE
	TAG_PRINTF           ,  //  60 printf          OS_PUSH | OS_ANTE
	TAG_DPRINTF          ,  //  60 dprintf         OS_PUSH | OS_ANTE
	TAG_SNPRINTF         ,  //  60 snprintf        OS_PUSH | OS_ANTE
	TAG_SNWPRINTF        ,  //  60 snwprintf       OS_PUSH | OS_ANTE
	TAG_STRDUP           ,  //  60 strdup          OS_PUSH | OS_ANTE
	TAG_WCSDUP           ,  //  60 wcsdup          OS_PUSH | OS_ANTE
	TAG_STRINC           ,  //  60 strinc          OS_PUSH | OS_ANTE
	TAG_WCSINC           ,  //  60 wcsinc          OS_PUSH | OS_ANTE
	TAG_MBSINC           ,  //  60 mbsinc          OS_PUSH | OS_ANTE
	TAG_STRDEC           ,  //  60 strdec          OS_PUSH | OS_ANTE
	TAG_WCSDEC           ,  //  60 wcsdec          OS_PUSH | OS_ANTE
	TAG_MBSDEC           ,  //  60 mbsdec          OS_PUSH | OS_ANTE
	TAG_STRNINC          ,  //  60 strninc         OS_PUSH | OS_ANTE
	TAG_WCSNINC          ,  //  60 wcsninc         OS_PUSH | OS_ANTE
	TAG_MBSNINC          ,  //  60 mbsninc         OS_PUSH | OS_ANTE
	TAG_STRNEXTC         ,  //  60 strnextc        OS_PUSH | OS_ANTE
	TAG_WCSNEXTC         ,  //  60 wcsnextc        OS_PUSH | OS_ANTE
	TAG_MBSNEXTC         ,  //  60 mbsnextc        OS_PUSH | OS_ANTE
	TAG_STRLEN           ,  //  60 strlen          OS_PUSH | OS_ANTE
	TAG_WCSLEN           ,  //  60 wcslen          OS_PUSH | OS_ANTE
	TAG_MBSLEN           ,  //  60 mbslen          OS_PUSH | OS_ANTE
	TAG_STRNLEN          ,  //  60 strnlen         OS_PUSH | OS_ANTE
	TAG_WCSNLEN          ,  //  60 wcsnlen         OS_PUSH | OS_ANTE
	TAG_MBSNLEN          ,  //  60 mbsnlen         OS_PUSH | OS_ANTE
	TAG_MBSNBCNT         ,  //  60 mbsnbcnt        OS_PUSH | OS_ANTE
	TAG_MBSNCCNT         ,  //  60 mbsnccnt        OS_PUSH | OS_ANTE
	TAG_STRCMP           ,  //  60 strcmp          OS_PUSH | OS_ANTE
	TAG_WCSCMP           ,  //  60 wcscmp          OS_PUSH | OS_ANTE
	TAG_STRICMP          ,  //  60 stricmp         OS_PUSH | OS_ANTE
	TAG_WCSICMP          ,  //  60 wcsicmp         OS_PUSH | OS_ANTE
	TAG_MBSICMP          ,  //  60 mbsicmp         OS_PUSH | OS_ANTE
	TAG_STRNCMP          ,  //  60 strncmp         OS_PUSH | OS_ANTE
	TAG_WCSNCMP          ,  //  60 wcsncmp         OS_PUSH | OS_ANTE
	TAG_MBSNCMP          ,  //  60 mbsncmp         OS_PUSH | OS_ANTE
	TAG_STRNICMP         ,  //  60 strnicmp        OS_PUSH | OS_ANTE
	TAG_WCSNICMP         ,  //  60 wcsnicmp        OS_PUSH | OS_ANTE
	TAG_MBSNBICMP        ,  //  60 mbsnbicmp       OS_PUSH | OS_ANTE
	TAG_MBSNICMP         ,  //  60 mbsnicmp        OS_PUSH | OS_ANTE
	TAG_STRCPY           ,  //  60 strcpy          OS_PUSH | OS_ANTE
	TAG_WCSCPY           ,  //  60 wcscpy          OS_PUSH | OS_ANTE
	TAG_STPCPY           ,  //  60 stpcpy          OS_PUSH | OS_ANTE
	TAG_WCPCPY           ,  //  60 wcpcpy          OS_PUSH | OS_ANTE
	TAG_STRCAT           ,  //  60 strcat          OS_PUSH | OS_ANTE
	TAG_WCSCAT           ,  //  60 wcscat          OS_PUSH | OS_ANTE
	TAG_STRNCPY          ,  //  60 strncpy         OS_PUSH | OS_ANTE
	TAG_WCSNCPY          ,  //  60 wcsncpy         OS_PUSH | OS_ANTE
	TAG_MBSNBCPY         ,  //  60 mbsnbcpy        OS_PUSH | OS_ANTE
	TAG_MBSNCPY          ,  //  60 mbsncpy         OS_PUSH | OS_ANTE
	TAG_STPNCPY          ,  //  60 stpncpy         OS_PUSH | OS_ANTE
	TAG_WCPNCPY          ,  //  60 wcpncpy         OS_PUSH | OS_ANTE
	TAG_STRNCAT          ,  //  60 strncat         OS_PUSH | OS_ANTE
	TAG_WCSNCAT          ,  //  60 wcsncat         OS_PUSH | OS_ANTE
	TAG_MBSNBCAT         ,  //  60 mbsnbcat        OS_PUSH | OS_ANTE
	TAG_MBSNCAT          ,  //  60 mbsncat         OS_PUSH | OS_ANTE
	TAG_STRLCPY          ,  //  60 strlcpy         OS_PUSH | OS_ANTE
	TAG_WCSLCPY          ,  //  60 wcslcpy         OS_PUSH | OS_ANTE
	TAG_STRLCAT          ,  //  60 strlcat         OS_PUSH | OS_ANTE
	TAG_WCSLCAT          ,  //  60 wcslcat         OS_PUSH | OS_ANTE
	TAG_STRCHR           ,  //  60 strchr          OS_PUSH | OS_ANTE
	TAG_WCSCHR           ,  //  60 wcschr          OS_PUSH | OS_ANTE
	TAG_MBSCHR           ,  //  60 mbschr          OS_PUSH | OS_ANTE
	TAG_STRICHR          ,  //  60 strichr         OS_PUSH | OS_ANTE
	TAG_WCSICHR          ,  //  60 wcsichr         OS_PUSH | OS_ANTE
	TAG_MBSICHR          ,  //  60 mbsichr         OS_PUSH | OS_ANTE
	TAG_STRRCHR          ,  //  60 strrchr         OS_PUSH | OS_ANTE
	TAG_WCSRCHR          ,  //  60 wcsrchr         OS_PUSH | OS_ANTE
	TAG_MBSRCHR          ,  //  60 mbsrchr         OS_PUSH | OS_ANTE
	TAG_STRRICHR         ,  //  60 strrichr        OS_PUSH | OS_ANTE
	TAG_WCSRICHR         ,  //  60 wcsrichr        OS_PUSH | OS_ANTE
	TAG_MBSRICHR         ,  //  60 mbsrichr        OS_PUSH | OS_ANTE
	TAG_STRSTR           ,  //  60 strstr          OS_PUSH | OS_ANTE
	TAG_WCSSTR           ,  //  60 wcsstr          OS_PUSH | OS_ANTE
	TAG_MBSSTR           ,  //  60 mbsstr          OS_PUSH | OS_ANTE
	TAG_STRISTR          ,  //  60 stristr         OS_PUSH | OS_ANTE
	TAG_WCSISTR          ,  //  60 wcsistr         OS_PUSH | OS_ANTE
	TAG_MBSISTR          ,  //  60 mbsistr         OS_PUSH | OS_ANTE
	TAG_STRRSTR          ,  //  60 strrstr         OS_PUSH | OS_ANTE
	TAG_WCSRSTR          ,  //  60 wcsrstr         OS_PUSH | OS_ANTE
	TAG_MBSRSTR          ,  //  60 mbsrstr         OS_PUSH | OS_ANTE
	TAG_STRRISTR         ,  //  60 strristr        OS_PUSH | OS_ANTE
	TAG_WCSRISTR         ,  //  60 wcsristr        OS_PUSH | OS_ANTE
	TAG_MBSRISTR         ,  //  60 mbsristr        OS_PUSH | OS_ANTE
	TAG_STRSPN           ,  //  60 strspn          OS_PUSH | OS_ANTE
	TAG_WCSSPN           ,  //  60 wcsspn          OS_PUSH | OS_ANTE
	TAG_MBSSPN           ,  //  60 mbsspn          OS_PUSH | OS_ANTE
	TAG_STRSPNP          ,  //  60 strspnp         OS_PUSH | OS_ANTE
	TAG_WCSSPNP          ,  //  60 wcsspnp         OS_PUSH | OS_ANTE
	TAG_MBSSPNP          ,  //  60 mbsspnp         OS_PUSH | OS_ANTE
	TAG_STRCSPN          ,  //  60 strcspn         OS_PUSH | OS_ANTE
	TAG_WCSCSPN          ,  //  60 wcscspn         OS_PUSH | OS_ANTE
	TAG_MBSCSPN          ,  //  60 mbscspn         OS_PUSH | OS_ANTE
	TAG_STRPBRK          ,  //  60 strpbrk         OS_PUSH | OS_ANTE
	TAG_WCSPBRK          ,  //  60 wcspbrk         OS_PUSH | OS_ANTE
	TAG_MBSPBRK          ,  //  60 mbspbrk         OS_PUSH | OS_ANTE
	TAG_STRSET           ,  //  60 strset          OS_PUSH | OS_ANTE
	TAG_WCSSET           ,  //  60 wcsset          OS_PUSH | OS_ANTE
	TAG_MBSSET           ,  //  60 mbsset          OS_PUSH | OS_ANTE
	TAG_STRNSET          ,  //  60 strnset         OS_PUSH | OS_ANTE
	TAG_WCSNSET          ,  //  60 wcsnset         OS_PUSH | OS_ANTE
	TAG_MBSNBSET         ,  //  60 mbsnbset        OS_PUSH | OS_ANTE
	TAG_MBSNSET          ,  //  60 mbsnset         OS_PUSH | OS_ANTE
	TAG_STRTOK           ,  //  60 strtok          OS_PUSH | OS_ANTE
	TAG_WCSTOK           ,  //  60 wcstok          OS_PUSH | OS_ANTE
	TAG_MBSTOK           ,  //  60 mbstok          OS_PUSH | OS_ANTE
	TAG_STRLWR           ,  //  60 strlwr          OS_PUSH | OS_ANTE
	TAG_WCSLWR           ,  //  60 wcslwr          OS_PUSH | OS_ANTE
	TAG_MBSLWR           ,  //  60 mbslwr          OS_PUSH | OS_ANTE
	TAG_STRUPR           ,  //  60 strupr          OS_PUSH | OS_ANTE
	TAG_WCSUPR           ,  //  60 wcsupr          OS_PUSH | OS_ANTE
	TAG_MBSUPR           ,  //  60 mbsupr          OS_PUSH | OS_ANTE
	TAG_STRREV           ,  //  60 strrev          OS_PUSH | OS_ANTE
	TAG_WCSREV           ,  //  60 wcsrev          OS_PUSH | OS_ANTE
	TAG_MBSREV           ,  //  60 mbsrev          OS_PUSH | OS_ANTE
	TAG_ISALNUM          ,  //  60 isalnum         OS_PUSH | OS_ANTE
	TAG_ISALPHA          ,  //  60 isalpha         OS_PUSH | OS_ANTE
	TAG_ISASCII          ,  //  60 isascii         OS_PUSH | OS_ANTE
	TAG_ISBLANK          ,  //  60 isblank         OS_PUSH | OS_ANTE
	TAG_ISCNTRL          ,  //  60 iscntrl         OS_PUSH | OS_ANTE
	TAG_ISCSYM           ,  //  60 iscsym          OS_PUSH | OS_ANTE
	TAG_ISCSYMF          ,  //  60 iscsymf         OS_PUSH | OS_ANTE
	TAG_ISDIGIT          ,  //  60 isdigit         OS_PUSH | OS_ANTE
	TAG_ISGRAPH          ,  //  60 isgraph         OS_PUSH | OS_ANTE
	TAG_ISKANA           ,  //  60 iskana          OS_PUSH | OS_ANTE
	TAG_ISLEADBYTE       ,  //  60 isleadbyte      OS_PUSH | OS_ANTE
	TAG_ISLOWER          ,  //  60 islower         OS_PUSH | OS_ANTE
	TAG_ISPRINT          ,  //  60 isprint         OS_PUSH | OS_ANTE
	TAG_ISPUNCT          ,  //  60 ispunct         OS_PUSH | OS_ANTE
	TAG_ISSPACE          ,  //  60 isspace         OS_PUSH | OS_ANTE
	TAG_ISTRAILBYTE      ,  //  60 istrailbyte     OS_PUSH | OS_ANTE
	TAG_ISUPPER          ,  //  60 isupper         OS_PUSH | OS_ANTE
	TAG_ISXDIGIT         ,  //  60 isxdigit        OS_PUSH | OS_ANTE
	TAG_ISMBBALNUM       ,  //  60 ismbbalnum      OS_PUSH | OS_ANTE
	TAG_ISMBBALPHA       ,  //  60 ismbbalpha      OS_PUSH | OS_ANTE
	TAG_ISMBBGRAPH       ,  //  60 ismbbgraph      OS_PUSH | OS_ANTE
	TAG_ISMBBPRINT       ,  //  60 ismbbprint      OS_PUSH | OS_ANTE
	TAG_ISMBCALNUM       ,  //  60 ismbcalnum      OS_PUSH | OS_ANTE
	TAG_ISMBCALPHA       ,  //  60 ismbcalpha      OS_PUSH | OS_ANTE
	TAG_ISMBCDIGIT       ,  //  60 ismbcdigit      OS_PUSH | OS_ANTE
	TAG_ISMBCGRAPH       ,  //  60 ismbcgraph      OS_PUSH | OS_ANTE
	TAG_ISMBCHIRA        ,  //  60 ismbchira       OS_PUSH | OS_ANTE
	TAG_ISMBCKATA        ,  //  60 ismbckata       OS_PUSH | OS_ANTE
	TAG_ISMBCL0          ,  //  60 ismbcl0         OS_PUSH | OS_ANTE
	TAG_ISMBCL1          ,  //  60 ismbcl1         OS_PUSH | OS_ANTE
	TAG_ISMBCL2          ,  //  60 ismbcl2         OS_PUSH | OS_ANTE
	TAG_ISMBCLEGAL       ,  //  60 ismbclegal      OS_PUSH | OS_ANTE
	TAG_ISMBCLOWER       ,  //  60 ismbclower      OS_PUSH | OS_ANTE
	TAG_ISMBCPRINT       ,  //  60 ismbcprint      OS_PUSH | OS_ANTE
	TAG_ISMBCPUNCT       ,  //  60 ismbcpunct      OS_PUSH | OS_ANTE
	TAG_ISMBCSPACE       ,  //  60 ismbcspace      OS_PUSH | OS_ANTE
	TAG_ISMBCSYMBOL      ,  //  60 ismbcsymbol     OS_PUSH | OS_ANTE
	TAG_ISMBCUPPER       ,  //  60 ismbcupper      OS_PUSH | OS_ANTE
	TAG_ISWALNUM         ,  //  60 iswalnum        OS_PUSH | OS_ANTE
	TAG_ISWALPHA         ,  //  60 iswalpha        OS_PUSH | OS_ANTE
	TAG_ISWASCII         ,  //  60 iswascii        OS_PUSH | OS_ANTE
	TAG_ISWBLANK         ,  //  60 iswblank        OS_PUSH | OS_ANTE
	TAG_ISWCNTRL         ,  //  60 iswcntrl        OS_PUSH | OS_ANTE
	TAG_ISWCSYM          ,  //  60 iswcsym         OS_PUSH | OS_ANTE
	TAG_ISWCSYMF         ,  //  60 iswcsymf        OS_PUSH | OS_ANTE
	TAG_ISWDIGIT         ,  //  60 iswdigit        OS_PUSH | OS_ANTE
	TAG_ISWGRAPH         ,  //  60 iswgraph        OS_PUSH | OS_ANTE
	TAG_ISWLOWER         ,  //  60 iswlower        OS_PUSH | OS_ANTE
	TAG_ISWPRINT         ,  //  60 iswprint        OS_PUSH | OS_ANTE
	TAG_ISWPUNCT         ,  //  60 iswpunct        OS_PUSH | OS_ANTE
	TAG_ISWSPACE         ,  //  60 iswspace        OS_PUSH | OS_ANTE
	TAG_ISWUPPER         ,  //  60 iswupper        OS_PUSH | OS_ANTE
	TAG_ISWXDIGIT        ,  //  60 iswxdigit       OS_PUSH | OS_ANTE
	TAG_TOASCII          ,  //  60 toascii         OS_PUSH | OS_ANTE
	TAG_TOLOWER          ,  //  60 tolower         OS_PUSH | OS_ANTE
	TAG_TOUPPER          ,  //  60 toupper         OS_PUSH | OS_ANTE
#if USE_PLUGIN
	TAG_PLUGIN           ,  //  60                 OS_PUSH | OS_ANTE
#endif
	TAG_FUNCTION         ,  //  60                 OS_PUSH | OS_ANTE
	TAG_NEG              ,  //  56 -               OS_PUSH | OS_ANTE
	TAG_NOT              ,  //  56 !               OS_PUSH | OS_ANTE
	TAG_BIT_NOT          ,  //  56 ~               OS_PUSH | OS_ANTE
	TAG_INDIRECTION      ,  //  56 *               OS_PUSH | OS_ANTE
	TAG_ADDRESS_OF       ,  //  56 &               OS_PUSH | OS_ANTE
	TAG_MUL              ,  //  52 *    ( 8 *= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_DIV              ,  //  52 /    ( 8 /= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_MOD              ,  //  52 %    ( 8 %= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_IDIV             ,  //  52 idiv            OS_PUSH
	TAG_IMOD             ,  //  52 imod            OS_PUSH
	TAG_ADD              ,  //  48 +    ( 8 += )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SUB              ,  //  48 -    ( 8 -= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHL              ,  //  44 <<   ( 8 <<=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SHR              ,  //  44 >>   ( 8 >>=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_SAR              ,  //  44 sar             OS_PUSH
	TAG_ROL              ,  //  44 rol             OS_PUSH
	TAG_ROR              ,  //  44 ror             OS_PUSH
	TAG_BT               ,  //  40 <               OS_PUSH
	TAG_AT               ,  //  40 >               OS_PUSH
	TAG_BE               ,  //  40 <=              OS_PUSH
	TAG_AE               ,  //  40 >=              OS_PUSH
	TAG_LT               ,  //  40 lt              OS_PUSH
	TAG_GT               ,  //  40 gt              OS_PUSH
	TAG_LE               ,  //  40 le              OS_PUSH
	TAG_GE               ,  //  40 ge              OS_PUSH
	TAG_EQ               ,  //  36 ==              OS_PUSH
	TAG_NE               ,  //  36 !=              OS_PUSH
	TAG_BIT_AND          ,  //  32 &    ( 8 &= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_XOR              ,  //  28 ^    ( 8 ^= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_BIT_OR           ,  //  24 |    ( 8 |= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	TAG_AND              ,  //  20 &&   (20 and)   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_OR               ,  //  16 ||   (16 or )   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	TAG_TERNARY          ,  //  12 ?               OS_PUSH | OS_TERNARIES | OS_OPEN
	TAG_TERNARY_SPLIT    ,  //  12 :               OS_PUSH | OS_TERNARIES | OS_CLOSE
	TAG_RIGHT_ASSIGN     ,  //   8 =>              OS_PUSH
	TAG_LEFT_ASSIGN      ,  //   8 =               OS_PUSH | OS_LEFT_ASSIGN
	TAG_DELIMITER        ,  //   4 ,               OS_PUSH | OS_DELIMITER
	TAG_REMOTE1          ,  //   4 :1]             OS_PUSH | OS_CLOSE
	TAG_REMOTE2          ,  //   4 :2]             OS_PUSH | OS_CLOSE
	TAG_REMOTE3          ,  //   4 :3]             OS_PUSH | OS_CLOSE
	TAG_REMOTE4          ,  //   4 :] :4]          OS_PUSH | OS_CLOSE
	TAG_REMOTE5          ,  //   4 :5]             OS_PUSH | OS_CLOSE
	TAG_REMOTE6          ,  //   4 :6]             OS_PUSH | OS_CLOSE
	TAG_REMOTE7          ,  //   4 :7]             OS_PUSH | OS_CLOSE
	TAG_REMOTE8          ,  //   4 :8]             OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER1  ,  //   4 :I1]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER2  ,  //   4 :I2]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER3  ,  //   4 :I3]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER4  ,  //   4 :I] :I4]        OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER5  ,  //   4 :I5]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER6  ,  //   4 :I6]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER7  ,  //   4 :I7]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_INTEGER8  ,  //   4 :I8] :F8]       OS_PUSH | OS_CLOSE
	TAG_REMOTE_REAL4     ,  //   4 :R] :R4]        OS_PUSH | OS_CLOSE
	TAG_REMOTE_REAL8     ,  //   4 :R8]            OS_PUSH | OS_CLOSE
	TAG_REMOTE_FLOAT4    ,  //   4 :F] :F4]        OS_PUSH | OS_CLOSE
	TAG_LOCAL1           ,  //   4 :L1]            OS_PUSH | OS_CLOSE
	TAG_LOCAL2           ,  //   4 :L2]            OS_PUSH | OS_CLOSE
	TAG_LOCAL3           ,  //   4 :L3]            OS_PUSH | OS_CLOSE
	TAG_LOCAL4           ,  //   4 :L] :L4]        OS_PUSH | OS_CLOSE
	TAG_LOCAL5           ,  //   4 :L5]            OS_PUSH | OS_CLOSE
	TAG_LOCAL6           ,  //   4 :L6]            OS_PUSH | OS_CLOSE
	TAG_LOCAL7           ,  //   4 :L7]            OS_PUSH | OS_CLOSE
	TAG_LOCAL8           ,  //   4 :L8] :LF8]      OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER1   ,  //   4 :LI1]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER2   ,  //   4 :LI2]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER3   ,  //   4 :LI3]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER4   ,  //   4 :LI] :LI4]      OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER5   ,  //   4 :LI5]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER6   ,  //   4 :LI6]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER7   ,  //   4 :LI7]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_INTEGER8   ,  //   4 :LI8]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_REAL4      ,  //   4 :LR] :LR4]      OS_PUSH | OS_CLOSE
	TAG_LOCAL_REAL8      ,  //   4 :LR8]           OS_PUSH | OS_CLOSE
	TAG_LOCAL_FLOAT4     ,  //   4 :LF] :LF4]      OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN2      ,  //   4 ~2]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN3      ,  //   4 ~3]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN4      ,  //   4 ~] ~4]          OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN5      ,  //   4 ~5]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN6      ,  //   4 ~6]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN7      ,  //   4 ~7]             OS_PUSH | OS_CLOSE
	TAG_REV_ENDIAN8      ,  //   4 ~8]             OS_PUSH | OS_CLOSE
	TAG_ADDR_REPLACE     ,  //   4 .]              OS_PUSH | OS_CLOSE
	TAG_ADDR_ADJUST      ,  //   4 _]              OS_PUSH | OS_CLOSE
	TAG_IF_EXPR          ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESES
	TAG_SWITCH_EXPR      ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESES
	TAG_WHILE_EXPR       ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESES
	TAG_FOR_INITIALIZE   ,  //   4 ;               OS_PUSH | OS_SPLIT
	TAG_FOR_CONDITION    ,  //   4 ;               OS_PUSH | OS_SPLIT | OS_LOOP_BEGIN
	TAG_FOR_UPDATE       ,  //   4 )               OS_PUSH | OS_CLOSE | OS_SPLIT | OS_PARENTHESES
	TAG_PARENTHESIS_CLOSE,  //   4 )               OS_CLOSE | OS_PARENTHESES
	TAG_BRACE_CLOSE      ,  //   4 }               OS_CLOSE | OS_BRACES
	TAG_SPLIT            ,  //   4 ;               OS_SPLIT
	TAG_GOTO             ,  //   0 goto            OS_PUSH
	TAG_RETURN           ,  //   0 return          OS_PUSH
	TAG_PARSE_ERROR      ,
} TAG;

typedef enum PRIORITY {
	PRIORITY_NOT_OPERATOR      = 127,   //                 OS_PUSH
	PRIORITY_IF                = 127,   // if              OS_PUSH | OS_HAS_EXPR
	PRIORITY_ELSE              = 127,   // else            OS_PUSH
	PRIORITY_DO                = 127,   // do              OS_PUSH
	PRIORITY_WHILE             = 127,   // while           OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN (OS_PUSH | OS_POST)
	PRIORITY_FOR               = 127,   // for             OS_PUSH | OS_HAS_EXPR
	PRIORITY_BREAK             = 127,   // break           OS_PUSH
	PRIORITY_CONTINUE          = 127,   // continue        OS_PUSH
	PRIORITY_PARAM_LOCAL       = 127,   // L               OS_PUSH
#if IMPLEMENTED
	PRIORITY_CO_AWAIT          = 127,   // co_await        OS_PUSH
	PRIORITY_CO_YIELD          = 127,   // co_yield        OS_PUSH
	PRIORITY_CO_RETURN         = 127,   // co_return       OS_PUSH
#endif
	PRIORITY_SWITCH            = 127,   // switch          OS_PUSH | OS_HAS_EXPR
	PRIORITY_CASE              = 127,   // case            OS_PUSH
	PRIORITY_DEFAULT           = 127,   // default         OS_PUSH
	PRIORITY_LABEL             = 127,   // :               OS_PUSH
	PRIORITY_BRACE_OPEN        =  64,   // {               OS_OPEN | OS_BRACES
	PRIORITY_PARENTHESIS_OPEN  =  64,   // (               OS_OPEN | OS_PARENTHESES
	PRIORITY_ADDR_ADJUST_OPEN  =  64,   // [_              OS_OPEN
	PRIORITY_ADDR_REPLACE_OPEN =  64,   // [.              OS_OPEN
	PRIORITY_REV_ENDIAN_OPEN   =  64,   // [~              OS_OPEN
	PRIORITY_REMOTE_OPEN       =  64,   // [:              OS_OPEN
	PRIORITY_POST_INC_DEC      =  64,   // N++, N--        OS_PUSH | OS_POST
	PRIORITY_FUNCTION          =  60,   // MName           OS_PUSH | OS_ANTE
	                                    // ::              OS_PUSH
	                                    // :!              OS_PUSH
	                                    // :&              OS_PUSH
	                                    // := :+           OS_PUSH
	                                    // ProcessId       OS_PUSH | OS_ANTE
	                                    // HNumber         OS_PUSH | OS_ANTE
	                                    // Memory          OS_PUSH | OS_ANTE
	                                    // IsBadCodePtr    OS_PUSH | OS_ANTE
	                                    // IsBadReadPtr    OS_PUSH | OS_ANTE
	                                    // IsBadWritePtr   OS_PUSH | OS_ANTE
	                                    // IsBadStringPtrA OS_PUSH | OS_ANTE
	                                    // IsBadStringPtrW OS_PUSH | OS_ANTE
	                                    // Cast32          OS_PUSH | OS_ANTE
	                                    // Cast64          OS_PUSH | OS_ANTE
	                                    // I1toI4          OS_PUSH | OS_ANTE
	                                    // I2toI4          OS_PUSH | OS_ANTE
	                                    // I4toI8          OS_PUSH | OS_ANTE
	                                    // cbd             OS_PUSH | OS_ANTE
	                                    // cwd             OS_PUSH | OS_ANTE
	                                    // cdq             OS_PUSH | OS_ANTE
	                                    // utof            OS_PUSH | OS_ANTE
	                                    // itof            OS_PUSH | OS_ANTE
	                                    // ftoi            OS_PUSH | OS_ANTE
	                                    // trunc           OS_PUSH | OS_ANTE
	                                    // round           OS_PUSH | OS_ANTE
	                                    // isfinite        OS_PUSH | OS_ANTE
	                                    // isinf           OS_PUSH | OS_ANTE
	                                    // isnan           OS_PUSH | OS_ANTE
	                                    // BitScanForward  OS_PUSH | OS_ANTE
	                                    // bsf             OS_PUSH | OS_ANTE
	                                    // BitScanReverse  OS_PUSH | OS_ANTE
	                                    // bsr             OS_PUSH | OS_ANTE
	                                    // rotl8           OS_PUSH | OS_ANTE
	                                    // rotl16          OS_PUSH | OS_ANTE
	                                    // rotl32          OS_PUSH | OS_ANTE
	                                    // rotl            OS_PUSH | OS_ANTE
	                                    // rotl64          OS_PUSH | OS_ANTE
	                                    // rotr8           OS_PUSH | OS_ANTE
	                                    // rotr16          OS_PUSH | OS_ANTE
	                                    // rotr32          OS_PUSH | OS_ANTE
	                                    // rotr            OS_PUSH | OS_ANTE
	                                    // rotr64          OS_PUSH | OS_ANTE
	                                    // A2U             OS_PUSH | OS_ANTE
	                                    // A2W             OS_PUSH | OS_ANTE
	                                    // U2A             OS_PUSH | OS_ANTE
	                                    // U2W             OS_PUSH | OS_ANTE
	                                    // W2A             OS_PUSH | OS_ANTE
	                                    // W2U             OS_PUSH | OS_ANTE
	                                    // assert          OS_PUSH | OS_ANTE
	                                    // wait            OS_PUSH | OS_ANTE
	                                    // sleep           OS_PUSH | OS_ANTE
#if ALLOCATE_SUPPORT
	                                    // realloc         OS_PUSH | OS_ANTE
#endif
	                                    // alloca          OS_PUSH | OS_ANTE
	                                    // atoi            OS_PUSH | OS_ANTE
	                                    // wtoi            OS_PUSH | OS_ANTE
	                                    // atof            OS_PUSH | OS_ANTE
	                                    // wtof            OS_PUSH | OS_ANTE
	                                    // tick            OS_PUSH | OS_ANTE
	                                    // rand32          OS_PUSH | OS_ANTE
	                                    // rand64          OS_PUSH | OS_ANTE
	                                    // min             OS_PUSH | OS_ANTE
	                                    // max             OS_PUSH | OS_ANTE
	                                    // imin            OS_PUSH | OS_ANTE
	                                    // imax            OS_PUSH | OS_ANTE
	                                    // memdup          OS_PUSH | OS_ANTE
	                                    // wmemdup         OS_PUSH | OS_ANTE
	                                    // memccpy         OS_PUSH | OS_ANTE
	                                    // wmemccpy        OS_PUSH | OS_ANTE
	                                    // memcmp          OS_PUSH | OS_ANTE
	                                    // wmemcmp         OS_PUSH | OS_ANTE
	                                    // memicmp         OS_PUSH | OS_ANTE
	                                    // wmemicmp        OS_PUSH | OS_ANTE
	                                    // memcpy          OS_PUSH | OS_ANTE
	                                    // wmemcpy         OS_PUSH | OS_ANTE
	                                    // mempcpy         OS_PUSH | OS_ANTE
	                                    // wmempcpy        OS_PUSH | OS_ANTE
	                                    // memmove         OS_PUSH | OS_ANTE
	                                    // wmemmove        OS_PUSH | OS_ANTE
	                                    // memset          OS_PUSH | OS_ANTE
	                                    // wmemset         OS_PUSH | OS_ANTE
	                                    // memset8         OS_PUSH | OS_ANTE
	                                    // memset16        OS_PUSH | OS_ANTE
	                                    // memset32        OS_PUSH | OS_ANTE
	                                    // memset64        OS_PUSH | OS_ANTE
	                                    // memchr          OS_PUSH | OS_ANTE
	                                    // wmemchr         OS_PUSH | OS_ANTE
	                                    // memichr         OS_PUSH | OS_ANTE
	                                    // wmemichr        OS_PUSH | OS_ANTE
	                                    // memrchr         OS_PUSH | OS_ANTE
	                                    // wmemrchr        OS_PUSH | OS_ANTE
	                                    // memrichr        OS_PUSH | OS_ANTE
	                                    // wmemrichr       OS_PUSH | OS_ANTE
	                                    // memmem          OS_PUSH | OS_ANTE
	                                    // wmemmem         OS_PUSH | OS_ANTE
	                                    // memimem         OS_PUSH | OS_ANTE
	                                    // wmemimem        OS_PUSH | OS_ANTE
	                                    // memrmem         OS_PUSH | OS_ANTE
	                                    // wmemrmem        OS_PUSH | OS_ANTE
	                                    // memrimem        OS_PUSH | OS_ANTE
	                                    // wmemrimem       OS_PUSH | OS_ANTE
	                                    // printf          OS_PUSH | OS_ANTE
	                                    // dprintf         OS_PUSH | OS_ANTE
	                                    // snprintf        OS_PUSH | OS_ANTE
	                                    // snwprintf       OS_PUSH | OS_ANTE
	                                    // strdup          OS_PUSH | OS_ANTE
	                                    // wcsdup          OS_PUSH | OS_ANTE
	                                    // strinc          OS_PUSH | OS_ANTE
	                                    // wcsinc          OS_PUSH | OS_ANTE
	                                    // mbsinc          OS_PUSH | OS_ANTE
	                                    // strdec          OS_PUSH | OS_ANTE
	                                    // wcsdec          OS_PUSH | OS_ANTE
	                                    // mbsdec          OS_PUSH | OS_ANTE
	                                    // strninc         OS_PUSH | OS_ANTE
	                                    // wcsninc         OS_PUSH | OS_ANTE
	                                    // mbsninc         OS_PUSH | OS_ANTE
	                                    // strnextc        OS_PUSH | OS_ANTE
	                                    // wcsnextc        OS_PUSH | OS_ANTE
	                                    // mbsnextc        OS_PUSH | OS_ANTE
	                                    // strlen          OS_PUSH | OS_ANTE
	                                    // wcslen          OS_PUSH | OS_ANTE
	                                    // mbslen          OS_PUSH | OS_ANTE
	                                    // strnlen         OS_PUSH | OS_ANTE
	                                    // wcsnlen         OS_PUSH | OS_ANTE
	                                    // mbsnlen         OS_PUSH | OS_ANTE
	                                    // mbsnbcnt        OS_PUSH | OS_ANTE
	                                    // mbsnccnt        OS_PUSH | OS_ANTE
	                                    // strcmp          OS_PUSH | OS_ANTE
	                                    // wcscmp          OS_PUSH | OS_ANTE
	                                    // stricmp         OS_PUSH | OS_ANTE
	                                    // wcsicmp         OS_PUSH | OS_ANTE
	                                    // mbsicmp         OS_PUSH | OS_ANTE
	                                    // strncmp         OS_PUSH | OS_ANTE
	                                    // wcsncmp         OS_PUSH | OS_ANTE
	                                    // mbsncmp         OS_PUSH | OS_ANTE
	                                    // strnicmp        OS_PUSH | OS_ANTE
	                                    // wcsnicmp        OS_PUSH | OS_ANTE
	                                    // mbsnbicmp       OS_PUSH | OS_ANTE
	                                    // mbsnicmp        OS_PUSH | OS_ANTE
	                                    // strcpy          OS_PUSH | OS_ANTE
	                                    // wcscpy          OS_PUSH | OS_ANTE
	                                    // stpcpy          OS_PUSH | OS_ANTE
	                                    // wcpcpy          OS_PUSH | OS_ANTE
	                                    // strcat          OS_PUSH | OS_ANTE
	                                    // wcscat          OS_PUSH | OS_ANTE
	                                    // strncpy         OS_PUSH | OS_ANTE
	                                    // wcsncpy         OS_PUSH | OS_ANTE
	                                    // mbsnbcpy        OS_PUSH | OS_ANTE
	                                    // mbsncpy         OS_PUSH | OS_ANTE
	                                    // stpncpy         OS_PUSH | OS_ANTE
	                                    // wcpncpy         OS_PUSH | OS_ANTE
	                                    // strncat         OS_PUSH | OS_ANTE
	                                    // wcsncat         OS_PUSH | OS_ANTE
	                                    // strlcpy         OS_PUSH | OS_ANTE
	                                    // wcslcpy         OS_PUSH | OS_ANTE
	                                    // strlcat         OS_PUSH | OS_ANTE
	                                    // wcslcat         OS_PUSH | OS_ANTE
	                                    // strchr          OS_PUSH | OS_ANTE
	                                    // wcschr          OS_PUSH | OS_ANTE
	                                    // mbschr          OS_PUSH | OS_ANTE
	                                    // strichr         OS_PUSH | OS_ANTE
	                                    // wcsichr         OS_PUSH | OS_ANTE
	                                    // mbsichr         OS_PUSH | OS_ANTE
	                                    // strrchr         OS_PUSH | OS_ANTE
	                                    // wcsrchr         OS_PUSH | OS_ANTE
	                                    // mbsrchr         OS_PUSH | OS_ANTE
	                                    // strrichr        OS_PUSH | OS_ANTE
	                                    // wcsrichr        OS_PUSH | OS_ANTE
	                                    // mbsrichr        OS_PUSH | OS_ANTE
	                                    // strstr          OS_PUSH | OS_ANTE
	                                    // wcsstr          OS_PUSH | OS_ANTE
	                                    // mbsstr          OS_PUSH | OS_ANTE
	                                    // stristr         OS_PUSH | OS_ANTE
	                                    // wcsistr         OS_PUSH | OS_ANTE
	                                    // mbsistr         OS_PUSH | OS_ANTE
	                                    // strrstr         OS_PUSH | OS_ANTE
	                                    // wcsrstr         OS_PUSH | OS_ANTE
	                                    // mbsrstr         OS_PUSH | OS_ANTE
	                                    // strristr        OS_PUSH | OS_ANTE
	                                    // wcsristr        OS_PUSH | OS_ANTE
	                                    // mbsristr        OS_PUSH | OS_ANTE
	                                    // strspn          OS_PUSH | OS_ANTE
	                                    // wcsspn          OS_PUSH | OS_ANTE
	                                    // mbsspn          OS_PUSH | OS_ANTE
	                                    // strspnp         OS_PUSH | OS_ANTE
	                                    // wcsspnp         OS_PUSH | OS_ANTE
	                                    // mbsspnp         OS_PUSH | OS_ANTE
	                                    // strcspn         OS_PUSH | OS_ANTE
	                                    // wcscspn         OS_PUSH | OS_ANTE
	                                    // mbscspn         OS_PUSH | OS_ANTE
	                                    // strpbrk         OS_PUSH | OS_ANTE
	                                    // wcspbrk         OS_PUSH | OS_ANTE
	                                    // mbspbrk         OS_PUSH | OS_ANTE
	                                    // strset          OS_PUSH | OS_ANTE
	                                    // wcsset          OS_PUSH | OS_ANTE
	                                    // mbsset          OS_PUSH | OS_ANTE
	                                    // strnset         OS_PUSH | OS_ANTE
	                                    // wcsnset         OS_PUSH | OS_ANTE
	                                    // mbsnbset        OS_PUSH | OS_ANTE
	                                    // mbsnset         OS_PUSH | OS_ANTE
	                                    // strtok          OS_PUSH | OS_ANTE
	                                    // wcstok          OS_PUSH | OS_ANTE
	                                    // mbstok          OS_PUSH | OS_ANTE
	                                    // strlwr          OS_PUSH | OS_ANTE
	                                    // wcslwr          OS_PUSH | OS_ANTE
	                                    // mbslwr          OS_PUSH | OS_ANTE
	                                    // strupr          OS_PUSH | OS_ANTE
	                                    // wcsupr          OS_PUSH | OS_ANTE
	                                    // mbsupr          OS_PUSH | OS_ANTE
	                                    // strrev          OS_PUSH | OS_ANTE
	                                    // wcsrev          OS_PUSH | OS_ANTE
	                                    // mbsrev          OS_PUSH | OS_ANTE
	                                    // isalnum         OS_PUSH | OS_ANTE
	                                    // isalpha         OS_PUSH | OS_ANTE
	                                    // isascii         OS_PUSH | OS_ANTE
	                                    // isblank         OS_PUSH | OS_ANTE
	                                    // iscntrl         OS_PUSH | OS_ANTE
	                                    // iscsym          OS_PUSH | OS_ANTE
	                                    // iscsymf         OS_PUSH | OS_ANTE
	                                    // isdigit         OS_PUSH | OS_ANTE
	                                    // isgraph         OS_PUSH | OS_ANTE
	                                    // iskana          OS_PUSH | OS_ANTE
	                                    // isleadbyte      OS_PUSH | OS_ANTE
	                                    // islower         OS_PUSH | OS_ANTE
	                                    // isprint         OS_PUSH | OS_ANTE
	                                    // ispunct         OS_PUSH | OS_ANTE
	                                    // isspace         OS_PUSH | OS_ANTE
	                                    // istrailbyte     OS_PUSH | OS_ANTE
	                                    // isupper         OS_PUSH | OS_ANTE
	                                    // isxdigit        OS_PUSH | OS_ANTE
	                                    // ismbbalnum      OS_PUSH | OS_ANTE
	                                    // ismbbalpha      OS_PUSH | OS_ANTE
	                                    // ismbbgraph      OS_PUSH | OS_ANTE
	                                    // ismbbprint      OS_PUSH | OS_ANTE
	                                    // ismbcalnum      OS_PUSH | OS_ANTE
	                                    // ismbcalpha      OS_PUSH | OS_ANTE
	                                    // ismbcdigit      OS_PUSH | OS_ANTE
	                                    // ismbcgraph      OS_PUSH | OS_ANTE
	                                    // ismbchira       OS_PUSH | OS_ANTE
	                                    // ismbckata       OS_PUSH | OS_ANTE
	                                    // ismbcl0         OS_PUSH | OS_ANTE
	                                    // ismbcl1         OS_PUSH | OS_ANTE
	                                    // ismbcl2         OS_PUSH | OS_ANTE
	                                    // ismbclegal      OS_PUSH | OS_ANTE
	                                    // ismbclower      OS_PUSH | OS_ANTE
	                                    // ismbcprint      OS_PUSH | OS_ANTE
	                                    // ismbcpunct      OS_PUSH | OS_ANTE
	                                    // ismbcspace      OS_PUSH | OS_ANTE
	                                    // ismbcsymbol     OS_PUSH | OS_ANTE
	                                    // ismbcupper      OS_PUSH | OS_ANTE
	                                    // iswalnum        OS_PUSH | OS_ANTE
	                                    // iswalpha        OS_PUSH | OS_ANTE
	                                    // iswascii        OS_PUSH | OS_ANTE
	                                    // iswblank        OS_PUSH | OS_ANTE
	                                    // iswcntrl        OS_PUSH | OS_ANTE
	                                    // iswcsym         OS_PUSH | OS_ANTE
	                                    // iswcsymf        OS_PUSH | OS_ANTE
	                                    // iswdigit        OS_PUSH | OS_ANTE
	                                    // iswgraph        OS_PUSH | OS_ANTE
	                                    // iswlower        OS_PUSH | OS_ANTE
	                                    // iswprint        OS_PUSH | OS_ANTE
	                                    // iswpunct        OS_PUSH | OS_ANTE
	                                    // iswspace        OS_PUSH | OS_ANTE
	                                    // iswupper        OS_PUSH | OS_ANTE
	                                    // iswxdigit       OS_PUSH | OS_ANTE
	                                    // toascii         OS_PUSH | OS_ANTE
	                                    // tolower         OS_PUSH | OS_ANTE
	                                    // toupper         OS_PUSH | OS_ANTE
	PRIORITY_NEG               =  56,   // -               OS_PUSH | OS_ANTE
	PRIORITY_NOT               =  56,   // !               OS_PUSH | OS_ANTE
	PRIORITY_BIT_NOT           =  56,   // ~               OS_PUSH | OS_ANTE
	PRIORITY_INDIRECTION       =  56,   // *               OS_PUSH | OS_ANTE
	PRIORITY_ADDRESS_OF        =  56,   // &               OS_PUSH | OS_ANTE
	PRIORITY_PRE_INC           =  56,   // ++N             OS_PUSH | OS_ANTE
	PRIORITY_PRE_DEC           =  56,   // --N             OS_PUSH | OS_ANTE
	PRIORITY_MUL               =  52,   // *    ( 8 *= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_DIV               =  52,   // /    ( 8 /= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_MOD               =  52,   // %    ( 8 %= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_IDIV              =  52,   // idiv            OS_PUSH
	PRIORITY_IMOD              =  52,   // imod            OS_PUSH
	PRIORITY_ADD               =  48,   // +    ( 8 += )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SUB               =  48,   // -    ( 8 -= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHL               =  44,   // <<   ( 8 <<=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SHR               =  44,   // >>   ( 8 >>=)   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_SAR               =  44,   // sar             OS_PUSH
	PRIORITY_ROL               =  44,   // rol             OS_PUSH
	PRIORITY_ROR               =  44,   // ror             OS_PUSH
	PRIORITY_BT                =  40,   // <               OS_PUSH
	PRIORITY_AT                =  40,   // >               OS_PUSH
	PRIORITY_BE                =  40,   // <=              OS_PUSH
	PRIORITY_AE                =  40,   // >=              OS_PUSH
	PRIORITY_LT                =  40,   // lt              OS_PUSH
	PRIORITY_GT                =  40,   // gt              OS_PUSH
	PRIORITY_LE                =  40,   // le              OS_PUSH
	PRIORITY_GE                =  40,   // ge              OS_PUSH
	PRIORITY_EQ                =  36,   // ==              OS_PUSH
	PRIORITY_NE                =  36,   // !=              OS_PUSH
	PRIORITY_BIT_AND           =  32,   // &    ( 8 &= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_XOR               =  28,   // ^    ( 8 ^= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_BIT_OR            =  24,   // |    ( 8 |= )   OS_PUSH (OS_PUSH | OS_LEFT_ASSIGN)
	PRIORITY_AND               =  20,   // &&   (20 and)   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_OR                =  16,   // ||   (16 or )   OS_PUSH | OS_SHORT_CIRCUIT (OS_PUSH | OS_SHORT_CIRCUIT | OS_RET_OPERAND)
	PRIORITY_TERNARY           =  12,   // ? :             OS_PUSH | OS_TERNARIES | (OS_OPEN or OS_CLOSE)
	PRIORITY_RIGHT_ASSIGN      =   8,   // =>              OS_PUSH
	PRIORITY_LEFT_ASSIGN       =   8,   // =               OS_PUSH | OS_LEFT_ASSIGN
	PRIORITY_DELIMITER         =   4,   // ,               OS_PUSH | OS_DELIMITER
	PRIORITY_READ_WRITE        =   4,   // :]  :1] :2] :3] OS_PUSH | OS_CLOSE
	                                    // :4] :5] :6] :7]
	                                    // :8]
	                                    // :I]  :I1] :I2]
	                                    // :I3] :I4] :I5]
	                                    // :I6] :I7] :I8]
	                                    // :R]  :R4] :R8]
	                                    // :F]  :F4] :F8]
	                                    // :L]  :L1] :L2]
	                                    // :L3] :L4] :L5]
	                                    // :L6] :L7] :L8]
	                                    // :LI]  :LI1]
	                                    // :LI2] :LI3]
	                                    // :LI4] :LI5]
	                                    // :LI6] :LI7]
	                                    // :LI8]
	                                    // :LR]
	                                    // :LR4] :LR8]
	                                    // :LF]
	                                    // :LF4] :LF8]
	PRIORITY_REV_ENDIAN        =   4,   // ~]  ~2] ~3] ~4] OS_PUSH | OS_CLOSE
	                                    // ~5] ~6] ~7] ~8]
	PRIORITY_ADDR_REPLACE      =   4,   // .]              OS_PUSH | OS_CLOSE
	PRIORITY_ADDR_ADJUST       =   4,   // _]              OS_PUSH | OS_CLOSE
	PRIORITY_PARENTHESIS_CLOSE =   4,   // )               OS_CLOSE | OS_PARENTHESES
	PRIORITY_BRACE_CLOSE       =   4,   // }               OS_CLOSE | OS_BRACES
	PRIORITY_SPLIT             =   4,   // ;               OS_SPLIT
	PRIORITY_GOTO              =   0,   // goto            OS_PUSH
	PRIORITY_RETURN            =   0,   // return          OS_PUSH
} PRIORITY;

typedef struct _MARKUP {
	TAG                     Tag;
	enum OS                 Type;
	LPSTR                   String;
	WORD                    Length;
	BYTE                    Priority;
	size_t                  Depth;
	size_t                  LoopDepth;
	struct _MARKUP          *Param;
	struct _MARKUP          *Next;
	union {
		uint64_t            UnionBlock;
		struct {
			struct _MARKUP  **TruePart;
			struct _MARKUP  **FalsePart;
		};
		struct {
			struct _MARKUP  **Jump;
			struct _MARKUP  *Close;
		};
		struct {
			size_t          NumberOfOperand;
#if USE_PLUGIN
			PLUGIN_FUNCTION *Function;
#endif
		};
		size_t              UnescapedString;
	};
} MARKUP, *PMARKUP;

// Avoid conflicts with OLE
typedef struct _VARIAUNT {
	union VARIANT {
		struct QUAD {
			DWORD Low;
			DWORD High;
		};
		uint64_t  Quad;
		double    Real;
		float     Float;
		LPCSTR    AnsiString;
		LPCWSTR   WideString;
	};
	BOOL IsQuad;
} VARIAUNT, *PVARIAUNT;

typedef struct _VARIABLE {
	size_t   Length;
	LPCSTR   String;
	VARIAUNT Value;
#if SCOPE_SUPPORT
	map_iterator Node;
#endif
} VARIABLE, *PVARIABLE;

#ifndef TYPEDEF_PROCESSMEMORYBLOCK
#define TYPEDEF_PROCESSMEMORYBLOCK
typedef struct _PROCESSMEMORYBLOCK {
	DWORD  Id;
	LPVOID Address;
	size_t Size;
	DWORD  Protect;
} PROCESSMEMORYBLOCK, *PPROCESSMEMORYBLOCK;
#endif

size_t             nNumberOfProcessMemory = 0;
PROCESSMEMORYBLOCK *lpProcessMemory = NULL;
FILETIME           ftProcessCreationTime = { 0, 0 };

#if USE_PLUGIN
BOOL ParsingContinue;
#endif

typedef struct {
	size_t SizeOfReplace;
	LPBYTE Replace;
	LPSTR  Source;
	size_t NumberOfMarkup;
	MARKUP *Markup;
	size_t NumberOfPostfix;
	MARKUP **Postfix;
	size_t Next;
} CODECACHE, *PCODECACHE;

size_t nNumberOfCodeCache       = 0;
CODECACHE *lpCodeCache          = NULL;

LPBYTE lpReadOnlyBuffer         = NULL;
size_t nSizeOfConstStringRegion = 0;
LPBYTE lpConstStringRegion      = NULL;

static const size_t Terminator  = 0;

#if 0
//---------------------------------------------------------------------
#define AllocMarkup() \
	(MARKUP *)HeapAlloc(hHeap, 0, 0x10 * sizeof(MARKUP))
//---------------------------------------------------------------------
static MARKUP * __fastcall ReAllocMarkup(MARKUP **lplpMarkupArray, size_t *lpnNumberOfMarkup)
{
	MARKUP *lpMarkupArray;
	size_t nNumberOfMarkup;

	lpMarkupArray = *lplpMarkupArray;
	nNumberOfMarkup = *lpnNumberOfMarkup;
	if (nNumberOfMarkup && !(nNumberOfMarkup & 0x0F))
	{
		lpMarkupArray = (MARKUP *)HeapReAlloc(hHeap, 0, lpMarkupArray, (nNumberOfMarkup + 0x10) * sizeof(MARKUP));
		if (!lpMarkupArray)
			return NULL;
		*lplpMarkupArray = lpMarkupArray;
	}
	*lpnNumberOfMarkup = nNumberOfMarkup + 1;
	return lpMarkupArray + nNumberOfMarkup;
}
#endif
//---------------------------------------------------------------------
static __forceinline size_t TrimMarkupString(char **pfirst, const char *last)
{
	unsigned __int64 result;

	result = __reg64return_TrimSpace(*pfirst, last);
	return (const char *)(result >> 32) - (*pfirst = (char *)result);
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindBracketClose(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	enum OS const brackets = lpMarkup[-1].Type & (OS_PARENTHESES | OS_BRACES);
	if (brackets && lpMarkup < lpEndOfMarkup)
	{
		size_t nDepth = 1;

		do
			if (lpMarkup->Type & brackets)
				if (lpMarkup->Type & OS_OPEN)
					nDepth++;
				else if (!--nDepth)
					break;
		while (++lpMarkup < lpEndOfMarkup);
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindDelimiter(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	if (lpMarkup < lpEndOfMarkup)
	{
		size_t nDepth = 0;

		do
			if (lpMarkup->Type & (OS_OPEN | OS_CLOSE | OS_DELIMITER))
				if (lpMarkup->Type & (OS_OPEN | OS_CLOSE))
					if (lpMarkup->Type & OS_OPEN)
						nDepth++;
					else if (nDepth)
						nDepth--;
					else
						return (MARKUP *)lpEndOfMarkup;
				else if (!nDepth)
					break;
		while (++lpMarkup < lpEndOfMarkup);
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindSplit(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	if (lpMarkup < lpEndOfMarkup)
	{
		size_t nDepth = 0;

		do
			if (lpMarkup->Type & (OS_OPEN | OS_CLOSE | OS_SPLIT))
				if (lpMarkup->Type & (OS_OPEN | OS_CLOSE))
					if (lpMarkup->Type & OS_OPEN)
						nDepth++;
					else if (nDepth)
						nDepth--;
					else
						return (MARKUP *)lpEndOfMarkup;
				else if (!nDepth)
					break;
		while (++lpMarkup < lpEndOfMarkup);
	}
	return (MARKUP *)lpMarkup;
}
//---------------------------------------------------------------------
static MARKUP * __fastcall FindEndOfStructuredStatement(const MARKUP *lpMarkup, const MARKUP *lpEndOfMarkup)
{
	for (; ; )
	{
		if (lpMarkup >= lpEndOfMarkup)
			break;
		if (lpMarkup->Type & OS_HAS_EXPR)
		{
			TAG tag = lpMarkup->Tag;

			if (++lpMarkup >= lpEndOfMarkup)
				break;
			if (lpMarkup->Tag != TAG_PARENTHESIS_OPEN)
				break;
			if ((lpMarkup = FindBracketClose(lpMarkup + 1, lpEndOfMarkup)) + 1 >= lpEndOfMarkup)
				break;
			if ((lpMarkup = FindEndOfStructuredStatement(lpMarkup + 1, lpEndOfMarkup)) >= lpEndOfMarkup)
				break;
			if (tag != TAG_SWITCH && lpMarkup + 1 < lpEndOfMarkup && lpMarkup[1].Tag == TAG_ELSE)
			{
				lpMarkup += 2;
				continue;
			}
		}
		else if (lpMarkup->Tag == TAG_DO)
		{
			if ((lpMarkup = FindEndOfStructuredStatement(lpMarkup + 1, lpEndOfMarkup)) + 3 >= lpEndOfMarkup)
				break;
			if (lpMarkup[1].Tag != TAG_WHILE)
				break;
			if (lpMarkup[2].Tag != TAG_PARENTHESIS_OPEN)
				break;
			if ((lpMarkup = FindBracketClose(lpMarkup + 3, lpEndOfMarkup) + 1) >= lpEndOfMarkup)
				break;
			if (lpMarkup->Tag != TAG_SPLIT)
				break;
			if (lpMarkup + 1 < lpEndOfMarkup && lpMarkup[1].Tag == TAG_ELSE)
			{
				lpMarkup += 2;
				continue;
			}
		}
		else if (lpMarkup->Type & OS_OPEN && lpMarkup->Type & (OS_PARENTHESES | OS_BRACES))
			lpMarkup = FindBracketClose(lpMarkup + 1, lpEndOfMarkup);
		else
			lpMarkup = FindSplit(lpMarkup, lpEndOfMarkup);
		return (MARKUP *)lpMarkup;
	}
	return (MARKUP *)lpEndOfMarkup;
}
//---------------------------------------------------------------------
BOOL __fastcall CorrectFunction(MARKUP *lpMarkup, MARKUP *lpEndOfMarkup, size_t nNumberOfParams)
{
	MARKUP *lpFunction, *lpOpen, *lpClose;

	if ((lpOpen = (lpFunction = lpMarkup) + 1) >= lpEndOfMarkup ||
		lpOpen->Priority <= PRIORITY_MUL && lpOpen->Priority >= PRIORITY_SPLIT)
	{
		lpMarkup->Tag = TAG_NOT_OPERATOR;
		lpMarkup->Type = OS_PUSH;
		lpMarkup->Priority = PRIORITY_NOT_OPERATOR;
		return TRUE;
	}
	if (lpOpen->Tag == TAG_PARENTHESIS_OPEN &&
		(lpClose = FindBracketClose(lpOpen + 1, lpEndOfMarkup)) < lpEndOfMarkup)
	{
		size_t nCount;

		nCount = 0;
		if (lpOpen + 1 < lpClose)
		{
			MARKUP *lpList;

			lpMarkup = lpOpen;
			lpList = lpFunction;
			goto LOOP_ENTRY;

			do
			{
				MARKUP *lpParam;

				lpMarkup->Type &= ~OS_PUSH;
				if (lpMarkup + 1 == lpClose) break;
				lpList = lpList->Next = lpMarkup;
			LOOP_ENTRY:
				lpParam = lpMarkup;
				do
					if (++lpParam >= lpEndOfMarkup)
						return FALSE;
				while (lpParam->Tag == TAG_PARENTHESIS_OPEN);
				lpList->Param = lpParam;
				nCount++;
			} while ((lpMarkup = FindDelimiter(lpMarkup + 1, lpClose)) < lpClose);
		}
		if (nCount >= nNumberOfParams)
		{
			lpFunction->NumberOfOperand = nCount;
			return TRUE;
		}
	}
	return FALSE;
}
//---------------------------------------------------------------------
static MARKUP * __stdcall Markup(IN LPSTR lpSrc, IN size_t nSrcLength, OUT size_t *lpnNumberOfMarkup)
{
	MARKUP  *lpTagArray, *lpEndOfTag;
	size_t  nNumberOfTag;
	BOOLEAN bIsSeparatedLeft, bNextIsSeparatedLeft;
	LPBYTE  p, end;
	MARKUP  *lpMarkupArray;
	MARKUP  *lpMarkup, *lpEndOfMarkup, *lpFirstSwitch;
	size_t  nFirstTernary;
	BOOL    bCorrectTag;
	LPVOID  lpMem;
	size_t  nDepth;

	assert(("parameter error", lpSrc != NULL));
	assert(("parameter error", !__intrinsic_isspace(*lpSrc)));

	// check parameters
	if (!nSrcLength)
		return NULL;

	// mark up the tags and operators
	lpTagArray = HeapAlloc(hHeap, 0, nSrcLength * sizeof(MARKUP));
	if (!lpTagArray)
		return NULL;

	lpEndOfMarkup = lpTagArray + nSrcLength;
	nNumberOfTag = 0;
	lpFirstSwitch = NULL;
	nFirstTernary = -1;
	bCorrectTag = FALSE;
	bIsSeparatedLeft = TRUE;
	for (p = lpSrc, end = lpSrc + nSrcLength; p < end; bIsSeparatedLeft = bNextIsSeparatedLeft)
	{
		BYTE   c, comparand;
		TAG    iTag;
		size_t nLength;
		BYTE   bPriority;

		#define APPEND_TAG(tag, length, priority, type)                                     \
		do                                                                                  \
		{                                                                                   \
		    lpMarkup = lpTagArray + nNumberOfTag++;                                         \
		    assert(("allocation error", lpMarkup < lpEndOfMarkup));                         \
		    lpMarkup->Tag        = tag;                                                     \
		    lpMarkup->Type       = type;                                                    \
		    lpMarkup->String     = p;                                                       \
		    lpMarkup->Length     = (WORD)(length);                                          \
		    lpMarkup->Priority   = priority;                                                \
		    lpMarkup->Depth      = 0;                                                       \
		    lpMarkup->Param      = NULL;                                                    \
		    lpMarkup->Next       = NULL;                                                    \
		    lpMarkup->UnionBlock = 0;                                                       \
		} while (0)

		#define APPEND_TAG_WITH_CONTINUE(tag, length, priority, type)                       \
		if (1)                                                                              \
		{                                                                                   \
		    APPEND_TAG(tag, length, priority, type);                                        \
		    p += length;                                                                    \
		    continue;                                                                       \
		} else do { } while (0)

		#define APPEND_FUNCTION_SINGLE_PARAM(tag, length)                                   \
		if (1)                                                                              \
		{                                                                                   \
		    if (p[length] == '(' || __intrinsic_isspace(p[length]))                         \
		        nLength = length;                                                           \
		    else if (p[length] == ':' && p[length + 1] == ':')                              \
		        nLength = length + 2;                                                       \
		    else                                                                            \
		        break;                                                                      \
		    bNextIsSeparatedLeft = TRUE;                                                    \
		    bCorrectTag = TRUE;                                                             \
		    APPEND_TAG_WITH_CONTINUE(tag, nLength, PRIORITY_FUNCTION, OS_PUSH | OS_ANTE);   \
		} else do { } while (0)

		#define APPEND_FUNCTION_MULTI_PARAM(tag, length)                                    \
		if (1)                                                                              \
		{                                                                                   \
		    if (p[length] != '(' && !__intrinsic_isspace(p[length]))                        \
		        break;                                                                      \
		    bNextIsSeparatedLeft = TRUE;                                                    \
		    bCorrectTag = TRUE;                                                             \
		    APPEND_TAG_WITH_CONTINUE(tag, length, PRIORITY_FUNCTION, OS_PUSH | OS_ANTE);    \
		} else do { } while (0)

		/*
		#define IS_SEPARATED_LEFT(p) (                                                      \
		    __intrinsic_isascii((p)[0]) &&                                                  \
		    !__intrinsic_isalnum((p)[0]) &&                                                 \
		    ((p)[0] != '_' || ((p) != lpSrc && (p)[-1] == '[')) &&                          \
		    (p)[0] != '$')
		*/

		#define IS_SEPARATED_RIGHT(p) (                                                     \
		    __intrinsic_isascii((p)[0]) &&                                                  \
		    !__intrinsic_isalnum((p)[0]) &&                                                 \
		    ((p)[0] != '_' || (p)[1] == ']'))

		bNextIsSeparatedLeft = FALSE;
		switch (c = *p)
		{
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
		case ' ':
		case '#':
#if !SCOPE_SUPPORT
		case '@':
#endif
		case '\\':
		case '`':
			bNextIsSeparatedLeft = TRUE;
			break;
		case '!':
			// "!", "!="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] == '=')
				APPEND_TAG_WITH_CONTINUE(TAG_NE, 2, PRIORITY_NE, OS_PUSH);
			else if (!nNumberOfTag || (lpTagArray[nNumberOfTag - 1].Tag != TAG_IMPORT_FUNCTION && lpTagArray[nNumberOfTag - 1].Tag != TAG_IMPORT_REFERENCE))
				APPEND_TAG_WITH_CONTINUE(TAG_NOT, 1, PRIORITY_NOT, OS_PUSH | OS_ANTE);
			lpMarkup = lpTagArray + nNumberOfTag++;
			assert(("allocation error", lpMarkup < lpEndOfMarkup));
			lpMarkup->Tag        = TAG_MODULENAME;
			lpMarkup->Length     = p - lpTagArray[nNumberOfTag - 2].String - 1;
			lpMarkup->String     = lpTagArray[nNumberOfTag - 2].String + 2;
			lpMarkup->Priority   = PRIORITY_FUNCTION;
			lpMarkup->Type       = OS_PUSH;
			lpMarkup->Depth      = 0;
			lpMarkup->Param      = NULL;
			lpMarkup->Next       = NULL;
		    lpMarkup->UnionBlock = 0;
			break;
		case '"':
		case '\'':
			// string literals, character literals
			p++;
		STRING_LITERALS:
		CHARACTER_LITERALS:
			bNextIsSeparatedLeft = TRUE;
			comparand = c;
			do
			{
				switch (c = *(p++))
				{
				default:
					continue;
				case '"':
				case '\'':
					if (c != comparand)
						continue;
					break;
				case '\\':
					if (p >= end)
						break;
					c = *(p++);
					if (!__intrinsic_isleadbyte(c))
						continue;
					/* FALLTHROUGH */
				case_unsigned_leadbyte:
					if (p >= end)
						break;
					p++;
					continue;
				}
				break;
			} while (p < end);
			continue;
		case '%':
			// "%", "%="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_MOD, 1, PRIORITY_MOD, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_MOD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '&':
			// "&", "&&", "&="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '&':
				APPEND_TAG(TAG_AND, 2, PRIORITY_AND, OS_PUSH | OS_SHORT_CIRCUIT);
				APPEND_TAG_WITH_CONTINUE(TAG_AND, 2, PRIORITY_AND, OS_PUSH);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_BIT_AND, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_BIT_AND, 1, PRIORITY_BIT_AND, OS_PUSH);
			}
		case '(':
			// "("
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_OPEN, 1, PRIORITY_PARENTHESIS_OPEN, OS_OPEN | OS_PARENTHESES);
		case ')':
			// ")"
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_PARENTHESIS_CLOSE, 1, PRIORITY_PARENTHESIS_CLOSE, OS_CLOSE | OS_PARENTHESES);
		case '*':
			// "*", "*="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_MUL, 1, PRIORITY_MUL, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_MUL, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '+':
			// "+", "++", "+="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '+':
				APPEND_TAG_WITH_CONTINUE(TAG_INC, 2, PRIORITY_PRE_INC, OS_PUSH | OS_ANTE);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_ADD, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_ADD, 1, PRIORITY_ADD, OS_PUSH);
			}
		case ',':
			// ","
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_DELIMITER, 1, PRIORITY_DELIMITER, OS_PUSH | OS_DELIMITER);
		case '-':
			// "-", "--", "-="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '-':
				APPEND_TAG_WITH_CONTINUE(TAG_DEC, 2, PRIORITY_PRE_DEC, OS_PUSH | OS_ANTE);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_SUB, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_SUB, 1, PRIORITY_SUB, OS_PUSH);
			}
		case '.':
			// ".]"
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != ']')
				break;
			APPEND_TAG_WITH_CONTINUE(TAG_ADDR_REPLACE, 2, PRIORITY_ADDR_REPLACE, OS_PUSH | OS_CLOSE);
		case '/':
			// "/", "/="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_DIV, 1, PRIORITY_DIV, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_DIV, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case ':':
			// ":", "::", ":!", ":&", ":=", ":+"
			// ":]", ":1]", ":2]", ":3]", ":4]", ":5]", ":6]", ":7]", ":8]"
			// ":I]", ":I1]", ":I2]", ":I3]", ":I4]", ":I5]", ":I6]", ":I7]", ":I8]"
			// ":R]", ":R4]", ":R8]"
			// ":F]", ":F4]", ":F8]"
			// ":L]", ":L1]", ":L2]", ":L3]", ":L4]", ":L5]", ":L6]", ":L7]", ":L8]"
			// ":LI]", ":LI1]", ":LI2]", ":LI3]", ":LI4]", ":LI5]", ":LI6]", ":LI7]", ":LI8]"
			// ":LR]", ":LR4]", ":LR8]"
			// ":LF]", ":LF4]", ":LF8]"
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '!':
				APPEND_TAG_WITH_CONTINUE(TAG_IMPORT_FUNCTION, 2, PRIORITY_FUNCTION, OS_PUSH);
			case '&':
				APPEND_TAG_WITH_CONTINUE(TAG_IMPORT_REFERENCE, 2, PRIORITY_FUNCTION, OS_PUSH);
			case '+':
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_SECTION, 2, PRIORITY_FUNCTION, OS_PUSH);
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				if (p[2] != ']')
					break;
				iTag = (TAG)(TAG_REMOTE1 + p[1] - '1');
				nLength = 3;
				goto APPEND_READ_WRITE;
			case ':':
				APPEND_TAG_WITH_CONTINUE(TAG_PROCEDURE, 2, PRIORITY_FUNCTION, OS_PUSH);
			case 'F':
				switch (p[2])
				{
				case '4':
					iTag = TAG_REMOTE_FLOAT4;
					goto REMOTE_FLOAT;
				case '8':
					iTag = TAG_REMOTE8;
				REMOTE_FLOAT:
					if (p[3] != ']')
						break;
					nLength = 4;
					goto APPEND_READ_WRITE;
				case ']':
					iTag = TAG_REMOTE_FLOAT4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case 'I':
				switch (p[2])
				{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (p[3] != ']')
						break;
					iTag = (TAG)(TAG_REMOTE_INTEGER1 + p[2] - '1');
					nLength = 4;
					goto APPEND_READ_WRITE;
				case ']':
					iTag = TAG_REMOTE_INTEGER4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case 'L':
				switch (p[2])
				{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (p[3] != ']')
						break;
					iTag = (TAG)(TAG_LOCAL1 + p[2] - '1');
					nLength = 4;
					goto APPEND_READ_WRITE;
				case 'F':
					switch (p[3])
					{
					case '4':
						iTag = TAG_LOCAL_FLOAT4;
						goto LOCAL_FLOAT;
					case '8':
						iTag = TAG_LOCAL8;
					LOCAL_FLOAT:
						if (p[4] != ']')
							break;
						nLength = 5;
						goto APPEND_READ_WRITE;
					case ']':
						iTag = TAG_LOCAL_FLOAT4;
						nLength = 4;
						goto APPEND_READ_WRITE;
					}
					break;
				case 'I':
					switch (p[3])
					{
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
						if (p[4] != ']')
							break;
						iTag = (TAG)(TAG_LOCAL_INTEGER1 + p[2] - '1');
						nLength = 5;
						goto APPEND_READ_WRITE;
					case ']':
						iTag = TAG_LOCAL_INTEGER4;
						nLength = 4;
						goto APPEND_READ_WRITE;
					}
					break;
				case 'R':
					switch (p[3])
					{
					case '4':
						iTag = TAG_LOCAL_REAL4;
						goto LOCAL_REAL;
					case '8':
						iTag = TAG_LOCAL_REAL8;
					LOCAL_REAL:
						if (p[4] != ']')
							break;
						nLength = 5;
						goto APPEND_READ_WRITE;
					case ']':
						iTag = TAG_LOCAL_REAL4;
						nLength = 4;
						goto APPEND_READ_WRITE;
					}
					break;
				case ']':
					iTag = TAG_LOCAL4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case 'R':
				switch (p[2])
				{
				case '4':
					iTag = TAG_REMOTE_REAL4;
					goto REMOTE_REAL;
				case '8':
					iTag = TAG_REMOTE_REAL8;
				REMOTE_REAL:
					if (p[3] != ']')
						break;
					nLength = 4;
					goto APPEND_READ_WRITE;
				case ']':
					iTag = TAG_REMOTE_REAL4;
					nLength = 3;
					goto APPEND_READ_WRITE;
				}
				break;
			case ']':
				iTag = TAG_REMOTE4;
				nLength = 2;
			APPEND_READ_WRITE:
				APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_READ_WRITE, OS_PUSH | OS_CLOSE);
			}
			APPEND_TAG_WITH_CONTINUE(TAG_TERNARY_SPLIT, 1, PRIORITY_TERNARY, OS_PUSH/* | OS_TERNARIES*/ | OS_CLOSE);
		case ';':
			// ";"
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_SPLIT, 1, PRIORITY_SPLIT, OS_SPLIT);
		case '<':
			// "<", "<<", "<<=", "<="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '<':
				if (p[2] != '=')
					APPEND_TAG_WITH_CONTINUE(TAG_SHL, 2, PRIORITY_SHL, OS_PUSH);
				else
					APPEND_TAG_WITH_CONTINUE(TAG_SHL, 3, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_BE, 2, PRIORITY_BE, OS_PUSH);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_BT, 1, PRIORITY_BT, OS_PUSH);
			}
		case '=':
			// "=", "==", "=>"
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_EQ, 2, PRIORITY_EQ, OS_PUSH);
			case '>':
				APPEND_TAG_WITH_CONTINUE(TAG_RIGHT_ASSIGN, 2, PRIORITY_RIGHT_ASSIGN, OS_PUSH);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_LEFT_ASSIGN, 1, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			}
		case '>':
			// ">", ">>", ">>=", ">="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '>':
				if (p[2] != '=')
					APPEND_TAG_WITH_CONTINUE(TAG_SHR, 2, PRIORITY_SHR, OS_PUSH);
				else
					APPEND_TAG_WITH_CONTINUE(TAG_SHR, 3, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_AE, 2, PRIORITY_AE, OS_PUSH);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_AT, 1, PRIORITY_AT, OS_PUSH);
			}
		case '?':
			// "?"
			bNextIsSeparatedLeft = TRUE;
			if (nFirstTernary == -1)
				nFirstTernary = nNumberOfTag;
			APPEND_TAG_WITH_CONTINUE(TAG_TERNARY, 1, PRIORITY_TERNARY, OS_PUSH | OS_TERNARIES | OS_OPEN);
		case 'A':
			// "A2U", "A2W"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('2U'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_A2U, 3);
			case BSWAP16('2W'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_A2W, 3);
			}
			break;
		case 'B':
			// "BitScanForward", "BitScanReverse"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('itSc'))
				break;
			switch (*(uint32_t *)(p + 5))
			{
			case BSWAP32('anFo'):
				if (*(uint32_t *)(p + 9) != BSWAP32('rwar'))
					break;
				if (p[13] != 'd')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_BSF, 14);
			case BSWAP32('anRe'):
				if (*(uint32_t *)(p + 9) != BSWAP32('vers'))
					break;
				if (p[13] != 'e')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_BSR, 14);
			}
			break;
		case 'C':
			// "Cast32", "Cast64"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)p != BSWAP32('Cast'))
				break;
			switch (*(uint16_t *)(p + 4))
			{
			case BSWAP16('32'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_CAST32, 6);
			case BSWAP16('64'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_CAST64, 6);
			}
			break;
		case 'H':
			// "HNumber"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('Numb'))
				break;
			if (*(uint16_t *)(p + 5) != BSWAP16('er'))
				break;
			APPEND_FUNCTION_SINGLE_PARAM(TAG_HNUMBER, 7);
		case 'I':
			// "I1toI4", "I2toI4", "I4toI8"
			// "IsBadCodePtr", "IsBadReadPtr", "IsBadStringPtrA", "IsBadStringPtrW", "IsBadWritePtr"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case '1':
				if (*(uint32_t *)(p + 2) != BSWAP32('toI4'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I1TOI4, 6);
			case '2':
				if (*(uint32_t *)(p + 2) != BSWAP32('toI4'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I2TOI4, 6);
			case '4':
				if (*(uint32_t *)(p + 2) != BSWAP32('toI8'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I4TOI8, 6);
			case 's':
				if (*(uint32_t *)(p + 1) != BSWAP32('sBad'))
					break;
				switch (p[5])
				{
				case 'C':
					if (*(uint32_t *)(p + 6) != BSWAP32('odeP'))
						break;
					if (*(uint16_t *)(p + 10) != BSWAP16('tr'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_ISBADCODEPTR, 12);
				case 'R':
					if (*(uint32_t *)(p + 6) != BSWAP32('eadP'))
						break;
					if (*(uint16_t *)(p + 10) != BSWAP16('tr'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_ISBADREADPTR, 12);
				case 'S':
					if (*(uint32_t *)(p + 6) != BSWAP32('trin'))
						break;
					if (*(uint32_t *)(p + 10) != BSWAP32('gPtr'))
						break;
					switch (p[14])
					{
					case 'A':
						APPEND_FUNCTION_MULTI_PARAM(TAG_ISBADSTRINGPTRA, 15);
					case 'W':
						APPEND_FUNCTION_MULTI_PARAM(TAG_ISBADSTRINGPTRW, 15);
					}
					break;
				case 'W':
					if (*(uint32_t *)(p + 6) != BSWAP32('rite'))
						break;
					if (*(uint32_t *)(p + 9) != BSWAP32('ePtr'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_ISBADWRITEPTR, 13);
				}
				break;
			}
			break;
		case 'L':
			// "L"
			if (bIsSeparatedLeft && nNumberOfTag > 1)
			{
				MARKUP *lpPrev;

				lpPrev = lpTagArray + nNumberOfTag - 1;
				if (lpPrev->Tag != TAG_PARENTHESIS_OPEN && lpPrev->Tag != TAG_DELIMITER)
					break;
				if (p[-1] != ',' && p[-1] != '(')
				{
					BYTE *first, c;

					if (!__intrinsic_isspace(p[-1]))
						break;
					first = lpPrev->String;
					do
						c = *(++first);
					while (__intrinsic_isspace(c));
					if (first != p)
						break;
				}
				if (!__intrinsic_isspace(p[1]) &&
					p[1] != '$' &&
					p[1] != '(' &&
					p[1] != '@' &&
					p[1] != '[' &&
					p[1] != '{' &&
					p[1] != '~' &&
					(p[1] != '!' || p[2] == '='))
					break;
				if (lpPrev->Tag == TAG_DELIMITER)
				{
					size_t nDepth;

					nDepth = 1;
					while (--lpPrev != lpTagArray)
						if (lpPrev->Type & (OS_OPEN | OS_CLOSE))
							if (lpPrev->Type & OS_CLOSE)
								nDepth++;
							else if (!--nDepth)
								break;
					if (lpPrev == lpTagArray)
						break;
				}
				switch (lpPrev[-1].Tag)
				{
				case TAG_A2U:
				case TAG_A2W:
				case TAG_U2A:
				case TAG_U2W:
				case TAG_W2A:
				case TAG_W2U:
				case TAG_STRINC:
				case TAG_WCSINC:
				case TAG_MBSINC:
				case TAG_STRLEN:
				case TAG_WCSLEN:
				case TAG_MBSLEN:
				case TAG_STRDUP:
				case TAG_WCSDUP:
				case TAG_STRLWR:
				case TAG_WCSLWR:
				case TAG_MBSLWR:
				case TAG_STRUPR:
				case TAG_WCSUPR:
				case TAG_MBSUPR:
				case TAG_STRREV:
				case TAG_WCSREV:
				case TAG_MBSREV:
					if (lpPrev->Tag != TAG_PARENTHESIS_OPEN)
						break;
					/* FALLTHROUGH */
				case TAG_ISBADCODEPTR:
				case TAG_ISBADREADPTR:
				case TAG_ISBADWRITEPTR:
				case TAG_ISBADSTRINGPTRA:
				case TAG_ISBADSTRINGPTRW:
				case TAG_ATOI:
				case TAG_WTOI:
				case TAG_ATOF:
				case TAG_WTOF:
				case TAG_MEMDUP:
				case TAG_WMEMDUP:
				case TAG_MEMCMP:
				case TAG_WMEMCMP:
				case TAG_MEMICMP:
				case TAG_WMEMICMP:
				case TAG_MEMCPY:
				case TAG_WMEMCPY:
				case TAG_MEMCCPY:
				case TAG_WMEMCCPY:
				case TAG_MEMPCPY:
				case TAG_WMEMPCPY:
				case TAG_MEMMOVE:
				case TAG_WMEMMOVE:
				case TAG_MEMSET8:
				case TAG_MEMSET16:
				case TAG_MEMSET32:
				case TAG_MEMSET64:
				case TAG_MEMCHR:
				case TAG_WMEMCHR:
				case TAG_MEMICHR:
				case TAG_WMEMICHR:
				case TAG_MEMRCHR:
				case TAG_WMEMRCHR:
				case TAG_MEMRICHR:
				case TAG_WMEMRICHR:
				case TAG_MEMMEM:
				case TAG_WMEMMEM:
				case TAG_MEMIMEM:
				case TAG_WMEMIMEM:
				case TAG_MEMRMEM:
				case TAG_WMEMRMEM:
				case TAG_MEMRIMEM:
				case TAG_WMEMRIMEM:
				case TAG_PRINTF:
				case TAG_DPRINTF:
				case TAG_SNPRINTF:
				case TAG_SNWPRINTF:
				case TAG_STRDEC:
				case TAG_WCSDEC:
				case TAG_MBSDEC:
				case TAG_STRNINC:
				case TAG_WCSNINC:
				case TAG_MBSNINC:
				case TAG_STRNEXTC:
				case TAG_MBSNEXTC:
				case TAG_WCSNEXTC:
				case TAG_STRNLEN:
				case TAG_WCSNLEN:
				case TAG_MBSNLEN:
				case TAG_MBSNBCNT:
				case TAG_MBSNCCNT:
				case TAG_STRCMP:
				case TAG_WCSCMP:
				case TAG_STRICMP:
				case TAG_WCSICMP:
				case TAG_MBSICMP:
				case TAG_STRNCMP:
				case TAG_WCSNCMP:
				case TAG_MBSNCMP:
				case TAG_STRNICMP:
				case TAG_WCSNICMP:
				case TAG_MBSNBICMP:
				case TAG_MBSNICMP:
				case TAG_STRCPY:
				case TAG_WCSCPY:
				case TAG_STPCPY:
				case TAG_WCPCPY:
				case TAG_STRCAT:
				case TAG_WCSCAT:
				case TAG_STRNCPY:
				case TAG_WCSNCPY:
				case TAG_MBSNBCPY:
				case TAG_MBSNCPY:
				case TAG_STPNCPY:
				case TAG_WCPNCPY:
				case TAG_STRNCAT:
				case TAG_WCSNCAT:
				case TAG_MBSNBCAT:
				case TAG_MBSNCAT:
				case TAG_STRLCPY:
				case TAG_WCSLCPY:
				case TAG_STRLCAT:
				case TAG_WCSLCAT:
				case TAG_STRCHR:
				case TAG_WCSCHR:
				case TAG_MBSCHR:
				case TAG_STRICHR:
				case TAG_WCSICHR:
				case TAG_MBSICHR:
				case TAG_STRRCHR:
				case TAG_WCSRCHR:
				case TAG_MBSRCHR:
				case TAG_STRRICHR:
				case TAG_WCSRICHR:
				case TAG_MBSRICHR:
				case TAG_STRSTR:
				case TAG_WCSSTR:
				case TAG_MBSSTR:
				case TAG_STRISTR:
				case TAG_WCSISTR:
				case TAG_MBSISTR:
				case TAG_STRRSTR:
				case TAG_WCSRSTR:
				case TAG_MBSRSTR:
				case TAG_STRRISTR:
				case TAG_WCSRISTR:
				case TAG_MBSRISTR:
				case TAG_STRSPN:
				case TAG_WCSSPN:
				case TAG_MBSSPN:
				case TAG_STRSPNP:
				case TAG_WCSSPNP:
				case TAG_MBSSPNP:
				case TAG_STRCSPN:
				case TAG_WCSCSPN:
				case TAG_MBSCSPN:
				case TAG_STRPBRK:
				case TAG_WCSPBRK:
				case TAG_MBSPBRK:
				case TAG_STRSET:
				case TAG_WCSSET:
				case TAG_MBSSET:
				case TAG_STRNSET:
				case TAG_WCSNSET:
				case TAG_MBSNBSET:
				case TAG_MBSNSET:
				case TAG_STRTOK:
				case TAG_WCSTOK:
				case TAG_MBSTOK:
				case TAG_SWITCH:
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARAM_LOCAL, 1, PRIORITY_PARAM_LOCAL, OS_PUSH);
				}
			}
			break;
		case 'M':
			// "MName", "Memory"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)(p + 1))
			{
			case BSWAP32('Name'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_MNAME, 5);
			case BSWAP32('emor'):
				if (p[5] != 'y')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_MEMORY, 6);
			}
			break;
		case 'P':
			// "ProcessId"
			if (!bIsSeparatedLeft)
				break;
			if (*(uint32_t *)(p + 1) != BSWAP32('roce'))
				break;
			if (*(uint32_t *)(p + 5) != BSWAP32('ssId'))
				break;
			APPEND_FUNCTION_SINGLE_PARAM(TAG_PROCESSID, 9);
		case 'U':
			// "U2A", "U2W"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('2A'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_U2A, 3);
			case BSWAP16('2W'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_U2W, 3);
			}
			break;
		case 'W':
			// "W2A", "W2U"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('2A'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_W2A, 3);
			case BSWAP16('2U'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_W2U, 3);
			}
			break;
		case '[':
			// "[_", "[.", "[~", "[:"
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '_':
				APPEND_TAG_WITH_CONTINUE(TAG_ADDR_ADJUST_OPEN, 2, PRIORITY_ADDR_ADJUST_OPEN, OS_OPEN);
			case '.':
				APPEND_TAG_WITH_CONTINUE(TAG_ADDR_REPLACE_OPEN, 2, PRIORITY_ADDR_REPLACE_OPEN, OS_OPEN);
			case '~':
				APPEND_TAG_WITH_CONTINUE(TAG_REV_ENDIAN_OPEN, 2, PRIORITY_REV_ENDIAN_OPEN, OS_OPEN);
			case ':':
				APPEND_TAG_WITH_CONTINUE(TAG_REMOTE_OPEN, 2, PRIORITY_REMOTE_OPEN, OS_OPEN);
			}
			break;
		case '^':
			// "^", "^="
			bNextIsSeparatedLeft = TRUE;
			if (p[1] != '=')
				APPEND_TAG_WITH_CONTINUE(TAG_XOR, 1, PRIORITY_XOR, OS_PUSH);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_XOR, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
		case '_':
			// "_]"
			if (p[1] != ']')
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(TAG_ADDR_ADJUST, 2, PRIORITY_ADDR_ADJUST, OS_PUSH | OS_CLOSE);
		case 'a':
			// "alloca", "and", "assert", "atoi", "atof"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'l':
				if (*(uint32_t *)(p + 2) != BSWAP32('loca'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_ALLOCA, 6);
			case 'n':
				if (p[2] != 'd')
					break;
				iTag = TAG_AND;
				nLength = 3;
				bPriority = PRIORITY_AND;
				goto APPEND_YIELD_OPERAND_OPERATOR;
			case 's':
				if (*(uint32_t *)(p + 2) != BSWAP32('sert'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_ASSERT, 6);
			case 't':
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('oi'):
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ATOI, 4);
				case BSWAP16('of'):
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ATOF, 4);
				}
				break;
			}
			break;
		case 'b':
			// "break"
			// "bsf", "bsr"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'r':
				if (*(uint32_t *)(p + 1) != BSWAP32('reak'))
					break;
				if (!__intrinsic_isspace(p[5]) && p[5] != ';')
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_BREAK, 5, PRIORITY_BREAK, OS_PUSH);
			case 's':
				switch (p[2])
				{
				case 'f':
					APPEND_FUNCTION_SINGLE_PARAM(TAG_BSF, 3);
				case 'r':
					APPEND_FUNCTION_SINGLE_PARAM(TAG_BSR, 3);
				}
				break;
			}
			break;
		case 'c':
			// "cbd", "cwd", "cdq"
			// "case", "continue"
			// not implemented: "co_await", "co_return", "co_yield"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('se'):
					if (!__intrinsic_isspace(p[4]) || !FixTheProcedure)
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_CASE, 4, PRIORITY_CASE, OS_PUSH | OS_YIELD_OPERAND);
				}
				break;
			case 'b':
				if (p[2] != 'd')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I1TOI4, 3);
			case 'd':
				if (p[2] != 'q')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I4TOI8, 3);
			case 'o':
				switch (p[2])
				{
#if IMPLEMENTED
				case '_':
					switch (p[3])
					{
					case 'a':
						if (*(uint32_t *)(p + 4) != BSWAP32('wait'))
							break;
						if (!__intrinsic_isspace(p[8]) && p[8] != ';')
							break;
						bNextIsSeparatedLeft = TRUE;
						APPEND_TAG_WITH_CONTINUE(TAG_CO_AWAIT, 8, PRIORITY_CO_AWAIT, OS_PUSH);
					case 'r':
						if (*(uint32_t *)(p + 4) != BSWAP32('etur'))
							break;
						if (p[8] != 'n')
							break;
						if (!__intrinsic_isspace(p[9]) && p[9] != ';')
							break;
						bNextIsSeparatedLeft = TRUE;
						APPEND_TAG_WITH_CONTINUE(TAG_CO_RETURN, 9, PRIORITY_CO_RETURN, OS_PUSH);
					case 'y':
						if (*(uint32_t *)(p + 4) != BSWAP32('ield'))
							break;
						if (!__intrinsic_isspace(p[8]) && p[8] != ';')
							break;
						bNextIsSeparatedLeft = TRUE;
						APPEND_TAG_WITH_CONTINUE(TAG_CO_YIELD, 8, PRIORITY_CO_YIELD, OS_PUSH);
					}
					break;
#endif
				case 'n':
					if (*(uint32_t *)(p + 3) != BSWAP32('tinu'))
						break;
					if (p[7] != 'e')
						break;
					if (!__intrinsic_isspace(p[8]) && p[8] != ';')
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_CONTINUE, 8, PRIORITY_CONTINUE, OS_PUSH);
				}
				break;
			case 'w':
				if (p[2] != 'd')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_I2TOI4, 3);
			}
			break;
		case 'd':
			// "default"
			// "do", "dprintf"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'e':
				if (*(uint32_t *)(p + 2) != BSWAP32('faul'))
					break;
				if (p[6] != 't')
					break;
				if (p[7] != ':' && !__intrinsic_isspace(p[7]) && p[7] != ';' || !FixTheProcedure)
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_DEFAULT, 7, PRIORITY_DEFAULT, OS_PUSH | OS_YIELD_OPERAND);
			case 'o':
				if (p[2] != '(' && !__intrinsic_isspace(p[2]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_DO, 2, PRIORITY_DO, OS_PUSH | OS_LOOP_BEGIN);
			case 'p':
				if (*(uint32_t *)(p + 2) != BSWAP32('rint'))
					break;
				if (p[6] != 'f')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_DPRINTF, 7);
			}
			break;
		case 'e':
			// "elif", "else", "elseif"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)p)
			{
			case BSWAP32('elif'):
				if (p[4] != '(' && !__intrinsic_isspace(p[4]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG(TAG_ELSE, 4, PRIORITY_ELSE, 0);
				p += 4;
				APPEND_TAG_WITH_CONTINUE(TAG_IF, 0, PRIORITY_IF, OS_PUSH | OS_HAS_EXPR);
			case BSWAP32('else'):
				if (p[4] != '(' && !__intrinsic_isspace(p[4]) && p[4] != ';')
					if (p[4] != 'i' || p[5] != 'f' || p[6] != '(' && !__intrinsic_isspace(p[6]))
						break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_ELSE, 4, PRIORITY_ELSE, 0);
			}
			break;
		case 'f':
			// "for", "ftoi"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'o':
				if (p[2] != 'r')
					break;
				if (p[3] != '(' && !__intrinsic_isspace(p[3]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_FOR, 3, PRIORITY_FOR, OS_PUSH | OS_HAS_EXPR);
			case 't':
				if (*(uint16_t *)(p + 2) != BSWAP16('oi'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_FTOI, 4);
			}
			break;
		case 'g':
			// "gt", "ge"
			// "goto"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 't':
				iTag = TAG_GT;
				nLength = 2;
				bPriority = PRIORITY_GT;
				goto APPEND_WORD_OPERATOR;
			case 'e':
				iTag = TAG_GE;
				nLength = 2;
				bPriority = PRIORITY_GE;
				goto APPEND_WORD_OPERATOR;
			case 'o':
				if (*(uint16_t *)(p + 2) != BSWAP16('to'))
					break;
				if (!__intrinsic_isspace(p[4]))
					break;
				bNextIsSeparatedLeft = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_GOTO, 4, PRIORITY_GOTO, OS_PUSH);
			}
			break;
		case 'i':
			// "imax", "imin"
			// "idiv", "imod", "if", "itof",
			// "isfinite", "isinf", "isnan"
			// "isalnum", "isalpha", "isascii", "isblank", "iscntrl", "iscsym", "iscsymf",
			// "isdigit", "isgraph", "iskana", "isleadbyte", "islower",
			// "isprint", "ispunct", "isspace", "istrailbyte", "isupper", "isxdigit",
			// "ismbbalnum", "ismbbalpha", "ismbbgraph", "ismbbprint",
			// "ismbcalnum", "ismbcalpha", "ismbcdigit", "ismbcgraph", "ismbchira",
			// "ismbckata", "ismbcl0", "ismbcl1", "ismbcl2", "ismbclegal", "ismbclower",
			// "ismbcprint", "ismbcpunct", "ismbcspace", "ismbcsymbol","ismbcupper",
			// "iswalnum", "iswalpha", "iswascii", "iswblank", "iswcntrl", "iswcsym",
			// "iswcsymf", "iswdigit", "iswgraph", "iswlower", "iswprint", "iswpunct",
			// "iswspace", "iswupper", "iswxdigit"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'd':
				if (*(uint16_t *)(p + 2) != BSWAP16('iv'))
					break;
				iTag = TAG_IDIV;
				nLength = 4;
				bPriority = PRIORITY_IDIV;
				goto APPEND_WORD_OPERATOR;
			case 'm':
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('ax'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_IMAX, 4);
				case BSWAP16('in'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_IMIN, 4);
				case BSWAP16('od'):
					iTag = TAG_IMOD;
					nLength = 4;
					bPriority = PRIORITY_IMOD;
					goto APPEND_WORD_OPERATOR;
				}
				break;
			case 'f':
				if (p[2] != '(' && !__intrinsic_isspace(p[2]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_IF, 2, PRIORITY_IF, OS_PUSH | OS_HAS_EXPR);
			case 's':
				switch (p[2])
				{
				case 'a':
					switch (*(uint32_t *)(p + 3))
					{
					case BSWAP32('lnum'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISALNUM, 7);
					case BSWAP32('lpha'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISALPHA, 7);
					case BSWAP32('scii'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISASCII, 7);
					}
					break;
				case 'b':
					if (*(uint32_t *)(p + 3) != BSWAP32('lank'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISBLANK, 7);
				case 'c':
					switch (*(uint16_t *)(p + 3))
					{
					case BSWAP16('nt'):
						if (*(uint16_t *)(p + 5) != BSWAP16('rl'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISCNTRL, 7);
					case BSWAP16('sy'):
						if (p[5] != 'm')
							break;
						if (p[6] != 'f')
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISCSYM, 6);
						else
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISCSYMF, 7);
						break;
					}
					break;
				case 'd':
					if (*(uint32_t *)(p + 3) != BSWAP32('igit'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISDIGIT, 7);
				case 'f':
					if (*(uint32_t *)(p + 3) != BSWAP32('init'))
						break;
					if (p[7] != 'e')
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISFINITE, 8);
				case 'g':
					if (*(uint32_t *)(p + 3) != BSWAP32('raph'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISGRAPH, 7);
				case 'i':
					if (*(uint16_t *)(p + 3) != BSWAP16('nf'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISINF, 5);
				case 'k':
					if (*(uint32_t *)(p + 2) != BSWAP32('kana'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISKANA, 6);
				case 'l':
					switch (*(uint32_t *)(p + 3))
					{
					case BSWAP32('eadb'):
						if (*(uint32_t *)(p + 6) != BSWAP32('byte'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISLEADBYTE, 10);
					case BSWAP32('ower'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISLOWER, 7);
					}
					break;
				case 'm':
					switch (*(uint16_t *)(p + 3))
					{
					case BSWAP16('bb'):
						switch (p[5])
						{
						case 'a':
							switch (*(uint32_t *)(p + 6))
							{
							case BSWAP32('lnum'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBBALNUM, 10);
							case BSWAP32('lpha'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBBALPHA, 10);
							}
							break;
						case 'g':
							if (*(uint32_t *)(p + 6) != BSWAP32('raph'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBBGRAPH, 10);
							break;
						case 'p':
							if (*(uint32_t *)(p + 6) != BSWAP32('rint'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBBPRINT, 10);
							break;
						}
						break;
					case BSWAP16('bc'):
						switch (p[5])
						{
						case 'a':
							switch (*(uint32_t *)(p + 6))
							{
							case BSWAP32('lnum'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCALNUM, 10);
							case BSWAP32('lpha'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCALPHA, 10);
							}
							break;
						case 'd':
							if (*(uint32_t *)(p + 6) != BSWAP32('igit'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCDIGIT, 10);
						case 'g':
							if (*(uint32_t *)(p + 6) != BSWAP32('raph'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCGRAPH, 10);
						case 'h':
							if (*(uint32_t *)(p + 5) != BSWAP32('hira'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCHIRA, 9);
						case 'k':
							if (*(uint32_t *)(p + 5) != BSWAP32('kata'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCKATA, 9);
						case 'l':
							switch (p[6])
							{
							case '0':
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCL0, 7);
							case '1':
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCL1, 7);
							case '2':
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCL2, 7);
							case 'e':
								if (*(uint32_t *)(p + 6) != BSWAP32('egal'))
									break;
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCLEGAL, 10);
							case 'o':
								if (*(uint32_t *)(p + 6) != BSWAP32('ower'))
									break;
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCLOWER, 10);
							}
							break;
						case 'p':
							switch (*(uint32_t *)(p + 6))
							{
							case BSWAP32('rint'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCPRINT, 10);
							case BSWAP32('unct'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCPUNCT, 10);
							}
							break;
						case 's':
							switch (*(uint32_t *)(p + 6))
							{
							case BSWAP32('pace'):
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCSPACE, 10);
							case BSWAP32('ymbo'):
								if (p[10] != 'l')
									break;
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCSYMBOL, 11);
							}
							break;
						case 'u':
							if (*(uint32_t *)(p + 6) != BSWAP32('pper'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISMBCUPPER, 10);
						}
						break;
					}
					break;
				case 'n':
					if (*(uint16_t *)(p + 3) != BSWAP16('an'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISNAN, 5);
				case 'p':
					switch (*(uint32_t *)(p + 3))
					{
					case BSWAP32('rint'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISPRINT, 7);
					case BSWAP32('unct'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISPUNCT, 7);
					}
					break;
				case 's':
					if (*(uint32_t *)(p + 3) != BSWAP32('pace'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISSPACE, 7);
				case 't':
					if (*(uint32_t *)(p + 3) != BSWAP32('rail'))
						break;
					if (*(uint32_t *)(p + 7) != BSWAP32('byte'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISTRAILBYTE, 11);
				case 'u':
					if (*(uint32_t *)(p + 3) != BSWAP32('pper'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISUPPER, 7);
				case 'w':
					switch (p[3])
					{
					case 'a':
						switch (*(uint32_t *)(p + 4))
						{
						case BSWAP32('lnum'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWALNUM, 8);
						case BSWAP32('lpha'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWALPHA, 8);
						case BSWAP32('scii'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWASCII, 8);
						}
						break;
					case 'b':
						if (*(uint32_t *)(p + 4) != BSWAP32('lank'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWBLANK, 8);
					case 'c':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('nt'):
							if (*(uint16_t *)(p + 6) != BSWAP16('rl'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWCNTRL, 8);
						case BSWAP16('sy'):
							if (p[6] != 'm')
								break;
							if (p[7] != 'f')
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWCSYM, 7);
							else
								APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWCSYMF, 8);
						}
						break;
					case 'd':
						if (*(uint32_t *)(p + 4) != BSWAP32('igit'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWDIGIT, 8);
					case 'g':
						if (*(uint32_t *)(p + 4) != BSWAP32('raph'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWGRAPH, 8);
					case 'l':
						if (*(uint32_t *)(p + 4) != BSWAP32('ower'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWLOWER, 8);
					case 'p':
						switch (*(uint32_t *)(p + 4))
						{
						case BSWAP32('rint'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWPRINT, 8);
						case BSWAP32('unct'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWPUNCT, 8);
						}
						break;
					case 's':
						if (*(uint32_t *)(p + 4) != BSWAP32('pace'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWSPACE, 8);
					case 'u':
						if (*(uint32_t *)(p + 4) != BSWAP32('pper'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWUPPER, 8);
					case 'x':
						if (*(uint32_t *)(p + 4) != BSWAP32('digi'))
							break;
						if (p[8] != 't')
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_ISWXDIGIT, 9);
					}
					break;
				case 'x':
					if (*(uint32_t *)(p + 3) != BSWAP32('digi'))
						break;
					if (p[7] != 't')
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ISXDIGIT, 8);
				}
				break;
			case 't':
				if (*(uint16_t *)(p + 2) != BSWAP16('of'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_ITOF, 4);
			}
			break;
		case 'l':
			// "lt", "le"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 't':
				iTag = TAG_LT;
				nLength = 2;
				bPriority = PRIORITY_LT;
				goto APPEND_WORD_OPERATOR;
			case 'e':
				iTag = TAG_LE;
				nLength = 2;
				bPriority = PRIORITY_LE;
				goto APPEND_WORD_OPERATOR;
			}
			break;
		case 'm':
			// "max", "min"
			// "mbschr", "mbscspn", "mbsdec", "mbsichr", "mbsicmp", "mbsinc", "mbsistr", "mbslen", "mbslwr", "mbsnbcnt", "mbsnbcat", "mbsnbcpy", "mbsnbicmp", "mbsnbset", "mbsnccnt", "mbsncmp", "mbsncat", "mbsncpy", "mbsnextc", "mbsninc", "mbsnicmp", "mbsnlen", "mbsnset", "mbspbrk", "mbsrchr", "mbsrev", "mbsrichr", "mbsristr", "mbsrstr", "mbsset", "mbsspn", "mbsspnp", "mbsstr", "mbstok", "mbsupr",
			// "memccpy", "memchr", "memcmp", "memcpy", "memdup", "memichr", "memicmp", "memimem", "memmem", "memmove", "mempcpy", "memrchr", "memrmem", "memrichr", "memrimem", "memset", "memset16", "memset32", "memset64"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint16_t *)(p + 1))
			{
			case BSWAP16('ax'):
				APPEND_FUNCTION_MULTI_PARAM(TAG_MAX, 3);
			case BSWAP16('bs'):
				switch (p[3])
				{
				case 'c':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('hr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSCHR, 6);
					case BSWAP16('sp'):
						if (p[6] != 'n')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSCSPN, 7);
					}
					break;
				case 'd':
					if (*(uint16_t *)(p + 4) != BSWAP16('ec'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MBSDEC, 6);
				case 'i':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSICHR, 7);
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSICMP, 7);
					case BSWAP16('nc'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSINC, 6);
					case BSWAP16('st'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSISTR, 7);
					}
					break;
				case 'l':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('wr'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSLWR, 6);
					case BSWAP16('en'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSLEN, 6);
					}
					break;
				case 'n':
					switch (p[4])
					{
					case 'b':
						switch (p[5])
						{
						case 'c':
							switch (*(uint16_t *)(p + 6))
							{
							case BSWAP16('at'):
								APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBCAT, 8);
							case BSWAP16('nt'):
								APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBCNT, 8);
							case BSWAP16('py'):
								APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBCPY, 8);
							}
							break;
						case 'i':
							if (*(uint32_t *)(p + 5) != BSWAP32('icmp'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBICMP, 9);
						case 's':
							if (*(uint16_t *)(p + 6) != BSWAP16('et'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNBSET, 8);
						}
						break;
					case 'c':
						switch (*(uint16_t *)(p + 5))
						{
						case BSWAP16('at'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNCAT, 7);
						case BSWAP16('cn'):
							if (p[7] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNCCNT, 8);
						case BSWAP16('mp'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNCMP, 7);
						case BSWAP16('py'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNCPY, 7);
						}
						break;
					case 'e':
						if (*(uint32_t *)(p + 4) != BSWAP32('extc'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNEXTC, 8);
					case 'i':
						switch (*(uint16_t *)(p + 5))
						{
						case BSWAP16('nc'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNINC, 7);
						case BSWAP16('cm'):
							if (p[7] != 'p')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNICMP, 8);
						}
						break;
					case 'l':
						if (*(uint16_t *)(p + 5) != BSWAP16('en'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNLEN, 7);
					case 's':
						if (*(uint16_t *)(p + 5) != BSWAP16('et'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSNSET, 7);
					}
					break;
				case 'p':
					if (*(uint32_t *)(p + 3) != BSWAP32('pbrk'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MBSPBRK, 7);
				case 'r':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSRCHR, 7);
					case BSWAP16('ev'):
						APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSREV, 6);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSRICHR, 8);
					case BSWAP16('is'):
						if (*(uint16_t *)(p + 6) != BSWAP16('tr'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSRISTR, 8);
					case BSWAP16('st'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSRSTR, 7);
					}
					break;
				case 's':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('et'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSET, 6);
					case BSWAP16('pn'):
						if (p[6] != 'p')
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSPN, 6);
						else
							APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSPNP, 7);
					case BSWAP16('tr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MBSSTR, 6);
					}
					break;
				case 't':
					if (*(uint16_t *)(p + 4) != BSWAP16('ok'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MBSTOK, 6);
				case 'u':
					if (*(uint16_t *)(p + 4) != BSWAP16('pr'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_MBSUPR, 6);
				}
				break;
			case BSWAP16('em'):
				switch (p[3])
				{
				case 'c':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('cp'):
						if (p[6] != 'y')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMCCPY, 7);
					case BSWAP16('hr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMCHR, 6);
					case BSWAP16('mp'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMCMP, 6);
					case BSWAP16('py'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMCPY, 6);
					}
					break;
				case 'd':
					if (*(uint16_t *)(p + 4) != BSWAP16('up'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MEMDUP, 6);
					break;
				case 'i':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMICHR, 7);
					case BSWAP16('cm'):
						if (p[6] != 'p')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMICMP, 7);
					case BSWAP16('me'):
						if (p[6] != 'm')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMIMEM, 7);
					}
					break;
				case 'm':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('em'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMMEM, 6);
					case BSWAP16('ov'):
						if (p[6] != 'e')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMMOVE, 7);
					}
					break;
				case 'p':
					if (*(uint32_t *)(p + 3) != BSWAP32('pcpy'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MEMPCPY, 7);
				case 'r':
					switch (*(uint16_t *)(p + 4))
					{
					case BSWAP16('ch'):
						if (p[6] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMRCHR, 7);
					case BSWAP16('ic'):
						if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMRICHR, 8);
					case BSWAP16('im'):
						if (*(uint16_t *)(p + 6) != BSWAP16('em'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMRIMEM, 8);
					case BSWAP16('me'):
						if (p[6] != 'm')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMRMEM, 7);
					}
					break;
				case 's':
					if (*(uint16_t *)(p + 4) != BSWAP16('et'))
						break;
					switch (*(uint16_t *)(p + 6))
					{
					case BSWAP16('16'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET16, 8);
					case BSWAP16('32'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET32, 8);
					case BSWAP16('64'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET64, 8);
					default:
						APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET8, 6 + (p[6] == '8'));
					}
					break;
				}
				break;
			case BSWAP16('in'):
				APPEND_FUNCTION_MULTI_PARAM(TAG_MIN, 3);
			}
			break;
		case 'o':
			// "or"
			if (!bIsSeparatedLeft)
				break;
			if (p[1] != 'r')
				break;
			iTag = TAG_OR;
			nLength = 2;
			bPriority = PRIORITY_OR;
		APPEND_YIELD_OPERAND_OPERATOR:
			if (!IS_SEPARATED_RIGHT(p + nLength))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG(iTag, nLength, bPriority, OS_PUSH | OS_SHORT_CIRCUIT | OS_YIELD_OPERAND);
			APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH | OS_YIELD_OPERAND);
		case 'p':
			// "parse_int", "parse_real", "parse_reset", "printf"
			if (!bIsSeparatedLeft)
				break;
			switch (*(uint32_t *)(p + 1))
			{
			case BSWAP32('arse'):
				switch (*(uint32_t *)(p + 5))
				{
				case BSWAP32('_int'):
					if (p[9] != ';' && p[9] != '(' && p[9] != '{' && !__intrinsic_isspace(p[9]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_INT, 9, PRIORITY_NOT_OPERATOR, OS_PUSH);
				case BSWAP32('_rea'):
					if (p[9] != 'l')
						break;
					if (p[10] != ';' && p[10] != '(' && p[10] != '{' && !__intrinsic_isspace(p[10]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_REAL, 10, PRIORITY_NOT_OPERATOR, OS_PUSH);
				case BSWAP32('_res'):
					if (*(uint16_t *)(p + 9) != BSWAP16('et'))
						break;
					if (p[11] != ';' && p[11] != '(' && p[11] != '{' && !__intrinsic_isspace(p[11]))
						break;
					bNextIsSeparatedLeft = TRUE;
					bCorrectTag = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_RESET, 11, PRIORITY_NOT_OPERATOR, OS_PUSH);
				}
				break;
			case BSWAP32('rint'):
				if (p[5] != 'f')
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_PRINTF, 6);
			}
			break;
		case 'r':
			// "rand32", "rand64", "realloc", "return", "rol", "ror", "rotl", "rotl8", "rotl16", "rotl32", "rotl64", "rotr", "rotr8", "rotr16", "rotr32", "rotr64", "round"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				switch (*(uint32_t *)(p + 2))
				{
				case BSWAP32('nd32'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_RAND32, 6);
				case BSWAP32('nd64'):
					APPEND_FUNCTION_MULTI_PARAM(TAG_RAND64, 6);
				}
				break;
			case 'e':
#if ALLOCATE_SUPPORT
				if (*(uint32_t *)(p + 2) == BSWAP32('allo'))
				{
					if (p[6] != 'c')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_REALLOC, 7);
				}
				else
#endif
				if (*(uint32_t *)(p + 2) == BSWAP32('turn'))
				{
					if (p[6] != '(' && p[6] != ';' && !__intrinsic_isspace(p[6]))
						break;
					bNextIsSeparatedLeft = TRUE;
					APPEND_TAG_WITH_CONTINUE(TAG_RETURN, 6, PRIORITY_RETURN, OS_PUSH);
				}
				break;
			case 'o':
				switch (p[2])
				{
				case 'l':
					iTag = TAG_ROL;
					nLength = 3;
					bPriority = PRIORITY_ROL;
					goto APPEND_WORD_OPERATOR;
				case 'r':
					iTag = TAG_ROR;
					nLength = 3;
					bPriority = PRIORITY_ROR;
					goto APPEND_WORD_OPERATOR;
				case 't':
					switch (p[3])
					{
					case 'l':
						switch (p[4])
						{
						case '1':
							if (p[5] != '6')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTL16, 6);
						case '6':
							if (p[5] != '4')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTL64, 6);
						case '8':
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTL8, 5);
						default:
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTL32, *(uint16_t *)(p + 4) == BSWAP16('32') ? 6 : 4);
						}
						break;
					case 'r':
						switch (p[4])
						{
						case '1':
							if (p[5] != '6')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTR16, 6);
						case '6':
							if (p[5] != '4')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTR64, 6);
						case '8':
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTR8, 5);
						default:
							APPEND_FUNCTION_MULTI_PARAM(TAG_ROTR32, *(uint16_t *)(p + 4) == BSWAP16('32') ? 6 : 4);
						}
						break;
					}
					break;
				case 'u':
					if (*(uint16_t *)(p + 3) != BSWAP16('nd'))
						break;
					APPEND_FUNCTION_SINGLE_PARAM(TAG_ROUND, 5);
				}
				break;
			}
			break;
		case 's':
			// "sar"
			// "sleep", "snprintf", "snwprintf", "stpcpy", "stpncpy", "strcat", "strcnt", "strchr", "strcmp", "strcpy", "strcspn", "strdec", "strdup", "strichr", "stricmp", "strinc", "stristr", "strlcat", "strlcpy", "strlen", "strlwr", "strncat", "strncmp", "strncpy", "strnextc", "strninc, "strnicmp", "strnlen", "strnset", "strpbrk", "strrchr", "strrev", "strrichr", "strristr", "strrstr", "strset", "strspn", "strspnp", "strstr", "strtok", "strupr"
			// "switch"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				if (p[2] != 'r')
					break;
				iTag = TAG_SAR;
				nLength = 3;
				bPriority = PRIORITY_SAR;
				goto APPEND_WORD_OPERATOR;
			case 'l':
				if (*(uint32_t *)(p + 1) != BSWAP32('leep'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_SLEEP, 5);
			case 'n':
				switch (*(uint32_t *)(p + 2))
				{
				case BSWAP32('prin'):
					if (*(uint16_t *)(p + 6) != BSWAP16('tf'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_SNPRINTF, 8);
				case BSWAP32('wpri'):
					if (*(uint32_t *)(p + 5) != BSWAP32('intf'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_SNWPRINTF, 9);
				}
				break;
			case 't':
				switch (p[2])
				{
				case 'p':
					switch (p[3])
					{
					case 'c':
						if (*(uint16_t *)(p + 4) != BSWAP16('py'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STPCPY, 6);
					case 'n':
						if (*(uint32_t *)(p + 3) != BSWAP32('ncpy'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STPNCPY, 7);
					}
					break;
				case 'r':
					switch (p[3])
					{
					case 'c':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('at'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRCAT, 6);
						case BSWAP16('hr'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRCHR, 6);
						case BSWAP16('mp'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRCMP, 6);
						case BSWAP16('py'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRCPY, 6);
						case BSWAP16('sp'):
							if (p[6] != 'n')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRCSPN, 7);
						}
						break;
					case 'd':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ec'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRDEC, 6);
						case BSWAP16('up'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_STRDUP, 6);
						}
						break;
					case 'i':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ch'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRICHR, 7);
						case BSWAP16('cm'):
							if (p[6] != 'p')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRICMP, 7);
						case BSWAP16('nc'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_STRINC, 6);
						case BSWAP16('st'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRISTR, 7);
						}
						break;
					case 'l':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ca'):
							if (p[6] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRLCAT, 7);
						case BSWAP16('cp'):
							if (p[6] != 'y')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRLCPY, 7);
						case BSWAP16('en'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_STRLEN, 6);
						case BSWAP16('wr'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_STRLWR, 6);
						}
						break;
					case 'n':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ca'):
							if (p[6] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNCAT, 7);
						case BSWAP16('cm'):
							if (p[6] != 'p')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNCMP, 7);
						case BSWAP16('cp'):
							if (p[6] != 'y')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNCPY, 7);
						case BSWAP16('ex'):
							if (*(uint16_t *)(p + 6) != BSWAP16('tc'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNEXTC, 8);
						case BSWAP16('in'):
							if (p[6] != 'c')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNINC, 7);
						case BSWAP16('ic'):
							if (*(uint16_t *)(p + 6) != BSWAP16('mp'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNICMP, 8);
						case BSWAP16('le'):
							if (p[6] != 'n')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNLEN, 7);
						case BSWAP16('se'):
							if (p[6] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRNSET, 7);
						}
						break;
					case 'p':
						if (*(uint32_t *)(p + 3) != BSWAP32('pbrk'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRPBRK, 7);
					case 'r':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ch'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRRCHR, 7);
						case BSWAP16('ev'):
							if (*(uint16_t *)(p + 4) != BSWAP16('ev'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_STRREV, 6);
						case BSWAP16('ic'):
							if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRRICHR, 8);
						case BSWAP16('is'):
							if (*(uint16_t *)(p + 6) != BSWAP16('tr'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRRISTR, 8);
						case BSWAP16('st'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRRSTR, 7);
						}
						break;
					case 's':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('et'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRSET, 6);
						case BSWAP16('pn'):
							if (p[6] != 'p')
								APPEND_FUNCTION_MULTI_PARAM(TAG_STRSPN, 6);
							else
								APPEND_FUNCTION_MULTI_PARAM(TAG_STRSPNP, 7);
						case BSWAP16('tr'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_STRSTR, 6);
						}
						break;
					case 't':
						if (*(uint16_t *)(p + 4) != BSWAP16('ok'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_STRTOK, 6);
					case 'u':
						if (*(uint16_t *)(p + 4) != BSWAP16('pr'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_STRUPR, 6);
					}
					break;
				}
				break;
			case 'w':
				if (*(uint32_t *)(p + 2) != BSWAP32('itch'))
					break;
				if (p[6] != '(' && !__intrinsic_isspace(p[6]) || !FixTheProcedure)
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_SWITCH, 6, PRIORITY_SWITCH, OS_PUSH | OS_HAS_EXPR);
			}
			break;
		APPEND_WORD_OPERATOR:
			if (!IS_SEPARATED_RIGHT(p + nLength))
				break;
			bNextIsSeparatedLeft = TRUE;
			APPEND_TAG_WITH_CONTINUE(iTag, nLength, bPriority, OS_PUSH);
		case 't':
			// "tick", "toascii", "tolower", "toupper", "trunc"
			if (!bIsSeparatedLeft)
				break;
			if (*(LPDWORD)p == BSWAP32('tick'))
				APPEND_FUNCTION_MULTI_PARAM(TAG_TICK, 4);
			switch (*(uint32_t *)(p + 1))
			{
			case BSWAP32('oasc'):
				if (*(uint16_t *)(p + 5) != BSWAP16('ii'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TOASCII, 7);
			case BSWAP32('olow'):
				if (*(uint16_t *)(p + 5) != BSWAP16('er'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TOLOWER, 7);
			case BSWAP32('oupp'):
				if (*(uint16_t *)(p + 5) != BSWAP16('er'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TOUPPER, 7);
			case BSWAP32('runc'):
				APPEND_FUNCTION_SINGLE_PARAM(TAG_TRUNC, 5);
			}
			break;
		case 'u':
			// unicode or utf-8 string (u", u8")
			// "utof"
			if (!bIsSeparatedLeft)
				break;
			switch (c = p[1])
			{
			case '"':
				p += 2;
				goto STRING_LITERALS;
			case '\'':
				p += 2;
				goto CHARACTER_LITERALS;
			case '8':
				switch (c = p[2])
				{
				case '"':
					p += 3;
					goto STRING_LITERALS;
				case '\'':
					p += 3;
					goto CHARACTER_LITERALS;
				}
				break;
			case 't':
				if (*(uint16_t *)(p + 2) != BSWAP16('of'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_UTOF, 4);
			}
			break;
		case 'w':
			// "wait",
			// "wcpcpy", "wcpncpy", "wcscat", "wcschr", "wcscmp", "wcscpy", "wcscspn", "wcsdec", "wcsdup", "wcsichr", "wcsicmp", "wcsinc", "wcsistr", "wcslcat", "wcslcpy", "wcslen", "wcslwr", "wcsncat", "wcsncmp", "wcsncpy", "wcsnextc", "wcsninc", "wcsnicmp", "wcsnlen", "wcsnset", "wcspbrk", "wcsrchr", "wcsrev", "wcsrichr", "wcsristr", "wcsrstr", "wcsset", "wcsspn", "wcsspnp", "wcsstr", "wcstok", "wcsupr",
			// "wmemccpy", "wmemchr", "wmemcmp", "wmemcpy", "wmemdup", "wmemichr", "wmemicmp", "wmemimem", "wmemmem", "wmemmove", "wmempcpy", "wmemrchr", "wmemrmem", "wmemrichr", "wmemrimem", "wmemset", "wtoi", "wtof",
			// "while"
			if (!bIsSeparatedLeft)
				break;
			switch (p[1])
			{
			case 'a':
				if (*(uint16_t *)(p + 2) != BSWAP16('it'))
					break;
				APPEND_FUNCTION_SINGLE_PARAM(TAG_WAIT, 4);
			case 'c':
				switch (p[2])
				{
				case 'p':
					switch (p[3])
					{
					case 'c':
						if (*(uint16_t *)(p + 4) != BSWAP16('py'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCPCPY, 6);
					case 'n':
						if (*(uint32_t *)(p + 3) != BSWAP32('ncpy'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCPNCPY, 7);
					}
					break;
				case 's':
					switch (p[3])
					{
					case 'c':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('at'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCAT, 6);
						case BSWAP16('hr'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCHR, 6);
						case BSWAP16('mp'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCMP, 6);
						case BSWAP16('py'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCPY, 6);
						case BSWAP16('sp'):
							if (p[6] != 'n')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSCSPN, 7);
						}
						break;
					case 'd':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ec'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSDEC, 6);
						case BSWAP16('up'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSDUP, 6);
						}
						break;
					case 'i':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ch'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSICHR, 7);
						case BSWAP16('cm'):
							if (p[6] != 'p')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSICMP, 7);
						case BSWAP16('nc'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSINC, 6);
						case BSWAP16('st'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSISTR, 7);
						}
						break;
					case 'l':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ca'):
							if (p[6] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSLCAT, 7);
						case BSWAP16('cp'):
							if (p[6] != 'y')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSLCPY, 7);
						case BSWAP16('en'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSLEN, 6);
						case BSWAP16('wr'):
							APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSLWR, 6);
						}
						break;
					case 'n':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ca'):
							if (p[6] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNCAT, 7);
						case BSWAP16('cm'):
							if (p[6] != 'p')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNCMP, 7);
						case BSWAP16('cp'):
							if (p[6] != 'y')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNCPY, 7);
						case BSWAP16('ex'):
							if (*(uint16_t *)(p + 6) != BSWAP16('tc'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNEXTC, 8);
						case BSWAP16('in'):
							if (p[6] != 'c')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNINC, 7);
						case BSWAP16('ic'):
							if (*(uint16_t *)(p + 6) != BSWAP16('mp'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNICMP, 8);
						case BSWAP16('le'):
							if (p[6] != 'n')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNLEN, 7);
						case BSWAP16('se'):
							if (p[6] != 't')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSNSET, 7);
						}
						break;
					case 'p':
						if (*(uint32_t *)(p + 3) != BSWAP32('pbrk'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSPBRK, 7);
					case 'r':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('ch'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSRCHR, 7);
						case BSWAP16('ev'):
							if (*(uint16_t *)(p + 4) !=  BSWAP16('ev'))
								break;
							APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSREV, 6);
						case BSWAP16('ic'):
							if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSRICHR, 8);
						case BSWAP16('is'):
							if (*(uint16_t *)(p + 6) != BSWAP16('tr'))
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSRISTR, 8);
						case BSWAP16('st'):
							if (p[6] != 'r')
								break;
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSRSTR, 7);
						}
						break;
					case 's':
						switch (*(uint16_t *)(p + 4))
						{
						case BSWAP16('et'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSET, 6);
						case BSWAP16('pn'):
							if (p[6] != 'p')
								APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSPN, 6);
							else
								APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSPNP, 7);
						case BSWAP16('tr'):
							APPEND_FUNCTION_MULTI_PARAM(TAG_WCSSTR, 6);
						}
						break;
					case 't':
						if (*(uint16_t *)(p + 4) != BSWAP16('ok'))
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WCSTOK, 6);
					case 'u':
						if (*(uint16_t *)(p + 4) != BSWAP16('pr'))
							break;
						APPEND_FUNCTION_SINGLE_PARAM(TAG_WCSUPR, 6);
					}
					break;
				}
				break;
			case 'h':
				if (*(uint32_t *)(p + 1) != BSWAP32('hile'))
					break;
				if (p[5] != '(' && !__intrinsic_isspace(p[5]))
					break;
				bNextIsSeparatedLeft = TRUE;
				bCorrectTag = TRUE;
				APPEND_TAG_WITH_CONTINUE(TAG_WHILE, 5, PRIORITY_WHILE, OS_PUSH | OS_HAS_EXPR | OS_LOOP_BEGIN);
			case 'm':
				if (*(uint16_t *)(p + 2) != BSWAP16('em'))
					break;
				switch (*(uint16_t *)(p + 4))
				{
				case BSWAP16('cc'):
					if (*(uint16_t *)(p + 6) != BSWAP16('py'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMCCPY, 8);
				case BSWAP16('ch'):
					if (p[6] != 'r')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMCHR, 7);
				case BSWAP16('cm'):
					if (p[6] != 'p')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMCMP, 7);
				case BSWAP16('cp'):
					if (p[6] != 'y')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMCPY, 7);
				case BSWAP16('du'):
					if (p[6] != 'p')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMDUP, 7);
				case BSWAP16('ic'):
					switch (*(uint16_t *)(p + 6))
					{
					case BSWAP16('hr'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMICHR, 8);
					case BSWAP16('mp'):
						APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMICMP, 8);
					}
					break;
				case BSWAP16('im'):
					if (*(uint16_t *)(p + 6) != BSWAP16('em'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMIMEM, 8);
				case BSWAP16('me'):
					if (p[6] != 'm')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMMEM, 7);
				case BSWAP16('mo'):
					if (*(uint16_t *)(p + 6) != BSWAP16('ve'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMMOVE, 8);
				case BSWAP16('pc'):
					if (*(uint16_t *)(p + 6) != BSWAP16('py'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMPCPY, 8);
				case BSWAP16('rc'):
					if (*(uint16_t *)(p + 6) != BSWAP16('hr'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMRCHR, 8);
				case BSWAP16('rm'):
					if (*(uint16_t *)(p + 6) != BSWAP16('em'))
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMRMEM, 8);
				case BSWAP16('ri'):
					switch (*(uint16_t *)(p + 6))
					{
					case BSWAP16('ch'):
						if (p[8] != 'r')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMRICHR, 9);
					case BSWAP16('me'):
						if (p[8] != 'm')
							break;
						APPEND_FUNCTION_MULTI_PARAM(TAG_WMEMRIMEM, 9);
					}
					break;
				case BSWAP16('se'):
					if (p[6] != 't')
						break;
					APPEND_FUNCTION_MULTI_PARAM(TAG_MEMSET16, 7);
				}
				break;
			case 't':
				switch (*(uint16_t *)(p + 2))
				{
				case BSWAP16('oi'):
					APPEND_FUNCTION_SINGLE_PARAM(TAG_WTOI, 4);
				case BSWAP16('of'):
					APPEND_FUNCTION_SINGLE_PARAM(TAG_WTOF, 4);
				}
				break;
			}
			break;
		case '|':
			// "||", "|="
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case '|':
				APPEND_TAG(TAG_OR, 2, PRIORITY_OR, OS_PUSH | OS_SHORT_CIRCUIT);
				APPEND_TAG_WITH_CONTINUE(TAG_OR, 2, PRIORITY_OR, OS_PUSH);
			case '=':
				APPEND_TAG_WITH_CONTINUE(TAG_BIT_OR, 2, PRIORITY_LEFT_ASSIGN, OS_PUSH | OS_LEFT_ASSIGN);
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_BIT_OR, 1, PRIORITY_BIT_OR, OS_PUSH);
			}
		case '{':
			// "{"
			bNextIsSeparatedLeft = TRUE;
			if (__intrinsic_isspace(p[1]))
			{
				APPEND_TAG_WITH_CONTINUE(TAG_BRACE_OPEN, 1, PRIORITY_BRACE_OPEN, OS_OPEN | OS_BRACES);
			}
			else
			{
				LPCBYTE b = p;
				while (++b < end && *b != '}');
				if (b < end)
				{
					size_t const length = ++b - p;
					APPEND_TAG_WITH_CONTINUE(TAG_PARSE_ERROR, length, PRIORITY_NOT_OPERATOR, OS_PUSH);
				}
				p = (LPBYTE)b;
				break;
			}
		case '}':
			// "}"
			bNextIsSeparatedLeft = TRUE;
			if (bIsSeparatedLeft)
				APPEND_TAG_WITH_CONTINUE(TAG_BRACE_CLOSE, 1, PRIORITY_BRACE_CLOSE, OS_CLOSE | OS_BRACES);
			else
				APPEND_TAG_WITH_CONTINUE(TAG_PARSE_ERROR, 1, PRIORITY_NOT_OPERATOR, OS_PUSH);
		case '~':
			// "~", "~]", "~2]", "~3]", "~4]", "~5]", "~6]", "~7]", "~8]"
			bNextIsSeparatedLeft = TRUE;
			switch (p[1])
			{
			case ']':
				iTag = TAG_REV_ENDIAN4;
				nLength = 2;
				goto APPEND_REV_ENDIAN;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				if (p[2] == ']')
				{
					iTag = (TAG)(TAG_REV_ENDIAN2 + p[1] - '2');
					nLength = 3;
				APPEND_REV_ENDIAN:
					APPEND_TAG_WITH_CONTINUE(iTag, nLength, PRIORITY_REV_ENDIAN, OS_PUSH | OS_CLOSE);
				}
				/* FALLTHROUGH */
			default:
				APPEND_TAG_WITH_CONTINUE(TAG_BIT_NOT, 1, PRIORITY_BIT_NOT, OS_PUSH | OS_ANTE);
			}
			break;
#if CODEPAGE_SUPPORT
		default:
			if (!__intrinsic_isleadbyte(*p))
				break;
#else
		case_unsigned_leadbyte:
#endif
			// lead byte
			p += 2;
			continue;
		}
#if USE_PLUGIN
		if (bIsSeparatedLeft && PluginFunctions)
		{
			PLUGIN_FUNCTION_VECTOR *Vector;

			Vector = &PluginFunctionVector[*p];
			if (Vector->First)
			{
				size_t low = 0;
				size_t high = Vector->Last - Vector->First;
				do
				{
					size_t middle = (low + high - 1) / 2;
					PLUGIN_FUNCTION *Function = &Vector->First[middle];
					int ret = strncmp(Function->Name + 1, p + 1, Function->NameLength - 1);
					if (ret < 0)
					{
						low = middle + 1;
					}
					else if (ret > 0)
					{
						high = middle;
					}
					else
					{
						if (p[Function->NameLength] != '(' && !__intrinsic_isspace(p[Function->NameLength]))
						{
							BOOLEAN match = FALSE;
							while (++Function < Vector->Last && strncmp(Function->Name + 1, p + 1, Function->NameLength - 1) == 0)
								if (match = (p[Function->NameLength] == '(' || __intrinsic_isspace(p[Function->NameLength])))
									break;
							if (!match)
								break;
						}
						lpMarkup = lpTagArray + nNumberOfTag++;
						assert(("allocation error", lpMarkup < lpEndOfMarkup));
						bNextIsSeparatedLeft = TRUE;
						bCorrectTag = TRUE;
						lpMarkup->Tag             = TAG_PLUGIN;
						lpMarkup->Type            = OS_PUSH;
						lpMarkup->String          = p;
						lpMarkup->Length          = (WORD)Function->NameLength;
						lpMarkup->Priority        = PRIORITY_FUNCTION;
						lpMarkup->Depth           = 0;
						lpMarkup->Param           = NULL;
						lpMarkup->Next            = NULL;
						lpMarkup->NumberOfOperand = 0;
						lpMarkup->Function        = Function;
						p += Function->NameLength;
						goto CONTINUE;
					}
				} while (low < high);
			}
		}
#endif
		p++;

#if USE_PLUGIN
	CONTINUE:;
#endif
		#undef APPEND_TAG
		#undef APPEND_TAG_WITH_CONTINUE
		#undef IS_SEPARATED_RIGHT
	}

	lpEndOfTag = lpTagArray + nNumberOfTag;

	// add ternary block
	if (nFirstTernary != -1)
	{
		for (MARKUP *lpTag1 = lpTagArray + nFirstTernary; lpTag1 < lpEndOfTag; lpTag1++)
		{
			MARKUP *lpBegin, *lpEnd;
			size_t nDepth;

			if (lpTag1->Tag != TAG_TERNARY)
				continue;
			lpBegin = NULL;
			nDepth = 0;
			for (MARKUP *lpElement = lpTag1 - 1; lpElement >= lpTagArray; lpElement--)
			{
				if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
				{
					if (nDepth || lpElement->Priority >= PRIORITY_TERNARY)
						continue;
				}
				else if (lpElement->Type & OS_CLOSE)
				{
					nDepth++;
					continue;
				}
				else if (nDepth)
				{
					nDepth--;
					continue;
				}
				lpBegin = lpElement;
				break;
			}
			lpEnd = NULL;
			nDepth = 0;
			for (MARKUP *lpElement = lpTag1 + 1; lpElement < lpEndOfTag; lpElement++)
			{
				if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
					continue;
				else if (lpElement->Type & OS_OPEN)
				{
					nDepth++;
					continue;
				}
				else if (nDepth-- || lpElement->Tag != TAG_TERNARY_SPLIT)
					continue;
				nDepth = 0;
				lpElement->Type |= OS_TERNARIES;
				while (++lpElement < lpEndOfTag)
				{
					if (!(lpElement->Type & (OS_OPEN | OS_CLOSE)))
					{
						if (nDepth || !(lpElement->Type & OS_SPLIT) && lpElement->Priority >= PRIORITY_TERNARY)
							continue;
					}
					else if (lpElement->Type & OS_OPEN)
					{
						nDepth++;
						continue;
					}
					else if (nDepth)
					{
						nDepth--;
						continue;
					}
					lpEnd = lpElement;
					break;
				}
				break;
			}
			if (lpBegin && (lpBegin->Type & OS_OPEN) && lpEnd && (lpEnd->Type & OS_CLOSE))
			{
				lpEnd->Type |= OS_PUSH | OS_TERNARY_END;
				continue;
			}
			if (lpEnd)
				(LPBYTE)lpEnd -= (size_t)lpTagArray;
			(LPBYTE)lpTag1     -= (size_t)lpTagArray;
			(LPBYTE)lpEndOfTag -= (size_t)lpTagArray;
			if (lpBegin)
			{
				size_t  size;
				MARKUP *src;

				(LPBYTE)lpBegin -= (size_t)lpTagArray;
				nNumberOfTag++;
				assert(("allocation error", nNumberOfTag <= nSrcLength));
				size = (size_t)lpEndOfTag - (size_t)lpBegin;
				(LPBYTE)lpBegin += (size_t)lpTagArray;
				src = lpBegin++;
				memmove(lpBegin, src, size);
				lpBegin->String += lpBegin->Length;
			}
			else
			{
				nNumberOfTag++;
				assert(("allocation error", nNumberOfTag <= nSrcLength));
				memmove(lpTagArray + 1, lpTagArray, (size_t)lpEndOfTag);
				lpBegin = lpTagArray;
				lpBegin->String = lpSrc;
			}
			lpBegin->Tag        = TAG_PARENTHESIS_OPEN;
			lpBegin->Length     = 0;
			lpBegin->Priority   = PRIORITY_PARENTHESIS_OPEN;
			lpBegin->Type       = OS_OPEN | OS_PARENTHESES;
			lpBegin->Depth      = 0;
			lpBegin->Param      = NULL;
			lpBegin->Next       = NULL;
		    lpBegin->UnionBlock = 0;
			if (lpEnd)
			{
				size_t size;

				nNumberOfTag++;
				assert(("allocation error", nNumberOfTag <= nSrcLength));
				size = (size_t)lpEndOfTag - (size_t)lpEnd;
				(LPBYTE)lpEnd += (size_t)(lpTagArray + 1);
				memmove(lpEnd + 1, lpEnd, size);
			}
			else
			{
				lpEnd = lpTagArray + nNumberOfTag++;
				assert(("allocation error", lpEnd < lpEndOfMarkup));
				lpEnd->String = lpSrc + nSrcLength;
			}
			(LPBYTE)lpTag1     += (size_t)(lpTagArray + 1);
			(LPBYTE)lpEndOfTag += (size_t)(lpTagArray + 2);
			lpEnd->Tag        = TAG_PARENTHESIS_CLOSE;
			lpEnd->Length     = 0;
			lpEnd->Priority   = PRIORITY_PARENTHESIS_CLOSE;
			lpEnd->Type       = OS_PUSH | OS_CLOSE | OS_PARENTHESES | OS_TERNARY_END;
			lpEnd->Depth      = 0;
			lpEnd->Param      = NULL;
			lpEnd->Next       = NULL;
		    lpEnd->UnionBlock = 0;
		}
	}

	// allocate markup array
	lpMarkupArray = (MARKUP *)HeapAlloc(hHeap, 0, (nNumberOfTag * 2 + 1) * sizeof(MARKUP));
	if (!lpMarkupArray)
		goto FAILED1;

	// copy tags, and mark up values
	lpMarkup = lpMarkupArray;
	p = lpSrc;
	for (MARKUP *lpTag = lpTagArray; ; )
	{
		size_t length;

		if ((length = TrimMarkupString(&p, lpTag < lpEndOfTag ? lpTag[0].String : lpSrc + nSrcLength)) ||
			lpTag > lpTagArray && lpTag < lpEndOfTag && lpTag[0].Tag == TAG_DELIMITER && (
				lpTag[-1].Tag == TAG_PARENTHESIS_OPEN || lpTag[-1].Tag == TAG_DELIMITER))
		{
			size_t prefixLength;

			lpMarkup->Tag        = TAG_NOT_OPERATOR;
			lpMarkup->Type       = OS_PUSH;
			lpMarkup->String     = p;
			lpMarkup->Length     = (WORD)length;
			lpMarkup->Priority   = PRIORITY_NOT_OPERATOR;
			lpMarkup->Depth      = lpTag >= lpEndOfTag && lpTag != lpTagArray && (lpTag[-1].Tag == TAG_IF_EXPR || lpTag[-1].Tag == TAG_ELSE);
			lpMarkup->Param      = NULL;
			lpMarkup->Next       = NULL;
		    lpMarkup->UnionBlock = 0;
			if (lpMarkup->String[prefixLength = 0] != '"' && (lpMarkup->String[0] != 'u' ||
				lpMarkup->String[prefixLength = 1] != '"' && (lpMarkup->String[1] != '8' ||
				lpMarkup->String[prefixLength = 2] != '"')))
			{
				#define TAG_ADD_SUB_LENGTH 1

				char *p, *end, *next;
				UINT bound;

				if (lpTag >= lpEndOfTag)
					goto INC_MARKUP;
#if 1
				if (lpTag->Tag == TAG_PARENTHESIS_OPEN && length > 1 && *lpMarkup->String == '$')
				{
					// variable function
					lpMarkup->Tag      = TAG_FUNCTION;
					lpMarkup->Type     = OS_PUSH | OS_ANTE | OS_YIELD_OPERAND;
					lpMarkup->Priority = PRIORITY_FUNCTION;
					bCorrectTag = TRUE;
				}
				else
#endif
				if (lpTag + 3 < lpEndOfTag &&
					lpTag[0].Tag == TAG_SUB && !(lpTag[0].Type & OS_LEFT_ASSIGN) &&
					lpTag[0].String == lpMarkup->String + lpMarkup->Length &&
					lpTag[1].Tag == TAG_AT &&
					lpTag[1].String == lpTag[0].String + lpTag[0].Length &&
					(bound = 2 + (lpTag[2].String == lpTag[1].String + lpTag[1].Length), lpTag[bound].Tag == TAG_PARENTHESIS_OPEN) &&
					(end = TrimRightSpace(lpTag[1].String + lpTag[1].Length, lpTag[bound].String)) > lpTag[1].String + lpTag[1].Length)
				{
					// function
					lpMarkup->Tag      = TAG_FUNCTION;
					lpMarkup->Type     = OS_PUSH | OS_ANTE;
					lpMarkup->Length   = end - lpMarkup->String;
					lpMarkup->Priority = PRIORITY_FUNCTION;
					bCorrectTag = TRUE;
					lpTag += bound;
				}
				else if ((lpTag[0].Tag == TAG_ADD || lpTag[0].Tag == TAG_SUB) &&
					!(lpTag[0].Type & OS_LEFT_ASSIGN) &&
					(next = lpTag + 1 < lpEndOfTag ? lpTag[1].String : lpSrc + nSrcLength) > lpTag[0].String + TAG_ADD_SUB_LENGTH &&
					(end = (p = lpMarkup->String) + lpMarkup->Length) == lpTag[0].String &&
					p < --end)
				{
					// correct the scientific notation of floating point number (e-notation, p-notation)
					BOOL hex, decpt;

					decpt = hex = FALSE;
					if (*end != 'e' && *end != 'E')
					{
						if (*end != 'p' && *end != 'P' ||
							p[0] != '0' || p[1] != 'x' && p[1] != 'X' ||
							(p += 2) >= end)
							goto INC_MARKUP;
						hex = TRUE;
					}
					if (*p == '.' && p + 1 == end)
						goto INC_MARKUP;
					do
					{
						switch (*(p++))
						{
						case '.':
							if (decpt)
								break;
							decpt = TRUE;
							continue;
						case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
							continue;
						case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
						case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
							if (hex)
								continue;
							break;
						}
						goto INC_MARKUP;
					} while (p != end);
					end += 1 + TAG_ADD_SUB_LENGTH;
					for (; ; )
					{
						if (*end >= '0' && *end <= '9')
							if (++end < next)
								continue;
							else
								break;
						p = end;
						do
							if (!__intrinsic_isspace(*p))
								goto INC_MARKUP;
						while (++p < next);
						break;
					}
					lpMarkup->Length = end - lpMarkup->String;
					lpTag++;
				}
				else if (lpMarkup - 2 >= lpMarkupArray && lpMarkup[-1 - (lpMarkup[-1].Tag == TAG_SUB)].Tag == TAG_CASE)
				{
					next = (p = lpMarkup->String) + lpMarkup->Length;
					if ((--lpMarkup)->Tag == TAG_SUB)
						p = (lpMarkup--)->String;
					lpMarkup->NumberOfOperand = strtoul(p, &end, 0);
					lpMarkup->Length = next - lpMarkup->String;
					if (end < next)
						lpMarkup->Type |= OS_SHORT_CIRCUIT;
					switch (lpTag->Tag)
					{
					case TAG_SPLIT:
						if (lpMarkup == lpMarkupArray || lpMarkup[-1].Tag != TAG_GOTO)
							lpMarkup->Type |= OS_SHORT_CIRCUIT;
						break;
					case TAG_TERNARY_SPLIT:
						if (!(lpTag->Type & OS_TERNARIES))
						{
							lpTag->Type = 0;
							break;
						}
						/* FALLTHROUGH */
					default:
						lpTag->Tag = TAG_PARSE_ERROR;
						lpTag->Type |= OS_PUSH;
					}
				}
				else if (lpTag->Tag == TAG_TERNARY_SPLIT && !(lpTag->Type & OS_TERNARIES))
				{
					lpMarkup->Tag = TAG_LABEL;
					lpMarkup->Priority = PRIORITY_LABEL;
					lpTag->Type = 0;
				}

				#undef TAG_ADD_SUB_LENGTH
			}
			else
			{
				char *p, *end;

				// correct string literals
				switch (lpMarkup[-(lpMarkup != lpMarkupArray)].Tag)
				{
				case TAG_PROCEDURE:
				case TAG_IMPORT_FUNCTION:
				case TAG_IMPORT_REFERENCE:
				case TAG_MODULENAME:
				case TAG_SECTION:
				case TAG_PROCESSID:
				case TAG_MNAME:
					break;
				case TAG_CASE:
					lpMarkup[-1].Length = lpMarkup->String - lpMarkup[-1].String + lpMarkup->Length;
					lpMarkup[-1].Param = lpMarkup;
					lpMarkup[-1].UnescapedString = prefixLength;
					lpMarkup->Type = 0;
					if (lpTag->Tag == TAG_TERNARY_SPLIT && !(lpTag->Type & OS_TERNARIES))
						lpTag->Type = 0;
					else
					{
						lpTag->Tag = TAG_PARSE_ERROR;
						lpTag->Type |= OS_PUSH;
					}
				default:
					lpMarkup->Type |= OS_STRING;
					goto INC_MARKUP;
				}
				end = (p = lpMarkup->String) + lpMarkup->Length;
				for (; ; )
				{
					char   c;
					size_t size;

					if ((c = *(p++)) != '"')
						if (p < end && !__intrinsic_isleadbyte(c) || ++p < end)
							continue;
						else
							break;
					if (!(size = --end - --p))
						break;
					memcpy(p, p + 1, size);
					if (*(p = UnescapeA(p, &end, FALSE)) != '"')
						break;
					if (!(size = --end - p))
						break;
					memcpy(p, p + 1, size);
				}
				lpMarkup->Length = end - lpMarkup->String;
			}
		INC_MARKUP:
			lpMarkup++;
		}
		if (lpTag >= lpEndOfTag)
			break;
		*lpMarkup = *(lpTag++);
		p = lpMarkup->String + lpMarkup->Length;
		lpMarkup++;
	}
	lpEndOfMarkup = lpMarkup;

	// shrink
	(LPBYTE)lpEndOfMarkup -= (size_t)lpMarkupArray;
	if (!(lpMem = HeapReAlloc(hHeap, 0, lpMarkupArray, (size_t)lpEndOfMarkup)))
		goto FAILED2;
	lpMarkupArray = (MARKUP *)lpMem;
	(LPBYTE)lpEndOfMarkup += (size_t)lpMarkupArray;

	// release
	HeapFree(hHeap, 0, lpTagArray);

	// correct tag
	if (bCorrectTag)
	{
		MARKUP *lpFirstDoWhileLoop;

		lpFirstDoWhileLoop = NULL;
		for (lpMarkup = lpMarkupArray; lpMarkup < lpEndOfMarkup; lpMarkup++)
		{
			switch (lpMarkup->Tag)
			{
			default:
				continue;
			case TAG_IF:
				// correct if block
				{
					MARKUP *lpOpen, *lpClose, *lpBegin, *lpEnd, *lpElse;

					if ((lpOpen = lpMarkup + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpBegin = (lpClose = FindBracketClose(lpOpen + 1, lpEndOfMarkup)) + 1) >= lpEndOfMarkup)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpBegin, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					if ((lpElse = lpEnd + 1) < lpEndOfMarkup && lpElse->Tag == TAG_ELSE)
						lpElse->Type = OS_PUSH;
					lpClose->Tag = TAG_IF_EXPR;
					lpClose->Type |= OS_PUSH | OS_SPLIT;
					lpClose->Param = lpMarkup;
					lpEnd->Type |= OS_SPLIT;
					for (MARKUP *lpElement = lpBegin; lpElement < lpEnd; lpElement++)
						lpElement->Depth++;
				}
				continue;
			case TAG_ELSE:
				// correct else block
				{
					MARKUP *lpBegin, *lpEnd;

					if (!lpMarkup->Type)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpBegin = lpMarkup + 1, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					lpEnd->Type |= OS_SPLIT;
					for (MARKUP *lpElement = lpBegin; lpElement < lpEnd; lpElement++)
						lpElement->Depth++;
				}
				continue;
			case TAG_SWITCH:
				// correct switch block
				{
					MARKUP *lpOpen, *lpClose, *lpEnd;

					if ((lpOpen = lpMarkup + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpEnd = (lpClose = FindBracketClose(lpOpen + 1, lpEndOfMarkup)) + 1) >= lpEndOfMarkup)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpEnd, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					lpClose->Tag = TAG_SWITCH_EXPR;
					lpClose->Type |= OS_PUSH | OS_SPLIT;
					lpClose->Param = lpMarkup;
					lpEnd->Type |= OS_PUSH | OS_SPLIT;
					lpMarkup->Param = lpClose;
					lpMarkup->Next  = lpEnd;
					if (!lpFirstSwitch)
						lpFirstSwitch = lpMarkup;
				}
				continue;
			case TAG_DO:
				// correct do-while loop
				{
					MARKUP *lpWhile, *lpEnd, *lpNext;

					if ((lpWhile = FindEndOfStructuredStatement(lpMarkup + 1, lpEndOfMarkup) + 1) + 2 >= lpEndOfMarkup)
						break;
					if (lpWhile[0].Tag != TAG_WHILE)
						break;
					if (lpWhile[1].Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpNext = (lpEnd = FindBracketClose(lpWhile + 2, lpEndOfMarkup)) + 1) >= lpEndOfMarkup)
						break;
					if (lpNext->Tag != TAG_SPLIT)
						break;
					if (++lpNext < lpEndOfMarkup && lpNext->Tag == TAG_ELSE)
						lpNext->Type = OS_PUSH;
					lpWhile->Type = OS_PUSH | OS_HAS_EXPR | OS_POST;
					lpEnd->Tag = TAG_WHILE_EXPR;
					lpEnd->Type |= OS_PUSH | OS_POST | OS_LOOP_END;
					lpEnd->Param = lpWhile;
					if (!lpFirstDoWhileLoop)
						lpFirstDoWhileLoop = lpMarkup;
				}
				continue;
			case TAG_WHILE:
				// correct while loop
				{
					MARKUP *lpOpen, *lpClose, *lpEnd, *lpElse;

					if (lpMarkup->Type & OS_POST)
						continue;
					if ((lpOpen = lpMarkup + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpEnd = (lpClose = FindBracketClose(lpOpen + 1, lpEndOfMarkup)) + 1) >= lpEndOfMarkup)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpEnd, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					if ((lpElse = lpEnd + 1) < lpEndOfMarkup && lpElse->Tag == TAG_ELSE)
						lpElse->Type = OS_PUSH;
					lpClose->Tag = TAG_WHILE_EXPR;
					lpClose->Type |= OS_PUSH | OS_SPLIT;
					lpClose->Param = lpMarkup;
					lpEnd->Type |= OS_PUSH | OS_SPLIT | OS_LOOP_END;
				}
				continue;
			case TAG_FOR:
				// correct for loop
				{
					MARKUP *lpOpen, *lpInitialize, *lpCondition, *lpUpdate, *lpEnd, *lpElse;

					if ((lpOpen = lpMarkup + 1) >= lpEndOfMarkup)
						break;
					if (lpOpen->Tag != TAG_PARENTHESIS_OPEN)
						break;
					if ((lpEnd = (lpUpdate = FindBracketClose(lpOpen + 1, lpEndOfMarkup)) + 1) >= lpEndOfMarkup)
						break;
					if ((lpInitialize = FindSplit(lpOpen + 1, lpUpdate)) >= lpUpdate)
						break;
					if ((lpCondition = FindSplit(lpInitialize + 1, lpUpdate)) >= lpUpdate)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpEnd, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					if ((lpElse = lpEnd + 1) < lpEndOfMarkup && lpElse->Tag == TAG_ELSE)
						lpElse->Type = OS_PUSH;
					lpInitialize->Tag = TAG_FOR_INITIALIZE;
					lpInitialize->Type |= OS_PUSH;
					lpCondition->Tag = TAG_FOR_CONDITION;
					lpCondition->Type |= OS_PUSH | OS_LOOP_BEGIN;
					lpCondition->Param = lpMarkup;
					lpUpdate->Tag = TAG_FOR_UPDATE;
					lpUpdate->Type |= OS_PUSH | OS_SPLIT;
					lpEnd->Type |= OS_PUSH | OS_SPLIT | OS_LOOP_END;
				}
				continue;
			case TAG_PARSE_INT:       // parse_int
			case TAG_PARSE_REAL:      // parse_real
			case TAG_PARSE_RESET:     // parse_reset
				{
					MARKUP *lpOpen, *lpClose;

					if ((lpClose = (lpOpen = lpMarkup + 1) + 1) >= lpEndOfMarkup)
						continue;
					if (!(lpOpen->Type & OS_OPEN && lpOpen->Type & (OS_PARENTHESES | OS_BRACES)))
						continue;
					if (lpClose->Type & OS_CLOSE && lpClose->Type & (OS_PARENTHESES | OS_BRACES))
						continue;
					if ((lpClose = FindBracketClose(lpClose, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					(lpMarkup->Close = lpClose)->Type |= OS_PUSH;
				}
				continue;
			case TAG_TICK:            // tick
			case TAG_RAND32:          // rand32
			case TAG_RAND64:          // rand64
#if USE_PLUGIN
			case TAG_PLUGIN:          // plugin function
#endif
			case TAG_FUNCTION:        // function
				if (CorrectFunction(lpMarkup, lpEndOfMarkup, 0))
					continue;
				break;
			case TAG_ASSERT:          // assert
				{
					MARKUP *lpBegin, *lpEnd;

					lpMarkup->Type |= OS_VERIFY;
					if ((lpBegin = lpMarkup + 1) >= lpEndOfMarkup)
						break;
					if ((lpEnd = FindEndOfStructuredStatement(lpBegin, lpEndOfMarkup)) >= lpEndOfMarkup)
						break;
					lpMarkup->Close = lpEnd;
					for (MARKUP *lpElement = lpBegin; lpElement <= lpEnd; lpElement++)
						lpElement->Type |= OS_VERIFY;
				}
				/* FALLTHROUGH */
			case TAG_MNAME:           // MName
			case TAG_PROCESSID:       // ProcessId
			case TAG_HNUMBER:         // HNumber
			case TAG_MEMORY:          // Memory
			case TAG_ISBADCODEPTR:    // IsBadCodePtr
			case TAG_CAST32:          // Cast32
			case TAG_CAST64:          // Cast64
			case TAG_I1TOI4:          // I1toI4, cbd
			case TAG_I2TOI4:          // I2toI4, cwd
			case TAG_I4TOI8:          // I4toI8, cdq
			case TAG_UTOF:            // utof
			case TAG_ITOF:            // itof
			case TAG_FTOI:            // ftoi
			case TAG_TRUNC:           // trunc
			case TAG_ROUND:           // round
			case TAG_ISFINITE:        // isfinite
			case TAG_ISINF:           // isinf
			case TAG_ISNAN:           // isnan
			case TAG_BSF:             // BitScanForward, bsf
			case TAG_BSR:             // BitScanReverse, bsr
			case TAG_A2U:             // A2U
			case TAG_A2W:             // A2W
			case TAG_U2A:             // U2A
			case TAG_U2W:             // U2W
			case TAG_W2A:             // W2A
			case TAG_W2U:             // W2U
			case TAG_WAIT:            // wait
			case TAG_SLEEP:           // sleep
			case TAG_ALLOCA:          // alloca
			case TAG_ATOI:            // atoi
			case TAG_WTOI:            // wtoi
			case TAG_ATOF:            // atof
			case TAG_WTOF:            // wtof
			case TAG_PRINTF:          // printf
			case TAG_DPRINTF:         // dprintf
			case TAG_STRDUP:          // strdup
			case TAG_WCSDUP:          // wcsdup
			case TAG_STRINC:          // strinc
			case TAG_WCSINC:          // wcsinc
			case TAG_MBSINC:          // mbsinc
			case TAG_STRNEXTC:        // strnextc
			case TAG_WCSNEXTC:        // wcsnextc
			case TAG_MBSNEXTC:        // mbsnextc
			case TAG_STRLEN:          // strlen
			case TAG_WCSLEN:          // wcslen
			case TAG_MBSLEN:          // mbslen
			case TAG_STRLWR:          // strlwr
			case TAG_WCSLWR:          // wcslwr
			case TAG_MBSLWR:          // mbslwr
			case TAG_STRUPR:          // strupr
			case TAG_WCSUPR:          // wcsupr
			case TAG_MBSUPR:          // mbsupr
			case TAG_STRREV:          // strrev
			case TAG_WCSREV:          // wcsrev
			case TAG_MBSREV:          // mbsrev
			case TAG_ISALNUM:         // isalnum
			case TAG_ISALPHA:         // isalpha
			case TAG_ISASCII:         // isascii
			case TAG_ISBLANK:         // isblank
			case TAG_ISCNTRL:         // iscntrl
			case TAG_ISCSYM:          // iscsym
			case TAG_ISCSYMF:         // iscsymf
			case TAG_ISDIGIT:         // isdigit
			case TAG_ISGRAPH:         // isgraph
			case TAG_ISKANA:          // iskana
			case TAG_ISLEADBYTE:      // isleadbyte
			case TAG_ISLOWER:         // islower
			case TAG_ISPRINT:         // isprint
			case TAG_ISPUNCT:         // ispunct
			case TAG_ISSPACE:         // isspace
			case TAG_ISTRAILBYTE:     // istrailbyte
			case TAG_ISUPPER:         // isupper
			case TAG_ISXDIGIT:        // isxdigit
			case TAG_ISMBBALNUM:      // ismbbalnum
			case TAG_ISMBBALPHA:      // ismbbalpha
			case TAG_ISMBBGRAPH:      // ismbbgraph
			case TAG_ISMBBPRINT:      // ismbbprint
			case TAG_ISMBCALNUM:      // ismbcalnum
			case TAG_ISMBCALPHA:      // ismbcalpha
			case TAG_ISMBCDIGIT:      // ismbcdigit
			case TAG_ISMBCGRAPH:      // ismbcgraph
			case TAG_ISMBCHIRA:       // ismbchira
			case TAG_ISMBCKATA:       // ismbckata
			case TAG_ISMBCL0:         // ismbcl0
			case TAG_ISMBCL1:         // ismbcl1
			case TAG_ISMBCL2:         // ismbcl2
			case TAG_ISMBCLEGAL:      // ismbclegal
			case TAG_ISMBCLOWER:      // ismbclower
			case TAG_ISMBCPRINT:      // ismbcprint
			case TAG_ISMBCPUNCT:      // ismbcpunct
			case TAG_ISMBCSPACE:      // ismbcspace
			case TAG_ISMBCSYMBOL:     // ismbcsymbol
			case TAG_ISMBCUPPER:      // ismbcupper
			case TAG_ISWALNUM:        // iswalnum
			case TAG_ISWALPHA:        // iswalpha
			case TAG_ISWASCII:        // iswascii
			case TAG_ISWBLANK:        // iswblank
			case TAG_ISWCNTRL:        // iswcntrl
			case TAG_ISWCSYM:         // iswcsym
			case TAG_ISWCSYMF:        // iswcsymf
			case TAG_ISWDIGIT:        // iswdigit
			case TAG_ISWGRAPH:        // iswgraph
			case TAG_ISWLOWER:        // iswlower
			case TAG_ISWPRINT:        // iswprint
			case TAG_ISWPUNCT:        // iswpunct
			case TAG_ISWSPACE:        // iswspace
			case TAG_ISWUPPER:        // iswupper
			case TAG_ISWXDIGIT:       // iswxdigit
			case TAG_TOASCII:         // toascii
			case TAG_TOLOWER:         // tolower
			case TAG_TOUPPER:         // toupper
				if (lpMarkup->Length < 2 || *(uint16_t *)(lpMarkup->String + lpMarkup->Length - 2) != '::')
				{
					if (!CorrectFunction(lpMarkup, lpEndOfMarkup, 1))
						break;
					if (lpMarkup->Tag != TAG_MNAME && lpMarkup->Tag != TAG_PROCESSID)
						continue;
					if (lpMarkup->NumberOfOperand == 1)
						continue;
					lpMarkup->Type &= ~OS_ANTE;
				}
				else
				{
					MARKUP *lpParam;

					lpParam = lpMarkup;
					do
						if (++lpParam >= lpEndOfMarkup)
							goto PARSING_ERROR;
					while (lpParam->Tag == TAG_PARENTHESIS_OPEN);
					lpMarkup->Param = lpParam;
					lpMarkup->NumberOfOperand = 1;
				}
				continue;
			case TAG_ISBADREADPTR:    // IsBadReadPtr
			case TAG_ISBADWRITEPTR:   // IsBadWritePtr
			case TAG_ISBADSTRINGPTRA: // IsBadStringPtrA
			case TAG_ISBADSTRINGPTRW: // IsBadStringPtrW
			case TAG_ROTL8:           // rotl8
			case TAG_ROTL16:          // rotl16
			case TAG_ROTL32:          // rotl32, rotl
			case TAG_ROTL64:          // rotl64
			case TAG_ROTR8:           // rotr8
			case TAG_ROTR16:          // rotr16
			case TAG_ROTR32:          // rotr32, rotr
			case TAG_ROTR64:          // rotr64
#if ALLOCATE_SUPPORT
			case TAG_REALLOC:         // realloc
#endif
			case TAG_MIN:             // min
			case TAG_MAX:             // max
			case TAG_IMIN:            // imin
			case TAG_IMAX:            // imax
			case TAG_MEMDUP:          // memdup
			case TAG_WMEMDUP:         // wmemdup
			case TAG_STRDEC:          // strdec
			case TAG_WCSDEC:          // wcsdec
			case TAG_MBSDEC:          // mbsdec
			case TAG_STRNINC:         // strninc
			case TAG_WCSNINC:         // wcsninc
			case TAG_MBSNINC:         // mbsninc
			case TAG_STRNLEN:         // strnlen
			case TAG_WCSNLEN:         // wcsnlen
			case TAG_MBSNLEN:         // mbsnlen
			case TAG_MBSNBCNT:        // mbsnbcnt
			case TAG_MBSNCCNT:        // mbsnccnt
			case TAG_STRCMP:          // strcmp
			case TAG_WCSCMP:          // wcscmp
			case TAG_STRICMP:         // stricmp
			case TAG_WCSICMP:         // wcsicmp
			case TAG_MBSICMP:         // mbsicmp
			case TAG_STRCPY:          // strcpy
			case TAG_WCSCPY:          // wcscpy
			case TAG_STPCPY:          // stpcpy
			case TAG_WCPCPY:          // wcpcpy
			case TAG_STRCAT:          // strcat
			case TAG_WCSCAT:          // wcscat
			case TAG_STRCHR:          // strchr
			case TAG_WCSCHR:          // wcschr
			case TAG_MBSCHR:          // mbschr
			case TAG_STRICHR:         // strichr
			case TAG_WCSICHR:         // wcsichr
			case TAG_MBSICHR:         // mbsichr
			case TAG_STRRCHR:         // strrchr
			case TAG_WCSRCHR:         // wcsrchr
			case TAG_MBSRCHR:         // mbsrchr
			case TAG_STRRICHR:        // strrichr
			case TAG_WCSRICHR:        // wcsrichr
			case TAG_MBSRICHR:        // mbsrichr
			case TAG_STRSTR:          // strstr
			case TAG_WCSSTR:          // wcsstr
			case TAG_MBSSTR:          // mbsstr
			case TAG_STRISTR:         // stristr
			case TAG_WCSISTR:         // wcsistr
			case TAG_MBSISTR:         // mbsistr
			case TAG_STRRSTR:         // strrstr
			case TAG_WCSRSTR:         // wcsrstr
			case TAG_MBSRSTR:         // mbsrstr
			case TAG_STRRISTR:        // strristr
			case TAG_WCSRISTR:        // wcsristr
			case TAG_MBSRISTR:        // mbsristr
			case TAG_STRSPN:          // strspn
			case TAG_WCSSPN:          // wcsspn
			case TAG_MBSSPN:          // mbsspn
			case TAG_STRSPNP:         // strspnp
			case TAG_WCSSPNP:         // wcsspnp
			case TAG_MBSSPNP:         // mbsspnp
			case TAG_STRCSPN:         // strcspn
			case TAG_WCSCSPN:         // wcscspn
			case TAG_MBSCSPN:         // mbscspn
			case TAG_STRPBRK:         // strpbrk
			case TAG_WCSPBRK:         // wcspbrk
			case TAG_MBSPBRK:         // mbspbrk
			case TAG_STRSET:          // strset
			case TAG_WCSSET:          // wcsset
			case TAG_MBSSET:          // mbsset
			case TAG_STRTOK:          // strtok
			case TAG_WCSTOK:          // wcstok
			case TAG_MBSTOK:          // mbstok
				if (CorrectFunction(lpMarkup, lpEndOfMarkup, 2))
					continue;
				break;
			case TAG_MEMCMP:          // memcmp
			case TAG_WMEMCMP:         // wmemcmp
			case TAG_MEMICMP:         // memicmp
			case TAG_WMEMICMP:        // wmemicmp
			case TAG_MEMCPY:          // memcpy
			case TAG_WMEMCPY:         // wmemcpy
			case TAG_MEMPCPY:         // mempcpy
			case TAG_WMEMPCPY:        // wmempcpy
			case TAG_MEMMOVE:         // memmove
			case TAG_WMEMMOVE:        // wmemmove
			case TAG_MEMSET8:         // memset8, memset
			case TAG_MEMSET16:        // memset16, wmemset
			case TAG_MEMSET32:        // memset32
			case TAG_MEMSET64:        // memset64
			case TAG_MEMCHR:          // memchr
			case TAG_WMEMCHR:         // wmemchr
			case TAG_MEMICHR:         // memichr
			case TAG_WMEMICHR:        // wmemichr
			case TAG_MEMRCHR:         // memrchr
			case TAG_WMEMRCHR:        // wmemrchr
			case TAG_MEMRICHR:        // memrichr
			case TAG_WMEMRICHR:       // wmemrichr
			case TAG_SNPRINTF:        // snprintf
			case TAG_SNWPRINTF:       // snwprintf
			case TAG_STRNCMP:         // strncmp
			case TAG_WCSNCMP:         // wcsncmp
			case TAG_MBSNCMP:         // mbsncmp
			case TAG_STRNICMP:        // strnicmp
			case TAG_WCSNICMP:        // wcsnicmp
			case TAG_MBSNBICMP:       // mbsnbicmp
			case TAG_MBSNICMP:        // mbsnicmp
			case TAG_STRNCPY:         // strncpy
			case TAG_WCSNCPY:         // wcsncpy
			case TAG_MBSNBCPY:        // mbsnbcpy
			case TAG_MBSNCPY:         // mbsncpy
			case TAG_STPNCPY:         // stpncpy
			case TAG_WCPNCPY:         // wcpncpy
			case TAG_STRNCAT:         // strncat
			case TAG_WCSNCAT:         // wcsncat
			case TAG_MBSNBCAT:        // mbsnbcat
			case TAG_MBSNCAT:         // mbsncat
			case TAG_STRLCPY:         // strlcpy
			case TAG_WCSLCPY:         // wcslcpy
			case TAG_STRLCAT:         // strlcat
			case TAG_WCSLCAT:         // wcslcat
			case TAG_STRNSET:         // strnset
			case TAG_WCSNSET:         // wcsnset
			case TAG_MBSNBSET:        // mbsnbset
			case TAG_MBSNSET:         // mbsnset
				if (CorrectFunction(lpMarkup, lpEndOfMarkup, 3))
					continue;
				break;
			case TAG_MEMCCPY:         // memccpy
			case TAG_WMEMCCPY:        // wmemccpy
			case TAG_MEMMEM:          // memmem
			case TAG_WMEMMEM:         // wmemmem
			case TAG_MEMIMEM:         // memimem
			case TAG_WMEMIMEM:        // wmemimem
			case TAG_MEMRMEM:         // memrmem
			case TAG_WMEMRMEM:        // wmemrmem
			case TAG_MEMRIMEM:        // memrimem
			case TAG_WMEMRIMEM:       // wmemrimem
				if (CorrectFunction(lpMarkup, lpEndOfMarkup, 4))
					continue;
				break;
			}
		PARSING_ERROR:
			lpMarkup->Tag = TAG_PARSE_ERROR;
			lpMarkup->Type |= OS_PUSH;
		}

		if (lpFirstDoWhileLoop)
		{
			// correct continue of do-while loop
			for (MARKUP *lpMarkup = lpFirstDoWhileLoop; lpMarkup < lpEndOfMarkup; lpMarkup++)
			{
				size_t nDepth;

				if (lpMarkup->Tag != TAG_DO)
					continue;
				nDepth = 0;
				for (MARKUP *lpMarkup2 = lpMarkup + 1; lpMarkup2 < lpEndOfMarkup; lpMarkup2++)
				{
					if (lpMarkup2->Type & OS_LOOP_BEGIN)
					{
						nDepth++;
					}
					else if (lpMarkup2->Type & OS_LOOP_END)
					{
						if (!nDepth)
							break;
						nDepth--;
					}
					else if (!nDepth && lpMarkup2->Tag == TAG_CONTINUE)
					{
						lpMarkup2->Type |= OS_POST;
					}
				}
			}
		}
	}

	nDepth = 0;
	for (lpMarkup = lpMarkupArray; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		// get depth of nested loop
		lpMarkup->LoopDepth =
			(lpMarkup->Type & OS_LOOP_BEGIN) ? nDepth++ :
			(lpMarkup->Type & OS_LOOP_END && nDepth) ? --nDepth :
			nDepth;
		lpMarkup->Depth += lpMarkup->LoopDepth;

		// correct operators
		switch (lpMarkup->Tag)
		{
		case TAG_INC:
		case TAG_DEC:
			if (lpMarkup + 1 < lpEndOfMarkup)
				if (lpMarkup[1].Tag == TAG_NOT_OPERATOR)
					break;
			// post increment/decrement operator
			if (lpMarkup == lpMarkupArray)
				break;
			if (lpMarkup[-1].Tag != TAG_NOT_OPERATOR)
				break;
			lpMarkup->Type = OS_PUSH | OS_POST;
			lpMarkup->Priority = PRIORITY_POST_INC_DEC;
			break;
		case TAG_ADD:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & (OS_SPLIT | OS_TERNARIES)))
					break;
			// plus-sign operator (remove)
			lpMarkup->Type = 0;
			break;
		case TAG_SUB:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & (OS_SPLIT | OS_TERNARIES)))
					break;
			// negative operator
			lpMarkup->Tag = TAG_NEG;
			lpMarkup->Type = OS_PUSH | OS_ANTE;
			lpMarkup->Priority = PRIORITY_NEG;
			break;
		case TAG_MUL:
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & (OS_SPLIT | OS_TERNARIES)))
					break;
			// indirection operator
			lpMarkup->Tag = TAG_INDIRECTION;
			lpMarkup->Type = OS_PUSH | OS_ANTE;
			lpMarkup->Priority = PRIORITY_INDIRECTION;
			break;
		case TAG_BIT_AND:
			if (lpMarkup + 1 >= lpEndOfMarkup)
				break;
			if (lpMarkup[1].Tag != TAG_NOT_OPERATOR)
				break;
			if (lpMarkup->Type & OS_LEFT_ASSIGN)
				break;
			if (lpMarkup != lpMarkupArray)
				if (lpMarkup[-1].Tag == TAG_NOT_OPERATOR ||
					lpMarkup[-1].Type & (OS_CLOSE | OS_POST) && !(lpMarkup[-1].Type & (OS_SPLIT | OS_TERNARIES)))
					break;
			// address-of operator
			lpMarkup->Tag = TAG_ADDRESS_OF;
			lpMarkup->Type = OS_PUSH | OS_ANTE;
			lpMarkup->Priority = PRIORITY_ADDRESS_OF;
			break;
		case TAG_MODULENAME:
			lpMarkup->Length--;
			break;
		case TAG_DEFAULT:
			if (lpMarkup + 1 < lpEndOfMarkup) switch (lpMarkup[1].Tag)
			{
			case TAG_SPLIT:
				if (lpMarkup == lpMarkupArray || lpMarkup[-1].Tag != TAG_GOTO)
					lpMarkup->Type |= OS_SHORT_CIRCUIT;
				break;
			case TAG_TERNARY_SPLIT:
				if (!(lpMarkup[1].Type & OS_TERNARIES))
				{
					lpMarkup[1].Type = 0;
					break;
				}
				/* FALLTHROUGH */
			default:
				lpMarkup[1].Tag = TAG_PARSE_ERROR;
				lpMarkup[1].Type |= OS_PUSH;
			}
			break;
		}
	}

	// setup switch block
	if (lpFirstSwitch) for (lpMarkup = lpFirstSwitch; lpMarkup < lpEndOfMarkup; lpMarkup++) if (lpMarkup->Tag == TAG_SWITCH)
	{
		MARKUP *lpLink = lpMarkup->Param;// TAG_SWITCH_EXPR
		for (MARKUP *lpCode = lpMarkup->Param + 1; lpCode < lpMarkup->Next; lpCode++) switch (lpCode->Tag)
		{
		case TAG_SWITCH:
			lpCode = lpCode->Next;// postpone nested switch
			continue;
		case TAG_BREAK:
			if (lpCode->LoopDepth != lpMarkup->LoopDepth)
				continue;
			lpCode->Next = lpMarkup->Next;
			break;
		case TAG_GOTO:
			if (++lpCode >= lpMarkup->Next ||
				lpCode->Tag != TAG_CASE && lpCode->Tag != TAG_DEFAULT)
				continue;
			lpCode->Next = lpMarkup->Param;// jump resolver
#ifndef GOTO_ALLOW_CASE_STRING
			if (!lpCode->Param)
#endif
				lpCode->Type &= ~OS_YIELD_OPERAND;
			break;
		case TAG_CASE:
#ifdef SWITCH_LIMITED_DEPTH
			if (lpCode->Depth != lpMarkup->Depth) continue;
#endif
			lpCode->Type &= ~OS_YIELD_OPERAND;
			if (lpLink->Tag != TAG_DEFAULT)
				lpLink = lpLink->Next = lpCode;
			else// swap chain
			{
				lpCode->Next = lpLink;
				lpLink->Param->Next = lpCode;
				lpLink->Param = lpCode;
			}
			break;
		case TAG_DEFAULT:
#ifdef SWITCH_LIMITED_DEPTH
			if (lpCode->Depth != lpMarkup->Depth) continue;
#endif
			lpCode->Type &= ~OS_YIELD_OPERAND;
			lpCode->Param = lpLink;
			lpLink = lpLink->Next = lpCode;
			break;
		}
		lpLink->Next = lpMarkup->Next;// sentinel (end switch)
		if (lpLink->Tag == TAG_DEFAULT)
			lpLink->Param = NULL;
		lpMarkup = lpMarkup->Param;
	}

	// return
	*lpnNumberOfMarkup = lpEndOfMarkup - lpMarkupArray;
	return lpMarkupArray;

FAILED2:
	HeapFree(hHeap, 0, lpMarkupArray);
FAILED1:
	HeapFree(hHeap, 0, lpTagArray);
	return NULL;
}
//---------------------------------------------------------------------
#define IsStringOperand(markup) \
	((markup)->Type & OS_STRING)
//---------------------------------------------------------------------
static BOOLEAN __fastcall CheckStringOperand(const MARKUP *element, size_t *prefixLength)
{
	if (!IsStringOperand(element))
		return FALSE;
	if (element->String[0] == '"')
		*prefixLength = 0;
	else if (element->String[1] == '"')
		*prefixLength = 1;
	else
		*prefixLength = 2;
	return TRUE;
}
//---------------------------------------------------------------------
static BOOL __fastcall UnescapeConstStrings(IN MARKUP *lpMarkupArray, IN MARKUP *lpEndOfMarkup, OUT LPVOID *lplpConstStringBuffer)
{
	LPBYTE lpBuffer, lpFirst, lpLast, p;
	size_t nSizeOfBuffer, nRegion;
	DWORD  dwProtect;

	nSizeOfBuffer = 0;
	for (MARKUP *lpMarkup = lpMarkupArray; lpMarkup != lpEndOfMarkup; lpMarkup++)
	{
		size_t nPrefixLength, nSize;

		if (!CheckStringOperand(lpMarkup, &nPrefixLength))
			continue;

		// assert(1 == strlen(  "\""));
		// assert(2 == strlen( "u\""));
		// assert(3 == strlen("u8\""));

		// assert(1 == strlen(  "ｱ")                  );
		// assert(2 == wcslen( L"ｱ") * sizeof(wchar_t));
		// assert(3 == strlen(u8"ｱ")                  );

		switch (nPrefixLength)
		{
		case 0:
			nSize = (lpMarkup->Length - 1) * 1 + 15;
			break;
		case 1:
			nSize = (lpMarkup->Length - 2) * 2 + 15;
			break;
		case 2:
			nSize = (lpMarkup->Length - 3) * 3 + 15;
			break;
		default:
			__assume(0);
		}
		nSize &= -16;
		nSizeOfBuffer += nSize;
	}
	if (!nSizeOfBuffer)
		return TRUE;
	if (lplpConstStringBuffer)
	{
		if (!(lpBuffer = VirtualAlloc(NULL, nSizeOfBuffer, MEM_COMMIT, PAGE_READWRITE)))
			return FALSE;
		nRegion = 0;
		lpFirst = *lplpConstStringBuffer = lpBuffer;
	}
	else if (lpReadOnlyBuffer)
	{
		size_t nOldSize, nNewSize;

		if (lpConstStringRegion)
			VirtualProtect(lpConstStringRegion, nSizeOfConstStringRegion, PAGE_READWRITE, &dwProtect);
		nNewSize = (nOldSize = nSizeOfConstStringRegion + PAGE_SIZE - 1) + nSizeOfBuffer;
		nOldSize &= -PAGE_SIZE;
		nNewSize &= -PAGE_SIZE;
		if (nOldSize != nNewSize)
		{
			LPBYTE lpMem;

			nNewSize += PAGE_SIZE - 1;
			if (!(lpMem = (LPBYTE)HeapReAlloc(hReadOnlyHeap, 0, lpReadOnlyBuffer, nNewSize)))
				goto FAILED;
			lpConstStringRegion = (LPBYTE)((size_t)((lpReadOnlyBuffer = lpMem) + PAGE_SIZE - 1) & -PAGE_SIZE);
		}
		lpFirst = (lpBuffer = lpConstStringRegion) + (nRegion = nSizeOfConstStringRegion);
	}
	else
	{
		if (!(lpReadOnlyBuffer = (LPBYTE)HeapAlloc(hReadOnlyHeap, 0, ((nSizeOfBuffer + PAGE_SIZE - 1) & -PAGE_SIZE) + PAGE_SIZE - 1)))
			return FALSE;
		nRegion = 0;
		lpFirst = lpBuffer = lpConstStringRegion = (LPBYTE)((size_t)(lpReadOnlyBuffer + PAGE_SIZE - 1) & -PAGE_SIZE);
	}
	lpLast = (p = lpFirst) + nSizeOfBuffer;
	for (MARKUP *lpMarkup = lpMarkupArray; lpMarkup != lpEndOfMarkup; lpMarkup++)
	{
		size_t nPrefixLength, cbMultiByte;
		LPCSTR lpMultiByteStr;

		if (!CheckStringOperand(lpMarkup, &nPrefixLength))
			continue;
		lpMarkup->UnescapedString = nRegion + (p - lpFirst);
		lpMultiByteStr = lpMarkup->String + nPrefixLength + 1;
		cbMultiByte = lpMarkup->Length - nPrefixLength - 1;
		switch (nPrefixLength)
		{
		case 0:
			{
				BYTE *end;

				memcpy(p, lpMultiByteStr, cbMultiByte);
				end = p + cbMultiByte;
				while (*(p = UnescapeA(p, &end, FALSE)) == '"')
				{
					BYTE   *next, c;
					size_t size;

					if ((next = p + 1) >= end)
						break;
					do
						c = *(next++);
					while (__intrinsic_isspace(c) && next < end);
					if (c != '"' || !(size = (end -= next - p) - p))
						break;
					memcpy(p, next, size);
				}
				*(p++) = '\0';
			}
			break;
		case 1:
			{
				#define p ((wchar_t *)p)

				size_t  cchWideChar;
				wchar_t *end;

				cchWideChar = (unsigned int)MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cbMultiByte, p, (LPWSTR)lpLast - p);
				end = p + cchWideChar;
				while (*(p = UnescapeW(p, &end, FALSE)) == L'"')
				{
					wchar_t *next, c;
					size_t  size;

					if ((next = p + 1) >= end)
						break;
					do
						c = *(next++);
					while (__intrinsic_iswspace(c) && next < end);
					if (c != L'u' || next >= end || *(next++) != L'"' || !(size = ((char *)end -= (char *)next - (char *)p) - (char *)p))
						break;
					memcpy(p, next, size);
				}
				*(p++) = L'\0';

				#undef p
			}
			break;
		case 2:
			{
				size_t cchWideChar;

				if (cchWideChar = (unsigned int)MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cbMultiByte, NULL, 0))
				{
					LPWSTR lpWideCharStr;
					size_t cbUtf8;
					LPBYTE end;

					if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar + (size_t)cchWideChar)))
						goto FAILED;
					MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
					cbUtf8 = (unsigned int)WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, cchWideChar, p, lpLast - p, NULL, NULL);
					HeapFree(hHeap, 0, lpWideCharStr);
					end = p + cbUtf8;
					while (*(p = UnescapeU(p, &end, FALSE)) == '"')
					{
						unsigned char *next, c;
						size_t        size;

						if ((next = p + 1) >= end)
							break;
						do
							c = *(next++);
						while (__intrinsic_isspace(c) && next < end);
						if (c != 'u' || next >= end || *(next++) != '8' || next >= end || *(next++) != '"' || !(size = (end -= next - p) - p))
							break;
						memcpy(p, next, size);
					}
				}
				*(p++) = '\0';
			}
			break;
		default:
			__assume(0);
		}
		if ((uintptr_t)p & 1)
			*(p++) = 0;
		if ((uintptr_t)p & 2)
			*(((uint16_t *)p)++) = 0;
		if ((uintptr_t)p & 4)
			*(((uint32_t *)p)++) = 0;
		if ((uintptr_t)p & 8)
			*(((uint64_t *)p)++) = 0;
	}
	*(((uint64_t *)p)++) = 0;
	*(((uint64_t *)p)++) = 0;
	nSizeOfBuffer = nRegion + (p - lpFirst);
	if (!lplpConstStringBuffer)
		nSizeOfConstStringRegion = nSizeOfBuffer;
	return VirtualProtect(lpBuffer, nSizeOfBuffer, PAGE_READONLY, &dwProtect);

FAILED:
	if (!lplpConstStringBuffer && lpConstStringRegion)
		VirtualProtect(lpConstStringRegion, nSizeOfConstStringRegion, PAGE_READONLY, &dwProtect);
	return FALSE;
}
//---------------------------------------------------------------------
enum OPT
{
	UNICODE_FUNCTION       = 0x0000001,// must be 1
	INSENSITIVE_CASE       = 0x0000002,
	REVERSE                = 0x0000004,
	NULL_TERMINATED        = 0x0800000,
	NUMBER_OF_CHARS        = 0x1000000,
	HANDLE_OVERLAP_REGIONS = 0x2000000,
	CONCATENATE            = 0x4000000,
	EXTENDED_POINTER       = 0x8000000,
};
//---------------------------------------------------------------------
static_assert(UNICODE_FUNCTION == 1, "Cannot adjust unicode string.");
#define SIZE_OF_CHAR(flags) (((flags) & UNICODE_FUNCTION) + 1)
#define SIZE_OF_STRING(length, flags) ((length) << ((flags) & UNICODE_FUNCTION))
#define TERMINATE_STRING(string, length, flags) \
    if (!((flags) & UNICODE_FUNCTION))          \
        ((LPSTR)(string))[length] = '\0';       \
    else                                        \
        ((LPWSTR)(string))[length] = L'\0'
//---------------------------------------------------------------------
static NTSTATUS __stdcall CompareProcessMemory(
	OUT         int          *lpiResult,
	IN OPTIONAL HANDLE       hProcess1,
	IN          LPCVOID      lpAddress1,
	IN OPTIONAL HANDLE       hProcess2,
	IN          LPCVOID      lpAddress2,
	IN          size_t       nCount,
	IN          unsigned int uFlags)
{
	return
		(!(uFlags & UNICODE_FUNCTION) ?
			((size_t(__stdcall *)(int *, HANDLE, LPCVOID, HANDLE, LPCVOID, size_t, unsigned int))CompareProcessMemoryA) :
			((size_t(__stdcall *)(int *, HANDLE, LPCVOID, HANDLE, LPCVOID, size_t, unsigned int))CompareProcessMemoryW))
		(lpiResult, hProcess1, lpAddress1, hProcess2, lpAddress2, nCount, uFlags & INSENSITIVE_CASE);
}
//---------------------------------------------------------------------
static size_t __stdcall FindProcessMemory(
	IN OPTIONAL HANDLE       hProcess,
	IN          LPCVOID      lpString,
	IN          unsigned int c,
	IN          size_t       nMaxLength,
	IN          unsigned int uFlags)
{
	return
		(!(uFlags & REVERSE) ?
			!(uFlags & UNICODE_FUNCTION) ?
				((size_t(__stdcall *)(HANDLE, LPCVOID, unsigned int, size_t, unsigned int))FindProcessMemoryA) :
				((size_t(__stdcall *)(HANDLE, LPCVOID, unsigned int, size_t, unsigned int))FindProcessMemoryW) :
			!(uFlags & UNICODE_FUNCTION) ?
				((size_t(__stdcall *)(HANDLE, LPCVOID, unsigned int, size_t, unsigned int))FindReverseProcessMemoryA) :
				((size_t(__stdcall *)(HANDLE, LPCVOID, unsigned int, size_t, unsigned int))FindReverseProcessMemoryW))
		(hProcess, lpString, c, nMaxLength, uFlags & INSENSITIVE_CASE);
}
//---------------------------------------------------------------------
static size_t __stdcall StringLength(
	IN OPTIONAL HANDLE       hProcess,
	IN          LPCVOID      lpString,
	IN          size_t       nMaxLength,
	IN          unsigned int uFlags)
{
	return
		(!(uFlags & UNICODE_FUNCTION) ?
			((size_t(__stdcall *)(HANDLE, LPCVOID, unsigned int, size_t, unsigned int))FindProcessMemoryA) :
			((size_t(__stdcall *)(HANDLE, LPCVOID, unsigned int, size_t, unsigned int))FindProcessMemoryW))
		(hProcess, lpString, '\0', nMaxLength, FALSE);
}
//---------------------------------------------------------------------
static void * __fastcall GetEndOfReadableRegion(const void *lpAddress)
{
	#define IsReadableProtect(Protect) ( \
	    ((Protect) & (                   \
	        PAGE_READONLY          |     \
	        PAGE_READWRITE         |     \
	        PAGE_WRITECOPY         |     \
	        PAGE_EXECUTE_READ      |     \
	        PAGE_EXECUTE_READWRITE |     \
	        PAGE_EXECUTE_WRITECOPY)) &&  \
	    !((Protect) & (                  \
	        PAGE_NOACCESS          |     \
	        PAGE_GUARD)))

	const void               *lpEndOfRegion;
	MEMORY_BASIC_INFORMATION mbi;

	lpEndOfRegion = NULL;
	while (VirtualQuery(lpAddress, &mbi, sizeof(mbi)) && IsReadableProtect(mbi.Protect))
		lpEndOfRegion = lpAddress = (char *)mbi.BaseAddress + mbi.RegionSize;
	return (void *)lpEndOfRegion;

	#undef IsReadableProtect
}
//---------------------------------------------------------------------
static void * __fastcall GetEndOfWritableRegion(const void *lpAddress)
{
	#define IsWritableProtect(Protect) ( \
	    ((Protect) & (                   \
	        PAGE_READWRITE         |     \
	        PAGE_WRITECOPY         |     \
	        PAGE_EXECUTE_READWRITE |     \
	        PAGE_EXECUTE_WRITECOPY)) &&  \
	    !((Protect) & (                  \
	        PAGE_NOACCESS          |     \
	        PAGE_GUARD)))

	const void               *lpEndOfRegion;
	MEMORY_BASIC_INFORMATION mbi;

	lpEndOfRegion = NULL;
	while (VirtualQuery(lpAddress, &mbi, sizeof(mbi)) && IsWritableProtect(mbi.Protect))
		lpEndOfRegion = lpAddress = (char *)mbi.BaseAddress + mbi.RegionSize;
	return (void *)lpEndOfRegion;

	#undef IsWritableProtect
}
//---------------------------------------------------------------------
static unsigned char * __fastcall RemoveComments(unsigned char *first, unsigned char *last)
{
	unsigned char *p1, *p2, c1, c2;

	p1 = first;
	for (c1 = *(p1++); p1 < last; c1 = *(p1++))
	{
		switch (c1)
		{
		case '"':
		case '\'':
			// string literals, character literals
			do
			{
				switch (c2 = *(p1++))
				{
				default:
					continue;
				case '"':
				case '\'':
					if (c2 != c1)
						continue;
					break;
				case '\\':
					if (p1 >= last)
						break;
					if (!__intrinsic_isleadbyte(c2))
						continue;
					/* FALLTHROUGH */
				case_unsigned_leadbyte:
					if (p1 >= last)
						break;
					p1++;
					continue;
				}
				break;
			} while (p1 < last);
			break;
		case '/':
			switch (*p1)
			{
			case '*':
				// block comment
				p2 = p1;
				p1--;
				p2++;
				for (; ; )
				{
					c1 = *(p2++);
					if (p2 >= last)
						goto TERMINATE;
					switch (c1)
					{
					default:
						continue;
					case '*':
						if (*p2 != '/')
							continue;
						p2++;
						memcpy(p1, p2, (last -= p2 - p1) - p1 + 1);
						break;
					case_unsigned_leadbyte:
						if (++p2 < last)
							continue;
						goto TERMINATE;
					}
					break;
				}
				break;
			case '/':
				// end of line comment
				p2 = p1;
				p1--;
				p2++;
				for (; ; )
				{
					c1 = *(p2++);
					if (p2 >= last)
						goto TERMINATE;
					switch (c1)
					{
					default:
						continue;
					case '\r':
						if (*p2 == '\n')
							p2++;
						/* FALLTHROUGH */
					case '\n':
						memcpy(p1, p2, (last -= p2 - p1) - p1 + 1);
						break;
					case_unsigned_leadbyte:
						if (++p2 < last)
							continue;
						goto TERMINATE;
					}
					break;
				}
				break;
			}
			break;
#if CODEPAGE_SUPPORT
		default:
			if (!__intrinsic_isleadbyte(c1))
				break;
#else
		case_unsigned_leadbyte:
#endif
			// lead byte
			p1++;
			break;
		}
	}
	return (unsigned char *)last;

TERMINATE:
	*(last = p1) = '\0';
	return (unsigned char *)last;
}
//---------------------------------------------------------------------
//「中置記法の文字列を、後置記法（逆ポーランド記法）に変換し
//	因子単位で格納したベクタを返す関数」
//---------------------------------------------------------------------
static MARKUP ** __stdcall Postfix(IN MARKUP *lpMarkupArray, IN size_t nNumberOfMarkup, OUT size_t *lpnNumberOfPostfix)
{
	MARKUP **lpPostfixBuffer, **lpPostfixFirst, **lpPostfixTop, **lpEndOfPostfix;
	MARKUP **lpFactorBuffer, **lpFactorTop, **lpEndOfFactor;
	signed *lpnNestBuffer, *lpnNestTop, *lpnEndOfNest;
	MARKUP *lpMarkup, *lpEndOfMarkup;
	size_t nSizeOfPostfix;
	LPVOID lpMem;

	lpPostfixBuffer = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpPostfixBuffer)
		goto FAILED1;
	lpFactorBuffer = (MARKUP **)HeapAlloc(hHeap, 0, sizeof(MARKUP *) * nNumberOfMarkup);
	if (!lpFactorBuffer)
		goto FAILED2;
	lpnNestBuffer = (size_t *)HeapAlloc(hHeap, 0, sizeof(size_t) * (nNumberOfMarkup + 1));
	if (!lpnNestBuffer)
		goto FAILED3;
	lpPostfixTop = lpEndOfPostfix = lpPostfixFirst = lpPostfixBuffer;
	lpFactorTop = lpEndOfFactor = lpFactorBuffer;
	lpnNestTop = lpnEndOfNest = lpnNestBuffer;

	#define POSTFIX_IS_EMPTY()  (lpEndOfPostfix == lpPostfixFirst)
	#define POSTFIX_PUSH(value) (*(lpPostfixTop = lpEndOfPostfix++) = (value))
	#define POSTFIX_POP()       (!POSTFIX_IS_EMPTY() ? *(lpEndOfPostfix = lpPostfixTop != lpPostfixFirst ? lpPostfixTop-- : lpPostfixTop) : NULL)
	#define FACTOR_IS_EMPTY()   (lpEndOfFactor == lpFactorBuffer)
	#define FACTOR_PUSH(value)  (*(lpFactorTop = lpEndOfFactor++) = (value))
	#define FACTOR_POP()        (!FACTOR_IS_EMPTY() ? *(lpEndOfFactor = lpFactorTop != lpFactorBuffer ? lpFactorTop-- : lpFactorTop) : NULL)
	#define NEST_IS_EMPTY()     (lpnEndOfNest == lpnNestBuffer)
	#define NEST_PUSH(value)    (*(lpnNestTop = lpnEndOfNest++) = (value))
	#define NEST_POP()          (!NEST_IS_EMPTY() ? *(lpnEndOfNest = lpnNestTop != lpnNestBuffer ? lpnNestTop-- : lpnNestTop) : 0)
	#define SHUNTING 1

	NEST_PUSH(0);
	for (lpMarkup = lpMarkupArray, lpEndOfMarkup = lpMarkupArray + nNumberOfMarkup; lpMarkup < lpEndOfMarkup; lpMarkup++)
	{
		if (lpMarkup->Type & (OS_CLOSE | OS_SPLIT | OS_DELIMITER | OS_LEFT_ASSIGN | OS_TERNARIES))
		{
			if (lpMarkup->Type & (OS_CLOSE | OS_TERNARIES))
			{
				for (; *lpnNestTop; (*lpnNestTop)--)
					POSTFIX_PUSH(FACTOR_POP());
				NEST_POP();
				while (*lpnNestTop && (lpEndOfFactor[-*lpnNestTop]->Type & OS_LEFT_ASSIGN))
				{
					for (; *lpnNestTop; (*lpnNestTop)--)
						POSTFIX_PUSH(FACTOR_POP());
					NEST_POP();
				}
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
				if (lpMarkup->Type & OS_TERNARIES)
					NEST_PUSH(0);
			}
			else if (lpMarkup->Type & OS_LEFT_ASSIGN)
			{
				lpFactorTop = lpEndOfFactor++;
				if (*lpnNestTop)
				{
					MARKUP **lpFactor = lpFactorTop - *lpnNestTop;
#if !SHUNTING
					while (!lpFactor[0]->Priority && lpFactor < lpFactorTop) lpFactor++;
#endif
					memmove(lpFactor + 1, lpFactor, (size_t)lpFactorTop - (size_t)lpFactor);
					*lpFactor = lpMarkup;
				}
				else
				{
					*lpFactorTop = lpMarkup;
				}
				(*lpnNestTop)++;

				if (lpMarkup != lpMarkupArray && (lpMarkup[-1].Type & OS_CLOSE))
				{
					size_t  nDepth = 0;
					MARKUP *lpPrev = lpMarkup;

					do if (!((--lpPrev)->Type & (OS_OPEN | OS_CLOSE)))
						continue;
					else if (lpPrev->Type & OS_CLOSE)
						nDepth++;
					else if (!--nDepth)
						break;
					while (lpPrev != lpMarkupArray);

					while (!POSTFIX_IS_EMPTY() && (*lpPostfixTop)->String >= lpPrev->String)
						FACTOR_PUSH(POSTFIX_POP()), (*lpnNestTop)++;
				}
				NEST_PUSH(0);
			}
			else if (lpMarkup->Type & OS_SPLIT)
			{
#if SHUNTING
				for (; *lpnNestTop; (*lpnNestTop)--)
					POSTFIX_PUSH(FACTOR_POP());
				while (lpnNestTop > lpnNestBuffer && lpnNestTop[-1] && (
					lpEndOfFactor[-lpnNestTop[-1]]->Type & OS_LEFT_ASSIGN ||
					lpEndOfFactor[-lpnNestTop[-1]]->Priority == 0))
				{
					NEST_POP();
					for (; *lpnNestTop; (*lpnNestTop)--)
						POSTFIX_PUSH(FACTOR_POP());
				}
#else
				while (!FACTOR_IS_EMPTY() && (*lpFactorTop)->Depth >= lpMarkup->Depth)
				{
					POSTFIX_PUSH(FACTOR_POP());
					while (!*lpnNestTop)
						lpnEndOfNest = lpnNestTop--;
					(*lpnNestTop)--;
				}
#endif
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
				lpPostfixTop = lpPostfixFirst = lpEndOfPostfix;
			}
			else	// OS_DELIMITER
			{
#if SHUNTING
				for (; *lpnNestTop; (*lpnNestTop)--)
					POSTFIX_PUSH(FACTOR_POP());
				while (lpnNestTop > lpnNestBuffer && lpnNestTop[-1] && (lpEndOfFactor[-lpnNestTop[-1]]->Type & OS_LEFT_ASSIGN))
				{
					NEST_POP();
					for (; *lpnNestTop; (*lpnNestTop)--)
						POSTFIX_PUSH(FACTOR_POP());
				}
#else
				if (!FACTOR_IS_EMPTY())
				{
					size_t  nDepth = 0;
					MARKUP *lpPrev = lpMarkup;

					while (--lpPrev >= lpMarkupArray)
					{
						if (!(lpPrev->Type & (OS_OPEN | OS_CLOSE | OS_SPLIT | OS_DELIMITER | OS_TERNARIES)) &&
							lpPrev->Priority >= lpMarkup->Priority)
							continue;
						if (lpPrev->Type & OS_CLOSE)
							nDepth++;
						else if (!nDepth)
						{
							lpPrev++;
							break;
						}
						else if (lpPrev->Type & OS_OPEN)
							nDepth--;
					}
					while (!FACTOR_IS_EMPTY() && (*lpFactorTop)->String >= lpPrev->String)
					{
						POSTFIX_PUSH(FACTOR_POP());
						while (!*lpnNestTop)
							lpnEndOfNest = lpnNestTop--;
						(*lpnNestTop)--;
					}
				}
#endif
				if (lpMarkup->Type & OS_PUSH)
					POSTFIX_PUSH(lpMarkup);
			}
			continue;
		}
		while (*lpnNestTop && (*lpFactorTop)->Priority >= lpMarkup->Priority)
		{
			POSTFIX_PUSH(FACTOR_POP());
			(*lpnNestTop)--;
		}
		if (lpMarkup->Type & OS_OPEN)
		{
			// 開始括弧ならスタックプッシュ
			NEST_PUSH(0);
			if (lpMarkup->Type & OS_PUSH)
				POSTFIX_PUSH(lpMarkup);
		}
		else if (lpMarkup->Type & OS_PUSH)
		{
			// 除去不可フラグが立っているならスタックへ
			FACTOR_PUSH(lpMarkup);
			(*lpnNestTop)++;
#if SHUNTING
			if (lpMarkup->Priority == 0)
				NEST_PUSH(0);
#endif
			if (lpMarkup->Type & OS_ANTE)
			{
				for (MARKUP *lpNext;
					 (lpNext = lpMarkup + 1) < lpEndOfMarkup && (lpNext->Type & OS_ANTE);
					 (*lpnNestTop)++)
				{
					FACTOR_PUSH(lpMarkup = lpNext);
				}
			}
		}
	}
	while (!FACTOR_IS_EMPTY())
		POSTFIX_PUSH(FACTOR_POP());
	HeapFree(hHeap, 0, lpnNestBuffer);
	HeapFree(hHeap, 0, lpFactorBuffer);
	if (!(nSizeOfPostfix = (size_t)lpEndOfPostfix - (size_t)lpPostfixBuffer))
		goto FAILED2;
	lpMem = HeapReAlloc(hHeap, 0, lpPostfixBuffer, nSizeOfPostfix);
	if (!lpMem)
		goto FAILED2;
	*lpnNumberOfPostfix = nSizeOfPostfix / sizeof(MARKUP *);
	return (MARKUP **)lpMem;

FAILED3:
	HeapFree(hHeap, 0, lpFactorBuffer);
FAILED2:
	HeapFree(hHeap, 0, lpPostfixBuffer);
FAILED1:
	return NULL;

	#undef SHUNTING
	#undef POSTFIX_PUSH
	#undef FACTOR_IS_EMPTY
	#undef FACTOR_PUSH
	#undef FACTOR_POP
	#undef NEST_IS_EMPTY
	#undef NEST_PUSH
	#undef NEST_POP
}
//---------------------------------------------------------------------
static LPVOID __fastcall AllocateHeapBuffer(LPVOID **lplpHeapBuffer, size_t *lpnNumberOfHeapBuffer, size_t cbSize)
{
	LPVOID *lpHeapBuffer;
	size_t nNumberOfHeapBuffer;
	LPVOID lpBuffer;

	lpHeapBuffer = *lplpHeapBuffer;
	nNumberOfHeapBuffer = *lpnNumberOfHeapBuffer;
	if (lpHeapBuffer)
	{
		if (!(nNumberOfHeapBuffer & 0x0F))
		{
			lpHeapBuffer = (LPVOID *)HeapReAlloc(hHeap, 0, lpHeapBuffer, (nNumberOfHeapBuffer + 0x10) * sizeof(LPVOID));
			if (!lpHeapBuffer)
				return NULL;
			*lplpHeapBuffer = lpHeapBuffer;
		}
	}
	else
	{
		lpHeapBuffer = (LPVOID *)HeapAlloc(hHeap, 0, 0x10 * sizeof(LPVOID));
		if (!lpHeapBuffer)
			return NULL;
		*lplpHeapBuffer = lpHeapBuffer;
	}
	lpBuffer = HeapAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, cbSize);
	if (!lpBuffer)
		return NULL;
	lpHeapBuffer[nNumberOfHeapBuffer] = lpBuffer;
	*lpnNumberOfHeapBuffer = nNumberOfHeapBuffer + 1;
	return lpBuffer;
}
//---------------------------------------------------------------------
//「文字列Srcを、一旦逆ポーランド記法にしたあと解析する関数」
//---------------------------------------------------------------------
uint64_t __cdecl InternalParsing(TSSGCtrl *const this, TSSGSubject *const SSGS, const string *const Src, BOOL const bInitialIsInteger, va_list register ArgPtr)
{
	#define PROCESS_DESIRED_ACCESS (PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION)

	uint64_t                    qwResult;
	BOOL                        bCached;
	LPBYTE                      lpszReplace;
	LPSTR                       lpszSrc;
	MARKUP                      *lpMarkupArray;
	MARKUP                      **lpPostfixBuffer, **lpPostfix, **lpEndOfPostfix;
	LPBYTE                      lpConstStringBuffer;
	VARIAUNT                    *lpOperandBuffer, *lpOperandTop, *lpEndOfOperand;
	VARIABLE                    *lpVariable;
	size_t                      nNumberOfVariable;
	LPVOID                      *lpHeapBuffer;
	size_t                      nNumberOfHeapBuffer;
#if REPEAT_INDEX
	LPSTR                       lpVariableStringBuffer;
#endif
#if ADDITIONAL_TAGS || SCOPE_SUPPORT
	vector_TSSGAttributeElement *attributes;
#endif
	BOOL                        IsInteger;
	VARIAUNT                    operandZero;
	HANDLE                      hProcess;
	HANDLE                      strtok_process;
	HANDLE                      wcstok_process;
	HANDLE                      mbstok_process;
	char                        *strtok_context;
	wchar_t                     *wcstok_context;
	unsigned char               *mbstok_context;
	BOOLEAN                     bCompoundAssign;

	qwResult               = 0;
	bCached                = FALSE;
	lpszReplace            = NULL;
	lpszSrc                = NULL;
	lpMarkupArray          = NULL;
	lpPostfixBuffer        = NULL;
	lpEndOfPostfix         = NULL;
	lpConstStringBuffer    = NULL;
	lpOperandBuffer        = NULL;
	lpVariable             = NULL;
	nNumberOfVariable      = 0;
	lpHeapBuffer           = NULL;
	nNumberOfHeapBuffer    = 0;
#if REPEAT_INDEX
	lpVariableStringBuffer = NULL;
#endif
	hProcess               = NULL;

	{
		char   *p, *end;
		size_t nNumberOfMarkup;
		size_t nNumberOfPostfix;
		size_t length;

		do	/* do { ... } while (0); */
		{
			#define BOM '\xEF\xBB\xBF\x00'

			BOOL               bCaching;
			size_t             nSizeOfReplace;
			char               c;
			size_t             cacheNext;
			LPVOID             lpMem;
			size_t             nSrcLength;
#if ADDITIONAL_TAGS
			TPrologueAttribute *variable;
			const string       *code;
			size_t             capacity;
#endif

			bCaching = FALSE;
#if ADDITIONAL_TAGS
			if (!(attributes = TSSGSubject_GetAttribute(SSGS)))// check for TSSGCtrl::LoopSSRFile
				attributes = TSSGAttributeSelector_GetNowAtteributeVec(&this->attributeSelector);
			variable = TSSGCtrl_GetAttribute(this, SSGS, atPROLOGUE);
#endif
			p = string_begin(Src);
			if (string_length(Src) >= sizeof(size_t) * 2 && *(size_t *)string_begin(Src) == BOM)
			{
				size_t offset;

				nSizeOfReplace = 0;
#if ADDITIONAL_TAGS
				if (variable && string_length(code = TEndWithAttribute_GetCode(variable)))
					nSizeOfReplace = string_length(code);
				if (attributes)
				{
					const COORD coord = TSSGAttributeElement_GetViaCoord(atDEFINE, attributes).dwFontSize;
					for (TDefineAttribute **it = &vector_type_at(attributes, TDefineAttribute *, coord.Y), **end = it + coord.X;
						 it < end;
						 it++)
						nSizeOfReplace +=
							string_length(TIO_FEPAttribute_GetInputCode (*it)) + 1 +
							string_length(TIO_FEPAttribute_GetOutputCode(*it)) + 1;
				}
				if (nSizeOfReplace)
				{
					LPBYTE p, dest;
					size_t n;

					if (!(lpszReplace = (LPBYTE)HeapAlloc(hHeap, 0, ++nSizeOfReplace)))
						goto ALLOC_ERROR;
					p = lpszReplace;
					if (variable && string_length(code = TEndWithAttribute_GetCode(variable)))
					{
						p = (dest = p) + (n = string_length(code));
						memcpy(dest, string_c_str(code), n);
					}
					*(p++) = '\0';
					if (attributes)
					{
						const COORD coord = TSSGAttributeElement_GetViaCoord(atDEFINE, attributes).dwFontSize;
						for (TDefineAttribute **it = &vector_type_at(attributes, TDefineAttribute *, coord.Y), **end = it + coord.X;
							 it < end;
							 it++)
						{
							code = TIO_FEPAttribute_GetInputCode (*it);
							p = (dest = p) + (n = string_length(code) + 1);
							memcpy(dest, string_c_str(code), n);
							code = TIO_FEPAttribute_GetOutputCode(*it);
							p = (dest = p) + (n = string_length(code) + 1);
							memcpy(dest, string_c_str(code), n);
						}
					}
				}
#endif
				cacheNext = 0;
				if ((offset = ((size_t *)string_begin(Src))[1]) != -1)
				{
					CODECACHE *cache, *last;

					cache = (CODECACHE *)((LPBYTE)lpCodeCache + offset);
					while (!(bCached = cache->SizeOfReplace == nSizeOfReplace && memcmp(cache->Replace, lpszReplace, nSizeOfReplace) == 0))
						if ((offset = (last = cache)->Next) == -1)
							break;
						else
							cache = (CODECACHE *)((LPBYTE)lpCodeCache + offset);
					if (bCached)
					{
						lpszSrc = cache->Source;
						nNumberOfMarkup = cache->NumberOfMarkup;
						lpMarkupArray = cache->Markup;
						nNumberOfPostfix = cache->NumberOfPostfix;
						lpPostfixBuffer = cache->Postfix;
						if (!lpszReplace)
							break;
						HeapFree(hHeap, 0, lpszReplace);
						lpszReplace = NULL;
						break;
					}
					cacheNext = (LPBYTE)&last->Next - (LPBYTE)lpCodeCache;
				}
				bCaching = TRUE;
				p = string_begin(Src) + sizeof(size_t) * 2;
			}

			do
				c = *(p++);
			while (__intrinsic_isspace(c));

			if (!c)
				goto FAILED;

#if LOCAL_MEMORY_SUPPORT
			if (c == 'L')
			{
				c = *p;
				if (__intrinsic_isascii(c))
					if (__intrinsic_isspace(c))
					{
						p++;
						do
							c = *(p++);
						while (__intrinsic_isspace(c));
					}
					else if (!__intrinsic_isalnum(c) && c != '_')
						p++;
			}
#endif

			p--;
			end = string_end(Src);
			while (p < end)
			{
				c = *(--end);
				if (__intrinsic_isspace(c))
					continue;
				end++;
				break;
			}
			nSrcLength = end - p;

#if !ADDITIONAL_TAGS
			if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, nSrcLength + sizeof(uint32_t))))
				goto ALLOC_ERROR;
			memcpy(lpszSrc, p, nSrcLength);
			lpszSrc[nSrcLength] = '\0';
#else
			if (variable && (length = string_length(code = TEndWithAttribute_GetCode(variable))))
			{
				unsigned long bits;

#ifndef _WIN64
				_BitScanReverse(&bits, length + nSrcLength + sizeof(uint32_t) - 1);
#else
				_BitScanReverse64(&bits, length + nSrcLength + sizeof(uint32_t) - 1);
#endif
				capacity = (size_t)1 << (bits + 1);
				if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, capacity)))
					goto ALLOC_ERROR;
				memcpy(lpszSrc, string_c_str(code), length);
				memcpy(lpszSrc + length, p, nSrcLength);
				nSrcLength += length;
			}
			else
			{
				unsigned long bits;

#ifndef _WIN64
				_BitScanReverse(&bits, nSrcLength + sizeof(uint32_t) - 1);
#else
				_BitScanReverse64(&bits, nSrcLength + sizeof(uint32_t) - 1);
#endif
				capacity = (size_t)1 << (bits + 1);
				if (!(lpszSrc = (LPSTR)HeapAlloc(hHeap, 0, capacity)))
					goto ALLOC_ERROR;
				memcpy(lpszSrc, p, nSrcLength);
			}
			lpszSrc[nSrcLength] = '\0';

			// remove the c style comments
			nSrcLength = RemoveComments(lpszSrc, lpszSrc + nSrcLength) - lpszSrc;

			if (attributes)
			{
				nSrcLength = ReplaceDefineByHeap(attributes, &lpszSrc, nSrcLength, capacity);
				if (nSrcLength == -1)
					goto ALLOC_ERROR;
			}

#if LOCAL_MEMORY_SUPPORT
			p = lpszSrc;
			do
				c = *(p++);
			while (__intrinsic_isspace(c));
			if (c == 'L' && *p && __intrinsic_isascii(*p) && !__intrinsic_iscsym(*p) && *p != '=')
			{
				p++;
				do
					c = *(p++);
				while (__intrinsic_isspace(c));
				nSrcLength -= --p - lpszSrc;
				memcpy(lpszSrc, p, nSrcLength + 1);
			}
#endif
#endif

			// 4 bytes terminator
			if (!(lpMem = HeapReAlloc(hHeap, 0, lpszSrc, nSrcLength + sizeof(uint32_t))))
				goto ALLOC_ERROR;
			*(uint32_t *)&(lpszSrc = (LPSTR)lpMem)[nSrcLength] = '\0';

			lpMarkupArray = Markup(lpszSrc, nSrcLength, &nNumberOfMarkup);
			if (!lpMarkupArray)
				goto ALLOC_ERROR;

			lpPostfixBuffer = Postfix(lpMarkupArray, nNumberOfMarkup, &nNumberOfPostfix);
			if (!lpPostfixBuffer)
				goto ALLOC_ERROR;

			if (!UnescapeConstStrings(lpMarkupArray, lpMarkupArray + nNumberOfMarkup, !bCaching ? &lpConstStringBuffer : NULL))
				goto ALLOC_ERROR;

			if (bCaching)
			{
				size_t    *next;
				CODECACHE *cache;

				if (!(nNumberOfCodeCache & 0x0F))
				{
					if (nNumberOfCodeCache)
					{
						LPVOID lpMem;

						lpMem = HeapReAlloc(hHeap, 0, lpCodeCache, sizeof(CODECACHE) * (nNumberOfCodeCache + 0x10));
						if (!lpMem)
							goto ALLOC_ERROR;
						lpCodeCache = lpMem;
					}
					else
					{
						lpCodeCache = HeapAlloc(hHeap, 0, sizeof(CODECACHE) * 0x10);
						if (!lpCodeCache)
							goto ALLOC_ERROR;
					}
				}
				next = cacheNext ? (size_t *)((LPBYTE)lpCodeCache + cacheNext) : (size_t *)string_begin(Src) + 1;
				cache = (CODECACHE *)((LPBYTE)lpCodeCache + (*next = nNumberOfCodeCache++ * sizeof(CODECACHE)));
				cache->SizeOfReplace = nSizeOfReplace;
				cache->Replace = lpszReplace;
				cache->Source = lpszSrc;
				cache->NumberOfMarkup = nNumberOfMarkup;
				cache->Markup = lpMarkupArray;
				cache->NumberOfPostfix = nNumberOfPostfix;
				cache->Postfix = lpPostfixBuffer;
				cache->Next = -1;
				bCached = TRUE;
			}

			#undef BOM
		} while (0);

		lpOperandBuffer = (VARIAUNT *)HeapAlloc(hHeap, 0, sizeof(VARIAUNT) * (2 + nNumberOfPostfix));
		if (!lpOperandBuffer)
			goto ALLOC_ERROR;

		lpVariable = (VARIABLE *)HeapAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, sizeof(VARIABLE) * 0x10);
		if (!lpVariable)
			goto ALLOC_ERROR;

		#define OPERAND_IS_EMPTY()  (lpEndOfOperand == lpOperandBuffer)
		#define OPERAND_PUSH(value) (*(lpOperandTop = lpEndOfOperand++) = (value))
		#define OPERAND_POP()       (!OPERAND_IS_EMPTY() ? *(lpEndOfOperand = lpOperandTop != lpOperandBuffer ? lpOperandTop-- : lpOperandTop) : operandZero)
		#define OPERAND_CLEAR()     (*(lpOperandTop = lpEndOfOperand = lpOperandBuffer) = operandZero, lpEndOfOperand++)

		IsInteger = bInitialIsInteger;
		if (nNumberOfPostfix && !lpPostfixBuffer[0]->Close) switch (lpPostfixBuffer[0]->Tag)
		{
		case TAG_PARSE_INT:
			IsInteger = TRUE;
			break;
		case TAG_PARSE_REAL:
			IsInteger = FALSE;
			break;
		}
		operandZero.Quad = 0;
		operandZero.IsQuad = !IsInteger;
		OPERAND_CLEAR();
#if SUBJECT_STATUS
		lpVariable[0].Length = 4;
		lpVariable[0].String = "Addr";
		lpVariable[1].Length = 4;
		lpVariable[1].String = "Read";
		lpVariable[2].Length = 4;
		lpVariable[2].String = "Size";
		lpVariable[3].Length = 4;
		lpVariable[3].String = "Type";
		if (IsInteger)
		{
			lpVariable[0].Value.Quad = (uint64_t)SSGS->lastAddr;
			lpVariable[0].Value.IsQuad = sizeof(SSGS->lastAddr) > sizeof(uint32_t);
			lpVariable[1].Value.Quad = (uint64_t)SSGS->evaluateAtRead;
			lpVariable[1].Value.IsQuad = sizeof(SSGS->evaluateAtRead) > sizeof(uint32_t);
			lpVariable[2].Value.Quad = (uint64_t)TSSGSubject_GetSize(SSGS);
			lpVariable[2].Value.IsQuad = sizeof(unsigned long) > sizeof(uint32_t);
			lpVariable[3].Value.Quad = (uint64_t)SSGS->type;
			lpVariable[3].Value.IsQuad = sizeof(SSGS->type) > sizeof(uint32_t);
		}
		else
		{
			lpVariable[0].Value.Real = (double)(size_t)SSGS->lastAddr;
			lpVariable[0].Value.IsQuad = TRUE;
			lpVariable[1].Value.Real = (double)SSGS->evaluateAtRead;
			lpVariable[1].Value.IsQuad = TRUE;
			lpVariable[2].Value.Real = (double)TSSGSubject_GetSize(SSGS);
			lpVariable[2].Value.IsQuad = TRUE;
			lpVariable[3].Value.Real = (double)SSGS->type;
			lpVariable[3].Value.IsQuad = TRUE;
		}
		nNumberOfVariable = 4;
#endif
		while (length = va_arg(ArgPtr, size_t))
		{
#if !SUBJECT_STATUS
			if (nNumberOfVariable)
#endif
			if (!(nNumberOfVariable & 0x0F))
			{
				LPVOID lpMem;
				size_t nBytes;

				nBytes = (nNumberOfVariable + 0x10) * sizeof(VARIABLE);
				lpMem = HeapReAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, lpVariable, nBytes);
				if (!lpMem)
					goto ALLOC_ERROR;
				lpVariable = (VARIABLE *)lpMem;
			}
			lpVariable[nNumberOfVariable].Length = length;
			lpVariable[nNumberOfVariable].String = va_arg(ArgPtr, LPCSTR);
			lpVariable[nNumberOfVariable].Value.Quad = va_arg(ArgPtr, uint64_t);
			lpVariable[nNumberOfVariable].Value.IsQuad = !!lpVariable[nNumberOfVariable].Value.High || !IsInteger;
			nNumberOfVariable++;
		}
#if REPEAT_INDEX
		do	/* do { ... } while (0); */
		{
			TSSGSubjectProperty *lpProperty;
			size_t              nPrevNumberOfVariable;
			size_t              nSize, nCapacity, nForward;

			lpProperty = GetSubjectProperty(SSGS);
			if (!lpProperty)
				break;
			if (!lpProperty->RepeatDepth)
				break;
			lpVariableStringBuffer = (LPSTR)HeapAlloc(hHeap, 0, 32);
			if (!lpVariableStringBuffer)
				goto ALLOC_ERROR;
			nPrevNumberOfVariable = nNumberOfVariable;
			p = lpVariableStringBuffer;
			nSize = 0;
			nCapacity = 32;
			nForward = -1;
			do
			{
				size_t nVariableLength;

				if (nCapacity < nSize + (3 + 10 + 1) + (6 + 10 + 1))
				{
					LPVOID lpMem;

					lpMem = HeapReAlloc(hHeap, 0, lpVariableStringBuffer, nCapacity <<= 1);
					if (!lpMem)
						goto ALLOC_ERROR;
					p += (size_t)lpMem - (size_t)lpVariableStringBuffer;
					lpVariableStringBuffer = (LPSTR)lpMem;
				}
				*(uint32_t *)p = BSWAP32('Idx\0');
				_ultoa(lpProperty->RepeatDepth, p + 3, 10);
#if !SUBJECT_STATUS
				if (nNumberOfVariable)
#endif
				if (!(nNumberOfVariable & 0x0F))
				{
					LPVOID lpMem;
					size_t nBytes;

					nBytes = (nNumberOfVariable + 0x10) * sizeof(VARIABLE);
					lpMem = HeapReAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, lpVariable, nBytes);
					if (!lpMem)
						goto ALLOC_ERROR;
					lpVariable = (VARIABLE *)lpMem;
				}
				lpVariable[nNumberOfVariable].Length = nVariableLength = strlen(p + 3) + 3;
				nSize += ++nVariableLength;
				lpVariable[nNumberOfVariable].String = (LPSTR)(p - lpVariableStringBuffer);
				p += nVariableLength;
				if (!(lpVariable[nNumberOfVariable].Value.IsQuad = !IsInteger))
					lpVariable[nNumberOfVariable].Value.Quad = lpProperty->RepeatIndex;
				else
					lpVariable[nNumberOfVariable].Value.Real = (double)lpProperty->RepeatIndex;
				nNumberOfVariable++;
				if (++nForward)
				{
					*(uint32_t *) p      = BSWAP32('FwdI');
					*(uint16_t *)(p + 4) = BSWAP16('dx'  );
					_ultoa(nForward, p + 6, 10);
					nVariableLength = strlen(p + 6) + 6;
				}
				else
				{
					*(uint32_t *)p = BSWAP32('Idx\0');
					nVariableLength = 3;
				}
#if !SUBJECT_STATUS
				if (nNumberOfVariable)
#endif
				if (!(nNumberOfVariable & 0x0F))
				{
					LPVOID lpMem;
					size_t nBytes;

					nBytes = (nNumberOfVariable + 0x10) * sizeof(VARIABLE);
					lpMem = HeapReAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, lpVariable, nBytes);
					if (!lpMem)
						goto ALLOC_ERROR;
					lpVariable = (VARIABLE *)lpMem;
				}
				lpVariable[nNumberOfVariable].Length = nVariableLength;
				nSize += ++nVariableLength;
				lpVariable[nNumberOfVariable].String = (LPSTR)(p - lpVariableStringBuffer);
				p += nVariableLength;
				if (!(lpVariable[nNumberOfVariable].Value.IsQuad = !IsInteger))
					lpVariable[nNumberOfVariable].Value.Quad = lpProperty->RepeatIndex;
				else
					lpVariable[nNumberOfVariable].Value.Real = (double)lpProperty->RepeatIndex;
				nNumberOfVariable++;
			} while ((lpProperty = GetOuterRepeat(lpProperty)) && lpProperty->RepeatDepth);
			for (size_t i = nPrevNumberOfVariable; i < nNumberOfVariable; i++)
				lpVariable[i].String += (size_t)lpVariableStringBuffer;
		} while (0);
#endif
		lpEndOfPostfix = (lpPostfix = lpPostfixBuffer) + nNumberOfPostfix;
	}

	strtok_process  = NULL;
	wcstok_process  = NULL;
	mbstok_process  = NULL;
	strtok_context  = NULL;
	wcstok_context  = NULL;
	mbstok_context  = NULL;
	bCompoundAssign = FALSE;
	for (OPERAND_CLEAR(); lpPostfix < lpEndOfPostfix; lpPostfix++)
	{
		MARKUP       *lpMarkup;
		bool         boolValue;
		size_t       nDepth;
		LPVOID       lpAddress;
		size_t       nSize;
		MARKUP       *lpNext;
		enum OPT     uFlags;
		FARPROC      lpFunction;
		HANDLE       *lpStrtokProcess;
		LPVOID       *lpStrtokContext;
		LPVOID       lpBuffer1;
		LPVOID       lpBuffer2;
		LPVOID       lpBuffer3;
		LPSTR        lpGuideText;
#if !defined(__BORLANDC__)
		size_t       nGuideTextLength;
#endif
		LPCSTR       lpMessage;
		WORD         wCharTypeMask;
		size_t(__cdecl *lpStrlen)(const void *);

		if (((lpMarkup = *lpPostfix)->Type & OS_VERIFY) && TMainForm_GetUserMode(MainForm) < 2)
			continue;
		switch (lpMarkup->Tag)
		{
		case TAG_IF:
			OPERAND_CLEAR();
			if (lpMarkup->Length)
				break;
			continue;
		case TAG_SWITCH:
		case TAG_DO:
		case TAG_WHILE:
		case TAG_FOR:
			OPERAND_CLEAR();
			break;
		case TAG_IF_EXPR:
			boolValue = IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real;
			if (OPERAND_POP(), OPERAND_IS_EMPTY())
			{
				lpMarkup = lpMarkup->Param;
				goto PARSING_ERROR;
			}
			OPERAND_CLEAR();
			if (boolValue)
				continue;
			if (lpMarkup->FalsePart)
			{
				if ((lpMarkup = *(lpPostfix = lpMarkup->FalsePart))->Tag == TAG_ELSE)
					break;
			}
			else
			{
				while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->Depth > lpMarkup->Depth);
				if (lpPostfix < lpEndOfPostfix && (*lpPostfix)->Tag == TAG_ELSE && (*lpPostfix)->Depth == lpMarkup->Depth)
				{
					lpMarkup = *(lpMarkup->FalsePart = lpPostfix);
					break;
				}
				lpMarkup->FalsePart = --lpPostfix;
			}
			continue;
		case TAG_ELSE:
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->Depth > lpMarkup->Depth);
				lpMarkup->Jump = --lpPostfix;
			}
			continue;
		case TAG_CASE:
		case TAG_DEFAULT:
			if (lpMarkup->Type != OS_PUSH)
				goto PARSING_ERROR;
			if (lpPostfix + 1 >= lpEndOfPostfix || lpPostfix[1]->Tag != TAG_GOTO)
				continue;
			OPERAND_CLEAR();
			if (lpMarkup->FalsePart)
				lpPostfix = lpMarkup->FalsePart;
			else
			{
				lpNext = lpMarkup->Next;
				while (lpNext && (lpNext->Tag != lpMarkup->Tag ||
								  lpNext->Param != lpMarkup->Param ||
								  lpNext->NumberOfOperand != lpMarkup->NumberOfOperand))
					lpNext = lpNext->Next;
				if (lpNext)
				{
					if (lpNext->FalsePart)
						lpPostfix = lpMarkup->FalsePart = lpNext->FalsePart;
					else// make allowances for come here by go-to LABEL
					{
						lpPostfix = !lpMarkup->Next->Jump ? lpPostfixBuffer : lpMarkup->Next->Jump;
						while (++lpPostfix < lpEndOfPostfix && *lpPostfix != lpNext);
						lpMarkup->FalsePart = lpNext->FalsePart = lpPostfix;
					}
				}
				else
					goto PARSING_ERROR;
			}
			if (!this->ssgActionListner)
				continue;
			lpGuideText = lpMarkup[-1].String;
#if !defined(__BORLANDC__)
			nGuideTextLength = lpMarkup->String - lpMarkup[-1].String + lpMarkup->Length;
#endif
			goto OUTPUT_GUIDE;
		case TAG_SWITCH_EXPR:
			lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
			if (OPERAND_POP(), OPERAND_IS_EMPTY())
			{
				lpMarkup = lpMarkup->Param;
				goto PARSING_ERROR;
			}
			OPERAND_CLEAR();
			{
				#define lpBuffer lpBuffer1

				int    iResult;
				HANDLE hSrcProcess = NULL;
				LPVOID pSrc = NULL;
				size_t nCount = 0;

				lpBuffer = NULL;
				(lpNext = lpMarkup)->Jump = lpPostfix;
				lpMarkup = lpNext->Param;// for error
				mbstok_context = !bCached ? lpConstStringBuffer : lpConstStringRegion;

				if (uFlags = lpNext->Next->Tag == TAG_CASE && lpNext->Next->Param
					? NULL_TERMINATED | UNICODE_FUNCTION & lpNext->Next->UnescapedString : 0)
				{
					if (lpNext->Param[2].Tag == TAG_PARAM_LOCAL || IsStringOperand(&lpNext->Param[2]))
						hSrcProcess = NULL;
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						hSrcProcess = hProcess;
					else
						goto OPEN_ERROR;

				GET_LENGTH:
					if (!(nCount = StringLength(hSrcProcess, pSrc = lpAddress, -1, uFlags) + 1))
						goto READ_ERROR;

					if (hSrcProcess)
					{
						if (!(lpBuffer = HeapAlloc(hHeap, 0, nSize = SIZE_OF_STRING(nCount, uFlags))))
							goto ALLOC_ERROR;
						if (!ReadProcessMemory(hSrcProcess, lpAddress, pSrc = lpBuffer, nSize, NULL))
							goto READ_ERROR_FREE1;
						hSrcProcess = NULL;
					}
				}
				else if (CheckStringOperand(&lpNext->Param[2], &nSize))
				{
					uFlags = NULL_TERMINATED | UNICODE_FUNCTION & nSize;
					goto GET_LENGTH;
				}

				lpMarkup = lpNext->Next;// get case chain
				while (lpMarkup->Next && (lpMarkup->Tag == TAG_CASE && lpMarkup->Type == OS_PUSH && (
					lpMarkup->Param ? !NT_SUCCESS(CompareProcessMemory(
						&iResult,
						pSrc ? hSrcProcess : hProcess ? hProcess : (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)),
						pSrc ? pSrc : lpAddress,
						NULL,
						mbstok_context + lpMarkup->Param->UnescapedString,
						nCount ? nCount : StringLength(
							NULL,
							mbstok_context + lpMarkup->Param->UnescapedString,
							-1,
							NULL_TERMINATED | UNICODE_FUNCTION & lpMarkup->UnescapedString) + 1,
						NUMBER_OF_CHARS | NULL_TERMINATED | UNICODE_FUNCTION & lpMarkup->UnescapedString)) || iResult
					: lpMarkup->NumberOfOperand != (size_t)lpAddress)))
					lpMarkup = lpMarkup->Next;

				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);

				if (lpMarkup->Type & OS_SHORT_CIRCUIT)
					goto PARSING_ERROR;

				if (lpMarkup->FalsePart)
					lpPostfix = lpMarkup->FalsePart;
				else
				{
					while (++lpPostfix < lpEndOfPostfix && *lpPostfix != lpMarkup);
					lpMarkup->FalsePart = lpPostfix;
				}

				#undef lpBuffer
			}
			if (lpMarkup->Type & OS_SPLIT)
				goto LOOP_CHECK;
			break;
		case TAG_WHILE_EXPR:
			boolValue = IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real;
			if (OPERAND_POP(), OPERAND_IS_EMPTY())
			{
				lpMarkup = lpMarkup->Param;
				goto PARSING_ERROR;
			}
			OPERAND_CLEAR();
			if (boolValue)
			{
				if (lpMarkup->TruePart)
					lpPostfix = lpMarkup->TruePart;
				else
				{
					if (!(lpMarkup->Type & OS_POST))
					{
						if (lpMarkup->Type & OS_LOOP_END)
							goto LOOP_END;
					}
					else
					{
						if (lpPostfix > lpPostfixBuffer)
							while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->LoopDepth > lpMarkup->LoopDepth);
					}
					lpMarkup->TruePart = lpPostfix;
				}
			}
			else
			{
				if (lpMarkup->FalsePart)
				{
					if ((lpMarkup = *(lpPostfix = lpMarkup->FalsePart))->Tag == TAG_ELSE)
						break;
				}
				else
				{
					if (!(lpMarkup->Type & OS_POST))
					{
						nDepth = lpMarkup->Depth;
						if (lpMarkup->Type & OS_LOOP_END)
							goto LOOP_ELSE;
						nDepth--;
						while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth);
					}
					else
					{
						nDepth = lpMarkup->Depth;
					}
					goto LOOP_ELSE;
				}
			}
			continue;
		case TAG_FOR_INITIALIZE:
			OPERAND_CLEAR();
			continue;
		case TAG_FOR_CONDITION:
			boolValue = IsInteger ? !!lpOperandTop->Quad : !!lpOperandTop->Real;
			if (lpMarkup[-1].Tag == TAG_FOR_INITIALIZE)
				boolValue = TRUE;
			else if (OPERAND_POP(), OPERAND_IS_EMPTY())
			{
				lpMarkup = lpMarkup->Param;
				goto PARSING_ERROR;
			}
			OPERAND_CLEAR();
			if (boolValue)
			{
				if (lpMarkup->TruePart)
					lpPostfix = lpMarkup->TruePart;
				else
				{
					while (++lpPostfix < lpEndOfPostfix)
						if ((*lpPostfix)->Tag == TAG_FOR_UPDATE)
							if (!((*lpPostfix)->Type & OS_LOOP_END))
								break;
							else
								goto LOOP_END;
					lpMarkup->TruePart = lpPostfix;
				}
			}
			else
			{
				if (lpMarkup->FalsePart)
				{
					if ((lpMarkup = *(lpPostfix = lpMarkup->FalsePart))->Tag == TAG_ELSE)
						break;
				}
				else
				{
					while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth > lpMarkup->LoopDepth);
					nDepth = lpMarkup->Depth;
				LOOP_ELSE:
					if (++lpPostfix < lpEndOfPostfix &&
						(*lpPostfix)->Tag == TAG_ELSE && (*lpPostfix)->Depth == nDepth) {
						lpMarkup = *(lpMarkup->FalsePart = lpPostfix);
						break;
					}
					lpMarkup->FalsePart = --lpPostfix;
				}
			}
			continue;
		case TAG_FOR_UPDATE:
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				if (lpPostfix > lpPostfixBuffer)
					while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->Tag != TAG_FOR_INITIALIZE);
				lpMarkup->Jump = lpPostfix;
			}
			continue;
		case TAG_PARENTHESIS_CLOSE:
		case TAG_BRACE_CLOSE:
		case TAG_SPLIT:
		LOOP_CHECK:
			if (!(lpMarkup->Type & OS_LOOP_END))
				continue;
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
			LOOP_END:
				if (lpPostfix > lpPostfixBuffer)
					while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->LoopDepth > lpMarkup->LoopDepth);
				lpMarkup->Jump = lpPostfix;
			}
			continue;
		case TAG_BREAK:
#ifdef CLEAR_PER_BREAK
			OPERAND_CLEAR();
#endif
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else if (lpMarkup->Next)
			{
				while (++lpPostfix < lpEndOfPostfix && *lpPostfix != lpMarkup->Next);
				lpMarkup->Jump = --lpPostfix;
			}
			else
			{
				while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth);
				lpMarkup->Jump = lpPostfix;
			}
			break;
		case TAG_CONTINUE:
			OPERAND_CLEAR();
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else
			{
				if (lpMarkup->Type & OS_POST)
				{
					while (++lpPostfix < lpEndOfPostfix && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth)
						if ((*lpPostfix)->LoopDepth == lpMarkup->LoopDepth && (*lpPostfix)->Tag == TAG_WHILE)
							break;
					lpPostfix--;
				}
				else
				{
					if (lpPostfix > lpPostfixBuffer)
						while (--lpPostfix > lpPostfixBuffer && (*lpPostfix)->LoopDepth >= lpMarkup->LoopDepth);
				}
				lpMarkup->Jump = lpPostfix;
			}
			break;
		case TAG_LABEL:
		case TAG_PARAM_LOCAL:
		case TAG_IMPORT_FUNCTION:
		case TAG_IMPORT_REFERENCE:
			continue;
		case TAG_DELIMITER:
			OPERAND_POP();
			/* FALLTHROUGH */
		case TAG_RETURN:
			break;
		case TAG_ADD:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
					if (!(lpOperandTop->IsQuad |= operand.IsQuad))
						lpOperandTop->Low += operand.Low;
					else
						lpOperandTop->Quad += operand.Quad;
				else
					lpOperandTop->Real += operand.Real;
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_SUB:
			{
				VARIAUNT operand;

				if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
				{
					operand = OPERAND_POP();
				}
				else
				{
					VARIAUNT swap = OPERAND_POP();
					operand = *lpOperandTop;
					*lpOperandTop = swap;
				}
				if (IsInteger)
					if (!(lpOperandTop->IsQuad |= operand.IsQuad))
						lpOperandTop->Low -= operand.Low;
					else
						lpOperandTop->Quad -= operand.Quad;
				else
					lpOperandTop->Real -= operand.Real;
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_MUL:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
					if (!(lpOperandTop->IsQuad |= operand.IsQuad))
						lpOperandTop->Low *= operand.Low;
					else
						lpOperandTop->Quad *= operand.Quad;
				else
					lpOperandTop->Real *= operand.Real;
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_IDIV:
			if (IsInteger)
			{
				VARIAUNT operand;
				BOOL     IsQuad;

				operand = OPERAND_POP();
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto DIVIDE_BY_ZERO;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Low = (int32_t)lpOperandTop->Low / (int32_t)operand.Low;
				else
					lpOperandTop->Quad = (lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) / (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
				lpOperandTop->IsQuad = IsQuad;
				break;
			}
			/* FALLTHROUGH */
		case TAG_DIV:
			{
				VARIAUNT operand;

				if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
				{
					operand = OPERAND_POP();
				}
				else
				{
					VARIAUNT swap = OPERAND_POP();
					operand = *lpOperandTop;
					*lpOperandTop = swap;
				}
				if (IsInteger)
				{
					if (!operand.Low && (!operand.IsQuad || !operand.High))
						goto DIVIDE_BY_ZERO;
					if (!(lpOperandTop->IsQuad |= operand.IsQuad))
						lpOperandTop->Low /= operand.Low;
					else
						lpOperandTop->Quad /= operand.Quad;
				}
				else
				{
					if (!operand.Real)
						goto DIVIDE_BY_ZERO;
					lpOperandTop->Real /= operand.Real;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_IMOD:
			if (IsInteger)
			{
				VARIAUNT operand;
				BOOL     IsQuad;

				operand = OPERAND_POP();
				if (!operand.Low && (!operand.IsQuad || !operand.High))
					goto DIVIDE_BY_ZERO;
				IsQuad = lpOperandTop->IsQuad | operand.IsQuad;
				if (!IsQuad)
					lpOperandTop->Low = (int32_t)lpOperandTop->Low % (int32_t)operand.Low;
				else
					lpOperandTop->Quad = (lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) % (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
				lpOperandTop->IsQuad = IsQuad;
				break;
			}
			/* FALLTHROUGH */
		case TAG_MOD:
			{
				VARIAUNT operand;

				if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
				{
					operand = OPERAND_POP();
				}
				else
				{
					VARIAUNT swap = OPERAND_POP();
					operand = *lpOperandTop;
					*lpOperandTop = swap;
				}
				if (IsInteger)
				{
					if (!operand.Low && (!operand.IsQuad || !operand.High))
						goto DIVIDE_BY_ZERO;
					if (!(lpOperandTop->IsQuad |= operand.IsQuad))
						lpOperandTop->Low %= operand.Low;
					else
						lpOperandTop->Quad %= operand.Quad;
				}
				else
				{
					if (!operand.Real)
						goto DIVIDE_BY_ZERO;
					lpOperandTop->Real = fmod(lpOperandTop->Real, operand.Real);
					lpOperandTop->IsQuad = TRUE;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_NEG:
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = -(int32_t)lpOperandTop->Low;
				else
					lpOperandTop->Quad = -(int64_t)lpOperandTop->Quad;
			}
			else
			{
				lpOperandTop->Real = -lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_SHL:
			{
				VARIAUNT operand;

				if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
				{
					operand = OPERAND_POP();
				}
				else
				{
					VARIAUNT swap = OPERAND_POP();
					operand = *lpOperandTop;
					*lpOperandTop = swap;
				}
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad << operand.Low;
					if (!lpOperandTop->IsQuad)
						lpOperandTop->High = 0;
				}
				else
				{
					lpOperandTop->Real = ldexp(lpOperandTop->Real, (int)operand.Real);
					lpOperandTop->IsQuad = TRUE;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_SAR:
			if (IsInteger)
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Quad = (long)lpOperandTop->Low;
				lpOperandTop->Quad = (int64_t)lpOperandTop->Quad >> operand.Low;
				if (!lpOperandTop->IsQuad)
					lpOperandTop->High = 0;
				break;
			}
			/* FALLTHROUGH */
		case TAG_SHR:
			{
				VARIAUNT operand;

				if (!(lpMarkup->Type & OS_LEFT_ASSIGN))
				{
					operand = OPERAND_POP();
				}
				else
				{
					VARIAUNT swap = OPERAND_POP();
					operand = *lpOperandTop;
					*lpOperandTop = swap;
				}
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad >> operand.Low;
				}
				else
				{
					lpOperandTop->Real = ldexp(lpOperandTop->Real, -(int)operand.Real);
					lpOperandTop->IsQuad = TRUE;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_ROL:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (!IsInteger)
				{
					int32_t msw;

					msw = lpOperandTop->High;
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
					operand.Low = (int)operand.Real;
				}
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = _rotl(lpOperandTop->Low, operand.Low);
				else
					lpOperandTop->Quad = _rotl64(lpOperandTop->Quad, operand.Low);
				if (!IsInteger)
				{
					lpOperandTop->Real = (double)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_ROR:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (!IsInteger)
				{
					int32_t msw;

					msw = lpOperandTop->High;
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
					operand.Low = (int)operand.Real;
				}
				if (!lpOperandTop->IsQuad)
					lpOperandTop->Low = _rotr(lpOperandTop->Low, operand.Low);
				else
					lpOperandTop->Quad = _rotr64(lpOperandTop->Quad, operand.Low);
				if (!IsInteger)
				{
					lpOperandTop->Real = (double)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_BIT_AND:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad &= operand.Quad;
					lpOperandTop->IsQuad |= operand.IsQuad;
				}
				else
				{
					lpOperandTop->Real = fband(lpOperandTop->Real, operand.Real);
					lpOperandTop->IsQuad = TRUE;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_BIT_OR:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad |= operand.Quad;
					lpOperandTop->IsQuad |= operand.IsQuad;
				}
				else
				{
					lpOperandTop->Real = fbor(lpOperandTop->Real, operand.Real);
					lpOperandTop->IsQuad = TRUE;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_XOR:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad ^= operand.Quad;
					lpOperandTop->IsQuad |= operand.IsQuad;
				}
				else
				{
					lpOperandTop->Real = fbxor(lpOperandTop->Real, operand.Real);
					lpOperandTop->IsQuad = TRUE;
				}
				if (bCompoundAssign)
					lpPostfix -= 2;
			}
			break;
		case TAG_BIT_NOT:
			if (IsInteger)
			{
				lpOperandTop->Low = ~lpOperandTop->Low;
				if (lpOperandTop->IsQuad)
					lpOperandTop->High = ~lpOperandTop->High;
			}
			else
			{
				lpOperandTop->Real = fbnot32(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_AND:
			if (IsInteger)
			{
				boolValue = !!lpOperandTop->Quad;
				if (!(lpMarkup->Type & OS_YIELD_OPERAND))
				{
					lpOperandTop->Quad = boolValue;
					lpOperandTop->IsQuad = FALSE;
				}
			}
			else
			{
				boolValue = !!lpOperandTop->Real;
				if (!(lpMarkup->Type & OS_YIELD_OPERAND))
				{
					lpOperandTop->Real = boolValue;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			if (lpMarkup->Type & OS_SHORT_CIRCUIT)
			{
				if (!boolValue)
				{
					while (++lpPostfix < lpEndOfPostfix)
					{
						if ((*lpPostfix)->Tag != TAG_AND)
							continue;
						if ((*lpPostfix)->String != lpMarkup->String)
							continue;
						break;
					}
				}
				else
				{
					OPERAND_POP();
					continue;
				}
			}
			break;
		case TAG_OR:
			if (IsInteger)
			{
				boolValue = !!lpOperandTop->Quad;
				if (!(lpMarkup->Type & OS_YIELD_OPERAND))
				{
					lpOperandTop->Quad = boolValue;
					lpOperandTop->IsQuad = FALSE;
				}
			}
			else
			{
				boolValue = !!lpOperandTop->Real;
				if (!(lpMarkup->Type & OS_YIELD_OPERAND))
				{
					lpOperandTop->Real = boolValue;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			if (lpMarkup->Type & OS_SHORT_CIRCUIT)
			{
				if (boolValue)
				{
					while (++lpPostfix < lpEndOfPostfix)
					{
						if ((*lpPostfix)->Tag != TAG_OR)
							continue;
						if ((*lpPostfix)->String != lpMarkup->String)
							continue;
						break;
					}
				}
				else
				{
					OPERAND_POP();
					continue;
				}
			}
			break;
		case TAG_NOT:
			if (IsInteger)
			{
				lpOperandTop->Quad = !lpOperandTop->Quad;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				lpOperandTop->Real = !lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_EQ:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad == operand.Quad;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = lpOperandTop->Real == operand.Real;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_NE:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad != operand.Quad;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = lpOperandTop->Real != operand.Real;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_LT:
			if (IsInteger)
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low < (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) < (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
			/* FALLTHROUGH */
		case TAG_BT:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad < operand.Quad;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = lpOperandTop->Real < operand.Real;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_GT:
			if (IsInteger)
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low > (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) > (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
			/* FALLTHROUGH */
		case TAG_AT:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad > operand.Quad;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = lpOperandTop->Real > operand.Real;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_LE:
			if (IsInteger)
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low <= (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) <= (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
			/* FALLTHROUGH */
		case TAG_BE:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad <= operand.Quad;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = lpOperandTop->Real <= operand.Real;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_GE:
			if (IsInteger)
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = !(lpOperandTop->IsQuad | operand.IsQuad) ?
					(int32_t)lpOperandTop->Low >= (int32_t)operand.Low :
					(lpOperandTop->IsQuad ? (int64_t)lpOperandTop->Quad : (int32_t)lpOperandTop->Low) >= (operand.IsQuad ? (int64_t)operand.Quad : (int32_t)operand.Low);
				lpOperandTop->IsQuad = FALSE;
				break;
			}
			/* FALLTHROUGH */
		case TAG_AE:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (IsInteger)
				{
					lpOperandTop->Quad = lpOperandTop->Quad >= operand.Quad;
					lpOperandTop->IsQuad = FALSE;
				}
				else
				{
					lpOperandTop->Real = lpOperandTop->Real >= operand.Real;
					lpOperandTop->IsQuad = TRUE;
				}
			}
			break;
		case TAG_TERNARY:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				if (OPERAND_IS_EMPTY())
					goto PARSING_ERROR;
				if (IsInteger ? !operand.Quad : !operand.Real)
				{
					if (lpMarkup->FalsePart)
						lpPostfix = lpMarkup->FalsePart;
					else
					{
						size_t nDepth = 0;

						while (++lpPostfix < lpEndOfPostfix)
						{
							if ((*lpPostfix)->Tag == TAG_TERNARY)
								nDepth++;
							else if ((*lpPostfix)->Tag == TAG_TERNARY_SPLIT && (*lpPostfix)->Type & OS_TERNARIES && !nDepth--)
								break;
						}
						if (lpPostfix < lpEndOfPostfix)
							lpMarkup->FalsePart = lpPostfix;
						else
							goto PARSING_ERROR;
					}
				}
			}
			break;
		case TAG_TERNARY_SPLIT:
			if (lpMarkup->Jump)
				lpPostfix = lpMarkup->Jump;
			else if (lpMarkup->Type & OS_TERNARIES)
			{
				for (; lpPostfix + 1 < lpEndOfPostfix; lpPostfix++)
					if (lpPostfix[1]->Type & OS_TERNARY_END)
						break;
				if (lpPostfix + 1 < lpEndOfPostfix)
					lpMarkup->Jump = lpPostfix;
				else
					goto PARSING_ERROR;
			}
			else
				goto PARSING_ERROR;
			continue;
		case TAG_INDIRECTION:
			nSize = sizeof(DWORD) << (FixTheProcedure && !IsInteger);
			goto PROCESS_MEMORY;
		case TAG_REMOTE1:
		case TAG_REMOTE2:
		case TAG_REMOTE3:
		case TAG_REMOTE4:
		case TAG_REMOTE5:
		case TAG_REMOTE6:
		case TAG_REMOTE7:
		case TAG_REMOTE8:
			nSize = lpMarkup->Tag - TAG_REMOTE1 + 1;
			goto PROCESS_MEMORY;
		case TAG_REMOTE_INTEGER1:
		case TAG_REMOTE_INTEGER2:
		case TAG_REMOTE_INTEGER3:
		case TAG_REMOTE_INTEGER4:
		case TAG_REMOTE_INTEGER5:
		case TAG_REMOTE_INTEGER6:
		case TAG_REMOTE_INTEGER7:
		case TAG_REMOTE_INTEGER8:
			nSize = lpMarkup->Tag - TAG_REMOTE_INTEGER1 + 1;
			goto PROCESS_MEMORY;
		case TAG_REMOTE_REAL4:
		case TAG_REMOTE_FLOAT4:
			nSize = 4;
			goto PROCESS_MEMORY;
		case TAG_REMOTE_REAL8:
			nSize = 8;
		PROCESS_MEMORY:
			if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
				goto OPEN_ERROR;
			if (bCompoundAssign)
			{
				VARIAUNT op1 = OPERAND_POP();
				VARIAUNT op2 = *lpOperandTop;
				*lpOperandTop = op1;
				OPERAND_PUSH(op2);
			}
			lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
			if (!bCompoundAssign && (!(lpNext = lpPostfix + 1 < lpEndOfPostfix ? lpPostfix[1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
			{
				bCompoundAssign = lpNext && (lpNext->Type & OS_LEFT_ASSIGN);
				if (bCompoundAssign)
				{
					VARIAUNT op1 = OPERAND_POP();
					VARIAUNT op2 = *lpOperandTop;
					*lpOperandTop = op1;
					OPERAND_PUSH(op2);
					OPERAND_PUSH(op2);
					if (FixTheProcedure && lpMarkup->Tag == TAG_INDIRECTION)
						nSize = sizeof(DWORD) << lpOperandTop->IsQuad;
				}
				lpOperandTop->Quad = 0;
				if (!ReadProcessMemory(hProcess, lpAddress, &lpOperandTop->Quad, nSize, NULL))
				{
					if (this->ssgActionListner)
						TSSGActionListner_OnSubjectReadError(this->ssgActionListner, SSGS, (uint32_t)lpAddress);
					lpOperandTop->Quad = 0;
				}
				switch (lpMarkup->Tag)
				{
				case TAG_REMOTE1:
				case TAG_REMOTE2:
				case TAG_REMOTE3:
					if (lpOperandTop->IsQuad = !IsInteger)
						goto PARSING_ERROR;
					break;
				case TAG_INDIRECTION:
					if (FixTheProcedure)
					{
						lpOperandTop->IsQuad = nSize > sizeof(DWORD);
						break;
					}
					/* FALLTHROUGH */
				case TAG_REMOTE4:
					if (lpOperandTop->IsQuad = !IsInteger)
						lpOperandTop->Real = lpOperandTop->Float;
					break;
				case TAG_REMOTE5:
				case TAG_REMOTE6:
				case TAG_REMOTE7:
					lpOperandTop->IsQuad = TRUE;
					if (!IsInteger)
						goto PARSING_ERROR;
					break;
				case TAG_REMOTE_INTEGER1:
				case TAG_REMOTE_INTEGER2:
				case TAG_REMOTE_INTEGER3:
				case TAG_REMOTE_INTEGER4:
					if (lpOperandTop->IsQuad = !IsInteger)
						lpOperandTop->Real = lpOperandTop->Low;
					break;
				case TAG_REMOTE_INTEGER5:
				case TAG_REMOTE_INTEGER6:
				case TAG_REMOTE_INTEGER7:
				case TAG_REMOTE_INTEGER8:
					if (!IsInteger)
					{
						lpOperandTop->Real = (double)lpOperandTop->Quad;
						lpOperandTop->IsQuad = TRUE;
					}
					break;
				case TAG_REMOTE_REAL4:
					if (!IsInteger)
					{
						lpOperandTop->Real = lpOperandTop->Float;
						lpOperandTop->IsQuad = TRUE;
					}
					else
					{
						int32_t msw;

						msw = lpOperandTop->Low;
						lpOperandTop->Quad = (uint64_t)lpOperandTop->Float;
						if (lpOperandTop->IsQuad = lpOperandTop->High)
							if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
								lpOperandTop->High = 0;
					}
					break;
				case TAG_REMOTE_REAL8:
					if (IsInteger)
					{
						int32_t msw;

						msw = lpOperandTop->High;
						lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						if (lpOperandTop->IsQuad = lpOperandTop->High)
							if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
								lpOperandTop->High = 0;
					}
					break;
				case TAG_REMOTE_FLOAT4:
					lpOperandTop->Real = lpOperandTop->Float;
					/* FALLTHROUGH */
				default:
					lpOperandTop->IsQuad = TRUE;
					break;
				}
			}
			else
			{
				uint64_t qw;

				OPERAND_POP();
				qw = lpOperandTop->Quad;
				switch (lpMarkup->Tag)
				{
				case TAG_REMOTE1:
				case TAG_REMOTE2:
				case TAG_REMOTE3:
				case TAG_REMOTE5:
				case TAG_REMOTE6:
				case TAG_REMOTE7:
					if (!IsInteger)
						goto PARSING_ERROR;
					break;
				case TAG_INDIRECTION:
					if (FixTheProcedure)
					{
						nSize = sizeof(DWORD) << lpOperandTop->IsQuad;
						break;
					}
					/* FALLTHROUGH */
				case TAG_REMOTE4:
					if (IsInteger) break;
					/* FALLTHROUGH */
				case TAG_REMOTE_FLOAT4:
					if (lpOperandTop->IsQuad)
						*(float *)&qw = (float)*(double *)&qw;
					break;
				case TAG_REMOTE_INTEGER1:
				case TAG_REMOTE_INTEGER2:
				case TAG_REMOTE_INTEGER3:
				case TAG_REMOTE_INTEGER4:
					if (!IsInteger)
						*(uint32_t *)&qw = (uint32_t)*(double *)&qw;
					break;
				case TAG_REMOTE_INTEGER5:
				case TAG_REMOTE_INTEGER6:
				case TAG_REMOTE_INTEGER7:
				case TAG_REMOTE_INTEGER8:
					if (!IsInteger)
						qw = (uint64_t)*(double *)&qw;
					break;
				case TAG_REMOTE_REAL4:
					*(float *)&qw = IsInteger ? (float)qw : (float)*(double *)&qw;
					break;
				case TAG_REMOTE_REAL8:
					if (IsInteger)
						*(double *)&qw = (double)qw;
					break;
				}
				if (!WriteProcessMemory(hProcess, lpAddress, &qw, nSize, NULL))
					goto WRITE_ERROR;
				lpPostfix++;
				if (bCompoundAssign)
				{
					bCompoundAssign = FALSE;
					continue;
				}
			}
			if (lpMarkup->Tag != TAG_INDIRECTION)
				break;
			if (!this->ssgActionListner)
				continue;
			lpGuideText = "* 間接参照";
#if !defined(__BORLANDC__)
			nGuideTextLength = 10;
#endif
			goto OUTPUT_GUIDE;
#if LOCAL_MEMORY_SUPPORT
		case TAG_LOCAL1:
		case TAG_LOCAL2:
		case TAG_LOCAL3:
		case TAG_LOCAL4:
		case TAG_LOCAL5:
		case TAG_LOCAL6:
		case TAG_LOCAL7:
		case TAG_LOCAL8:
			nSize = lpMarkup->Tag - TAG_LOCAL1 + 1;
			goto LOCAL_MEMORY;
		case TAG_LOCAL_INTEGER1:
		case TAG_LOCAL_INTEGER2:
		case TAG_LOCAL_INTEGER3:
		case TAG_LOCAL_INTEGER4:
		case TAG_LOCAL_INTEGER5:
		case TAG_LOCAL_INTEGER6:
		case TAG_LOCAL_INTEGER7:
		case TAG_LOCAL_INTEGER8:
			nSize = lpMarkup->Tag - TAG_LOCAL_INTEGER1 + 1;
			goto LOCAL_MEMORY;
		case TAG_LOCAL_REAL4:
		case TAG_LOCAL_FLOAT4:
			nSize = 4;
			goto LOCAL_MEMORY;
		case TAG_LOCAL_REAL8:
			nSize = 8;
		LOCAL_MEMORY:
			if (bCompoundAssign)
			{
				VARIAUNT op1 = OPERAND_POP();
				VARIAUNT op2 = *lpOperandTop;
				*lpOperandTop = op1;
				OPERAND_PUSH(op2);
			}
			lpAddress = IsInteger ? (LPVOID)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
			if (!bCompoundAssign && (!(lpNext = lpPostfix + 1 < lpEndOfPostfix ? lpPostfix[1] : NULL) || lpNext->Tag != TAG_LEFT_ASSIGN))
			{
				bCompoundAssign = lpNext && (lpNext->Type & OS_LEFT_ASSIGN);
				if (bCompoundAssign)
				{
					VARIAUNT op1 = OPERAND_POP();
					VARIAUNT op2 = *lpOperandTop;
					*lpOperandTop = op1;
					OPERAND_PUSH(op2);
					OPERAND_PUSH(op2);
				}
				if (!IsBadReadPtr(lpAddress, nSize))
				{
					switch (nSize)
					{
					case 1:
						lpOperandTop->Quad = *(uint8_t *)lpAddress;
						break;
					case 2:
						lpOperandTop->Quad = *(uint16_t *)lpAddress;
						break;
					case 3:
						lpOperandTop->Quad = *(uint16_t *)lpAddress | ((uint32_t)*((uint8_t *)lpAddress + 2) << 16);
						break;
					case 4:
						lpOperandTop->Quad = *(uint32_t *)lpAddress;
						break;
					case 5:
						lpOperandTop->Quad = *(uint32_t *)lpAddress | ((uint64_t)*((uint8_t *)lpAddress + 4) << 32);
						break;
					case 6:
						lpOperandTop->Quad = *(uint32_t *)lpAddress | ((uint64_t)*((uint16_t *)lpAddress + 2) << 32);
						break;
					case 7:
						lpOperandTop->Quad = *(uint32_t *)lpAddress | ((uint64_t)*((uint16_t *)lpAddress + 2) << 32) | ((uint64_t)*((uint8_t *)lpAddress + 6) << 48);
						break;
					case 8:
						lpOperandTop->Quad = *(uint64_t *)lpAddress;
						break;
					}
					switch (lpMarkup->Tag)
					{
					case TAG_LOCAL1:
					case TAG_LOCAL2:
					case TAG_LOCAL3:
						if (lpOperandTop->IsQuad = !IsInteger)
							goto PARSING_ERROR;
						break;
					case TAG_LOCAL4:
						if (lpOperandTop->IsQuad = !IsInteger)
							lpOperandTop->Real = lpOperandTop->Float;
						break;
					case TAG_LOCAL5:
					case TAG_LOCAL6:
					case TAG_LOCAL7:
						lpOperandTop->IsQuad = TRUE;
						if (!IsInteger)
							goto PARSING_ERROR;
						break;
					case TAG_LOCAL_INTEGER1:
					case TAG_LOCAL_INTEGER2:
					case TAG_LOCAL_INTEGER3:
					case TAG_LOCAL_INTEGER4:
						if (lpOperandTop->IsQuad = !IsInteger)
							lpOperandTop->Real = lpOperandTop->Low;
						break;
					case TAG_LOCAL_INTEGER5:
					case TAG_LOCAL_INTEGER6:
					case TAG_LOCAL_INTEGER7:
					case TAG_LOCAL_INTEGER8:
						if (!IsInteger)
							lpOperandTop->Real = (double)lpOperandTop->Quad;
						lpOperandTop->IsQuad = TRUE;
						break;
					case TAG_LOCAL_REAL4:
						if (!IsInteger)
						{
							lpOperandTop->Real = lpOperandTop->Float;
							lpOperandTop->IsQuad = TRUE;
						}
						else
						{
							int32_t msw;

							msw = lpOperandTop->Low;
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Float;
							if (lpOperandTop->IsQuad = lpOperandTop->High)
								if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
									lpOperandTop->High = 0;
						}
						break;
					case TAG_LOCAL_REAL8:
						if (IsInteger)
						{
							int32_t msw;

							msw = lpOperandTop->High;
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
							if (lpOperandTop->IsQuad = lpOperandTop->High)
								if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
									lpOperandTop->High = 0;
						}
						break;
					case TAG_LOCAL_FLOAT4:
						lpOperandTop->Real = lpOperandTop->Float;
						/* FALLTHROUGH */
					default:
						lpOperandTop->IsQuad = TRUE;
						break;
					}
				}
				else
				{
					if (this->ssgActionListner)
						TSSGActionListner_OnSubjectReadError(this->ssgActionListner, SSGS, (uint32_t)lpAddress);
					lpOperandTop->Quad = 0;
					lpOperandTop->IsQuad = nSize > sizeof(uint32_t);
				}
			}
			else
			{
				uint64_t qw;

				if (IsBadWritePtr(lpAddress, nSize))
					goto WRITE_ERROR;
				OPERAND_POP();
				qw = lpOperandTop->Quad;
				switch (lpMarkup->Tag)
				{
				case TAG_LOCAL1:
				case TAG_LOCAL2:
				case TAG_LOCAL3:
				case TAG_LOCAL5:
				case TAG_LOCAL6:
				case TAG_LOCAL7:
					if (!IsInteger)
						goto PARSING_ERROR;
					break;
				case TAG_LOCAL4:
					if (IsInteger) break;
					/* FALLTHROUGH */
				case TAG_LOCAL_FLOAT4:
					if (lpOperandTop->IsQuad)
						*(float *)&qw = (float)*(double *)&qw;
					break;
				case TAG_LOCAL_INTEGER1:
				case TAG_LOCAL_INTEGER2:
				case TAG_LOCAL_INTEGER3:
				case TAG_LOCAL_INTEGER4:
					if (!IsInteger)
						*(uint32_t *)&qw = (uint32_t)*(double *)&qw;
					break;
				case TAG_LOCAL_INTEGER5:
				case TAG_LOCAL_INTEGER6:
				case TAG_LOCAL_INTEGER7:
				case TAG_LOCAL_INTEGER8:
					if (!IsInteger)
						qw = (uint64_t)*(double *)&qw;
					break;
				case TAG_LOCAL_REAL4:
					*(float *)&qw = IsInteger ? (float)qw : (float)*(double *)&qw;
					break;
				case TAG_LOCAL_REAL8:
					if (IsInteger)
						*(double *)&qw = (double)qw;
					break;
				}
				switch (nSize)
				{
				case 1:
					*(uint8_t *)lpAddress = (uint8_t)qw;
					break;
				case 2:
					*(uint16_t *)lpAddress = (uint16_t)qw;
					break;
				case 3:
					*(uint16_t *)lpAddress = (uint16_t)qw;
					*((uint8_t *)lpAddress + 2) = (uint8_t)((uint32_t)qw >> 16);
					break;
				case 4:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					break;
				case 5:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					*((uint8_t *)lpAddress + 4) = (uint8_t)(qw >> 32);
					break;
				case 6:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					*((uint16_t *)lpAddress + 2) = (uint16_t)(qw >> 32);
					break;
				case 7:
					*(uint32_t *)lpAddress = (uint32_t)qw;
					*((uint16_t *)lpAddress + 2) = (uint16_t)(qw >> 32);
					*((uint8_t *)lpAddress + 6) = (uint8_t)((uint32_t)(qw >> 32) >> 16);
					break;
				case 8:
					*(uint64_t *)lpAddress = qw;
					break;
				}
				lpPostfix++;
				if (bCompoundAssign)
				{
					bCompoundAssign = FALSE;
					continue;
				}
			}
			break;
#endif
		case TAG_REV_ENDIAN2:
			lpOperandTop->Quad = __intrinsic_bswap32(lpOperandTop->Low) >> 16;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN3:
			lpOperandTop->Quad = __intrinsic_bswap24(lpOperandTop->Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN4:
			lpOperandTop->Quad = __intrinsic_bswap32(lpOperandTop->Low);
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_REV_ENDIAN5:
			lpOperandTop->Quad = __intrinsic_bswap40(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_REV_ENDIAN6:
			lpOperandTop->Quad = __intrinsic_bswap48(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_REV_ENDIAN7:
			lpOperandTop->Quad = __intrinsic_bswap56(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_REV_ENDIAN8:
			lpOperandTop->Quad = __intrinsic_bswap64(lpOperandTop->Quad);
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_ADDR_REPLACE:
			if (!IsInteger)
				lpOperandTop->Quad = (uintptr_t)lpOperandTop->Real;
			if (TSSGCtrl_AddressAttributeFilter(this, SSGS, (unsigned long *)&lpOperandTop->Quad, atREPLACE) != 0)
				goto FAILED_ADDR_REPLACE;
			if (!(lpOperandTop->IsQuad = !IsInteger))
				lpOperandTop->High = 0;
			else
				lpOperandTop->Real = *(uintptr_t *)&lpOperandTop->Quad;
			break;
		case TAG_ADDR_ADJUST:
			if (!IsInteger)
				lpOperandTop->Quad = (uintptr_t)lpOperandTop->Real;
			if (TSSGCtrl_AddressAttributeFilter(this, SSGS, (unsigned long *)&lpOperandTop->Quad, atADJUST) != 0)
				goto FAILED_ADDR_ADJUST;
			if (!(lpOperandTop->IsQuad = !IsInteger))
				lpOperandTop->High = 0;
			else
				lpOperandTop->Real = *(uintptr_t *)&lpOperandTop->Quad;
			break;
#if 0
		case TAG_LEFT_ASSIGN:
			{
				size_t length;
				LPSTR  p;
				size_t i;

				if (lpPostfix + 1 >= lpEndOfPostfix)
					goto PARSING_ERROR;
				lpNext = lpPostfix[1];
				if (lpNext->Tag != TAG_NOT_OPERATOR)
					goto PARSING_ERROR;
				if (*lpNext->String == '$')
				{
					length = lpNext->Length - 1;
					if (!length)
						goto PARSING_ERROR;
					p = lpNext->String + 1;
				}
				else
				{
					length = lpNext->Length;
					p = lpNext->String;
				}
				for (i = 0; i < nNumberOfVariable; i++)
					if (lpVariable[i].Length == length)
						if (memcmp(lpVariable[i].String, p, length) == 0)
							break;
				if (i < nNumberOfVariable)
				{
					lpVariable[i].Value = *lpOperandTop;
				}
				else
				{
					if (!(nNumberOfVariable & 0x0F))
					{
						LPVOID lpMem;
						size_t nBytes;

						nBytes = (nNumberOfVariable + 0x10) * sizeof(VARIABLE);
						lpMem = HeapReAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, lpVariable, nBytes);
						if (!lpMem)
							goto ALLOC_ERROR;
						lpVariable = (VARIABLE *)lpMem;
					}
					lpVariable[nNumberOfVariable].Length = length;
					lpVariable[nNumberOfVariable].String = p;
					lpVariable[nNumberOfVariable].Value = *lpOperandTop;
					nNumberOfVariable++;
				}
				lpPostfix++;
			}
			break;
#endif
		case TAG_PARSE_INT:
			if (lpMarkup->Close)
				lpMarkup->Close->Tag = IsInteger ? TAG_PARSE_INT : TAG_PARSE_REAL;
			IsInteger = TRUE;
			operandZero.IsQuad = FALSE;
			if (lpMarkup->Type & OS_CLOSE)
				continue;
			break;
		case TAG_PARSE_REAL:
			if (lpMarkup->Close)
				lpMarkup->Close->Tag = IsInteger ? TAG_PARSE_INT : TAG_PARSE_REAL;
			IsInteger = FALSE;
			operandZero.IsQuad = TRUE;
			if (lpMarkup->Type & OS_CLOSE)
				continue;
			break;
		case TAG_PARSE_RESET:
			if (lpMarkup->Close)
				lpMarkup->Close->Tag = IsInteger ? TAG_PARSE_INT : TAG_PARSE_REAL;
			IsInteger = bInitialIsInteger;
			operandZero.IsQuad = !IsInteger;
			if (lpMarkup->Type & OS_CLOSE)
				continue;
			break;
		case TAG_PROCEDURE:
			{
				VARIAUNT operand;

				if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					goto OPEN_ERROR;
				operand = OPERAND_POP();
				if (!IsInteger)
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (!operand.High && IS_INTRESOURCE(operand.Low))
				{
					lpOperandTop->Quad = (uintptr_t)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, (LPSTR)operand.Quad);
#if INTPTR_MAX > INT32_MAX
					lpOperandTop->IsQuad = TRUE;
					if (!IsInteger)
#else
					if (lpOperandTop->IsQuad = !IsInteger)
#endif
						lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				}
				else
				{
					lpOperandTop->Quad = 0;
					if (IsInteger)
						lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
				}
			}
			break;
		case TAG_MODULENAME:
			{
				VARIAUNT operand;

				if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					goto OPEN_ERROR;
				operand = OPERAND_POP();
				if (!IsInteger)
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (!operand.High && IS_INTRESOURCE(operand.Low))
				{
					char   c;
					LPVOID lpFunction;

					c = lpMarkup->String[lpMarkup->Length];
					lpMarkup->String[lpMarkup->Length] = '\0';
					lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpMarkup->String, (LPSTR)operand.Quad);
					if (lpFunction && lpMarkup[-1].Tag == TAG_IMPORT_FUNCTION)
						if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
							lpFunction = NULL;
					lpOperandTop->Quad = (uintptr_t)lpFunction;
					lpMarkup->String[lpMarkup->Length] = c;
#if INTPTR_MAX > INT32_MAX
					lpOperandTop->IsQuad = TRUE;
					if (!IsInteger)
#else
					if (lpOperandTop->IsQuad = !IsInteger)
#endif
						lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				}
				else
				{
					lpOperandTop->Quad = 0;
					if (IsInteger)
						lpOperandTop->IsQuad = sizeof(FARPROC) > sizeof(uint32_t);
				}
			}
			break;
		case TAG_HNUMBER:
			{
				THeapListData *HeapL;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (!IsInteger)
					lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (!lpOperandTop->High && (HeapL = TProcessCtrl_GetHeapList(&this->processCtrl, lpOperandTop->Low - 1)))
					if (IsInteger)
						lpOperandTop->Quad = HeapL->heapListAddress;
					else
						lpOperandTop->Real = HeapL->heapListAddress;
				else
					lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger || sizeof(((THeapListData *)NULL)->heapListAddress) > sizeof(uint32_t);
			}
			break;
#if ALLOCATE_SUPPORT
		case TAG_MEMORY:
			{
				LPVOID lpAddress;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (!IsInteger)
					lpOperandTop->Quad = (DWORD)(long)lpOperandTop->Real;
				lpAddress = NULL;
				if (lpOperandTop->Quad >> sizeof(lpProcessMemory[0].Id) * 8)
					goto PARSING_ERROR;
				else
				{
					FILETIME creationTime = { 0, 0 };

					for (size_t i = 0; i < nNumberOfProcessMemory; i++)
					{
						size_t allocSize;

						if (lpOperandTop->Quad != lpProcessMemory[i].Id)
							continue;
						if (lpProcessMemory[i].Address)
						{
							lpAddress = lpProcessMemory[i].Address;
							break;
						}
						allocSize = lpProcessMemory[i].Size;
#ifdef _WIN64
						if (!IsInteger)
							allocSize += ULL2DBL_LOST_MAX;
#endif
						if (lpProcessMemory[i].Protect)
						{
							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto OPEN_ERROR;
							if (!creationTime.dwLowDateTime && !creationTime.dwHighDateTime)
							{
								FILETIME exitTime, kernelTime, userTime;

								if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
									goto FAILED_GET_PROCESS_TIMES;
								if (ftProcessCreationTime.dwLowDateTime || ftProcessCreationTime.dwHighDateTime)
								{
									if (creationTime.dwLowDateTime  != ftProcessCreationTime.dwLowDateTime ||
										creationTime.dwHighDateTime != ftProcessCreationTime.dwHighDateTime)
									{
										goto FAILED_GET_PROCESS_TIMES;
									}
								}
								else
								{
									ftProcessCreationTime = creationTime;
								}
							}
							lpProcessMemory[i].Address = lpAddress = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, lpProcessMemory[i].Protect);
						}
						else
						{
							lpProcessMemory[i].Address = lpAddress = HeapAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, allocSize);
							if (!lpAddress) goto ALLOC_ERROR;
						}
						break;
					}
				}
				lpOperandTop->Quad = (uintptr_t)lpAddress;
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
				{
#ifdef _WIN64
					lpOperandTop->Quad = (lpOperandTop->Quad + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				}
			}
			break;
#endif
		case TAG_ISBADCODEPTR:
		case TAG_ISBADREADPTR:
		case TAG_ISBADWRITEPTR:
		case TAG_ISBADSTRINGPTRA:
		case TAG_ISBADSTRINGPTRW:
			{
				HANDLE hTargetProcess;
				LPVOID lp;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lp = IsInteger ? (LPVOID)lpOperandTop[0].Quad : (LPVOID)(size_t)lpOperandTop[0].Real;
				if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = GetCurrentProcess();
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				if (lpMarkup->Tag == TAG_ISBADCODEPTR)
				{
					lpOperandTop->Quad = IsBadCodePtrEx(hTargetProcess, lp);
				}
				else
				{
					UINT_PTR ucb;

					if (IsStringOperand(lpMarkup->Next->Param))
						goto PARSING_ERROR;
					ucb = IsInteger ? (UINT_PTR)lpOperandTop[1].Quad : (UINT_PTR)lpOperandTop[1].Real;
					switch (lpMarkup->Tag)
					{
					case TAG_ISBADREADPTR:
						lpOperandTop->Quad = IsBadReadPtrEx(hTargetProcess, lp, ucb);
						break;
					case TAG_ISBADWRITEPTR:
						lpOperandTop->Quad = IsBadWritePtrEx(hTargetProcess, lp, ucb);
						break;
					case TAG_ISBADSTRINGPTRA:
						lpOperandTop->Quad = IsBadStringPtrExA(hTargetProcess, lp, ucb);
						break;
					case TAG_ISBADSTRINGPTRW:
						lpOperandTop->Quad = IsBadStringPtrExW(hTargetProcess, lp, ucb);
						break;
					default:
						__assume(0);
					}
				}
				if (lpOperandTop->IsQuad = !IsInteger)
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
			}
			break;
		case TAG_CAST32:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger)
			{
			CLEAR_HIGH_DWORD:
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else if (bInitialIsInteger)
			{
				lpOperandTop->Float = (float)lpOperandTop->Real;
				goto CLEAR_HIGH_DWORD;
			}
			else
			{
				// floating-point calculation is always 64bit(double).
				// this cast is lost value of 'double' to 'float'.
				lpOperandTop->Real = (float)lpOperandTop->Real;
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_CAST64:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger && bInitialIsInteger && !lpOperandTop->IsQuad)
				lpOperandTop->Real = lpOperandTop->Float;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_I1TOI4:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = (uint32_t)(int8_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I2TOI4:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = (uint32_t)(int16_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = FALSE;
			break;
		case TAG_I4TOI8:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = (int64_t)(int32_t)lpOperandTop->Low;
			if (IsInteger)
				lpOperandTop->IsQuad = TRUE;
			else if (!lpOperandTop->IsQuad)
				lpOperandTop->High = 0;
			break;
		case TAG_UTOF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Real = (double)lpOperandTop->Quad;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_ITOF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Real = lpOperandTop->IsQuad ? (double)(int64_t)lpOperandTop->Quad : (double)(int32_t)lpOperandTop->Quad;
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_FTOI:
			{
				int32_t msw;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				msw = lpOperandTop->High;
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
				if (lpOperandTop->IsQuad = lpOperandTop->High)
					if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
						lpOperandTop->High = 0;
			}
			break;
		case TAG_TRUNC:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
			{
				lpOperandTop->Real = trunc(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ROUND:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
			{
				lpOperandTop->Real = round(lpOperandTop->Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ISFINITE:
		case TAG_ISINF:
		case TAG_ISNAN:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			switch (lpMarkup->Tag)
			{
			case TAG_ISFINITE:
				boolValue = (lpOperandTop->High & 0x7FF00000) < 0x7FF00000;
				break;
			case TAG_ISINF:
				boolValue = (lpOperandTop->Quad & 0x7FFFFFFFFFFFFFFF) == 0x7FF0000000000000;
				break;
			case TAG_ISNAN:
				boolValue = (lpOperandTop->Quad & 0x7FFFFFFFFFFFFFFF) > 0x7FF0000000000000;
				break;
			default:
				__assume(0);
			}
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = boolValue;
			else
				lpOperandTop->Quad = boolValue;
			break;
		case TAG_BSF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad)
				{
					if (!_BitScanForward(&lpOperandTop->Low, lpOperandTop->Low))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				else
				{
					if (!_BitScanForward64(&lpOperandTop->Low, lpOperandTop->Quad))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				int32_t exp;

				if ((lpOperandTop->Quad & 0x7FFFFFFFFFFFFFFF) && (exp = (lpOperandTop->High & 0x7FF00000)) != 0x7FF00000)
				{
					unsigned long bits;

					_BitScanForward64(&bits, lpOperandTop->Quad);
					lpOperandTop->Real = (exp >> 20) - (1023 + 52) + (bits <= 52 ? (int32_t)bits : 52);
				}
				else
				{
					lpOperandTop->Real = -1;
				}
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_BSR:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger)
			{
				if (!lpOperandTop->IsQuad || !IsInteger)
				{
					if (!_BitScanReverse(&lpOperandTop->Low, lpOperandTop->Low))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				else
				{
					if (!_BitScanReverse64(&lpOperandTop->Low, lpOperandTop->Quad))
						lpOperandTop->Low = 0xFFFFFFFF;
				}
				lpOperandTop->High = 0;
				lpOperandTop->IsQuad = FALSE;
			}
			else
			{
				uint64_t x;
				int32_t  exp;

				if ((x = lpOperandTop->Quad & 0x7FFFFFFFFFFFFFFF) && (exp = (lpOperandTop->High & 0x7FF00000)) != 0x7FF00000)
				{
					if (exp >>= 20)
					{
						lpOperandTop->Real = exp - 1023;
					}
					else
					{
						unsigned long bits;

						_BitScanReverse64(&bits, x);
						lpOperandTop->Real = exp - (1023 + 52) + (int32_t)bits;
					}
				}
				else
				{
					lpOperandTop->Real = -1;
				}
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_ROTL8:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotl8((unsigned char)lpOperandTop->Low, (unsigned char)operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTL16:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotl16((unsigned short)lpOperandTop->Low, (unsigned char)operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTL32:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotl(lpOperandTop->Low, operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTL64:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotl64(lpOperandTop->Quad, operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTR8:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotr8((unsigned char)lpOperandTop->Low, (unsigned char)operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTR16:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotr16((unsigned short)lpOperandTop->Low, (unsigned char)operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTR32:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotr(lpOperandTop->Low, operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ROTR64:
			{
				VARIAUNT operand;

				operand = OPERAND_POP();
				lpOperandTop->Quad = _rotr64(lpOperandTop->Quad, operand.Low);
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_A2U:
			{
				#define lpBuffer ((LPSTR)lpBuffer1)

				LPCSTR lpMultiByteStr;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				if (lpMarkup->Param->Tag != TAG_NOT_OPERATOR || !lpMarkup->Param->Length || *lpMarkup->Param->String != '"')
					lpMultiByteStr = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				else
					lpMultiByteStr = lpMarkup->Param->String + 1;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpMultiByteStr, -1)) == -1)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpMultiByteStr, lpBuffer, length, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[length] = '\0';
					lpMultiByteStr = lpBuffer;
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, NULL, 0))
				{
					#define lpWideCharStr ((LPWSTR)lpBuffer2)

					int cbMultiByte;

					if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar + (size_t)cchWideChar)))
						goto ALLOC_ERROR_FREE1;
					MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
					if (cbMultiByte = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL))
					{
						size_t nAllocSize;
						LPSTR  lpUtf8Str;

						nAllocSize = cbMultiByte;
#ifdef _WIN64
						if (!IsInteger)
							nAllocSize += ULL2DBL_LOST_MAX;
#endif
						if (!(lpUtf8Str = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
							goto ALLOC_ERROR_FREE2;
#ifdef _WIN64
						*(size_t *)&lpUtf8Str = ((size_t)lpUtf8Str + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
						lpOperandTop->Quad = (size_t)lpUtf8Str;
						WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, cchWideChar, lpUtf8Str, cbMultiByte, NULL, NULL);
					}
					HeapFree(hHeap, 0, lpWideCharStr);

					#undef lpWideCharStr
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				break;

				#undef lpBuffer
			}
			break;
		case TAG_A2W:
			{
				#define lpBuffer ((LPSTR)lpBuffer1)

				LPCSTR lpMultiByteStr;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				if (lpMarkup->Param->Tag != TAG_NOT_OPERATOR || !lpMarkup->Param->Length || *lpMarkup->Param->String != '"')
					lpMultiByteStr = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				else
					lpMultiByteStr = lpMarkup->Param->String + 1;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpMultiByteStr, -1)) == -1)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpMultiByteStr, lpBuffer, length, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[length] = '\0';
					lpMultiByteStr = lpBuffer;
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, NULL, 0))
				{
					size_t nAllocSize;
					LPWSTR lpWideCharStr;

					nAllocSize = (size_t)cchWideChar + (size_t)cchWideChar;
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpWideCharStr = (LPWSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto ALLOC_ERROR_FREE1;
#ifdef _WIN64
					*(size_t *)&lpWideCharStr = ((size_t)lpWideCharStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpWideCharStr;
					MultiByteToWideChar(CP_THREAD_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, cchWideChar);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				break;

				#undef lpBuffer
			}
			break;
		case TAG_U2A:
			{
				#define lpBuffer ((LPSTR)lpBuffer1)

				LPCSTR lpUtf8Str;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				lpUtf8Str = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpUtf8Str, -1)) == -1)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpUtf8Str, lpBuffer, length, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[length] = '\0';
					lpUtf8Str = lpBuffer;
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, NULL, 0))
				{
					#define lpWideCharStr ((LPWSTR)lpBuffer2)

					int cbMultiByte;

					if (!(lpWideCharStr = (LPWSTR)HeapAlloc(hHeap, 0, (size_t)cchWideChar + (size_t)cchWideChar)))
						goto ALLOC_ERROR_FREE1;
					MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, lpWideCharStr, cchWideChar);
					if (cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL))
					{
						size_t nAllocSize;
						LPSTR  lpMultiByteStr;

						nAllocSize = cbMultiByte;
#ifdef _WIN64
						if (!IsInteger)
							nAllocSize += ULL2DBL_LOST_MAX;
#endif
						if (!(lpMultiByteStr = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
							goto ALLOC_ERROR_FREE2;
#ifdef _WIN64
						*(size_t *)&lpMultiByteStr = ((size_t)lpMultiByteStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
						lpOperandTop->Quad = (size_t)lpMultiByteStr;
						WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, NULL, NULL);
					}
					HeapFree(hHeap, 0, lpWideCharStr);

					#undef lpWideCharStr
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				break;

				#undef lpBuffer
			}
			break;
		case TAG_U2W:
			{
				#define lpBuffer ((LPSTR)lpBuffer1)

				LPCSTR lpUtf8Str;
				int    cchWideChar;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpUtf8Str = IsInteger ? (LPCSTR)lpOperandTop->Quad : (LPCSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthA(hProcess, lpAddress = (LPVOID)lpUtf8Str, -1)) == -1)
						goto READ_ERROR;
					lpBuffer = (LPSTR)HeapAlloc(hHeap, 0, length + 1);
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpUtf8Str, lpBuffer, length, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[length] = '\0';
					lpUtf8Str = lpBuffer;
				}
				lpOperandTop->Quad = 0;
				if (cchWideChar = MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, NULL, 0))
				{
					size_t nAllocSize;
					LPWSTR lpWideCharStr;

					nAllocSize = (size_t)cchWideChar + (size_t)cchWideChar;
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpWideCharStr = (LPWSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto ALLOC_ERROR_FREE1;
#ifdef _WIN64
					*(size_t *)&lpWideCharStr = ((size_t)lpWideCharStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpWideCharStr;
					MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, lpWideCharStr, cchWideChar);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				break;

				#undef lpBuffer
			}
			break;
		case TAG_W2A:
			{
				#define lpBuffer ((LPWSTR)lpBuffer1)

				LPCWSTR lpWideCharStr;
				int     cbMultiByte;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				lpWideCharStr = IsInteger ? (LPCWSTR)lpOperandTop->Quad : (LPCWSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthW(hProcess, lpAddress = (LPVOID)lpWideCharStr, -1)) == -1)
						goto READ_ERROR;
					lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, length + length + sizeof(wchar_t));
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpWideCharStr, lpBuffer, length + length, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[length] = L'\0';
					lpWideCharStr = lpBuffer;
				}
				lpOperandTop->Quad = 0;
				if (cbMultiByte = WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL))
				{
					size_t nAllocSize;
					LPSTR  lpMultiByteStr;

					nAllocSize = cbMultiByte;
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpMultiByteStr = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto ALLOC_ERROR_FREE1;
#ifdef _WIN64
					*(size_t *)&lpMultiByteStr = ((size_t)lpMultiByteStr + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpMultiByteStr;
					WideCharToMultiByte(CP_THREAD_ACP, 0, lpWideCharStr, -1, lpMultiByteStr, cbMultiByte, NULL, NULL);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				break;

				#undef lpBuffer
			}
			break;
		case TAG_W2U:
			{
				#define lpBuffer ((LPWSTR)lpBuffer1)

				LPCWSTR lpWideCharStr;
				int     cbMultiByte;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpBuffer = NULL;
				lpWideCharStr = IsInteger ? (LPCWSTR)lpOperandTop->Quad : (LPCWSTR)(size_t)lpOperandTop->Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t length;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((length = StringLengthW(hProcess, lpAddress = (LPVOID)lpWideCharStr, -1)) == -1)
						goto READ_ERROR;
					lpBuffer = (LPWSTR)HeapAlloc(hHeap, 0, length + length + sizeof(wchar_t));
					if (!lpBuffer)
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpWideCharStr, lpBuffer, length + length, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[length] = L'\0';
					lpWideCharStr = lpBuffer;
				}
				lpOperandTop->Quad = 0;
				if (cbMultiByte = WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL))
				{
					size_t nAllocSize;
					LPSTR  lpUtf8Str;

					nAllocSize = cbMultiByte;
#ifdef _WIN64
					if (!IsInteger)
						nAllocSize += ULL2DBL_LOST_MAX;
#endif
					if (!(lpUtf8Str = (LPSTR)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nAllocSize)))
						goto ALLOC_ERROR_FREE1;
#ifdef _WIN64
					*(size_t *)&lpUtf8Str = ((size_t)lpUtf8Str + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
					lpOperandTop->Quad = (size_t)lpUtf8Str;
					WideCharToMultiByte(CP_UTF8, 0, lpWideCharStr, -1, lpUtf8Str, cbMultiByte, NULL, NULL);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (!IsInteger)
#else
				if (lpOperandTop->IsQuad = !IsInteger)
#endif
					lpOperandTop->Real = (size_t)lpOperandTop->Quad;
				break;

				#undef lpBuffer
			}
			break;
		case TAG_ASSERT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger ? !lpOperandTop->Quad : !lpOperandTop->Real)
			{
				#define buffer1 ((LPSTR)lpBuffer1)
				#define buffer2 ((LPSTR)lpBuffer2)
				#define lpApplicationName (LPCSTR)0x006020C4

				char   *first, *last;
				size_t length, size;
				int    result;

				last = first = NULL;
				if (lpMarkup->Close && lpMarkup->Close - 1 >= lpMarkup + 2)
				{
					first = lpMarkup[2].String;
					last = lpMarkup->Close[-1].String + lpMarkup->Close[-1].Length;
				}
				size = string_length(&MainForm->selectScript.filePath) + (length = last - first) + 94;
				if (!(buffer1 = (char *)HeapAlloc(hHeap, 0, size)))
					goto ALLOC_ERROR;
				_snprintf(buffer1, size,
					"Debug Assertion Failed!\n"
					"\n"
					"SSG File: %s\n"
					"Expression: %.*s\n"
					"Code position: %u / %u bytes",
					string_c_str(&MainForm->selectScript.filePath),
					length, first,
					lpMarkup->String - lpszSrc, strlen(lpszSrc));
				TMainForm_Guide(buffer1, 0);
				size = (length = lpMarkup->String - lpszSrc) + 16;
				if (!(buffer2 = (char *)HeapAlloc(hHeap, 0, size)))
					goto ALLOC_ERROR_FREE1;
				_snprintf(buffer2, size, "Previous code: %.*s", length, lpszSrc);
				TMainForm_Guide(buffer2, 0);
				HeapFree(hHeap, 0, buffer2);
				result = MessageBoxA(TWinControl_GetHandle(MainForm), buffer1, lpApplicationName, MB_ABORTRETRYIGNORE | MB_ICONSTOP);
				HeapFree(hHeap, 0, buffer1);
				switch (result)
				{
				case IDABORT:
					ExitProcess(0);
					break;
				case IDRETRY:
					qwResult = InternalParsing(this, SSGS, Src, bInitialIsInteger, ArgPtr);
					goto RELEASE;
				}

				#undef buffer1
				#undef buffer2
				#undef lpApplicationName
			}
			break;
		case TAG_WAIT:
		case TAG_SLEEP:
			{
				DWORD dwMilliseconds;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				dwMilliseconds = IsInteger ? (DWORD)lpOperandTop->Quad : (DWORD)lpOperandTop->Real;
				if (lpMarkup->Tag == TAG_WAIT)
					Wait(dwMilliseconds);
				else// if (lpMarkup->Tag == TAG_SLEEP)
					Sleep(dwMilliseconds);
			}
			break;
#if ALLOCATE_SUPPORT
		case TAG_REALLOC:
			{
				MARKUP   *element;
				uint64_t id;
				uint64_t size;
				LPVOID   address;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				id = IsInteger ? (uint64_t)lpOperandTop[0].Quad : (uint64_t)(DWORD)(long)lpOperandTop[0].Real;
				if (id >> sizeof(lpProcessMemory[0].Id) * 8)
					goto PARSING_ERROR;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				size = IsInteger ? (uint64_t)lpOperandTop[1].Quad : (uint64_t)lpOperandTop[1].Real;
				lpOperandTop->Quad = 0;
				address = NULL;
				if ((sizeof(size_t) > sizeof(uint32_t) || !(size >> 32)) && size && !(id >> 32))
				{
					FILETIME creationTime = { 0, 0 };

					for (size_t i = 0; i < nNumberOfProcessMemory; i++)
					{
						size_t oldSize, newSize, allocSize;

						if (id != lpProcessMemory[i].Id)
							continue;
						address = lpProcessMemory[i].Address;
						oldSize = lpProcessMemory[i].Size;
						newSize = (size_t)size;
						if (lpProcessMemory[i].Protect)
						{
							oldSize = (oldSize + PAGE_SIZE - 1) & -PAGE_SIZE;
							newSize = (newSize + PAGE_SIZE - 1) & -PAGE_SIZE;
						}
						if (address && oldSize == newSize)
							break;
						if (lpProcessMemory[i].Protect)
						{
							#define IsReadableProtect(Protect) ( \
							    ((Protect) & (                   \
							        PAGE_READONLY          |     \
							        PAGE_READWRITE         |     \
							        PAGE_WRITECOPY         |     \
							        PAGE_EXECUTE_READ      |     \
							        PAGE_EXECUTE_READWRITE |     \
							        PAGE_EXECUTE_WRITECOPY)) &&  \
							    !((Protect) & (                  \
							        PAGE_NOACCESS          |     \
							        PAGE_GUARD)))

							#define IsWritableProtect(Protect) ( \
							    ((Protect) & (                   \
							        PAGE_READWRITE         |     \
							        PAGE_WRITECOPY         |     \
							        PAGE_EXECUTE_READWRITE |     \
							        PAGE_EXECUTE_WRITECOPY)) &&  \
							    !((Protect) & (                  \
							        PAGE_NOACCESS          |     \
							        PAGE_GUARD)))

							DWORD protect;

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto OPEN_ERROR;
							if (!creationTime.dwLowDateTime && !creationTime.dwHighDateTime)
							{
								FILETIME exitTime, kernelTime, userTime;

								if (!GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
									goto FAILED_GET_PROCESS_TIMES;
								if (ftProcessCreationTime.dwLowDateTime || ftProcessCreationTime.dwHighDateTime)
								{
									if (creationTime.dwLowDateTime  != ftProcessCreationTime.dwLowDateTime ||
										creationTime.dwHighDateTime != ftProcessCreationTime.dwHighDateTime)
									{
										goto FAILED_GET_PROCESS_TIMES;
									}
								}
								else
								{
									ftProcessCreationTime = creationTime;
								}
							}
							protect = lpProcessMemory[i].Protect;
							if (!IsWritableProtect(protect) && lpProcessMemory[i].Address)
								protect = PAGE_READWRITE;
							allocSize = newSize;
#ifdef _WIN64
							if (!IsInteger)
								allocSize += ULL2DBL_LOST_MAX;
#endif
							address = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, protect);
							if (!address)
								break;
							if (lpProcessMemory[i].Address)
							{
								NTSTATUS status;

								if (!IsReadableProtect(lpProcessMemory[i].Protect))
									VirtualProtectEx(hProcess, lpProcessMemory[i].Address, oldSize, PAGE_READONLY, &protect);
								status = MoveProcessMemory(hProcess, address, hProcess, lpProcessMemory[i].Address, min(oldSize, newSize));
								if (NT_SUCCESS(status))
								{
									if (!IsWritableProtect(lpProcessMemory[i].Protect))
										VirtualProtectEx(hProcess, address, newSize, lpProcessMemory[i].Protect, &protect);
									VirtualFreeEx(hProcess, lpProcessMemory[i].Address, 0, MEM_RELEASE);
								}
								else
								{
									if (!IsReadableProtect(lpProcessMemory[i].Protect))
										VirtualProtectEx(hProcess, lpProcessMemory[i].Address, oldSize, lpProcessMemory[i].Protect, &protect);
									VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
									address = NULL;
									break;
								}
							}

							#undef IsReadableProtect
							#undef IsWritableProtect
						}
						else
						{
							allocSize = (size_t)size;
#ifdef _WIN64
							if (!IsInteger)
								allocSize += ULL2DBL_LOST_MAX;
#endif
							address = lpProcessMemory[i].Address
								? HeapReAlloc(hPrivateHeap,
											  HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY | HEAP_REALLOC_IN_PLACE_ONLY * (allocSize <= lpProcessMemory[i].Size),
											  lpProcessMemory[i].Address,
											  allocSize)
								: HeapAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, allocSize);
							if (!address)
								goto ALLOC_ERROR;
						}
						lpProcessMemory[i].Address = address;
						lpProcessMemory[i].Size = (size_t)size;
						break;
					}
				}
				lpOperandTop->Quad = (uint64_t)address;
			}
#if INTPTR_MAX > INT32_MAX
			lpOperandTop->IsQuad = TRUE;
			if (!IsInteger)
#else
			if (lpOperandTop->IsQuad = !IsInteger)
#endif
			{
#ifdef _WIN64
				lpOperandTop->Quad = (lpOperandTop->Quad + ULL2DBL_LOST_MAX) & -(ULL2DBL_LOST_MAX + 1);
#endif
				lpOperandTop->Real = (size_t)lpOperandTop->Quad;
			}
			break;
#endif
		case TAG_ALLOCA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!(uintptr_t)(lpOperandTop->Quad = (uintptr_t)AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, (size_t)lpOperandTop->Quad)))
				goto ALLOC_ERROR;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ATOI:
			uFlags = 0;
			lpFunction = (FARPROC)_strtoui64;
			goto ATOI;
		case TAG_WTOI:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)_wcstoui64;
			goto ATOI;
		case TAG_ATOF:
			uFlags = 0;
			lpFunction = (FARPROC)strtod;
			goto ATOI;
		case TAG_WTOF:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)wcstod;
		ATOI:
			{
				#define buffer ((LPBYTE)lpBuffer1)

				MARKUP  *element;
				HANDLE  endptrProcess;
				BOOLEAN sign;
				LPBYTE  p;
				LPCBYTE nptr;
				LPBYTE  *endptr;
				int     base;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				buffer = NULL;
				nptr = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nLength = StringLength(hProcess, lpAddress = (LPVOID)nptr, -1, uFlags)) == -1)
						goto READ_ERROR;
					if (!(buffer = (LPBYTE)HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, nptr, buffer, SIZE_OF_STRING(nLength, uFlags), NULL))
						goto READ_ERROR_FREE1;
					TERMINATE_STRING(buffer, nLength, uFlags);
				}
				endptr = NULL;
				base = 0;
				if (element = element->Next)
				{
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR_FREE1;
					endptr = IsInteger ? (LPBYTE *)(uintptr_t)lpOperandTop[1].Quad : (LPBYTE *)(uintptr_t)lpOperandTop[1].Real;
					if (!endptr || element->Param->Tag == TAG_PARAM_LOCAL)
						endptrProcess = NULL;
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						endptrProcess = hProcess;
					else
						goto OPEN_ERROR_FREE1;
					switch (lpMarkup->Tag)
					{
					case TAG_ATOI:
					case TAG_WTOI:
						if (element = element->Next)
						{
							if (IsStringOperand(element->Param))
								goto PARSING_ERROR_FREE1;
							base = IsInteger ? (int)lpOperandTop[2].Quad : (int)lpOperandTop[2].Real;
						}
						break;
					case TAG_ATOF:
					case TAG_WTOF:
						break;
					default:
						__assume(0);
					}
				}
				switch (lpMarkup->Tag)
				{
				case TAG_ATOF:
				case TAG_WTOF:
					if (!IsInteger)
						break;
					/* FALLTHROUGH */
				case TAG_ATOI:
				case TAG_WTOI:
					p = buffer ? buffer : (LPBYTE)nptr;
					if (!(uFlags & UNICODE_FUNCTION))
					{
						unsigned char c;

						do
							c = *(p++);
						while (__intrinsic_isspace(c));
						sign = c == '-';
					}
					else
					{
						wchar_t c;

						do
							c = *(((wchar_t *)p)++);
						while (__intrinsic_iswspace(c));
						sign = c == L'-';
					}
					break;
				default:
					__assume(0);
				}
				do	/* do { ... } while (0); */
				{
					switch (lpMarkup->Tag)
					{
					case TAG_ATOI:
					case TAG_WTOI:
						lpOperandTop->Quad = ((unsigned __int64(__cdecl *)(const void *, void **, int))lpFunction)
							(buffer ? buffer : nptr, endptr ? &p : NULL, base);
						if (IsInteger)
							break;
						lpOperandTop->Real = !sign ? (double)lpOperandTop->Quad : (double)(int64_t)lpOperandTop->Quad;
						lpOperandTop->IsQuad = TRUE;
						continue;
					case TAG_ATOF:
					case TAG_WTOF:
						lpOperandTop->Real = ((double(__cdecl *)(const void *, void **))lpFunction)
							(buffer ? buffer : nptr, endptr ? &p : NULL);
						if (lpOperandTop->IsQuad = !IsInteger)
							continue;
						lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						break;
					default:
						__assume(0);
					}
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = !sign || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				} while (0);
				if (endptr)
				{
					if (buffer)
						p += nptr - buffer;
					if (IsInteger)
					{
						if (endptrProcess)
						{
							if (!WriteProcessMemory(endptrProcess, lpAddress = endptr, &p, sizeof(void *), NULL))
								goto WRITE_ERROR_FREE1;
						}
						else
						{
							if (IsBadWritePtr(lpAddress = endptr, sizeof(void *)))
								goto WRITE_ERROR_FREE1;
							*endptr = p;
						}
					}
					else
					{
						double dAddress;

						dAddress = (double)(uintptr_t)p;
						if (endptrProcess)
						{
							if (!WriteProcessMemory(endptrProcess, lpAddress = endptr, &dAddress, sizeof(dAddress), NULL))
								goto WRITE_ERROR_FREE1;
						}
						else
						{
							if (IsBadWritePtr(lpAddress = endptr, sizeof(dAddress)))
								goto WRITE_ERROR_FREE1;
							*(double *)endptr = dAddress;
						}
					}
				}
				if (buffer)
					HeapFree(hHeap, 0, buffer);
				break;

				#undef buffer
			}
			break;
		case TAG_TICK:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			lpOperandTop->Quad = GetTickCount();
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_RAND32:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!(lpOperandTop->IsQuad = !IsInteger))
				lpOperandTop->Quad = rand32();
			else
				lpOperandTop->Real = randf32();
			break;
		case TAG_RAND64:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger)
				lpOperandTop->Quad = rand64();
			else
				lpOperandTop->Real = randf64();
			lpOperandTop->IsQuad = TRUE;
			break;
		case TAG_MIN:
		case TAG_MAX:
			if (IsInteger)
			{
				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (lpMarkup->Tag == TAG_MIN)
					lpOperandTop->Quad = min(lpOperandTop[0].Quad, lpOperandTop[1].Quad);
				else// if (lpMarkup->Tag == TAG_MAX)
					lpOperandTop->Quad = max(lpOperandTop[0].Quad, lpOperandTop[1].Quad);
				lpOperandTop->IsQuad = lpOperandTop[0].IsQuad && lpOperandTop[1].IsQuad;
			}
			break;
		case TAG_IMIN:
		case TAG_IMAX:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (IsInteger)
			{
				if (!(lpOperandTop[0].IsQuad | lpOperandTop[1].IsQuad))
				{
					if (lpMarkup->Tag == TAG_IMIN)
						lpOperandTop->Low = min((int32_t)lpOperandTop[0].Low, (int32_t)lpOperandTop[1].Low);
					else// if (lpMarkup->Tag == TAG_IMAX)
						lpOperandTop->Low = max((int32_t)lpOperandTop[0].Low, (int32_t)lpOperandTop[1].Low);
				}
				else
				{
					int64_t a, b;

					a = lpOperandTop[0].IsQuad ? (int64_t)lpOperandTop[0].Quad : (int32_t)lpOperandTop[0].Low;
					b = lpOperandTop[1].IsQuad ? (int64_t)lpOperandTop[1].Quad : (int32_t)lpOperandTop[1].Low;
					if (lpMarkup->Tag == TAG_IMIN)
						lpOperandTop->Quad = min(a, b);
					else// if (lpMarkup->Tag == TAG_IMAX)
						lpOperandTop->Quad = max(a, b);
					if (lpOperandTop->IsQuad = lpOperandTop->High)
						if (!(lpOperandTop->IsQuad = (int64_t)lpOperandTop->Quad > INT32_MAX || (int64_t)lpOperandTop->Quad < INT32_MIN))
							lpOperandTop->High = 0;
				}
			}
			else
			{
				if (lpMarkup->Tag == TAG_IMIN)
					lpOperandTop->Real = min(lpOperandTop[0].Real, lpOperandTop[1].Real);
				else// if (lpMarkup->Tag == TAG_IMAX)
					lpOperandTop->Real = max(lpOperandTop[0].Real, lpOperandTop[1].Real);
				lpOperandTop->IsQuad = TRUE;
			}
			break;
		case TAG_PRINTF:
			uFlags = 0;
			lpFunction = (FARPROC)GuidePrintV;
			goto PRINTF;
		case TAG_DPRINTF:
			uFlags = 0;
			lpFunction = (FARPROC)DebugPrintV;
			goto PRINTF;
		case TAG_SNPRINTF:
			uFlags = NUMBER_OF_CHARS;
#ifdef _MSC_VER
			lpFunction = (FARPROC)internal_vsnprintf;
#else
			lpFunction = (FARPROC)_vsnprintf;
#endif
			lpStrlen = (size_t(__cdecl *)(const void *))strlen;
			goto PRINTF;
		case TAG_SNWPRINTF:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
#ifdef _MSC_VER
			lpFunction = (FARPROC)internal_vsnwprintf;
#else
			lpFunction = (FARPROC)_vsnwprintf;
#endif
			lpStrlen = (size_t(__cdecl *)(const void *))wcslen;
		PRINTF:
			{
				#define lpStack        ((uintptr_t *)lpBuffer1)
				#define lpFormatBuffer ((LPBYTE)lpBuffer2)
				#define lpDestBuffer   ((LPBYTE)lpBuffer3)

				MARKUP    *element;
				size_t    nStackSize;
				uintptr_t *lpParam;
				VARIAUNT  *lpOperand;
				int       iResult;
				HANDLE    hDestProcess;
				LPBYTE    lpDest;
				size_t    nCount;
				LPCBYTE   lpFormat;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					iResult = 0;
					if (lpMarkup->Tag == TAG_PRINTF && TMainForm_GetUserMode(MainForm) == 1)
						goto PRINTF_CONTINUE;
				}
				nStackSize = 0;
				element = lpMarkup;
				lpOperand = lpOperandTop;
				do
				{
#ifndef _WIN64
					if (IsStringOperand(element->Param) || !lpOperand->IsQuad)
						nStackSize += sizeof(uint32_t);
					else
#endif
						nStackSize += sizeof(uint64_t);
					lpOperand++;
				} while (element = element->Next);
				if (!nStackSize)
					goto PARSING_ERROR;
				if (!(lpStack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nStackSize)))
					goto ALLOC_ERROR;
				lpFormatBuffer = NULL;
				lpDestBuffer = NULL;
				lpParam = lpStack;
				element = lpMarkup;
				lpOperand = lpOperandTop;
				do	/* do { ... } while (0); */
				{
					if (uFlags & NUMBER_OF_CHARS)
					{
						if (IsStringOperand(lpMarkup->Param))
							goto PARSING_ERROR_FREE1;
						if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
							hDestProcess = NULL;
						else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							hDestProcess = hProcess;
						else
							goto OPEN_ERROR_FREE1;
						lpDest = IsInteger ? (LPBYTE)(uintptr_t)lpOperand->Quad : (LPBYTE)(uintptr_t)lpOperand->Real;
						lpOperand++;
						if (!(element = element->Next))
							continue;
						if (IsStringOperand(element->Param))
							goto PARSING_ERROR_FREE1;
						nCount = IsInteger ? (size_t)lpOperand->Quad : (size_t)lpOperand->Real;
						lpOperand++;
						if (!(element = element->Next))
							continue;
					}
					lpFormat = IsInteger ? (LPCBYTE)(uintptr_t)lpOperand->Quad : (LPCBYTE)(uintptr_t)lpOperand->Real;
					lpOperand++;
					if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
					{
						size_t nLength;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR_FREE1;
						if ((nLength = StringLength(hProcess, lpAddress = (LPVOID)lpFormat, -1, uFlags)) == -1)
							goto READ_ERROR_FREE1;
						if (!(lpFormatBuffer = (LPBYTE)HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
							goto ALLOC_ERROR_FREE1;
						if (!ReadProcessMemory(hProcess, lpFormat, lpFormatBuffer, SIZE_OF_STRING(nLength, uFlags), NULL))
							goto READ_ERROR_FREE2;
						TERMINATE_STRING(lpFormatBuffer, nLength, uFlags);
						lpFormat = lpFormatBuffer;
					}
					while (element = element->Next)
					{
						if (!IsStringOperand(element->Param))
						{
#ifndef _WIN64
							if (!lpOperand->IsQuad)
								*(lpParam++) = lpOperand->Low;
							else
#endif
								*(((uint64_t *)lpParam)++) = lpOperand->Quad;
						}
						else
						{
							*(lpParam++) = IsInteger ? (uintptr_t)lpOperand->Quad : (uintptr_t)lpOperand->Real;
						}
						lpOperand++;
					}
				} while (0);
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					iResult = ((int(__fastcall *)(const char *, va_list, const va_list))lpFunction)
						(lpFormat, (va_list)lpStack, (va_list)lpParam);
				}
				else
				{
					if (hDestProcess && nCount && !(lpDestBuffer = (LPBYTE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, SIZE_OF_STRING(nCount, uFlags))))
						goto ALLOC_ERROR_FREE2;
#ifdef _MSC_VER
					iResult = ((int(__fastcall *)(void *, size_t, const void *, va_list, const va_list))lpFunction)
						(lpDestBuffer ? lpDestBuffer : lpDest, nCount, lpFormat, (va_list)lpStack, (va_list)lpParam);
#else
					iResult = ((int(__cdecl *)(void *, size_t, const void *, va_list))lpFunction)
						(lpDestBuffer ? lpDestBuffer : lpDest, nCount, lpFormat, (va_list)lpStack);
#endif
					if (lpDestBuffer)
					{
						if (nCount && !WriteProcessMemory(hDestProcess, lpAddress = lpDest, lpDestBuffer, SIZE_OF_STRING(lpStrlen(lpDestBuffer) + 1, uFlags), NULL))
							goto WRITE_ERROR_FREE3;
						HeapFree(hHeap, 0, lpDestBuffer);
					}
				}
				if (lpFormatBuffer)
					HeapFree(hHeap, 0, lpFormatBuffer);
				HeapFree(hHeap, 0, lpStack);

			PRINTF_CONTINUE:
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (unsigned int)iResult;
				else
					lpOperandTop->Real = iResult;
				if (!(uFlags & NUMBER_OF_CHARS))
					continue;

				#undef lpStack
				#undef lpFormatBuffer
				#undef lpDestBuffer
			}
			break;
		case TAG_STRDUP:
			uFlags = NULL_TERMINATED;
			goto STRDUP;
		case TAG_WCSDUP:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED;
			goto STRDUP;
		case TAG_MEMDUP:
			uFlags = NUMBER_OF_CHARS;
			goto STRDUP;
		case TAG_WMEMDUP:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
		STRDUP:
			{
				NTSTATUS Status;
				LPVOID   lpDest;
				HANDLE   hSrcProcess;
				LPCBYTE  lpSrc;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpSrc = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop->Quad : (LPCBYTE)(uintptr_t)lpOperandTop->Real;
				if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hSrcProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hSrcProcess = hProcess;
				else
					goto OPEN_ERROR;
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					if ((nCount = StringLength(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1, uFlags)) == -1)
						goto READ_ERROR;
					if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, SIZE_OF_STRING(nCount + 1, uFlags))))
						goto STRDUP_ALLOC_FAILED;
					Status = CopyProcessMemory(NULL, lpDest, hSrcProcess, lpSrc, SIZE_OF_STRING(nCount, uFlags));
					if (NT_SUCCESS(Status))
						TERMINATE_STRING(lpDest, nCount, uFlags);
				}
				else
				{
					if (IsStringOperand(lpMarkup->Next))
						goto PARSING_ERROR;
					nCount = IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real;
					if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, nCount = SIZE_OF_STRING(nCount, uFlags))))
						goto STRDUP_ALLOC_FAILED;
					Status = CopyProcessMemory(NULL, lpDest, hSrcProcess, lpSrc, nCount);
				}
				if (NT_SUCCESS(Status))
				{
#if INTPTR_MAX > INT32_MAX
					lpOperandTop->IsQuad = TRUE;
					if (IsInteger)
#else
					if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
						lpOperandTop->Quad = (uintptr_t)lpDest;
					else
						lpOperandTop->Real = (uintptr_t)lpDest;
				}
				else
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
						lpAddress = lpDest;
						goto WRITE_ERROR;
					}
					goto PARSING_ERROR;
				}
				break;

			STRDUP_ALLOC_FAILED:
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = sizeof(void *) > sizeof(uint32_t);
			}
			break;
		case TAG_STRINC:
		case TAG_WCSINC:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			assert(lpMarkup->Tag == TAG_STRINC || lpMarkup->Tag == TAG_WCSINC);
			__assume(lpMarkup->Tag == TAG_STRINC || lpMarkup->Tag == TAG_WCSINC);
#if INTPTR_MAX > INT32_MAX
			lpOperandTop->IsQuad = TRUE;
			if (IsInteger)
#else
			if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
				lpOperandTop->Quad = ((uintptr_t)lpOperandTop->Quad + (lpMarkup->Tag == TAG_STRINC ? 1 : 2));
			else
				lpOperandTop->Real = ((uintptr_t)lpOperandTop->Real + (lpMarkup->Tag == TAG_STRINC ? 1 : 2));
			break;
		case TAG_MBSINC:
			{
				#define lpCurrent ((LPCBYTE)lpAddress)

				HANDLE hTargetProcess;
				BYTE   c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpCurrent = IsInteger ? (LPBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				if (hTargetProcess)
				{
					if (!ReadProcessMemory(hTargetProcess, lpCurrent, &c, sizeof(c), NULL))
						goto READ_ERROR;
				}
				else
				{
					if (IsBadReadPtr(lpCurrent, sizeof(c)))
						goto READ_ERROR;
					c = *lpCurrent;
				}
				lpCurrent++;
				if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				{
					if (hTargetProcess)
					{
						if (!ReadProcessMemory(hTargetProcess, lpCurrent, &c, sizeof(c), NULL))
							goto READ_ERROR;
					}
					else
					{
						if (IsBadReadPtr(lpCurrent, sizeof(c)))
							goto READ_ERROR;
						c = *lpCurrent;
					}
					if (c)
						lpCurrent++;
				}
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (IsInteger)
#else
				if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
					lpOperandTop->Quad = (uintptr_t)lpCurrent;
				else
					lpOperandTop->Real = (uintptr_t)lpCurrent;

				#undef lpCurrent
			}
			break;
		case TAG_STRDEC:
		case TAG_WCSDEC:
		case TAG_MBSDEC:
			{
				#define lpResult ((LPBYTE)lpAddress)

				LPCBYTE lpStart, lpCurrent;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpStart = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				lpCurrent = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[1].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[1].Real;
				if (lpStart >= lpCurrent)
				{
					lpResult = NULL;
				}
				else
				{
					switch (lpMarkup->Tag)
					{
					case TAG_STRDEC:
						lpResult = (LPBYTE)lpCurrent - 1;
						break;
					case TAG_WCSDEC:
						lpResult = (LPBYTE)lpStart + ((lpCurrent - lpStart - 1) & -2);
						break;
					case TAG_MBSDEC:
						if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
						{
							if (!IsBadReadPtr(lpAddress = (LPVOID)lpStart, lpCurrent - lpStart))
								goto READ_ERROR;
							lpResult = _mbsdec(lpStart, lpCurrent);
						}
						else
						{
							BYTE c;

							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto OPEN_ERROR;
							do
								if (!ReadProcessMemory(hProcess, lpResult = (LPBYTE)lpStart, &c, sizeof(c), NULL))
									goto READ_ERROR;
							while ((lpStart += IsDBCSLeadByteEx(CP_THREAD_ACP, c) ? 2 : 1) < lpCurrent);
						}
						break;
					default:
						__assume(0);
					}
				}
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (IsInteger)
#else
				if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
					lpOperandTop->Quad = (uintptr_t)lpResult;
				else
					lpOperandTop->Real = (uintptr_t)lpResult;

				#undef lpResult
			}
			break;
		case TAG_STRNINC:
		case TAG_WCSNINC:
		case TAG_MBSNINC:
			{
				LPCBYTE lpString;
				size_t  nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Next->Param))
					goto PARSING_ERROR;
				lpString = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				nCount = IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real;
				switch (lpMarkup->Tag)
				{
				case TAG_STRNINC:
					lpString += nCount;
					break;
				case TAG_WCSNINC:
					((LPCWSTR)lpString) += nCount;
					break;
				case TAG_MBSNINC:
					if (lpString && nCount)
					{
						BYTE c;

						if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
						{
							#define lpEndOfRegion ((LPCBYTE)lpAddress)

							lpEndOfRegion = (LPCBYTE)GetEndOfReadableRegion(lpString);
							do
								if (lpString >= lpEndOfRegion)
									goto READ_ERROR_FREE1;
								else if (!(c = lpString[0]))
									break;
								else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
									lpString++;
								else if (lpString + 1 >= lpEndOfRegion)
									goto READ_ERROR_FREE1;
								else if (!lpString[1])
									break;
								else
									lpString += 2;
							while (--nCount);

							#undef lpEndOfRegion
						}
						else
						{
							if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
								goto OPEN_ERROR;
							do
								if (!ReadProcessMemory(hProcess, lpAddress = (LPVOID)lpString, &c, 1, NULL))
									goto READ_ERROR_FREE1;
								else if (!c)
									break;
								else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
									lpString++;
								else if (!ReadProcessMemory(hProcess, lpAddress = (LPVOID)(lpString + 1), &c, 1, NULL))
									goto READ_ERROR_FREE1;
								else if (!c)
									break;
								else
									lpString += 2;
							while (--nCount);
						}
					}
					break;
				default:
					__assume(0);
				}
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (IsInteger)
#else
				if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
					lpOperandTop->Quad = (uintptr_t)lpString;
				else
					lpOperandTop->Real = (uintptr_t)lpString;
			}
			break;
		case TAG_STRNEXTC:
		case TAG_MBSNEXTC:
			uFlags = 0;
			goto STRNEXTC;
		case TAG_WCSNEXTC:
			uFlags = UNICODE_FUNCTION;
		STRNEXTC:
			{
				#define lpString ((LPCBYTE)lpAddress)

				HANDLE       hTargetProcess;
				unsigned int uResult;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpString = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(lpMarkup->Param) || lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				if (hTargetProcess)
				{
					uResult = 0;
					if (!ReadProcessMemory(hTargetProcess, lpString, &uResult, SIZE_OF_CHAR(uFlags), NULL))
						goto READ_ERROR;
				}
				else
				{
					if (IsBadReadPtr(lpString, SIZE_OF_CHAR(uFlags)))
						goto READ_ERROR;
					if (!(uFlags & UNICODE_FUNCTION))
						uResult = *lpString;
					else
						uResult = *(LPCWSTR)lpString;
				}
				if (lpMarkup->Tag == TAG_MBSNEXTC && IsDBCSLeadByteEx(CP_THREAD_ACP, (BYTE)uResult))
				{
					BYTE c;

					lpString++;
					if (hTargetProcess)
					{
						if (!ReadProcessMemory(hTargetProcess, lpString, &c, 1, NULL))
							goto READ_ERROR;
					}
					else
					{
						if (IsBadReadPtr(lpString, 1))
							goto READ_ERROR;
						c = *lpString;
					}
					if (c)
						uResult = (uResult << 8) | c;
				}
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (IsInteger)
#else
				if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
					lpOperandTop->Quad = uResult;
				else
					lpOperandTop->Real = uResult;

				#undef lpString
			}
			break;
		case TAG_STRLEN:
			uFlags = 0;
			lpFunction = (FARPROC)strlen;
			goto STRLEN;
		case TAG_WCSLEN:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)wcslen;
			goto STRLEN;
		case TAG_STRNLEN:
			uFlags = NUMBER_OF_CHARS;
			lpFunction = (FARPROC)strnlen;
			goto STRLEN;
		case TAG_WCSNLEN:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
			lpFunction = (FARPROC)wcsnlen;
		STRLEN:
			{
				size_t nLength;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[0].Quad : (LPVOID)(uintptr_t)lpOperandTop[0].Real;
				if (lpMarkup->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nMaxLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (!(uFlags & NUMBER_OF_CHARS))
					{
						nMaxLength = -1;
					}
					else
					{
						if (IsStringOperand(lpMarkup->Next->Param))
							goto PARSING_ERROR;
						nMaxLength = IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real;
					}
					if ((nLength = StringLength(hProcess, lpAddress, nMaxLength, uFlags)) == -1)
						goto READ_ERROR;
				}
				else
				{
					if (!(uFlags & NUMBER_OF_CHARS))
					{
						nLength = ((size_t(__cdecl *)(const void *))lpFunction)(lpAddress);
					}
					else
					{
						size_t nMaxLength;

						if (IsStringOperand(lpMarkup->Next->Param))
							goto PARSING_ERROR;
						nMaxLength = IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real;
						nLength = ((size_t(__cdecl *)(const void *, size_t))lpFunction)(lpAddress, nMaxLength);
					}
				}
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = nLength;
				else
					lpOperandTop->Real = nLength;
			}
			break;
		case TAG_MBSLEN:
			lpFunction = NULL;
			goto MBSLEN;
		case TAG_MBSNLEN:
			lpFunction = (FARPROC)_mbsnlen;
			goto MBSLEN;
		case TAG_MBSNBCNT:
			lpFunction = (FARPROC)_mbsnbcnt;
			goto MBSLEN;
		case TAG_MBSNCCNT:
			lpFunction = (FARPROC)_mbsnccnt;
		MBSLEN:
			{
				#define lpBuffer ((LPBYTE)lpBuffer1)

				MARKUP  *element;
				LPCBYTE lpString;
				size_t  nResult;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == -1)
						goto READ_ERROR;
					if (!(lpBuffer = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpString, lpBuffer, nLength, NULL))
						goto READ_ERROR_FREE1;
					((LPBYTE)lpString = lpBuffer)[nLength] = '\0';
				}
				if (!lpFunction)
				{
					nResult = _mbslen(lpString);
				}
				else
				{
					size_t nNumber;

					element = element->Next;
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR_FREE1;
					nNumber = IsInteger ? (size_t)lpOperandTop[1].Quad : (size_t)lpOperandTop[1].Real;
					nResult = ((size_t(__cdecl *)(const unsigned char *, size_t))lpFunction)(lpString, nNumber);
				}
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = nResult;
				else
					lpOperandTop->Real = nResult;

				#undef lpBuffer
			}
			break;
		case TAG_STRCMP:
			uFlags = NULL_TERMINATED;
			goto STRCMP;
		case TAG_WCSCMP:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED;
			goto STRCMP;
		case TAG_STRICMP:
			uFlags = INSENSITIVE_CASE | NULL_TERMINATED;
			goto STRCMP;
		case TAG_WCSICMP:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NULL_TERMINATED;
			goto STRCMP;
		case TAG_STRNCMP:
			uFlags = NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_WCSNCMP:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_STRNICMP:
			uFlags = INSENSITIVE_CASE | NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_WCSNICMP:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_MEMCMP:
			uFlags = NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_WMEMCMP:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_MEMICMP:
			uFlags = INSENSITIVE_CASE | NUMBER_OF_CHARS;
			goto STRCMP;
		case TAG_WMEMICMP:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NUMBER_OF_CHARS;
		STRCMP:
			{
				MARKUP   *element;
				NTSTATUS Status;
				int      iResult;
				HANDLE   hProcess1;
				LPCVOID  lpAddress1;
				HANDLE   hProcess2;
				LPCVOID  lpAddress2;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				hProcess1 = (HANDLE)(!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL);
				lpAddress1 = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[0].Quad : (LPCVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				hProcess2 = (HANDLE)(!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL);
				lpAddress2 = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				if (hProcess1 || hProcess2)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hProcess1)
						hProcess1 = hProcess;
					if (hProcess2)
						hProcess2 = hProcess;
				}
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					nCount = -1;
				}
				else
				{
					element = element->Next;
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR;
					nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				}
				if (nCount && (uFlags & NULL_TERMINATED))
				{
					nCount--;
					if ((nCount = StringLength(hProcess1, lpAddress = (LPVOID)lpAddress1, nCount, uFlags)) == -1)
						goto READ_ERROR;
					if ((nCount = StringLength(hProcess2, lpAddress = (LPVOID)lpAddress2, nCount, uFlags)) == -1)
						goto READ_ERROR;
					nCount++;
				}
				Status = CompareProcessMemory(&iResult, hProcess1, lpAddress1, hProcess2, lpAddress2, nCount, uFlags);
				if (NT_SUCCESS(Status))
				{
					if (!(lpOperandTop->IsQuad = !IsInteger))
						lpOperandTop->Quad = (unsigned int)iResult;
					else
						lpOperandTop->Real = iResult;
				}
				else
				{
					if (Status == STATUS_MEMORY_READ1_FAILED)
					{
						lpAddress = (LPVOID)lpAddress1;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_READ2_FAILED)
					{
						lpAddress = (LPVOID)lpAddress2;
						goto READ_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_MBSICMP:
			lpFunction = NULL;
			goto MBSICMP;
		case TAG_MBSNCMP:
			lpFunction = (FARPROC)_mbsncmp;
			goto MBSICMP;
		case TAG_MBSNBICMP:
			lpFunction = (FARPROC)_mbsnbicmp;
			goto MBSICMP;
		case TAG_MBSNICMP:
			lpFunction = (FARPROC)_mbsnicmp;
		MBSICMP:
			{
				#define lpBuffer1 ((LPBYTE)lpBuffer1)
				#define lpBuffer2 ((LPBYTE)lpBuffer2)

				MARKUP  *element;
				int     iResult;
				LPCBYTE lpString1;
				LPCBYTE lpString2;
				size_t  nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == -1)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpString1, lpBuffer1, nLength, NULL))
						goto READ_ERROR_FREE1;
					((LPBYTE)lpString1 = lpBuffer1)[nLength] = '\0';
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[1].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR_FREE1;
					if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == -1)
						goto READ_ERROR_FREE1;
					if (!(lpBuffer2 = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR_FREE1;
					if (!ReadProcessMemory(hProcess, lpString2, lpBuffer2, nLength, NULL))
						goto READ_ERROR_FREE2;
					((LPBYTE)lpString2 = lpBuffer2)[nLength] = '\0';
				}
				if (!lpFunction)
				{
					iResult = _mbsicmp(lpString1, lpString2);
				}
				else
				{
					element = element->Next;
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR_FREE2;
					nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
					iResult = ((int(__cdecl *)(const unsigned char *, const unsigned char *, size_t))lpFunction)(lpString1, lpString2, nCount);
				}
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (unsigned int)iResult;
				else
					lpOperandTop->Real = iResult;

				#undef lpBuffer1
				#undef lpBuffer2
			}
			break;
		case TAG_STRCPY:
			uFlags = NULL_TERMINATED;
			goto STRCPY;
		case TAG_WCSCPY:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED;
			goto STRCPY;
		case TAG_STPCPY:
			uFlags = NULL_TERMINATED | EXTENDED_POINTER;
			goto STRCPY;
		case TAG_WCPCPY:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | EXTENDED_POINTER;
			goto STRCPY;
		case TAG_STRNCPY:
			uFlags = NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRCPY;
		case TAG_WCSNCPY:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRCPY;
		case TAG_STPNCPY:
			uFlags = NULL_TERMINATED | NUMBER_OF_CHARS | EXTENDED_POINTER;
			goto STRCPY;
		case TAG_WCPNCPY:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | NUMBER_OF_CHARS | EXTENDED_POINTER;
			goto STRCPY;
		case TAG_STRCAT:
			uFlags = NULL_TERMINATED | CONCATENATE;
			goto STRCPY;
		case TAG_WCSCAT:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | CONCATENATE;
			goto STRCPY;
		case TAG_STRNCAT:
			uFlags = NULL_TERMINATED | NUMBER_OF_CHARS | CONCATENATE;
			goto STRCPY;
		case TAG_WCSNCAT:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | NUMBER_OF_CHARS | CONCATENATE;
			goto STRCPY;
		case TAG_MEMCPY:
			uFlags = NUMBER_OF_CHARS;
			goto STRCPY;
		case TAG_WMEMCPY:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
			goto STRCPY;
		case TAG_MEMPCPY:
			uFlags = NUMBER_OF_CHARS | EXTENDED_POINTER;
			goto STRCPY;
		case TAG_WMEMPCPY:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS | EXTENDED_POINTER;
			goto STRCPY;
		case TAG_MEMMOVE:
			uFlags = NUMBER_OF_CHARS | HANDLE_OVERLAP_REGIONS;
			goto STRCPY;
		case TAG_WMEMMOVE:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS | HANDLE_OVERLAP_REGIONS;
		STRCPY:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPVOID   lpDest;
				size_t   nDestLength;
				HANDLE   hSrcProcess;
				LPCVOID  lpSrc;
				size_t   nSrcLength;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				hDestProcess = (HANDLE)(element->Param->Tag != TAG_PARAM_LOCAL);
				lpDest = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[0].Quad : (LPVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				hSrcProcess = (HANDLE)(!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL);
				lpSrc = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					if (!(uFlags & CONCATENATE))
					{
						nDestLength = 0;
					}
					else
					{
						if ((nDestLength = StringLength(hDestProcess, lpAddress = lpDest, -1, uFlags)) == -1)
							goto READ_ERROR;
						(LPBYTE)lpDest += SIZE_OF_STRING(nDestLength, uFlags);
					}
					if ((nSrcLength = StringLength(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1, uFlags)) == -1)
						goto READ_ERROR;
					if (element = element->Next)
					{
						size_t nLength;
						HANDLE hTargetProcess;

						nLength = nDestLength + nSrcLength;
						lpAddress = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[2].Quad : (LPVOID)(uintptr_t)lpOperandTop[2].Real;
						if (element->Param->Tag == TAG_PARAM_LOCAL)
							hTargetProcess = GetCurrentProcess();
						else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							hTargetProcess = hProcess;
						else
							goto OPEN_ERROR;
						if (IsInteger)
						{
							if (!WriteProcessMemory(hTargetProcess, lpAddress, &nLength, sizeof(nLength), NULL))
								goto WRITE_ERROR;
						}
						else
						{
							double dLength;

							dLength = (double)nLength;
							if (!WriteProcessMemory(hTargetProcess, lpAddress, &dLength, sizeof(dLength), NULL))
								goto WRITE_ERROR;
						}
					}
					if (!(uFlags & CONCATENATE) && !hDestProcess && !lpDest)
					{
						if (!(lpDest = AllocateHeapBuffer(&lpHeapBuffer, &nNumberOfHeapBuffer, SIZE_OF_STRING(nSrcLength + 1, uFlags))))
							goto ALLOC_ERROR;
#if INTPTR_MAX > INT32_MAX
						lpOperandTop->IsQuad = TRUE;
						if (IsInteger)
#else
						if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
							lpOperandTop->Quad = (size_t)lpDest;
						else
							lpOperandTop->Real = (size_t)lpDest;
					}
					Status = CopyProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, SIZE_OF_STRING(nSrcLength + 1, uFlags));
				}
				else
				{
					element = element->Next;
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR;
					nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
					Status = STATUS_SUCCESS;
					if (nSrcLength = nCount)
					{
						if (uFlags & CONCATENATE)
						{
							if ((nDestLength = StringLength(hDestProcess, lpAddress = lpDest, -1, uFlags)) == -1)
								goto READ_ERROR;
							(LPBYTE)lpDest += SIZE_OF_STRING(nDestLength, uFlags);
						}
						if (uFlags & NULL_TERMINATED)
							if ((nSrcLength = StringLength(hSrcProcess, lpAddress = (LPVOID)lpSrc, nCount, uFlags)) == -1)
								goto READ_ERROR;
						Status = InternalMoveProcessMemory(uFlags & HANDLE_OVERLAP_REGIONS, hDestProcess, lpDest, hSrcProcess, lpSrc, SIZE_OF_STRING(nSrcLength, uFlags));
						if (NT_SUCCESS(Status) && (uFlags & NULL_TERMINATED))
						{
							if (!(uFlags & CONCATENATE))
							{
								if (nCount -= nSrcLength)
								{
									if (!(uFlags & UNICODE_FUNCTION))
									{
										if (!FillProcessMemory(hDestProcess, (LPSTR)lpDest + nSrcLength, nCount, 0))
											Status = STATUS_MEMORY_WRITE_FAILED;
									}
									else
									{
										if (!FillProcessMemory16(hDestProcess, (LPWSTR)lpDest + nSrcLength, nCount, 0))
											Status = STATUS_MEMORY_WRITE_FAILED;
									}
								}
							}
							else
							{
								LPVOID lpEndOfString;

								lpEndOfString = (LPBYTE)lpDest + SIZE_OF_STRING(nSrcLength, uFlags);
								if (hDestProcess)
								{
									if (!WriteProcessMemory(hDestProcess, lpEndOfString, &Terminator, SIZE_OF_CHAR(uFlags), NULL))
										Status = STATUS_MEMORY_WRITE_FAILED;
								}
								else
								{
									if (IsBadWritePtr(lpEndOfString, SIZE_OF_CHAR(uFlags)))
										Status = STATUS_MEMORY_WRITE_FAILED;
									TERMINATE_STRING(lpEndOfString, 0, uFlags);
								}
							}
						}
					}
				}
				if (NT_SUCCESS(Status))
				{
					if (uFlags & EXTENDED_POINTER)
						if (IsInteger)
							lpOperandTop->Quad = (size_t)lpDest + SIZE_OF_STRING(nSrcLength, uFlags);
						else
							lpOperandTop->Real = (size_t)lpDest + SIZE_OF_STRING(nSrcLength, uFlags);
				}
				else
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
						lpAddress = lpDest;
						goto WRITE_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_MEMCCPY:
			uFlags = 0;
			goto MEMCCPY;
		case TAG_WMEMCCPY:
			uFlags = UNICODE_FUNCTION;
		MEMCCPY:
			{
				MARKUP       *element;
				LPVOID       lpResult;
				NTSTATUS     Status;
				HANDLE       hDestProcess;
				LPVOID       lpDest;
				HANDLE       hSrcProcess;
				LPCVOID      lpSrc;
				unsigned int c;
				size_t       nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				hDestProcess = (HANDLE)(element->Param->Tag != TAG_PARAM_LOCAL);
				lpDest = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[0].Quad : (LPVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				hSrcProcess = (HANDLE)(!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL);
				lpSrc = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				c = IsInteger ? lpOperandTop[2].Low : (unsigned int)lpOperandTop[2].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[3].Quad : (size_t)lpOperandTop[3].Real;
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				lpResult = NULL;
				if (nCount)
				{
					size_t nPos, nSize;

					if ((nPos = FindProcessMemory(hProcess, lpAddress = (LPVOID)lpSrc, c, nCount, uFlags)) == -1)
						goto READ_ERROR;
					nSize =
						(lpResult = nPos < nCount ? (LPBYTE)lpDest + SIZE_OF_STRING(nPos + 1, uFlags) : NULL) ?
							(LPBYTE)lpResult - (LPBYTE)lpDest :
							SIZE_OF_STRING(nCount, uFlags);
					Status = CopyProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, nSize);
					if (!NT_SUCCESS(Status))
					{
						if (Status == STATUS_MEMORY_READ_FAILED)
						{
							lpAddress = (LPVOID)lpSrc;
							goto READ_ERROR;
						}
						if (Status == STATUS_MEMORY_WRITE_FAILED)
						{
							lpAddress = lpDest;
							goto WRITE_ERROR;
						}
						goto PARSING_ERROR;
					}
				}
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (size_t)lpResult;
				else
					lpOperandTop->Real = (size_t)lpResult;
			}
			break;
		case TAG_MBSNBCPY:
			uFlags = 0;
			goto MBSNBCPY;
		case TAG_MBSNCPY:
			uFlags = NUMBER_OF_CHARS;
			goto MBSNBCPY;
		case TAG_MBSNBCAT:
			uFlags = CONCATENATE;
			goto MBSNBCPY;
		case TAG_MBSNCAT:
			uFlags = NUMBER_OF_CHARS | CONCATENATE;
			goto MBSNBCPY;
		MBSNBCPY:
			{
				#define lpSrcBuffer ((LPBYTE)lpBuffer1)

				MARKUP  *element;
				HANDLE  hDestProcess;
				LPBYTE  lpDest;
				HANDLE  hSrcProcess;
				LPCBYTE lpSrc;
				size_t  nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpSrcBuffer = NULL;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				hDestProcess = (HANDLE)(element->Param->Tag != TAG_PARAM_LOCAL);
				lpDest = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[0].Quad : (LPVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				hSrcProcess = (HANDLE)(!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL);
				lpSrc = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if (!nCount)
					break;
				if (hSrcProcess)
				{
					size_t nLength;

					if ((nLength = StringLengthA(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1)) == -1)
						goto READ_ERROR;
					if (!(lpSrcBuffer = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hSrcProcess, lpSrc, lpSrcBuffer, nLength, NULL))
						goto READ_ERROR_FREE1;
					((LPBYTE)lpSrc = lpSrcBuffer)[nLength] = '\0';
				}
				if (hDestProcess)
				{
					unsigned char c, lead;

					if (uFlags & CONCATENATE)
						for (; ; )
							if (!ReadProcessMemory(hDestProcess, lpAddress = lpDest, &c, 1, NULL))
								goto READ_ERROR_FREE1;
							else if (!c)
								break;
							else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
								lpDest++;
							else if (!ReadProcessMemory(hDestProcess, lpAddress = lpDest + 1, &c, 1, NULL))
								goto READ_ERROR_FREE1;
							else if (!c)
								break;
							else
								lpDest += 2;
					do
					{
						if (!(c = *(lpSrc++)))
							break;
						if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
						{
							if (!(uFlags & NUMBER_OF_CHARS) && !--nCount)
								break;
							lead = c;
							if (!(c = *lpSrc))
								break;
							if (!WriteProcessMemory(hDestProcess, lpAddress = lpDest++, &lead, 1, NULL))
								goto WRITE_ERROR_FREE1;
						}
						if (!WriteProcessMemory(hDestProcess, lpAddress = lpDest++, &c, 1, NULL))
							goto WRITE_ERROR_FREE1;
					} while (--nCount);
					if (!(uFlags & CONCATENATE))
					{
						if (nCount && !FillProcessMemory(hDestProcess, lpAddress = lpDest, nCount, '\0'))
							goto WRITE_ERROR_FREE1;
					}
					else
					{
						if (!WriteProcessMemory(hDestProcess, lpAddress = lpDest, &Terminator, 1, NULL))
							goto WRITE_ERROR_FREE1;
					}
				}
				else
				{
					#define lpEndOfRegion ((LPBYTE)lpAddress)

					unsigned char c, lead;

					if (uFlags & CONCATENATE)
					{
						lpEndOfRegion = (LPBYTE)GetEndOfReadableRegion(lpDest);
						for (; ; )
							if (lpDest >= lpEndOfRegion)
								goto READ_ERROR_FREE1;
							else if (!(c = *lpDest))
								break;
							else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
								lpDest++;
							else if (lpDest + 1 >= lpEndOfRegion)
								goto READ_ERROR_FREE1;
							else if (!lpDest[1])
								break;
							else
								lpDest += 2;
					}
					lpEndOfRegion = (LPBYTE)GetEndOfWritableRegion(lpDest);
					do
					{
						if (!(c = *(lpSrc++)))
							break;
						if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
						{
							if (!(uFlags & NUMBER_OF_CHARS) && !--nCount)
								break;
							lead = c;
							if (!(c = *lpSrc))
								break;
							if (lpDest >= lpEndOfRegion)
								goto WRITE_ERROR_FREE1;
							*(lpDest++) = lead;
						}
						if (lpDest >= lpEndOfRegion)
							goto WRITE_ERROR_FREE1;
						*(lpDest++) = c;
					} while (--nCount);
					if (!(uFlags & CONCATENATE))
					{
						if (nCount)
						{
							if (lpDest + nCount < lpDest || lpDest + nCount > lpEndOfRegion)
								goto WRITE_ERROR_FREE1;
							memset(lpDest, '\0', nCount);
						}
					}
					else
					{
						if (lpDest >= lpEndOfRegion)
							goto WRITE_ERROR_FREE1;
						*lpDest = '\0';
					}

					#undef lpEndOfRegion
				}
				if (lpSrcBuffer)
					HeapFree(hHeap, 0, lpSrcBuffer);

				#undef lpSrcBuffer
			}
			break;
		case TAG_STRLCPY:
			uFlags = 0;
			goto STRLCPY;
		case TAG_STRLCAT:
			uFlags = CONCATENATE;
			goto STRLCPY;
		case TAG_WCSLCPY:
			uFlags = UNICODE_FUNCTION;
			goto STRLCPY;
		case TAG_WCSLCAT:
			uFlags = UNICODE_FUNCTION | CONCATENATE;
		STRLCPY:
			{
				MARKUP   *element;
				NTSTATUS Status;
				HANDLE   hDestProcess;
				LPVOID   lpDest;
				size_t   nDestLength;
				HANDLE   hSrcProcess;
				LPCVOID  lpSrc;
				size_t   nSrcLength;
				size_t   nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				hDestProcess = (HANDLE)(element->Param->Tag != TAG_PARAM_LOCAL);
				lpDest = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[0].Quad : (LPVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				hSrcProcess = (HANDLE)(!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL);
				lpSrc = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				if (hDestProcess || hSrcProcess)
				{
					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if (hDestProcess)
						hDestProcess = hProcess;
					if (hSrcProcess)
						hSrcProcess = hProcess;
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				if ((nSrcLength = StringLength(hSrcProcess, lpAddress = (LPVOID)lpSrc, -1, uFlags)) == -1)
					goto READ_ERROR;
				Status = STATUS_SUCCESS;
				if (!(uFlags & CONCATENATE))
				{
					nDestLength = 0;
					if (nCount)
					{
						if (nCount = min(nCount - 1, nSrcLength))
							Status = CopyProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, SIZE_OF_STRING(nCount, uFlags));
						if (NT_SUCCESS(Status))
						{
							LPVOID lpEndOfString;

							lpEndOfString = (LPBYTE)lpDest + SIZE_OF_STRING(nCount, uFlags);
							if (hDestProcess)
							{
								if (!WriteProcessMemory(hDestProcess, lpEndOfString, &Terminator, SIZE_OF_CHAR(uFlags), NULL))
									Status = STATUS_MEMORY_WRITE_FAILED;
							}
							else
							{
								if (IsBadWritePtr(lpEndOfString, SIZE_OF_CHAR(uFlags)))
									Status = STATUS_MEMORY_WRITE_FAILED;
								TERMINATE_STRING(lpEndOfString, 0, uFlags);
							}
						}
					}
				}
				else
				{
					if ((nDestLength = StringLength(hDestProcess, lpAddress = lpDest, -1, uFlags)) == -1)
						goto READ_ERROR;
					if (nCount > nDestLength + 1)
					{
						LPVOID lpEndOfString;

						(LPBYTE)lpDest += SIZE_OF_STRING(nDestLength, uFlags);
						if (nCount = min(nCount - (nDestLength + 1), nSrcLength))
							Status = CopyProcessMemory(hDestProcess, lpDest, hSrcProcess, lpSrc, SIZE_OF_STRING(nCount, uFlags));
						lpEndOfString = (LPBYTE)lpDest + SIZE_OF_STRING(nCount, uFlags);
						if (hDestProcess)
						{
							if (!WriteProcessMemory(hDestProcess, lpEndOfString, &Terminator, SIZE_OF_CHAR(uFlags), NULL))
								Status = STATUS_MEMORY_WRITE_FAILED;
						}
						else
						{
							if (IsBadWritePtr(lpEndOfString, SIZE_OF_CHAR(uFlags)))
								Status = STATUS_MEMORY_WRITE_FAILED;
							TERMINATE_STRING(lpEndOfString, 0, uFlags);
						}
					}
				}
				if (NT_SUCCESS(Status))
				{
#if INTPTR_MAX > INT32_MAX
					lpOperandTop->IsQuad = TRUE;
					if (IsInteger)
#else
					if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
						lpOperandTop->Quad = nDestLength + nSrcLength;
					else
						lpOperandTop->Real = nDestLength + nSrcLength;
				}
				else
				{
					if (Status == STATUS_MEMORY_READ_FAILED)
					{
						lpAddress = (LPVOID)lpSrc;
						goto READ_ERROR;
					}
					if (Status == STATUS_MEMORY_WRITE_FAILED)
					{
						lpAddress = lpDest;
						goto WRITE_ERROR;
					}
					goto PARSING_ERROR;
				}
			}
			break;
		case TAG_STRCHR:
			uFlags = NULL_TERMINATED;
			goto STRCHR;
		case TAG_WCSCHR:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED;
			goto STRCHR;
		case TAG_STRICHR:
			uFlags = INSENSITIVE_CASE | NULL_TERMINATED;
			goto STRCHR;
		case TAG_WCSICHR:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NULL_TERMINATED;
			goto STRCHR;
		case TAG_STRRCHR:
			uFlags = REVERSE | NULL_TERMINATED;
			goto STRCHR;
		case TAG_WCSRCHR:
			uFlags = UNICODE_FUNCTION | REVERSE | NULL_TERMINATED;
			goto STRCHR;
		case TAG_STRRICHR:
			uFlags = INSENSITIVE_CASE | REVERSE | NULL_TERMINATED;
			goto STRCHR;
		case TAG_WCSRICHR:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | REVERSE | NULL_TERMINATED;
			goto STRCHR;
		case TAG_MEMCHR:
			uFlags = NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_WMEMCHR:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_MEMICHR:
			uFlags = INSENSITIVE_CASE | NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_WMEMICHR:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_MEMRCHR:
			uFlags = REVERSE | NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_WMEMRCHR:
			uFlags = UNICODE_FUNCTION | REVERSE | NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_MEMRICHR:
			uFlags = INSENSITIVE_CASE | REVERSE | NUMBER_OF_CHARS;
			goto STRCHR;
		case TAG_WMEMRICHR:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | REVERSE | NUMBER_OF_CHARS;
		STRCHR:
			{
				MARKUP       *element;
				HANDLE       hTargetProcess;
				LPVOID       lpResult;
				LPCVOID      lpBuffer;
				unsigned int c;
				size_t       nCount;
				size_t       nPos;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[0].Quad : (LPCVOID)(uintptr_t)lpOperandTop[0].Real;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				c = IsInteger ? lpOperandTop[1].Low : (unsigned int)lpOperandTop[1].Real;
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					if ((nCount = StringLength(hProcess, lpAddress = (LPVOID)lpBuffer, -1, uFlags)) == -1)
						goto READ_ERROR;
					nCount++;
				}
				else
				{
					element = element->Next;
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR;
					nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
				}
				nPos = FindProcessMemory(hProcess, lpAddress = (LPVOID)lpBuffer, c, nCount, uFlags);
				if (nPos == -1)
					goto READ_ERROR;
				lpResult = nPos < nCount ? (LPBYTE)lpBuffer + SIZE_OF_STRING(nPos, uFlags) : NULL;
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (size_t)lpResult;
				else
					lpOperandTop->Real = (size_t)lpResult;
			}
			break;
		case TAG_MBSCHR:
			lpFunction = (FARPROC)_mbschr;
			goto MBSCHR;
		case TAG_MBSICHR:
			lpFunction = (FARPROC)_mbsichr;
			goto MBSCHR;
		case TAG_MBSRCHR:
			lpFunction = (FARPROC)_mbsrchr;
			goto MBSCHR;
		case TAG_MBSRICHR:
			lpFunction = (FARPROC)_mbsrichr;
		MBSCHR:
			{
				#define lpBuffer ((LPBYTE)lpBuffer1)

				MARKUP       *element;
				LPBYTE       lpResult;
				LPCBYTE      lpString;
				unsigned int c;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer = NULL;
				lpString = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString, -1)) == -1)
						goto READ_ERROR;
					if (!(lpBuffer = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpString, lpBuffer, nLength, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer[nLength] = '\0';
				}
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR_FREE1;
				c = IsInteger ? lpOperandTop[1].Low : (unsigned int)lpOperandTop[1].Real;
				lpResult = ((unsigned char *(__cdecl *)(const unsigned char *, unsigned int))lpFunction)(lpBuffer ? lpBuffer : lpString, c);
				if (lpResult && lpBuffer)
					lpResult += lpString - lpBuffer;
				if (lpBuffer)
					HeapFree(hHeap, 0, lpBuffer);
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (size_t)lpResult;
				else
					lpOperandTop->Real = (size_t)lpResult;

				#undef lpBuffer
			}
			break;
		case TAG_STRSTR:
			uFlags = NULL_TERMINATED;
			goto STRSTR;
		case TAG_WCSSTR:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED;
			goto STRSTR;
		case TAG_STRISTR:
			uFlags = INSENSITIVE_CASE | NULL_TERMINATED;
			goto STRSTR;
		case TAG_WCSISTR:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NULL_TERMINATED;
			goto STRSTR;
		case TAG_STRRSTR:
			uFlags = REVERSE | NULL_TERMINATED;
			goto STRSTR;
		case TAG_WCSRSTR:
			uFlags = UNICODE_FUNCTION | REVERSE | NULL_TERMINATED;
			goto STRSTR;
		case TAG_STRRISTR:
			uFlags = INSENSITIVE_CASE | REVERSE | NULL_TERMINATED;
			goto STRSTR;
		case TAG_WCSRISTR:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | REVERSE | NULL_TERMINATED;
			goto STRSTR;
		case TAG_MEMMEM:
			uFlags = NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_WMEMMEM:
			uFlags = UNICODE_FUNCTION | NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_MEMIMEM:
			uFlags = INSENSITIVE_CASE | NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_WMEMIMEM:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_MEMRMEM:
			uFlags = REVERSE | NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_WMEMRMEM:
			uFlags = UNICODE_FUNCTION | REVERSE | NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_MEMRIMEM:
			uFlags = INSENSITIVE_CASE | REVERSE | NUMBER_OF_CHARS;
			goto STRSTR;
		case TAG_WMEMRIMEM:
			uFlags = UNICODE_FUNCTION | INSENSITIVE_CASE | REVERSE | NUMBER_OF_CHARS;
		STRSTR:
			{
				NTSTATUS status;
				MARKUP   *element;
				VARIAUNT *lpOperand;
				LPVOID   result;
				HANDLE   haystack_process;
				LPCVOID  haystack;
				size_t   haystack_len;
				HANDLE   needle_process;
				LPCVOID  needle;
				size_t   needle_len;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpOperand = lpOperandTop;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					haystack_process = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					haystack_process = hProcess;
				else
					goto OPEN_ERROR;
				haystack = IsInteger ? (LPCVOID)(uintptr_t)(lpOperand++)->Quad : (LPCVOID)(uintptr_t)(lpOperand++)->Real;
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					if ((haystack_len = StringLength(haystack_process, haystack, -1, uFlags)) == -1)
						goto STRSTR_HAYSTACK_READ_ERROR;
				}
				else
				{
					element = element->Next;
					haystack_len = IsInteger ? (size_t)(lpOperand++)->Quad : (size_t)(lpOperand++)->Real;
				}
				element = element->Next;
				if (IsStringOperand(element->Param) || element->Param->Tag == TAG_PARAM_LOCAL)
					needle_process = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					needle_process = hProcess;
				else
					goto OPEN_ERROR;
				needle = IsInteger ? (LPCVOID)(uintptr_t)(lpOperand++)->Quad : (LPCVOID)(uintptr_t)(lpOperand++)->Real;
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					if ((needle_len = StringLength(needle_process, needle, -1, uFlags)) == -1)
						goto STRSTR_NEEDLE_READ_ERROR;
				}
				else
				{
					needle_len = IsInteger ? (size_t)(lpOperand++)->Quad : (size_t)(lpOperand++)->Real;
				}
				result = (LPVOID)haystack;
				if (needle_len)
				{
					result = NULL;
					if (haystack_len >= needle_len)
					{
						unsigned int c;
						BOOL         bInsensitiveCase;

						if (!ReadProcessMemory(needle_process ? needle_process : GetCurrentProcess(), needle, &c, SIZE_OF_CHAR(uFlags), NULL))
							goto STRSTR_NEEDLE_READ_ERROR;
						bInsensitiveCase = uFlags & INSENSITIVE_CASE;
						if (!(uFlags & REVERSE))
						{
							if (!(uFlags & UNICODE_FUNCTION))
							{
								LPCSTR end;

								end = (LPCSTR)haystack + haystack_len - needle_len + 1;
								do
								{
									size_t maxlen, pos;
									int    i;

									if ((pos = FindProcessMemoryA(haystack_process, haystack, (char)c, maxlen = end - (LPCSTR)haystack, bInsensitiveCase)) >= maxlen)
										if (pos != -1)
											break;
										else
											goto STRSTR_HAYSTACK_READ_ERROR;
									status = CompareProcessMemoryA(&i, haystack_process, (LPCSTR)haystack += pos, needle_process, needle, needle_len, bInsensitiveCase);
									if (!NT_SUCCESS(status))
										goto STRSTR_COMPARE_ERROR;
									if (i)
										continue;
									result = (LPVOID)haystack;
									break;
								} while (end > ++(LPCSTR)haystack);
							}
							else
							{
								LPCWSTR end;

								end = (LPCWSTR)haystack + haystack_len - needle_len + 1;
								do
								{
									size_t maxlen, pos;
									int    i;

									if ((pos = FindProcessMemoryW(haystack_process, haystack, (wchar_t)c, maxlen = end - (LPCWSTR)haystack, bInsensitiveCase)) >= maxlen)
										if (pos != -1)
											break;
										else
											goto STRSTR_HAYSTACK_READ_ERROR;
									status = CompareProcessMemoryW(&i, haystack_process, (LPCWSTR)haystack += pos, needle_process, needle, needle_len, bInsensitiveCase);
									if (!NT_SUCCESS(status))
										goto STRSTR_COMPARE_ERROR;
									if (i)
										continue;
									result = (LPVOID)haystack;
									break;
								} while (end > ++(LPCWSTR)haystack);
							}
						}
						else
						{
							if (!(uFlags & UNICODE_FUNCTION))
							{
								LPCSTR p;

								p = (LPCSTR)haystack + haystack_len - needle_len + 1;
								do
								{
									size_t maxlen, pos;
									int    i;

									if ((pos = FindReverseProcessMemoryA(haystack_process, haystack, (char)c, maxlen = p - (LPCSTR)haystack, bInsensitiveCase)) >= maxlen)
										if (pos != -1)
											break;
										else
											goto STRSTR_HAYSTACK_READ_ERROR;
									status = CompareProcessMemoryA(&i, haystack_process, p = (LPCSTR)haystack + pos, needle_process, needle, needle_len, bInsensitiveCase);
									if (!NT_SUCCESS(status))
										goto STRSTR_COMPARE_ERROR;
									if (i)
										continue;
									result = (LPVOID)p;
									break;
								} while (p != haystack);
							}
							else
							{
								LPCWSTR p;

								p = (LPCWSTR)haystack + haystack_len - needle_len + 1;
								do
								{
									size_t maxlen, pos;
									int    i;

									if ((pos = FindReverseProcessMemoryW(haystack_process, haystack, (wchar_t)c, maxlen = p - (LPCWSTR)haystack, bInsensitiveCase)) >= maxlen)
										if (pos != -1)
											break;
										else
											goto STRSTR_HAYSTACK_READ_ERROR;
									status = CompareProcessMemoryW(&i, haystack_process, p = (LPCWSTR)haystack + pos, needle_process, needle, needle_len, bInsensitiveCase);
									if (!NT_SUCCESS(status))
										goto STRSTR_COMPARE_ERROR;
									if (i)
										continue;
									result = (LPVOID)p;
									break;
								} while (p != haystack);
							}
						}
					}
				}
#if INTPTR_MAX > INT32_MAX
				lpOperandTop->IsQuad = TRUE;
				if (IsInteger)
#else
				if (!(lpOperandTop->IsQuad = !IsInteger))
#endif
					lpOperandTop->Quad = (size_t)result;
				else
					lpOperandTop->Real = (size_t)result;
				break;

			STRSTR_COMPARE_ERROR:
				if (status == STATUS_MEMORY_READ1_FAILED)
				{
			STRSTR_HAYSTACK_READ_ERROR:
					lpAddress = (LPVOID)haystack;
					goto READ_ERROR;
				}
				if (status == STATUS_MEMORY_READ2_FAILED)
				{
			STRSTR_NEEDLE_READ_ERROR:
					lpAddress = (LPVOID)needle;
					goto READ_ERROR;
				}
				goto PARSING_ERROR;
			}
			break;
		case TAG_MBSSTR:
			lpFunction = (FARPROC)_mbsstr;
			goto MBSSTR;
		case TAG_MBSISTR:
			lpFunction = (FARPROC)_mbsistr;
			goto MBSSTR;
		case TAG_MBSRSTR:
			lpFunction = (FARPROC)_mbsrstr;
			goto MBSSTR;
		case TAG_MBSRISTR:
			lpFunction = (FARPROC)_mbsristr;
		MBSSTR:
			{
				#define lpBuffer1 ((LPBYTE)lpBuffer1)
				#define lpBuffer2 ((LPBYTE)lpBuffer2)

				MARKUP  *element;
				LPBYTE  lpResult;
				LPCBYTE lpString1;
				LPCBYTE lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[0].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString1, -1)) == -1)
						goto READ_ERROR;
					if (!(lpBuffer1 = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpString1, lpBuffer1, nLength, NULL))
						goto READ_ERROR_FREE1;
					lpBuffer1[nLength] = '\0';
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCBYTE)(uintptr_t)lpOperandTop[1].Quad : (LPCBYTE)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR_FREE1;
					if ((nLength = StringLengthA(hProcess, lpAddress = (LPVOID)lpString2, -1)) == -1)
						goto READ_ERROR_FREE1;
					if (!(lpBuffer2 = (LPBYTE)HeapAlloc(hHeap, 0, nLength + 1)))
						goto ALLOC_ERROR_FREE1;
					if (!ReadProcessMemory(hProcess, lpString2, lpBuffer2, nLength, NULL))
						goto READ_ERROR_FREE2;
					((LPBYTE)lpString2 = lpBuffer2)[nLength] = '\0';
				}
				lpResult = ((unsigned char *(__cdecl *)(const unsigned char *, const unsigned char *))lpFunction)(lpBuffer1 ? lpBuffer1 : lpString1, lpString2);
				if (lpResult && lpBuffer1)
					lpResult += lpString1 - lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (size_t)lpResult;
				else
					lpOperandTop->Real = (size_t)lpResult;

				#undef lpBuffer1
				#undef lpBuffer2
			}
			break;
		case TAG_STRSPN:
			uFlags = 0;
			lpFunction = (FARPROC)strspn;
			goto STRSPN;
		case TAG_STRSPNP:
			uFlags = EXTENDED_POINTER;
			lpFunction = (FARPROC)_strspnp;
			goto STRSPN;
		case TAG_STRCSPN:
			uFlags = 0;
			lpFunction = (FARPROC)strcspn;
			goto STRSPN;
		case TAG_STRPBRK:
			uFlags = EXTENDED_POINTER;
			lpFunction = (FARPROC)strpbrk;
			goto STRSPN;
		case TAG_MBSSPN:
			uFlags = 0;
			lpFunction = (FARPROC)_mbsspn;
			goto STRSPN;
		case TAG_MBSSPNP:
			uFlags = EXTENDED_POINTER;
			lpFunction = (FARPROC)_mbsspnp;
			goto STRSPN;
		case TAG_MBSCSPN:
			uFlags = 0;
			lpFunction = (FARPROC)_mbscspn;
			goto STRSPN;
		case TAG_MBSPBRK:
			uFlags = EXTENDED_POINTER;
			lpFunction = (FARPROC)_mbspbrk;
			goto STRSPN;
		case TAG_WCSSPN:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)wcsspn;
			goto STRSPN;
		case TAG_WCSSPNP:
			uFlags = UNICODE_FUNCTION | EXTENDED_POINTER;
			lpFunction = (FARPROC)_wcsspnp;
			goto STRSPN;
		case TAG_WCSCSPN:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)wcscspn;
			goto STRSPN;
		case TAG_WCSPBRK:
			uFlags = UNICODE_FUNCTION | EXTENDED_POINTER;
			lpFunction = (FARPROC)wcspbrk;
			goto STRSPN;
		STRSPN:
			{
				#define lpBuffer1 ((LPVOID)lpBuffer1)
				#define lpBuffer2 ((LPVOID)lpBuffer2)

				MARKUP  *element;
				size_t  nResult;
				LPCVOID lpString1;
				LPCVOID lpString2;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				lpString1 = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[0].Quad : (LPCVOID)(uintptr_t)lpOperandTop[0].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR;
					if ((nLength = StringLength(hProcess, lpAddress = (LPVOID)lpString1, -1, uFlags)) == -1)
						goto READ_ERROR;
					if (!(lpBuffer1 = HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hProcess, lpString1, lpBuffer1, SIZE_OF_STRING(nLength, uFlags), NULL))
						goto READ_ERROR_FREE1;
					TERMINATE_STRING(lpBuffer1, nLength, uFlags);
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR_FREE1;
					if ((nLength = StringLength(hProcess, lpAddress = (LPVOID)lpString2, -1, uFlags)) == -1)
						goto READ_ERROR_FREE1;
					if (!(lpBuffer2 = HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
						goto ALLOC_ERROR_FREE1;
					if (!ReadProcessMemory(hProcess, lpString2, lpBuffer2, SIZE_OF_STRING(nLength, uFlags), NULL))
						goto READ_ERROR_FREE2;
					TERMINATE_STRING(lpBuffer2, nLength, uFlags);
				}
				nResult = ((size_t(__cdecl *)(const void *, const void *))lpFunction)(lpBuffer1 ? lpBuffer1 : lpString1, lpString2);
				if ((uFlags & EXTENDED_POINTER) && nResult && lpBuffer1)
					nResult += (LPBYTE)lpString1 - (LPBYTE)lpBuffer1;
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = nResult;
				else
					lpOperandTop->Real = nResult;

				#undef lpBuffer1
				#undef lpBuffer2
			}
			break;
		case TAG_STRSET:
		case TAG_MBSSET:
			uFlags = NULL_TERMINATED;
			goto STRSET;
		case TAG_WCSSET:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED;
			goto STRSET;
		case TAG_STRNSET:
		case TAG_MBSNBSET:
		case TAG_MBSNSET:
			uFlags = NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRSET;
		case TAG_WCSNSET:
			uFlags = UNICODE_FUNCTION | NULL_TERMINATED | NUMBER_OF_CHARS;
			goto STRSET;
		case TAG_MEMSET8:
		case TAG_MEMSET16:
		case TAG_MEMSET32:
		case TAG_MEMSET64:
			uFlags = NUMBER_OF_CHARS;
		STRSET:
			{
				#define lpDest lpAddress

				MARKUP         *element;
				HANDLE         hDestProcess;
				ULARGE_INTEGER qwFill;
				size_t         nCount;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hDestProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hDestProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpDest = IsInteger ? (PVOID)(uintptr_t)lpOperandTop[0].Quad : (PVOID)(uintptr_t)lpOperandTop[0].Real;
				element = element->Next;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				qwFill.QuadPart = IsInteger ? lpOperandTop[1].Quad : (uint64_t)lpOperandTop[1].Real;
				if (!(uFlags & NUMBER_OF_CHARS))
				{
					if ((nCount = StringLength(hDestProcess, lpDest, -1, uFlags)) == -1)
						goto READ_ERROR;
				}
				else
				{
					element = element->Next;
					if (IsStringOperand(element->Param))
						goto PARSING_ERROR;
					nCount = IsInteger ? (size_t)lpOperandTop[2].Quad : (size_t)lpOperandTop[2].Real;
					if (uFlags & NULL_TERMINATED)
					{
						if (lpMarkup->Tag == TAG_MBSNSET && qwFill.QuadPart >= 0x100)
							if (_add_uintptr(nCount, nCount, &nCount))
								nCount = -1;
						if ((nCount = StringLength(hDestProcess, lpDest, nCount, uFlags)) == -1)
							goto READ_ERROR;
					}
				}
				switch (lpMarkup->Tag)
				{
				case TAG_MBSSET:
				case TAG_MBSNBSET:
				case TAG_MBSNSET:
					if (qwFill.QuadPart >= 0x100)
					{
						if (qwFill.QuadPart >= 0x10000 || !(BYTE)qwFill.LowPart)
							break;
						if (!IsDBCSLeadByteEx(CP_THREAD_ACP, (BYTE)(qwFill.LowPart = _byteswap_ulong(qwFill.LowPart) >> 16)))
							break;
						if (!FillProcessMemory16(hDestProcess, lpDest, nCount / 2, (WORD)qwFill.LowPart))
							goto WRITE_ERROR;
						if (!(nCount & 1))
							break;
						if (hDestProcess)
						{
							const char spaceChar = ' ';

							if (!WriteProcessMemory(hDestProcess, (LPBYTE)lpDest + nCount - 1, &spaceChar, 1, NULL))
								goto WRITE_ERROR;
						}
						else
						{
							if (IsBadWritePtr((LPBYTE)lpDest + nCount - 1, 1))
								goto WRITE_ERROR;
							((LPBYTE)lpDest)[nCount - 1] = ' ';
						}
						break;
					}
					/* FALLTHROUGH */
				case TAG_STRSET:
				case TAG_STRNSET:
				case TAG_MEMSET8:
					if (!FillProcessMemory(hDestProcess, lpDest, nCount, (BYTE)qwFill.LowPart))
						goto WRITE_ERROR;
					break;
				case TAG_WCSSET:
				case TAG_WCSNSET:
				case TAG_MEMSET16:
					if (!FillProcessMemory16(hDestProcess, lpDest, nCount, (WORD)qwFill.LowPart))
						goto WRITE_ERROR;
					break;
				case TAG_MEMSET32:
					if (!FillProcessMemory32(hDestProcess, lpDest, nCount, qwFill.LowPart))
						goto WRITE_ERROR;
					break;
				case TAG_MEMSET64:
					if (!FillProcessMemory64(hDestProcess, lpDest, nCount, qwFill.QuadPart))
						goto WRITE_ERROR;
					break;
				default:
					__assume(0);
				}

				#undef lpDest
			}
			break;
		case TAG_STRTOK:
			uFlags = 0;
			lpFunction = (FARPROC)internal_strtok;
			lpStrlen = (size_t(__cdecl *)(const void *))strlen;
			lpStrtokProcess = &strtok_process;
			lpStrtokContext = (LPVOID *)&strtok_context;
			goto STRTOK;
		case TAG_WCSTOK:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)internal_wcstok;
			lpStrlen = (size_t(__cdecl *)(const void *))wcslen;
			lpStrtokProcess = &wcstok_process;
			lpStrtokContext = (LPVOID *)&wcstok_context;
			goto STRTOK;
		case TAG_MBSTOK:
			uFlags = 0;
			lpFunction = (FARPROC)internal_mbstok;
			lpStrlen = (size_t(__cdecl *)(const void *))strlen;
			lpStrtokProcess = &mbstok_process;
			lpStrtokContext = (LPVOID *)&mbstok_context;
		STRTOK:
			{
				#define lpBuffer1 ((LPVOID)lpBuffer1)
				#define lpBuffer2 ((LPVOID)lpBuffer2)

				MARKUP  *element;
				HANDLE  hTargetProcess;
				HANDLE  hContextProcess;
				LPVOID  lpResult;
				LPVOID  lpString1;
				LPCVOID lpString2;
				LPVOID  *lpContext;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				element = lpMarkup;
				lpBuffer2 = lpBuffer1 = NULL;
				if (IsStringOperand(element->Param))
					goto PARSING_ERROR;
				if (element->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				hContextProcess = NULL;
				lpContext = NULL;
				if (*lpStrtokProcess != hTargetProcess)
				{
					*lpStrtokProcess = hTargetProcess;
					*lpStrtokContext = NULL;
				}
				else if (lpMarkup->NumberOfOperand >= 3 && (lpContext = IsInteger ? (LPVOID *)(uintptr_t)lpOperandTop[2].Quad : (LPVOID *)(uintptr_t)lpOperandTop[2].Real))
				{
					if (element->Next->Next->Param->Tag == TAG_PARAM_LOCAL)
					{
						if (IsBadReadPtr(lpAddress = lpContext, sizeof(LPVOID)))
							goto READ_ERROR;
						*lpStrtokContext = *lpContext;
					}
					else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					{
						if (!ReadProcessMemory(hContextProcess = hProcess, lpAddress = lpContext, lpStrtokContext, sizeof(LPVOID), NULL))
							goto READ_ERROR;
					}
					else
					{
						goto OPEN_ERROR;
					}
				}
				lpString1 = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop[0].Quad : (LPVOID)(uintptr_t)lpOperandTop[0].Real;
				if (lpAddress = lpString1 ? (LPVOID)lpString1 : hTargetProcess ? *lpStrtokContext : NULL)
				{
					size_t nLength;

					if ((nLength = StringLength(hTargetProcess, lpAddress, -1, uFlags)) == -1)
						goto READ_ERROR;
					if (hTargetProcess)
					{
						if (!(lpBuffer1 = HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
							goto ALLOC_ERROR;
						if (!ReadProcessMemory(hTargetProcess, lpAddress, lpBuffer1, SIZE_OF_STRING(nLength, uFlags), NULL))
							goto READ_ERROR_FREE1;
						TERMINATE_STRING(lpBuffer1, nLength, uFlags);
						if (!lpString1)
						{
							lpString1 = *lpStrtokContext;
							*lpStrtokContext = lpBuffer1;
						}
					}
				}
				element = element->Next;
				lpString2 = IsInteger ? (LPCVOID)(uintptr_t)lpOperandTop[1].Quad : (LPCVOID)(uintptr_t)lpOperandTop[1].Real;
				if (!IsStringOperand(element->Param) && element->Param->Tag != TAG_PARAM_LOCAL)
				{
					size_t nLength;

					if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
						goto OPEN_ERROR_FREE1;
					if ((nLength = StringLength(hProcess, lpAddress = (LPVOID)lpString2, -1, uFlags)) == -1)
						goto READ_ERROR_FREE1;
					if (!(lpBuffer2 = HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
						goto ALLOC_ERROR_FREE1;
					if (!ReadProcessMemory(hProcess, lpString2, lpBuffer2, SIZE_OF_STRING(nLength, uFlags), NULL))
						goto READ_ERROR_FREE2;
					TERMINATE_STRING(lpBuffer2, nLength, uFlags);
				}
				if (lpResult = ((void *(__fastcall *)(void *, const void *, void **))lpFunction)(lpBuffer1 ? lpBuffer1 : lpString1, lpString2, lpStrtokContext))
				{
					if (lpBuffer1)
					{
						LPVOID    lpEndOfString;
						size_t    nSize;
						ptrdiff_t iDiff;

						nSize = (LPBYTE)*lpStrtokContext - ((LPBYTE)lpEndOfString = (LPBYTE)lpResult + SIZE_OF_STRING(lpStrlen(lpResult), uFlags));
						*(LPBYTE *)lpStrtokContext += (iDiff = (LPBYTE)lpString1 - (LPBYTE)lpBuffer1);
						(LPBYTE)lpResult += iDiff;
						if (nSize && !WriteProcessMemory(*lpStrtokProcess, lpAddress = (LPBYTE)lpEndOfString + iDiff, lpEndOfString, nSize, NULL))
							goto WRITE_ERROR_FREE2;
					}
					if (lpContext)
					{
						if (hContextProcess)
						{
							if (!WriteProcessMemory(hContextProcess, lpAddress = lpContext, lpStrtokContext, sizeof(LPVOID), NULL))
								goto WRITE_ERROR_FREE2;
						}
						else
						{
							if (IsBadWritePtr(lpAddress = lpContext, sizeof(LPVOID)))
								goto WRITE_ERROR_FREE2;
							*lpContext = *lpStrtokContext;
						}
					}
				}
				if (lpBuffer2)
					HeapFree(hHeap, 0, lpBuffer2);
				if (lpBuffer1)
					HeapFree(hHeap, 0, lpBuffer1);
				if (!(lpOperandTop->IsQuad = !IsInteger))
					lpOperandTop->Quad = (size_t)lpResult;
				else
					lpOperandTop->Real = (size_t)lpResult;

				#undef lpBuffer1
				#undef lpBuffer2
			}
			break;
		case TAG_STRLWR:
			uFlags = 0;
			lpFunction = (FARPROC)_strlwr;
			goto STRLWR;
		case TAG_WCSLWR:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)_wcslwr;
			goto STRLWR;
		case TAG_MBSLWR:
			uFlags = 0;
			lpFunction = (FARPROC)_mbslwr;
			goto STRLWR;
		case TAG_STRUPR:
			uFlags = 0;
			lpFunction = (FARPROC)_strupr;
			goto STRLWR;
		case TAG_WCSUPR:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)_wcsupr;
			goto STRLWR;
		case TAG_MBSUPR:
			uFlags = 0;
			lpFunction = (FARPROC)_mbsupr;
			goto STRLWR;
		case TAG_STRREV:
			uFlags = 0;
			lpFunction = (FARPROC)_strrev;
			goto STRLWR;
		case TAG_WCSREV:
			uFlags = UNICODE_FUNCTION;
			lpFunction = (FARPROC)_wcsrev;
			goto STRLWR;
		case TAG_MBSREV:
			uFlags = 0;
			lpFunction = (FARPROC)_mbsrev;
		STRLWR:
			{
				#define lpBuffer ((LPVOID)lpBuffer1)

				HANDLE hTargetProcess;
				size_t nLength;
				LPVOID lpString;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (IsStringOperand(lpMarkup->Param))
					goto PARSING_ERROR;
				if (lpMarkup->Param->Tag == TAG_PARAM_LOCAL)
					hTargetProcess = NULL;
				else if (hProcess || (hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
					hTargetProcess = hProcess;
				else
					goto OPEN_ERROR;
				lpString = IsInteger ? (LPVOID)(uintptr_t)lpOperandTop->Quad : (LPVOID)(uintptr_t)lpOperandTop->Real;
				if ((nLength = StringLength(hTargetProcess, lpAddress = lpString, -1, uFlags)) == -1)
					goto READ_ERROR;
				lpBuffer = NULL;
				if (hTargetProcess)
				{
					if (!(lpBuffer = HeapAlloc(hHeap, 0, SIZE_OF_STRING(nLength + 1, uFlags))))
						goto ALLOC_ERROR;
					if (!ReadProcessMemory(hTargetProcess, lpString, lpBuffer, SIZE_OF_STRING(nLength, uFlags), NULL))
						goto READ_ERROR_FREE1;
					TERMINATE_STRING(lpBuffer, nLength, uFlags);
				}
				((void *(__cdecl *)(void *))lpFunction)(lpBuffer ? lpBuffer : lpString);
				if (lpBuffer)
				{
					if (!WriteProcessMemory(hProcess, lpString, lpBuffer, SIZE_OF_STRING(nLength, uFlags), NULL))
						goto WRITE_ERROR_FREE1;
					HeapFree(hHeap, 0, lpBuffer);
				}

				#undef lpBuffer
			}
			break;
		case TAG_ISALNUM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISALNUM:
			lpOperandTop->Quad = !lpOperandTop->High && ((lpOperandTop->Low | ('a' - 'A')) - 'a' < 'z' - 'a' + 1 || lpOperandTop->Low - '0' < '9' - '0' + 1);
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISALPHA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISALPHA:
			lpOperandTop->Quad = !lpOperandTop->High && ((lpOperandTop->Low | ('a' - 'A')) - 'a' < 'z' - 'a' + 1);
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISASCII:
		case TAG_ISWASCII:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = lpOperandTop->Quad <= 0x7F;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISBLANK:
		case TAG_ISWBLANK:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low == ' ' || lpOperandTop->Low == '\t');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISCNTRL:
		case TAG_ISWCNTRL:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low < 0x1F + 1 || lpOperandTop->Low == 0x7F);
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISCSYM:
		case TAG_ISWCSYM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && ((lpOperandTop->Low | ('a' - 'A')) - 'a' < 'z' - 'a' + 1 || lpOperandTop->Low - '0' < '9' - '0' + 1 || lpOperandTop->Low == '_');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISCSYMF:
		case TAG_ISWCSYMF:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && ((lpOperandTop->Low | ('a' - 'A')) - 'a' < 'z' - 'a' + 1 || lpOperandTop->Low == '_');
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISDIGIT:
			lpOperandTop->Quad = !lpOperandTop->High && lpOperandTop->Low - '0' < '9' - '0' + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISGRAPH:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISGRAPH:
			lpOperandTop->Quad = !lpOperandTop->High && lpOperandTop->Low - 0x21 < 0x7E - 0x21 + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISKANA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad =
				(BYTE)(lpOperandTop->Low - 0xA1) < 0xDF - 0xA1 + 1 &&
				(!lpOperandTop->IsQuad ?
					lpOperandTop->Low < 0x100 || lpOperandTop->Low >= ~UINT32_C(0xFF) :
					lpOperandTop->Quad < 0x100 || lpOperandTop->Quad >= ~UINT64_C(0xFF));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISLEADBYTE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad =
				((BYTE)(lpOperandTop->Low - 0x81) < 0x9F - 0x81 + 1 || (BYTE)(lpOperandTop->Low - 0xE0) < 0xFC - 0xE0 + 1) &&
				(!lpOperandTop->IsQuad ?
					lpOperandTop->Low < 0x100 || lpOperandTop->Low >= ~UINT32_C(0xFF) :
					lpOperandTop->Quad < 0x100 || lpOperandTop->Quad >= ~UINT64_C(0xFF));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISLOWER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISLOWER:
			lpOperandTop->Quad = !lpOperandTop->High && lpOperandTop->Low - 'a' < 'z' - 'a' + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISPRINT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISPRINT:
			lpOperandTop->Quad = !lpOperandTop->High && lpOperandTop->Low - 0x20 < 0x7E - 0x20 + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISPUNCT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISPUNCT:
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x21) < 0x7E - 0x21 + 1 && (lpOperandTop->Low | ('a' - 'A')) - 'z' - 1 < 'a' - 'z' - 1 && lpOperandTop->Low - '9' - 1 < '0' - '9' - 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISSPACE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISSPACE:
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low == ' ' || lpOperandTop->Low - '\t' < '\r' - '\t' + 1);
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISTRAILBYTE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad =
				((BYTE)(lpOperandTop->Low - 0x40) < 0x7E - 0x40 + 1 || (BYTE)(lpOperandTop->Low - 0x80) < 0xEC - 0x80 + 1) &&
				((char)lpOperandTop->Low >= 0 ?
					lpOperandTop->Quad < 0x100 :
					!lpOperandTop->IsQuad ?
						lpOperandTop->Low < 0x100 || lpOperandTop->Low >= ~UINT32_C(0xFF) :
						lpOperandTop->Quad < 0x100 || lpOperandTop->Quad >= ~UINT64_C(0xFF));
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISUPPER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISUPPER:
			lpOperandTop->Quad = !lpOperandTop->High && lpOperandTop->Low - 'A' < 'Z' - 'A' + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISXDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
		ISXDIGIT:
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - '0' < '9' - '0' + 1 || (lpOperandTop->Low | ('a' - 'A')) - 'a' < 'f' - 'a' + 1);
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBBALNUM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISALNUM;
				wCharTypeMask = C1_UPPER | C1_LOWER | C1_DIGIT;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBBALPHA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISALPHA;
				wCharTypeMask = C1_UPPER | C1_LOWER;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBBGRAPH:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISGRAPH;
				wCharTypeMask = C1_UPPER | C1_LOWER | C1_DIGIT | C1_PUNCT;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBBPRINT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISPRINT;
				wCharTypeMask = C1_UPPER | C1_LOWER | C1_DIGIT | C1_PUNCT | C1_BLANK;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCALNUM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISALNUM;
				wCharTypeMask = C1_ALPHA | C1_DIGIT;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCALPHA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISALPHA;
				wCharTypeMask = C1_ALPHA;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISDIGIT;
				wCharTypeMask = C1_DIGIT;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCGRAPH:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISGRAPH;
				wCharTypeMask = C1_ALPHA | C1_DIGIT | C1_PUNCT;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCHIRA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x829F) < 0x82F1 - 0x829F + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCKATA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x8340) < 0x8396 - 0x8340 + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCL0:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x8140) < 0x889E - 0x8140 + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCL1:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x889F) < 0x9872 - 0x889F + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCL2:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x989F) < 0xEA9E - 0x989F + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCLEGAL:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x8140 < 0x9FFC - 0x8140 + 1 || lpOperandTop->Low - 0xE040 < 0xFCFC - 0xE040 + 1) && (unsigned char)lpOperandTop->Low != 0x7F;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCLOWER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISLOWER;
				wCharTypeMask = C1_LOWER;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCPRINT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISPRINT;
				wCharTypeMask = C1_ALPHA | C1_DIGIT | C1_PUNCT | C1_BLANK;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCPUNCT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISPUNCT;
				wCharTypeMask = C1_PUNCT;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCSPACE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISSPACE;
				wCharTypeMask = C1_BLANK;
				goto ISMBCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISMBCSYMBOL:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad = !lpOperandTop->High && (lpOperandTop->Low - 0x8141) < 0x81AC - 0x8141 + 1;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_ISMBCUPPER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				char lpSrcStr[2];
				WORD wCharType;

				if (lpOperandTop->Low < 0x100)
					goto ISUPPER;
				wCharTypeMask = C1_UPPER;
			ISMBCTYPE:
				lpSrcStr[0] = (char)(lpOperandTop->Low >> 8);
				lpSrcStr[1] = (char)lpOperandTop->Low;
				lpOperandTop->Low = GetStringTypeA(GetThreadLocale(), CT_CTYPE1, lpSrcStr, 2, &wCharType) && (wCharType & wCharTypeMask);
				if (lpOperandTop->IsQuad = !IsInteger)
					lpOperandTop->Real = (double)lpOperandTop->Quad;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWALNUM:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISALNUM;
				wCharTypeMask = C1_ALPHA | C1_DIGIT;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWALPHA:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISALPHA;
				wCharTypeMask = C1_ALPHA;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISDIGIT;
				wCharTypeMask = C1_DIGIT;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWGRAPH:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISGRAPH;
				wCharTypeMask = C1_ALPHA | C1_DIGIT | C1_PUNCT;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWLOWER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISLOWER;
				wCharTypeMask = C1_LOWER;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWPRINT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISPRINT;
				wCharTypeMask = C1_ALPHA | C1_DIGIT | C1_PUNCT | C1_BLANK;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWPUNCT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISPUNCT;
				wCharTypeMask = C1_PUNCT;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWSPACE:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISSPACE;
				wCharTypeMask = C1_BLANK;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWUPPER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				if (lpOperandTop->Low < 0x100)
					goto ISUPPER;
				wCharTypeMask = C1_UPPER;
				goto ISWCTYPE;
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_ISWXDIGIT:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (lpOperandTop->Quad < 0x10000)
			{
				WORD wCharType;

				if (lpOperandTop->Low < 0x100)
					goto ISXDIGIT;
				wCharTypeMask = C1_XDIGIT;
			ISWCTYPE:
				lpOperandTop->Low = GetStringTypeW(CT_CTYPE1, (LPCWCH)&lpOperandTop->Low, 2, &wCharType) && (wCharType & wCharTypeMask);
			}
			else
			{
				lpOperandTop->Quad = 0;
				lpOperandTop->IsQuad = !IsInteger;
			}
			break;
		case TAG_TOASCII:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			lpOperandTop->Quad &= 0x7F;
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_TOLOWER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!lpOperandTop->High && lpOperandTop->Low - 'A' < 'Z' - 'A' + 1)
				lpOperandTop->Low += 'a' - 'A';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
		case TAG_TOUPPER:
			if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
				goto PARSING_ERROR;
			lpEndOfOperand = lpOperandTop + 1;
			if (!IsInteger)
				lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
			if (!lpOperandTop->High && lpOperandTop->Low - 'a' < 'z' - 'a' + 1)
				lpOperandTop->Low -= 'a' - 'A';
			if (lpOperandTop->IsQuad = !IsInteger)
				lpOperandTop->Real = (double)lpOperandTop->Quad;
			break;
#if USE_PLUGIN
		case TAG_PLUGIN:
			{
				MARKUP          *element;
				size_t          nStackSize;
				VARIAUNT        *lpOperand;
				PLUGIN_FUNCTION *lpFunction;
				uintptr_t       *lpStack;
				PARAM_TYPE      *lpParamType;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				lpFunction = lpMarkup->Function;
				lpParamType = lpFunction->ParamTypes;
				nStackSize = 0;
				if (lpMarkup->NumberOfOperand)
				{
					element = lpMarkup;
#ifndef _WIN64
					lpOperand = lpOperandTop;
#endif
					do
					{
#ifndef _WIN64
						if (lpParamType != lpFunction->EndOfParamTypes)
						{
							nStackSize +=
								*lpParamType != PARAM_QWORD && *lpParamType != PARAM_DOUBLE ?
									sizeof(uint32_t) :
									sizeof(uint64_t);
							lpParamType++;
						}
						else
						{
							nStackSize +=
								(!lpOperand->IsQuad && IsInteger || IsStringOperand(element->Param)) ?
									sizeof(uint32_t) :
									sizeof(uint64_t);
						}
						lpOperand++;
#else
						if (lpParamType != lpFunction->EndOfParamTypes)
							lpParamType++;
						nStackSize += sizeof(uintptr_t);
#endif
					} while (element = element->Next);
					if (nStackSize < lpFunction->StackSize)
						nStackSize = lpFunction->StackSize;
				}
				if (nStackSize)
				{
					uintptr_t *lpParam;

					lpStack = (uintptr_t *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, nStackSize);
					if (!lpStack)
						goto ALLOC_ERROR;
					lpParam = lpStack;
					lpParamType = lpFunction->ParamTypes;
					element = lpMarkup;
					lpOperand = lpOperandTop;
					do
					{
						if (lpParamType != lpFunction->EndOfParamTypes)
						{
							switch (*(lpParamType++))
							{
							default:
#ifndef _WIN64
								*(lpParam++) = IsInteger ? lpOperand->Low : (uint32_t)lpOperand->Real;
								break;
							case PARAM_QWORD:
#endif
								*(((uint64_t *)lpParam)++) = IsInteger ? lpOperand->Quad : (uint64_t)lpOperand->Real;
								break;
							case PARAM_DOUBLE:
								*(((double *)lpParam)++) = IsInteger ? (double)lpOperand->Quad : lpOperand->Real;
								break;
							case PARAM_STRING_A:
							case PARAM_STRING_W:
								*(lpParam++) = IsInteger ? (uintptr_t)lpOperand->Quad : (uintptr_t)lpOperand->Real;
								break;
							}
						}
						else if (!IsStringOperand(element->Param))
						{
#ifndef _WIN64
							if (!lpOperand->IsQuad && IsInteger)
								*(lpParam++) = lpOperand->Low;
							else
#endif
								*(((uint64_t *)lpParam)++) = lpOperand->Quad;
						}
						else
						{
							*(lpParam++) = IsInteger ? (uintptr_t)lpOperand->Quad : (uintptr_t)lpOperand->Real;
						}
						lpOperand++;
					} while (element = element->Next);
				}
				else
				{
					lpStack = NULL;
				}
				ParsingContinue = TRUE;
				if (lpFunction->ReturnType != RETURN_DOUBLE)
					lpOperandTop->Quad = CallPluginFunction(lpFunction->Address, lpStack, nStackSize);
				else
					lpOperandTop->Real = ((double(__cdecl *)(const void *, const void *, size_t))CallPluginFunction)(lpFunction->Address, lpStack, nStackSize);
				if (lpStack)
					HeapFree(hHeap, 0, lpStack);
				switch (lpFunction->ReturnType)
				{
				case RETURN_DWORD:
					if (!(lpOperandTop->IsQuad = !IsInteger))
						lpOperandTop->High = 0;
					else
						lpOperandTop->Real = lpOperandTop->Low;
					break;
				case RETURN_QWORD:
					if (!IsInteger)
						lpOperandTop->Real = (double)lpOperandTop->Quad;
					lpOperandTop->IsQuad = TRUE;
					break;
				case RETURN_DOUBLE:
					if (!(lpOperandTop->IsQuad = !IsInteger))
					{
						int32_t msw;

						msw = lpOperandTop->High;
						lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						if (lpOperandTop->IsQuad = lpOperandTop->High)
							if (!(lpOperandTop->IsQuad = msw >= 0 || (int64_t)lpOperandTop->Quad < INT32_MIN))
								lpOperandTop->High = 0;
					}
					break;
				default:
					OPERAND_CLEAR();
					break;
				}
				if (!ParsingContinue)
					goto FAILED;
			}
			break;
#endif
		case TAG_FUNCTION:
			{
				typedef struct {
					size_t   Length;
					LPCSTR   String;
					uint64_t Actual;
				} PARAMETER;

				static WCHAR const VA[] = L"0123456789";

				vector_string *File;
				string        FName, DefaultExt;
				PARAMETER     *lpParams;
				const string  *Source, *Finish;
				size_t        count, extra = 0, nLength = lpMarkup->Length;
				const char    *sFName = lpMarkup->String;
				TSSGSubject   *Object = SSGS;

				if ((lpOperandTop = lpEndOfOperand - lpMarkup->NumberOfOperand) <= lpOperandBuffer)
					goto PARSING_ERROR;
				lpEndOfOperand = lpOperandTop + 1;
				if (lpMarkup->Type & OS_YIELD_OPERAND)
				{
					VARIABLE *element = NULL;
					size_t length = lpMarkup->Length - 1;
					LPVOID p = lpMarkup->String + 1;
					for (size_t i = 0; i < nNumberOfVariable; i++)
					{
						if (lpVariable[i].Length != length)
							continue;
						if (memcmp(lpVariable[i].String, p, length) != 0)
							continue;
						element = lpVariable + i;
						break;
					}
					if (!element || !element->Value.Quad)
						goto PARSING_ERROR;
					lpAddress = !IsInteger ? (void *)(uint64_t)element->Value.Real : (void *)element->Value.Quad;
					if (IsBadReadPtr(lpAddress, sizeof(char)))
						goto READ_ERROR;
					nLength = strlen(sFName = lpAddress);
				}
				{
					const BOOL Lexical = *sFName == '@';
					string_ctor_assign_cstr_with_length(&FName, sFName + Lexical, nLength - Lexical);
					string_ctor_assign_cstr_with_length(&DefaultExt, ".CHN", 4);
					File = TSSGCtrl_GetSSGDataFile(this, SSGS, FName, DefaultExt, NULL);
					if (!File || vector_empty(File))
						goto PARSING_ERROR;
					Source = vector_begin(File);
					Finish = vector_end  (File);
					if (Lexical && !(Object = &SSGS->folder->super))
						goto PARSING_ERROR;
				}
				lpParams = NULL;
				if (count = lpMarkup->NumberOfOperand)
				{
					PARAMETER    *arg;
					const string *it;
					size_t        i;

					lpParams = (PARAMETER *)HeapAlloc(hHeap, 0, sizeof(*lpParams) * count + sizeof(*lpParams) + sizeof(lpParams->Length));
					if (!lpParams)
						goto ALLOC_ERROR;
					arg = lpParams;
					it = Source;
					i = 0;
					do
					{
						if (++it < Finish)
						{
							if (!(arg->Length = string_length(it)))
								continue;// anonymous parameter ignore argument
							arg->String = string_c_str(it);
						}
						else if (extra < 9)
						{
							arg->Length = sizeof(CHAR);
							arg->String = (LPSTR)&VA[++extra];
						}
						else
							break;
						arg->Actual = lpOperandTop[i].Quad;
						arg++;
					} while (++i < count);
					if (extra)
					{
						arg->Length = sizeof(CHAR);
						arg->String = (LPSTR)&VA[0];
						arg->Actual = extra;
						arg++;
					}
					arg->Length = 0;
				}
#if SCOPE_SUPPORT
				for (register PVARIABLE v = lpVariable, end = v + nNumberOfVariable; v < end; v++)
					if (v->Node && !((ScopeVariant *)pair_first(v->Node))->Identifier.sstIndex)
					{
						((ScopeVariant *)pair_first(v->Node))->Identifier.sstIndex = MAXDWORD;
						((ScopeVariant *)pair_first(v->Node))->Quad = v->Value.Quad;
					}
#endif
				if (this->ssgActionListner && nLength + 10 < 1024) switch (TMainForm_GetUserMode(MainForm))
				{
				case 3:
				case 4:
					CHAR Mes[0x0400] = "Step in `";
					__movsb(&Mes[9], sFName, nLength);
					*(LPWCH)&Mes[9 + nLength] = L'`';
					TMainForm_Guide(Mes, 0);
				}
				lpOperandTop->Quad = InternalParsing(this, Object, Source, IsInteger, lpParams ? (va_list)lpParams : (va_list)&Terminator);
				lpOperandTop->IsQuad = !IsInteger || lpOperandTop->High;
#if SCOPE_SUPPORT
				for (register PVARIABLE v = lpVariable, end = v + nNumberOfVariable; v < end; v++)
					if (v->Node)
					{
						((ScopeVariant *)pair_first(v->Node))->Identifier.sstIndex = 0;
						v->Value.Quad   = ((ScopeVariant *)pair_first(v->Node))->Quad;
						v->Value.IsQuad = !IsInteger || v->Value.High;
					}
#endif
				if (lpParams)
					HeapFree(hHeap, 0, lpParams);
			}
			break;
		case TAG_NOT_OPERATOR:
			{
				VARIAUNT operand;
				size_t   length;
				LPSTR    p, end;
				VARIABLE *element;
				char     *endptr;
				LPSTR    lpEndOfModuleName;
				LPSTR    lpModuleName;
				char     c;

				end = (p = lpMarkup->String) + (length = lpMarkup->Length);
				element = NULL;
				endptr = NULL;
				lpNext = lpPostfix + 1 < lpEndOfPostfix ? lpPostfix[1] : NULL;
#pragma region Null_terminated
				c = *end;
				*end = '\0';
				do	/* do { ... } while (0); */
				{
					size_t prefixLength;
					size_t i;

					if (!length ||
						lpNext && (lpNext->Tag <= TAG_MNAME && lpNext->Tag >= TAG_MODULENAME || lpNext->Tag == TAG_GOTO))
					{
						endptr = end;
						operand = (VARIAUNT) { { 0, 0 }, !IsInteger };
						break;// Operand is always immediate string.
					}
					if (p[prefixLength = 0] != '\'' && (p[0] != 'u' ||
						p[prefixLength = 1] != '\'' && (p[1] != '8' ||
						p[prefixLength = 2] != '\'')))
					{
						if (!IsStringOperand(lpMarkup))
						{
							if (*p == '$')
							{
								p++;
								if (!--length)
									break;
							}
							else if (IsInteger)
							{
								operand.Quad   = _strtoui64(p, &endptr, 0);
								operand.IsQuad = !!operand.High;
							}
							else
							{
								operand.Real   = strtod(p, &endptr);
								operand.IsQuad = TRUE;
							}
						}
						else
						{
							LPCVOID lpAddress;

							endptr = end;
							lpAddress = (!bCached ? lpConstStringBuffer : lpConstStringRegion) + lpMarkup->UnescapedString;
							if (!(operand.IsQuad = !IsInteger))
								operand.Quad = (uintptr_t)lpAddress;
							else
								operand.Real = (uintptr_t)lpAddress;
							break;
						}
					}
					else
					{
						__int64 n;

						endptr = p + prefixLength + 1;
						if (prefixLength < 1)
							n = UnescapeAnsiCharA(&endptr, end);
						else if (prefixLength == 1)
							n = UnescapeUnicodeCharA(&endptr, end);
						else/* if (prefixLength == 2)*/
							n = UnescapeUtf8CharA(&endptr, end);
						if (endptr < end && *endptr == '\'')
							endptr++;
						if (!(operand.IsQuad = !IsInteger))
							operand.Quad = (uint32_t)n;
						else
							operand.Real = (double)n;
					}
					if (endptr == end)
						break;
					for (i = 0; i < nNumberOfVariable; i++)
					{
						if (lpVariable[i].Length != length)
							continue;
						if (memcmp(lpVariable[i].String, p, length) != 0)
							continue;
						element = lpVariable + i;
						break;
					}
				} while (0);
				if (!element && length && endptr != end && (
#if SCOPE_SUPPORT
					FixTheProcedure && *p == '.' ||
					*p == SCOPE_PREFIX ||
#endif
					lpNext && (lpNext->Tag == TAG_INC ||
							   lpNext->Tag == TAG_DEC ||
							   lpNext->Tag == TAG_ADDRESS_OF)
					))
				{
#if SCOPE_SUPPORT
					map_iterator it = NULL;
					map *variantMap = &SSGS->fields;
					ScopeVariant sv = { { p + 1, p + length, ._M_end_of_storage = p + 1 }, 0, 0 };
#endif
					if (!(nNumberOfVariable & 0x0F))
					{
						LPVOID lpMem;
						size_t nBytes;

						nBytes = (nNumberOfVariable + 0x10) * sizeof(VARIABLE);
						lpMem = HeapReAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, lpVariable, nBytes);
						if (!lpMem)
							goto ALLOC_ERROR;
						lpVariable = (VARIABLE *)lpMem;
					}
					element = lpVariable + nNumberOfVariable++;
					element->Length = length;
					element->String = p;
					element->Value.Quad = 0;
					element->Value.IsQuad = !IsInteger;
#if SCOPE_SUPPORT
					if (SSGS->type && SSGS->folder && FixTheProcedure && *p == '.')
					{
						if (!map_end(variantMap))
						{
							if (!(map_end(variantMap) = node_alloc_allocate(sizeof(bcb6_std_map_node))))
								goto ALLOC_ERROR;
							map_end(variantMap)->_M_color  = FALSE;
							map_end(variantMap)->_M_left   = map_end(variantMap);
							map_end(variantMap)->_M_parent = NULL;
							map_end(variantMap)->_M_right  = map_end(variantMap);
						}
						it = map_lower_bound(variantMap, &sv.Identifier);
						if (it != map_end(variantMap) && string_equals(&((ScopeVariant *)pair_first(it))->Identifier, &sv.Identifier))
						{
							((ScopeVariant *)pair_first(it))->Identifier.sstIndex = 0;
							element->Value.Quad    = ((ScopeVariant *)pair_first(it))->Quad;
							element->Value.IsQuad |= !!element->Value.High;
							element->Node = it;
						}
						else
							goto INSERT;
					}
					else if (attributes && *p == SCOPE_PREFIX)
					{
						const COORD coord = TSSGAttributeElement_GetViaCoord(atSCOPE, attributes).dwFontSize;
						if (coord.X)
						{
							for (TScopeAttribute **base = &vector_type_at(attributes, TScopeAttribute *, coord.Y),
								 **cur  = base + coord.X;
								 --cur >= base; )
							{
								it = map_lower_bound(variantMap = &(*cur)->heapMap, &sv.Identifier);
								if (it != map_end(variantMap) && string_equals(&((ScopeVariant *)pair_first(it))->Identifier, &sv.Identifier))
								{
									((ScopeVariant *)pair_first(it))->Identifier.sstIndex = 0;
									element->Value.Quad    = ((ScopeVariant *)pair_first(it))->Quad;
									element->Value.IsQuad |= !!element->Value.High;
									element->Node = it;
									break;
								}
							}
							if (!element->Node)
							{
							INSERT:
								map_string_quad_insert(&element->Node, variantMap, it, &sv);
								((ScopeVariant *)pair_first(element->Node))->Identifier.sstIndex = 0;
							}
						}
					}
#endif
				}
#if SCOPE_SUPPORT
				else if (element && element->Node && ((ScopeVariant *)pair_first(element->Node))->Identifier.sstIndex)
				{
					((ScopeVariant *)pair_first(element->Node))->Identifier.sstIndex = 0;
					element->Value.Quad   = ((ScopeVariant *)pair_first(element->Node))->Quad;
					element->Value.IsQuad = !IsInteger || element->Value.High;
				}
#endif
				*end = c;
#pragma endregion end
				switch (lpNext ? lpNext->Tag : -1)
				{
				case TAG_ADDRESS_OF:
					if (!element)
						break;
					operand.Quad =
#if SCOPE_SUPPORT
						element->Node && (&lpPostfix[2] >= lpEndOfPostfix || lpPostfix[2]->Tag == TAG_RETURN) ?
						(uint64_t)&((ScopeVariant *)pair_first(element->Node))->Quad :
#endif
						(uint64_t)&element->Value;
#if INTPTR_MAX > INT32_MAX
					operand.IsQuad = TRUE;
					if (!IsInteger)
#else
					if (operand.IsQuad = !IsInteger)
#endif
						operand.Real = (uintptr_t)operand.Quad;
					OPERAND_PUSH(operand);
					lpPostfix++;
					if (!this->ssgActionListner)
						continue;
					lpGuideText = lpNext->String;
#if !defined(__BORLANDC__)
					nGuideTextLength = lpMarkup->String - lpNext->String + lpMarkup->Length;
#endif
					goto OUTPUT_GUIDE;
				case TAG_INC:
					if (!element)
						break;
					operand = element->Value;
					lpGuideText = lpNext->Type & OS_POST ? lpMarkup->String : lpNext->String;
#if !defined(__BORLANDC__)
					nGuideTextLength = lpNext->Length + lpMarkup->Length;
#endif
					if (!(lpNext->Type & OS_POST))
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = ++element->Value.Low;
							else
								operand.Quad = ++element->Value.Quad;
						}
						else
						{
							operand.Real = element->Value.Real += 1;
							operand.IsQuad = TRUE;
						}
						lpMarkup = lpNext;
					}
					else
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = element->Value.Low++;
							else
								operand.Quad = element->Value.Quad++;
						}
						else
						{
							operand.Real = element->Value.Real;
							operand.IsQuad = TRUE;
							element->Value.Real += 1;
						}
					}
					OPERAND_PUSH(operand);
					lpPostfix++;
					if (!this->ssgActionListner)
						continue;
					goto OUTPUT_GUIDE;
				case TAG_DEC:
					if (!element)
						break;
					operand = element->Value;
					lpGuideText = lpNext->Type & OS_POST ? lpMarkup->String : lpNext->String;
#if !defined(__BORLANDC__)
					nGuideTextLength = lpNext->Length + lpMarkup->Length;
#endif
					if (!(lpNext->Type & OS_POST))
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = --element->Value.Low;
							else
								operand.Quad = --element->Value.Quad;
						}
						else
						{
							operand.Real = element->Value.Real -= 1;
							operand.IsQuad = TRUE;
						}
						lpMarkup = lpNext;
					}
					else
					{
						if (IsInteger)
						{
							if (!operand.IsQuad)
								operand.Low = element->Value.Low--;
							else
								operand.Quad = element->Value.Quad--;
						}
						else
						{
							operand.Real = element->Value.Real;
							operand.IsQuad = TRUE;
							element->Value.Real -= 1;
						}
					}
					OPERAND_PUSH(operand);
					lpPostfix++;
					if (!this->ssgActionListner)
						continue;
					goto OUTPUT_GUIDE;
				case TAG_RIGHT_ASSIGN:
				case TAG_LEFT_ASSIGN:
				ASSIGN:
					if (!length)
						break;
					if (!element)
					{
						if (!(nNumberOfVariable & 0x0F))
						{
							LPVOID lpMem;
							size_t nBytes;

							nBytes = (nNumberOfVariable + 0x10) * sizeof(VARIABLE);
							lpMem = HeapReAlloc(hPrivateHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, lpVariable, nBytes);
							if (!lpMem)
								goto ALLOC_ERROR;
							lpVariable = (VARIABLE *)lpMem;
						}
						element = lpVariable + nNumberOfVariable++;
						element->Length = length;
						element->String = p;
					}
					element->Value = *lpOperandTop;
					lpPostfix++;
					if (bCompoundAssign)
					{
						bCompoundAssign = FALSE;
						continue;
					}
					break;
				case TAG_MNAME:
					{
						LPMODULEENTRY32A lpme = TProcessCtrl_GetModuleFromName(&this->processCtrl, &(string) {
							lpMarkup->String, lpMarkup->String + lpMarkup->Length, ._M_end_of_storage = lpMarkup->String
						});
						operand.Quad = lpme ? (uintptr_t)lpme->hModule : 0;
#if INTPTR_MAX > INT32_MAX
						operand.IsQuad = TRUE;
						if (!IsInteger)
#else
						if (operand.IsQuad = !IsInteger)
#endif
							operand.Real = (size_t)operand.Quad;
						OPERAND_PUSH(operand);
					}
					lpPostfix++;
					if (!this->ssgActionListner)
						continue;
					lpGuideText = lpNext->String;
#if !defined(__BORLANDC__)
					nGuideTextLength = lpMarkup->String - lpNext->String + lpMarkup->Length;
#endif
					goto OUTPUT_GUIDE;
				case TAG_PROCEDURE:
					if (lpOperandTop->Quad)
					{
						LPSTR lpProcName;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR;
						if (endptr != end && element)
						{
							endptr = end;
							operand = element->Value;
						}
						if (!IsInteger && endptr == end)
							operand.Quad = (uint64_t)operand.Real;
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.High || !IS_INTRESOURCE(operand.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Quad;
						if (!IsInteger)
							lpOperandTop->Quad = (uintptr_t)lpOperandTop->Real;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpOperandTop->Quad = (uintptr_t)GetExportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpProcName);
						lpMarkup->String[lpMarkup->Length] = c;
#if INTPTR_MAX > INT32_MAX
						lpOperandTop->IsQuad = TRUE;
						if (!IsInteger)
#else
						if (lpOperandTop->IsQuad = !IsInteger)
#endif
							lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					}
					lpPostfix++;
					break;
				case TAG_MODULENAME:
					lpEndOfModuleName = (lpModuleName = lpNext->String) + lpNext->Length;
					c = *lpEndOfModuleName;
					*lpEndOfModuleName = '\0';
					if (this->ssgActionListner)
					{
#if defined(__BORLANDC__)
						if (IsInteger)
							TSSGActionListner_OnParsingProcess(this->ssgActionListner SSGS, lpModuleName, 0);
						else
							TSSGActionListner_OnParsingDoubleProcess(this->ssgActionListner, SSGS, lpModuleName, 0);
#else
						if (IsInteger)
							TSSGActionListner_OnParsingProcess(lpModuleName, lpNext->Length, 0);
						else
							TSSGActionListner_OnParsingDoubleProcess(lpModuleName, lpNext->Length, 0);
#endif
					}
					lpPostfix++;
					lpNext--;
					goto GET_IMPORT_FUNCTION;
				case TAG_IMPORT_FUNCTION:
				case TAG_IMPORT_REFERENCE:
					lpEndOfModuleName = lpModuleName = NULL;
				GET_IMPORT_FUNCTION:
					if (lpOperandTop->Quad)
					{
						LPSTR  lpProcName;
						char   c2;
						LPVOID lpFunction;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR;
						if (endptr != end && element)
						{
							endptr = end;
							operand = element->Value;
						}
						if (!IsInteger && endptr == end)
							operand.Quad = (uint64_t)operand.Real;
						lpProcName =
							!lpMarkup->Length || endptr != end || operand.High || !IS_INTRESOURCE(operand.Low) ?
							lpMarkup->String :
							(LPSTR)operand.Quad;
						if (!IsInteger)
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						c2 = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						lpFunction = GetImportFunction(hProcess, (HMODULE)lpOperandTop->Quad, lpModuleName, lpProcName);
						if (lpFunction && lpNext->Tag == TAG_IMPORT_FUNCTION)
							if (!ReadProcessMemory(hProcess, lpFunction, &lpFunction, sizeof(lpFunction), NULL))
								lpFunction = NULL;
						lpOperandTop->Quad = (uintptr_t)lpFunction;
						lpMarkup->String[lpMarkup->Length] = c2;
#if INTPTR_MAX > INT32_MAX
						lpOperandTop->IsQuad = TRUE;
						if (!IsInteger)
#else
						if (lpOperandTop->IsQuad = !IsInteger)
#endif
							lpOperandTop->Real = (size_t)lpOperandTop->Quad;
					}
					if (lpEndOfModuleName)
						*lpEndOfModuleName = c;
					lpPostfix++;
					break;
				case TAG_SECTION:
					if (lpOperandTop->Quad)
					{
						BOOL  IsEndOfSection;
						DWORD dwSectionSize;

						if (!hProcess && !(hProcess = TProcessCtrl_Open(&this->processCtrl, PROCESS_DESIRED_ACCESS)))
							goto OPEN_ERROR;
						c = lpMarkup->String[lpMarkup->Length];
						lpMarkup->String[lpMarkup->Length] = '\0';
						IsEndOfSection = *(uint16_t *)lpNext->String == BSWAP16(':+');
						if (!IsInteger)
							lpOperandTop->Quad = (uint64_t)lpOperandTop->Real;
						lpOperandTop->Quad = (uintptr_t)GetSectionAddress(hProcess, (HMODULE)lpOperandTop->Quad, lpMarkup->String, IsEndOfSection ? &dwSectionSize : NULL);
						if (*(uintptr_t *)&lpOperandTop->Quad && IsEndOfSection)
							*(uintptr_t *)&lpOperandTop->Quad += dwSectionSize;
#if INTPTR_MAX > INT32_MAX
						lpOperandTop->IsQuad = TRUE;
						if (!IsInteger)
#else
						if (lpOperandTop->IsQuad = !IsInteger)
#endif
							lpOperandTop->Real = (size_t)lpOperandTop->Quad;
						lpMarkup->String[lpMarkup->Length] = c;
					}
					lpPostfix++;
					break;
				case TAG_PROCESSID:
					{
						LPMODULEENTRY32A lpme = TProcessCtrl_GetModuleFromName(&this->processCtrl, &(string) {
							lpMarkup->String, lpMarkup->String + lpMarkup->Length, ._M_end_of_storage = lpMarkup->String
						});
						operand.Quad = lpme ? lpme->th32ProcessID : 0;
						if (operand.IsQuad = !IsInteger)
							operand.Real = operand.Low;
						OPERAND_PUSH(operand);
					}
					lpPostfix++;
					if (!this->ssgActionListner)
						continue;
					lpGuideText = lpNext->String;
#if !defined(__BORLANDC__)
					nGuideTextLength = lpMarkup->String - lpNext->String + lpMarkup->Length;
#endif
					goto OUTPUT_GUIDE;
				case TAG_GOTO:
					OPERAND_CLEAR();
					if (lpMarkup->Jump)
						lpPostfix = lpMarkup->Jump;
					else
					{
						for (lpPostfix = lpPostfixBuffer; lpPostfix < lpEndOfPostfix; lpPostfix++)
							if ((*lpPostfix)->Tag == TAG_LABEL &&
								(*lpPostfix)->Length == lpMarkup->Length &&
								!memcmp((*lpPostfix)->String, lpMarkup->String, lpMarkup->Length))
								break;
						if (lpPostfix < lpEndOfPostfix)
							lpMarkup->Jump = lpPostfix;
						else
							goto PARSING_ERROR;
					}
					if (!this->ssgActionListner)
						continue;
					lpGuideText = lpNext->String;
#if !defined(__BORLANDC__)
					nGuideTextLength = lpMarkup->String - lpNext->String + lpMarkup->Length;
#endif
					goto OUTPUT_GUIDE;
				default:
					if (bCompoundAssign)
						goto ASSIGN;
					if (lpNext && lpNext->Tag != TAG_LEFT_ASSIGN && (lpNext->Type & OS_LEFT_ASSIGN))
						bCompoundAssign = TRUE;
					if (element)
						OPERAND_PUSH(element->Value);
					else if (endptr == end)
						OPERAND_PUSH(operand);
					else
						OPERAND_PUSH(operandZero);
					break;
				}
			}
			break;
		default:
			if (lpMarkup->Type & (OS_TERNARY_END | OS_LOOP_END))
				continue;
			goto PARSING_ERROR;
		}
		if (this->ssgActionListner)
		{
#if defined(__BORLANDC__)
			char c;

			c = (lpGuideText = lpMarkup->String)[lpMarkup->Length];
			lpGuideText[lpMarkup->Length] = '"';
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(this->ssgActionListner, SSGS, lpGuideText, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(this->ssgActionListner, SSGS, lpGuideText, lpOperandTop->Real);
			if (lpGuideText == lpMarkup->String)
				lpGuideText[lpMarkup->Length] = c;
#else
			lpGuideText = lpMarkup->String;
			nGuideTextLength = lpMarkup->Length;
		OUTPUT_GUIDE:
			if (IsInteger)
				TSSGActionListner_OnParsingProcess(lpGuideText, nGuideTextLength, lpOperandTop->Quad);
			else
				TSSGActionListner_OnParsingDoubleProcess(
					lpGuideText,
					nGuideTextLength,
					lpOperandTop->IsQuad ? lpOperandTop->Real : (double)lpOperandTop->Float);
#endif
		}
		if (lpMarkup->Tag != TAG_RETURN)
			continue;
		break;

	PARSING_ERROR_FREE2:
		if (lpBuffer2)
			HeapFree(hHeap, 0, lpBuffer2);
	PARSING_ERROR_FREE1:
		if (lpBuffer1)
			HeapFree(hHeap, 0, lpBuffer1);
	PARSING_ERROR:
		if (this->ssgActionListner)
		{
			lpMarkup->String[lpMarkup->Length] = '\0';
			TSSGActionListner_OnParsingError(this->ssgActionListner, SSGS, lpMarkup->String);
		}
		goto FAILED;

	OPEN_ERROR_FREE1:
		if (lpBuffer1)
			HeapFree(hHeap, 0, lpBuffer1);
	OPEN_ERROR:
		if (this->ssgActionListner &&
			this->script.sPos &&
			TMainForm_GetUserMode(MainForm) >= 3 &&
			FormatMessageA(
				FORMAT_MESSAGE_MAX_WIDTH_MASK |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				0,
				(LPSTR)&lpMessage,
				sizeof(double),
				NULL))
		{
			TMainForm_Guide(lpMessage, 0);
			LocalFree((HLOCAL)lpMessage);
			goto PARSING_ERROR;
		}
		goto FAILED;

	READ_ERROR_FREE2:
		if (lpBuffer2)
			HeapFree(hHeap, 0, lpBuffer2);
	READ_ERROR_FREE1:
		if (lpBuffer1)
			HeapFree(hHeap, 0, lpBuffer1);
	READ_ERROR:
		if (this->ssgActionListner)
			TSSGActionListner_OnSubjectReadError(this->ssgActionListner, SSGS, (uint32_t)lpAddress);
		goto PARSING_ERROR;

	WRITE_ERROR_FREE3:
		if (lpBuffer3)
			HeapFree(hHeap, 0, lpBuffer3);
	WRITE_ERROR_FREE2:
		if (lpBuffer2)
			HeapFree(hHeap, 0, lpBuffer2);
	WRITE_ERROR_FREE1:
		if (lpBuffer1)
			HeapFree(hHeap, 0, lpBuffer1);
	WRITE_ERROR:
		if (this->ssgActionListner)
			TSSGActionListner_OnSubjectWriteError(this->ssgActionListner, SSGS, (uint32_t)lpAddress);
		goto PARSING_ERROR;

	ALLOC_ERROR_FREE2:
		if (lpBuffer2)
			HeapFree(hHeap, 0, lpBuffer2);
	ALLOC_ERROR_FREE1:
		if (lpBuffer1)
			HeapFree(hHeap, 0, lpBuffer1);
	ALLOC_ERROR:
		lpMessage = "メモリの確保に失敗しました。";
		goto GUIDE;

	DIVIDE_BY_ZERO:
		lpMessage = "0 で除算しました。";
		goto GUIDE;

	FAILED_GET_PROCESS_TIMES:
		lpMessage = "プロセスの同一性検証に失敗しました。";
		goto GUIDE;

	FAILED_ADDR_REPLACE:
		lpMessage = "オフセット演算に失敗しました。";
		goto GUIDE;

	FAILED_ADDR_ADJUST:
		lpMessage = "アドレス補正に失敗しました。";
		goto GUIDE;

	GUIDE:
		if (this->ssgActionListner && TMainForm_GetUserMode(MainForm) != 1)
		{
			TMainForm_Guide(lpMessage, 0);
			if (lpEndOfPostfix)
				goto PARSING_ERROR;
		}
		goto FAILED;
	}
	qwResult = lpOperandTop->Quad;
FAILED:
#if SCOPE_SUPPORT
	for (register PVARIABLE v = lpVariable, end = v + nNumberOfVariable; v < end; v++)
		if (v->Node && !((ScopeVariant *)pair_first(v->Node))->Identifier.sstIndex)
		{
			((ScopeVariant *)pair_first(v->Node))->Identifier.sstIndex = MAXDWORD;
			((ScopeVariant *)pair_first(v->Node))->Quad = v->Value.Quad;
		}
#endif
	if (hProcess)
		CloseHandle(hProcess);
	if (lpEndOfPostfix && this->ssgActionListner && TMainForm_GetUserMode(MainForm) < 5)
		TSSGActionListner_OnProcessOpen(this->ssgActionListner, SSGS, 0);
	if ((nNumberOfProcessMemory || nNumberOfHeapBuffer)
		&& this->script.sPos
		&& TMainForm_GetUserMode(MainForm) >= 3
		&& !HeapValidate(hPrivateHeap, HEAP_NO_SERIALIZE, NULL)) {
		CHAR lpText[0x0100];
#if USE_TOOLTIP
		extern BOOL bActive;
		if (!bActive && FormatMessageA(
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			ERROR_SXS_CORRUPTION,
			0,
			lpText,
			_countof(lpText),
			NULL))
			ShowToolTip(lpText, (HICON)TTI_ERROR);
#endif
	}
RELEASE:
	if (lpHeapBuffer)
	{
		size_t i;

		i = nNumberOfHeapBuffer;
		while (i)
			HeapFree(hPrivateHeap, HEAP_NO_SERIALIZE, lpHeapBuffer[--i]);
		HeapFree(hHeap, 0, lpHeapBuffer);
	}
#if REPEAT_INDEX
	if (lpVariableStringBuffer)
		HeapFree(hHeap, 0, lpVariableStringBuffer);
#endif
	if (lpVariable)
		HeapFree(hPrivateHeap, HEAP_NO_SERIALIZE, lpVariable);
	if (lpOperandBuffer)
		HeapFree(hHeap, 0, lpOperandBuffer);
	if (!bCached)
	{
		if (lpConstStringBuffer)
			VirtualFree(lpConstStringBuffer, 0, MEM_RELEASE);
		if (lpPostfixBuffer)
			HeapFree(hHeap, 0, lpPostfixBuffer);
		if (lpMarkupArray)
			HeapFree(hHeap, 0, lpMarkupArray);
		if (lpszSrc)
			HeapFree(hHeap, 0, lpszSrc);
		if (lpszReplace)
			HeapFree(hHeap, 0, lpszReplace);
	}
	return qwResult;

	#undef PROCESS_DESIRED_ACCESS
	#undef SCOPE_PREFIX
	#undef OPERAND_IS_EMPTY
	#undef OPERAND_PUSH
	#undef OPERAND_POP
	#undef OPERAND_CLEAR
}
//---------------------------------------------------------------------
#if defined(__BORLANDC__)
unsigned long TSSGCtrl::Parsing(IN TSSGSubject *SSGS, IN const string &_Src, ...)
{
	#define Src (&_Src)
#else
unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, ...)
{
#endif
	uint64_t Result;
	va_list  ArgPtr;

#if defined(__BORLANDC__)
	va_start(ArgPtr, SSGS);
	va_arg(ArgPtr, LPCVOID);
#else
	va_start(ArgPtr, Src);
#endif
	Result = InternalParsing(this, SSGS, Src, TRUE, ArgPtr);
	va_end(ArgPtr);

	return (unsigned long)Result;

#if defined(__BORLANDC__)
	#undef Src
#endif
}
//---------------------------------------------------------------------
//「文字列Srcを、一旦逆ポーランド記法にしたあと解析する関数（double版）」
//	・四則演算や比較演算のみ(剰余演算子[%]は不可)
//---------------------------------------------------------------------
#if defined(__BORLANDC__)
double TSSGCtrl::ParsingDouble(IN TSSGSubject *SSGS, IN const string &_Src, IN double Val)
{
	#define Src (&_Src)
#else
double __cdecl ParsingDouble(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const string *Src, IN double Val)
{
#endif
	union {
		uint64_t Quad;
		double   Real;
	} Result;
	struct {
		struct {
			size_t Length;
			LPCSTR String;
			double Value;
		} Data;
		size_t Terminator;
	} Param;

	Param.Data.Length = 3;
	Param.Data.String = "Val";
	Param.Data.Value = Val;
	Param.Terminator = 0;
	Result.Quad = InternalParsing(this, SSGS, Src, FALSE, (va_list)&Param);

	return Result.Real;

#if defined(__BORLANDC__)
	#undef Src
#endif
}
//---------------------------------------------------------------------

#undef IMPLEMENTED

#if defined(__BORLANDC__)
#undef _ultoa
#undef string
#undef string_c_str
#undef string_begin
#undef string_end
#undef string_length
#undef vector_TSSGAttributeElement
#undef TMainForm_GetUserMode
#undef TMainForm_Guide
#undef TSSGCtrl_GetAttribute
#undef TSSGCtrl_GetSSGActionListner
#undef TSSGCtrl_AddressAttributeFilter
#undef TSSGSubject_GetAttribute
#undef TSSGSubject_GetSize
#undef TSSGActionListner_OnProcessOpenError
#undef TSSGActionListner_OnSubjectReadError
#undef TSSGActionListner_OnSubjectWriteError
#undef TSSGActionListner_OnParsingError
#undef TSSGActionListner_OnParsingProcess
#undef TSSGActionListner_OnParsingDoubleProcess
#undef TSSGAttributeElement_GetType
#undef TEndWithAttribute_GetCode
#undef TIO_FEPAttribute_GetInputCode
#undef TIO_FEPAttribute_GetOutputCode
#undef TProcessCtrl_Open
#undef TProcessCtrl_GetModuleFromName
#undef TProcessCtrl_GetHeapList
#endif

#undef AllocMarkup

