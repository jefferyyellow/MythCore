
#pragma  once
//#include <Windows.h>
//#include <WinNT.h>
//enum
//{
//	ITEM_FILE,				// 模板数据表
//	ITEM_DIRTEMP,			// 模板数据的原始模板
//	ITEM_DIRNOTEMP,			// 模板文件夹
//	ITEM_TEMPLATE_DATA,		// 模板数据
//	ITEM_TEMPLATE_TABLE,	// 模板表
//	ITEM_TEMPLATE_FILTER,	// 模板过滤器
//	ITEM_MAX_NUM			// 定义的最大数量
//};


enum TmTempItemType  
{
	TEMP_ITEM_NULL,				// 无
	TEMP_ITEM_DIR,				// 模板文件夹节点，纯粹的文件夹，里面没有数据
	TEMP_ITEM_FILTER,			// 模板过滤器节点
	TEMP_ITEM_ORIGIN,			// 原始模板文件节点，这个节点表示其链接着一个原始模板数据
	TEMP_ITEM_TABLE,			// 模板表
	TEMP_ITEM_DATA,				// 模板数据
	TEMP_ITEM_MAX_NUM
};

enum TmTempLoadState
{
	emLoadState_Null,		// 没初始化
	emLoadState_InitHead,	// 只初始化头
	emLoadState_Initing,	// 正在进行完全初始化
	emLoadState_Inited,		// 已经完全初始化
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
	TRACE("\n********name: 一共运行了%fms********\n", d); \
}