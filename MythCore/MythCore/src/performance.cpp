#include "performance.h"
#include "logmanager.h"


namespace Myth
{
	void CPerforNode::EnterCode()
	{
		// increment the call times
		++mCallNum;
		// if the node is not in the tree
		if (NULL == mParentNode)
		{
			mParentNode = const_cast<CPerforNode*>(CPerformance::Inst()->GetCurrNode());
			CPerforNode* pLastBrother = mParentNode->mLastSonNode;
			// the parenet node has no children
			if (NULL == pLastBrother)
			{
				mParentNode->mFirstSonNode = this;
				mParentNode->mLastSonNode = this;
			}
			else
			{
				// no this node is the last pLastBrother
				pLastBrother->mNextSiblingNode = this;
				mParentNode->mLastSonNode = this;
			}
		}
		// now this  node is current node
		CPerformance::Inst()->SetCurrNode(this);
		// start time
		mClockTime.Start();
	}
	void CPerforNode::LeaveCode()
	{
		// end time
		mClockTime.End();
		// get the code's time consuming
		uint32 nTime = (uint32)mClockTime.GetInterval();
		// update the mMinTime
		if (nTime < mMinTime)
		{
			mMinTime = nTime;
		}
		// update the mMaxTime
		if (nTime > mMaxTime)
		{
			mMaxTime = nTime;
		}
		// add to total time
		mTotalTime += nTime;
		// code out this node scope and enter parent node scope
		CPerformance::Inst()->SetCurrNode(mParentNode);
	}

	void CPerforNode::Print(int nLevel)
	{
		//LOG_DEBUG("default", "Name: %s, CallNum: %d, MaxTime: %d, MinTime: %d, AveTime: %d, TotalTime, %d\n", 
		//	mName, mCallNum, mMaxTime, mMinTime, mCallNum > 0 ? mTotalTime/mCallNum : 0, mTotalTime);
		// printf the node of statistics infomation
		printf("Level: %d, Name: %s, CallNum: %d, MaxTime: %d, MinTime: %d, AveTime: %d, TotalTime, %d\n",
			nLevel, mName, mCallNum, mMaxTime, mMinTime, mCallNum > 0 ? mTotalTime / mCallNum : 0, mTotalTime);
		CPerforNode* pChild = mFirstSonNode;
		// Traverse all child and print the result
		++nLevel;
		for (; NULL != pChild; pChild = pChild->mNextSiblingNode)
		{
			pChild->Print(nLevel);
		}
	}

	/// print all timer statistics result
	void CPerformance::PrintResult()
	{
		mRootNode.Print(0);
	}
}
