#include "stdafx.h"
#include "TaskTemplate.h"
#include "tinyxml2.h"
#include "commondefine.h"
using namespace tinyxml2;
void CTaskTemplate::loadTaskTemplate(const char* pTemplateFile)
{
	if (NULL == pTemplateFile)
	{
		return;
	}
	tinyxml2::XMLDocument tDocument;
	if (XML_SUCCESS != tDocument.LoadFile(pTemplateFile))
	{
		// �����޷�����xml�ļ�
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// ������Root�ڵ㶼û��
		return;
	}

	XMLElement* pTextNodeElem = pRoot->FirstChildElement("TextNode");
	for (; NULL != pTextNodeElem; pTextNodeElem = pTextNodeElem->NextSiblingElement("TextNode"))
	{
		 CTaskTempNode tNode;
		 tNode.mName = s2ws(pTextNodeElem->Attribute("Name"));
		 tNode.mType = s2ws(pTextNodeElem->Attribute("Type"));
		 tNode.mEditType = s2ws(pTextNodeElem->Attribute("EditType"));
		 mNodeList.push_back(tNode);
	}
}