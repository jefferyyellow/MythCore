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
	/// ע�⣬Host��ֻ����IP��������������
	char			mDBHost[STR_LENGTH_64];
	/// �û���
	char			mDBUserName[STR_LENGTH_16];
	/// ����
	char			mDBPasswd[STR_LENGTH_16];
	/// ���ӵ����ݿ���
	char			mDefaultDataBase[STR_LENGTH_16];
	/// �������������˿ڣ������ط�����������
	short			mDBPort;
	/// �����������˿�
	short			mListenPort;
	// ������ID
	int				mServerID;
};
#endif