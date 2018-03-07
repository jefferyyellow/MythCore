#ifndef __DBMODULE_H__
#define __DBMODULE_H__
#include "dbmessage.h"
#include "loginmodule.h"
#include "messagefactory.h"
class CDBModule : public CLogicModule, public CSingleton < CDBModule >
{
	friend class CSingleton < CDBModule >;
private:
	CDBModule(){};
	~CDBModule(){};

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
	/// ʱ��
	virtual void onTimer(unsigned int nTickOffset);

public:
	/// ѹ��DB����
	void			pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, const char* pSql, ...);
	void			pushDBTask(int nPlayerID, int nSessionType, int nParam1, int nParam2, Message* pMessage);
	CDBResponse*	getDBResponse(){return &mDBResponse;}
	void			onDBSession();
private:

private:
	CDBRequest		mDBRequest;
	CDBResponse		mDBResponse;
};
#endif