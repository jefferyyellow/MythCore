#ifndef __MAILMODULE_H__
#define __MAILMODULE_H__
#include "singleton.h"
#include "logicmodule.h"
#include "commontype.h"
#include "mythlist.h"
#include "mail.h"
using namespace Myth;
class CEntityPlayer;
class CMail;
class CDBResponse;
class PBMailItemList;
class CMailModule : public CLogicModule, public CSingleton<CMailModule>
{
	friend class CSingleton<CMailModule>;
public:
	typedef CMythList<CMail, MAX_PLAYER_MAIL>	MAIL_LIST;

private:
	CMailModule()
	{
		mPlayerMailID = 0;
		mGlobalMailID = 0;
		mGlobalLoadComplete = false;
	}
	virtual ~CMailModule(){}

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
	/// ���������ʼ�ID
	void		loadMaxMailID();
	/// ���������ʼ�ID�ص�
	void		onLoadMaxMailID(CDBResponse& rResponse);
	/// ��������ȫ���ʼ�ID
	void		loadMaxGlobalMailID();
	/// ��������ȫ���ʼ�ID�ص�
	void		onLoadMaxGlobalMailID(CDBResponse& rResponse);

	/// ɾ�����ڵ��ʼ�
	void		deleteExpireMail();
	/// ɾ�����ڵ�ȫ���ʼ�
	void		deleteExpireGlobalMail();
	/// ȫ���ʼ��б�
	void		sendGlobalMail(CMail& rMail);
	/// ����ҷ����ʼ�
	void		sendPlayerMail(uint nRoleID, CMail& rMail);
	/// ����ҷ����ʼ�
	void		sendPlayerMail(CEntityPlayer& rPlayer, CMail& rMail);
	/// �����ʼ������ݿ�
	void		saveMail2DB(uint nRoleID, CMail& rMail);
	/// ����ȫ���ʼ������ݿ�
	void		saveGlobalMail2DB(CMail& rMail);
	/// �����ʼ�
	void		loadPlayerMail(int nRoleID);
	/// �����ʼ����ݿ�ص�
	void		onLoadPlayerMail(CDBResponse& rResponse);
	/// ����ȫ���ʼ�
	void		loadGlobalMail();
	/// ����ȫ���ʼ��ص�
	void		onLoadGlobalMail(CDBResponse& rResponse);
	/// �����ʼ������б�
	void		loadMailItemList(CMail& rMail, PBMailItemList& pbMailItemList);
	/// �����ʼ������б�
	void		saveMailItemList(CMail& rMail, PBMailItemList* pbMailItemList);
	/// ɾ��ȫ���ʼ�
	void		deleteGlobalMail2DB(uint nMailID);
	/// ����ҷ�ȫ���ʼ�
	void		giveAllGlobalMail(CEntityPlayer& rPlayer);
	/// �����Ե���ҷ�ȫ���ʼ�
	void		giveAllPlayerGlobalMail(CMail& rMail);
private:
	/// ����ʼ�ID������
	uint		mPlayerMailID;
	/// ȫ���ʼ�ID������
	uint		mGlobalMailID;
	/// �Ƿ��Ƿ����ȫ���ʼ����
	bool		mGlobalLoadComplete;
	/// ȫ���ʼ��б�
	MAIL_LIST	mGlobalList;
};
#endif