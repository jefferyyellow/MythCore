#include "mapjob.h"
#include "mapmamager.h"
CMapJob::CMapJob()
{

}

CMapJob::~CMapJob()
{

}

void CMapJob::doing(int uParam)
{
	unsigned int timeOffset = 1;
	for (int i = 0; i < mMapNum; ++i)
	{
		mMapList[i]->onTimer(timeOffset);
	}
}

