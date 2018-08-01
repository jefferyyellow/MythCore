#include "class.h"
void CClass::formatSourceFile(const char* pName, const char* pPath)
{
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%svoid C%s::LoadXXXFromXml(const char* pXmlFile)\n", mBuffer, mName);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s{\n", mBuffer);


	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\ttinyxml2::XMLDocument tDocument;\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\tif (XML_SUCCESS != tDocument.LoadFile(pXmlFile))\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\t{\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\t\treturn false;\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\t}\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\n", mBuffer);

	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\tXMLElement* pRootElement = tDocument.RootElement();\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\tif (NULL == pRootElement)\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\t{\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\t\treturn false;\n", mBuffer);
	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\t}\n", mBuffer);

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
	formatAttribute(pRootElement, 0, true);

	XMLElement* pChildElement = pRootElement->FirstChildElement();
	for (; NULL != pChildElement; pChildElement = pChildElement->NextSiblingElement())
	{
		formatParseXmlElement(pChildElement, pRootElement, this, 0, true);
	}

	_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s}\n", mBuffer);
}

void CClass::formatAttribute(XMLElement* pElement, int nIndentNum, bool bRoot)
{
	++ nIndentNum;
	const XMLAttribute* pAttribute = pElement->FirstAttribute();
	for (; NULL != pAttribute; pAttribute = pAttribute->Next())
	{
		const char* pVariableName = pAttribute->Name();
		if (0 == strncmp("value", pAttribute->Name(), CLASS_NAME_LENGTH))
		{
			pVariableName = pElement->Name();
		}

		CVariable* pVariable = getVariable(pVariableName);

		if (NULL == pVariable)
		{
			continue;
		}
		if (pVariable->getAlreadyWrite())
		{
			continue;
		}

		pVariable->setAlreadyWrite(true);

		if (bRoot)
		{
			if (pVariable->getCount() > 1)
			{
				appendIndent(nIndentNum);
				fromatVariableArray(pVariable, pElement, pAttribute, bRoot);
			}
			else
			{
				appendIndent(nIndentNum);
				formatVariable(pVariable, pElement, pAttribute, bRoot);
			}
		}
		else
		{
			if (pVariable->getCount() > 1)
			{
				appendIndent(nIndentNum);
				fromatVariableArray(pVariable, pElement, pAttribute, bRoot);
			}
			else
			{
				appendIndent(nIndentNum);
				formatVariable(pVariable, pElement, pAttribute, bRoot);
			}
		}
	}

}

void CClass::formatVariable(CVariable* pVariable, XMLElement* pElement, const XMLAttribute* pAttribute, bool bRoot)
{
	if (NULL == pVariable || NULL == pAttribute)
	{
		return;
	}

	if (0 == strcmp(pVariable->getType(), "int"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = pRootElement->IntAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = p%sEle->IntAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "int64"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = pRootElement->Int64Attribute(\"%s\");\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = p%sEle->Int64Attribute(\"%s\");\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "bool"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = pRootElement->BoolAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = p%sEle->BoolAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "double"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = pRootElement->DoubleAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = p%sEle->DoubleAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "float"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = pRootElement->FloatAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = p%sEle->FloatAttribute(\"%s\");\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	// 其实这个用不上，最多就是解析后发给客户端，类型不再用char数组，直接用string
	else if (0 == strcmp(pVariable->getType(), "string"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = string(pRootElement->FindAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = string(p%sEle->FindAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
}

void CClass::fromatVariableArray(CVariable* pVariable, XMLElement* pElement, const XMLAttribute* pAttribute, bool bRoot)
{
	if (0 == strcmp(pVariable->getType(), "int"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->IntAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->IntAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "int64"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->Int64Attribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->Int64Attribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "bool"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->BoolAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->BoolAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "double"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->DoubleAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->DoubleAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	else if (0 == strcmp(pVariable->getType(), "float"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->FloatAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->FloatAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
	// 其实这个用不上，最多就是解析后发给客户端，类型不再用char数组，直接用string
	else if (0 == strcmp(pVariable->getType(), "string"))
	{
		if (bRoot)
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->FindAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pAttribute->Name());
		}
		else
		{
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->FindAttribute(\"%s\"));\n", mBuffer, pVariable->getName(), pElement->Name(), pAttribute->Name());
		}
	}
}


void CClass::formatClass(XMLElement* pElement, XMLElement* pParentElement, bool bClass, int nIndentNum)
{
	if (bClass)
	{
		_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%svoid C%s::LoadFromXml(XMLElement* p%sEle)\n", mBuffer, pElement->Name(), pElement->Name());
		_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s{\n", mBuffer);
		nIndentNum = 0;
	}

	formatAttribute(pElement, nIndentNum, false);
	XMLElement* pChildElement = pElement->FirstChildElement();
	for (; NULL != pChildElement; pChildElement = pChildElement->NextSiblingElement())
	{
		formatParseXmlElement(pChildElement, pElement, this, nIndentNum, false);
	}

	if (bClass)
	{
		_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s}\n", mBuffer);
	}
}

// 写分析xml节点
void CClass::formatParseXmlElement(XMLElement* pElement, XMLElement* pParentElement, CClass* pParent, int nIndentNum, bool bRoot)
{
	if (NULL == pElement)
	{
		return;
	}

	const char* pTmpName = pElement->Name();
	const XMLAttribute* pRootAttribute = pElement->FirstAttribute();

	++ nIndentNum;
	CClass* pCurClass = getSubClass(pElement->Name());
	if (NULL == pCurClass)
	{
		pCurClass = pParent;

		CXmlElement* pClassXmlElement = pCurClass->getXmlElement(pElement->Name());
		if (NULL == pClassXmlElement)
		{
			return;
		}
		if (pClassXmlElement->getAlreadyWrite())
		{
			return;
		}
		pClassXmlElement->setAlreadyWrite(true);

		if (bRoot)
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\n", mBuffer);
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sXMLElement* p%sEle = pRootElement->FirstChildElement(\"%s\");\n", mBuffer, pElement->Name(), pElement->Name());
		}
		else
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\n", mBuffer);
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sXMLElement* p%sEle = p%sEle->FirstChildElement(\"%s\");\n", mBuffer, pElement->Name(), pParentElement->Name(), pElement->Name());
		}

		if (pClassXmlElement->getCount() > 1)
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sfor (; NULL !=  p%sEle;  p%sEle =  p%sEle->NextSiblingElement(\"%s\"))\n",
				mBuffer, pElement->Name(), pElement->Name(), pElement->Name(), pElement->Name());
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s{\n", mBuffer, pElement->Name());

		}
		else
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sif (NULL != p%sEle)\n", mBuffer, pElement->Name());
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s{\n", mBuffer, pElement->Name());
		}

		pCurClass->formatClass(pElement, pParentElement, false, nIndentNum);
		appendIndent(nIndentNum);
		_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s}\n", mBuffer, pElement->Name());

	}
	else
	{
		CXmlElement* pClassXmlElement = pCurClass->getXmlElement(pElement->Name());
		if (NULL == pClassXmlElement)
		{
			return;
		}
		if (pClassXmlElement->getAlreadyWrite())
		{
			return;
		}
		pClassXmlElement->setAlreadyWrite(true);

		if (bRoot)
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\n", mBuffer);
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sXMLElement* p%sEle = pRootElement->FirstChildElement(\"%s\");\n", mBuffer, pElement->Name(), pElement->Name());
		}
		else
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s\n", mBuffer);
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sXMLElement* p%sEle = p%sEle->FirstChildElement(\"%s\");\n", mBuffer, pElement->Name(), pParentElement->Name(), pElement->Name());
		}
		if (pClassXmlElement->getCount() > 1)
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sfor (; NULL !=  p%sEle;  p%sEle =  p%sEle->NextSiblingElement(\"%s\"))\n",
				mBuffer, pElement->Name(), pElement->Name(), pElement->Name(), pElement->Name());
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s{\n", mBuffer, pElement->Name());

			++ nIndentNum;

			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sC%s t%s;\n", mBuffer, pElement->Name(), pElement->Name());
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%st%s.LoadFromXml(p%sEle);\n", mBuffer, pElement->Name(), pElement->Name());
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(t%s);\n", mBuffer, pElement->Name(), pElement->Name());
		}
		else
		{
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sif (NULL != p%sEle)\n", mBuffer, pElement->Name());
			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s{\n", mBuffer, pElement->Name());

			++nIndentNum;

			appendIndent(nIndentNum);
			_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.LoadFromXml(p%sEle)\n", mBuffer, pElement->Name(), pElement->Name());
		}

		pCurClass->formatClass(pElement, pParentElement, true, nIndentNum);

		-- nIndentNum;
		appendIndent(nIndentNum);
		_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%s}\n", mBuffer, pElement->Name());

	}

}

// 写源文件
void CClass::writeSourceFile(FILE* pFile)
{
	if (NULL == pFile)
	{
		return;
	}

	for (int i = 0; i < mvecSubClassList.size(); ++ i)
	{
		mvecSubClassList[i]->writeSourceFile(pFile);
	}

	fwrite(mBuffer, strlen(mBuffer), 1, pFile);
}

// 检查列表里有没有该名字的变量
CVariable* CClass::getVariable(const char* pName)
{
	if (NULL == pName)
	{
		return NULL;
	}

	for (int i = 0; i < mVariableList.size(); ++i)
	{
		if (NULL == mVariableList[i])
		{
			continue;
		}
		if (0 == strncmp(mVariableList[i]->getName(), pName, CLASS_NAME_LENGTH))
		{
			return mVariableList[i];
		}
	}

	return NULL;
}

// 根据名字得到对应的类
CClass* CClass::getSubClass(const char* pName)
{
	if (NULL == pName)
	{
		return NULL;
	}

	for (int i = 0; i < mvecSubClassList.size(); ++i)
	{
		if (0 == strncmp(pName, mvecSubClassList[i]->getName(), CLASS_NAME_LENGTH))
		{
			return mvecSubClassList[i];
		}
	}
	return NULL;
}

// 已经格式化节点的名字
CXmlElement* CClass::getXmlElement(const char* pName)
{
	for (int i = 0; i < mXmlElement.size(); ++i)
	{
		if (mXmlElement[i].getName() == string(pName))
		{
			return &mXmlElement[i];
		}
	}
	return NULL;
}

void CClass::appendIndent(int nNum)
{
	for (int i = 0; i < nNum; ++ i)
	{
		strcat(mBuffer, "\t");
	}
}