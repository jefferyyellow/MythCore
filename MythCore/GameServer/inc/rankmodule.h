#ifndef __RANKMODULE_H__
#define __RANKMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "ranktype.h"
#include "rank.h"
#include "messagefactory.h"
using namespace Myth;

typedef CRank<MAX_RANK_NUM> CRankList;
class CEntityPlayer;
class CRankModule : public CLogicModule, public CSingleton<CRankModule>
{
	friend class CSingleton<CRankModule>;
private:
	CRankModule();
	~CRankModule();

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

public:
	void onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);
	void onGetRankInfoRequest(CEntityPlayer* pPlayer, Message* pMessage);
	void sendGetRankInfoResponse();

public:
	// ������ҵ����а�
	void updateRoleRank(EmRankType eType, CEntityPlayer* pPlayer, int nValue);

private:
	CRankList		mRankList[emRankTypeMax];
};
#endif