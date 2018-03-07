#include "strutility.h"
StrTokens strSplit(const std::string &strSource, const std::string &strSep)
{
	StrTokens tTokens;
	std::string strTemp;
	for (std::string::const_iterator i = strSource.begin(); i != strSource.end(); i++)
	{
		if (strSep.find(*i) != std::string::npos)
		{
			if (strTemp.length() > 0)
			{
				tTokens.push_back(strTemp);
				strTemp = "";
			}
		}
		else
		{
			strTemp += *i;
		}
	}
	if (strTemp.length())
	{
		tTokens.push_back(strTemp);
	}
	return tTokens;
}