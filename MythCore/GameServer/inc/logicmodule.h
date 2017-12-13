#ifndef __LOGICMODULE_H__
#define __LOGICMODULE_H__
/// 逻辑模块
class CLogicModule
{
public:
	CLogicModule()
	{}

	virtual ~CLogicModule()
	{}
public:
	/// 时间函数
	virtual		void	OnTimer(unsigned int nTickOffset)				= 0;
};
#endif // !__LOGICMODULE_H__
