// propertyDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "itbook.h"
#include "propertyDlg.h"
#include "itbookView.h"
#include "itbookDoc.h"

#include "txtDoc.h"
#include "txtView.h"

#include "conndb.h"
#include "MyPropertySheet.h"
#include "MainFrm.h"
// CpropertyDlg 对话框
#include "DLsyn.h"

#include "CppSQLite3U.h"

IMPLEMENT_DYNAMIC(CpropertyDlg, CDialogBar)

CpropertyDlg::CpropertyDlg()
{
	//m_pLocalResult=new ClocalSearchResult(this);
	m_classtreedlg=new CmyClassTreeDlg(this);
	m_classdlgIsCreate=FALSE;
	m_pDoc=NULL;
}

CpropertyDlg::~CpropertyDlg()
{
	delete m_pLocalResult;
	delete m_classtreedlg;
}

void CpropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CpropertyDlg, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
	ON_MESSAGE(WM_MY_CHANGEINFO, OnchangeInfo)
	ON_WM_CREATE()
	//ON_NOTIFY(NM_DBLCLK, IDC_CLASSTREE, &CpropertyDlg::OnNMDblclkClasstree)
	ON_CBN_SELCHANGE(IDC_NOWCLASSNAME, &CpropertyDlg::OnCbnSelchangeNowclass)
	ON_MESSAGE(WM_MY_SETCLASSINIT, setClassinit)
	ON_CBN_SETFOCUS(IDC_NOWCLASSNAME, &CpropertyDlg::OnCbnSetfocusNowclass)
	ON_EN_CHANGE(IDC_TITLE, &CpropertyDlg::OnEnChangeTitle)
	ON_EN_KILLFOCUS(IDC_AUTHOR, &CpropertyDlg::OnEnKillfocusAuthor)
	ON_EN_KILLFOCUS(IDC_KEYWORD, &CpropertyDlg::OnEnKillfocusKeyword)
	ON_CBN_SELCHANGE(IDC_DIFFICULTY, &CpropertyDlg::OnCbnSelchangeDifficulty)
	ON_BN_CLICKED(IDC_INSERTTODB, &CpropertyDlg::OnBnClickedInserttodb)
	ON_BN_CLICKED(IDC_BUTTON1, &CpropertyDlg::OnBnClickedButton1)

	ON_UPDATE_COMMAND_UI(IDC_BUTTON1,OnUpdateButtontest)
	ON_BN_CLICKED(IDC_SAVELOCAL, &CpropertyDlg::OnBnClickedSavelocal)
END_MESSAGE_MAP()


// CpropertyDlg 消息处理程序
LONG CpropertyDlg::OnInitDialog ( UINT wParam, LONG lParam)
{
 
    // <-- 加上以下几行. -->
    BOOL bRet = HandleInitDialog(wParam, lParam);
    if (!UpdateData(FALSE))
    {
        TRACE0("Warning: UpdateData failed during dialog init.\n");
    }
	classInfoInit();
     return bRet;
}
LRESULT  CpropertyDlg::OnchangeInfo(WPARAM wParam, LPARAM lParam)//如果lparam等于0 说明是tit文档 则为txt文档
{
	

	CString m_nowPath;//现在的路存
	int m_classid; //分类号
	CString m_title; //标题
	CString m_author;//作者
	int m_difficulty;//文章难度
	CString m_artitletype;//文本类型
	CString m_keyword;//关键字
	CString classname;
	BOOL savetodb;

	CComboBox* pCb=(CComboBox*)GetDlgItem(IDC_NOWCLASSNAME);

	if(lParam==0)
	{
		CitbookView* pView=(CitbookView*)wParam;
		CitbookDoc* pdoc=pView->GetDocument();
		if(m_pDoc==pdoc && pdoc->m_infoIsFi==TRUE)  //如果同一文档则返回
			return 0;
		m_pDoc=pdoc;
		m_classid=_ttoi(pdoc->m_classid);
		m_nowPath=pdoc->m_nowPath;
		classname=pdoc->m_classname;
		m_title=pdoc->m_title;
		if(m_title.IsEmpty())
			m_title=pdoc->GetTitle();
		m_author=pdoc->m_author;
		m_difficulty=_ttoi(pdoc->m_difficulty);
		m_artitletype=pdoc->m_artitletype;
		m_keyword=pdoc->m_keyword;
		savetodb=pdoc->m_isSavetoDb;
		m_pDoc=pdoc;

		if(m_classid<=0)
		{
			int defaultid=pCb->GetItemData(pCb->GetCurSel());
			pdoc->m_classid.Format(_T("%d"),defaultid);
			return 0;
		}

		if(!m_title.IsEmpty())
		{
			pdoc->m_infoIsFi=TRUE;
		}
		((CMainFrame*)AfxGetMainWnd())->m_editToolBar.GetDlgItem(ID_FONTSIZE)->EnableWindow(TRUE);
	}
	else
	{
		CtxtView* pView=(CtxtView*)wParam;
		CtxtDoc* pdoc=pView->GetDocument();
		if(m_pDoc==pdoc && pdoc->m_infoIsFi==TRUE)
			return 0;
		m_pDoc=pdoc;
		m_classid=_ttoi(pdoc->m_classid);
		m_nowPath=pdoc->m_nowPath;
		classname=pdoc->m_classname;
		m_title=pdoc->m_title;
		if(m_title.IsEmpty())
			m_title=pdoc->GetTitle();
		m_author=pdoc->m_author;
		m_difficulty=_ttoi(pdoc->m_difficulty);
		m_artitletype=pdoc->m_artitletype;
		m_keyword=pdoc->m_keyword;
		savetodb=pdoc->m_isSavetoDb;

		m_pDoc=pdoc;

		if(m_classid<=0)
		{
			int defaultid=pCb->GetItemData(pCb->GetCurSel());
			pdoc->m_classid.Format(_T("%d"),defaultid);
			return 0;
		}
		if(!m_title.IsEmpty())
		{
			pdoc->m_infoIsFi=TRUE;
		}
		//((CMainFrame*)AfxGetMainWnd())->m_editToolBar.GetDlgItem(ID_FONTSIZE)->EnableWindow(FALSE);

	}
	//
	

	if(m_pDoc==NULL)
	{
		return 0;
	}
	//如何设置路径
	CEdit* pe=(CEdit*)GetDlgItem(IDC_POSITON);
	pe->SetWindowTextW(m_nowPath);

	CButton* cb=(CButton*)GetDlgItem(IDC_INSERTTODB);
	cb->SetCheck(savetodb);

	//设置分类
	pCb->SetWindowTextW(classname);
	pCb->SetItemData(0,m_classid);
	//设置标题
	pe=(CEdit*)GetDlgItem(IDC_TITLE);
	pe->SetWindowTextW(m_title);
	
	//设置作者
	pe=(CEdit*)GetDlgItem(IDC_AUTHOR);
	pe->SetWindowTextW(m_author);
	//设置难度
	pCb=(CComboBox*)GetDlgItem(IDC_DIFFICULTY);
	pCb->SetCurSel(m_difficulty);
	//设置关键字
	pe=(CEdit*)GetDlgItem(IDC_KEYWORD);
	pe->SetWindowTextW(m_keyword);
	return 0;

}
void CpropertyDlg::classInfoInit()
{
	CComboBox* pCb=(CComboBox*)GetDlgItem(IDC_DIFFICULTY);
	CString num;
	for(int i=0;i<6;i++)
	{
		
		num.Format(_T("%d"),i);
		pCb->AddString(num);
		pCb->SetCurSel(0);
	}
	pCb=(CComboBox*)GetDlgItem(IDC_NOWCLASSNAME);
	pCb->AddString(_T("分类"));
	pCb->SetItemData(0,1);
	pCb->SetCurSel(0);
	
	
}
int CpropertyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	return 0;
}



void CpropertyDlg::OnCbnSelchangeNowclass()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
LRESULT CpropertyDlg::setClassinit(WPARAM wParam,LPARAM lParam)
{
	m_classtreedlg->m_classIsInit=FALSE;
	return 0;
}

//得到输入焦点
void CpropertyDlg::OnCbnSetfocusNowclass()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_classtreedlg->GetSafeHwnd())
	{
		CRect cbr;
		CRect tcr;
		GetDlgItem(IDC_NOWCLASSNAME)->GetWindowRect(cbr);		
		m_classtreedlg->GetWindowRect(tcr);
		cbr.top+=20;
		cbr.right=cbr.left + tcr.Width();
		cbr.bottom=cbr.top+tcr.Height();
		m_classtreedlg->MoveWindow(cbr);

		m_classtreedlg->ShowWindow(SW_SHOW);
	}
	else
	{
		CRect cbr;
		CRect tcr;
		CComboBox* cb=(CComboBox*)GetDlgItem(IDC_NOWCLASSNAME);
		cb->GetWindowRect(cbr);
		//ScreenToClient(cbr);
		
		m_classtreedlg->Create(IDD_CLASSTREEDLG,this);
		m_classtreedlg->GetWindowRect(tcr);
		//ScreenToClient(tcr);
		cbr.top+=20;
		cbr.right=cbr.left + tcr.Width();
		cbr.bottom=cbr.top+tcr.Height();
		m_classtreedlg->MoveWindow(cbr);
		m_classtreedlg->ShowWindow(SW_SHOW);
	}

}
 void CpropertyDlg::ChangeClassName(int cid,CString cname)
 {
    if(m_pDoc==NULL)
		return;
	CComboBox* pCb=(CComboBox*)this->GetDlgItem(IDC_NOWCLASSNAME);
	//CTreeCtrl* tc=(CTreeCtrl*)(FromHandle(::GetDlgItem(m_classtreedlg->m_hWnd,IDC_CLASSDLGTREE)));
	//pCb->SetItemData(0,tc->GetItemData(hitem));
	pCb->SetWindowTextW(cname);
	

	CDocTemplate* tp=m_pDoc->GetDocTemplate();
	CString type;
	tp->GetDocString(type,CDocTemplate::filterExt);
	if(type==_T(".tit"))
	{
		CitbookDoc* pdoc=(CitbookDoc*)m_pDoc;
		pdoc->m_classid.Format(_T("%d"),cid);
		pdoc->m_classname.Format(_T("%s"),cname);
	}
	else
	{
		CtxtDoc* pdoc=(CtxtDoc*)m_pDoc;
		pdoc->m_classid.Format(_T("%d"),cid);
		pdoc->m_classname.Format(_T("%s"),cname);
	}
	
 }
 void CpropertyDlg::OnEnChangeTitle()
 {
	 // TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	 // 发送该通知，除非重写 CDialogBar::OnInitDialog()
	 // 函数并调用 CRichEditCtrl().SetEventMask()，
	 // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	 // TODO:  在此添加控件通知处理程序代码
	 if(m_pDoc==NULL)
		return;
	CString title;
	GetDlgItem(IDC_TITLE)->GetWindowTextW(title);
	CDocTemplate* tp=m_pDoc->GetDocTemplate();
	CString type;
	tp->GetDocString(type,CDocTemplate::filterExt);
	if(type==_T(".tit"))
	{
		CitbookDoc* pdoc=(CitbookDoc*)m_pDoc;
		pdoc->m_title=title;
	}
	else
	{
		CtxtDoc* pdoc=(CtxtDoc*)m_pDoc;
		pdoc->m_title=title;
	}
	
 }

 void CpropertyDlg::OnEnKillfocusAuthor()
 {
	 // TODO: 在此添加控件通知处理程序代码
	if(m_pDoc==NULL)
		return;
	CString author;
	GetDlgItem(IDC_AUTHOR)->GetWindowTextW(author);
	CDocTemplate* tp=m_pDoc->GetDocTemplate();
	CString type;
	tp->GetDocString(type,CDocTemplate::filterExt);
	if(type==_T(".tit"))
	{
		CitbookDoc* pdoc=(CitbookDoc*)m_pDoc;
		pdoc->m_author=author;

	}
	else
	{
		CtxtDoc* pdoc=(CtxtDoc*)m_pDoc;
		pdoc->m_author=author;

	}
 }

 void CpropertyDlg::OnEnKillfocusKeyword()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(m_pDoc==NULL)
		return;
	CString text;
	GetDlgItem(IDC_KEYWORD)->GetWindowTextW(text);
	CDocTemplate* tp=m_pDoc->GetDocTemplate();
	CString type;
	tp->GetDocString(type,CDocTemplate::filterExt);
	if(type==_T(".tit"))
	{
		CitbookDoc* pdoc=(CitbookDoc*)m_pDoc;
		pdoc->m_keyword=text;

	}
	else
	{
		CtxtDoc* pdoc=(CtxtDoc*)m_pDoc;
		pdoc->m_keyword=text;

	}
 }

 void CpropertyDlg::OnCbnSelchangeDifficulty()
 {
	 // TODO: 在此添加控件通知处理程序代码
	  if(m_pDoc==NULL)
		return;
	CComboBox* cb=(CComboBox*)GetDlgItem(IDC_DIFFICULTY);
	CString num;
	cb->GetWindowTextW(num);
	CDocTemplate* tp=m_pDoc->GetDocTemplate();
	CString type;
	tp->GetDocString(type,CDocTemplate::filterExt);
	if(type==_T(".tit"))
	{
		CitbookDoc* pdoc=(CitbookDoc*)m_pDoc;
		pdoc->m_difficulty=num;

	}
	else
	{
		CtxtDoc* pdoc=(CtxtDoc*)m_pDoc;
		pdoc->m_difficulty=num;

	}
 }



 void CpropertyDlg::OnBnClickedInserttodb()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 BOOL ischeck;
	 if(BST_CHECKED==((CButton*)GetDlgItem(IDC_INSERTTODB))->GetCheck())
		ischeck=TRUE;
	 else
		ischeck=FALSE;
	 if(m_pDoc==NULL)
		 return;
	((CtxtDoc *)m_pDoc)->m_isSavetoLocal=ischeck;
	 //CDocTemplate* tp=m_pDoc->GetDocTemplate();
	 //CString type;
	 //tp->GetDocString(type,CDocTemplate::filterExt);
	 //if(type==_T(".tit"))
	 //{
		//CitbookDoc* pdoc=(CitbookDoc*)m_pDoc;
		//pdoc->m_isSavetoDb=ischeck;
	 //}
	 //else
	 //{
		//CtxtDoc* pdoc=(CtxtDoc*)m_pDoc;
		//pdoc->m_isSavetoDb=ischeck;

	 //}
 }

 void CpropertyDlg::OnBnClickedButton1()
 {
	 // TODO: 在此添加控件通知处理程序代码
	//CDLsyn sendData;
	//sendData.PostData(_T("dlive.sinaapp.com"),_T("test.php"),_T("class=haha&comtent=tttttt"),_T("http://dlive.sinaapp.com.com"),80);
	//CConndb db;
	//CString sql=_T("select a.title,a.content,a.classid,a.id,a.addtime,a.txtaddress,a.author,a.artitletype,a.keyword,a.difficulty from articles AS a");
	//db.search(sql);

	//CString titles,id,addtime,locaddress,classname,author,diffiNum,filetype,content,cid,keyword;
	//CString cmd;
	//_variant_t var;
	//try
	//{
	//	if(!db.m_pRecordset->BOF)
	//	{
	//		db.m_pRecordset->MoveFirst();
	//		//m_serResult.DeleteAllItems();
	//	}
	//	else
	//	{
	//		AfxMessageBox(_T("没有相关文章，请重新搜索"));
	//		return;
	//	}
	//	int i=0;
	//	CStringA sql;
	//	while(!db.m_pRecordset->adoEOF)
	//	{
	//		var=db.m_pRecordset->GetCollect("title");
	//		if(var.vt!=VT_NULL)
	//			titles=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("content");
	//		if(var.vt!=VT_NULL)
	//			content=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("keyword");
	//		if(var.vt!=VT_NULL)
	//			keyword=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("id");
	//		if(var.vt!=VT_NULL)
	//			id=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("classid");
	//		if(var.vt!=VT_NULL)
	//			cid=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("addtime");
	//		if(var.vt!=VT_NULL)
	//			addtime=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("txtaddress");
	//		if(var.vt!=VT_NULL)
	//			locaddress=(LPCSTR)_bstr_t(var);
	//		var=db.m_pRecordset->GetCollect("author");
	//		if(var.vt!=VT_NULL)
	//			author=(LPCSTR)_bstr_t(var);
	//		else
	//			author=_T("");
	//		var=db.m_pRecordset->GetCollect("difficulty");
	//		if(var.vt!=VT_NULL)
	//			diffiNum=(LPCSTR)_bstr_t(var);
	//		else
	//			diffiNum=_T("");
	//		var=db.m_pRecordset->GetCollect("artitletype");
	//		if(var.vt!=VT_NULL)
	//			filetype=(LPCSTR)_bstr_t(var);
	//		else
	//			filetype=_T("tit");
	//		
	//		cmd.Format(_T("insert into articles ('title','content','classid','addtime','txtaddress','artitletype','author','keyword','difficulty') values (?,?,?,?,?,?,?,?,?)"));
	//		//cmd.Format(_T("insert into articles ('title','content','classid','addtime','txtaddress','artitletype','author','keyword','difficulty') values (\"%s\",\"%s\",\"%s\",'%s','%s','%s','%s','%s','%s')"),
	//		//	titles,content,cid,addtime,locaddress,filetype,author,keyword,diffiNum
	//		//	);
	//		CppSQLite3Statement stmt = db.m_db->compileStatement(_T("insert into articles ('title','content','classid','addtime','txtaddress','artitletype','author','keyword','difficulty') values (?,?,?,?,?,?,?,?,?);")); 
	//		stmt.bind(1,titles);
	//		stmt.bind(2,content);
	//		stmt.bind(3,cid);
	//		stmt.bind(4,addtime);
	//		stmt.bind(5,locaddress);
	//		stmt.bind(6,filetype);
	//		stmt.bind(7,author);
	//		stmt.bind(7,keyword);
	//		stmt.bind(7,diffiNum);
	//		stmt.execDML();
	//		stmt.reset();
	//		//db.insert(cmd);
	//		db.m_pRecordset->MoveNext();
	//	}
	//	db.m_pRecordset.Release();
	//}
	//catch(_com_error *e)
	//{
	//	AfxMessageBox(e->ErrorMessage());
	//	return ;
	//}
 }

 void CpropertyDlg::OnUpdateButtontest(CCmdUI * pCmdUI){
	 pCmdUI ->Enable(TRUE);
 }
 void CpropertyDlg::OnBnClickedSavelocal()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 BOOL ischeck;
	 if(BST_CHECKED==((CButton*)GetDlgItem(IDC_SAVELOCAL))->GetCheck())
		 ischeck=TRUE;
	 else
		 ischeck=FALSE;
	 if(m_pDoc==NULL)
		 return;

	 
	 ((CtxtDoc *)m_pDoc)->m_isSavetoLocal=ischeck;
	 /*CDocTemplate* tp=m_pDoc->GetDocTemplate();
	 CString type;
	 tp->GetDocString(type,CDocTemplate::filterExt);*/
 }
