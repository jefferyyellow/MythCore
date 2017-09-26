#include "TemplateCode.h"
#include <fstream>
#include <sstream> 


bool CParseCodeFile::BeginParseFile(string & rStrPath, CTemplateData & rTemplateData)
{
	// 打开模板头文件
	ifstream InFile;
	InFile.open(rStrPath.c_str());
	if (!InFile.is_open())
	{
		return false;
	}

	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = { 0 };

	string strLine = "";
	while (InFile.getline(acBuffer, emLine_Max_Char))
	{
		strLine = acBuffer;
		// 只分析GlobalClass，且不是声明和注释
		if ((nIndex = strLine.find("class")) != string::npos 
			&& strLine.find(";") == string::npos
			&& strLine.find("//") == string::npos)
		{
			CTemplateInfo tTemplateInfo;
			if (ParseClass(tTemplateInfo, strLine, InFile))
			{
				rTemplateData.push_back(tTemplateInfo);
			}
		}
	}

	InFile.close();
	return true;
}
// 只是简单的把各个字段分析到mFiledInfoList，解析部分放在BuildCode中
bool CParseCodeFile::ParseClass(CTemplateInfo & rTemplateInfo, string & rCurStrLine, ifstream& InFile)
{

	const int ncClassLen = sizeof("class");
	const int ncPublicLen = sizeof("public");

	string::size_type nIndex = 0;
	char acBuffer[emLine_Max_Char] = { 0 };
	if ((nIndex = rCurStrLine.find("class")) == string::npos)
	{
		return false;
	}
	//获得类的名称
	string strCurName = GetFirstWord(rCurStrLine.substr(nIndex + ncClassLen));
	if (strCurName.empty())
	{
		return false;
	}
	rTemplateInfo.mClassName = strCurName;
	rTemplateInfo.mKeyNameWord = GetClassNameKeyWord(rTemplateInfo.mClassName);
	//获得父类名称
	if ((nIndex = rCurStrLine.find("public")) != string::npos)
	{
		string strParentName = GetFirstWord(rCurStrLine.substr(nIndex + ncPublicLen));
		if (!strCurName.empty())
		{
			rTemplateInfo.mParentName = strParentName;
			rTemplateInfo.mParentKeyNameWord = GetClassNameKeyWord(rTemplateInfo.mParentName);
		}
	}
	static int  nInClass = 0;
	static bool bInTemplateEdit = true;
	static bool bInEnum = false;
	++nInClass;
	while (InFile.getline(acBuffer, emLine_Max_Char))
	{
		string strLine = acBuffer;
		//过滤注释和空行，有等号的，有//且第一个单词开头为// ， 如果没有; 语法错误

		string strFristWord = GetFirstWord(strLine);
		if (strFristWord.find("#ifndef") != string::npos && strLine.find("TEMPEDIT") != string::npos)
		{//进入了 #ifndef TEMPEDIT ，里面的变量，有另一套生成方式
			bInTemplateEdit = false;
			continue;
		}
		else if (strFristWord.find("#endif") != string::npos)
		{//退出
			bInTemplateEdit = true;
			continue;
		}
		else if (strFristWord.find("enum") != string::npos)
		{//枚举，暂时忽略
			bInEnum = true;
			continue;
		}
		

		if (strFristWord.find("};") != string::npos)
		{//类的结尾或者枚举的结尾
			if (bInEnum)
			{
				bInEnum = false;
				continue;
			}

			--nInClass;
			if (nInClass <= 0)
			{//已经分析完了
				return true;
			}
			else if (rTemplateInfo.mFiledInfoList.empty())
			{//如果嵌套类，没有啥字段的，舍去
				return false;
			}
			else
			{
				return true;
			}
		}

		//进入嵌套模板处理
		if ((nIndex = strFristWord.find("struct")) != string::npos ||
			(nIndex = strFristWord.find("class")) != string::npos)
		{
			++nInClass;
			if (nInClass > 1)
			{
				string nestTpName = GetFirstWord(strLine);
				CTemplateInfo tTemplateInfo;
				strLine = acBuffer;
				//递归嵌套处理函数，兼容之前格式，若读取类中无字段，不做操作
				if (ParseClass(tTemplateInfo, strLine, InFile))
				{
					std::pair<string, CTemplateInfo> pa(nestTpName, tTemplateInfo);
					rTemplateInfo.mNestClassMap.insert(pa);
				}
			}
			continue;
		}
		//除了注释，和嵌套类，在类中，只有声明或者声明定义的函数，忽略赋值语句，函数
		if (strFristWord.find("//") != string::npos ||
			strFristWord.empty() ||
			strLine.find(";") == string::npos ||
			strLine.find("=") != string::npos ||
			strLine.find("(") != string::npos)
		{
			continue;
		}


		int nDimensional = 0;
		int ipos = strLine.find("[");
		while (ipos != string::npos)
		{
			nDimensional++;
			ipos = strLine.find("[", ipos + 2);
		}

		int nType = emTplField_None;
		int nSkipLen = 0;
		string strFieldName;
		if ((nIndex = strFristWord.find("int")) != string::npos && strFristWord.size() == strlen("int"))
		{//是int
			nType = emTplField_Int;
			if (nDimensional)
			{
				nType = emTplField_IntArray;
			}
			nSkipLen = sizeof("int");
		}
		else if (strFristWord.find("char") != string::npos && strFristWord.size() == strlen("char"))
		{//是char
			nType = emTplField_String;
			if (nDimensional > 1)
			{//如果两个地方不相等
				nType = emTplField_StringArray;
			}
			nSkipLen = sizeof("char");
		}
		else 
		{//类
			nType = emTplField_Class;
			if (nDimensional)
			{
				nType = emTplField_ClassArray;
			}
			nSkipLen = strFristWord.size();
		}

		if (nType == emTplField_None)
		{
			continue;
		}
		//如果是函数，提取的变量名
		//去掉;及以后的文字，和中间的空格，TAB
		if (nDimensional > 0)
		{
			strFieldName = GetFirstWord(strLine.substr(0, strLine.find("[")));
		}
		else
		{
			strFieldName = GetFirstWord(strLine.substr(0, strLine.find(";")));
		}


		if (strFieldName.empty())
		{
			continue;
		}
		
		CFieldInfo tFieldInfo;
		tFieldInfo.mType = nType;
		tFieldInfo.mFieldName = strFieldName;
		tFieldInfo.mDimensional = nDimensional;
		tFieldInfo.mFieldType = strFristWord;
		if (nType == emTplField_Class || nType == emTplField_ClassArray)
		{
			tFieldInfo.mTypeKeyWord = GetClassNameKeyWord(strFristWord);
		}

		tFieldInfo.mKeyWord = GetFieldKeyWord(tFieldInfo.mFieldName);
		if (bInTemplateEdit)
		{
			rTemplateInfo.mFiledInfoList.push_back(tFieldInfo);
		}
		else
		{
			rTemplateInfo.mNotInTemplateEdit.push_back(tFieldInfo);
		}
	}
	return true;
}

string CParseCodeFile::GetFirstWord(string& strSource)
{
	int nLength = (int)strSource.length();
	if (nLength <= 0)
	{
		return "";
	}

	int nBeginIndex = 0;
	for (int i = 0; i < nLength; i++)
	{
		// 如果不是空格且不是TAB键
		if (strSource[i] != 32 && strSource[i] != 9)
		{
			nBeginIndex = i;
			break;
		}
	}

	int nEndIndex = nLength;
	for (int i = nBeginIndex; i < nLength; i++)
	{
		// 如果是空格或者TAB键
		if (strSource[i] == 32 || strSource[i] == 9)
		{
			nEndIndex = i;
			break;
		}
	}

	// 将字符串取出来
	string strResult = strSource.substr(nBeginIndex, nEndIndex - nBeginIndex);
	// 截取原字符串
	if (nEndIndex >= nLength)
	{
		strSource = "";
	}
	else
	{
		strSource = strSource.substr(nEndIndex + 1);
	}
	return strResult;
}

string	CParseCodeFile::GetFieldKeyWord(string& strFieldName)
{
	int nLen = strFieldName.size();
	if (nLen <= 1)
	{
		return "";
	}
	return strFieldName.substr(1, nLen);
}
string	CParseCodeFile::GetClassNameKeyWord(string& strClassName)
{
	int nLen = strClassName.size();
	if (nLen <= 1)
	{
		return "";
	}
	string::size_type nIndex = 0;
	if ((nIndex = strClassName.find("CTemplate")) != string::npos)
	{
		return strClassName.substr(nIndex + sizeof("CTemplate") - 1);
	}
	else if ((nIndex = strClassName.find("CTpl")) != string::npos)
	{
		return strClassName.substr(nIndex + sizeof("CTpl") - 1);
	}
	else
	{//如果还有其他，去掉第一个字符吧
		return strClassName.substr(1, nLen);
	}
}

//暂时不检测生成目录
bool CBuildCodeFile::BeginBuildCode(const string & rStrPath, string & rClassName, CTemplateData & rTemplateData)
{
	vector<string> strWriteHList;
	vector<string> strWriteCppList;
	vector<string> strWriteHxxList;

	//找到要生成的模板类
	int nIndex = -1;
	CTemplateInfo tProduceTemplateCode;
	for (int i = 0; i < rTemplateData.size(); ++i)
	{
		if (rTemplateData[i].mClassName.compare(rClassName) == 0)
		{
			nIndex = i;
			break;
		}
	}
	if (nIndex < 0)
	{
		return false;
	}
	tProduceTemplateCode = rTemplateData[nIndex];

	string strClassPBName = "PB" + tProduceTemplateCode.mKeyNameWord;

	//生成标题，".h"文件 
	//begin:
	//先生成该类的模板函数，不管存不存在，都生成了
	strWriteHList.push_back("public : ");
	strWriteHList.push_back(ONE_TAB "int Initialize();");
	strWriteHList.push_back(ONE_TAB "void SetFromPB(bool bServer,   " + strClassPBName + "* pbData);");
	strWriteHList.push_back(ONE_TAB "void CreateTlvPB(bool bServer, " + strClassPBName + "* pbData);");

	//嵌套类的模板函数，只考虑第一层，
	for (auto it = tProduceTemplateCode.mNestClassMap.begin(); it != tProduceTemplateCode.mNestClassMap.end(); ++it)
	{
		CTemplateInfo & rtTemplateInfo = it->second;
		string strNestClassPBName = "PB" + rtTemplateInfo.mKeyNameWord;
		strWriteHList.push_back(ONE_TAB"void Set" + rtTemplateInfo.mKeyNameWord + "FromPB(" + rtTemplateInfo.mClassName + "& r" + rtTemplateInfo.mKeyNameWord + ",   " + strNestClassPBName + "* pbData);");
		strWriteHList.push_back(ONE_TAB"void Create" + rtTemplateInfo.mKeyNameWord + "TlvPB(" + rtTemplateInfo.mClassName + "& r" + rtTemplateInfo.mKeyNameWord + ", " + strNestClassPBName + "* pbData);");
	}

	strWriteHList.push_back(ONE_TAB + tProduceTemplateCode.mClassName+ "(){ Initialize(); }");
	strWriteHList.push_back(ONE_TAB"~" + tProduceTemplateCode.mClassName + "(){}");
	//end;

	//写入cpp文件
	//begin:
	//写入Initial
	strWriteCppList.push_back("int " + tProduceTemplateCode.mClassName + "::Initialize() \n{");
	if (tProduceTemplateCode.mParentName.empty() == false)
	{
		strWriteCppList.push_back(ONE_TAB "mTempID = 0;");
	}
	WriteFieldToInitCpp(tProduceTemplateCode.mFiledInfoList, strWriteCppList);
	//如果有非模板声明变量
	if (!tProduceTemplateCode.mNotInTemplateEdit.empty())
	{
		strWriteCppList.push_back("#ifndef TEMPEDIT");
		WriteFieldToInitCpp(tProduceTemplateCode.mNotInTemplateEdit,strWriteCppList);
		strWriteCppList.push_back("#endif");
	}
	//结束
	strWriteCppList.push_back(ONE_TAB "return 0;\n}");

	//SetFromPB
	strWriteCppList.push_back("void " + tProduceTemplateCode.mClassName + "::SetFromPB(bool bServer, " + strClassPBName + "* pbData)  \n{");
	WriteClassFieldSetFromPB(tProduceTemplateCode, "pbData", "\t", "", strWriteCppList);

	//CreatePB
	strWriteCppList.push_back("void " + tProduceTemplateCode.mClassName + "::CreateTlvPB(bool bServer, " + strClassPBName + "* pbData) \n{");
	WriteClassFieldCreateFromPB(tProduceTemplateCode, "pbData", "\t", "", strWriteCppList);
	//end;


	//写入hxx文件
	//begin:
	WriteClassFieldPB(tProduceTemplateCode, strWriteHxxList);
	//end;

	//写入文件
	string strBasePath = rStrPath + rClassName; //生成的文件路径
	WriteFile(strBasePath + ".h", strWriteHList);
	WriteFile(strBasePath + ".cpp", strWriteCppList);
	WriteFile(strBasePath + ".hxx", strWriteHxxList);
	return true;
}

string CBuildCodeFile::GetLowString(const string &strSrc)
{
	string strDest;
	for (int i = 0; i < strSrc.size(); ++i)
	{
		strDest.push_back(tolower(strSrc.at(i)));
	}
	return strDest;
}

void CBuildCodeFile::WriteFieldToInitCpp(vector<CFieldInfo> & rFieldInfoList, vector<string> & rWriteList)
{
	for (auto it = rFieldInfoList.begin(); it != rFieldInfoList.end(); ++it)
	{
		if (it->mType == emTplField_Int)
		{
			rWriteList.push_back(ONE_TAB  + it->mFieldName + " = 0;");
		}
		if (it->mType == emTplField_IntArray || it->mType == emTplField_String
			|| it->mType == emTplField_ClassArray || it->mType == emTplField_StringArray)
		{
			rWriteList.push_back(ONE_TAB "memset("  + it->mFieldName + ", 0, sizeof(" + it->mFieldName + ")); ");
		}
		if (it->mType == emTplField_Class)
		{
			rWriteList.push_back(ONE_TAB "memset(&" + it->mFieldName + ",0,sizeof(" + it->mFieldName + "));");
		}
	}
}

void CBuildCodeFile::WriteClassFieldSetFromPB(CTemplateInfo & rTemplateInfo, const string & pbData, const string & strPrefix, const string & strPreData, vector<string> & rWriteList)
{
	bool bParenIsTemplate = rTemplateInfo.mParentName.compare("CTemplate") == 0 ? true : false;
	bool bHasParent = !rTemplateInfo.mParentName.empty();
	string strClassPBName = "PB" + rTemplateInfo.mKeyNameWord;

	//判空
	rWriteList.push_back(ONE_TAB "if ( pbData == NULL ) \n" ONE_TAB "{\n" TWO_TAB "return;\n" ONE_TAB "} ");
	
	//处理父类
	if (bHasParent == true)
	{
		if (bParenIsTemplate)
		{
			rWriteList.push_back(ONE_TAB + strPreData + "mTempID = pbData->tempid();");
		}
		else
		{
			rWriteList.push_back(ONE_TAB + strPreData + rTemplateInfo.mParentName + "::SetFromPB(bServer, pbData->mutable_baseinfo());");
		}
	}

	for (auto it = rTemplateInfo.mFiledInfoList.begin(); it != rTemplateInfo.mFiledInfoList.end(); ++it)
	{
		if (it->mType == emTplField_Int)
		{
			rWriteList.push_back(strPrefix + strPreData + it->mFieldName + " = " + pbData + "->" + GetLowString(it->mKeyWord) + "();");
		}
		if (it->mType == emTplField_Class)
		{
			if (rTemplateInfo.mNestClassMap.find(it->mFieldType) == rTemplateInfo.mNestClassMap.end())
			{//非嵌套类，假设已经实现SetFromPB
				rWriteList.push_back(strPrefix + strPreData + it->mFieldName + ".SetFromPB(bServer," + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "());");
			}
			else
			{//嵌套类，会自动生成带关键字的SetFromPB
				rWriteList.push_back(strPrefix + "Set" + it->mTypeKeyWord + "FromPB(" + strPreData + it->mFieldName + "," + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "());");
			}
		}
		if (it->mType == emTplField_String)
		{
			rWriteList.push_back(strPrefix + "strncpy(" + strPreData + it->mFieldName + ", " + pbData + "->" + GetLowString(it->mKeyWord) + "().c_str(), sizeof(" + strPreData + it->mFieldName + ") - 1);");
		} // || it->mType == emTplField_ClassArray
		if (it->mType == emTplField_IntArray)
		{
			if (it->mDimensional == 1)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + ") && i < " + pbData + "->" + GetLowString(it->mKeyWord) + "_size(); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB + strPreData + it->mFieldName + "[i] = " + pbData + "->" + GetLowString(it->mKeyWord) + "(i);");
				rWriteList.push_back(strPrefix + "}");
			}

			if (it->mDimensional == 2)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + ") && i < " + pbData + "->" + GetLowString(it->mKeyWord) + "_size(); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB "PB" + it->mKeyWord + "* pb" + it->mKeyWord + " = " + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "(i);");
				rWriteList.push_back(strPrefix + ONE_TAB "for (int j = 0; j < ARRAY_CNT(" + strPreData + it->mFieldName + "[0]) && j < pb" + it->mKeyWord + "->" + GetLowString(it->mKeyWord) + "_size(); ++j) \n" + strPrefix + ONE_TAB "{\n");
				rWriteList.push_back(strPrefix + TWO_TAB + strPreData + it->mFieldName + "[i][j] = pb" + it->mKeyWord + "->" + GetLowString(it->mKeyWord) + "(j);");
				rWriteList.push_back(strPrefix + ONE_TAB "}\n" + strPrefix + "}");
			}
		}
		else if (it->mType == emTplField_StringArray)
		{//是数组，只可能是二维了
			if (it->mDimensional == 2)
			{
				rWriteList.push_back(strPrefix + "for int i = 0;i < ARRAY_CNT(" + strPreData + it->mFieldName + ") && i < " + pbData + "->" + GetLowString(it->mKeyWord) + "_size(); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB "strncpy(" + strPreData + it->mFieldName + "[i]," + pbData + "->" + GetLowString(it->mKeyWord) + ".c_str(),ARRAY_CNT(" + strPreData + it->mFieldName + "[0]) - 1);");
				rWriteList.push_back(strPrefix + "}");
			}
		}
		else if (it->mType == emTplField_ClassArray)
		{
			if (it->mDimensional == 1)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + ") && i < " + pbData + "->" + GetLowString(it->mKeyWord) + "_size(); ++i) \n" + strPrefix + "{");
				if (rTemplateInfo.mNestClassMap.find(it->mFieldType) == rTemplateInfo.mNestClassMap.end())
				{//非嵌套类，假设已经实现SetFromPB
					rWriteList.push_back(strPrefix + ONE_TAB + strPreData + it->mFieldName + "[i].SetFromPB(bServer," + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "(i));");
				}
				else
				{//嵌套类，会自动生成带关键字的SetFromPB
					rWriteList.push_back(strPrefix + ONE_TAB "Set" + it->mTypeKeyWord + "FromPB(" + strPreData + it->mFieldName + "[i]," + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "(i));");
				}
				rWriteList.push_back(strPrefix + "}");
			}

			if (it->mDimensional == 2)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + ") && i < " + pbData + "->" + GetLowString(it->mKeyWord) + "data_size(); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB "PB" + it->mKeyWord + "Data * pb" + it->mKeyWord + "Data = " + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "data(i);");
				rWriteList.push_back(strPrefix + ONE_TAB "for (int j = 0;  j < ARRAY_CNT(" + strPreData + it->mFieldName + "[0]) && j < pb" + it->mKeyWord + "Data->" + GetLowString(it->mKeyWord) + "_size(); ++j) \n" + strPrefix + ONE_TAB "{");
				if (rTemplateInfo.mNestClassMap.find(it->mFieldType) == rTemplateInfo.mNestClassMap.end())
				{//非嵌套类，假设已经实现SetFromPB
					rWriteList.push_back(strPrefix + TWO_TAB + strPreData + it->mFieldName + "[i][j].SetFromPB(bServer, pb" + it->mKeyWord + "Data->mutable_" + GetLowString(it->mKeyWord) + "(j)); ");
				}
				else
				{//嵌套类，会自动生成带关键字的SetFromPB
					rWriteList.push_back(strPrefix + TWO_TAB "Set" + it->mTypeKeyWord + "FromPB(" + strPreData + it->mFieldName + "[i][j], pb" + it->mKeyWord + "Data->mutable_" + GetLowString(it->mKeyWord) + "(j)); ");
				}
				rWriteList.push_back(strPrefix + ONE_TAB "}\n" + strPrefix + "}");
			}
		}
	}
	//结束
	rWriteList.push_back("}");

	for (auto it = rTemplateInfo.mNestClassMap.begin(); it != rTemplateInfo.mNestClassMap.end(); ++it)
	{
		CTemplateInfo & rNestTemplate = it->second;
		rWriteList.push_back("void " + rTemplateInfo.mClassName + "::Set" + rNestTemplate.mKeyNameWord + "FromPB(" + rNestTemplate.mClassName + "& r" + rNestTemplate.mKeyNameWord + ",PB" + rNestTemplate.mKeyNameWord + "* pbData)  \n{");
		WriteClassFieldSetFromPB(rNestTemplate, "pbData", "\t", "r" + rNestTemplate.mKeyNameWord + ".", rWriteList);
	}
}

void CBuildCodeFile::WriteClassFieldCreateFromPB(CTemplateInfo & rTemplateInfo, const string & pbData, const string & strPrefix, const string & strPreData, vector<string> & rWriteList)
{
	bool bParenIsTemplate = rTemplateInfo.mParentName.compare("CTemplate") == 0 ? true : false;
	bool bHasParent = !rTemplateInfo.mParentName.empty();
	string strClassPBName = "PB" + rTemplateInfo.mKeyNameWord;

	//判空
	rWriteList.push_back(ONE_TAB "if ( pbData == NULL ) \n" ONE_TAB "{\n" TWO_TAB "return;\n" ONE_TAB "} ");

	//处理父类
	if (bHasParent == true)
	{
		if (bParenIsTemplate)
		{
			rWriteList.push_back(ONE_TAB "pbData->set_tempid(mTempID);");
		}
		else
		{
			rWriteList.push_back(ONE_TAB + rTemplateInfo.mParentName + "::CreateTlvPB(bServer, pbData->mutable_baseinfo());");
		}
	}

	for (auto it = rTemplateInfo.mFiledInfoList.begin(); it != rTemplateInfo.mFiledInfoList.end(); ++it)
	{
		if (it->mType == emTplField_Int)
		{
			rWriteList.push_back(strPrefix + pbData + "->set_" + GetLowString(it->mKeyWord) + "(" + strPreData + it->mFieldName + ");");
		}
		if (it->mType == emTplField_Class)
		{
			if (rTemplateInfo.mNestClassMap.find(it->mFieldType) == rTemplateInfo.mNestClassMap.end())
			{//非嵌套类，假设已经实现CreateTlvPB
				rWriteList.push_back(strPrefix + strPreData + it->mFieldName + ".CreateTlvPB(bServer," + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "());");
			}
			else
			{//嵌套类，会自动生成带关键字的CreateTlvPB
				rWriteList.push_back(strPrefix + "Create" + it->mTypeKeyWord + "TlvPB(" + strPreData + it->mFieldName + "," + pbData + "->mutable_" + GetLowString(it->mKeyWord) + "());");
			}
		}
		if (it->mType == emTplField_String)
		{
			rWriteList.push_back(strPrefix + pbData + "->set_" + GetLowString(it->mKeyWord) + "(" + strPreData + it->mFieldName + ");");
		}
		if (it->mType == emTplField_IntArray)
		{
			if (it->mDimensional == 1)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + "); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB + pbData + "->add_" + GetLowString(it->mKeyWord) + "(" + strPreData + it->mFieldName + "[i]" + ");");
				rWriteList.push_back(strPrefix + "}");
			}

			if (it->mDimensional == 2)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + "); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB "PB" + it->mKeyWord + "* pb" + it->mKeyWord + " = " + pbData + "->add_" + GetLowString(it->mKeyWord) + "();");
				rWriteList.push_back(strPrefix + ONE_TAB "for (int j = 0; j < ARRAY_CNT(" + strPreData + it->mFieldName + "[0]) ; ++j) \n" + strPrefix + ONE_TAB "{");
				rWriteList.push_back(strPrefix + TWO_TAB "pb" + it->mKeyWord + "->add_" + GetLowString(it->mKeyWord) + "(" + strPreData + it->mFieldName + "[i][j]);");
				rWriteList.push_back(strPrefix + ONE_TAB "}\n" + strPrefix + "}"); 
			}
		}
		else if (it->mType == emTplField_StringArray)
		{//是数组，只可能是二维了
			if (it->mDimensional == 2)
			{
				rWriteList.push_back(strPrefix + "for int i = 0;i < ARRAY_CNT(" + strPreData + it->mFieldName + "); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB + pbData + "->add_" + GetLowString(it->mKeyWord) + "("+ strPreData + it->mFieldName + "[i]);");
				rWriteList.push_back(strPrefix + "}");
			}
		}
		else if (it->mType == emTplField_ClassArray)
		{
			if (it->mDimensional == 1)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + "); ++i) \n" + strPrefix + "{");
				if (rTemplateInfo.mNestClassMap.find(it->mFieldType) == rTemplateInfo.mNestClassMap.end())
				{//非嵌套类，假设已经实现SetFromPB
					rWriteList.push_back(strPrefix + ONE_TAB + strPreData + it->mFieldName + "[i].CreateTlvPB(bServer," + pbData + "->add_" + GetLowString(it->mKeyWord) + "());");
				}
				else
				{//嵌套类，会自动生成带关键字的SetFromPB
					rWriteList.push_back(strPrefix + ONE_TAB + "Create" + it->mTypeKeyWord + "TlvPB(" + strPreData + it->mFieldName + "[i]," + pbData + "->add_" + GetLowString(it->mKeyWord) + "());");
				}
				rWriteList.push_back(strPrefix + "}");
			}

			if (it->mDimensional == 2)
			{
				rWriteList.push_back(strPrefix + "for (int i = 0; i < ARRAY_CNT(" + strPreData + it->mFieldName + "); ++i) \n" + strPrefix + "{");
				rWriteList.push_back(strPrefix + ONE_TAB "PB" + it->mKeyWord + "Data * pb" + it->mKeyWord + " = " + pbData + "->add_" + GetLowString(it->mKeyWord) + "data();");
				rWriteList.push_back(strPrefix + ONE_TAB "for (int j = 0;  j < ARRAY_CNT(" + strPreData + it->mFieldName + "[0]); ++j) \n" + strPrefix + ONE_TAB "{");
				if (rTemplateInfo.mNestClassMap.find(it->mFieldType) == rTemplateInfo.mNestClassMap.end())
				{//非嵌套类，假设已经实现SetFromPB
					rWriteList.push_back(strPrefix + TWO_TAB + strPreData + it->mFieldName + "[i][j].CreateTlvPB(bServer,pb" + it->mKeyWord + "Data->add_" + GetLowString(it->mKeyWord) + "(); ");
				}
				else
				{//嵌套类，会自动生成带关键字的SetFromPB
					rWriteList.push_back(strPrefix + TWO_TAB "Create" + it->mTypeKeyWord + "TlvPB(" + strPreData + it->mFieldName + "[i][j], pb" + it->mKeyWord + "Data->add_" + GetLowString(it->mKeyWord) + "(); ");
				}
				rWriteList.push_back(strPrefix + ONE_TAB "}\n" + strPrefix + "}");
			}
		}
	}
	//结束
	rWriteList.push_back("}");

	//生成嵌套类相应函数
	for (auto it = rTemplateInfo.mNestClassMap.begin(); it != rTemplateInfo.mNestClassMap.end(); ++it)
	{
		CTemplateInfo & rNestTemplate = it->second;
		rWriteList.push_back("void " + rTemplateInfo.mClassName + "::Create" + rNestTemplate.mKeyNameWord + "TlvPB(" + rNestTemplate.mClassName + "& r" + rNestTemplate.mKeyNameWord + ",PB" + rNestTemplate.mKeyNameWord + "* pbData)  \n{");
		WriteClassFieldCreateFromPB(rNestTemplate, "pbData", "\t", "r" + rNestTemplate.mKeyNameWord + ".", rWriteList);
	}
}

void CBuildCodeFile::WriteClassFieldPB(CTemplateInfo & rTemplateInfo, vector<string> & rWriteList)
{
	bool bParenIsTemplate = rTemplateInfo.mParentName.compare("CTemplate") == 0 ? true : false;
	bool bHasParent = !rTemplateInfo.mParentName.empty();
	string strClassPBName = "PB" + rTemplateInfo.mKeyNameWord;
	vector<string> strExtraList;  //额外需要生成的，比如二维数组，需要多生成一个PB
	//处理父类
	int nFlagNumber = 1;
	rWriteList.push_back("message " + strClassPBName+"\n{");
	if (bHasParent == true)
	{
		if (bParenIsTemplate)
		{
			rWriteList.push_back(ONE_TAB "optional uint32 TempID = " + Int2String(nFlagNumber++) + ";");
		}
		else
		{
			rWriteList.push_back(ONE_TAB "optional PB" + rTemplateInfo.mParentKeyNameWord + " BaseInfo " + " = " + Int2String(nFlagNumber++) + "; ");
		}
	}

	for (auto it = rTemplateInfo.mFiledInfoList.begin(); it != rTemplateInfo.mFiledInfoList.end(); ++it)
	{
		string strLast = it->mKeyWord + " = " + Int2String(nFlagNumber) + ";";
		if (it->mType == emTplField_Int)
		{
			rWriteList.push_back(ONE_TAB "optional uint32 " + strLast);
		}
		if (it->mType == emTplField_Class)
		{
			rWriteList.push_back(ONE_TAB "optional PB" + it->mKeyWord + " " + strLast);
		}
		if (it->mType == emTplField_String)
		{
			rWriteList.push_back(ONE_TAB "optional string " +strLast);
		}
		if (it->mType == emTplField_IntArray)
		{
			if (it->mDimensional == 1)
			{
				rWriteList.push_back(ONE_TAB "repeated uint32 " + strLast);
			}

			if (it->mDimensional == 2)
			{//添加额外PB
				rWriteList.push_back(ONE_TAB "repeated PB" + it->mKeyWord + " " + strLast);
	
				strExtraList.push_back("message PB" + it->mKeyWord + "\n{");
				strExtraList.push_back(ONE_TAB "repeated uint32 " + it->mKeyWord + " = " + "1 ;");
				strExtraList.push_back("}");
			}
		}
		else if (it->mType == emTplField_StringArray)
		{//是数组，只可能是二维了
			if (it->mDimensional == 2)
			{
				rWriteList.push_back(ONE_TAB "repeated string " + strLast);
			}
		}
		else if (it->mType == emTplField_ClassArray)
		{//嵌套类和非嵌套类都不用添加，假设非嵌套类已经生成好，嵌套类在后面代码生成
			if (it->mDimensional == 1)
			{
				rWriteList.push_back(ONE_TAB "repeated PB" + it->mTypeKeyWord + " " + strLast);
			}

			if (it->mDimensional == 2)
			{//添加额外PB
				rWriteList.push_back(ONE_TAB "repeated PB" + it->mTypeKeyWord + "Data " + strLast);

				strExtraList.push_back("message PB" + it->mKeyWord + "Data\n{");
				strExtraList.push_back(ONE_TAB "repeated PB" + it->mTypeKeyWord + " " + it->mKeyWord + " = " + "1 ;");
				strExtraList.push_back("}");
			}
		}
		++nFlagNumber;
	}

	rWriteList.push_back("}");

	for (auto it = strExtraList.begin(); it != strExtraList.end(); ++it)
	{
		rWriteList.push_back(*it);
	}

	//生成嵌套类相应结构
	for (auto it = rTemplateInfo.mNestClassMap.begin(); it != rTemplateInfo.mNestClassMap.end(); ++it)
	{
		WriteClassFieldPB(it->second, rWriteList);
	}
}

string CBuildCodeFile::Int2String(int nValue)
{
	std::stringstream ss;
	ss << nValue;
	return ss.str();
}

void CBuildCodeFile::WriteFile(const string & rStrPath, vector<string> & rWriteList)
{
	ofstream out;
	out.open(rStrPath, ios::trunc | ios::out);
	if (!out)
	{
		return;
	}

	for (auto it = rWriteList.begin(); it != rWriteList.end(); ++it)
	{
		out << *it << endl;
	}
}