#include "parsexml.h"
int main()
{
	CParseXml tParseXml;
	tParseXml.parseXml("FashionMall.xml");
	tParseXml.writeHeadFile("FashionMall.h");
}