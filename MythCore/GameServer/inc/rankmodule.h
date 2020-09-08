#ifndef __RANKMODULE_H__
#define __RANKMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "ranktype.h"
#include "commontype.h"
using namespace Myth;

class CEntityPlayer;
class CInternalMsg;
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
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);
	void onGetRankInfoRequest(CEntityPlayer& rPlayer, Message* pMessage);

public:
	void onIMGetRankInfoResponse(CInternalMsg* pIMMsg);

public:
	// ������ҵ����а�
	void updateRoleRank(EmRankType eType, uint nRoleID, int nValue);
};
#endif