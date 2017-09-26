#ifndef DIRTYWORD_DIRTYWORD_SWEXCHANGE_H_
#define DIRTYWORD_DIRTYWORD_SWEXCHANGE_H_

#include <Windows.h>
#include <string>
#pragma warning(disable:4996)

namespace std
{

    // 多字节转宽字符，这个函数效率很低，高效率函数不同调用
    wstring s2ws(const string& s);

    // 宽字符转多字节，这个函数效率很低，高效率函数不同调用
    string ws2s(const wstring& ws);
}

#endif