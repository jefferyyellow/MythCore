#include "parsexml.h"


void CParseXml::parseXml(const char* pPath)
{
	if (NULL == pPath)
	{
		return;
	}

	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pPath))
	{
		return;
	}

	XMLElement* pRootElement = tDocument.RootElement();
	if (NULL == pRootElement)
	{
		return;
	}
	parseXmlElement(pRootElement);

}

void CParseXml::parseXmlElement(XMLElement* pElement)
{
	if (NULL == pElement)
	{
		return;
	}

	const XMLAttribute* pAttribute = pElement->FirstAttribute();
	for (; NULL != pAttribute; pAttribute = pAttribute->Next())
	{

	}

	XMLElement* pChildElement = pElement->FirstChildElement();
	for (; NULL != pChildElement; pChildElement = pChildElement->NextSiblingElement())
	{
		parseXmlElement(pChildElement);
	}
}