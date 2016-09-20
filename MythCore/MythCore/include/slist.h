#ifndef __SLIST_H__
#define __SLIST_H__
#include <crtdefs.h>
#include "iterator.h"
#include "blockmemory.h"
#include <type_traits>
namespace Myth
{
	class CSlistNodeBase
	{
	public:
		CSlistNodeBase*	mpNext;
	};

	template<typename T>
	class CSlistNode : public CSlistNodeBase
	{
	public:
		T	mData;
	};


	template <class _Tp, class _Ref, class _Ptr>
	struct _Slist_iterator
	{
		typedef _Slist_iterator<_Tp, _Tp&, _Tp*>             iterator;
		typedef _Slist_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;
		typedef _Slist_iterator<_Tp, _Ref, _Ptr>             _Self;

		typedef _Tp              value_type;
		typedef _Ptr             pointer;
		typedef _Ref             reference;
		typedef CSlistNode<_Tp>	 _Node;
		typedef size_t                    size_type;
		typedef ptrdiff_t                 difference_type;
		typedef MythInternal::forward_iterator_tag iterator_category;

		CSlistNodeBase* _M_node;

		explicit
			_Slist_iterator(CSlistNodeBase* __x)
			: _M_node(__x) {}

		_Slist_iterator()
			: _M_node(0) {}

		_Slist_iterator(const iterator& __x)
			: _M_node(__x._M_node) {}

		void _M_incr()
		{ _M_node = _M_node->mpNext; }

		bool operator==(const _Slist_iterator& __x) const
		{ return _M_node == __x._M_node; }

		bool operator!=(const _Slist_iterator& __x) const
		{ return _M_node != __x._M_node; }

		reference operator*() const
		{ return ((_Node*) _M_node)->mData; }

		pointer operator->() const
		{ return &(operator*()); }

		_Self& operator++()
		{
			_M_incr();
			return *this;
		}

		_Self operator++(int)
		{
			_Self __tmp = *this;
			_M_incr();
			return __tmp;
		}
	};

	template<typename T, uint BaseCount=32, uint Increment=8>
	class CSlist
	{
	public:
		typedef T                 value_type;
		typedef value_type*       pointer;
		typedef const value_type* const_pointer;
		typedef value_type&       reference;
		typedef const value_type& const_reference;
		typedef size_t            size_type;
		typedef ptrdiff_t         difference_type;

		typedef _Slist_iterator<T, T&, T*>             iterator;
		typedef _Slist_iterator<T, const T&, const T*> const_iterator;

	public:
		explicit CSlist()
		{
			mHead.mpNext = NULL;
		}

		CSlist(int nSize, const value_type& value)
		{
			mHead.mpNext = NULL;
			initWithInitialValue((size_type)nSize, value);
		}

		CSlist(size_type nSize, const value_type& value)
		{
			mHead.mpNext = NULL;
			initWithInitialValue(nSize, value);
		}

		template <class _InputIterator>
		CSlist(_InputIterator first, _InputIterator last)
		{
			mHead.mpNext = NULL;
			CSlistNodeBase* tTempNodeBase = &mHead;
			for (; first != last; ++ first)
			{
				tTempNodeBase = insertAfter(tTempNodeBase, *first);
			}
		}

		CSlist(const CSlist& x)
		{
			mHead.mpNext = NULL;
			CSlistNodeBase* tTempNodeBase = &mHead;

			const_iterator it = x.begin();
			const_iterator itEnd = x.end();
			for (; it != itEnd; ++ it)
			{
				tTempNodeBase = insertAfter(tTempNodeBase, *it);
			}
		}

		CSlist& operator= (const CSlist& x)
		{
			mHead.mpNext = NULL;
			CSlistNodeBase* tTempNodeBase = &mHead;

			iterator it = begin();
			iterator itEnd = end();
			for (; it != itEnd; ++ it)
			{
				tTempNodeBase = insertAfter(tTempNodeBase, *it);
			}
		}

		~CSlist()
		{
			clear();
		}

	public:
		iterator begin(){return iterator(mHead.mpNext);}
		const_iterator begin() const{return const_iterator(mHead.mpNext);}

		iterator end(){return iterator(NULL);}
		const_iterator end() const{return const_iterator(NULL);}


		iterator before_begin(){return iterator(&mHead);}
		const_iterator before_begin() const {return iterator(&mHead);}

		size_type size() const
		{
			return getSize();
		}

		bool empty()const {return mHead.mpNext == NULL;}

		reference front(){return ((CSlistNode<T>*)mHead.mpNext)->mData;}
		const_reference front() const {return ((CSlistNode<T>*)mHead.mpNext)->mData;}

		void push_front(const value_type& value)
		{
			insertAfter(&mHead, value);
		}

		void pop_front()
		{
			eraseAfterNode(&mHead);
		}

		iterator previous(const_iterator pos)
		{
			iterator(getPrevious(pos._M_node));
		}

		const_iterator previous(const_iterator pos) const
		{
			return const_iterator(getPrevious(pos._M_node));
		}

	public:
		iterator insert_after(iterator pos, const value_type& value)
		{
			CSlistNodeBase* pTempNode = insertAfter(pos._M_node, value);
			return iterator(pTempNode);
		}

		void insert_after(iterator pos, size_type nSize, const value_type& value)
		{
			for (uint i = 0; i < nSize; ++ i)
			{
				insertAfter(pos._M_node, value);
			}
		}

		template <class _InIterator>
		void insert_after(iterator pos, _InIterator first, _InIterator last)
		{
			for (; first != last; ++ first)
			{
				insertAfter(pos._M_node, *first);
			}
		}

		iterator insert(iterator pos, const value_type& value)
		{
			CSlistNodeBase * pTempNode = getPrevious(pos._M_node);
			return iterator(insertAfter(pTempNode, value));
		}

		void insert(iterator pos, size_type nSize, const value_type& value)
		{
			CSlistNodeBase * pTempNode = getPrevious(pos._M_node);
			for (uint i = 0; i < nSize; ++ i)
			{
				insertAfter(pTempNode, value);
			}
		}

		template <class _InIterator>
		void insert(iterator pos, _InIterator first, _InIterator last)
		{
			CSlistNodeBase * pTempNode = getPrevious(pos._M_node);
			for (; first != last; ++ first)
			{
				insertAfter(pos._M_node, *first);
			}
		}

	public:
		iterator erase_after(iterator pos)
		{
			CSlistNodeBase* pTempNode = eraseAfterNode(pos._M_node);
			return iterator(pTempNode);
		}

		iterator erase_after(iterator before_first, iterator last)
		{
			CSlistNodeBase* pTempNode = ((CSlistNodeBase*)(before_first._M_node))->mpNext;
			CSlistNodeBase* pLast = last._M_node;
			for (; pTempNode != pLast; )
			{
				pTempNode = eraseAfterNode(pTempNode);
			}
			return last;
		}

		iterator erase(iterator pos)
		{
			CSlistNodeBase * pTempNode = getPrevious(pos._M_node);
			return iterator(eraseAfterNode(pTempNode));
		}

		iterator erase(iterator first, iterator last)
		{
			CSlistNodeBase * pTempNode = getPrevious(first._M_node);
			CSlistNodeBase* pLast = last._M_node;
			for (; pTempNode != pLast; )
			{
				pTempNode = eraseAfterNode(pTempNode);
			}

			return last;
		}

		void clear()
		{
			while(mHead.mpNext != NULL)
			{
				eraseAfterNode(&mHead);
			}
		}

		void reverse()
		{
			CSlistNodeBase* pHead = mHead.mpNext;
			CSlistNodeBase* pTemp = NULL;
			CSlistNodeBase* pNext = NULL;
			for (; NULL != pHead; )
			{
				pNext = pHead->mpNext;
				pHead->mpNext = pTemp;
				pTemp = pHead;
				pHead = pNext;
			}
			mHead.mpNext = pTemp;
		}

		void remove(const value_type& val)
		{
			CSlistNodeBase* pHead = mHead.mpNext;
			CSlistNodeBase* pPrev = &mHead;
			for (; NULL != pHead; )
			{
				if (((CSlistNode<T>*)pHead)->mData == val)
				{
					pHead = eraseAfterNode(pPrev);
				}
				else
				{
					pPrev = pHead;
					pHead = pHead->mpNext;
				}
			}
		}

		void unique()
		{
			CSlistNodeBase* pBefore = mHead.mpNext;
			if (NULL == pBefore)
			{
				return;
			}
			CSlistNodeBase* pAfter = pBefore->mpNext;
			for (; pAfter != NULL;)
			{
				if (((CSlistNode<T>*)pBefore)->mData == ((CSlistNode<T>*)pAfter)->mData)
				{
					pAfter = eraseAfterNode(pBefore);
				}
				else
				{
					pBefore = pAfter;
					pAfter = pAfter->mpNext;
				}
			}
		}

	private:
		void initWithInitialValue(size_type nSize, const value_type& value)
		{
			CSlistNodeBase* tTempNodeBase = &mHead;
			for (uint i = 0; i < nSize; ++ i)
			{
				tTempNodeBase = insertAfter(tTempNodeBase, value);
			}
		}

		CSlistNodeBase* eraseAfterNode(CSlistNodeBase* pPreNode)
		{
			CSlistNode<T>* pCurrNode = (CSlistNode<T>*)(pPreNode->mpNext);
			pPreNode->mpNext = pCurrNode->mpNext;
#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			comdeconstruct<T, std::has_trivial_destructor<T>::value>::deconstruct(&(((CSlistNode<T>*)pCurrNode)->mData));
#else
			// gcc has no std::has_trivial_destructor<T>::value
			comdeconstruct<T, false>::deconstruct(&(((CSlistNode<T>*)pCurrNode)->mData));
#endif
			mAlloc.free(pCurrNode);
			return pPreNode->mpNext;
		}

		CSlistNodeBase* insertAfter(CSlistNodeBase* pPreNode, const value_type& value)
		{
			CSlistNode<T>* pNode = mAlloc.allocate();
			if (NULL == pNode)
			{
				return NULL;
			}
#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			comconstruct<T, std::has_trivial_constructor<T>::value>::construct(&(pNode->mData), value);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			comconstruct<T, false>::construct(&(pNode->mData), value);
#endif
			pNode->mpNext = pPreNode->mpNext;
			pPreNode->mpNext = pNode;
			return pNode;
		}

		CSlistNodeBase* getPrevious(CSlistNodeBase* pNode)
		{
			CSlistNodeBase* pHead = mHead.mpNext;
			for (; NULL != pHead; pHead = pHead->mpNext)
			{
				if (pHead->mpNext == pNode)
				{
					return pHead;
				}
			}

			return NULL;
		}

		size_type getSize() const
		{
			size_type nSize = 0;
			CSlistNodeBase* pHead = mHead.mpNext;
			for (; NULL != pHead; pHead = pHead->mpNext)
			{
				++ nSize;
			}

			return nSize;
		}

	protected:
		CSlistNodeBase mHead;
		CBlockMemory<CSlistNode<T>, BaseCount, Increment>	mAlloc;
	};
}
#endif