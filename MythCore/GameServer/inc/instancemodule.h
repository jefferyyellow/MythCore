#ifndef __INSTANCEMODULE_H__
#define __INSTANCEMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "messagefactory.h"
#include "instancetype.h"
#include "timeutility.h"
using namespace Myth;
class CInstance;
class CInstanceConfig;
class CEntityOgre;
class CInstanceModule : public CLogicModule, public CSingleton < CInstanceModule >
{
	friend class CSingleton<CInstanceModule>;
private:
	CInstanceModule();
	~CInstanceModule();

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

public:
	// �������еĸ�������
	void loadAllInstConfig(const char* pFindPath, const char* pPath);
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);

public:
	// �������Ƿ��ܽ�����
	int checkPlayerEnterInstance(CEntityPlayer& rPlayer, int nInstanceType, int nInstanceID);
	// ��ҽ��븱��������
	void onEnterInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage);
	// ��ҽ��븱���Ļ�Ӧ
	void sendEnterInstanceResponse(CEntityPlayer& rPlayer, int nResult, int nInstanceID);
	// ����뿪����������
	void onLeaveInstanceRequest(CEntityPlayer& rPlayer, Message* pMessage);
	// ����뿪�����Ļ�Ӧ
	void sendLeaveInstanceResponse(CEntityPlayer& rPlayer, int nResult);

public:
	/// ��������
	CInstance*	createInstance(int nInstanceID);
	/// ���ٸ���
	void		destoryInstance(CInstance* pInstance);
	/// ������еĸ���ʱ��
	void		checkAllInstanceTime();
	/// ----- �����¼���� -----
	/// ��������
	void		onOgreDead(CEntityOgre *pOgre);
	/// �������
	void		onPlayerDead(CEntityPlayer& rPlayer);
	/// ����뿪
	void		onPlayerLeave(CEntityPlayer& rPlayer);
	/// ----- �����¼���� -----

public:
	CInstanceConfig*	mAllInstConfig[MAX_INSTANCE_ID];
	time_t				mLastCheckTime;
};
#endif