#include "stdafx.h"
#include "commondefine.h"
// 多字节转宽字符，这个函数效率很低，高效率函数不同调用
wstring s2ws(const string& s)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;

	TCHAR acDest[MAX_PATH] = { 0 };

	mbstowcs(acDest, _Source, _Dsize);
	wstring result = acDest;
	setlocale(LC_ALL, curLocale.c_str());

	return result;
}
// 宽字符转多字节，这个函数效率很低，高效率函数不同调用
string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = ws.size() * 2 + 1;
	char acDest[MAX_PATH * 2] = { 0 };

	wcstombs(acDest, _Source, _Dsize);
	string result = acDest;


	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

char* UnicodeToAnsi(const wchar_t* pSrc, char* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}

	if (0 == WideCharToMultiByte(CP_ACP, 0, pSrc, -1, pDst, nMaxSize - 1, NULL, 0))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] = 0;
	return pDst;

}

char* UnicodeToUtf8(wchar_t* pSrc, char* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}

	if (0 == WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, pDst, nMaxSize - 1, NULL, 0))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] = 0;
	return pDst;

}

wchar_t* Utf8ToUnicode(const char* pSrc, wchar_t* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
	if (0 == MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, pDst, nMaxSize - 1))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] = 0;
	return pDst;
}

char* Utf8ToAnsi(char* pSrc, char* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}

	wchar_t tBuffer[STRING_LENGTH_1024] = { 0 };
	if (0 == MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, tBuffer, STRING_LENGTH_1024 - 1))
	{
		return NULL;
	}
	tBuffer[STRING_LENGTH_1024 - 1] = 0;

	if (0 == WideCharToMultiByte(CP_ACP, 0, tBuffer, -1, pDst, nMaxSize - 1, NULL, 0))
	{
		return NULL;
	}

	pDst[nMaxSize - 1] = 0;
	return pDst;
}

wchar_t* AnsiToUnicode(char*pSrc, wchar_t* pDst, int nMaxSize)
{
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
	if (0 == MultiByteToWideChar(CP_ACP, 0, pSrc, -1, pDst, nMaxSize - 1))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] = 0;
	return pDst;

}

char* AnsiToUtf8(char* pSrc, char* pDst, int nMaxSize)
{
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
	wchar_t tBuffer[STRING_LENGTH_1024] = { 0 };
	if (0 == MultiByteToWideChar(CP_ACP, 0, pSrc, -1, tBuffer, STRING_LENGTH_1024 - 1))
	{
		return NULL;
	}
	tBuffer[STRING_LENGTH_1024 - 1] = 0;

	if (0 == WideCharToMultiByte(CP_UTF8, 0, tBuffer, -1, pDst, nMaxSize - 1, NULL, 0))
	{
		return NULL;
	}

	pDst[nMaxSize - 1] = 0;
	return pDst;

}