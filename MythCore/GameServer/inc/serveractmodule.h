#ifndef __SERVERACTMODULE_H__
#define __SERVERACTMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "serveractivitytype.h"
#include "servercommon.h"

using namespace Myth;

class CEntityPlayer;
class CServerActivity;
class CIMPlatWebResponse;
/// ��ҵĿ������ģ������ÿ��һˢ�£�������������������ͻ����ˢ��
/// ���Ի��¼����������ʱ��
/// �����ģ��
class CServerActModule : public CLogicModule, public CSingleton <CServerActModule>
{
	friend class CSingleton <CServerActModule> ;
public:
	typedef std::set<string> CONFIG_FILE_LIST;
private:
	CServerActModule();
	~CServerActModule();

	void init();

	// ��տ����
	void clearServerActivity();

public:
	typedef vector<string>				SERVER_LOAD_LIST;
public:
	/// ����������
	virtual void onLaunchServer();
	/// ������ɼ��
	virtual bool onCheckLaunch();
	/// �����������ɹ�
	virtual void onLaunchComplete();
	/// �˳�������
	virtual void onExitServer();
	/// ��һ�쵽��
	virtual void onNewDayCome();
	/// ��һ�ܵ���
	virtual void onNewWeekCome();
	/// ����ʵ��
	virtual void onCreatePlayer(CEntityPlayer* pPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();
	/// ���¼��������ļ�
	virtual	void onReloadConfig();

public:
	void				onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);
	void				onGetServerActRequest(CEntityPlayer* pPlayer, Message* pMessage);
	void				sendGetServerActResponse(CEntityPlayer* pPlayer);

public:
	/// ���ؿ��Ż����
	void				loadServerActivityConfig(const char* pConfigFile);
	/// ��������Ļ
	void				clearEndedActivity();
	/// �õ���Ӧ�Ŀ����
	CServerActivity*	getServerActivity(int nActivityID);
	/// ˢ�»����
	void				refreshProcess(EmSvrActType eType, CEntityPlayer& rPlayer, int nParam1, int nParam2);
	
	/// �Ƿ��ڻ�ڼ�
	bool				checkActOpen(int nActivityID);
	/// �õ��������
	int					getActType(int nActivityID);
	/// �Ƿ��ڿ����콱���ڼ�(������Ч�ڼ�ͷ����ڼ�)
	bool				checkActPrizeTime(int nActivityID);
	/// ÿ��ˢ�����Ե����
	void				dailyRefreshAllPlayer();
	/// ����ƽ̨�����ļ��ص�
	void				onLoadPlatFile(CIMPlatWebResponse* pResponse);
public:
	/// autocode
    time_t getServerOpenTime(){ return mServerOpenTime;}
    void setServerOpenTime(time_t value){ mServerOpenTime = value;}

    time_t getServerStartTime(){ return mServerStartTime;}
    void setServerStartTime(time_t value){ mServerStartTime = value;}
	/// end autocode

private:
	/// ����������ʱ��
	time_t					mServerOpenTime;
	/// ����������ʱ��
	time_t					mServerStartTime;
	/// �����
	CServerActivity*		mServerActivity[MAX_SERVER_ACT_NUM];
	/// �����
	CServerActivity*		mAvailActivity[MAX_SERVER_ACT_NUM];
	// ��Ч���Ż���������ļ�
	bool					mAvailLoadConfig[emServerActTypeMax];
	/// ��Ч�Ŀ������Ŀ
	int						mAvailActivityNum;
	/// ���ص������ļ��б�
	CONFIG_FILE_LIST		mConfigFileList;
	/// �����ļ��������
	bool					mConfigFileLoadComplete;
};
#endif