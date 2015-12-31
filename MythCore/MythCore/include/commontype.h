#ifndef __COMMONTYPE_H__
#define __COMMONTYPE_H__
#include <string.h>

#ifdef MYTH_OS_WINDOWS
#include <windows.h>
#include <time.h>
#endif

#ifdef MYTH_OS_WINDOWS
// windows has not snprintf,it has _snprintf
#define	snprintf _snprintf
#endif
#include <stddef.h>
#ifdef MYTH_OS_WINDOWS

typedef	signed		__int8		sint8;
typedef	unsigned	__int8		uint8;
typedef	signed		__int16		sint16;
typedef	unsigned	__int16		uint16;
typedef	signed		__int32		sint32;
typedef	unsigned	__int32		uint32;
typedef	signed		__int64		sint64;
typedef	unsigned	__int64		uint64;

typedef				int			sint;			// at least 32bits (depend of processor)
typedef	unsigned	int			uint;			// at least 32bits (depend of processor)

#elif defined (MYTH_OS_UNIX)

#include <sys/types.h>
#include <stdint.h>
#include <climits>

typedef	int8_t		sint8;
typedef	uint8_t		uint8;
typedef	int16_t		sint16;
typedef	uint16_t	uint16;
typedef	int32_t		sint32;
typedef	uint32_t	uint32;
typedef	int64_t		sint64;
typedef	uint64_t	uint64;

typedef				int			sint;			// at least 32bits (depend of processor)
typedef	unsigned	int			uint;			// at least 32bits (depend of processor)
#endif // MYTH_OS_UNIX

#define STRING_LENGTH_8		8			/// string length 8 byte
#define STRING_LENGTH_16	16			/// string length 16 byte
#define STRING_LENGTH_32	32			/// string length 32 byte
#define STRING_LENGTH_64	64			/// string length 64 byte
#define STRING_LENGTH_128	128			/// string length 128 byte
#define STRING_LENGTH_256	256			/// string length 256 byte
#define STRING_LENGTH_1024	1024		/// string length 1024 byte

#define	MAX_NUM_LOG_DISPLAYER	4			/// max displayer per log
#define MAX_NUM_DEBUG_LOG	16			/// max debug log
#endif
