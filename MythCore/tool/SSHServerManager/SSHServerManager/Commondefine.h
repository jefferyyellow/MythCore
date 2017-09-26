#ifndef __COMMONDEFINE_H__
#define __COMMONDEFINE_H__
#include <stdio.h>
#include "tiny/tinyxml.h"
#ifdef	WIN32
	#include <Windows.h>
	typedef DWORD pthread_t;
#else
	#include <pthread.h>
	#include <unistd.h>
#endif

extern wstring s2ws(const string& s);
extern string ws2s(const wstring& ws);

enum TmServerManagerType
{
	emServerManagerType_None			= 0,		// 无类型
	emServerManagerType_Stop			= 1,		// 停服
	emServerManagerType_Restart			= 2,		// 重启
	emServerManagerType_Update			= 3,		// 更新
	emServerManagerType_Compile			= 4,		// 编译
	emServerManagerType_Monitor			= 5,		// 监视
	emServerManagerType_SetTime			= 6,		// 修改系统时间
	emServerManagerType_UpdateDB		= 7,		// 更新数据库
	emServerManagerType_ChmodScript		= 8,		// 修改脚本执行权限
};

enum TmServerState
{
	emServerState_None					= 0,		// 无类型
	emServerState_Running				= 1,		// 运行中
	emServerState_Stop					= 2,		// 停止中
};

enum TmBatchOperType
{
	emBatchOperType_None				= 0,		// 无类型
	emBatchOperType_Stop				= 1,		// 批量停服
	emBatchOperType_Restart				= 2,		// 批量重启
	emBatchOperType_Update				= 3,		// 批量更新
	emBatchOperType_UpdateDB			= 4,		// 批量更新数据库
	emBatchOperType_ChmodScript			= 5,		// 批量修改脚本权限
};

extern char gTime[MAX_PATH];
extern char gRestartCommand[MAX_PATH];
extern char gStopCommand[MAX_PATH];
extern char gMonitor1Command[MAX_PATH];
extern char gMonitor2Command[MAX_PATH];
extern char gUpdateCommand[MAX_PATH];
extern char gUpdatedbCommand[MAX_PATH];
extern char gChmodScriptCommand[MAX_PATH];	// 修改脚本可执行命令
extern bool gServerType;					// true表示正式服,false表示测试服,用来确实检测不检测平台服务器是否启动成功
extern int  gRestartWaitTime;				// 重启服务器数据等待时间
extern int  gStopWaitTime;					// 关闭服务器数据等待时间
extern int	gUpdateWaitTime;				// 更新服务器等待时间
extern int	gUpdateDBWaitTime;				// 更新数据库等待时间


#define WM_MYUPDATEDATA WM_USER+1					// 定义一个消息来更新ListView

#endif