#include "stdafx.h"
#include "Commondefine.h"

char gTime[MAX_PATH] = {0};

char gRestartCommand[MAX_PATH] = {0};
char gStopCommand[MAX_PATH] = {0};
char gMonitor1Command[MAX_PATH] = {0};
char gMonitor2Command[MAX_PATH] = {0};
char gUpdateCommand[MAX_PATH] = {0};
char gUpdatedbCommand[MAX_PATH] = {0};
char gChmodScriptCommand[MAX_PATH] = {0};	
bool gServerType = true;
int  gRestartWaitTime = 0;
int  gStopWaitTime = 0;
int	 gUpdateWaitTime = 0;
int	 gUpdateDBWaitTime = 0;


// ���ֽ�ת���ַ����������Ч�ʺܵͣ���Ч�ʺ�����ͬ����
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
// ���ַ�ת���ֽڣ��������Ч�ʺܵͣ���Ч�ʺ�����ͬ����
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
