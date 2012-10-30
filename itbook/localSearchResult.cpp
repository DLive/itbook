// localSearchResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "itbook.h"
#include "localSearchResult.h"
#include "Conndb.h"
#include "itbookDoc.h"
//#include "itbookView.h"
#include "txtDoc.h"
#include "MainFrm.h"

#include "DLclassnamedlg.h" //�������Ի���
#include "CppSQLite3U.h"
#include "Ccommon.h"
#include "DLsyn.h"
// ClocalSearchResult �Ի���

IMPLEMENT_DYNAMIC(ClocalSearchResult, CDialogBar)

	ClocalSearchResult::ClocalSearchResult()
{
	m_pDialogBar=NULL;
	m_treeIsInit=FALSE;
	m_pageSize=30;
}

ClocalSearchResult::~ClocalSearchResult()
{
}

void ClocalSearchResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT, m_serResult);
	DDX_Control(pDX, IDC_DELLOCALFILE, m_deletLocalFile);
}


BEGIN_MESSAGE_MAP(ClocalSearchResult, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_RESULT, &ClocalSearchResult::OnNMDblclkResult)
	ON_BN_CLICKED(IDC_LOCALSER, &ClocalSearchResult::OnBnClickedLocalser)
	ON_BN_CLICKED(IDC_DELETE, &ClocalSearchResult::OnBnClickedDelete)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_RESULTCLASTREE, &ClocalSearchResult::OnTvnItemexpandingClasstree)
	ON_NOTIFY(NM_RCLICK,IDC_RESULTCLASTREE, &ClocalSearchResult::OnNMRClickClasstree)
	ON_NOTIFY(TVN_ENDLABELEDIT,IDC_RESULTCLASTREE,&ClocalSearchResult::OnTvnEndlabeleditClasstree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_RESULTCLASTREE, &ClocalSearchResult::OnTvnBeginlabeleditClasstree)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TCLASSADD, &ClocalSearchResult::OnTclassadd)
	ON_COMMAND(ID_TCLASSDEL, &ClocalSearchResult::OnTclassdel)
	ON_NOTIFY(NM_DBLCLK, IDC_RESULTCLASTREE, &ClocalSearchResult::OnNMDblclkResultclastree)

	ON_COMMAND(ID_ADDROOTCLASS, &ClocalSearchResult::OnAddrootclass)
	ON_BN_CLICKED(IDC_AFTERPAGE, &ClocalSearchResult::OnBnClickedAfterpage)
	ON_BN_CLICKED(IDC_PREPAGE, &ClocalSearchResult::OnBnClickedPrepage)
	ON_CBN_SELCHANGE(IDC_JMPPAGE, &ClocalSearchResult::OnCbnSelchangeJmppage)
	ON_COMMAND(ID_CLASSRENAME, &ClocalSearchResult::OnClassReName)

	ON_UPDATE_COMMAND_UI(IDC_PREPAGE,OnUpdateButtonPrepage)
	ON_UPDATE_COMMAND_UI(IDC_AFTERPAGE,OnUpdateButtonAfterpage)
	ON_WM_UPDATEUISTATE()
	ON_NOTIFY(NM_RCLICK, IDC_RESULT, &ClocalSearchResult::OnNMRClickResult)
	ON_COMMAND(ID_UPTOSERVER, &ClocalSearchResult::OnUptoserver)
	ON_COMMAND(ID_UPTOLOCAL, &ClocalSearchResult::OnUptolocal)
	ON_COMMAND(ID_UPCARTICLETODIS, &ClocalSearchResult::OnUpcarticletodis)
	ON_COMMAND(ID_UPCARTICLETOLOC, &ClocalSearchResult::OnUpcarticletoloc)
END_MESSAGE_MAP()

//BOOL ClocalSearchResult::Create()
//{
//	//CDialogBar::Create(ClocalSearchResult::IDD);
//
//	return this->ShowWindow(true);
//}
void ClocalSearchResult::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	if (m_pDialogBar!=NULL) {
		// modeless case -- do not call base class OnCancel
		m_pDialogBar->PostMessage(WM_GOODBYELOCAL, IDCANCEL);
	}
	//else {
	//	CDialogBar::OnCancel(); // modal case
	//}
}

void ClocalSearchResult::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���


}

LONG ClocalSearchResult::OnInitDialog(UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))

	{

		TRACE0("Warning: UpdateData failed during dialog init.\n");

	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_serResult.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_serResult.InsertColumn(0,_T("���±���"),LVCFMT_LEFT,250,0);
	m_serResult.InsertColumn(1,_T("����޸�ʱ��"),LVCFMT_LEFT,100,0);
	m_serResult.InsertColumn(2,_T("���"),LVCFMT_LEFT,40,0);
	m_serResult.InsertColumn(3,_T("����"),LVCFMT_LEFT,80,0);
	m_serResult.InsertColumn(4,_T("���·���"),LVCFMT_LEFT,80,0);
	m_serResult.InsertColumn(5,_T("�Ѷ�"),LVCFMT_LEFT,40,0);
	m_serResult.InsertColumn(6,_T("�ļ�·��"),LVCFMT_LEFT,80,0);
	m_serResult.InsertColumn(7,_T("����"),LVCFMT_LEFT,40,0);

	if(!m_treeIsInit)
	{
		ClassInit();
		m_treeIsInit=TRUE;
	}
	//setPageButtonInfo();

	return bRet;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ��������
bool ClocalSearchResult::searchArticle(CString &title)
{
	CString keyword;
	this->stringSplit(title,keyword,_T(" "));
	m_sqlCondition=keyword;  //�����ѯ����
	CString pagesql;
	pagesql.Format(_T("select count(*) from articles as a where %s "),keyword);

	//������תҳ
	CConndb db;
	int count=db.getCount(pagesql);
	int m_pageCount=count/m_pageSize;
	if(count%m_pageSize!=0)
		m_pageCount++;
	if(m_pageCount>0)
	{
		CComboBox* pcb=(CComboBox*)GetDlgItem(IDC_JMPPAGE);
		pcb->ResetContent();
		wchar_t num[10];
		for(int i=0;i<m_pageCount;i++)
		{
			pcb->InsertString(i,(LPCTSTR)_itow(i+1,num,10));
		}
		pcb->SetCurSel(0);
	}

	//��ѯ���
	m_pageIndex=0;
	CString sqlcmd;
	sqlcmd.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and (%s) ORDER BY a.id DESC LIMIT %d,%d")
		,keyword
		,m_pageSize*0
		,m_pageSize
		);
	//setPageButtonInfo();
	//sqlcmd.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and (%s)"),keyword);
	if(searchAndAdd(sqlcmd))
		return true;
	else
		return false;
}
//������ID����
bool ClocalSearchResult::searchClassId(int cid)
{
	CString cmdtext;
	/*if(cid!=0)
	{
	cmdtext.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and a.classid in(select id from class where id=%d or parentid=%d)"),cid,cid);
	}
	else
	{*/
	CString keyword;
	keyword.Format(_T("a.classid=%d"),cid);
	m_sqlCondition=keyword;  //�����ѯ����
	CString sqlcmd;

	CString pagesql;
	pagesql.Format(_T("select count(*) from articles as a where %s"),keyword);
	//������תҳ
	CConndb db;
	int count=db.getCount(pagesql);
	m_pageCount=count/m_pageSize;
	if(count%m_pageSize!=0)
		m_pageCount++;

	if(m_pageCount>0)
	{
		CComboBox* pcb=(CComboBox*)GetDlgItem(IDC_JMPPAGE);
		pcb->ResetContent();
		wchar_t num[10];
		for(int i=0;i<m_pageCount;i++)
		{
			pcb->InsertString(i,(LPCTSTR)_itow(i+1,num,10));
		}
		pcb->SetCurSel(0);
	}
	else
	{
		MessageBox(_T("û��������£�����������"));
		return false;
	}

	//��ѯ���
	m_pageIndex=0;
	CString cmdtext2;
	cmdtext2.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and (%s) ORDER BY a.id DESC LIMIT %d,%d")
		,keyword
		,m_pageSize*0
		,m_pageSize
		);
	//}
	//setPageButtonInfo();
	if(this->searchAndAdd(cmdtext2))
		return true;
	else
		return false;

}
bool ClocalSearchResult::searchAndAdd(CString sqlcmd)  //sqlcmdΪ��ѯ����
{
	//CConndb getarticle;
	//getarticle.search(sqlcmd);

	CConndb db;
	db.search(sqlcmd);

	CString titles,id,addtime,locaddress,classname,author,diffiNum,filetype;
	_variant_t var;
	try
	{
		if(!db.m_query->eof())
		{
			m_serResult.DeleteAllItems();
		}
		else
		{
			MessageBox(_T("û��������£�����������"));
			GetDlgItem(IDC_SEARCHKEY)->SetFocus();
			return false;
		}
		int i=0;
		while(!db.m_query->eof())
		{
			var=db.m_query->getStringField(_T("title"));
			if(var.vt!=VT_NULL)
				titles=(LPCSTR)_bstr_t(var);
			var=db.m_query->getIntField(_T("id"));
			if(var.vt!=VT_NULL)
				id=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("addtime"));
			if(var.vt!=VT_NULL)
				addtime=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("txtaddress"));
			if(var.vt!=VT_NULL)
				locaddress=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("classname"));
			if(var.vt!=VT_NULL)
				classname=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("author"));
			if(var.vt!=VT_NULL)
				author=(LPCSTR)_bstr_t(var);
			else
				author=_T("");
			var=db.m_query->getIntField(_T("difficulty"));
			if(var.vt!=VT_NULL)
				diffiNum=(LPCSTR)_bstr_t(var);
			else
				diffiNum=_T("");
			var=db.m_query->getStringField(_T("artitletype"));
			if(var.vt!=VT_NULL)
				filetype=(LPCSTR)_bstr_t(var);
			else
				filetype=_T("tit");
			//�����
			m_serResult.InsertItem(i,_T(""));
			//�������
			m_serResult.SetItemText(i,0,titles);
			m_serResult.SetItemText(i,1,addtime);
			m_serResult.SetItemText(i,2,id);
			m_serResult.SetItemText(i,3,author);
			m_serResult.SetItemText(i,4,classname);
			m_serResult.SetItemText(i,5,diffiNum);
			m_serResult.SetItemText(i,6,locaddress);
			m_serResult.SetItemText(i,7,filetype);
			db.m_query->nextRow();
		}
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return false;
	}
	m_serResult.SetFocus();
	return true;

}
void ClocalSearchResult::OnNMDblclkResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�õ���������
	CString pageno;
	for(int i=0;i<m_serResult.GetItemCount();i++)
	{
		if(m_serResult.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			pageno=m_serResult.GetItemText(i,2);
		}
	}
	viewArticle(pageno);
	*pResult = 0;
}
BOOL ClocalSearchResult::viewArticle(CString articleid)
{
	CString cmdsql;
	cmdsql.Format(_T("select a.title,a.content,a.classid,a.id,a.addtime,a.txtaddress,a.author,a.artitletype,a.keyword,a.difficulty,c.classname from articles AS a,class AS c where a.id=%s and a.classid=c.id"),articleid);
	//CConndb getarticle;
	//getarticle.getarticle(cmdsql);
	CConndb db;
	db.search(cmdsql);


	CString title,classid,id,addtime,locaddress,classname,author,keyword,difficulty,type;
	CString content;
	difficulty=_T("0");
	_variant_t var;
	try
	{
		if(db.m_query->eof())
		{
			MessageBox(_T("�������Ѳ����ڣ�����������"));
			return FALSE;
		}
		int i=0;
		while(!db.m_query->eof())
		{
			var=db.m_query->getStringField(_T("title"));
			if(var.vt!=VT_NULL)
				title=(LPCSTR)_bstr_t(var);
			var=db.m_query->getIntField(_T("id"));
			if(var.vt!=VT_NULL)
				id=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("addtime"));
			if(var.vt!=VT_NULL)
				addtime=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("content"));
			if(var.vt!=VT_NULL)
			{	
				content=(LPCSTR)_bstr_t(var);

			}
			var=db.m_query->getIntField(_T("classid"));
			if(var.vt!=VT_NULL)
				classid=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("txtaddress"));
			if(var.vt!=VT_NULL)
				locaddress=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("classname"));
			if(var.vt!=VT_NULL)
				classname=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("author"));
			if(var.vt!=VT_NULL)
				author=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("artitletype"));
			if(var.vt!=VT_NULL)
				type=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("difficulty"));
			if(var.vt!=VT_NULL)
				difficulty=(LPCSTR)_bstr_t(var);
			var=db.m_query->getStringField(_T("keyword"));
			if(var.vt!=VT_NULL)
				keyword=(LPCSTR)_bstr_t(var);

			db.m_query->nextRow();
		}
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
	//�ĳɴ����ݿ��ȡ  �ⲽ����
	bool istemp=false;//�Ƿ񻺴�	
	//if(GetFileAttributes(locaddress)==INVALID_FILE_ATTRIBUTES)
	//{
	//	//�ļ��Ƿ����
	//	CString filePath;
	//	getexeurl(&filePath);
	//	filePath+=_T("Temp\\");
	//	WIN32_FIND_DATA   wfd;
	//       HANDLE hFind = FindFirstFile(filePath, &wfd);
	//	//�ļ����Ƿ����
	//       if (!((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
	//    {
	//             SECURITY_ATTRIBUTES attrib;
	//		 attrib.bInheritHandle = FALSE;
	//		 attrib.lpSecurityDescriptor = NULL;
	//		 attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
	//		 //���涨������Կ���ʡ�ԡ� ֱ��return ::CreateDirectory( path, NULL); ����
	//		::CreateDirectory(filePath, &attrib);
	//       }
	//       FindClose(hFind);
	//	CFile aFile;
	//	if(type==_T("txt"))
	//	{
	//		locaddress.Format(_T("%s%s.txt"),filePath,title);
	//	}
	//	else
	//	{
	//		locaddress.Format(_T("%s%s.tit"),filePath,title);
	//	}
	//	if(!aFile.Open(locaddress,CFile::modeCreate|CFile::modeReadWrite,NULL))
	//	{
	//		AfxMessageBox(_T("�ļ���ʧ��"));
	//		return;
	//	}
	//	aFile.Write(content,content.GetLength());

	//	aFile.Close();
	//	istemp=true;
	//  
	// }

	//�½��ĵ�����
	CitbookApp *app=(CitbookApp*)AfxGetApp();
	//if(type==_T("txt"))
	//{
	CDocTemplate* pTemplate;
	// TODO: �ڴ���������������
	POSITION pz=app->m_pDocManager->GetFirstDocTemplatePosition();
	CString ext;
	while(pz)
	{
		pTemplate=app->m_pDocManager->GetNextDocTemplate(pz);
		pTemplate->GetDocString(ext,CDocTemplate::filterExt);
		if(ext==_T(".txt"))
			break;
	}
	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	//pTemplate->OpenDocumentFile(NULL);
	CtxtDoc* pdoc=(CtxtDoc*)pTemplate->OpenDocumentFile(NULL);
	if(type==_T("txt")){
		pdoc->m_bRTF=SF_TEXT;//ֻ�����ĵ��ڱ���ʱ��ѡ�������
	}
	else{
		pdoc->m_bRTF=SF_RTF;
	}

	pdoc->readContentFromStr(content);
	//CtxtDoc* pdoc=(CtxtDoc*)( app->OnFileNew());//locaddress
	pdoc->m_isFromDb=true;
	pdoc->m_aid=_ttoi(id);
	pdoc->m_classid=classid;
	pdoc->m_classname=classname;
	pdoc->m_title=title;
	pdoc->m_oldPath=locaddress;
	pdoc->m_nowPath=locaddress;
	pdoc->m_isTemp=istemp;
	pdoc->m_author=author;
	pdoc->m_artitletype=_T("txt");
	pdoc->m_keyword=keyword;
	pdoc->m_difficulty=difficulty;
	pdoc->SetModifiedFlag(FALSE);
	pdoc->SetTitle(title);
	//}
	//else
	//{
	// //CitbookDoc* pdoc=(CitbookDoc*)( app->OpenDocumentFile(locaddress) );
	// CtxtDoc* pdoc=(CtxtDoc*)( app->OpenDocumentFile(_T("")));//locaddress
	// pdoc->m_isFromDb=true;
	// pdoc->m_aid=_ttoi(id);
	// pdoc->m_classid=classid;
	// pdoc->m_classname=classname;
	// pdoc->m_title=title;
	// pdoc->m_oldPath=locaddress;
	// pdoc->m_nowPath=locaddress;
	// pdoc->m_isTemp=istemp;
	// pdoc->m_author=author;
	// pdoc->m_artitletype=_T("tit");
	// pdoc->m_keyword=keyword;
	// pdoc->m_difficulty=difficulty;

	// pdoc->SetModifiedFlag(FALSE);
	//}
	//ShowWindow(SW_MINIMIZE);
	//::SetWindowPos(AfxGetMainWnd()->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//this->SetWindowPos(AfxGetMainWnd(),0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return TRUE;
}
//�õ�����·��
void ClocalSearchResult::getexeurl(CString* purl)
{
	TCHAR exefileurl[MAX_PATH];
	GetModuleFileName(NULL,exefileurl,MAX_PATH);
	(_tcsrchr(exefileurl, _T('\\')))[1] = 0;
	*purl=exefileurl;
}
// �ַ����ָ�
void ClocalSearchResult::stringSplit(CString& source, CString& result, CString division)
{
	source.TrimLeft();
	source.TrimRight();
	result.Empty();
	int pos = 0;
	int pre_pos = 0;
	bool isfirst=true;
	CString temp;
	CString tempresult;
	CString word;
	while( -1 != pos )
	{
		pre_pos = pos;
		pos = source.Find(division,(pos+1));


		if(pos!=-1)
		{
			word=source.Mid(pre_pos,(pos-pre_pos));
			word.TrimLeft();
			word.TrimRight();

		}
		else
		{
			word=source.Mid(pre_pos);
			word.TrimLeft();
			word.TrimRight();
		}
		if(word.IsEmpty())
			continue;

		temp.Format(_T("(a.title like '%c%s%c')"),0x25,word,0x25);

		if(isfirst)
		{
			tempresult=temp;
			isfirst=false;
		}
		else
		{
			tempresult+=_T(" OR ");
			tempresult+=temp;
			//tempresult.Format(_T("%s OR %s"),tempresult,temp);
		}
	}
	result=tempresult;
}

void ClocalSearchResult::OnBnClickedLocalser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString title;
	GetDlgItem(IDC_SEARCHKEY)->GetWindowTextW(title);
	if(title.IsEmpty())
	{
		MessageBox(_T("���ⲻ��Ϊ��"));

	}
	else
	{		
		this->searchArticle(title);

	}
}

//δ����
void ClocalSearchResult::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(AfxMessageBox(_T("��ȷ���Ƿ�ɾ����"),MB_OKCANCEL)==IDCANCEL)
		return;
	//CConndb del;
	CString no;
	CString localpath;
	int count=0;
	bool isdellocal=false;
	if(m_deletLocalFile.GetCheck())
		isdellocal=true;
	CArray<int,int> arr;
	CString sqlcmd;
	CConndb db;
	for(int i=0;i<m_serResult.GetItemCount();i++)
	{
		if(m_serResult.GetCheck(i))
		{
			no=m_serResult.GetItemText(i,2);
			sqlcmd.Format(_T("delete from articles where id=%s"),no);

			db.excuteSql2(sqlcmd);
			arr.SetAtGrow(count,i);
			if(isdellocal)
			{
				localpath=m_serResult.GetItemText(i,3);
				DeleteFile(localpath);
			}
			count++;
		}

	}
	if(count==0)
		AfxMessageBox(_T("��δѡ������"));

	for(int i=0;i<arr.GetSize();i++)
	{
		m_serResult.DeleteItem(arr.GetAt(arr.GetSize()-i-1));		
	}

}


BOOL ClocalSearchResult::ClassInit(void)
{
	CTreeCtrl* tc=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	tc->DeleteAllItems();
	DWORD dwStyle = GetWindowLong(tc->m_hWnd,GWL_STYLE);
	dwStyle |= WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS;
	SetWindowLong(tc->m_hWnd,GWL_STYLE,dwStyle);

	//���ͼʾ�б�

	Cil1.Create(16,16,ILC_COLORDDB|ILC_MASK,0,2);
	Cil1.Add(AfxGetApp()->LoadIcon(IDI_FILES0));
	Cil1.Add(AfxGetApp()->LoadIcon(IDI_FILES1));
	tc->SetImageList(&Cil1,TVSIL_NORMAL);
	Cil1.SetBkColor(tc->GetBkColor());

	CConndb db;
	CString sqlcmd=_T("SELECT * from class order by lv ASC,parentid ASC,id ASC");
	db.search(sqlcmd);
	HTREEITEM hItem,hSubItem;//�����
	CString classname;
	int pid;
	int curlv=1;
	int curparentid=0;
	_variant_t var;
	DWORD cid;
	int i=0;
	try{
		while(!db.m_query->eof())
		{
			var=db.m_query->getIntField(_T("id"));//getclass.m_pRecordset->GetCollect("id");
			if(var.vt!=VT_NULL)
				cid=var.ulVal;
			LPTSTR test=(LPTSTR)db.m_query->getStringField(_T("classname"));
			var=db.m_query->getStringField(_T("classname"));//getclass.m_pRecordset->GetCollect("classname");
			if(var.vt!=VT_NULL)
				classname=(LPCSTR)_bstr_t(var);
			var=db.m_query->getIntField(_T("parentid"));//getclass.m_pRecordset->GetCollect("parentid");
			if(var.vt!=VT_NULL)
				pid=var.intVal;
			var=db.m_query->getIntField(_T("lv"));//getclass.m_pRecordset->GetCollect("lv");
			if(var.vt!=VT_NULL)
				curlv=var.intVal;
			if(pid!=0)
			{
				curlv--;//�õ�����ĸ��ڵ����
				if(pid!=curparentid)
				{
					int beginlv=1;
					hItem=FindItem(*tc,tc->GetRootItem(),pid,&curlv,&beginlv);
					curparentid=pid;
				}
				hSubItem = tc->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,classname,0,1,0,0,cid,hItem,TVI_LAST);
			}
			else
			{
				tc->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,classname,0,1,0,0,cid,TVI_ROOT,TVI_LAST);
			}


			i++;

			db.m_query->nextRow();
			//getclass.m_pRecordset->MoveNext();
		}
	}
	catch(CppSQLite3Exception& e)
	{
		printf("%s",e.errorMessage());
	}

	return 0;
}

void ClocalSearchResult::OnTvnItemexpandingClasstree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem=pNMTreeView->itemNew.hItem;

	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);

	if(pNMTreeView->action==TVE_EXPAND)
	{
		pTb->SetItemImage(hItem,1,1);
	}
	else
	{
		pTb->SetItemImage(hItem,0,1);
	}
	*pResult = 0;

}
//���һ�
void ClocalSearchResult::OnNMRClickClasstree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);

	CPoint   point; 
	TVHITTESTINFO   HitTestInfo; 
	GetCursorPos(&point); 
	pTb->ScreenToClient(&point); 
	HitTestInfo.pt   =   point; 
	//HTREEITEM   hItem   = pTb->HitTest(&HitTestInfo);

	UINT uFlags;
	HTREEITEM   hItem =pTb->HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{

		pTb->Select(hItem, TVGN_CARET);
		GetCursorPos(&point);
		this->SetForegroundWindow();

		CMenu m_menu;
		m_menu.LoadMenu(IDR_ICONMENU);

		CMenu *psub = (CMenu *)m_menu.GetSubMenu(1);  			
		DWORD dwID =psub->TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y ,this);
		m_menu.DestroyMenu();
	}


	*pResult = 0;
}

//��ӷ���
void ClocalSearchResult::OnTclassadd()
{

	// TODO: �ڴ���������������
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	HTREEITEM hItem=pTb->GetSelectedItem();
	int pid=pTb->GetItemData(hItem);

	CDLclassnamedlg getname;
	getname.DoModal();
	CString classname=getname.m_name;
	if(classname.IsEmpty())
	{
		MessageBox(_T("����������Ϊ��"));
		return;
	}
	CString sqlget;
	sqlget.Format(_T("select lv from class where id=%d"),pid);
	CConndb sqlite;
	sqlite.search(sqlget);

	int lv=0;
	if(!sqlite.m_query->eof())
	{
		_variant_t var;
		var=sqlite.m_query->getIntField(_T("lv"));
		if(var.vt!=VT_NULL)
			lv=var.intVal;		
	}
	//�����·���
	sqlget.Format(_T("insert into class (classname,parentid,lv) values ('%s',%d,%d)"),classname,pid,lv+1);
	sqlite.excuteSql(sqlget);

	//�õ��ղ����ID
	DWORD cid;
	sqlget.Format(_T("select max(id) as maxid from class"));
	sqlite.search(sqlget);
	if(!sqlite.m_query->eof())
	{
		_variant_t var;
		var=sqlite.m_query->getIntField(_T("maxid"));
		if(var.vt!=VT_NULL)
			cid=var.ulVal;
	}

	pTb->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,classname,0,1,0,0,cid,hItem,TVI_LAST);
	pTb->Expand(hItem,TVE_EXPAND);

	//֪ͨ��ʼ������������
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
	CpropertyDlg* pDlg=&pMain->m_propertyBar;
	pDlg->SendMessage(WM_MY_SETCLASSINIT,0,0);

}


//�༭��������
void ClocalSearchResult::OnTvnEndlabeleditClasstree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString nowname;
	nowname=pTVDispInfo->item.pszText;
	if(nowname==this->m_beginClassname)
		return;
	CString sqltext;
	sqltext.Format(_T("update class set classname='%s' where id=%d"),nowname,pTVDispInfo->item.lParam);
	CConndb update;
	if(update.excuteSql(sqltext))
	{
		CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
		TVITEMW tv;
		memset(&tv,0,sizeof(TVITEM));
		tv.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		tv.hItem=pTVDispInfo->item.hItem;
		pTb->GetItem(&tv);
		tv.pszText=(LPWSTR)(nowname.GetBuffer(nowname.GetLength()));
		nowname.ReleaseBuffer();
		pTb->SetItem(&tv);
	}

	//֪ͨ��ʼ������������
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
	CpropertyDlg* pDlg=&pMain->m_propertyBar;
	pDlg->SendMessage(WM_MY_SETCLASSINIT,0,0);
	*pResult = 0;
}


void ClocalSearchResult::OnTvnBeginlabeleditClasstree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_beginClassname=pTVDispInfo->item.pszText;
	*pResult = 0;
}
//ɾ������
void ClocalSearchResult::OnTclassdel()
{
	// TODO: �ڴ���������������
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	HTREEITEM hItem=pTb->GetSelectedItem();
	if(pTb->ItemHasChildren(hItem))
	{
		AfxMessageBox(_T("���ຬ������,����ɾ������"));
		return;
	}
	CString sqlcmd;
	CConndb del;
	sqlcmd.Format(_T("select count(id) as num from articles where classid=%d"),pTb->GetItemData(hItem));
	int count=del.getCount(sqlcmd);
	/*
	if(!del.m_query->eof());
	{
	_variant_t var;
	var=del.m_pRecordset->GetCollect("num");
	if(var.vt!=VT_NULL)
	count=var.ulVal;
	}*/
	CString info;
	info.Format(_T("�˴β�����ɾ��%dƪ����"),count);
	if(AfxMessageBox(info,MB_OKCANCEL)!=IDOK)
		return;
	if(count>0)
	{
		sqlcmd.Format(_T("delete from articles where classid=%d"),pTb->GetItemData(hItem));
		del.excuteSql(sqlcmd);
	}
	sqlcmd.Format(_T("delete from class where id=%d"),pTb->GetItemData(hItem));

	if(del.excuteSql(sqlcmd))
	{
		pTb->DeleteItem(hItem);
	}

	//֪ͨ��ʼ������������
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
	CpropertyDlg* pDlg=&pMain->m_propertyBar;
	pDlg->SendMessage(WM_MY_SETCLASSINIT,0,0);
}

void ClocalSearchResult::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->ShowWindow(FALSE);
	//CDialog::OnClose();
}



HTREEITEM ClocalSearchResult::FindItem(CTreeCtrl& tree, HTREEITEM item, DWORD strText,int* findLevel,int* curLevel)
{
	HTREEITEM hFind;	
	if(item == NULL)
		return NULL;

	while(item != NULL)
	{
		if(*findLevel==*curLevel)
		{
			//ֻ���ڵ���ָ�����ʱ,�űȽ�
			DWORD cc=tree.GetItemData(item);
			if(cc == strText)
				return item;
		}

		if(tree.ItemHasChildren(item))
		{
			(*curLevel)++;//��ȼ�һ
			item = tree.GetChildItem(item);
			hFind = FindItem(tree, item, strText,findLevel,curLevel);		
			if(hFind)
			{
				return hFind;		
			}
			else
			{
				item= tree.GetNextSiblingItem(tree.GetParentItem(item));
				(*curLevel)--;//��ȼ�һ
			}
		}
		else
		{
			item = tree.GetNextSiblingItem(item);
			if(item == NULL)
				return NULL;
		}		
	}

	return item;
}
void ClocalSearchResult::OnNMDblclkResultclastree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	int data;
	data=pTb->GetItemData(pTb->GetSelectedItem());
	searchClassId(data);
	*pResult = 0;
}

void ClocalSearchResult::OnAddrootclass()
{
	// TODO: �ڴ���������������
	CDLclassnamedlg getname;
	//getname.SetWindowTextW(_T("��ӷ���"));
	getname.DoModal();
	CString classname=getname.m_name;
	//getname.Create(CDLclassnamedlg::IDD);
	//getname.ShowWindow(TRUE);
	if(classname.IsEmpty())
	{
		MessageBox(_T("����������Ϊ��"));
		return;
	}
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	//CString classname=_T("�½�����,˫���޸�����");
	CConndb addclass;
	//�����·���
	CString sqlget;//=_T("insert into class (classname,parentid,lv) values ('�½�����,˫���޸�����',0,1)");
	sqlget.Format(_T("insert into class (classname,parentid,lv) values ('%s',0,1)"),classname);
	addclass.excuteSql(sqlget);

	//�õ��ղ����ID
	DWORD cid;
	sqlget.Format(_T("select max(id) as maxid from class"));
	addclass.search(sqlget);
	if(!addclass.m_query->eof())
	{
		_variant_t var;
		var=addclass.m_query->getIntField(_T("maxid"));
		if(var.vt!=VT_NULL)
			cid=var.ulVal;
	}
	pTb->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,classname,0,1,0,0,cid,TVI_ROOT,TVI_LAST);
	//pTb->Expand(hItem,TVE_EXPAND);
	//֪ͨ��ʼ������������
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
	CpropertyDlg* pDlg=&pMain->m_propertyBar;
	pDlg->SendMessage(WM_MY_SETCLASSINIT,0,0);
}

void ClocalSearchResult::OnBnClickedAfterpage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pageIndex++;
	if(m_pageIndex>=m_pageCount)
	{
		m_pageIndex--;
		return;
	}
	//setPageButtonInfo();

	CString sqlcmd;
	sqlcmd.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and %s ORDER BY a.id DESC LIMIT %d,%d")
		,m_sqlCondition
		,m_pageSize*m_pageIndex
		,m_pageSize
		);

	searchAndAdd(sqlcmd);
	((CComboBox*)GetDlgItem(IDC_JMPPAGE))->SetCurSel(m_pageIndex);
}

void ClocalSearchResult::OnBnClickedPrepage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pageIndex--;
	if(m_pageIndex<0)
	{
		m_pageIndex++;
		return;
	}
	//setPageButtonInfo();

	CString sqlcmd;
	sqlcmd.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and %s ORDER BY a.id DESC LIMIT %d,%d")
		,m_sqlCondition
		,m_pageSize*m_pageIndex
		,m_pageSize
		);

	searchAndAdd(sqlcmd);
	((CComboBox*)GetDlgItem(IDC_JMPPAGE))->SetCurSel(m_pageIndex);
}

void ClocalSearchResult::OnCbnSelchangeJmppage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pageIndex=((CComboBox*)GetDlgItem(IDC_JMPPAGE))->GetCurSel();
	if(m_pageIndex<0 || m_pageIndex>m_pageCount)
		return;
	//setPageButtonInfo();

	CString sqlcmd;
	sqlcmd.Format(_T("SELECT a.title,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype FROM articles AS a,class AS c where a.classid=c.id and %s ORDER BY a.id DESC LIMIT %d,%d")
		,m_sqlCondition
		,m_pageSize*m_pageIndex
		,m_pageSize
		);

	searchAndAdd(sqlcmd);
}

void ClocalSearchResult::setPageButtonInfo()
{
	////��ǰ
	//if(m_pageIndex<=1)
	//	((CButton*)GetDlgItem(IDC_PREPAGE))->EnableWindow(FALSE);
	//else
	//	((CButton*)GetDlgItem(IDC_PREPAGE))->EnableWindow(TRUE);

	////���
	//if(m_pageIndex>=m_pageCount)
	//	((CButton*)GetDlgItem(IDC_AFTERPAGE))->EnableWindow(FALSE);
	//else
	//	((CButton*)GetDlgItem(IDC_AFTERPAGE))->EnableWindow(TRUE);
}
void ClocalSearchResult::OnUpdateButtonPrepage(CCmdUI * pCmdUI){
	pCmdUI ->Enable(m_pageIndex<1?FALSE:TRUE);
}
void ClocalSearchResult::OnUpdateButtonAfterpage(CCmdUI * pCmdUI){
	pCmdUI ->Enable(m_pageIndex+1>=m_pageCount?FALSE:TRUE);
}


void ClocalSearchResult::OnClassReName()
{
	// TODO: �ڴ�������������

	CDLclassnamedlg getname;
	getname.DoModal();
	CString classname=getname.m_name;
	if(classname.IsEmpty())
	{
		MessageBox(_T("����������Ϊ��"));
		return;
	}

	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	HTREEITEM hItem=pTb->GetSelectedItem();
	int pid=pTb->GetItemData(hItem);


	CString sqltext;
	sqltext.Format(_T("update class set classname='%s' where id=%d"),classname,pid);
	CConndb update;
	if(update.excuteSql(sqltext))
	{
		pTb->SetItemText(hItem,classname);
	}

	//֪ͨ��ʼ������������
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
	CpropertyDlg* pDlg=&pMain->m_propertyBar;
	pDlg->SendMessage(WM_MY_SETCLASSINIT,0,0);
}

void ClocalSearchResult::OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/)
{
	// �ù���Ҫ��ʹ�� Windows 2000 ����߰汾��
	// ���� _WIN32_WINNT �� WINVER ���� >= 0x0500��
	// TODO: �ڴ˴������Ϣ����������
}

void ClocalSearchResult::OnNMRClickResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint   point; 
	GetCursorPos(&point);

	this->SetForegroundWindow();

	CMenu m_menu;
	m_menu.LoadMenu(IDR_ICONMENU);

	CMenu *psub = (CMenu *)m_menu.GetSubMenu(2);  			
	DWORD dwID =psub->TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y ,this);
	m_menu.DestroyMenu();
	*pResult = 0;
}

void ClocalSearchResult::OnUptoserver()
{
	// TODO: �ڴ���������������
	//CArray<int,int> arr;
	CString no;
	CStringArray data;
	int count=0;
	for(int i=0;i<m_serResult.GetItemCount();i++)
	{
		if(m_serResult.GetCheck(i))
		{
			data.Add(m_serResult.GetItemText(i,2));
			//no=m_serResult.GetItemText(i,2);
			//arr.SetAtGrow(count,i);
			count++;
		}
	}
	if(count==0){
		AfxMessageBox(_T("��δѡ������"));
		return;
	}
	CDLsyn sendData;
	if(!sendData.openSession(_T("dlive.sinaapp.com"),_T("test.php")))
	{
		AfxMessageBox(_T("���ӷ���������"));
		return;
	}

	//sendData.PostData(_T("dlive.sinaapp.com"),_T("test.php"),_T("class=haha&comtent=tttttt"),_T("http://dlive.sinaapp.com.com"),80);
	sendData.upFileToSer(data);
	//for(int i=0;i<arr.GetSize();i++)
	//{
	//	m_serResult.DeleteItem(arr.GetAt(arr.GetSize()-i-1));		
	//}
}

void ClocalSearchResult::OnUptolocal()
{
	// TODO: �ڴ���������������
	CString no;
	CStringArray data;
	int count=0;
	for(int i=0;i<m_serResult.GetItemCount();i++)
	{
		if(m_serResult.GetCheck(i))
		{
			data.Add(m_serResult.GetItemText(i,2));
			count++;
		}
	}
	if(count==0){
		AfxMessageBox(_T("��δѡ������"));
		return;
	}
	CDLsyn sendData;
	if(!sendData.openSession(_T("dlive.sinaapp.com"),_T("test.php")))
	{
		AfxMessageBox(_T("���ӷ���������"));
		return;
	}

	//sendData.PostData(_T("dlive.sinaapp.com"),_T("test.php"),_T("class=haha&comtent=tttttt"),_T("http://dlive.sinaapp.com.com"),80);
	sendData.upFileToLoc(data);

}


void ClocalSearchResult::OnUpcarticletodis()
{
	// TODO: �ڴ���������������
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	HTREEITEM hItem=pTb->GetSelectedItem();
	int id=pTb->GetItemData(hItem);

	CDLsyn sendData;
	if(!sendData.openSession(_T("dlive.sinaapp.com"),_T("test.php")))
	{
		AfxMessageBox(_T("���ӷ���������"));
		return;
	}
	CString idstr;
	idstr.Format(_T("%d"),id);
	//sendData.PostData(_T("dlive.sinaapp.com"),_T("test.php"),_T("class=haha&comtent=tttttt"),_T("http://dlive.sinaapp.com.com"),80);
	sendData.upClassFileToSer(idstr);
}


void ClocalSearchResult::OnUpcarticletoloc()
{
	// TODO: �ڴ���������������
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_RESULTCLASTREE);
	HTREEITEM hItem=pTb->GetSelectedItem();
	int id=pTb->GetItemData(hItem);

	CDLsyn sendData;
	if(!sendData.openSession(_T("dlive.sinaapp.com"),_T("test.php")))
	{
		AfxMessageBox(_T("���ӷ���������"));
		return;
	}
	CString idstr;
	idstr.Format(_T("%d"),id);
	//sendData.PostData(_T("dlive.sinaapp.com"),_T("test.php"),_T("class=haha&comtent=tttttt"),_T("http://dlive.sinaapp.com.com"),80);
	sendData.upClassFileToLocal(idstr);
}

BOOL ClocalSearchResult::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->wParam==VK_RETURN && pMsg->message==WM_KEYDOWN)
	{
		if(GetFocus()->GetDlgCtrlID()==IDC_SEARCHKEY){
			CString	str;   
			GetDlgItem(IDC_SEARCHKEY)->GetWindowText(str);
			this->searchArticle(str);
		}
		else if(GetFocus()->GetDlgCtrlID()==IDC_RESULT){
			POSITION pos=m_serResult.GetFirstSelectedItemPosition();
			while(pos){
				int iTem=m_serResult.GetNextSelectedItem(pos);
				CString pageno=m_serResult.GetItemText(iTem,2);
				viewArticle(pageno);
			}
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}


