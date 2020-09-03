#ifndef __RANKJOB_H__
#define __RANKJOB_H__
#include "commontype.h"
#include "job.h"
#include "singleton.h"
#include "ranktype.h"
#include "rank.h"
using namespace Myth;
typedef CRank<MAX_RANK_NUM> CRankList;
class CRankJob : public CJob<100, 100>, public CSingleton<CRankJob>
{
	friend class CSingleton<CRankJob>;
public:
	CRankJob()
	{

	}
	~CRankJob()
	{

	}

public:
	virtual void doing(int uParam);

public:
	// ������ҵ����а�
	void updateRoleRank(EmRankType eType, uint nRoleID, int nValue, time_t nTime);
	// �����ҵ�����
	int getRoleRank(EmRankType eType, int nRoleID);

private:
	CRankList		mRankList[emRankTypeMax];
};
#endif