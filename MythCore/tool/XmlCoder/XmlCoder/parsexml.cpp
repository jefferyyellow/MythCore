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
	parseXmlElement(pRootElement, NULL);

}

void CParseXml::parseXmlElement(XMLElement* pElement, CClass* pParent)
{
	if (NULL == pElement)
	{
		return;
	}

	CClass* pCurClass = pParent;
	const char* pClassAttribute = pElement->Attribute("class");
	if (NULL != pClassAttribute)
	{
		CClass* pClass = getSubClass(pParent, pElement->Name());
		if (NULL != pClass)
		{
			pClass->setCount(pClass->getCount() + 1);
			return;
		}
		else
		{
			CClass* pNewClass = new CClass;
			pNewClass->setName(pElement->Name());
			pNewClass->setNeedGeneral((bool)(atoi(pClassAttribute)));
			pNewClass->setCount(0);
			if (NULL == mRootClass)
			{
				mRootClass = pNewClass;
			}
			else
			{
				pParent->getSubClassList().push_back(pNewClass);
			}
			pCurClass = pNewClass;
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
		
		CVariable* pVariable = getVariable(pCurClass, pAttribute->Name());
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
			pCurClass->getVariableList().push_back(pNewVariable);
		}
	}

	XMLElement* pChildElement = pElement->FirstChildElement();
	for (; NULL != pChildElement; pChildElement = pChildElement->NextSiblingElement())
	{
		parseXmlElement(pChildElement, pCurClass);
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
CClass* CParseXml::getSubClass(CClass* pClass, const char* pName)
{
	if (NULL == pClass || NULL == pName)
	{
		return NULL;
	}

	for (int i = 0; i < pClass->getSubClassList().size(); ++i)
	{
		if (0 == strncmp(pName, pClass->getSubClassList()[i]->getName(), CLASS_NAME_LENGTH))
		{
			return pClass->getSubClassList()[i];
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

	writeClassFile(mRootClass, pFile);
}

// 写类的声明
void CParseXml::writeClassFile(CClass* pClass, FILE* pFile)
{
	if (NULL == pClass || NULL == pFile)
	{
		return;
	}

	CClass::CLASS_VECTOR& rClassList = pClass->getSubClassList();
	for (int i = 0; i < rClassList.size(); ++ i)
	{
		writeClassFile(rClassList[i], pFile);
	}

	char acBuffer[MAX_WRITE_BUFFER] = { 0 };
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%sclass C%s\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s{\n", acBuffer);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%spublic:\n", acBuffer);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tC%s()\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t\tinit();\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t~C%s(){}\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\n", acBuffer, pClass->getName());

	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%spublic:\n", acBuffer);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tvoid init()\n", acBuffer);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t{\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t}\n", acBuffer, pClass->getName());

	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t// autocode don't edit!!!\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t// autocode\n", acBuffer, pClass->getName());

	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\n", acBuffer, pClass->getName());
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%sprivate:\n", acBuffer, pClass->getName());
	
	for (int i = 0; i < rClassList.size(); ++i)
	{
		if (NULL == rClassList[i])
		{
			continue;
		}

		if (rClassList[i]->getCount() > 0)
		{
			_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s\tvector<C%s>\t\tm%s;\n", acBuffer, rClassList[i]->getName(), rClassList[i]->getName());
		}
		else
		{
			_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s\tC%s\t\tm%s;\n", acBuffer, rClassList[i]->getName(), rClassList[i]->getName());
		}
	}

	CClass::VARIABLE_VECTOR& rVariableList = pClass->getVariableList();
	for (int i = 0; i < rVariableList.size(); ++ i)
	{
		if (NULL == rVariableList[i])
		{
			continue;
		}

		if (rVariableList[i]->getCount() > 0)
		{
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\tvector<%s>\t\tm%s;\n", acBuffer, rVariableList[i]->getType(), rVariableList[i]->getName());
		}
		else
		{
			_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\t%s\t\tm%s;\n", acBuffer, rVariableList[i]->getType(), rVariableList[i]->getName());
		}
	}
	_snprintf_s(acBuffer, sizeof(acBuffer) - 1, "%s};\n", acBuffer);
	_snprintf_s(acBuffer, sizeof(acBuffer)-1, "%s\n", acBuffer, pClass->getName());

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