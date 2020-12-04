#ifndef __GUILDMODULE_H__
#define __GUILDMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
using namespace Myth;

class CEntityPlayer;
class CGuildModule : public CLogicModule, public CSingleton<CGuildModule>
{
	friend class CSingleton<CGuildModule>;
private:
	CGuildModule();
	~CGuildModule();

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