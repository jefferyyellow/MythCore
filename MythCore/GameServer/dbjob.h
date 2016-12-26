#ifndef __DBJOB_H__
#define __DBJOB_H__
#include "thread.h"
class CDBJob : public Myth::IJob
{
public:
	CDBJob();
	~CDBJob();

public:
	virtual void doing(uint32 uParam) = 0;
};
#endif
