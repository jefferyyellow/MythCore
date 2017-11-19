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
		CVariable* pVariable = getVariable(pAttribute->Name());
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
				_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(pRootElement->IntAttribute(\"%s\"));\n", mBuffer, pAttribute->Name(), pAttribute->Name());
			}
			else
			{
				appendIndent(nIndentNum);
				_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = pRootElement->IntAttribute(\"%s\");\n", mBuffer, pAttribute->Name(), pAttribute->Name());
			}
		}
		else
		{
			if (pVariable->getCount() > 1)
			{
				appendIndent(nIndentNum);
				_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s.push_back(p%sEle->IntAttribute(\"%s\"));\n", mBuffer, pAttribute->Name(), pElement->Name(), pAttribute->Name());
			}
			else
			{
				appendIndent(nIndentNum);
				_snprintf_s(mBuffer, sizeof(mBuffer)-1, "%sm%s = p%sEle->IntAttribute(\"%s\");\n", mBuffer, pAttribute->Name(), pElement->Name(), pAttribute->Name());
			}
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

// д����xml�ڵ�
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

// дԴ�ļ�
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

// ����б�����û�и����ֵı���
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

// �������ֵõ���Ӧ����
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

// �Ѿ���ʽ���ڵ������
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