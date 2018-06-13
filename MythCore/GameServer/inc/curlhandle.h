#include "curl.h"
#include <vector>
#define MAX_URL_LENGTH 256
#define RETURN_DATA_LENGTH 1024

enum EmHttpType
{ 
	emHttpTypeGet	= (1 << 0),	// ʹ��get��ʽ
	emHttpTypeFile	= (1 << 1),	// ��ȡ�ļ�
	emHttpTypeSSL	= (1 << 2),	// SSL���ʷ�ʽ��https)
};
class CURLSession
{
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
		strncpy_s(mHttpUrl, pUrl, size_t(mHttpUrl) - 1);
	}

	bool			getNeedCallBack() const { return mNeedCallBack; }
	void			setNeedCallBack(bool nValue) { mNeedCallBack = nValue; }


	char*			getReturnData(){return mReturnData;}
	void			setReturnData(char* pReturnData)
	{
		if (NULL == mReturnData)
		{
			return;
		}
		strncpy_s(mReturnData, pReturnData, sizeof(mReturnData) - 1);
	}

	int				getReturnSize() const { return mReturnSize; }
	void			setReturnSize(int nValue) { mReturnSize = nValue; }

private:
	/// ָ��Http������
	EmHttpType		mHttpType;
	/// ��ȡ�ļ�ʱ�ļ��ľ��
	FILE*			mFile;
	/// Web��ҳ���ַ
	char			mHttpUrl[MAX_URL_LENGTH];
	/// �Ƿ���Ҫ�ص�
	bool			mNeedCallBack;
	/// �������������
	char			mReturnData[RETURN_DATA_LENGTH];
	/// ������������ݵĴ�С
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
	// ��ն�Ӧ����Դ
	void		clear();
	/// ����Web����
	void		sendRequest(const char* pHttpURL, const char* pPostData, EmHttpType eHttpType, bool bNeedCallBack);
	/// ����HTTP Get��ʽ��ѡ��
	void		setHttpGetOption(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession);
	/// ����HTTP Post��ʽ��ѡ��
	void		sendHttpPostRequest(const char* pHttpURL, const char* pPostData, CURL* pURL, CURLSession* pURLSession);
	/// ��ʼEasy URL
	CURL*		initEasyURL();
	void		doing();
	/// ���һ��Web�����Ƿ����
	void		checkComplete();

public:
	void		setCompleteCallBack(URLCompleteCallBack* pCallBack)
	{
		mCompleteCallBack = pCallBack;
	}

private:
	/// multi URL
	CURLM*					mMultiURL;
	/// URL�����б�
	VEC_URL					mvecURL;
	/// Session�����б�
	VEC_URL_SESSION			mvecURLSession;
	/// ��ɵĻص�
	URLCompleteCallBack*	mCompleteCallBack;
};