#ifndef DIRTYWORD_DIRTYWORD_SWEXCHANGE_H_
#define DIRTYWORD_DIRTYWORD_SWEXCHANGE_H_

#include <Windows.h>
#include <string>
#pragma warning(disable:4996)

namespace std
{

    // ���ֽ�ת���ַ����������Ч�ʺܵͣ���Ч�ʺ�����ͬ����
    wstring s2ws(const string& s);

    // ���ַ�ת���ֽڣ��������Ч�ʺܵͣ���Ч�ʺ�����ͬ����
    string ws2s(const wstring& ws);
}

#endif