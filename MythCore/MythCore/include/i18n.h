#ifndef __I18N_H__
#define __I18N_H__
#include "commontype.h"

namespace Myth
{
	class CI18N
	{
	public:
		static	char*		UnicodeToAnsi(wchar_t* pSrc, char* pDst, int nMaxSize);
		static	char*		UnicodeToUtf8(wchar_t* pSrc, char* pDst, int nMaxSize);

		static	wchar_t*	Utf8ToUnicode(char* pSrc, wchar_t* pDst, int nMaxSize);
		static	char*		Utf8ToAnsi(char* pSrc, char* pDst, int nMaxSize);

		static	wchar_t*	AnsiToUnicode(char*pSrc, wchar_t* pDst, int nMaxSize);
		static	char*		AnsiToUtf8(char* pSrc, char* pDst, int nMaxSize);

	private:
#ifdef MYTH_OS_UNIX
		static	int			CharsetConvert(const char* pSrcCharset, const char* pDstCharset, char* pSrc, size_t nSrcLen, char* pDst, size_t nDstLen);
#endif
	};
}

#endif