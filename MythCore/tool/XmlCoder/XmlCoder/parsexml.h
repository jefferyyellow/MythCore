#ifndef __PARSEXML_H__
#define __PARSEXML_H__
#include "commontype.h"
#include "tinyxml2.h"
using namespace tinyxml2;
class CParseXml
{
public:
	CParseXml(){}
	~CParseXml(){}

public:
	void parseXml(const char* pPath);
	void parseXmlElement(XMLElement* pElement);

};
#endif