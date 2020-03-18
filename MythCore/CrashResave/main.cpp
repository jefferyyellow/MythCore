#include "commontype.h"
#include "sharememory.h"
using namespace Myth;
class CMemUnit
{
public:
	CMemUnit()
	{
		mPlayerID = 0;
		mExp = 0;
		mLevel = 0;
	}
	~CMemUnit()
	{

	}

public:
	int GetPlayerID() const { return mPlayerID; }
	void SetPlayerID(int nValue) { mPlayerID = nValue; }

	int GetExp() const { return mExp; }
	void SetExp(int nValue) { mExp = nValue; }

	int GetLevel() const { return mLevel; }
	void SetLevel(int nValue) { mLevel = nValue; }

private:
	int mPlayerID;
	int mExp;
	int	mLevel;
};
int main()
{
	bool bCreate = true;

	int nShareMemorySize = sizeof(int) + sizeof(CMemUnit) * 10;
	byte* pSharePoint = CShareMemory::createShareMemory(12343433, nShareMemorySize, bCreate);
	if (NULL == pSharePoint)
	{
		return false;
	}

	printf("%x\n", pSharePoint);
	int* pNum = (int*)pSharePoint;

	CMemUnit* pUnit = (CMemUnit*)(pSharePoint + sizeof(int));

	printf("Num:	%d\n", *pNum);
	CMemUnit* pTempUnit = pUnit;
	for (int i = 0; i < *pNum; ++ i)
	{
		printf("PlayerID:	%d\n", pTempUnit->GetPlayerID());
		printf("Level:	%d\n", pTempUnit->GetLevel());
		printf("Exp:	%d\n", pTempUnit->GetExp());
		pTempUnit += 1;
	}
	Sleep(100000);
}