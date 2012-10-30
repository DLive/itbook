// txtDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"

#include "txtDoc.h"

#include "InputDB.h"
#include "MainFrm.h"
// CtxtDoc

IMPLEMENT_DYNCREATE(CtxtDoc, CDocument)

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
}

CtxtDoc::~CtxtDoc()
{
}


BEGIN_MESSAGE_MAP(CtxtDoc, CDocument)
END_MESSAGE_MAP()


// CtxtDoc 诊断

#ifdef _DEBUG
void CtxtDoc::AssertValid() const
{
	CDocument::AssertValid();
}


void CtxtDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

//#ifndef _WIN32_WCE
// CtxtDoc 序列化

void CtxtDoc::Serialize(CArchive& ar)
{
	CDocument::Serialize(ar);
	//CRichEditDoc::Serialize(ar);
	//((CEditView*)(m_viewList.GetHead()))->SerializeRaw(ar);
	//reinterpret_cast
	
}
//#endif


// CtxtDoc 命令
BOOL CtxtDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

BOOL CtxtDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//转换成unicode
	CFile bb;
	bb.Open(lpszPathName,CFile::modeReadWrite|CFile::modeRead);
    byte head[2];
    bb.Read(head,2);
	
	if(!((head[0]==0xff&&head[1]==0xfe)||(head[0]==0xfe&&head[1]==0xff)))
    {
		try
		{
			bb.SeekToBegin();
			//文件非unicode
			 ULONGLONG l=bb.GetLength();
			 char* pbuf;
			 pbuf=new char[l+1];
			 memset(pbuf, '\0', l+1);

			 int i=bb.Read(pbuf,l);
			 
			 //ansi to unicode
			 size_t len=strlen(pbuf);
			 int wlen=MultiByteToWideChar(CP_ACP,NULL,pbuf,-1,NULL,NULL);
			 
			 wchar_t *pwText=new wchar_t[wlen];
			 if(!pwText)
			 {
				delete []pwText;
			 }
			
			 MultiByteToWideChar(CP_ACP,0,pbuf,-1,pwText,wlen);
			 CString contenttext;
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
	
	bb.Close();
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	this->SetModifiedFlag(FALSE);
	CInputDB setinfo(this);
	setinfo.m_getinfo(lpszPathName,_T("txt"));

	return TRUE;
}

BOOL CtxtDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString content;
	((CEditView*)(m_viewList.GetHead()))->GetWindowTextW(content);
	BOOL lb=this->selfSaveFile(content,lpszPathName);
	//BOOL lb=CDocument::OnSaveDocument(lpszPathName);
	if(this->m_isSavetoDb)
	{
		CInputDB insert(this);
		insert.getText(lpszPathName,_T("txt"));
		this->m_oldPath=lpszPathName;
		this->m_isFromDb=true;
	}
	return lb;
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
	CDocument::OnCloseDocument();
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