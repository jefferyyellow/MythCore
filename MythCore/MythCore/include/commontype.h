#ifndef __COMMONTYPE_H__
#define __COMMONTYPE_H__
#include <string>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#ifdef MYTH_OS_WINDOWS
// 如果定义了模型fd_set最大值，先清除，再重新定义
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif // _DEBUG
/// 将windows select 模型fd_set最大值改成1024
#define FD_SETSIZE  1024
#include <windows.h>
#include <time.h>
#include <tchar.h>
#else
#include <stdarg.h>
#include <sys/types.h>
#endif

#ifdef MYTH_OS_WINDOWS
// windows has not snprintf,it has _snprintf
#define	snprintf _snprintf
#endif
#include <stddef.h>
#ifdef MYTH_OS_WINDOWS
typedef unsigned char			byte;
typedef	signed		__int64		sint64;
typedef	unsigned	__int64		uint64;
typedef unsigned int			uint;
#elif defined (MYTH_OS_UNIX)

#include <sys/types.h>
#include <stdint.h>
#include <climits>
typedef unsigned char			byte;
typedef	int64_t					sint64;
typedef	uint64_t				uint64;
typedef unsigned int			uint;
#endif // MYTH_OS_UNIX

#define STR_LENGTH_8		8			/// string length 8 byte
#define STR_LENGTH_16		16			/// string length 16 byte
#define STR_LENGTH_32		32			/// string length 32 byte
#define STR_LENGTH_64		64			/// string length 64 byte
#define STR_LENGTH_128		128			/// string length 128 byte
#define STR_LENGTH_256		256			/// string length 256 byte
#define STR_LENGTH_1024		1024		/// string length 1024 byte

#define	MAX_NUM_LOG_DISPLAYER	4		/// max displayer per log
#define MAX_NUM_DEBUG_LOG	16			/// max debug log

#define CHAR_BIT_NUM		8			// char的位数目
#define INT_BIT_NUM			32			// (sizeof(int) * 8) int的位数目
// 32 = 2^5  ->   INT_BIT_NUM = 2 ^ INT_BIT_LEN_SHIFT
#define	INT_BIT_LEN_SHIFT	5			// int需要的位移数目
#define BIT_SET_LEN(length) ((length+INT_BIT_NUM-1)/INT_BIT_NUM)	// 计算length长度的bit需要多少个int
#define INT_ALL_BIT_SET		0xFFFFFFFF	// int所有的位都是置位状态
#endif
