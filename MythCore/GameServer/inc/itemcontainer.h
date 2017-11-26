#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__
#define MAX_INSERT_TYPE_NUM 16
#include "itemlist.h"
#define  MAX_CONTAINER_ITEM_NUM 100
class CItemBox : public CItemList < MAX_CONTAINER_ITEM_NUM >
{
public:
	CItemBox()
	{
		mSize = 0;
	}
	~CItemBox()
	{

	}

public:
	/// �Ƿ����㹻�Ŀռ������һ�ѵ���
	bool	checkSpace(int* pItemID, int* pNumber, int nSize);
	// �Ƿ����㹻�Ŀռ��ָ���ĵ���
	bool	checkSpace(int nItemID, int nNumber);
	// �������
	int		insertItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen);
	/// �������Ƿ����㹻�����ĵ���
	bool	checkEnough(int nItemID, int nItemNum);
	/// ɾ������
	void	removeItem(int nItemID, int nItemNum, int *pOutIndex, int *pOutNumber, int &rOutLen);
	/// ɾ������
	bool	removeItem(unsigned int nIndex, unsigned int nNum);

public:
	int		getSize(){return mSize;}
	void	setSize(int nSize){mSize = nSize;}

private:
	int		mSize;
};
#endif