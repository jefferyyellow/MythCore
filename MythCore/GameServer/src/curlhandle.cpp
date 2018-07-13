#include "locallogjob.h"
#include "curlhandle.h"
size_t onWriteFuncCallBack(void *pPtr, size_t nSize, size_t nMemNum, void *pUserData)
{
	if (NULL == pUserData)
	{
		return nSize * nMemNum;
	}

	CURLSession* pURLSession = (CURLSession*)pUserData;
	// ��������Ϊ�գ���ʾ��Ϊ�˻�ȡ�ļ�
	FILE* pFile = pURLSession->getFile();
	if (NULL != pFile)
	{
		fwrite(pPtr, nSize, nMemNum, pFile);
	}
	else
	{
		// �Ƿ���Ҫ�ص��������Ҫ�ص�������õ����ݴ�����
		if (pURLSession->getNeedCallBack())
		{
			int nFreeSize = RETURN_DATA_LENGTH - pURLSession->getReturnSize();
			size_t nDataSize = nSize * nMemNum;
			if (nFreeSize > (int)nDataSize)
			{
				strncpy(pURLSession->getReturnData(), (char*)pPtr, nDataSize);
				pURLSession->setReturnSize(pURLSession->getReturnSize() + nDataSize);
			}
		}
	}
	
	return nSize * nMemNum;
}

CURLHandle::CURLHandle()
{
	init();
}

CURLHandle::~CURLHandle()
{
	clear();
}

void CURLHandle::init()
{
    mMultiURL = NULL;
    mCompleteCallBack = NULL;
}

bool CURLHandle::initCURL(int nMaxURLNum)
{
	// ��ʼ��ȫ��
	curl_global_init(CURL_GLOBAL_ALL);
	// ��ʼ��multi curl
	mMultiURL = curl_multi_init();
	if (NULL == mMultiURL)
	{
		return false;
	}

	// ��������������
	curl_multi_setopt(mMultiURL, CURLMOPT_MAXCONNECTS, nMaxURLNum);
	return true;
}

void CURLHandle::clear()
{
	for (size_t i = 0; i < mvecURL.size(); ++ i)
	{
		if (NULL != mvecURL[i])
		{
			curl_easy_cleanup(mvecURL[i]);
		}
	}

	int nMessageNum = 0;
	CURLMsg* pURLMsg = NULL;
	while (true)
	{
		// ��ȡû�н��յ���Ϣ
		pURLMsg = curl_multi_info_read(mMultiURL, &nMessageNum);
		if (NULL == pURLMsg)
		{
			break;
		}
		// ɾ����Ӧ��easy url
		curl_multi_remove_handle(mMultiURL, pURLMsg->easy_handle);
		// ��ն�Ӧ��url
		curl_easy_cleanup(pURLMsg->easy_handle);
	}
	for (size_t i = 0; i < mvecURLSession.size(); ++i)
	{
		if (NULL != mvecURLSession[i])
		{
			delete mvecURLSession[i];
		}
	}
	mvecURLSession.clear();

	// ���multi url
	curl_multi_cleanup(mMultiURL);
	// ȫ������
	curl_global_cleanup();
}

void CURLHandle::sendRequest(const char* pHttpURL, const char* pPostData, EmHttpType eHttpType, bool bNeedCallBack)
{
	CURL* pURL = NULL;
	// ���û�п����URL��
	if (mvecURL.size() <= 0)
	{
		// ֱ�ӷ���һ��
		pURL = initEasyURL();
	}
	else
	{
		// ȡ�����б�����һ��
		pURL = mvecURL[mvecURL.size() - 1];
		mvecURL.pop_back();
	}

	if (NULL == pURL)
	{
		return;
	}

	CURLSession* pURLSession = NULL;
	// ��session�����б�����
	if (mvecURLSession.size() <= 0)
	{
		pURLSession = new CURLSession;
	}
	else
	{
		// ȡ�����б�����һ��
		pURLSession = mvecURLSession[mvecURLSession.size() - 1];
		mvecURLSession.pop_back();
	}

	if (NULL == pURLSession)
	{
		return;
	}

	// SSL(https)�ķ�ʽ��������֤�飬ѡ�񲻰�ȫ�ķ�ʽ��
	if (eHttpType & emHttpTypeSSL)
	{
		//����֤����
		curl_easy_setopt(pURL, CURLOPT_SSL_VERIFYPEER, 0L);
	}

	// ���ӳ�ʱ
	curl_easy_setopt(pURL, CURLOPT_CONNECTTIMEOUT, 20L);
	curl_easy_setopt(pURL, CURLOPT_PRIVATE, pURLSession);
	curl_easy_setopt(pURL, CURLOPT_WRITEDATA, pURLSession);
	pURLSession->setNeedCallBack(bNeedCallBack);
	pURLSession->setHttpType(eHttpType);
	pURLSession->setHttpUrl(pHttpURL);
	// �����ȡ�ļ���
	if (eHttpType & emHttpTypeFile && NULL != pPostData)
	{
		// pPostData���ļ���
		char tFilePath[256] = { 0 };
		sprintf(tFilePath, "%s", pPostData);
		// ���ļ����
		FILE* pFile = fopen(tFilePath, "w+");
		pURLSession->setFile(pFile);
	}

	if (eHttpType & emHttpTypeGet)
	{
		setHttpGetOption(pHttpURL, pPostData, pURL, pURLSession);
	}
	else
	{
		setHttpPostRequest(pHttpURL, pPostData, pURL, pURLSession);
	}

	curl_multi_add_handle(mMultiURL, pURL);
}

/// ����HTTP Get��ʽ��ѡ��
void CURLHandle::setHttpGetOption(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession)
{
	curl_easy_setopt(pURL, CURLOPT_URL, pURLSession->getHttpUrl());
	curl_easy_setopt(pURL, CURLOPT_HEADER, 0);
	curl_easy_setopt(pURL, CURLOPT_HTTPGET, 1);
}

/// ����HTTP Post��ʽ��ѡ��
void CURLHandle::setHttpPostRequest(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession)
{
	curl_easy_setopt(pURL, CURLOPT_URL, pURLSession->getHttpUrl());
	curl_easy_setopt(pURL, CURLOPT_HEADER, 1L);
	curl_easy_setopt(pURL, CURLOPT_POST, 1L);
	curl_easy_setopt(pURL, CURLOPT_POSTFIELDS, pURLSession);
}

/// ��ʼEasy URL
CURL* CURLHandle::initEasyURL()
{
	CURL* pURL = curl_easy_init();
	if (NULL == pURL)
	{
		return NULL;
	}

	curl_easy_setopt(pURL, CURLOPT_TCP_KEEPALIVE, 1L);
	curl_easy_setopt(pURL, CURLOPT_WRITEFUNCTION, onWriteFuncCallBack);
	//curl_easy_setopt(pURL, CURLOPT_VERBOSE, 1L);
	return pURL;
}

void CURLHandle::doing()
{
	int nHandle = 0;
	CURLMcode code = curl_multi_perform(mMultiURL, &nHandle);
	while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(mMultiURL, &nHandle));
	//curl_multi_perform(mMultiURL, &nHandle);

	fd_set readSet;
	fd_set writeSet;
	fd_set exceptSet;
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&exceptSet);
	
	int nMaxFd = 0;
	curl_multi_fdset(mMultiURL, &readSet, &writeSet, &exceptSet, &nMaxFd);
	if (nMaxFd > 0)
	{
		timeval tv = { 0 };
		int tRet = select(nMaxFd + 1, &readSet, &writeSet, &exceptSet, &tv);
		if (-1 != tRet)
		{
			while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(mMultiURL, &nHandle));
		}
	}

	checkComplete();
}

/// ���һ��Web�����Ƿ����
void CURLHandle::checkComplete()
{
	int nMessageNum = 0;
	CURLMsg* pMsg = NULL;

	while (true)
	{
		pMsg = curl_multi_info_read(mMultiURL, &nMessageNum);
		if (NULL == pMsg)
		{
			break;
		}

		if (CURLMSG_DONE != pMsg->msg)
		{
			continue;
		}

		CURLSession* pURLSession = NULL;
		curl_easy_getinfo(pMsg->easy_handle, CURLINFO_PRIVATE, &pURLSession);

		if (CURLE_OK != pMsg->data.result)
		{
			const char* pErrorMsg = curl_easy_strerror(pMsg->data.result);
			if (NULL != pErrorMsg)
			{
				LOG_ERROR("%s", pErrorMsg);
			}
		}

		if (NULL != pURLSession)
		{
			if (pURLSession->getNeedCallBack() && NULL != mCompleteCallBack)
			{
				mCompleteCallBack(pURLSession);
			}
			// �Ѿ��������ˣ����ļ�����ر�
			if (pURLSession->getHttpType() & emHttpTypeFile)
			{
				fclose(pURLSession->getFile());
			}
		}
		// ���յ�URL�����б�
		mvecURL.push_back(pMsg->easy_handle);
		curl_multi_remove_handle(mMultiURL, pMsg->easy_handle);
		// ���յ�URL Session�����б� 
		mvecURLSession.push_back(pURLSession);
	}
}