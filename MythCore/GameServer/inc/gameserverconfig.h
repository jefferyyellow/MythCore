#ifndef __GAMESERVERCONFIG_H__
#define __GAMESERVERCONFIG_H__
#include "commontype.h"
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
	char			mDBHost[STRING_LENGTH_64];
	/// �û���
	char			mDBUserName[STRING_LENGTH_16];
	/// ����
	char			mDBPasswd[STRING_LENGTH_16];
	/// ���ӵ����ݿ���
	char			mDefaultDataBase[STRING_LENGTH_16];
	/// �˿�
	short			mDBPort;
};
#endif