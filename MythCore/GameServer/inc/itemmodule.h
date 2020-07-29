#ifndef __ITEMMODULE_H__
#define __ITEMMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
#include "shopconfig.h"
using namespace Myth;
class CEntityPlayer;
class CItemModule : public CLogicModule, public CSingleton < CItemModule >
{
	friend class CSingleton < CItemModule >;
private:
	CItemModule();
	~CItemModule();

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

public:
	int	getFromDropTable(int nDropTableID, int& rDropItemNum);

public:
	SHOP_CONFIG_LIST&	getShopList(){return mShopList;}
public:
	SHOP_CONFIG_LIST	mShopList;
};
#endif