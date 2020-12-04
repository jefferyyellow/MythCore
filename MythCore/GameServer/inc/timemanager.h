#ifndef __SVRTIMEMANAGER_H__
#define __SVRTIMEMANAGER_H__
#include "commontype.h"
#include "singleton.h"
using namespace Myth;
class CTimeManager : public CSingleton<CTimeManager>
{
	friend class CSingleton<CTimeManager>;
private:
	CTimeManager()
	{
		mCurrTime = 0;
	}

	~CTimeManager()
	{

	}

public:
	///// ��ʱ��ת��Ϊ����Ϊ�����ʱ���,1530��ʾ������ʱ�䵱���15��30
	//time_t			time2TimeStamp(int nTime);
	/// ������ת��Ϊʱ���,20200804ת����2020��8��4���賿0��0��0���ʱ���
	time_t			date2TimeStamp(int nTime);
	/// �Ƿ���ͬһ��
	bool			checkSameDay(time_t nOldTime, time_t nNewTime);

public:
	inline time_t	getCurrTime(){ return mCurrTime; }
	void			setCurrTime(time_t tTime){ mCurrTime = tTime; }

private:
	/// ��ǰ��ʱ��(��Ϸʱ��)
	time_t			mCurrTime;
};
#endif