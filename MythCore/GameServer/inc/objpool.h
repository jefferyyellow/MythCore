#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__
#include "singleton.h"
#include "obj.h"
#include "servercommon.h"
using namespace Myth;
class CObjPoolImp;
class CShareObjPoolImp;
class CObjPool : public CSingleton<CObjPool>
{
	friend class CSingleton<CObjPool>;
private:
	CObjPool();
	~CObjPool();

public:
	int		getShareMemorySize();
	void	setShareMemory(byte* pShareMemory);

public:
	int		getPlayerMinID();
	int		getPlayerMaxID();

public:
	CObj*	allocObj(EmObjType eType);
	CObj*	getObj(int nObjID);
	void	free(int nObjID);
	void	logObjNum();

	CObjPoolImp* getPoolImp() const { return mPoolImp; }
private:
	CObjPoolImp*		mPoolImp;
	CShareObjPoolImp*	mSharePoolImp;
};
#endif