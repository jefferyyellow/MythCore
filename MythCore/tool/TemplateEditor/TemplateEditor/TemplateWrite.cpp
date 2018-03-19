#include "StdAfx.h"
#include "TemplateWrite.h"
#include "TemplateManager.h"
#include "template.h"
#include "template.hxx.pb.h"
#include "MainFrm.h"
#include "tinyxml2.h"
#include "ParseProtobuf.h"
#include <direct.h>
using namespace tinyxml2;
CTemplateWrite* CTemplateWrite::m_pInst = NULL;
CTemplateWrite::CTemplateWrite(void)
{
	if (m_pInst != NULL)
	{
		// ������
		return;
	}
	m_bWritingPB = FALSE;
	m_pInst = this;
}

CTemplateWrite::~CTemplateWrite(void)
{

}

// ������д��PB��ʽ�Ķ�����
BOOL CTemplateWrite::WriteToPB(CTemplateManager* pTemplateManager)
{
	if (!pTemplateManager->LoadAllData())
	{
		AfxMessageBox(_T("��ȡģ�����ݴ��󣡣���"));
		return FALSE;
	}

	hash_map<string, string>		HashName;

	char acTemplateName[256] = {0};
	CString strError;
	if(!VerifyValue(pTemplateManager, strError))
	{
		AfxMessageBox(strError);
		return FALSE;
	}

	// ����ͻ���ģ��ͷ�����ģ��
	PBTplTemplate* pServerTemp = new PBTplTemplate();
	// ��emLevelRange_None,������һ���ļ�,����������һ���ļ�,�������ļ�����
	PBTplTemplate* pClientTemp = new PBTplTemplate();
	if ( pServerTemp == NULL || pClientTemp == NULL )
	{
		::AfxMessageBox( L"����pb�ڴ�ʧ�ܣ�ģ������ʧ�ܣ���" );
		return FALSE;
	}


	// 10K�Ļ�����,Ӧ�ù�����
	char* pBuffer = new char[emSingleTemplateMaxSize];

	SetProcessText(_T("��ʼдģ��:"));
	SetProcess(0);
	float fPercent = 100.0f / (float)pTemplateManager->GetOriginalTemp().size();
	for (size_t l = 0; l < pTemplateManager->GetOriginalTemp().size(); l++)
	{
		SetProcess((int)(l * fPercent));
		COriginalTemplate* pOriginalTemplate = pTemplateManager->GetOriginalTemp()[l];
		if (NULL == pOriginalTemplate)
		{
			// �����
			return FALSE;
		}


		bool bItemTemplateType = false;
		bool bFashioMallTemplateType = false;
		for (size_t i = 0; i < pOriginalTemplate->m_vecTemplate.size(); i++)
		{
			CTemplateInfo* pTemplateInfo = pOriginalTemplate->m_vecTemplate[i];
			if (NULL == pTemplateInfo)
			{
				// ������,��ʾ�����
				return FALSE;
			}

			int nTemplateSize = pOriginalTemplate->m_nTemplateSize;
			nTemplateSize += 4;

			for (size_t j = 0; j < pTemplateInfo->m_vecTemplateFieldData.size(); j++)
			{
				CTemplateFieldData* pTemplateData = pTemplateInfo->m_vecTemplateFieldData[j];
				if (NULL == pTemplateData)
				{
					// ������,��ʾ�����
					return FALSE;
				}
				// ���û�����
				memset(pBuffer, 0, emSingleTemplateMaxSize);

				char* pDataBuffer = pBuffer;

				// д��ģ��ID
				{
					// �Ƿ��Ѿ�Խ��
					if (pDataBuffer + sizeof(int)- pBuffer > emSingleTemplateMaxSize)
					{
						// �����,�Ѿ�Խ���� 
						AfxMessageBox(_T("10K�Ļ�����Խ���ˣ�����"));
						return FALSE;
					}
					int* pTmpInt = (int*)pDataBuffer;
					*pTmpInt = pTemplateData->m_nTemplateID;
					pDataBuffer += sizeof(int);
				}

				int nLangCount = 0;

				for (size_t k = 0; k < pOriginalTemplate->m_vecOriginalField.size(); k++)
				{
					string& strFieldType = pOriginalTemplate->m_vecOriginalField[k].m_strFieldType;
					string& strFieldName = pOriginalTemplate->m_vecOriginalField[k].m_strFieldName;
					string& strValue = pTemplateData->m_vecValue[k];

					if (strFieldType.find("INT4") != string::npos)
					{
						int nValue = atoi(strValue.c_str());
						// �Ƿ��Ѿ�Խ��
						if (pDataBuffer + sizeof(int)- pBuffer > emSingleTemplateMaxSize)
						{
							// �����,�Ѿ�Խ���� 
							AfxMessageBox(_T("10K�Ļ�����Խ���ˣ�����"));
							return FALSE;
						}
						int* pTmpInt = (int*)pDataBuffer;
						*pTmpInt = nValue;
						pDataBuffer += sizeof(int);

					}
					else if (strFieldType.find("STRING") != string::npos)
					{

						int nIndex	= int ( strFieldType.find( "STRING") );
						// ȡ���ַ�������
						string	strNumberText = strFieldType.substr(6 + nIndex);
						int nStrNumber		= atoi(strNumberText.c_str());

						if (pDataBuffer + nStrNumber - pBuffer > emSingleTemplateMaxSize)
						{
							AfxMessageBox(_T("10K�Ļ�����Խ���ˣ�����"));
							// �����,�Ѿ�Խ����
							return FALSE;
						}

						if (strFieldName[0] == _T('$') && strValue[0] != _T('\0'))
						{
							char acUTF8[10 * 1024] = {0};
							ASCIIToUTF8(strValue.c_str(), strValue.size(), acUTF8, 10 * 1024);

							char acLangID[32] = {0};
							_snprintf(acLangID, sizeof(acLangID) - 1, "tpl_%d_%d", pTemplateData->m_nTemplateID, nLangCount);
							HashName[acLangID] = acUTF8;

							char acLangIDUTF8[MAX_PATH] = {0};
							ASCIIToUTF8(acLangID, sizeof(acLangID), acLangIDUTF8, MAX_PATH);


							char* pcStr = (char*)pDataBuffer;
							strncpy(pcStr, acLangIDUTF8, nStrNumber-1);
							*(pcStr + nStrNumber -1) = '\0';
							pDataBuffer += nStrNumber;

							++ nLangCount;
						}
						else
						{
							char acUTF8[10 * 1024] = {0};
							ASCIIToUTF8(strValue.c_str(), strValue.size(), acUTF8, 10 * 1024);

							char* pcStr = (char*)pDataBuffer;
							strncpy(pcStr, acUTF8, nStrNumber-1);
							*(pcStr + nStrNumber -1) = '\0';
							pDataBuffer += nStrNumber;
						}
					}
					else if (strFieldType.find("INT8") != string::npos)
					{
						__int64 lValue = _atoi64(strValue.c_str());
						if (pDataBuffer + sizeof(__int64) - pBuffer > emSingleTemplateMaxSize)
						{
							AfxMessageBox(_T("10K�Ļ�����Խ���ˣ�����"));
							// �����,�Ѿ�Խ����
							return FALSE;
						}
						__int64* pTmplong = (__int64*)pDataBuffer;
						*pTmplong = lValue;
						pDataBuffer += sizeof(__int64);
					}
					else if (strFieldType.find("FLOAT") != string::npos)
					{
						float fValue = float(atof(strValue.c_str()));
						// �Ƿ��Ѿ�Խ��
						if (pDataBuffer + sizeof(float) - pBuffer > emSingleTemplateMaxSize)
						{
							AfxMessageBox(_T("10K�Ļ�����Խ���ˣ�����"));
							// �����,�Ѿ�Խ���� 
							return FALSE;
						}

						float* pTmpFloat = (float*)pDataBuffer;
						*pTmpFloat = fValue;
						pDataBuffer += sizeof(float);
					}
					else if (strFieldType.find("DOUBLE") != string::npos)
					{
						double fValue = atof(strValue.c_str());
						// �Ƿ��Ѿ�Խ��
						if (pDataBuffer + sizeof(double) - pBuffer > emSingleTemplateMaxSize)
						{
							AfxMessageBox(_T("10K�Ļ�����Խ���ˣ�����"));
							// �����,�Ѿ�Խ���� 
							return FALSE;
						}

						double* pTmpDouble = (double*)pDataBuffer;
						*pTmpDouble = fValue;
						pDataBuffer += sizeof(double);
					}
				}


				// 
				bool bItem = false;
				bool bFashionMall = false;
				if(!AddPBData(pServerTemp, pOriginalTemplate->m_strOriginName, pBuffer, (int)(pDataBuffer - pBuffer)))
				{
					AfxMessageBox(CString(_T("ģ���ֶζ�Ӧ����,�����±���ģ��༭��������������ԭʼģ���!\n") + pOriginalTemplate->m_strOriginName));
					return FALSE;
				}

				if (!AddPBData(pClientTemp, pOriginalTemplate->m_strOriginName, pBuffer, (int)(pDataBuffer - pBuffer)))
				{
					AfxMessageBox(CString(_T("ģ���ֶζ�Ӧ����,�����±���ģ��༭��������������ԭʼģ���!\n") + pOriginalTemplate->m_strOriginName));
					return FALSE;
				}
			}
		}
	}
	//WritePBToXml(*pServerTemp);
	// �ֱ�д��������ͻ���ģ��
	fstream fstm_server( "template_server.dat", ios::out | ios::binary );
	if (fstm_server.is_open() == false)
	{
		::AfxMessageBox( L"����ģ��dat�ļ�ʧ��!" );
		return FALSE;
	}

	int nTempVersion = 1;
	// д�汾��
	fstm_server.write( (char*)&nTempVersion, sizeof( int ) );

	// ��������ģ�����л���һ��������
	char* pResultBuffer= new char[emAllTemplateMaxSize];
	if ( pServerTemp->SerializeToArray( pResultBuffer, pServerTemp->ByteSize() ) == false )
	{
		::AfxMessageBox( L"Serialize Server PB failed!" );
		return FALSE;
	}
	// ��������д���ļ�
	fstm_server.write( pResultBuffer, pServerTemp->ByteSize() );
	fstm_server.close();


	fstream fstm_client( "template_client_necessary.dat", ios::out | ios::binary );
	if ( fstm_client.is_open() == false )
	{
		::AfxMessageBox( L"����ģ��dat�ļ�ʧ��!" );
		return FALSE;
	}
	fstm_client.write( (char*)&nTempVersion, sizeof( int ) );

	// ���ͻ����ļ����л���������
	memset(pResultBuffer, 0, emAllTemplateMaxSize);
	if ( pClientTemp->SerializeToArray( pResultBuffer, pClientTemp->ByteSize() ) == false )
	{
		::AfxMessageBox( L"Serialize Client PB failed!" );
		return FALSE;
	}

	// �ͻ���ģ������д���ļ�
	fstm_client.write( pResultBuffer, pClientTemp->ByteSize() );
	fstm_client.close();

	delete pServerTemp;
	delete []pClientTemp;
	delete []pBuffer;
	delete []pResultBuffer;
	return TRUE;
}


// ����xml�汾��ǰ��
BOOL CTemplateWrite::WritePBToXml(PBTplTemplate& rTemplate)
{
	char acDirectoryName[MAX_PATH] = {0};
	_snprintf(acDirectoryName, sizeof(acDirectoryName), "client/%s", "Item");
	_mkdir(acDirectoryName);

	CParseProtobuf tProtoBuf;
	int nTempID = 0;
	tProtoBuf.Message2Xml(rTemplate, NULL, &nTempID);
	//tProtoBuf.Message2Lua(rTemplate);
	//tProtoBuf.WriteToLua(); //��ֹ��©���һ����lua�ļ�
	return TRUE;
}
// У������
BOOL CTemplateWrite::VerifyValue(CTemplateManager* pTemplateManager, CString& strOutError)
{
	if(pTemplateManager->GetOriginalTemp().size() <= 0)
	{
		return FALSE;
	}

	SetProcessText(_T("��ʼ��������У��:"));
	SetProcess(0);

	float fPercent = 100.0f / (float)pTemplateManager->GetOriginalTemp().size();

	BOOL bVerify = TRUE;
	for (size_t l = 0; l < pTemplateManager->GetOriginalTemp().size(); l++)
	{

		SetProcess((int)(l * fPercent));
		COriginalTemplate* pOriginalTemplate = pTemplateManager->GetOriginalTemp()[l];
		if (NULL == pOriginalTemplate)
		{
			// �����
			AfxMessageBox(_T("δ֪����"));
			return FALSE;
		}
		for (size_t i = 0; i < pOriginalTemplate->m_vecTemplate.size(); i++)
		{


			CTemplateInfo* pTemplateInfo = pOriginalTemplate->m_vecTemplate[i];
			if (NULL == pTemplateInfo)
			{
				// ������,��ʾ�����
				AfxMessageBox(_T("δ֪����"));
				return FALSE;
			}

			for (size_t j = 0; j < pTemplateInfo->m_vecTemplateFieldData.size(); j++)
			{
				CTemplateFieldData* pTemplateData = pTemplateInfo->m_vecTemplateFieldData[j];
				if (NULL == pTemplateData)
				{
					// ������,��ʾ�����
					AfxMessageBox(_T("δ֪����"));
					return FALSE;
				}

				for (size_t k = 0; k < pOriginalTemplate->m_vecOriginalField.size(); k++)
				{
					string& strFieldType = pOriginalTemplate->m_vecOriginalField[k].m_strFieldType;
					string& strValue = pTemplateData->m_vecValue[k];
					string& strType = pOriginalTemplate->m_vecOriginalField[k].m_strType;
					if (strType.find("COMBOSINGLE") != string::npos)
					{
						if(!VerifyComboSingle(strType, strValue))
						{
							bVerify = FALSE;
						}
					}
					else if (strType.find("CHECKMUTI") != string::npos)
					{
						if(!VerifyCheckMuti(strType, strValue))
						{
							bVerify = FALSE;
						}
					}
					else if (strType.find("EDITNUMBER") != string::npos)
					{
						if(!VerifyEditNumber(strType, strValue))
						{
							bVerify = FALSE;
						}
					}
					else if (strType.find("EDITSTRING") != string::npos)
					{
						if(!VerifyEditString(strFieldType, strValue))
						{
							bVerify = FALSE;
						}
					}
					else if (strType.find("TEMPPATH") != string::npos)
					{
						if (!VerifyTempPath(strFieldType, strValue))
						{
							bVerify = FALSE;
						}
					}

					if (!bVerify)
					{
						TCHAR acIndex[MAX_PATH]	= {0};
						_itow((int)(k + 1), acIndex, 10);
						
						TCHAR acTemplateID[MAX_PATH] = {0};
						_itow(pTemplateData->m_nTemplateID, acTemplateID, 10);

						strOutError = CString("����У����󣡣���\n") 
							+ CString("ģ���:")  + pTemplateInfo->m_strPathFile + CString("\n")
							+ CString("�ֶ�ID:") + CString(acTemplateID) + CString("\n")
							+ CString("ģ������:") + CString(s2ws(pTemplateData->m_strTemplateName).c_str()) + CString("\n")
							+ CString("�ֶ�����:") + CString(acIndex) + CString("\n")
							+ CString("�ֶ�����:") + CString(s2ws(pOriginalTemplate->m_vecOriginalField[k].m_strFieldName).c_str());
						return FALSE;
					}
					

				}
			}
		}
	}
	return TRUE;
}


void CTemplateWrite::SetProcess(int nProgress)
{
	CMainFrame *pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(NULL == pMain)
	{
		return;
	}
	pMain->SetProcess(nProgress);
	TRACE("\nPROCESS %d\n", nProgress);
}

void CTemplateWrite::SetProcessText(wchar_t* pText)
{
	if(NULL == pText)
	{
		return;
	}
	CMainFrame *pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(NULL == pMain)
	{
		return;
	}
	::SendMessage(pMain->GetStatusHwnd(), SB_SETTEXT, 1, LPARAM(pText));
}

// У�鵥ѡһ����
BOOL CTemplateWrite::VerifyComboSingle(string strType, string& strValue)
{
	size_t nIndex = strType.find("(");
	if (nIndex != string::npos)
	{
		strType = strType.substr(nIndex + 1);
	}
	int nOriginValue = atoi(strValue.c_str());

	size_t nValueIndex = 0;
	string strName;
	int nValue = 0;
	vector<DWORD>	vecData;
	while((nIndex = strType.find(",")) != string::npos)
	{
		strName = strType.substr(0, nIndex);
		nValueIndex = strName.find("=");
		if (nValueIndex != string::npos)
		{
			nValue = atoi(strName.substr(nValueIndex + 1).c_str());
		}
		else
		{
			if (((int)vecData.size()) - 1 >= 0)
			{
				nValue = vecData[vecData.size() - 1] + 1;
			}
			else
			{
				nValue = 1;
			}
		}

		vecData.push_back(nValue);
		if (nValue == nOriginValue)
		{
			return TRUE;
		}
		strType = strType.substr(nIndex + 1);
	}


	nIndex = strType.find(")");
	if (nIndex != string::npos)
	{
		strName = strType.substr(0, nIndex);
		nValueIndex = strName.find("=");
		if (nValueIndex != string::npos)
		{
			nValue = atoi(strName.substr(nValueIndex + 1).c_str());
			vecData.push_back(nValue);
		}
		else
		{
			if (((int)vecData.size()) - 1 >= 0)
			{
				nValue = vecData[vecData.size() - 1] + 1;
			}
			else
			{
				nValue = 1;
			}
		}

		if (nValue == nOriginValue)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// У���ѡ����
BOOL CTemplateWrite::VerifyCheckMuti(string strType, string& strValue)
{
	size_t nIndex = strType.find("(");
	if (nIndex != string::npos)
	{
		strType = strType.substr(nIndex + 1);
	}

	int nOriginValue = atoi(strValue.c_str());

	int nValue = 0;
	int nCheckCount = 0;
	int nOffset = 1;
	while((nIndex = strType.find(",")) != string::npos && nCheckCount < 32)
	{
		nValue |= nOffset;
		nOffset = nOffset << 1;
		strType = strType.substr(nIndex + 1);
		nCheckCount ++;
	}


	nIndex = strType.find(")");
	if (nIndex != string::npos && nCheckCount <= 32)
	{
		nValue |= nOffset;
	}

	return nValue >= nOriginValue;
}

// У����������
BOOL CTemplateWrite::VerifyEditNumber(string strType, string& strValue)
{

	size_t nStartIndex = strType.find("(");
	size_t nEndIndex = strType.find(")");
	strType = strType.substr(nStartIndex + 1, nEndIndex - nStartIndex - 1);
	size_t nInterval = strType.find(",");
	string strMin = strType.substr(0, nInterval);
	string strMax = strType.substr(nInterval + 1);

	__int64 nMin = _atoi64(strMin.c_str());
	__int64 nMax = _atoi64(strMax.c_str());

	__int64 nValue = _atoi64(strValue.c_str());
	if (nValue >= nMin && nValue <= nMax)
	{
		return TRUE;
	}

	return FALSE;
}
// У���ַ�������
BOOL CTemplateWrite::VerifyEditString(string strFieldType, string& strValue)
{

	size_t nIndex = string::npos;
	string strNum;
	int nValue = 0;
	if ((nIndex = strFieldType.find("STRING")) != string::npos)
	{
		strFieldType = strFieldType.substr(nIndex + 6);
		nValue = atoi(strFieldType.c_str());
	}
	else
	{
		return FALSE;
	}
	if(nValue <= (int)strValue.length())
	{
		return FALSE;
	}

	return TRUE;
}

// У��TEMPPATH����
BOOL CTemplateWrite::VerifyTempPath(string strFieldType, string& strValue)
{
	int nTemplateID = atoi(strValue.c_str());
	// ����ģ��IDû������
	if (nTemplateID == 0)
	{
		return TRUE;
	}

	if (nTemplateID < 0 && nTemplateID > 65535)
	{
		return FALSE;
	}
	if (CTemplateManager::GetSingletonPtr()->IsExistTemplate(nTemplateID))
	{
		return TRUE;
	}
	return FALSE;
}

// ���ַ���ת����UTF8
void CTemplateWrite::ASCIIToUTF8(const char* pSr, int nSrcLen, char*pDst, int nDstLen)
{
	wchar_t acBuffer[10* 1024] = {0};
	if (MultiByteToWideChar(CP_ACP, 0, pSr, -1, acBuffer, 10* 1024) == 0)
	{
		pDst[0] = '\0';
	}
	if (WideCharToMultiByte(CP_UTF8, 0, acBuffer, -1, pDst, nDstLen, NULL, 0) == 0)
	{
		pDst[0] = '\0';
	}
}

// ��PB�ṹ���������
BOOL CTemplateWrite::AddPBData(PBTplTemplate* pTplTemplate, CString& strTempName, char* pTempData, int nTempLen)
{
	if ( pTempData == NULL || pTplTemplate == NULL )
	{
		return FALSE;
	}

	PBTplItemSet*	pbTplItemSet = pTplTemplate->mutable_itemset();
	PBTplConfigSet* pbTplConfigSet = pTplTemplate->mutable_configset();
	PBTplSkillSet*	pbTplSkillSet = pTplTemplate->mutable_skillset();
	PBTplNPCSet*	pbTplNPCSet = pTplTemplate->mutable_npcset();

	CREATE_DATA_TO_PB("d�ȼ������", CTplLevelExpConfig, pbTplConfigSet, levelexpconfig);
	CREATE_DATA_TO_PB("VIP�����", CTplVIPConfig, pbTplConfigSet, vipconfig);
	CREATE_DATA_TO_PB("g����NPC", CTplFuncNPC, pbTplNPCSet, funcnpc);
	CREATE_DATA_TO_PB("NPC������Ϣ", CTplOgre, pbTplNPCSet, ogre);
	CREATE_DATA_TO_PB("x�������Ϣ", CTplNewPlayerConfig, pbTplConfigSet, newplayerconfig);
	CREATE_DATA_TO_PB("j����", CTplSkill, pbTplSkillSet, skill);
	CREATE_DATA_TO_PB("d�����", CTplDropTable, pbTplConfigSet, droptable);
	return TRUE;
}
