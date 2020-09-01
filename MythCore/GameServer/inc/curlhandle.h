#ifndef __CURLHANDLE_H__
#define __CURLHANDLE_H__
#include "curl.h"
#include <vector>
#define MAX_URL_LENGTH 256

enum EmHttpType
{ 
	emHttpTypePost	= 0,		// 默认为post请求
	emHttpTypeGet	= (1 << 0),	// 使用get方式
	emHttpTypeFile	= (1 << 1),	// 获取文件
	emHttpTypeSSL	= (1 << 2),	// SSL访问方式（https)
};
class CURLSession
{
public:
	CURLSession()
	{
		init();
	}
	~CURLSession()
	{

	}

	void			init()
	{
        mHttpType = emHttpTypeGet;
        mFile = NULL;
        mHttpUrl[0] = '\0';
        mNeedCallBack = false;
        mReturnData[0] = '\0';
        mReturnSize = 0;
	}
public:
	EmHttpType		getHttpType() const { return mHttpType; }
	void			setHttpType(EmHttpType nValue) { mHttpType = nValue; }

	FILE *			getFile() const { return mFile; }
	void			setFile(FILE * nValue) { mFile = nValue; }

	char*			getHttpUrl(){return mHttpUrl;}
	void			setHttpUrl(const char* pUrl)
	{
		if (NULL == pUrl)
		{
			return;
		}
		strncpy(mHttpUrl, pUrl, sizeof(mHttpUrl) - 1);
	}

	bool			getNeedCallBack() const { return mNeedCallBack; }
	void			setNeedCallBack(bool nValue) { mNeedCallBack = nValue; }


	char*			getReturnData(){return mReturnData;}
	void			setReturnData(const char* pReturnData)
	{
		if (NULL == mReturnData)
		{
			return;
		}
		strncpy(mReturnData, pReturnData, sizeof(mReturnData) - 1);
	}

	int				getReturnSize() const { return mReturnSize; }
	void			setReturnSize(int nValue) { mReturnSize = nValue; }

private:
	/// 指定Http的类型 default:emHttpTypeGet
	EmHttpType		mHttpType;
	/// 获取文件时文件的句柄
	FILE*			mFile;
	/// Web的页面地址
	char			mHttpUrl[MAX_URL_LENGTH];
	/// 是否需要回调
	bool			mNeedCallBack;
	/// 请求回来的数据
	char			mReturnData[RETURN_DATA_LENGTH];
	/// 请求回来的数据的大小
	int				mReturnSize;
};


typedef void URLCompleteCallBack(CURLSession* pURLSession);
class CURLHandle
{
public:
	typedef std::vector<CURL*>	VEC_URL;
	typedef std::vector<CURLSession*> VEC_URL_SESSION;

public:
	CURLHandle();
	~CURLHandle();
	void		init();
	bool		initCURL(int nMaxURLNum);
	// 清空对应的资源
	void		clear();
	/// 发送Web请求
	void		sendRequest(const char* pHttpURL, const char* pPostData, EmHttpType eHttpType, bool bNeedCallBack);
	/// 设置HTTP Get方式的选项
	void		setHttpGetOption(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession);
	/// 设置HTTP Post方式的选项
	void		setHttpPostRequest(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession);
	/// 初始Easy URL
	CURL*		initEasyURL();
	void		doing();
	/// 检查一个Web请求是否完成
	void		checkComplete();

public:
	void		setCompleteCallBack(URLCompleteCallBack* pCallBack)
	{
		mCompleteCallBack = pCallBack;
	}

private:
	/// multi URL
	CURLM*					mMultiURL;
	/// URL空闲列表
	VEC_URL					mvecURL;
	/// Session空闲列表
	VEC_URL_SESSION			mvecURLSession;
	/// 完成的回调
	URLCompleteCallBack*	mCompleteCallBack;
};

#endif