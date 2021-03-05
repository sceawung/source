#ifndef _DATETIME_H_
#define _DATETIME_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define __DATE_YEAR__         1970
#define __DATE_MONTH__        1
#define __DATE_DAY__          1
#define __TIME_HOUR__         0
#define __TIME_MINUTE__       0
#define __TIME_SECOND__       0
#define __TIME_MILLISECOND__  0

#define __DATE_YYYY__         1970
#define __DATE_MM__           01
#define __DATE_DD__           01
#define __TIME_HH__           00
#define __TIME_MM__           00
#define __TIME_SS__           00
#define __TIME_FFF__          000

#define __DATETIME_TIME_T__   0x0000000000000000
#define __DATETIME_FILETIME__ 0x019DB1DED53E8000

#endif	// _DATETIME_H_
