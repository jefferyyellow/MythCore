#include "stdafx.h"
#include "CommonDefine.h"
// 多字节转宽字符，这个函数效率很低，高效率函数不同调用
wstring s2ws(const string& s)
{
	string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs"); 

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;

	TCHAR acDest[MAX_PATH] = {0};

	mbstowcs(acDest,_Source,_Dsize);
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
	char acDest[MAX_PATH * 2] = {0};

	wcstombs(acDest, _Source, _Dsize);
	string result = acDest;


	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

// 从一个字符串中截取第一个单词,过滤前面的空间和TAB键
string GetFirstWord(string& strSource)
{
	int nLength = (int)strSource.length();
	if (nLength <= 0)
	{
		return "";
	}

	int nBeginIndex = nLength;
	for (int i = 0; i < nLength; i++)
	{
		// 如果不是空格且不是TAB键
		if(strSource[i] != 32 && strSource[i] != 9)
		{
			nBeginIndex = i;
			break;
		}
	}

	int nEndIndex = nLength;
	for (int i = nBeginIndex; i < nLength; i++)
	{
		// 如果是空格或者TAB键
		if(strSource[i] == 32 || strSource[i] == 9)
		{
			nEndIndex = i;
			break;
		}
	}

	// 将字符串取出来
	string strResult = strSource.substr(nBeginIndex, nEndIndex - nBeginIndex);
	// 截取原字符串
	if (nEndIndex >= nLength)
	{
		strSource = "";
	}
	else
	{
		strSource = strSource.substr(nEndIndex + 1);
	}
	return strResult;
}