#ifndef __GAMESERVERCONFIG_H__
#define __GAMESERVERCONFIG_H__
#include "servercommon.h"
#include "singleton.h"
using namespace Myth;

class CGameServerConfig : public CSingleton<CGameServerConfig>
{
	friend class CSingleton<CGameServerConfig>;
private:
	CGameServerConfig(){}
	~CGameServerConfig(){}

public:
	bool			loadGameServerConfigFromXml(const char* pFilePath);

public:
	char*			getDBHost(){return mDBHost;}
	char*			getDBUserName(){return mDBUserName;}
	char*			getDBPasswd(){return mDBPasswd;}
	char*			getDefaultDataBase(){return mDefaultDataBase;}
	short			getDBPort(){return mDBPort;}

private:
	/// ע�⣬Host��ֻ����IP��������������
	char			mDBHost[STR_LENGTH_64];
	/// �û���
	char			mDBUserName[STR_LENGTH_16];
	/// ����
	char			mDBPasswd[STR_LENGTH_16];
	/// ���ӵ����ݿ���
	char			mDefaultDataBase[STR_LENGTH_16];
	/// �˿�
	short			mDBPort;
};
#endif