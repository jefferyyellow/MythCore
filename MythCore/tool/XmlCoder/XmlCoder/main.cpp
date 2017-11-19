#include "parsexml.h"
int main()
{
	CParseXml tParseXml;
	tParseXml.parseXml("ActivityWorldFieldConfig.xml");
	tParseXml.writeHeadFile("ActivityWorldFieldConfig.h");
	tParseXml.writeSourceFile("ActivityWorldFieldConfig.cpp", "ActivityWorldFieldConfig.xml");
}