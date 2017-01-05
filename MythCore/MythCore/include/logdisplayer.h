#ifndef __LOGDISPLAYER_H__
#define __LOGDISPLAYER_H__
#include "commontype.h"
#include <stdio.h>
/** class of log displayer,a displayer dicides where the log display
 */

namespace Myth
{
	class CLogDisplayer
	{
	public:
		CLogDisplayer(){}
		virtual ~CLogDisplayer(){}

	public:
		virtual void DisplayLog(char* pLogMessage) = 0;
	};

	class CStdDisplayer : public CLogDisplayer
	{
	public:
		CStdDisplayer(){}
		virtual ~CStdDisplayer(){}

	public:
		virtual void DisplayLog(char* pLogMessage);
	};

	/** roll file: write log into file,when the file size big than mMaxFileSize,then create a new file which use to log,
	  * if mFileName.log.mMaxBackNum already exist,remove it.the old file change name mFileName.log.1,if mFileName.log.1 
	  * already exist,mFileName.log.1 will mFileName.log.2, ...,and so on.
	  */
	class CRollFileDisplayer : public CLogDisplayer
	{
	public:
		CRollFileDisplayer(char* pFileName, uint32 uMaxFileSize, uint uMaxBackNum);
		virtual ~CRollFileDisplayer();

	public:
		/// display log message
		virtual void DisplayLog(char* pLogMessage);

	private:
		/// role over when a log file is full
		void RollOver();

	private:
		/// base name of roll file
		char 	mFileName[STRING_LENGTH_64];
		/// current file fd
		int		mFd;
		/// max file size of roll file
		uint32	mMaxFileSize;
		/// max back file num
		uint32	mMaxBackNum;
	};

	/** debugger output displayer: display the log message in the debugger(for example: microsoft visual studio) 
	  * out put window in the windows
	  */
	class CDebuggerOutputDisplayer : public CLogDisplayer
	{
	public:
		CDebuggerOutputDisplayer(){}
		virtual ~CDebuggerOutputDisplayer(){}

	public:
		/// display log message
		virtual void DisplayLog(char* pLogMessage);
	};

}

#endif
