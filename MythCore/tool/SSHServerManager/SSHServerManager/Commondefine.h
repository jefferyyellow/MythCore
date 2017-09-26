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
	emServerManagerType_None			= 0,		// ������
	emServerManagerType_Stop			= 1,		// ͣ��
	emServerManagerType_Restart			= 2,		// ����
	emServerManagerType_Update			= 3,		// ����
	emServerManagerType_Compile			= 4,		// ����
	emServerManagerType_Monitor			= 5,		// ����
	emServerManagerType_SetTime			= 6,		// �޸�ϵͳʱ��
	emServerManagerType_UpdateDB		= 7,		// �������ݿ�
	emServerManagerType_ChmodScript		= 8,		// �޸Ľű�ִ��Ȩ��
};

enum TmServerState
{
	emServerState_None					= 0,		// ������
	emServerState_Running				= 1,		// ������
	emServerState_Stop					= 2,		// ֹͣ��
};

enum TmBatchOperType
{
	emBatchOperType_None				= 0,		// ������
	emBatchOperType_Stop				= 1,		// ����ͣ��
	emBatchOperType_Restart				= 2,		// ��������
	emBatchOperType_Update				= 3,		// ��������
	emBatchOperType_UpdateDB			= 4,		// �����������ݿ�
	emBatchOperType_ChmodScript			= 5,		// �����޸Ľű�Ȩ��
};

extern char gTime[MAX_PATH];
extern char gRestartCommand[MAX_PATH];
extern char gStopCommand[MAX_PATH];
extern char gMonitor1Command[MAX_PATH];
extern char gMonitor2Command[MAX_PATH];
extern char gUpdateCommand[MAX_PATH];
extern char gUpdatedbCommand[MAX_PATH];
extern char gChmodScriptCommand[MAX_PATH];	// �޸Ľű���ִ������
extern bool gServerType;					// true��ʾ��ʽ��,false��ʾ���Է�,����ȷʵ��ⲻ���ƽ̨�������Ƿ������ɹ�
extern int  gRestartWaitTime;				// �������������ݵȴ�ʱ��
extern int  gStopWaitTime;					// �رշ��������ݵȴ�ʱ��
extern int	gUpdateWaitTime;				// ���·������ȴ�ʱ��
extern int	gUpdateDBWaitTime;				// �������ݿ�ȴ�ʱ��


#define WM_MYUPDATEDATA WM_USER+1					// ����һ����Ϣ������ListView

#endif