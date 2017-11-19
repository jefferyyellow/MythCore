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
		if (NULL != pParent)
		{
			pCurClass = pParent->getSubClass(pElement->Name());
		}
		if (NULL != pCurClass)
		{
			CXmlElement* pClassElement = pCurClass->getXmlElement(pElement->Name());
			if (NULL != pClassElement)
			{
				pClassElement->setCount(pClassElement->getCount() + 1);
			}
			pCurClass->setCount(pCurClass->getCount() + 1);
			return;
		}
		else
		{
			CClass* pNewClass = new CClass;
			pNewClass->setName(pElement->Name());
			pNewClass->setNeedGeneral((bool)(atoi(pClassAttribute)));
			pNewClass->setCount(1);
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

	if (NULL != pCurClass)
	{
		CXmlElement* pClassElement = pCurClass->getXmlElement(pElement->Name());
		if (NULL != pClassElement)
		{
			pClassElement->setCount(pClassElement->getCount() + 1);
		}
		else
		{
			CXmlElement tElement;
			tElement.setName(string(pElement->Name()));
			tElement.setCount(1);
			tElement.setAlreadyWrite(false);
			if (NULL == pParent)
			{
				tElement.setRoot(true);
			}
			pCurClass->pushBack(tElement);

		}

		const XMLAttribute* pAttribute = pElement->FirstAttribute();
		for (; NULL != pAttribute; pAttribute = pAttribute->Next())
		{
			// 过滤class属性节点
			if (0 == strncmp("class", pAttribute->Name(), CLASS_NAME_LENGTH))
			{
				continue;
			}

			CVariable* pVariable = pCurClass->getVariable(pAttribute->Name());
			if (NULL != pVariable)
			{
				pVariable->setCount(pVariable->getCount() + 1);
			}
			else
			{
				CVariable* pNewVariable = new CVariable;
				pNewVariable->setName(pAttribute->Name());
				pNewVariable->setType(pAttribute->Value());
				pNewVariable->setCount(1);
				pCurClass->getVariableList().push_back(pNewVariable);
			}
		}
	}

	XMLElement* pChildElement = pElement->FirstChildElement();
	for (; NULL != pChildElement; pChildElement = pChildElement->NextSiblingElement())
	{
		parseXmlElement(pChildElement, pCurClass);
	}
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
	fclose(pFile);
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

		if (rClassList[i]->getCount() > 1)
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

		if (rVariableList[i]->getCount() > 1)
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
void CParseXml::writeSourceFile(const char* pName, const char* pPath)
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

	mRootClass->formatSourceFile(pName, pPath);
	mRootClass->writeSourceFile(pFile);
}

