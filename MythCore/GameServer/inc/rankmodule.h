#ifndef __RANKMODULE_H__
#define __RANKMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "ranktype.h"
#include "commontype.h"
#include "template.h"
#include "logintype.h"
#include <map>

using namespace Myth;

class CEntityPlayer;
class CInternalMsg;

class CPlayerRankInfo
{
public:
	CPlayerRankInfo()
	{}
	~CPlayerRankInfo()
	{}

public:
	/// ����
	char*		getName(){return mName;}
	void		setName(const char* pName)
	{
		if (NULL == pName)
		{
			return;
		}
		strncpy(mName, pName, sizeof(MAX_PLAYER_NAME_LEN) - 1);
	}

	/// ͷ��ID
	uint		getHeadID(){return mHeadID;}
	void		setHeadID(uint nHeadID){mHeadID = nHeadID;}

private:
	/// �������
	char		mName[MAX_PLAYER_NAME_LEN];
	/// ͷ��ID
	uint		mHeadID;
};

class CRankModule : public CLogicModule, public CSingleton<CRankModule>
{
	friend class CSingleton<CRankModule>;
	typedef std::map<uint, CPlayerRankInfo>	HASH_RANK_INFO;

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
	virtual void onCreatePlayer(CEntityPlayer& rPlayer);
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer& rPlayer);
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset);
	/// ���������ļ�
	virtual	void onLoadConfig();

public:
	void onClientMessage(CEntityPlayer& rPlayer, unsigned int nMessageID, Message* pMessage);
	void onGetRankInfoRequest(CEntityPlayer& rPlayer, Message* pMessage);

public:
	void onIMGetRankInfoResponse(CInternalMsg* pIMMsg);
	void onIMUpdateRankResponse(CInternalMsg* pIMMsg);

public:
	/// ������ҵ����а�
	void updateRoleRank(EmRankType eType, CEntityPlayer& rPlayer, int nValue);
	/// ����������а���Ϣ
	void updateRoleInfo(CEntityPlayer& rPlayer);
	/// ����������а���Ϣ
	void updateRoleInfo(CEntityPlayer& rPlayer, CPlayerRankInfo& rRankInfo);
	/// ����������а���Ϣ
	void addRoleInfo(CEntityPlayer& rPlayer);
private:
	HASH_RANK_INFO	mHashRankInfo;
};
#endif