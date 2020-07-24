#include "i18n.h"

#ifdef MYTH_OS_UNIX
#include <iconv.h>
#include <wchar.h>
#endif

namespace Myth
{
	#ifdef MYTH_OS_UNIX
	/// charset one convert to another
	int CI18N::CharsetConvert(const char* pSrcCharset, const char* pDestCharset, char* pSrc, size_t nSrcLen, char* pDest, size_t nDestLen)
	{  
		int nOldDestLen = nDestLen;
		if (NULL == pSrcCharset || NULL == pDestCharset)
		{
			return -1;
		}

		if (NULL == pSrc || nSrcLen <= 0 || NULL == pDest || nDestLen <= 0)
		{
			return -1;
		}

		iconv_t cd = 0; 
		cd = iconv_open(pDestCharset, pSrcCharset);  
		if(cd==0)  
		{
			return -1;
		}

		size_t nResult = iconv(cd, &pSrc, &nSrcLen, &pDest, &nDestLen);
		if(size_t(-1) == nResult) 
		{
			iconv_close(cd);
			return -1;  
		}
		iconv_close(cd);
		return nOldDestLen - nDestLen;  
	}

#endif

	char* CI18N::UnicodeToAnsi(const wchar_t* pSrc, char* pDest, int nMaxSize)
	{
		// check params
		if (NULL == pSrc || NULL == pDest || nMaxSize <= 0)
		{
			return NULL;
		}
#ifdef MYTH_OS_WINDOWS
		if (0 == WideCharToMultiByte(CP_ACP, 0, pSrc, -1, pDest, nMaxSize - 1, NULL, 0))
		{
			return NULL;
		}
		pDest[nMaxSize - 1] = 0;
		return pDest;
#else
		int nSize = CharsetConvert("unicode", "gbk", (char*)pSrc, strlen((char*)pSrc) << 1, pDest, nMaxSize - 1);
		if (0 >= nSize)
		{
			return NULL;
		}
		pDest[nSize] = 0;
		return pDest;
#endif

	}

	char* CI18N::UnicodeToUtf8(const wchar_t* pSrc, char* pDest, int nMaxSize)
	{
		// check params
		if (NULL == pSrc || NULL == pDest || nMaxSize <= 0)
		{
			return NULL;
		}

#ifdef MYTH_OS_WINDOWS
		if (0 == WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, pDest, nMaxSize - 1, NULL, 0))
		{
			return NULL;
		}
		pDest[nMaxSize - 1] = 0;
		return pDest;
#else
		int nSize = CharsetConvert("unicode", "utf8", (char*)pSrc, strlen((char*)pSrc), pDest, nMaxSize - 1);
		if (0 >= nSize)
		{
			return NULL;
		}
		pDest[nSize] = 0;
		return pDest;
#endif
	}

	wchar_t* CI18N::Utf8ToUnicode(const char* pSrc, wchar_t* pDest, int nMaxSize)
	{
		// check params
		if (NULL == pSrc || NULL == pDest || nMaxSize <= 0)
		{
			return NULL;
		}

#ifdef MYTH_OS_WINDOWS
		if (0 == MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, pDest, nMaxSize - 1))
		{
			return NULL;
		}
		pDest[nMaxSize - 1] = 0;
		return pDest;
#else
		int nSize = CharsetConvert("utf8", "unicode", const_cast<char*>(pSrc), strlen(pSrc), (char*)pDest, (nMaxSize - 1) << 1);
		if (0 >= nSize)
		{
			return NULL;
		}
		pDest[nSize] = 0;
		return pDest;
#endif
	}

	char* CI18N::Utf8ToAnsi(const char* pSrc, char* pDest, int nMaxSize)
	{
		// check params
		if (NULL == pSrc || NULL == pDest || nMaxSize <= 0)
		{
			return NULL;
		}
#ifdef MYTH_OS_WINDOWS

		wchar_t tBuffer[STR_LENGTH_1024] = { 0 };
		if (0 == MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, tBuffer, STR_LENGTH_1024 - 1))
		{
			return NULL;
		}
		tBuffer[STR_LENGTH_1024 - 1] = 0;

		if (0 == WideCharToMultiByte(CP_ACP, 0, tBuffer, -1, pDest, nMaxSize - 1, NULL, 0))
		{
			return NULL;
		}

		pDest[nMaxSize - 1] = 0;
		return pDest;
#else
		int nSize = CharsetConvert("UTF8", "GBK", const_cast<char*>(pSrc), strlen(pSrc), pDest, nMaxSize - 1);
		if (0 >= nSize)
		{
			return NULL;
		}
		pDest[nSize] = 0;
		return pDest;
#endif
	}

	wchar_t* CI18N::AnsiToUnicode(const char*pSrc, wchar_t* pDest, int nMaxSize)
	{
		if (NULL == pSrc || NULL == pDest || nMaxSize <= 0)
		{
			return NULL;
		}
#ifdef MYTH_OS_WINDOWS
		if (0 == MultiByteToWideChar(CP_ACP, 0, pSrc, -1, pDest, nMaxSize - 1))
		{
			return NULL;
		}
		pDest[nMaxSize - 1] = 0;
		return pDest;
#else
		int nSize = CharsetConvert("gbk", "unicode", const_cast<char*>(pSrc), strlen(pSrc), (char*)pDest, (nMaxSize - 1) << 1);
		if (0 >= nSize)
		{
			return NULL;
		}
		pDest[nSize] = 0;
		return pDest;
#endif
	}

	char* CI18N::AnsiToUtf8(const char* pSrc, char* pDest, int nMaxSize)
	{
		if (NULL == pSrc || NULL == pDest || nMaxSize <= 0)
		{
			return NULL;
		}
#ifdef MYTH_OS_WINDOWS

		wchar_t tBuffer[STR_LENGTH_1024] = { 0 };
		if (0 == MultiByteToWideChar(CP_ACP, 0, pSrc, -1, tBuffer, STR_LENGTH_1024 - 1))
		{
			return NULL;
		}
		tBuffer[STR_LENGTH_1024 - 1] = 0;

		if (0 == WideCharToMultiByte(CP_UTF8, 0, tBuffer, -1, pDest, nMaxSize - 1, NULL, 0))
		{
			return NULL;
		}

		pDest[nMaxSize - 1] = 0;
		return pDest;
#else
		int nSize = CharsetConvert("GBK", "UTF8", const_cast<char*>(pSrc), strlen(pSrc), pDest, nMaxSize - 1);
		if (0 >= nSize)
		{
			return NULL;
		}
		pDest[nSize] = 0;
		return pDest;
#endif
	}
}
