#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__
#include "singleton.h"
#include "obj.h"
#include "servercommon.h"
using namespace Myth;
class CObjPoolImp;
class CObjPool : public CSingleton<CObjPool>
{
	friend class CSingleton<CObjPool>;
private:
	CObjPool();
	~CObjPool(){}

public:
	CObj*	allocObj(EmObjType eType);
	CObj*	getObj(int nObjID);
	void	free(int nObjID);

private:
	CObjPoolImp* mPoolImp;
};
#endif