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

	const char* pClassAttribute = pElement->Attribute("class");
	if (NULL != pClassAttribute)
	{
		CClass* pClass = getClass(pElement->Name());
		if (NULL != pClass)
		{
			pClass->setCount(pClass->getCount() + 1);
			mCurClass = pClass;
		}
		else
		{
			CClass* pNewClass = new CClass;
			pNewClass->setName(pElement->Name());
			pNewClass->setNeedGeneral((bool)(atoi(pClassAttribute)));
			pNewClass->setCount(0);
			mvecClassList.push_back(pNewClass);
			mCurClass = pNewClass;
		}
	}

	const XMLAttribute* pAttribute = pElement->FirstAttribute();
	for (; NULL != pAttribute; pAttribute = pAttribute->Next())
	{
		// 过滤class属性节点
		if (0 == strncmp("class", pAttribute->Name(), CLASS_NAME_LENGTH))
		{
			continue;
		}
		
		CVariable* pVariable = getVariable(mCurClass, pAttribute->Name());
		if (NULL != pVariable)
		{
			pVariable->setCount(pVariable->getCount() + 1);
		}
		else
		{
			CVariable* pNewVariable = new CVariable;
			pNewVariable->setName(pAttribute->Name());
			pNewVariable->setType(pAttribute->Value());
			pNewVariable->setCount(0);
			mCurClass->getVariableList().push_back(pNewVariable);
		}
	}

	XMLElement* pChildElement = pElement->FirstChildElement();
	for (; NULL != pChildElement; pChildElement = pChildElement->NextSiblingElement())
	{
		parseXmlElement(pChildElement);
	}
}

// 检查列表里有没有该名字的变量
CVariable* CParseXml::getVariable(CClass* pClass, const char* pName)
{
	if (NULL == pClass || NULL == pName)
	{
		return NULL;
	}

	for (int i = 0; i < pClass->getVariableList().size(); ++ i)
	{
		CVariable* pVariable = pClass->getVariableList()[i];
		if (NULL == pVariable)
		{
			continue;
		}
		if (0 == strncmp(pVariable->getName(), pName, CLASS_NAME_LENGTH))
		{
			return pVariable;
		}
	}

	return NULL;
}

// 根据名字得到对应的类
CClass* CParseXml::getClass(const char* pName)
{
	if (NULL == pName)
	{
		return NULL;
	}

	for (int i = 0; i < mvecClassList.size(); ++i)
	{
		if (0 == strncmp(pName, mvecClassList[i]->getName(), CLASS_NAME_LENGTH))
		{
			return mvecClassList[i];
		}
	}
	return NULL;
}

// 写头文件
void CParseXml::writeHeadFile(const char* pName)
{
	if (NULL == pName)
	{
		return;
	}

	FILE* pFile = fopen(pName, "wt");
	if (NULL == pFile)
	{
		return;
	}

	for (int i = 0; i < mvecClassList.size(); ++ i)
	{
		writeClassFile(mvecClassList[i], pFile);
	}
}

// 写类的声明
void CParseXml::writeClassFile(CClass* pClass, FILE* pFile)
{
	if (NULL == pClass || NULL == pFile)
	{
		return;
	}

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%sclass C%s\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s{\n", acBuffer);
	CClass::VARIABLE_VECTOR& rVariableList = pClass->getVariableList();
	for (int i = 0; i < rVariableList.size(); ++ i)
	{
		if (NULL == rVariableList[i])
		{
			continue;
		}
		_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s\t%s\t\tm%s;\n", acBuffer, rVariableList[i]->getType(), rVariableList[i]->getName());
	}
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s};\n", acBuffer);

	fwrite(acBuffer, strlen(acBuffer), 1, pFile);
}

// 写源文件
void CParseXml::writeSourceFile(const char* pName)
{
	if (NULL == pName)
	{
		return;
	}


}