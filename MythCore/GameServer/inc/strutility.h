#ifndef __STRUTILITY_H__
#define __STRUTILITY_H__
#include <vector>
#include <string>

typedef std::vector<std::string> StrTokens;
StrTokens strSplit(const std::string &strSource, const std::string &strSep);

#endif // __STRUTILITY_H__