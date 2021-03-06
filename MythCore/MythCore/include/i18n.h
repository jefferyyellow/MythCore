#ifndef __I18N_H__
#define __I18N_H__
#include "commontype.h"

namespace Myth
{
	class CI18N
	{
	public:
		static	char*		UnicodeToAnsi(const wchar_t* pSrc, char* pDest, int nMaxSize);
		static	char*		UnicodeToUtf8(const wchar_t* pSrc, char* pDest, int nMaxSize);

		static	wchar_t*	Utf8ToUnicode(const char* pSrc, wchar_t* pDest, int nMaxSize);
		static	char*		Utf8ToAnsi(const char* pSrc, char* pDest, int nMaxSize);

		static	wchar_t*	AnsiToUnicode(const char*pSrc, wchar_t* pDest, int nMaxSize);
		static	char*		AnsiToUtf8(const char* pSrc, char* pDest, int nMaxSize);

	private:
#ifdef MYTH_OS_UNIX
		static	int			CharsetConvert(const char* pSrcCharset, const char* pDestCharset, char* pSrc, size_t nSrcLen, char* pDest, size_t nDestLen);
#endif
	};
}

#endif