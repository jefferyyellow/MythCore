#include "dir.h"
#include <tchar.h>
#include "md5.h"
#include "tinyxml/tinyxml2.h"
using namespace tinyxml2;

void CFileList::ListFileDirectory(const wchar_t* pParentFilePath, const wchar_t* pRelativePath)
{
	WIN32_FIND_DATA ffd;
	wchar_t acFindBuffer[MAX_PATH] = {0};
	swprintf(acFindBuffer, sizeof(acFindBuffer), _T("%s/*"), pParentFilePath);

	HANDLE hHandle = FindFirstFile(acFindBuffer, &ffd);
	if (INVALID_HANDLE_VALUE == hHandle)
	{
		return;
	}

	do 
	{
		if (ffd.cFileName[0] == _T('.'))
		{
			continue;
		}
		wchar_t acRelativeFilePath[MAX_PATH] = { 0 };
		if (pRelativePath[0] == '\0')
		{
			swprintf(acRelativeFilePath, sizeof(acRelativeFilePath), _T("%s"), ffd.cFileName);
		}
		else
		{
			swprintf(acRelativeFilePath, sizeof(acRelativeFilePath), _T("%s/%s"), pRelativePath, ffd.cFileName);
		}

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wchar_t acBuffer[MAX_PATH] = {0};
			swprintf(acBuffer, sizeof(acBuffer), _T("%s/%s"), pParentFilePath, ffd.cFileName);
			ListFileDirectory(acBuffer, acRelativeFilePath);
			continue;
		}

		wchar_t acLastFilePath[MAX_PATH] = {0};
		swprintf(acLastFilePath, sizeof(acLastFilePath), _T("%s/%s"), pParentFilePath, ffd.cFileName);
		CFileData* pFileData = new CFileData();
		bool bResult = GetFileInfo(acLastFilePath, pFileData, ffd.nFileSizeLow);
		if (!bResult)
		{
			exit(-1);
			return;
		}
		pFileData->mFileSize = ffd.nFileSizeLow;
		FileTimeToTime_t(ffd.ftLastWriteTime, &(pFileData->mModifyTime));

		mFileList[ws2s(acRelativeFilePath)] = pFileData;
	} while (FindNextFile(hHandle, &ffd) != 0);


	FindClose(hHandle);
}

char CFileList::C2Hex(char c)
{
	if (c < 10)
	{
		return  c + '0';
	}
	else
	{
		return (c - 10)+ 'a';
	}
}


bool CFileList::GetFileInfo(wchar_t *pFilePath, CFileData* pFileData, uint32 nFileSize)
{
	if (NULL == pFilePath || NULL == pFileData)
	{
		return false;
	}

	HANDLE hFileHandle = CreateFile(pFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFileHandle)
	{
		return false;
	}


	char* pFileBuff= new char[nFileSize];
	DWORD nTmpReadSize = 0;
	for (uint32 nReadSize = 0; nReadSize < nFileSize; )
	{
		ReadFile(hFileHandle, pFileBuff, nFileSize - nReadSize, &nTmpReadSize, NULL);
		nReadSize += nTmpReadSize;
	}

	unsigned char acMd5[MD5_LENGTH];
	CMD5::compute(pFileBuff, nFileSize, acMd5);
	int nCount = 0;
	for (int i = 0; i < MD5_LENGTH; ++i)
	{
		pFileData->mMd5[nCount++] = C2Hex(acMd5[i] >> 4);
		pFileData->mMd5[nCount++] = C2Hex(acMd5[i] & 0xF);
	}

	delete[]pFileBuff;
	return true;
}

void CFileList::FileTimeToTime_t(FILETIME ft, time_t *t)
{
	LONGLONG ll;

	ULARGE_INTEGER            ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ll = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
	*t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
}

void CFileList::TimetToFileTime(time_t t, LPFILETIME pft)
{
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}

// 将当前的文件列表写入XML
void CFileList::SaveFileList()
{
	tinyxml2::XMLDocument doc;
	XMLElement* pRoot = doc.NewElement("files");
	doc.InsertEndChild(pRoot);

	FILE_LIST::iterator it = mFileList.begin();
	for (; it != mFileList.end(); ++ it)
	{
		CFileData* pFileData = it->second;
		if (NULL == pFileData)
		{
			continue;
		}
		XMLElement* pFileElem = doc.NewElement("file");
		pFileElem->SetAttribute("path", it->first.c_str());


		pFileElem->SetAttribute("MD5", pFileData->mMd5);
		pFileElem->SetAttribute("Modified", pFileData->mModifyTime);
		pFileElem->SetAttribute("size", pFileData->mFileSize);

		pRoot->InsertEndChild(pFileElem);
	}

	doc.SaveFile("MD5.xml");
}

// 加载当前的文件列表
void CFileList::LoadFileList()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("MD5.xml");

	XMLElement* pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		return;
	}
	mFileList.clear();
	XMLElement* pFileElem = pRoot->FirstChildElement("file");
	for (; NULL != pFileElem; pFileElem = pFileElem->NextSiblingElement("file"))
	{
		CFileData* pFileData = new CFileData();
		if (NULL == pFileData)
		{
			continue;
		}
		char acFilePath[MAX_PATH] = {0};
		strncpy(acFilePath, pFileElem->Attribute("path"), sizeof(acFilePath));

		strncpy(pFileData->mMd5, pFileElem->Attribute("MD5"), sizeof(pFileData->mMd5));
		pFileData->mModifyTime = pFileElem->IntAttribute("Modified");
		pFileData->mFileSize = pFileElem->IntAttribute("size");

		mFileList[acFilePath] = pFileData;
	}
}

