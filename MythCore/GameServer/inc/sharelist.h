#ifndef __SHARELIST_H__
#define __SHARELIST_H__
#include "servercommon.h"
#include "blockmemory.h"

// 没有内存分配的list,共享着内存的list
template<typename T>
class CShareList
{
public:
	template<typename Node>
	class CShareListNode
	{
	public:
		CShareListNode<Node>*	mpPrev;
		CShareListNode<Node>*	mpNext;
		Node					mData;
	};

	template<typename Node>
	struct CShareListIterator
	{
		typedef CShareListIterator<Node>			Self;

		typedef Node								value_type;
		typedef Node*								pointer;
		typedef Node&								reference;

		CShareListIterator()
			: mNode(NULL) { }

		explicit
			CShareListIterator(CShareListNode<Node>* x)
			: mNode(x) { }

		// Must downcast from List_node_base to _List_node to get to mData.
		reference
			operator*() const
		{
			return mNode->mData;
		}

		pointer
			operator->() const
		{
			return &(mNode->mData);
		}

		Self&
			operator++()
		{
			mNode = mNode->mpNext;
			return *this;
		}

		Self
			operator++(int)
		{
			Self tmp = *this;
			mNode = mNode->mpNext;
			return tmp;
		}

		Self&
			operator--()
		{
			mNode = mNode->mpPrev;
			return *this;
		}

		Self
			operator--(int)
		{
			Self tmp = *this;
			mNode = mNode->mpPrev;
			return tmp;
		}

		bool
			operator==(const Self& x) const
		{
			return mNode == x.mNode;
		}

		bool
			operator!=(const Self& x) const
		{
			return mNode != x.mNode;
		}

		// The only member points to the %list element.
		CShareListNode<Node>* mNode;
	};

	template<typename Node>
	struct CShareListConstIterator
	{
		typedef CShareListConstIterator<Node>			Self;
		typedef CShareListIterator<Node>				iterator;

		typedef Node									value_type;
		typedef const Node*							pointer;
		typedef const Node&							reference;

		CShareListConstIterator()
			: mNode(NULL) { }

		explicit
			CShareListConstIterator(const CShareListNode<Node>* x)
			: mNode(x) { }

		CShareListConstIterator(const iterator& x)
			: mNode(x.mNode) { }

		// Must downcast from List_node_base to _List_node to get to
		// mData.
		reference
			operator*() const
		{
			return mNode->mData;
		}

		pointer
			operator->() const
		{
			return &(mNode->mData);
		}

		Self&
			operator++()
		{
			mNode = mNode->mpNext;
			return *this;
		}

		Self
			operator++(int)
		{
			Self tmp = *this;
			mNode = mNode->mpNext;
			return tmp;
		}

		Self&
			operator--()
		{
			mNode = mNode->mpPrev;
			return *this;
		}

		Self
			operator--(int)
		{
			Self tmp = *this;
			mNode = mNode->mpPrev;
			return tmp;
		}

		bool
			operator==(const Self& x) const
		{
			return mNode == x.mNode;
		}

		bool
			operator!=(const Self& x) const
		{
			return mNode != x.mNode;
		}

		// The only member points to the %list element.
		const CShareListNode<Node>* mNode;
	};



public:
	typedef T													value_type;
	typedef value_type*											pointer;
	typedef value_type&											reference;
	typedef const value_type*		   							const_pointer;
	typedef const value_type&									const_reference;
	typedef CShareListIterator<T>								iterator;
	typedef CShareListConstIterator<T>							const_iterator;
	typedef size_t												size_type;

public:
	CShareList()
	{
		mHeadNode = NULL;
	}

	~CShareList()
	{
		clear();
	}

	void clear()
	{
		CShareListNode<T>* pCurrNode = mHeadNode;
		while(NULL != mHeadNode)
		{
			CShareListNode<T>* tmpNode = pCurrNode->mpNext;
			eraseNode(pCurrNode);
			pCurrNode = tmpNode;
		}
		mHeadNode = NULL;
	}

public:
	iterator begin(){ return iterator(mHeadNode); }
	const_iterator begin() const{ return const_iterator(mHeadNode);}

	iterator end(){ return iterator(NULL); }
	const_iterator end() const{ return const_iterator(NULL); }

	bool empty() const{ return NULL == mHeadNode; }

	reference front(){ return *mHeadNode; }
	const_reference front() const{ return *mHeadNode; }

	void push_front(CShareListNode<T>* pNode)
	{
		if (NULL == pNode)
		{
			return;
		}

		if (NULL == mHeadNode)
		{
			mHeadNode = pNode;
			pNode->mpNext = NULL;
			pNode->mpPrev = NULL;
		}
		else
		{
			mHeadNode->mpPrev = pNode;
			pNode->mpNext = mHeadNode;
				
			pNode->mpPrev = NULL;

			mHeadNode = pNode;
		}
	}

	void pop_front()
	{
		eraseNode(mHeadNode);
	}

	void remove(const T& value)
	{
		iterator it = begin();
		iterator itEnd = end();
		iterator tTemp = it;
		while(it != itEnd)
		{
			if (*it == value)
			{
				tTemp = it;
				++ it;
				eraseNode(tTemp.mNode);
				break;
			}
			else
			{
				++ it;
			}
		}
	}

	iterator eraseNode(iterator it)
	{
		return eraseNode(it.mNode);
	}

private:
	iterator eraseNode(CShareListNode<T>* pCurrNode)
	{
		if (NULL == pCurrNode)
		{
			return iterator(NULL);
		}
			
		CShareListNode<T>* pPrevNode = pCurrNode->mpPrev;
		CShareListNode<T>* pNextNode = pCurrNode->mpNext;

		if (NULL != pPrevNode)
		{
			pPrevNode->mpNext = pCurrNode->mpNext;
		}

		if (NULL != pNextNode)
		{
			pNextNode->mpPrev = pCurrNode->mpPrev;
		}
			
		if (mHeadNode == pCurrNode)
		{
			mHeadNode = pCurrNode->mpNext;
		}
		return iterator(pNextNode);
	}

private:
	CShareListNode<T>*		mHeadNode;
};
#endif