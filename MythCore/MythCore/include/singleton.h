#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include <stddef.h>
namespace Myth
{
	/**
	 * Singleton design pattern
	 * 
	 * for instance
	 * \code 
		// class declaration
		class CTimeManager : public CSingleton<CTimeManager>
		{
			private:
				CTimeManager();
				~CTimeManager();
		};

		// call CreateInst before first use
		CTimeManager::CreateInst();
		...
		sint64 tNow = CTimeManager::Inst()->GetMSTime();
		...
		// call DestroyInst if nerver use CTimeManager
		CTimeManager::DestroyInst();
	 * \endcode 
	 * \author jefferyyellow
	 * \date 2014
	 */

	template<typename T>
	class CSingleton
	{
	protected:
		CSingleton()
		{
			mInst = static_cast<T*>(this);
		};
		~CSingleton(){}
	
	public:
		/// get the singleton instance
		static T*	Inst()
		{
			return mInst;
		}
		/// create the singleton instance
		static T*   createInst()
		{
			if (mInst == NULL)
			{
				return new T;
			}
			return mInst;
		}
		/// destroy the singleton instance
		static void destroyInst()
		{
			if (NULL != mInst)
			{
				delete mInst;
				mInst = NULL;
			}
		}
	private:
		static T*	mInst;
	};

	template< typename T >
	T* CSingleton<T>::mInst = NULL;

	//#define NEW_INST(SingletonClass) \
	//	static SingletonClass* NewInst() \
	//	{ \
	//		if (NULL == SingletonClass::Inst()) \
	//		{ \
	//			return new SingletonClass; \
	//		} \
	//		return SingletonClass::Inst(); \
	//	}
	//
	//#define DESTROY_INST(SingletonClass) \
	//	static void DestroyInst() \
	//	{ \
	//		if (NULL != SingletonClass::Inst()) \
	//		{ \
	//			delete SingletonClass::Inst(); \
	//		} \
	//	}

}

#endif
