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
		// 出错，无法加载xml文件
		return;
	}

	XMLElement* pRoot = tDocument.RootElement();
	if (NULL == pRoot)
	{
		// 出错，连Root节点都没有
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