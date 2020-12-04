#ifndef __MAPJOB_H__
#define __MAPJOB_H__
#include "job.h"
#include "commontype.h"
using namespace Myth;
#define MAX_MAP_PER_JOB		128
class CMap;
class CMapJob : public CJob
{
public:
	CMapJob();
	~CMapJob();

public:
	void		init()
	{

	}

	void	clear(){}

public:
	virtual void doing(int uParam);

public:
	CMap*	mMapList[MAX_MAP_PER_JOB];
	int		mMapNum;
};
#endif