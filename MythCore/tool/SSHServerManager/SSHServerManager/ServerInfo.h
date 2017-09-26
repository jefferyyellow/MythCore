#pragma once

class CServerInfo
{
public:
	CServerInfo(void);
	~CServerInfo(void);

public:
	CString		GetServerID(){return mServerID;}
	void		SetServerID(CString strServerID){mServerID = strServerID;}

	CString		GetInternetIP(){return mInernetIP;}
	void		SetInternetIP(CString strInternetIP){mInernetIP = strInternetIP;}

	CString		GetLocalIP(){return mLocalIP;}
	void		SetLocalIP(CString strLocalIP){mLocalIP = strLocalIP;}

	int			GetMonitorIndex(){return mMonitorIndex;}
	void		SetMonitorIndex(int nMonitorIndex){mMonitorIndex = nMonitorIndex;}

public:
	CString		mServerID;
	CString		mInernetIP;
	CString		mLocalIP;
	int			mMonitorIndex;
	TmServerState mServerState;
};
