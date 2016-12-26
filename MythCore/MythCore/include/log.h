#ifndef __LOG_H__
#define __LOG_H__
#include "commontype.h"
namespace Myth
{
	/** 
	 * log class
	 */
	class CLogDisplayer;
	class CLog
	{
	public:
		CLog()
		{
			Init();
		}
		~CLog()
		{
			Finial();
		}

		/// Initialize the log
		int		Init();

		/// Finalization, when release the log
		void	Finial();

	public:
		/// Get log's name
		const char*		GetLogName(){return mLogName;}

		/// Set log's name
		void			SetLogName(const char* pName){strncpy(mLogName, pName, sizeof(mLogName) - 1);}

		/// Add a displayer
		bool			AddDisplayer(CLogDisplayer* pDisplayer);
		/// Get a displayer
		CLogDisplayer*	GetDisplayer(uint32 uIndex);
		/// Get the size of displayer 
		uint8				GetDisplayerSize(){return mDisplayerSize;}
		/// Set the size of displayer
		void			SetDisplayerSize(uint8 nSize){mDisplayerSize = nSize;}
	
		void			DisplayLog(char* pLogMessage);
	private:
		/// Name of log
		char			mLogName[STRING_LENGTH_16];
		/// Displayers of log,these decide where the log display
		CLogDisplayer*	mDisplayer[MAX_NUM_LOG_DISPLAYER];
		/// Number of displayer
		uint8			mDisplayerSize;
	};
}

#endif
