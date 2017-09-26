#include "StdAfx.h"
#include "ServerInfo.h"

CServerInfo::CServerInfo(void)
{
	mMonitorIndex = -1;
	mServerState = emServerState_None;
}

CServerInfo::~CServerInfo(void)
{
	mMonitorIndex = -1;
}
