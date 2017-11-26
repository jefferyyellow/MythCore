#ifndef __PERFORMANCE_H__
#define __PERFORMANCE_H__
#include "commontype.h"
#include "timemanager.h"
#include "singleton.h"

namespace Myth
{
#define PERFOR_TIMER_BEFORE(name) static CPerforNode _PerNode##name(#name); _PerNode##name.EnterCode();
#define PERFOR_TIMER_AFTER(name) _PerNode##name.LeaveCode();

	/// auto timer,print the timer result when the obj destruct 
#define PERFOR_AUTO_TIMER(name)	CAutoClockTime _Clock##name(#name);

	class CPerforNode
	{
	public:
		CPerforNode(const char* pName)
		{
			strncpy(mName, pName, sizeof(mName) - 1);
			mName[sizeof(mName) - 1] = '\0';
			mParentNode = NULL;
			mFirstSonNode = NULL;
			mLastSonNode = NULL;
			mNextSiblingNode = NULL;
			mCallNum = 0;
			mMaxTime = 0;
			mMinTime = 0xFFFFFFFF;
			mAveTime = 0;
			mTotalTime = 0;
		}
		~CPerforNode()
		{
		}

	public:
		/// Start timer
		void		EnterCode();
		/// Stop timer
		void		LeaveCode();
		/// print the timer statistics result
		void		Print(int nLevel);

	private:
		/// parent of the node
		CPerforNode*	mParentNode;
		/// first son of the node
		CPerforNode*	mFirstSonNode;
		/// last son of the node
		CPerforNode*	mLastSonNode;
		/// next sibling of the node
		CPerforNode*	mNextSiblingNode;
		/// clock timer of the node
		CClockTime		mClockTime;
		/// name of the node
		char			mName[STRING_LENGTH_32];
		/// visit times
		int				mCallNum;
		/// max time of the clock timer record
		int				mMaxTime;
		/// min time of the clock timer record
		int				mMinTime;
		/// min time of the clock timer record(mTotalTime/mCallNum)
		int				mAveTime;
		/// total time of the clock timer record
		int				mTotalTime;
	};

	class CPerformance : public CSingleton < CPerformance >
	{
		friend class CSingleton < CPerformance > ;
	public:
		CPerformance()
			:mRootNode("root")
		{
			mCurrNode = &mRootNode;
		}
		~CPerformance()
		{}

	public:
		/// print all timer statistics result
		void PrintResult();

	public:
		/// get the root node
		CPerforNode&	GetRootNode(){ return mRootNode; }

		/// get the curr node
		const CPerforNode*	GetCurrNode() const{ return mCurrNode; }
		/// set the curr node
		void				SetCurrNode(CPerforNode* pNode){ mCurrNode = pNode; }

	private:
		/// the root of performance tree
		CPerforNode	mRootNode;
		/// node which being operated
		CPerforNode*	mCurrNode;
	};
}


#endif
