// DLupClass.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "DLupClass.h"
#include "DLsyn.h"
#include "Conndb.h" 
#include "Ccommon.h"
#include "json/json.h"

#include "Conndb.h"
// CDLupClass 对话框

IMPLEMENT_DYNAMIC(CDLupClass, CDialog)

CDLupClass::CDLupClass(CWnd* pParent /*=NULL*/)
	: CDialog(CDLupClass::IDD, pParent)
{
	m_selIndex=-1;
}

CDLupClass::~CDLupClass()
{
}

void CDLupClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLupClass, CDialog)
	//ON_NOTIFY(NM_CLICK, IDC_CLASSLIST, &CDLupClass::OnNMClickClasslist)
	//ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CLASSLIST, &CDLupClass::OnLvnItemActivateClasslist)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_CLASSLIST, &CDLupClass::OnNMCustomdrawClasslist)
	ON_CBN_SELCHANGE(IDC_DCLASSLIST, &CDLupClass::OnCbnSelchangeDclasslist)
END_MESSAGE_MAP()


// CDLupClass 消息处理程序

BOOL CDLupClass::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_classList= new DLlistdrop();
	//m_classList->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_EDITLABELS|LVS_OWNERDRAWFIXED,CRect(10,10,400,200),this, IDC_CLASSUPDATELIST);
	m_classList=(CListCtrl*)GetDlgItem(IDC_CLASSLIST);
	m_classList->SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);//LVS_EX_CHECKBOXES
	m_classList->InsertColumn(0,_T("分类名"),LVCFMT_LEFT,250,0);
	m_classList->InsertColumn(1,_T("远程分类名"),LVCFMT_LEFT,100,0);
	m_classList->InsertColumn(2,_T("编号"),LVCFMT_LEFT,40,0);
	m_classList->InsertColumn(3,_T("远程编号"),LVCFMT_LEFT,80,0);
	OutputDebugString(_T("fsadfasdfasdf"));
	//m_pSelClass =new CComboBox();
	m_pSelClass=(CComboBox*)GetDlgItem(IDC_DCLASSLIST);
	//m_pSelClass->Create(WS_CHILD |WS_VISIBLE| WS_VSCROLL |CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,CRect(0,0,0,0),,IDC_DISCLASSSELECT);//WS_VISIBLE
	//m_pSelClass->SetParent(CWnd::FromHandle(classlist->m_hWnd));
	getSerClass();
	getLocClass();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BOOL CDLupClass::getLocClass(){
	CConndb db;
	CString sql=_T("select * from class ");
	db.search(sql);

	if(db.m_query->eof())
	{
		return false;
	}
	int i=0;
	CString id,rid,name;
	_variant_t var;
	//CListCtrl *classlist=(CListCtrl*)GetDlgItem(IDC_CLASSLIST);
	while(!db.m_query->eof())
	{
		var=db.m_query->getStringField(_T("id"));
		if(var.vt!=VT_NULL)
			id=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("distancecid"));
		if(var.vt!=VT_NULL)
			rid=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("classname"));
		if(var.vt!=VT_NULL)
			name=(LPCSTR)_bstr_t(var);
		m_classList->InsertItem(i,_T(""));
		m_classList->SetItemText(i,0,name);
		m_classList->SetItemText(i,1,_T(""));
		m_classList->SetItemText(i,2,id);
		m_classList->SetItemText(i,3,rid);

		for(int jj=0;jj<m_pSelClass->GetCount();jj++){
			if(m_pSelClass->GetItemData(jj)==_ttoi(rid)){
				CString strText;
				m_pSelClass->GetLBText(jj,strText);
				m_classList->SetItemText(i,1,strText);
				break;
			}
		}
		i++;
		db.m_query->nextRow();
	}
	return TRUE;
}
BOOL CDLupClass::getSerClass()
{
	CDLsyn getSer;
	if(!getSer.openSession(_T("dlive.sinaapp.com"),_T("test.php")))
	{
		AfxMessageBox(_T("链接服务器出错"));
		return FALSE;
	}
	Json::Value *val=new Json::Value();
	
	getSer.getClass((void *)val);
	for(int i=0;i<val->size();i++){
		string abc = (*val)[i]["name"].asString(); 
		wstring test=UTF8ToUnicode(abc);
		int index=m_pSelClass->AddString(test.c_str());
		(*val)[i]["term_id"].asString();
		m_pSelClass->SetItemData(index,atoi((*val)[i]["term_id"].asString().c_str()));
		m_pSelClass->SetCurSel(0);
	}
	return TRUE;
}


void CDLupClass::OnCbnSelchangeDclasslist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_pSelClass->GetLBText(m_pSelClass->GetCurSel(),str);
	int rid=m_pSelClass->GetItemData(m_pSelClass->GetCurSel());
	CString idStr;
	idStr.Format(_T("%d"),rid);
	CConndb db;
	CString tsql=_T("update class set distancecid=%s where id=%s");
	CString sql;
	CString lid;
	POSITION ps = m_classList->GetFirstSelectedItemPosition();
	while(ps){
		int nIdx = m_classList->GetNextSelectedItem(ps);
		if(nIdx >= 0 )
		{
			lid=m_classList->GetItemText(nIdx,2);
			sql.Format(tsql,idStr,lid);
			db.excuteSql(sql);
			m_classList->SetItemText(nIdx,1,str);
			m_classList->SetItemText(nIdx,3,idStr);
			//str = m_classList->GetItemText(nIdx,0);//这里获取的是你选中的行第一个格子里的数据
		}
	}
}
