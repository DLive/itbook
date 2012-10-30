#pragma once
#include   <iostream> 
#include   <string> 

using   namespace   std;

class Ccommon
{
public:
	Ccommon(void);
	~Ccommon(void);

	//access转义－单引号加倍
	void transFormSinQuote(CString &source);
	void getExeUrl(CString *url);
	// 删除目录里所有文件
	bool DeleteDirectory(const CString &pathName);


};

TCHAR *getAppDbPath();

LPSTR unicodeToAnsi(LPCWSTR str);
char *unicodeToUtf82(LPCWSTR str);  //str传进去后会被删
void getExeUrl(TCHAR *exefileurl);
char *unicodeToUtf8(const WCHAR *zWideFilename);
wstring ANSIToUnicode( const string& str );
wstring UTF8ToUnicode( const string& str );
string ansiToUtf8(string &str);