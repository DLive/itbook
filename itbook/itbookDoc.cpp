// itbookDoc.cpp : CitbookDoc 类的实现
//

#include "stdafx.h"
#include "itbook.h"

#include "itbookDoc.h"
#include "CntrItem.h"
#include "KSFileDialog.h"
#include "itbookView.h"
#include "InputDB.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CitbookDoc

IMPLEMENT_DYNCREATE(CitbookDoc, CRichEditDoc)


BEGIN_MESSAGE_MAP(CitbookDoc, CRichEditDoc)
	// 启用默认的 OLE 容器实现
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
	//ON_COMMAND(ID_FILE_OPEN, &CitbookDoc::OnFileOpen)
END_MESSAGE_MAP()


// CitbookDoc 构造/析构

CitbookDoc::CitbookDoc()
{
	// TODO: 在此添加一次性构造代码
	m_isFromDb=false;
	m_isTemp=false;
	m_oldPath=_T("");
	m_nowPath=_T(" ");
	m_keyword=_T(" ");
	m_author=_T(" ");
	m_difficulty=_T("0");
	m_artitletype=_T("tit");
	m_isSavetoDb=TRUE;
	m_isSavetoLocal=FALSE;
	
}

CitbookDoc::~CitbookDoc()
{

}

BOOL CitbookDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

CRichEditCntrItem* CitbookDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CitbookCntrItem(preo, const_cast<CitbookDoc*>(this));
}




// CitbookDoc 序列化

void CitbookDoc::Serialize(CArchive& ar)
{
	
	if (ar.IsStoring())
	{

	}
	else
	{		
		// TODO: 在此添加加载代码	
	}

	// 调用基类 CRichEditDoc 将启用
	//  容器文档的 COleClientItem 对象的序列化。
	// TODO: 如果作为文本进行序列化，则设置 CRichEditDoc::m_bRTF = FALSE
	CRichEditDoc::Serialize(ar);		
}


// CitbookDoc 诊断

#ifdef _DEBUG
void CitbookDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CitbookDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

static DWORD CALLBACK MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{

	*pcb = cb;

	return 0;
}
BOOL CitbookDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	//EDITSTREAM es;
	//CString abc;
	//es.dwCookie = (DWORD) &abc;
	//es.pfnCallback = MyStreamOutCallback; 
	////->StreamOut(SF_RTF, es);
	//this->GetView()->SendMessage(EM_STREAMOUT,(WPARAM)SF_RTF,(LPARAM)&es);


	CKSFileDialog dlgFile(FALSE);
	dlgFile.SetDocumentPointer(this);
	dlgFile.SetAppPointer(AfxGetApp());
	dlgFile.SetMyParent(AfxGetMainWnd());
	return dlgFile.DoSave(lpszPathName,bReplace);

}



BOOL CitbookDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类

	 CRichEditDoc::OnSaveDocument(lpszPathName);
	 if(this->m_isSavetoDb)
	 {
		CInputDB insert(this);
		insert.getText(lpszPathName,NULL);
		this->m_oldPath=lpszPathName;
		this->m_isFromDb=true;
	 }
	return true;
}

void CitbookDoc::DeleteContents()
{
	// TODO: 在此添加专用代码和/或调用基类

	CRichEditDoc::DeleteContents();

}

BOOL CitbookDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO:  在此添加您专用的创建代码
	this->SetModifiedFlag(FALSE);
	CInputDB setinfo(this);
	setinfo.m_getinfo(lpszPathName,_T("tit"));
	
	return TRUE;
}
void CitbookDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类

	CRichEditDoc::OnCloseDocument();
	if(AfxGetApp()->m_pDocManager->GetOpenDocumentCount()<=0)
	{
		((CMainFrame*)AfxGetMainWnd())->m_propertyBar.m_pDoc=NULL;
	}
}
