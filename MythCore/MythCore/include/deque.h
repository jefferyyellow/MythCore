#ifndef __DEQUE_H__
#define __DEQUE_H__

template<typename T, typename Ref, typename Ptr, int Size>
struct CDequeIterator
{
	typedef CDequeIterator<T, T&, T*, Size>             iterator;
	typedef CDequeIterator<T, const T&, const T*, Size> const_iterator;

	static size_t SBufferSize()
	{ return Size; }

	typedef std::random_access_iterator_tag iterator_category;
	typedef T                             value_type;
	typedef Ptr                            pointer;
	typedef Ref                            reference;
	typedef size_t                          size_type;
	typedef ptrdiff_t                       difference_type;
	typedef T**                           _Map_pointer;
	typedef CDequeIterator                 _Self;

	T* _M_cur;
	T* _M_first;
	T* _M_last;
	_Map_pointer _M_node;

	CDequeIterator(T* __x, _Map_pointer __y)
		: _M_cur(__x), _M_first(*__y),
		_M_last(*__y + SBufferSize()), _M_node(__y) {
			int nSize = sizeof(T);
	}

	CDequeIterator()
		: _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }

	CDequeIterator(const iterator& __x)
		: _M_cur(__x._M_cur), _M_first(__x._M_first),
		_M_last(__x._M_last), _M_node(__x._M_node) { }

	reference
		operator*() const
	{ return *_M_cur; }

	pointer
		operator->() const
	{ return _M_cur; }

	_Self&
		operator++()
	{
		++_M_cur;
		if (_M_cur == _M_last)
		{
			_M_set_node(_M_node + 1);
			_M_cur = _M_first;
		}
		return *this;
	}

	_Self
		operator++(int)
	{
		_Self __tmp = *this;
		++*this;
		return __tmp;
	}

	_Self&
		operator--()
	{
		if (_M_cur == _M_first)
		{
			_M_set_node(_M_node - 1);
			_M_cur = _M_last;
		}
		--_M_cur;
		return *this;
	}

	_Self
		operator--(int)
	{
		_Self __tmp = *this;
		--*this;
		return __tmp;
	}

	_Self&
		operator+=(difference_type __n)
	{
		const difference_type __offset = __n + (_M_cur - _M_first);
		if (__offset >= 0 && __offset < difference_type(_S_buffer_size()))
			_M_cur += __n;
		else
		{
			const difference_type __node_offset =
				__offset > 0 ? __offset / difference_type(_S_buffer_size())
				: -difference_type((-__offset - 1)
				/ SBufferSize()) - 1;
			_M_set_node(_M_node + __node_offset);
			_M_cur = _M_first + (__offset - __node_offset
				* difference_type(SBufferSize()));
		}
		return *this;
	}

	_Self
		operator+(difference_type __n) const
	{
		_Self __tmp = *this;
		return __tmp += __n;
	}

	_Self&
		operator-=(difference_type __n)
	{ return *this += -__n; }

	_Self
		operator-(difference_type __n) const
	{
		_Self __tmp = *this;
		return __tmp -= __n;
	}

	reference
		operator[](difference_type __n) const
	{ return *(*this + __n); }

	/** 
	*  Prepares to traverse new_node.  Sets everything except
	*  _M_cur, which should therefore be set by the caller
	*  immediately afterwards, based on _M_first and _M_last.
	*/
	void
		_M_set_node(_Map_pointer __new_node)
	{
		_M_node = __new_node;
		_M_first = *__new_node;
		_M_last = _M_first + difference_type(SBufferSize());
	}
};

//// Note: we also provide overloads whose operands are of the same type in
//// order to avoid ambiguous overload resolution when std::rel_ops operators
//// are in scope (for additional details, see libstdc++/3628)
template<typename T, typename Ref, typename Ptr, int Size>
inline bool
operator==(const CDequeIterator<T, Ref, Ptr, Size>& x,
	const CDequeIterator<T, Ref, Ptr, Size>& y)
{ return x._M_cur == y._M_cur; }

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline bool
//operator==(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		   const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{ return x._M_cur == y._M_cur; }
//
template<typename T, typename Ref, typename Ptr, int Size>
inline bool
	operator!=(const CDequeIterator<T, Ref, Ptr, Size>& x,
	const CDequeIterator<T, Ref, Ptr, Size>& y)
{ return !(x == y); }

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline bool
//operator!=(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		   const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{ return !(x == y); }
//
template<typename T, typename Ref, typename Ptr, int Size>
inline bool
	operator<(const CDequeIterator<T, Ref, Ptr, Size>& x,
	const CDequeIterator<T, Ref, Ptr, Size>& y)
{ return (x._M_node == y._M_node) ? (x._M_cur < y._M_cur)
: (x._M_node < y._M_node); }

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline bool
//operator<(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		  const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{ return (x._M_node == y._M_node) ? (x._M_cur < y._M_cur)
//: (x._M_node < y._M_node); }
//
template<typename T, typename Ref, typename Ptr, int Size>
inline bool
operator>(const CDequeIterator<T, Ref, Ptr, Size>& x,
		  const CDequeIterator<T, Ref, Ptr, Size>& y)
{ return y < x; }

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline bool
//operator>(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		  const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{ return y < x; }

template<typename T, typename Ref, typename Ptr, int Size>
inline bool
operator<=(const CDequeIterator<T, Ref, Ptr, Size>& x,
		   const CDequeIterator<T, Ref, Ptr, Size>& y)
{ return !(y < x); }

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline bool
//operator<=(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		   const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{ return !(y < x); }
//
template<typename T, typename Ref, typename Ptr, int Size>
inline bool
	operator>=(const CDequeIterator<T, Ref, Ptr, Size>& x,
	const CDequeIterator<T, Ref, Ptr, Size>& y)
{ return !(x < y); }

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline bool
//operator>=(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		   const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{ return !(x < y); }
//
//// _GLIBCXX_RESOLVE_LIB_DEFECTS
//// According to the resolution of DR179 not only the various comparison
//// operators but also operator- must accept mixed iterator/const_iterator
//// parameters.
template<typename T, typename Ref, typename Ptr, int Size>
inline typename CDequeIterator<T, Ref, Ptr, Size>::difference_type
	operator-(const CDequeIterator<T, Ref, Ptr, Size>& x,
	const CDequeIterator<T, Ref, Ptr, Size>& y)
{
	return typename CDequeIterator<T, Ref, Ptr, Size>::difference_type
		(CDequeIterator<T, Ref, Ptr, Size>::SBufferSize())
		* (x._M_node - y._M_node - 1) + (x._M_cur - x._M_first)
		+ (y._M_last - y._M_cur);
}

//template<typename T, typename RefL, typename PtrL, int SizeL,
//typename RefR, typename PtrR, int SizeR>
//inline typename CDequeIterator<T, RefL, PtrL, SizeL>::difference_type
//operator-(const CDequeIterator<T, RefL, PtrL, SizeL>& x,
//		  const CDequeIterator<T, RefR, PtrR, SizeR>& y)
//{
//	return typename CDequeIterator<T, RefL, PtrL, SizeL>::difference_type
//		(CDequeIterator<T, _RefL, _PtrL, SizeL>::SBufferSize())
//		* (x._M_node - y._M_node - 1) + (x._M_cur - x._M_first)
//		+ (y._M_last - y._M_cur);
//}

template<typename T, typename Ref, typename Ptr, int Size>
inline CDequeIterator<T, Ref, Ptr, Size>
operator+(ptrdiff_t n, const CDequeIterator<T, Ref, Ptr, Size>& x)
{ return x + n; }

template<typename T, uint BlockSize, uint MapSize>
class CDeque
{
public:
	typedef T											value_type;
	typedef value_type*									pointer;
	typedef const value_type*							const_pointer;
	typedef value_type&							        reference;
	typedef const value_type&							const_reference;
	typedef typename CDequeIterator<value_type, reference, pointer, BlockSize> iterator;
	typedef typename CDequeIterator<value_type, const_reference, const_pointer, BlockSize> const_iterator;
	typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
	typedef std::reverse_iterator<iterator>				reverse_iterator;
	typedef size_t										size_type;
	typedef ptrdiff_t									difference_type;

public:
	struct CDequeBlock
	{
		value_type	mBlock[BlockSize];
	};


public:
	CDeque():mMap(0),mMapSize(0),
		mStart(),mFinish()
	{
	}

	explicit CDeque(sint nSize, const value_type& value = value_type())
	{
		mMap = 0;
		mMapSize = 0;
		initWithInitialValue(nSize, value);
	}

	explicit CDeque(size_type nSize, const value_type& value = value_type())
	{
		mMap = 0;
		mMapSize = 0;
		initWithInitialValue(nSize, value);
	}

	CDeque(const CDeque& x)
	{
		mMap = 0;
		mMapSize = 0;
		int nOffset = initSize(x.size());
		mStart = iterator((value_type*)mMap[nOffset], (T**)&mMap[nOffset]);
		mFinish = mStart;

		const_iterator it = x.begin();
		const_iterator itEnd = x.end();
		for (; it != itEnd; ++ it)
		{
			*mFinish = *it;
			++ mFinish;
		}
	}
	
	template<typename _InputIterator>
	CDeque(_InputIterator first, _InputIterator last)
	{
		mMap = 0;
		mMapSize = 0;
		int nOffset = initSize(last - first);
		mStart = iterator((value_type*)mMap[nOffset], (T**)&mMap[nOffset]);
		mFinish = mStart;
		for (; first != last; ++ first)
		{
			*mFinish = *first;
			++ mFinish;
		}
	}

	~CDeque()
	{
	}

	CDeque& operator=(const CDeque& x)
	{
		mMap = 0;
		mMapSize = 0;
		int nOffset = initSize(x.size());
		mStart = iterator((value_type*)mMap[nOffset], (T**)&mMap[nOffset]);
		mFinish = mStart;

		const_iterator it = x.begin();
		const_iterator itEnd = x.end();
		for (; it != itEnd; ++ it)
		{
			*mFinish = *it;
			++ mFinish;
		}
	}

	iterator begin()
	{ return mStart; }

	const_iterator begin() const
	{ return mStart; }

	iterator end()
	{ return mFinish; }

	const_iterator end() const
	{ return mFinish; }

	reverse_iterator rbegin()
	{ return reverse_iterator(mFinish); }

	const_reverse_iterator rbegin() const
	{ return const_reverse_iterator(mFinish); }

	reverse_iterator rend()
	{ return reverse_iterator(mStart); }

	const_reverse_iterator rend() const
	{ return const_reverse_iterator(mStart); }

	size_type size() const
	{ return mFinish - mStart; }

	bool empty() const
	{ return mFinish == mStart; }

	reference operator[](size_type n)
	{ return mStart[difference_type(n)]; }
	
	const_reference operator[](size_type n) const
	{ return mStart[difference_type(n)]; }

	reference at(size_type n)
	{
		return (*this)[n];
	}

	const_reference at(size_type n) const
	{
		return (*this)[n];
	}
	
	reference front()
	{ return *begin(); }

	const_reference front() const
	{ return *begin(); }

	reference back()
	{
		iterator tmp = end();
		--tmp;
		return *tmp;
	}

	const_reference
		back() const
	{
		const_iterator tmp = end();
		--tmp;
		return *tmp;
	}

	void push_front(const value_type& x)
	{

	}

	void push_back(const value_type& x)
	{

	}

	void pop_front()
	{

	}

	void pop_back()
	{

	}

	iterator insert(iterator position, const value_type& x)
	{

	}

	void insert(iterator position, size_type n, const value_type& x)
	{

	}

	template<typename InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{

	}

	iterator erase(iterator __position)
	{

	}

	iterator erase(iterator __first, iterator __last)
	{

	}

	void clear()
	{

	}
protected:
	int initSize(size_type nSize)
	{
		int nBlockNum = (nSize - 1) / BlockSize + 1;
		int nNum = nSize % BlockSize;
		return initBlockSize(nBlockNum);
	}

	int initBlockSize(uint32 nBlockNum)
	{
		int nMapSize = ((nBlockNum - 1) / MapSize + 1) * MapSize;
		resizeMap(nMapSize);

		int nOffset = (nMapSize - nBlockNum) >> 1;
		int nTempOffset = nOffset;
		for (uint i = 0; i < nBlockNum; ++ i)
		{
			CDequeBlock* pBlock = (CDequeBlock*)mAlloc.allocate();
			mMap[nTempOffset] = (value_type*)pBlock;
			++ nTempOffset;
		}
		return nTempOffset;
	}

	void initWithInitialValue(size_type nSize, const value_type& value)
	{
		if (nSize <= 0)
		{
			return;
		}
		int nBlockNum = (nSize - 1) / BlockSize + 1;
		int nNum = nSize % BlockSize;
		int nOffset = initBlockSize(nBlockNum);
		int nTempOffset = nOffset;
		if (nNum > 0)
		{
			for (int i = 0; i < nBlockNum - 1; ++ i)
			{
				initBlock(mMap[nTempOffset], BlockSize, value);
				++ nTempOffset;
			}
			initBlock(mMap[nTempOffset], nNum, value);
		}
		else
		{
			for (int i = 0; i < nBlockNum; ++ i)
			{
				initBlock(mMap[nTempOffset], BlockSize, value);
				++ nTempOffset;
			}
		}
		mStart = iterator((value_type*)mMap[nOffset], (T**)&mMap[nOffset]);
		mFinish = iterator((value_type*)mMap[nTempOffset] + nNum, (T**)&mMap[nTempOffset]);
	}

	void	initBlock(value_type* pBlock, uint32 nNodeNum, const value_type& value)
	{
		if (NULL == pBlock)
		{
			return;
		}
		value_type* pNode = (value_type*)pBlock;
		for (uint i = 0; i < nNodeNum; ++ i)
		{
#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			comconstruct<T, std::has_trivial_constructor<T>::value>::construct(pNode, value);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			comconstruct<T, false>::construct(pNode, value);
#endif
			++ pNode;
		}
	}

	void	resizeMap(uint32 nMapSize)
	{
		if (NULL != mMapSize)
		{
			delete []mMap;
		}
		mMap = new T* [nMapSize];
		mMapSize = nMapSize;
	}

private:
	T** mMap;
	size_t mMapSize;
	iterator mStart;
	iterator mFinish;
	CBlockMemory<CDequeBlock, 1, 1>	mAlloc;
};
#endif