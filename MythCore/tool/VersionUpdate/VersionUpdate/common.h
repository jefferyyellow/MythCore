
#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
using namespace std;
#ifdef WIN32
typedef __int64				int64;
typedef __int32				int32;
typedef __int16				int16;
typedef __int8				int8;
typedef unsigned __int64	uint64;
typedef unsigned __int32	uint32;
typedef unsigned __int16	uint16;
typedef unsigned __int8		uint8;
#else
typedef __int64_t   int64;
typedef __int32_t   int32;
typedef __int16_t   int16;
typedef __int8_t    int8;
typedef __uint64_t  uint64;
typedef __uint32_t  uint32;
typedef __uint16_t  uint16;
typedef __uint8_t   uint8;
#endif

extern wstring s2ws(const string& s);
extern string ws2s(const wstring& ws);
#endif