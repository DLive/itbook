#pragma once
#include "stdafx.h"

#include "afxinet.h"
#include <wininet.h>
#pragma comment(lib, "WinInet")

class CDLsyn
{
public:
	CDLsyn(void);
	~CDLsyn(void);
	
	
	CInternetSession m_session;
	CHttpConnection* m_pConnection;
	CString m_strHeaders;
	CHttpFile* m_pFile;

	BOOL openSession(LPCTSTR host,LPCTSTR object);
	bool upFileToSer(CStringArray &data);
	bool upFileToLoc(CStringArray &data);
	bool upClassFileToSer(CString str);
	BOOL upClassFileToLocal(CString cid);
	BOOL sendData(CStringA strFormData);
	BOOL getClass(LPVOID val);
	bool PostData(LPCTSTR host, LPCTSTR object, LPCTSTR postdata, LPCTSTR refererlink, int port);
};
