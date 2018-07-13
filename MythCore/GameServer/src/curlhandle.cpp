#include "locallogjob.h"
#include "curlhandle.h"
size_t onWriteFuncCallBack(void *pPtr, size_t nSize, size_t nMemNum, void *pUserData)
{
	if (NULL == pUserData)
	{
		return nSize * nMemNum;
	}

	CURLSession* pURLSession = (CURLSession*)pUserData;
	// 如果句柄不为空，表示是为了获取文件
	FILE* pFile = pURLSession->getFile();
	if (NULL != pFile)
	{
		fwrite(pPtr, nSize, nMemNum, pFile);
	}
	else
	{
		// 是否需要回调，如果需要回调，将获得的数据存起来
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
	// 初始化全局
	curl_global_init(CURL_GLOBAL_ALL);
	// 初始化multi curl
	mMultiURL = curl_multi_init();
	if (NULL == mMultiURL)
	{
		return false;
	}

	// 设置最大的连接数
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
		// 读取没有接收的信息
		pURLMsg = curl_multi_info_read(mMultiURL, &nMessageNum);
		if (NULL == pURLMsg)
		{
			break;
		}
		// 删除对应的easy url
		curl_multi_remove_handle(mMultiURL, pURLMsg->easy_handle);
		// 清空对应的url
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

	// 清空multi url
	curl_multi_cleanup(mMultiURL);
	// 全局清理
	curl_global_cleanup();
}

void CURLHandle::sendRequest(const char* pHttpURL, const char* pPostData, EmHttpType eHttpType, bool bNeedCallBack)
{
	CURL* pURL = NULL;
	// 如果没有空余的URL了
	if (mvecURL.size() <= 0)
	{
		// 直接分配一个
		pURL = initEasyURL();
	}
	else
	{
		// 取空闲列表的最后一个
		pURL = mvecURL[mvecURL.size() - 1];
		mvecURL.pop_back();
	}

	if (NULL == pURL)
	{
		return;
	}

	CURLSession* pURLSession = NULL;
	// 从session空闲列表里找
	if (mvecURLSession.size() <= 0)
	{
		pURLSession = new CURLSession;
	}
	else
	{
		// 取空闲列表的最后一个
		pURLSession = mvecURLSession[mvecURLSession.size() - 1];
		mvecURLSession.pop_back();
	}

	if (NULL == pURLSession)
	{
		return;
	}

	// SSL(https)的方式，忽略了证书，选择不安全的方式，
	if (eHttpType & emHttpTypeSSL)
	{
		//忽略证书检查
		curl_easy_setopt(pURL, CURLOPT_SSL_VERIFYPEER, 0L);
	}

	// 连接超时
	curl_easy_setopt(pURL, CURLOPT_CONNECTTIMEOUT, 20L);
	curl_easy_setopt(pURL, CURLOPT_PRIVATE, pURLSession);
	curl_easy_setopt(pURL, CURLOPT_WRITEDATA, pURLSession);
	pURLSession->setNeedCallBack(bNeedCallBack);
	pURLSession->setHttpType(eHttpType);
	pURLSession->setHttpUrl(pHttpURL);
	// 如果是取文件的
	if (eHttpType & emHttpTypeFile && NULL != pPostData)
	{
		// pPostData是文件名
		char tFilePath[256] = { 0 };
		sprintf(tFilePath, "%s", pPostData);
		// 打开文件句柄
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

/// 设置HTTP Get方式的选项
void CURLHandle::setHttpGetOption(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession)
{
	curl_easy_setopt(pURL, CURLOPT_URL, pURLSession->getHttpUrl());
	curl_easy_setopt(pURL, CURLOPT_HEADER, 0);
	curl_easy_setopt(pURL, CURLOPT_HTTPGET, 1);
}

/// 设置HTTP Post方式的选项
void CURLHandle::setHttpPostRequest(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession)
{
	curl_easy_setopt(pURL, CURLOPT_URL, pURLSession->getHttpUrl());
	curl_easy_setopt(pURL, CURLOPT_HEADER, 1L);
	curl_easy_setopt(pURL, CURLOPT_POST, 1L);
	curl_easy_setopt(pURL, CURLOPT_POSTFIELDS, pURLSession);
}

/// 初始Easy URL
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

/// 检查一个Web请求是否完成
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
			// 已经传输完了，将文件句柄关闭
			if (pURLSession->getHttpType() & emHttpTypeFile)
			{
				fclose(pURLSession->getFile());
			}
		}
		// 回收到URL空闲列表
		mvecURL.push_back(pMsg->easy_handle);
		curl_multi_remove_handle(mMultiURL, pMsg->easy_handle);
		// 回收到URL Session空闲列表 
		mvecURLSession.push_back(pURLSession);
	}
}