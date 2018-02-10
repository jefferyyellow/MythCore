#ifndef __STRUTILITY_H__
#define __STRUTILITY_H__
#include <string>
#include "servercommon.h"
typedef std::vector<std::string> StrTokens;
StrTokens strSplit(const std::string &strSource, const std::string &strSep);

#endif // __STRUTILITY_H__