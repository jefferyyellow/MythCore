#ifndef __SORTEDVECTOR_H__
#define __SORTEDVECTOR_H__
#include <vector>
#include <algorithm>
using namespace std;
template<typename T, typename Compare = std::less<T>>
class CSortedVector
{
public:
	typedef typename std::vector<T>::iterator			iterator;
	typedef typename std::vector<T>::const_iterator		const_iterator;
	typedef typename std::vector<T>::size_type			size_type;

	iterator			begin()			{ return mSortedVector.begin(); }
	const_iterator		begin() const	{ return mSortedVector.cbegin(); }
	iterator			end()			{ return mSortedVector.end(); }
	const_iterator		end()  const	{ return mSortedVector.cend(); }
	void				reserve(size_type sz) { mSortedVector.reserve(sz); }
	size_type			size(){return mSortedVector.size();}

	iterator insert(const T& elem)
	{
		iterator it = std::upper_bound(begin(), end(), elem, Compare());
		return mSortedVector.insert(it, elem);
	}

	const_iterator find(const T& elem) const
	{
		std::pair<const_iterator, const_iterator> tPair = std::equal_range(begin(), end(), elem, Compare());
		if (tPair.first != tPair.second)
		{
			return tPair.first;
		}

		return end();
	}

	void pushBack(const T& elem)
	{
		mSortedVector.push_back(elem);
	}

	void sort()
	{
		std::sort(begin(), end(), Compare());
	}

	iterator erase(const T& elem)
	{
		const_iterator it = find(elem);
		if (it != end())
		{
			return end();
		}

		return mSortedVector.erase(it);
	}

	T& operator[](size_type Pos)
	{
		return mSortedVector[Pos];
	}

private:
	vector<T> mSortedVector;
};
#endif