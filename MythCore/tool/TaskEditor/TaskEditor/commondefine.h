#pragma once
#include <xstring>
using namespace std;
#define STRING_LENGTH_1024 1024
extern wstring s2ws(const string& s);
extern string ws2s(const wstring& ws);

extern	char*		UnicodeToAnsi(const wchar_t* pSrc, char* pDst, int nMaxSize);
extern	char*		UnicodeToUtf8(wchar_t* pSrc, char* pDst, int nMaxSize);

extern	wchar_t*	Utf8ToUnicode(const char* pSrc, wchar_t* pDst, int nMaxSize);
extern	char*		Utf8ToAnsi(char* pSrc, char* pDst, int nMaxSize);

extern	wchar_t*	AnsiToUnicode(char*pSrc, wchar_t* pDst, int nMaxSize);
extern	char*		AnsiToUtf8(char* pSrc, char* pDst, int nMaxSize);
