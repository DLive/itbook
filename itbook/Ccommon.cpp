#include "StdAfx.h"
#include "Ccommon.h"


//全局db url
TCHAR *appDbPath=NULL;

TCHAR *getAppDbPath(){
	if(appDbPath!=NULL)
		return appDbPath;
	appDbPath=new TCHAR[MAX_PATH];
	ZeroMemory(appDbPath,MAX_PATH);
	getExeUrl(appDbPath);
	appDbPath=wcscat(appDbPath,_T("data\\notebook.sqlite"));
	return appDbPath;
}

Ccommon::Ccommon(void)
{
}

Ccommon::~Ccommon(void)
{
}

void Ccommon::transFormSinQuote(CString &source)
{
	//source.Replace("'","''");
}

void Ccommon::getExeUrl(CString *url)
{
	TCHAR exefileurl[MAX_PATH];
	GetModuleFileName(NULL,exefileurl,MAX_PATH);
	(_tcsrchr(exefileurl, _T('\\')))[1] = 0;
	*url=exefileurl;
}

// 删除目录里所有文件
bool Ccommon::DeleteDirectory(const CString  &pathName)
{
	CFileFind fFind;
	CString dirName=pathName;	
	if(dirName.Right(1)!=_T("\\"))
		dirName+=_T("\\");
	CString tempName=dirName;
	dirName+=_T("*.*");
	CString deletefile;
	BOOL isFind=fFind.FindFile(dirName);
	int num=0;
	while(isFind)
	{
		num++;
		isFind=fFind.FindNextFileW();
		if(!fFind.IsDirectory() && !fFind.IsDots())
		{
			//如是文件
			tempName=fFind.GetFilePath();
			DeleteFile(tempName);
			num--;
		}
		else if(fFind.IsDirectory() && !fFind.IsDots())
			DeleteDirectory(fFind.GetFilePath());
	}
	fFind.Close();
	if(num==0)
		return true; //文件夹为空
	else
		return false;
}

//must delete ret value
LPSTR unicodeToAnsi(LPCWSTR str){
	BOOL bUsedDefaultChar;
	UINT alen=WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	LPSTR pMultiByteStr = new char[alen+1];
	memset( ( void* )pMultiByteStr, 0, sizeof( char ) * (alen+1 ) );
	// convert to ANSI string

	WideCharToMultiByte(CP_ACP, 0,(str), -1, pMultiByteStr,alen,NULL, &bUsedDefaultChar);
	// create the ANSI file
	return pMultiByteStr;
}
void getExeUrl(TCHAR *exefileurl)
{
	//TCHAR exefileurl[MAX_PATH];
	GetModuleFileName(NULL,exefileurl,MAX_PATH);
	(wcsrchr(exefileurl,_T('\\')))[1] = 0;
}

//you need free 了zFilename
char *unicodeToUtf8(const WCHAR *zWideFilename)   
{   
	int nByte;   
	char *zFilename;   
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);   
	zFilename = (char *)malloc(nByte);   
	if(zFilename == 0)   
	{   
		return 0;   
	}   
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte, 0, 0);   
	if( nByte == 0 )   
	{   
		free(zFilename);   
		zFilename = 0;   
	}   
	return zFilename;   
}
wstring ANSIToUnicode( const string& str )
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0 );  
	wchar_t *  pUnicode;  
	pUnicode = new  wchar_t[unicodeLen+1];  
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
	::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen );  
	wstring  rt;  
	rt = ( wchar_t* )pUnicode;
	delete  pUnicode; 

	return  rt;  
}
wstring UTF8ToUnicode( const string& str )
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0 );  
	wchar_t *  pUnicode;  
	pUnicode = new  wchar_t[unicodeLen+1];  
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
	::MultiByteToWideChar( CP_UTF8,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen );  
	wstring  rt;  
	rt = ( wchar_t* )pUnicode;
	delete  pUnicode; 

	return  rt;  
}
string UnicodeToUTF8( const wstring& str )
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,iTextLen,0,0);
	string ret;
	ret=pElementText;
	delete pElementText;
	return ret;
}
string ansiToUtf8(string &str){
	 return UnicodeToUTF8(ANSIToUnicode(str));
}