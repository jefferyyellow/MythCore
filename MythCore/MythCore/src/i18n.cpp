#include "i18n.h"

#ifdef MYTH_OS_UNIX
#include <iconv.h>
#include <wchar.h>
/// charset one convert to another
int CI18N::CharsetConvert(const char* pSrcCharset, const char* pDstCharset, char* pSrc, size_t nSrcLen, char* pDst, size_t nDstLen)
{  
	int nOldDstLen = nDstLen;
	if (NULL == pSrcCharset || NULL == pDstCharset)
	{
		return -1;
	}

	if (NULL == pSrc || nSrcLen <= 0 || NULL == pDst || nDstLen <= 0)
	{
		return -1;
	}

	iconv_t cd = 0; 
	cd = iconv_open(pDstCharset, pSrcCharset);  
	if(cd==0)  
	{
		return -1;
	}
	
	size_t nResult = iconv(cd, &pSrc, &nSrcLen, &pDst, &nDstLen);
	if(size_t(-1) == nResult) 
	{
		iconv_close(cd);
		return -1;  
	}
	iconv_close(cd);
	return nOldDstLen - nDstLen;  
}

#endif

char* CI18N::UnicodeToAnsi(wchar_t* pSrc, char* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
#ifdef MYTH_OS_WINDOWS
	if (0 == WideCharToMultiByte(CP_ACP, 0, pSrc, -1, pDst, nMaxSize - 1, NULL, 0))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] =0;
	return pDst;
#else
	int nSize = CharsetConvert("unicode", "gbk", (char*)pSrc, strlen((char*)pSrc) << 1, pDst, nMaxSize - 1);
	if(0 >= nSize)
	{
		return NULL;
	}
	pDst[nSize] = 0;
	return pDst;
#endif

}

char* CI18N::UnicodeToUtf8(wchar_t* pSrc, char* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}

#ifdef MYTH_OS_WINDOWS
	if (0 == WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, pDst, nMaxSize - 1, NULL, 0))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] = 0;
	return pDst;
#else
	int nSize = CharsetConvert("unicode", "utf8", (char*)pSrc, strlen((char*)pSrc), pDst, nMaxSize - 1);
	if(0 >= nSize)
	{
		return NULL;
	}
	pDst[nSize] = 0;
	return pDst;
#endif
}

wchar_t* CI18N::Utf8ToUnicode(char* pSrc, wchar_t* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}

#ifdef MYTH_OS_WINDOWS
	if (0 == MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, pDst, nMaxSize - 1))
	{
		return NULL;
	}
	pDst[nMaxSize - 1]= 0;
	return pDst;
#else
	int nSize = CharsetConvert("utf8", "unicode", pSrc, strlen(pSrc), (char*)pDst, (nMaxSize -1) << 1);
	if(0 >= nSize)
	{
		return NULL;
	}
	pDst[nSize] = 0;
	return pDst;
#endif
}

char* CI18N::Utf8ToAnsi(char* pSrc, char* pDst, int nMaxSize)
{
	// check params
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
#ifdef MYTH_OS_WINDOWS

	wchar_t tBuffer[STRING_LENGTH_1024] = {0};
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
#else
	int nSize = CharsetConvert("UTF8", "GBK", pSrc, strlen(pSrc), pDst, nMaxSize -1);
	if(0 >= nSize)
	{
		return NULL;
	}
	pDst[nSize] = 0;
	return pDst;
#endif
}

wchar_t* CI18N::AnsiToUnicode(char*pSrc, wchar_t* pDst, int nMaxSize)
{
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
#ifdef MYTH_OS_WINDOWS
	if(0 == MultiByteToWideChar(CP_ACP, 0, pSrc, -1, pDst, nMaxSize -1))
	{
		return NULL;
	}
	pDst[nMaxSize - 1] = 0;
	return pDst;
#else
	int nSize = CharsetConvert("gbk", "unicode", pSrc, strlen(pSrc), (char*)pDst, (nMaxSize -1) << 1);
	if(0 >= nSize)
	{
		return NULL;
	}
	pDst[nSize] = 0;
	return pDst;
#endif
}

char* CI18N::AnsiToUtf8(char* pSrc, char* pDst, int nMaxSize)
{
	if (NULL == pSrc || NULL == pDst || nMaxSize <= 0)
	{
		return NULL;
	}
#ifdef MYTH_OS_WINDOWS

	wchar_t tBuffer[STRING_LENGTH_1024] = {0};
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
#else
	int nSize = CharsetConvert("GBK", "UTF8", pSrc, strlen(pSrc), pDst, nMaxSize -1);
	if(0 >= nSize)
	{
		return NULL;
	}
	pDst[nSize] = 0;
	return pDst;
#endif
}
