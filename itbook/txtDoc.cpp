// txtDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "CntrItem.h" //

#include "txtDoc.h"

#include "InputDB.h"
#include "MainFrm.h"
// CtxtDoc
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CtxtDoc, CRichEditDoc)

CtxtDoc::CtxtDoc()
{
	m_isFromDb=false;
	m_isTemp=false;
	m_oldPath=_T("");
	m_nowPath=_T(" ");

	m_keyword=_T(" ");
	m_author=_T(" ");
	m_difficulty=_T("0");
	m_artitletype=_T("txt");
	m_infoIsFi=FALSE;
	m_isSavetoDb=TRUE;
	m_isANSI=FALSE;
	m_isSavetoLocal=FALSE;
}

CtxtDoc::~CtxtDoc()
{
}


BEGIN_MESSAGE_MAP(CtxtDoc, CRichEditDoc)
END_MESSAGE_MAP()


// CtxtDoc 诊断

#ifdef _DEBUG
void CtxtDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}


void CtxtDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}

#endif //_DEBUG

//#ifndef _WIN32_WCE
// CtxtDoc 序列化

void CtxtDoc::Serialize(CArchive& ar)
{
	
	//CDocument::Serialize(ar);
	//CRichEditDoc::Serialize(ar);
	//((CEditView*)(m_viewList.GetHead()))->SerializeRaw(ar);
	//reinterpret_cast
	//this->GetView()->SetWindowTextW(ar.GetFile()->Read(
	//m_bRTF=SF_RTF;
	CRichEditDoc::Serialize(ar);
}
//#endif
CRichEditCntrItem* CtxtDoc::CreateClientItem(REOBJECT* preo) const
{
	return NULL;
}

// CtxtDoc 命令
BOOL CtxtDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}



BOOL CtxtDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	
	if(lpszPathName==NULL)
		return TRUE;
	//转换成unicode
	CFile bb;
	bb.Open(lpszPathName,CFile::modeReadWrite|CFile::modeRead);
    byte head[2];
    bb.Read(head,2);
	CString contenttext;
	if(!((head[0]==0xff&&head[1]==0xfe)||(head[0]==0xfe&&head[1]==0xff)))
    {
		try
		{
			bb.SeekToBegin();
			//文件非unicode
			 ULONGLONG lg=bb.GetLength();
			 char* pbuf;
			 pbuf=new char[lg+1];
			 memset(pbuf, '\0', lg+1);

			 int i=bb.Read(pbuf,lg);
			 
			 //ansi to unicode
			 size_t len=strlen(pbuf);
			 int wlen=MultiByteToWideChar(CP_ACP,NULL,pbuf,-1,NULL,NULL);
			 
			 wchar_t *pwText=new wchar_t[wlen];
			 if(!pwText)
			 {
				delete []pwText;
			 }
			 MultiByteToWideChar(CP_ACP,0,pbuf,-1,pwText,wlen);
			 
			 contenttext.Format(_T("%s"),pwText);
			 bb.SetLength(0);
			 DWORD h=0xfeff;
			 //bb.Write(_T("fffe"),2);
			 bb.Write(&h,2);
			 bb.Write(contenttext,wlen*2);
			 delete []pwText;
			 delete []pbuf;
			 this->m_isANSI=TRUE;
		    
		}
		catch (CFileException* e)
		{ 
			TCHAR szCause[255];
			e->GetErrorMessage(szCause, 255);
			AfxMessageBox(szCause);
			e->ReportError ();
			e->Delete ();
			return 0;
		}
	}
	else
	{
		//bb.SeekToBegin();
		long len=bb.GetLength();
		wchar_t *pwText=new wchar_t[len+1];
		memset(pwText,'\0',len+1);
		bb.Read(pwText,len);
		contenttext.Format(_T("%s"),pwText);

	}
	//contenttext.ReleaseBuffer();
	bb.Close();
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	this->GetView()->DeleteContents();
	this->GetView()->SetWindowTextW(contenttext.GetBuffer());
	contenttext.ReleaseBuffer();
	//this->GetView()->SetWindowTextW(contenttext.GetBuffer());
	//contenttext.ReleaseBuffer();
	// TODO:  在此添加您专用的创建代码
	this->SetModifiedFlag(FALSE);
	CInputDB setinfo(this);
	setinfo.m_getinfo(lpszPathName,_T("txt"));

	return TRUE;
}
static DWORD CALLBACK StreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{

	LPCSTR *con=(LPCSTR *)dwCookie;
	int len=strlen(*con);
	if(len<=0){
		return -1;
	}
	if(len<cb){
		strcpy((char*)pbBuff,*con);
		*con+=len;
		*pcb =len;
	}
	else{
		memcpy((char*)pbBuff,*con,cb);
		*con+=cb;
		*pcb = cb;
	}
	return 0;
}
BOOL CtxtDoc::readContentFromStr(CString &str)
{
	//str=_T("asdfasdfasdf\r\n");
	BOOL bUsedDefaultChar;
	UINT alen=WideCharToMultiByte(CP_ACP,0,(LPCWSTR)str,-1,NULL,0,NULL,NULL);
	PSTR pMultiByteStr = new char[alen+1];
	memset( ( void* )pMultiByteStr,0, sizeof( char ) * (alen+1 ) );
	// convert to ANSI string
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)str,-1, pMultiByteStr,alen,NULL, &bUsedDefaultChar);
	LPCSTR tmp=pMultiByteStr;
	EDITSTREAM es;
	es.dwCookie = (DWORD) &pMultiByteStr;
	es.pfnCallback = StreamInCallback;
	//m_bRTF=SF_TEXT;
	this->GetView()->SendMessage(EM_STREAMIN,(WPARAM)m_bRTF,(LPARAM)&es);
	delete tmp;
	return TRUE;
}

static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	size_t len=strlen((char *)pbBuff);
	int wlen=MultiByteToWideChar(CP_ACP,NULL,(LPCSTR)pbBuff,-1,NULL,NULL);

	wchar_t *pwText=new wchar_t[wlen];
	if(!pwText)
	{
		delete []pwText;
	}
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)pbBuff,-1,pwText,wlen);

	CString *con=(CString*)dwCookie;
	con->Append((LPCTSTR)pwText,cb);
	delete pwText;
	*pcb = cb;

	return 0;
}

BOOL CtxtDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{

	//CKSFileDialog dlgFile(FALSE);
	//dlgFile.SetDocumentPointer(this);
	//dlgFile.SetAppPointer(AfxGetApp());
	//dlgFile.SetMyParent(AfxGetMainWnd());
	//return dlgFile.DoSave(lpszPathName,bReplace);
	if(m_isSavetoLocal){
		CDocument::DoSave(lpszPathName,bReplace);
	}
	if(m_isSavetoDb){
		EDITSTREAM es;
		CString content;
		es.dwCookie = (DWORD) &content;
		es.pfnCallback = MyStreamOutCallback; 
		//->StreamOut(SF_RTF, es);
		
		this->GetView()->SendMessage(EM_STREAMOUT,(WPARAM)m_bRTF,(LPARAM)&es);
		//CString content;
		//((CEditView*)(m_viewList.GetHead()))->GetWindowTextW(content);
		this->SetModifiedFlag(FALSE);
		CInputDB insert(this);
		insert.getText(lpszPathName,content);
		this->m_oldPath=lpszPathName;
		this->m_isFromDb=true;
	}
	return TRUE;

	

}
BOOL CtxtDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CtxtDoc::SaveAsAnsi(LPCTSTR pathname)
{
	try
	{
		CFile file(pathname,CFile::modeReadWrite| CFile::modeRead); 
		UINT iFileLen = file.GetLength ();
		PSTR buffer = new char[iFileLen];
		memset(buffer, '\0', iFileLen);
		BOOL bUsedDefaultChar;
		file.Read (buffer, iFileLen);
		UINT alen=WideCharToMultiByte(CP_ACP,0,(LPCWSTR)buffer,-1,NULL,0,NULL,NULL);
		PSTR pMultiByteStr = new char[alen+1];
		memset( ( void* )pMultiByteStr, 0, sizeof( char ) * (alen+1 ) );
		// convert to ANSI string

		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)(buffer), -1, pMultiByteStr,alen,NULL, &bUsedDefaultChar);
		file.SetLength(0);
		// create the ANSI file
		file.Write(pMultiByteStr, alen);
		file.Close();
		delete[] buffer; 
		delete[] pMultiByteStr;
	}
	catch (CFileException* e)
	{ 
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		e->ReportError ();
		e->Delete ();
	}
	return 0;
}

void CtxtDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类

	
	CString path=this->GetPathName();
	if(this->m_isANSI)
	{
		SaveAsAnsi(path);
	}
	CRichEditDoc::OnCloseDocument();
	//CDocument::OnCloseDocument();
	if(AfxGetApp()->m_pDocManager->GetOpenDocumentCount()<=0)
	{
		((CMainFrame*)AfxGetMainWnd())->m_propertyBar.m_pDoc=NULL;
	}
}
BOOL CtxtDoc::selfSaveFile(CString& content,CString path)
{
	CFile file;
	file.Open(path,CFile::modeCreate|CFile::modeWrite|CFile::modeRead|CFile::modeNoTruncate);
	WORD head=0xfeff;
	file.SetLength(0);
	file.Write(&head,2);
	file.Write(content,content.GetLength()*2);
	file.Close();
	return TRUE;
}