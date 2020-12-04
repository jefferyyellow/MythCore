#ifndef __FRIENDMODULE_H__
#define __FRIENDMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
using namespace Myth;
class CFriendModule : public CLogicModule, public CSingleton<CFriendModule>
{
	friend class CSingleton<CFriendModule>;
private:
	CFriendModule();
	~CFriendModule();

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

};
#endif