#ifndef __SKILLMODULE_H__
#define __SKILLMODULE_H__
#include "logicmodule.h"
#include "singleton.h"
#include "messagefactory.h"
using namespace Myth;
class CEntityPlayer;
class CEntityCharacter;
class CTplSkill;
class CSkillModule : public CLogicModule, public CSingleton<CSkillModule>
{
	friend class CSingleton<CSkillModule>;
private:
	CSkillModule();
	~CSkillModule();

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
	void		onClientMessage(CEntityPlayer* pPlayer, unsigned int nMessageID, Message* pMessage);

public:
	void		onUseSkillRequest(CEntityPlayer* pPlayer, Message* pMessage);

	int			getSkillDamage(CEntityCharacter* pSrcEntity, CTplSkill* pTplSkill, int nSkillLevel, CEntityCharacter* pTargetEntity);
};
#endif