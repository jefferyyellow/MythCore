#ifndef __LOGICMODULE_H__
#define __LOGICMODULE_H__
/// �߼�ģ��
class CEntityPlayer;
class CLogicModule
{
public:
	CLogicModule()
	{}

	virtual ~CLogicModule()
	{}
public:
	/// ����������
	virtual void onLaunchServer() = 0;
	/// ������ɼ��
	virtual bool onCheckLaunch() = 0;
	/// �����������ɹ�
	virtual void onLaunchComplete() = 0;
	/// �˳�������
	virtual void onExitServer() = 0;
	/// ��һ�쵽��
	virtual void onNewDayCome() = 0;
	/// ��һ�ܵ���
	virtual void onNewWeekCome() = 0;
	/// ����ʵ��
	virtual void onCreatePlayer(CEntityPlayer* pPlayer) = 0;
	/// ����ʵ��
	virtual void onDestroyPlayer(CEntityPlayer* pPlayer) = 0;
	/// ʱ�亯��
	virtual	void onTimer(unsigned int nTickOffset) = 0;
};
#endif // !__LOGICMODULE_H__
