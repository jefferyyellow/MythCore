#include "timemanager.h"
#include "scenejob.h"
/// ��ʱ��ת��Ϊ����Ϊ�����ʱ���,1530��ʾ������ʱ�䵱���15��30
time_t CTimeManager::time2TimeStamp(int nTime)
{
	tm tTmNow = CSceneJob::Inst()->getTmNow();
	tTmNow.tm_hour = nTime / 100;
	tTmNow.tm_min = nTime % 100;
	tTmNow.tm_sec = 0;

	return mktime(&tTmNow);
}

/// ������ת��Ϊʱ���,20200804ת����2020��8��4���賿0��0��0���ʱ���
time_t CTimeManager::date2TimeStamp(int nDate)
{
	int nYear = nDate / 10000;
	int nLeft = nDate % 10000;

	int nMonth = nLeft / 100;
	int nDay = nLeft % 100;

	tm tTm;
	memset(&tTm, 0, sizeof(tTm));
	tTm.tm_year = nYear - 1900;
	tTm.tm_mon = nMonth - 1;
	tTm.tm_mday = nDay;

	return mktime(&tTm);
}

/// �Ƿ���ͬһ��
bool CTimeManager::isSameDay(time_t nOldTime, time_t nNewTime)
{
	tm	tTmOld;
	tm	tTmNew;
#ifdef MYTH_OS_WINDOWS
	localtime_s(&tTmOld, &nOldTime);
	localtime_s(&tTmNew, &nNewTime);
#else
	localtime_r(&nOldTime, &tTmOld);
	localtime_r(&nNewTime, &tTmNew);
#endif // MYTH_OS_WINDOWS

	if (tTmNew.tm_yday != tTmOld.tm_yday)
	{
		return true;
	}
	else
	{
		return false;
	}
}