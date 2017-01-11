#ifndef __HASHMAP_H__
#define __HASHMAP_H__
#include "commontype.h"
#include "iterator.h"
#include "blockmemory.h"
namespace Myth
{
	template<typename Key, typename Value, int BaseCount, int BucketNum, int Increment>
	class CHashMap
	{
	public:
		class CHashNodeBase
		{
		public:
			CHashNodeBase()
				:mpPrev(NULL),mpNext(NULL)
			{}
		public:
			CHashNodeBase*	mpPrev;
			CHashNodeBase*	mpNext;
		};

		class KeyValuePairNode : public CHashNodeBase
		{
		public:
			KeyValuePairNode(){}
			KeyValuePairNode(const Key& k, const Value& v)
				:mKey(k),mData(v),mpHashNext(NULL),mpHashPrev(NULL)
			{

			}
			KeyValuePairNode(const Key& k)
				:mKey(k),mpHashNext(NULL),mpHashPrev(NULL)
			{

			}
			~KeyValuePairNode()
			{
				mpHashPrev = NULL;
				mpHashNext = NULL;
			};

			KeyValuePairNode*	mpHashPrev;
			KeyValuePairNode*	mpHashNext;
			Key					mKey;
			Value				mData;
		};



		/**
		*  @brief A list::iterator.
		*
		*  All the functions are op overloads.
		*/
		struct HashIterator
		{
			typedef HashIterator					Self;

			typedef ptrdiff_t                       difference_type;
			typedef MythInternal::bidirectional_iterator_tag iterator_category;
			typedef KeyValuePairNode 				value_type;
			typedef KeyValuePairNode*				pointer;
			typedef KeyValuePairNode&				reference;

			HashIterator()
				: mNode() { }

			explicit
				HashIterator(CHashNodeBase* node)
				: mNode((KeyValuePairNode*)node) { }

			pointer
				operator->() const
			{ return mNode; }

			reference
				operator*() const
			{ return *mNode; }

			Self&
				operator++()
			{
				mNode = (KeyValuePairNode*)(mNode->mpNext);
				return *this;
			}

			Self
				operator++(int)
			{
				Self tmp = *this;
				mNode = (KeyValuePairNode*)(mNode->mpNext);
				return tmp;
			}

			Self&
				operator--()
			{
				mNode = (KeyValuePairNode*)(mNode->mpPrev);
				return *this;
			}

			Self
				operator--(int)
			{
				Self tmp = *this;
				mNode = (KeyValuePairNode*)(mNode->mpPrev);
				return tmp;
			}

			bool
				operator==(const Self& __x) const
			{ return mNode == __x.mNode; }

			bool
				operator!=(const Self& __x) const
			{ return mNode != __x.mNode; }

			// The only member points to the %list element.
			KeyValuePairNode* mNode;
		};


		/**
		*  @brief A list::const_iterator.
		*
		*  All the functions are op overloads.
		*/
		struct HashConstIterator
		{
			typedef HashConstIterator		        Self;

			typedef ptrdiff_t                          difference_type;
			typedef MythInternal::bidirectional_iterator_tag    iterator_category;
			typedef KeyValuePairNode  				   value_type;
			typedef const KeyValuePairNode*            pointer;
			typedef const KeyValuePairNode&            reference;

			HashConstIterator()
				: mNode() { }

			explicit
				HashConstIterator(const CHashNodeBase* node)
				: mNode((KeyValuePairNode*)node) { }

			HashConstIterator(HashIterator& x)
				: mNode(x.mNode) { }

			pointer
				operator->() const
			{ return mNode; }

			reference
				operator*() const
			{ return *mNode; }

			Self&
				operator++()
			{
				mNode = (KeyValuePairNode*)(mNode->mpNext);
				return *this;
			}

			Self
				operator++(int)
			{
				Self tmp = *this;
				mNode = (KeyValuePairNode*)(mNode->mpNext);
				return tmp;
			}

			Self&
				operator--()
			{
				mNode = (KeyValuePairNode*)(mNode->mpPrev);
				return *this;
			}

			Self
				operator--(int)
			{
				Self tmp = *this;
				mNode = (KeyValuePairNode*)(mNode->mpPrev);
				return tmp;
			}

			bool
				operator==(const Self& x) const
			{ return mNode == x.mNode; }

			bool
				operator!=(const Self& x) const
			{ return mNode != x.mNode; }

			// The only member points to the %list element.
			const KeyValuePairNode* mNode;
		};

		typedef HashIterator								iterator;
		typedef HashConstIterator							const_iterator;
		typedef MythInternal::reverse_iterator<const_iterator>      const_reverse_iterator;
		typedef MythInternal::reverse_iterator<iterator>            reverse_iterator;
		typedef size_t                                     size_type;
		typedef ptrdiff_t                                  difference_type;
		typedef KeyValuePairNode PairNode;


		CHashMap(void)
		{
			for(int i = 0; i < BucketNum; i++)
			{
				m_pBucket[i] = NULL;
			}
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;
		};
		~CHashMap(void){};


		iterator begin(){ return iterator(mBaseNode.mpNext); }
		const_iterator begin() const{ return const_iterator(mBaseNode.mpNext);}

		iterator end(){ return iterator(&mBaseNode); }
		const_iterator end() const{ return const_iterator(&mBaseNode); }

		reverse_iterator rbegin(){ return reverse_iterator(end()); }
		reverse_iterator rend(){ return reverse_iterator(begin()); }

		// 插入键值对
		bool Insert(const Key& k, const Value& v)
		{
			unsigned int hashValue = Hash((char*)&k, sizeof(Key));
			int nBucket = hashValue % BucketNum;
			// 健值已经存在，把这个去了就是multihashmap了
			if (KeyIsExit(k, nBucket))
			{
				return false;
			}

			KeyValuePairNode* pNode = mAlloc.allocate();
			if (NULL == pNode)
			{
				return false;
			}
			pNode->mKey = k;

#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			//comconstruct<Value, std::has_trivial_constructor<Value>::value>::construct(&(pNode->mData), v);
			//pNode->KeyValuePairNode(k,v);
			new (pNode) KeyValuePairNode(k, v);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			new (pNode) KeyValuePairNode(k, v);
#endif
			AddBucket(pNode, nBucket);
			return true;
		}

		// 插入一个空白值
		PairNode* Insert(const Key& k)
		{
			unsigned int hashValue = Hash((char*)&k, sizeof(Key));
			int nBucket = hashValue % BucketNum;
			

			KeyValuePairNode* pNode = mAlloc.allocate();
			if (NULL == pNode)
			{
				return NULL;
			}
			pNode->mKey = k;
#ifdef MYTH_OS_WINDOWS
			// vs2010 or above
			new (pNode) KeyValuePairNode(k);
#else
			// gcc has no std::has_trivial_constructor<T>::value
			new (pNode) KeyValuePairNode(k);
#endif
			AddBucket(pNode, nBucket);
			return pNode;
		}

		// 找到一个键值
		bool Find(const Key &k, Value& v)
		{
			unsigned int hashValue = Hash((char*)&k, sizeof(Key));
			int nBucket = hashValue % BucketNum;
			PairNode* pNode = GetValue(k, nBucket);
			if (NULL == pNode)
			{
				return false;
			}
			v = pNode->mData;
			return true;
		}

		// 找到一个键值
		PairNode* Find(const Key &k)
		{
			unsigned int hashValue = Hash((char*)&k, sizeof(Key));
			int nBucket = hashValue % BucketNum;
			PairNode* pNode = GetValue(k, nBucket);
			//assert(NULL != pNode);
			return pNode;
		}

		// 在某个桶位加上元素
		void AddBucket(PairNode* pNode, int nBucket)
		{
			pushBackListNode(pNode);
			if (m_pBucket[nBucket] == NULL)
			{
				m_pBucket[nBucket] = pNode;
			}
			else
			{
				pNode->mpHashNext =  m_pBucket[nBucket];
				m_pBucket[nBucket]->mpHashPrev = pNode;
				pNode->mpHashPrev =  NULL;
				m_pBucket[nBucket] = pNode;
			}
		}



		// 得到某个桶位上的元素列表
		PairNode* GetBucket(int nBucket)
		{
			return m_pBucket[nBucket];
		}


		// 使用经典的times33算法，如果有其它好的算法再替换
		unsigned int Hash(const char* pMem, size_t nLength)
		{
			unsigned int nHash = 0;
			for (unsigned int i = 0; i < nLength; i++)
			{
				nHash = (nHash<<5) + nHash + *pMem++;
			}
			return nHash;
		}

		// 键值是否存在
		bool KeyIsExit(const Key& k, int nBucket)
		{
			PairNode* pNode = GetBucket(nBucket);
			if (NULL == pNode)
			{
				return false;
			}

			for (; NULL != pNode; pNode = pNode->mpHashNext)
			{
				if(pNode->mKey == k)
				{
					return true;
				}
			}
			return false;
		}

		// 得到键值
		PairNode* GetValue(const Key& k, int nBucket)
		{
			PairNode* pNode = GetBucket(nBucket);
			if (NULL == pNode)
			{
				return pNode;
			}

			for (; NULL != pNode; pNode = pNode->mpHashNext)
			{
				if(pNode->mKey == k)
				{
					return pNode;
				}
			}
			return NULL;
		}

		PairNode* EraseNodeByKey(const Key& k, int nBucket)
		{
			PairNode* pBucketNode = GetValue(k, nBucket);
			if (NULL == pBucketNode)
			{
				return NULL;
			}

			pBucketNode->mpNext->mpPrev = pBucketNode->mpPrev;
			pBucketNode->mpPrev->mpNext = pBucketNode->mpNext;

			if(NULL != pBucketNode->mpHashNext)
			{
				pBucketNode->mpHashNext->mpHashPrev = pBucketNode->mpHashPrev;
			}
			if(NULL != pBucketNode->mpHashPrev)
			{
				pBucketNode->mpHashPrev->mpHashNext = pBucketNode->mpHashNext;
			}
			if (pBucketNode == m_pBucket[nBucket])
			{
				if (NULL != pBucketNode->mpHashNext)
				{
					m_pBucket[nBucket] = pBucketNode->mpHashNext;
				}
				else
				{
					m_pBucket[nBucket] = NULL;
				}
			}

			return pBucketNode;
		}

		// 根据删除某个键对应的节点
		bool erase(const Key& k)
		{
			unsigned int hashValue = Hash((char*)k, sizeof(Key));
			int nBucket = hashValue % BucketNum;
			PairNode* pNode = EraseNodeByKey(k, nBucket);
			if(pNode == NULL)
			{
				return false;
			}
			mAlloc.free(pNode);
			return true;
		}

		//Value& operator[](const Key& k)
		//{
		//	PairNode* pNode = Find(k);
		//	if (NULL == pNode)
		//	{
		//		pNode = Insert(k);
		//	}
		//	if (NULL == pNode)
		//	{
		//		return mFailure;
		//	}
		//	return pNode->mData;
		//}

		void Clear()
		{
			CHashNodeBase* pNode = mBaseNode.mpNext;
			CHashNodeBase* pEndNode = &mBaseNode;
			CHashNodeBase* pOld = NULL;
			for (; pNode != pEndNode;)
			{
				pOld = pNode;
				pNode = pNode->mpNext;
				mAlloc.free((PairNode*)pOld);
			}

			for(int i = 0; i < BucketNum; i++)
			{
				m_pBucket[i] = NULL;
			}
			mBaseNode.mpNext = &mBaseNode;
			mBaseNode.mpPrev = &mBaseNode;
		}

private:
		void pushBackListNode(PairNode* pInsertNode)
		{
			inserListBeforeNode(&mBaseNode, pInsertNode);
		} 

		void inserListBeforeNode(CHashNodeBase* pBaseNode, CHashNodeBase* pInsertNode)
		{
			pBaseNode->mpPrev->mpNext = pInsertNode;
			pInsertNode->mpPrev = pBaseNode->mpPrev;
			pInsertNode->mpNext = pBaseNode;
			pBaseNode->mpPrev = pInsertNode;
		}
private:
	CBlockMemory<KeyValuePairNode, BaseCount, Increment>	mAlloc;
	KeyValuePairNode*	m_pBucket[BucketNum];
	CHashNodeBase			mBaseNode;
	};

	//template<typename Key, typename Value, int BaseCount, int BucketNum, int Increment>
	//inline bool
	//	operator==(CHashMap<Key, Value, BaseCount, BucketNum, Increment>::HashIterator& __x,
	//	CHashMap<Key, Value, BaseCount, BucketNum, Increment>::HashConstIterator& __y);

	//template<typename _Val>
	//inline bool
	//	operator!=(const _List_iterator<_Val>& __x,
	//	const _List_const_iterator<_Val>& __y)
	//{ return __x._M_node != __y._M_node; }
}

#endif