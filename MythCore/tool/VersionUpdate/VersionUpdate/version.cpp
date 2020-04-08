#include "version.h"
#include "tinyxml/tinyxml2.h"
using namespace tinyxml2;
void CVersion::loadVersionXml(const char* pFilePath)
{
	if (NULL == pFilePath)
	{
		return;
	}

	tinyxml2::XMLDocument tDoc;
	if (XML_SUCCESS != tDoc.LoadFile(pFilePath))
	{
		return;
	}
	XMLElement* pRootElem = tDoc.RootElement();
	if (NULL == pRootElem)
	{
		return;
	}

	XMLElement* pVersionElem = pRootElem->FirstChildElement("Version");
	if (NULL != pVersionElem)
	{
		mStartVersion = pVersionElem->IntAttribute("start");
		mCurVersion = pVersionElem->IntAttribute("curr");
	}
}
