#ifndef __DIRTYWORD_H__
#define __DIRTYWORD_H__
#include <set>
#include <string>
#include "singleton.h"
using namespace Myth;
class CDirtyWord  : public CSingleton <CDirtyWord>
{
	friend class CSingleton <CDirtyWord> ;
private:
	CDirtyWord()
	{}
	~CDirtyWord()
	{
		clear();
	}

public:
	void	clear();
	bool	loadDirtyWordConfig(const char* pFilePath);
	bool	checkDirtyWord(const char* pWord);
	void	addDirtyWord(const char* pWord);

public:
	std::set<std::string> mDirtyWordSet;
};
#endif