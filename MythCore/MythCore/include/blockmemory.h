#ifndef __BLOCKMEMORY_H__
#define __BLOCKMEMORY_H__
#include <stdio.h>
#include "memory.h"
#include "commontype.h"
/**
 * Block memory allocate
 * This memory manager is fast memory allocator,do same thing as new/delete,it works by blocks,
 * blocks are always allocated and never freed until the destruct is called
 * 
 * in debug mode, mHeadIdent and mTailIdent use to check the memory is written in contrary to 
 * expectation
 * 
 * A memory block consist by some memory nodes,\n
 * BaseCount:allocate a memory block consist of BaseCount memory node when construct\n
 * Increment:when the all memory nodes used up, allocate a memory block 
 * consist of Increment memory nodes
 *
 * \author jefferyyellow
 * \date 2015
 */
template<typename T, uint BaseCount, uint Increment>
class CBlockMemory
{
private:
	struct CMemoryNode
	{
		/// mpNode:		idle mode
		/// uint8		mBuff[sizeof(T)]: allocate mode
		union BuffNode
		{
			uint8		mBuff[sizeof(T)];
			CMemoryNode	*mpNode;
		};	

#ifdef __DEBUG__
		uint32		mHeadIdent;
#endif
		BuffNode	mBuffNode;
#ifdef __DEBUG__
		uint32		mTailIdent;
#endif
	};
	struct CBlockListNode
	{
		CMemoryNode*	mBlockPoint;
		CBlockListNode*	mNextNode;
	};
public:
	CBlockMemory()
	{
		init();
	}
	~CBlockMemory()
	{
		final();
	}

	/// init member variables, allocate BaseCount memory node
	void init()
	{
		mpBlockHead = NULL;
		mAllocCount = 0;
		mpIdleList = allocBlock(BaseCount);
	}

	/// delete all memory allocate from system 
	void final()
	{
		CBlockListNode* pNode = mpBlockHead;
		while (NULL != pNode)
		{
			delete []pNode->mBlockPoint;
			pNode = pNode->mNextNode;
		}

		
		while (NULL != mpBlockHead)
		{
			pNode = mpBlockHead;
			mpBlockHead = mpBlockHead->mNextNode;
			delete pNode;
		}
		
	}

public:
	/// allocate an element
	T*			allocate()
	{
		if (NULL == mpIdleList)
		{
			mpIdleList = allocBlock(Increment);
		}

		// check all idle memory node used up
		if (NULL != mpIdleList)
		{
			T* pAllocNode = reinterpret_cast<T*>(mpIdleList->mBuffNode.mBuff);
#ifdef __DEBUG__
			// check the ident
			if (CheckDeletedIdent != mpIdleList->mHeadIdent)
			{
				printf("CheckDeletedIdent");
			}

			if (CheckDeletedIdent != mpIdleList->mTailIdent)
			{
				printf("CheckDeletedIdent");
			}
			// mark the elemt as allocate
			mpIdleList->mHeadIdent = CheckAllocatedIdent;
			mpIdleList->mTailIdent = CheckAllocatedIdent;
#endif
			mpIdleList = mpIdleList->mBuffNode.mpNode;
//#ifdef MYTH_OS_WINDOWS
//			// vs2010 or above
//			comconstruct<T, std::has_trivial_constructor<T>::value>::construct(pAllocNode, value);
//#else
//			// gcc has no std::has_trivial_constructor<T>::value
//			comconstruct<T, false>::construct(pAllocNode, value);
//#endif

			++ mAllocCount;
			return pAllocNode;
		}
		else
		{
			return NULL;
		}

	}

	/// free an element
	void		free(T* pFree)
	{
		if (NULL == pFree)
		{
			return;
		}
//#ifdef MYTH_OS_WINDOWS
//		// vs2010 or above
//		comdeconstruct<T, std::has_trivial_destructor<T>::value>::deconstruct(pFree);
//#else
//		// gcc has no std::has_trivial_destructor<T>::value
//		comdeconstruct<T, false>::deconstruct(pFree);
//#endif
#ifdef __DEBUG__
		uint8 *pTemp = (uint8*)pFree;
		CMemoryNode* pFreeNode = reinterpret_cast<CMemoryNode*>(pTemp - CLASS_MEM_OFFSET(CMemoryNode,mBuffNode));
		// check the ident
		if (CheckAllocatedIdent != pFreeNode->mHeadIdent)
		{
			printf("CheckAllocatedIdent");
		}

		if (CheckAllocatedIdent != pFreeNode->mTailIdent)
		{
			printf("CheckAllocatedIdent");
		}
		// mark the elemt as free
		pFreeNode->mHeadIdent = CheckDeletedIdent;
		pFreeNode->mTailIdent = CheckDeletedIdent;
#else
		CMemoryNode* pFreeNode = reinterpret_cast<CMemoryNode*>(pFree);; 
#endif
		pFreeNode->mBuffNode.mpNode = mpIdleList;
		mpIdleList = pFreeNode;
		-- mAllocCount;
	}

	/// check memory leak when recover all memory
	bool		checkMemoryLeak()
	{
		if (0 != mAllocCount)
		{
			return true;
		}

		return false;
	}

private:
	/// allocate a memory block consist of nSize memory nodes;
	CMemoryNode*		allocBlock(uint nSize)
	{
		if (nSize <= 0)
		{
			return NULL;
		}
		CMemoryNode* pNode = new CMemoryNode[nSize];
		if (NULL == pNode)
		{
			return NULL;
		}
		// make all memory node as a chain
		CMemoryNode* pTempNode = pNode;
		for (uint i = 0; i < nSize - 1; ++ i)
		{
			pTempNode->mBuffNode.mpNode = pTempNode + 1;
			++ pTempNode;
		}
		// the last node has no next node
		pTempNode->mBuffNode.mpNode = NULL;
#ifdef __DEBUG__
		pTempNode = pNode;
		for (uint i = 0; i < nSize; ++ i)
		{
			// mark as free node
			pTempNode->mHeadIdent = CheckDeletedIdent;
			pTempNode->mTailIdent = CheckDeletedIdent;
			++ pTempNode;
		}
#endif
		addBlockListNode(pNode);
		return pNode;
	}

	/// add a block in block list
	void addBlockListNode(CMemoryNode* pNode)
	{
		if (NULL == pNode)
		{
			return;
		}
		CBlockListNode* pBlockListNode = new CBlockListNode;
		if (NULL == pBlockListNode)
		{
			return;
		}
		pBlockListNode->mBlockPoint = pNode;
		pBlockListNode->mNextNode = mpBlockHead;
		mpBlockHead = pBlockListNode;
	}

private:
	/// For debug only, check ident.
	enum  TCheckIdent	{ CheckAllocatedIdent= 0x01234567, CheckDeletedIdent= 0x89ABCDEF };

private:
	/// idle memory node list head
	CMemoryNode*		mpIdleList;
	/// memory block list head
	CBlockListNode*		mpBlockHead;
	/// alread allocate memory node count
	uint32				mAllocCount;
};
#endif
