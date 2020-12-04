#ifndef __GAMESERVERCONFIG_H__
#define __GAMESERVERCONFIG_H__
#include "servercommon.h"
#include "singleton.h"
using namespace Myth;

class CGameServerConfig : public CSingleton<CGameServerConfig>
{
	friend class CSingleton<CGameServerConfig>;
private:
	CGameServerConfig()
	{
		init();
	}
	~CGameServerConfig(){}

	void init()
	{
        mDBHost[0] = '\0';
        mDBUserName[0] = '\0';
        mDBPasswd[0] = '\0';
        mDefaultDataBase[0] = '\0';
        mDBPort = 0;
	}

public:
	bool			loadGameServerConfigFromXml(const char* pFilePath);

public:
	/// autocode don't edit
    char* getDBHost(){ return mDBHost;}
    void setDBHost(const char* value)
    {
        if (NULL == value)
        {
            return;
        }
        strncpy(mDBHost, value, sizeof(mDBHost) - 1);
    }

    char* getDBUserName(){ return mDBUserName;}
    void setDBUserName(const char* value)
    {
        if (NULL == value)
        {
            return;
        }
        strncpy(mDBUserName, value, sizeof(mDBUserName) - 1);
    }

    char* getDBPasswd(){ return mDBPasswd;}
    void setDBPasswd(const char* value)
    {
        if (NULL == value)
        {
            return;
        }
        strncpy(mDBPasswd, value, sizeof(mDBPasswd) - 1);
    }

    char* getDefaultDataBase(){ return mDefaultDataBase;}
    void setDefaultDataBase(const char* value)
    {
        if (NULL == value)
        {
            return;
        }
        strncpy(mDefaultDataBase, value, sizeof(mDefaultDataBase) - 1);
    }

    short getDBPort(){ return mDBPort;}
    void setDBPort(short value){ mDBPort = value;}

	short getListenPort() const { return mListenPort; }
	void setListenPort(short nValue) { mListenPort = nValue; }

	/// end autocode

private:
	/// 注意，Host不只是有IP，还可能是域名
	char			mDBHost[STR_LENGTH_64];
	/// 用户名
	char			mDBUserName[STR_LENGTH_16];
	/// 密码
	char			mDBPasswd[STR_LENGTH_16];
	/// 连接的数据库名
	char			mDefaultDataBase[STR_LENGTH_16];
	/// 服务器的侦听端口，给网关服务器连接用
	short			mDBPort;
	/// 服务器侦听端口
	short			mListenPort;
	// 服务器ID
	int				mServerID;
};
#endif