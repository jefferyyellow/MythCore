#ifndef __LOGICMODULE_H__
#define __LOGICMODULE_H__
/// �߼�ģ��
class CLogicModule
{
public:
	CLogicModule()
	{}

	virtual ~CLogicModule()
	{}
public:
	/// ʱ�亯��
	virtual		void	OnTimer(unsigned int nTickOffset)				= 0;
};
#endif // !__LOGICMODULE_H__
