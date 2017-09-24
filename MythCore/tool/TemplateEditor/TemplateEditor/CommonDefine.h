
#pragma  once
//#include <Windows.h>
//#include <WinNT.h>
//enum
//{
//	ITEM_FILE,				// ģ�����ݱ�
//	ITEM_DIRTEMP,			// ģ�����ݵ�ԭʼģ��
//	ITEM_DIRNOTEMP,			// ģ���ļ���
//	ITEM_TEMPLATE_DATA,		// ģ������
//	ITEM_TEMPLATE_TABLE,	// ģ���
//	ITEM_TEMPLATE_FILTER,	// ģ�������
//	ITEM_MAX_NUM			// ������������
//};


enum TmTempItemType  
{
	TEMP_ITEM_NULL,				// ��
	TEMP_ITEM_DIR,				// ģ���ļ��нڵ㣬������ļ��У�����û������
	TEMP_ITEM_FILTER,			// ģ��������ڵ�
	TEMP_ITEM_ORIGIN,			// ԭʼģ���ļ��ڵ㣬����ڵ��ʾ��������һ��ԭʼģ������
	TEMP_ITEM_TABLE,			// ģ���
	TEMP_ITEM_DATA,				// ģ������
	TEMP_ITEM_MAX_NUM
};

enum TmTempLoadState
{
	emLoadState_Null,		// û��ʼ��
	emLoadState_InitHead,	// ֻ��ʼ��ͷ
	emLoadState_Initing,	// ���ڽ�����ȫ��ʼ��
	emLoadState_Inited,		// �Ѿ���ȫ��ʼ��
	emLoadState_Max
};

extern wstring s2ws(const string& s);
extern string ws2s(const wstring& ws);
extern string GetFirstWord(string& strSource);

#define PERFORMACE(name, code) \
{ \
	LARGE_INTEGER  frequency,start,end; \
	QueryPerformanceFrequency(&frequency); \
	QueryPerformanceCounter(&start);	\
	code; \
	QueryPerformanceCounter(&end); \
	double d   =   (double)(end.QuadPart   -   start.QuadPart)   /   (double)frequency.QuadPart   *   1000.0; \
	TRACE("\n********name: һ��������%fms********\n", d); \
}