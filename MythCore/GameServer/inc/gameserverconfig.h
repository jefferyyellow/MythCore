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
	/// 注意，Host不只是有IP，还可能是域名
	char			mDBHost[STRING_LENGTH_64];
	/// 用户名
	char			mDBUserName[STRING_LENGTH_16];
	/// 密码
	char			mDBPasswd[STRING_LENGTH_16];
	/// 连接的数据库名
	char			mDefaultDataBase[STRING_LENGTH_16];
	/// 端口
	short			mDBPort;
};
#endif