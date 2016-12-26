#ifndef __LIST_H__
#define __LIST_H__
#include "iterator.h"
#include "commontype.h"
#include "blockmemory.h"
#ifdef MYTH_OS_WINDOWS
#include <type_traits>
#endif
namespace Myth
{
	class CListNodeBase
	{
	public:
		CListNodeBase*	mpPrev;
		CListNodeBase*	mpNext;
	};

	template<typename T>
	class CListNode : public CListNodeBase
	{
	public:
		T	mData;
	};

	/**
	*  @brief A list::iterator.
	*
	*  All the functions are op overloads.
	*/
	template<typename T>
	struct _List_iterator
	{
		typedef _List_iterator<T>					Self;
		typedef CListNode<T>						Node;

		typedef ptrdiff_t                         difference_type;
		typedef MythInternal::bidirectional_iterator_tag   iterator_category;
		typedef T									value_type;
		typedef T*								pointer;
		typedef T&								reference;

		_List_iterator()
			: _M_node() { }

		explicit
			_List_iterator(CListNodeBase* __x)
			: _M_node(__x) { }

		// Must downcast from List_node_base to _List_node to get to mData.
		reference
			operator*() const
		{ return static_cast<Node*>(_M_node)->mData; }

		pointer
			operator->() const
		{ return &static_cast<Node*>(_M_node)->mData; }

		Self&
			operator++()
		{
			_M_node = _M_node->mpNext;
			return *this;
		}

		Self
			operator++(int)
		{
			Self __tmp = *this;
			_M_node = _M_node->mpNext;
			return __tmp;
		}

		Self&
			operator--()
		{
			_M_node = _M_node->mpPrev;
			return *this;
		}

		Self
			operator--(int)
		{
			Self __tmp = *this;
			_M_node = _M_node->mpPrev;
			return __tmp;
		}

		bool
			operator==(const Self& __x) const
		{ return _M_node == __x._M_node; }

		bool
			operator!=(const Self& __x) const
		{ return _M_node != __x._M_node; }

		// The only member points to the %list element.
		CListNodeBase* _M_node;
	};


	/**
	*  @brief A list::const_iterator.
	*
	*  All the functions are op overloads.
	*/
	template<typename T>
	struct _List_const_iterator
	{
		typedef _List_const_iterator<T>          Self;
		typedef const CListNode<T>              Node;
		typedef _List_iterator<T>                iterator;

		typedef ptrdiff_t                          difference_type;
		typedef MythInternal::bidirectional_iterator_tag    iterator_category;
		typedef T									 value_type;
		typedef const T*                           pointer;
		typedef const T&                           reference;

		_List_const_iterator()
			: _M_node() { }

		explicit
			_List_const_iterator(const CListNodeBase* __x)
			: _M_node(__x) { }

		_List_const_iterator(const iterator& __x)
			: _M_node(__x._M_node) { }

		// Must downcast from List_node_base to _List_node to get to
		// mData.
		reference
			operator*() const
		{ return static_cast<Node*>(_M_node)->mData; }

		pointer
			operator->() const
		{ return &static_cast<Node*>(_M_node)->mData; }

		Self&
			operator++()
		{
			_M_node = _M_node->mpNext;
			return *this;
		}

		Self
			operator++(int)
		{
			Self __tmp = *this;
			_M_node = _M_node->mpNext;
			return __tmp;
		}

		Self&
			operator--()
		{
			_M_node = _M_node->mpPrev;
			return *this;
		}

		Self
			operator--(int)
		{
			Self __tmp = *this;
			_M_node = _M_node->mpPrev;
			return __tmp;
		}

		bool
			operator==(const Self& __x) const
		{ return _M_node == __x._M_node; }

		bool
			operator!=(const Self& __x) const
		{ return _M_node != __x._M_node; }

		// The only member points to the %list element.
		const CListNodeBase* _M_node;
	};

	template<typename _Val>
	inline bool
		operator==(const _List_iterator<_Val>& __x,
		const _List_const_iterator<_Val>& __y)
	{ return __x._M_node == __y._M_node; }

	template<typename _Val>
	inline bool
		operator!=(const _List_iterator<_Val>& __x,
		const _List_const_iterator<_Val>& __y)
	{ return __x._M_node != __y._M_node; }

	template<typename T, uint BaseCount=32, uint Increment=8>
	class CList
	{
	public:
		typedef T										   value_type;
		typedef value_type*					   pointer;
		typedef value_type&					   reference;
		typedef const value_type*		   		   const_pointer;
		typedef const value_type&				   const_reference;
		typedef _List_iterator<T>						   iterator;
		typedef _List_const_iterator<T>					   const_iterator;
		typedef MythInternal::reverse_iterator<const_iterator>      const_reverse_iterator;
		typedef MythInternal::reverse_iterator<iterator>            reverse_iterator;
		typedef size_t                                     size_type;
		typedef ptrdiff_t                                  difference_type;

	public:
		CList()
		{
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;
		}

		CList(int nSize, const value_type& value = value_type())
		{
			initWithInitialValue((size_type)nSize, value);
		}

		CList(size_type nSize, const value_type& value = value_type())
		{
			initWithInitialValue(nSize, value);
		}

		CList(const CList& list)
		{
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;
			const_iterator it = list.begin();
			const_iterator end = list.end();
			for (; it != end; ++ it)
			{
				push_back(*it);
			}
		}

		template<typename _InputIterator>
		CList(_InputIterator __first, _InputIterator __last)
		{ 
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;
			for (; __first != __last; ++ __first)
			{
				push_back(*__first);
			}
		}

		CList& operator=(const CList& __x)
		{
			clear();
			const_iterator it = __x.begin();
			const_iterator end = __x.end();
			for (; it != end; ++ it)
			{
				push_back(*it);
			}
			return *this;
		}

		~CList()
		{
			clear();
		}

		void assign(sint nSize, const value_type& rVal)
		{
			assign(size_type(nSize), rVal);
		}

		void assign(size_type nSize, const value_type& rVal)
		{
			iterator i = begin();
			for (; i != end() && nSize > 0; ++ i, -- nSize)
			{
				*i = rVal;
			}

			if (nSize > 0)
			{
				insert(end(), nSize, rVal);
			}
			else
			{
				erase(i, end());
			}
		}

		template<typename _InputIterator>
		void assign(_InputIterator itstart, _InputIterator itend)
		{
			iterator dstIt = begin();
			iterator dstEndIt = end();
			for (; dstIt != dstEndIt && itstart != itend; ++dstIt, ++itstart)
			{
				*dstIt = *itstart;
			}
			if (itstart == itend)
			{
				erase(dstIt, dstEndIt);
			}
			else
			{
				insert(dstIt, itstart, itend);
			}
		}

		void clear()
		{
			CListNodeBase* pCurrNode = mBaseNode.mpNext;
			while(pCurrNode != &mBaseNode)
			{
				eraseNode((CListNode<T>*)pCurrNode);
				pCurrNode = mBaseNode.mpNext;
			}
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;
		}

	public:
		iterator begin(){ return iterator(mBaseNode.mpNext); }
		const_iterator begin() const{ return const_iterator(mBaseNode.mpNext);}

		iterator end(){ return iterator(&mBaseNode); }
		const_iterator end() const{ return const_iterator(&mBaseNode); }

		reverse_iterator rbegin(){ return reverse_iterator(end()); }
		reverse_iterator rend(){ return reverse_iterator(begin()); }

		bool empty() const{ return mBaseNode.mpNext == &mBaseNode; }

		reference front(){ return *begin(); }
		const_reference front() const{ return *begin(); }

		reference back()
		{ 
			iterator tmp = end();
			--tmp;
			return *tmp;
		}

		const_reference back() const
		{ 
			const_iterator tmp = end();
			--tmp;
			return *tmp;
		}

		void push_back(const value_type& value)
		{
			CListNode<T>* pNode = mAlloc.allocate();
			if (NULL == pNode)
			{
				return;
			}

#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			comconstruct<T, std::has_trivial_constructor<T>::value>::construct(&(pNode->mData), value);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			comconstruct<T, false>::construct(&(pNode->mData), value);
#endif
			inserBeforeNode(end()._M_node, pNode);
		}

		void pop_back()
		{
			CListNodeBase* pCurrNode = mBaseNode.mpPrev;
			eraseNode((CListNode<T>*)pCurrNode);
		}

		void push_front(const value_type& value)
		{
			CListNode<T>* pNode = mAlloc.allocate();
			if (NULL == pNode)
			{
				return;
			}

#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			comconstruct<T, std::has_trivial_constructor<T>::value>::construct(&(pNode->mData), value);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			comconstruct<T, false>::construct(&(pNode->mData), value);
#endif
			inserBeforeNode(begin()._M_node, pNode);
		}

		void pop_front()
		{
			CListNodeBase* pCurrNode = begin()._M_node;
			eraseNode((CListNode<T>*)pCurrNode);
		}

		iterator insert(iterator position, const value_type& rValue)
		{
			CListNode<T>* pNode = mAlloc.allocate();
			if (NULL == pNode)
			{
				return iterator(NULL);
			}

#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			comconstruct<T, std::has_trivial_constructor<T>::value>::construct(&(pNode->mData), rValue);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			comconstruct<T, false>::construct(&(pNode->mData), rValue);
#endif
			inserBeforeNode(position._M_node, pNode);
			return iterator(pNode);
		}

		void insert(iterator position, sint n, const value_type& value)
		{
			insert(position, (size_type)n, value);
		}

		void insert(iterator position, size_type n, const value_type& value)
		{
			iterator it = position;
			for (uint i = 0; i < n; ++ i)
			{
				it = insert(it, value);
			}
		}

		template<typename _InputIterator>
		void insert(iterator position, _InputIterator first, _InputIterator last)
		{
			iterator it = position;
			for (; first != last; ++ first)
			{
				it = insert(it, *first);
				++ it;
			}
		}

		iterator erase(iterator position)
		{
			iterator it = position++;
			eraseNode((CListNode<T>*)(it._M_node));
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator it = first;
			while(first != last)
			{
				it = first;
				++ first;
				eraseNode((CListNode<T>*)(it._M_node));
			}

			return last;
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
					eraseNode((CListNode<T>*)(tTemp._M_node));
				}
				else
				{
					++ it;
				}
			}
		}

		void unique()
		{
			iterator it = begin();
			iterator itEnd = end();
			if (it == itEnd)
			{
				return;
			}
			iterator tTemp = it;
			while(++ tTemp != itEnd)
			{
				if (*tTemp == *it)
				{
					eraseNode((CListNode<T>*)(tTemp._M_node));
					tTemp = it;
				}
				else
				{
					it = tTemp;
				}
			}
		}

		void reverse()
		{
			CListNodeBase* pCurrNode = mBaseNode.mpNext;
			CListNodeBase* pTempNext = NULL;
			while(pCurrNode != &mBaseNode)
			{
				pTempNext = pCurrNode->mpNext;
				pCurrNode->mpNext = pCurrNode->mpPrev;
				pCurrNode->mpPrev = pTempNext;

				pCurrNode = pTempNext;
			}

			pTempNext = mBaseNode.mpNext;
			mBaseNode.mpNext = mBaseNode.mpPrev;
			mBaseNode.mpPrev = pTempNext;
		}

	private:
		void initWithInitialValue(size_type nSize, const value_type& value)
		{
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;

			for (uint i = 0; i < nSize; ++ i)
			{
				push_back(value);
			}
		}

		void linkNode(CListNodeBase* pPrevNode, CListNodeBase* pNextNode)
		{
			pPrevNode->mpNext = pNextNode;
			pNextNode->mpPrev = pPrevNode;
		}

		void inserBeforeNode(CListNodeBase* pBaseNode, CListNodeBase* pInsertNode)
		{
			pBaseNode->mpPrev->mpNext = pInsertNode;
			pInsertNode->mpPrev = pBaseNode->mpPrev;
			pInsertNode->mpNext = pBaseNode;
			pBaseNode->mpPrev = pInsertNode;
		}

		void eraseNode(CListNode<T>* pCurrNode)
		{
			linkNode(pCurrNode->mpPrev, pCurrNode->mpNext);
			mAlloc.free(pCurrNode);
		}

	private:
		CListNodeBase			mBaseNode;
		CBlockMemory<CListNode<T>, BaseCount, Increment>	mAlloc;
	};
}
#endif
