#pragma once
#include <xstring>
#include <map>
using namespace std;
#include "tinyxml2.h"
using namespace tinyxml2;


#define STRING_LENGTH_1024 1024
#define MAX_COND_PARAM_NUM 4			// 最大的条件参数
#define MAX_DIAG_PARAM_NUM 2			// 最大的对话参数
extern wstring s2ws(const string& s);
extern string ws2s(const wstring& ws);

extern	char*		UnicodeToAnsi(const wchar_t* pSrc, char* pDst, int nMaxSize);
extern	char*		UnicodeToUtf8(const wchar_t* pSrc, char* pDst, int nMaxSize);

extern	wchar_t*	Utf8ToUnicode(const char* pSrc, wchar_t* pDst, int nMaxSize);
extern	char*		Utf8ToAnsi(const char* pSrc, char* pDst, int nMaxSize);

extern	wchar_t*	AnsiToUnicode(const char*pSrc, wchar_t* pDst, int nMaxSize);
extern	char*		AnsiToUtf8(const char* pSrc, char* pDst, int nMaxSize);
