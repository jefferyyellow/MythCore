#ifndef __VERSION_H__
#define __VERSION_H__
class CVersion
{
public:
	CVersion()
	{
		mStartVersion = 0;
		mCurVersion = 0;
	}

	~CVersion(){}

public:
	void loadVersionXml(const char* pFilePath);

public:
	int getStartVersion() const { return mStartVersion; }
	void setStartVersion(int nValue) { mStartVersion = nValue; }

	int getCurVersion() const { return mCurVersion; }
	void setCurVersion(int nValue) { mCurVersion = nValue; }

private:
	int		mStartVersion;
	int		mCurVersion;
};
#endif