// myClassTreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "myClassTreeDlg.h"

#include "propertyDlg.h"
#include "Conndb.h"

// CmyClassTreeDlg 对话框

IMPLEMENT_DYNAMIC(CmyClassTreeDlg, CDialog)

CmyClassTreeDlg::CmyClassTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmyClassTreeDlg::IDD, pParent)
{
	m_classIsInit=FALSE;

	m_parent=(CpropertyDlg*)pParent;
}

CmyClassTreeDlg::~CmyClassTreeDlg()
{
}

void CmyClassTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CmyClassTreeDlg, CDialog)
//	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
//	ON_WM_CAPTURECHANGED()
ON_NOTIFY(TVN_SELCHANGED, IDC_CLASSDLGTREE, &CmyClassTreeDlg::OnTvnSelchangedClassdlgtree)
ON_NOTIFY(NM_CLICK, IDC_CLASSDLGTREE, &CmyClassTreeDlg::OnNMClickClassdlgtree)
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CmyClassTreeDlg 消息处理程序

BOOL CmyClassTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	if(!m_classIsInit)
	{
		m_classInit();
		m_classIsInit=TRUE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BOOL CmyClassTreeDlg::m_classInit(void)
{
	CTreeCtrl* tc=(CTreeCtrl*)GetDlgItem(IDC_CLASSDLGTREE);
	//tc->Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
	//				 | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
	//				 | TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS | TVS_EDITLABELS,
	//				 CRect(10, 10, 400,550),m_pLocalResult,IDC_CALSSTREES);
	tc->DeleteAllItems();
	DWORD dwStyle = GetWindowLong(tc->m_hWnd,GWL_STYLE);
	dwStyle |= TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS;
    SetWindowLong(tc->m_hWnd,GWL_STYLE,dwStyle);
	
	//添加图示列表
	if(!Cil1.GetSafeHandle())
	{
		Cil1.Create(16,16,ILC_COLORDDB|ILC_MASK,0,2);
		Cil1.Add(AfxGetApp()->LoadIcon(IDI_FILES0));
		Cil1.Add(AfxGetApp()->LoadIcon(IDI_FILES1));
		tc->SetImageList(&Cil1,TVSIL_NORMAL);
		Cil1.SetBkColor(tc->GetBkColor());
	}
	//tc->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T("所有分类"),0,1,0,0,0,NULL,NULL);
	//tc->SetExtendedStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_DISABLEDRAGDROP);

	CString cmdsql;
	cmdsql=_T("SELECT * from class order by lv ASC,parentid ASC,id ASC");
	CConndb getclass;
	getclass.search(cmdsql);
	HTREEITEM hItem,hSubItem;//树句柄
	if(!getclass.m_query->eof())
	{
		CString classname;
		int pid;
		int curlv=1;
		int curparentid=0;
		_variant_t var;
		DWORD cid;
		int i=0;
		while(!getclass.m_query->eof())
		{
			var=getclass.m_query->getIntField(_T("id"));
			if(var.vt!=VT_NULL)
				cid=var.ulVal;
			
			var=getclass.m_query->getStringField(_T("classname"));
			if(var.vt!=VT_NULL)
				classname=(LPCSTR)_bstr_t(var);
			var=getclass.m_query->getIntField(_T("parentid"));
			if(var.vt!=VT_NULL)
				pid=var.intVal;
			var=getclass.m_query->getIntField(_T("lv"));
			if(var.vt!=VT_NULL)
				curlv=var.intVal;
			
			if(pid!=0)
			{
				curlv--;//得到该类的父节点深度
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
			getclass.m_query->nextRow();
		}

	}
	
	return 0;
}
HTREEITEM CmyClassTreeDlg::FindItem(CTreeCtrl& tree, HTREEITEM item, DWORD strText,int* findLevel,int* curLevel)
{
	HTREEITEM hFind;	
	if(item == NULL)
		return NULL;
	
	while(item != NULL)
	{
		if(*findLevel==*curLevel)
		{
			//只有在到达指定深度时,才比较
			DWORD cc=tree.GetItemData(item);
			if(cc == strText)
				return item;
		}
		
		if(tree.ItemHasChildren(item))
		{
			(*curLevel)++;//深度加一
			item = tree.GetChildItem(item);
			hFind = FindItem(tree, item, strText,findLevel,curLevel);		
			if(hFind)
			{
				return hFind;		
			}
			else
			{
				item= tree.GetNextSiblingItem(tree.GetParentItem(item));
				(*curLevel)--;//深度减一
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

void CmyClassTreeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//CDialog::OnActivate(nState, pWndOther, bMinimized);
	if (WA_INACTIVE == nState)
	{
		this->ShowWindow(SW_HIDE);
	}
	// TODO: 在此处添加消息处理程序代码
}


void CmyClassTreeDlg::OnTvnSelchangedClassdlgtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//pNMTreeView->itemNew.hItem;
	//CTreeCtrl* tc=(CTreeCtrl*)GetDlgItem(IDC_CLASSDLGTREE);
	//CpropertyDlg* pa=m_parent;
	//
	//pa->ChangeClassName(tc->GetItemData(pNMTreeView->itemNew.hItem),tc->GetItemText(pNMTreeView->itemNew.hItem));
	//this->ShowWindow(SW_HIDE);

	*pResult = 0;
}



void CmyClassTreeDlg::OnNMClickClassdlgtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	
	CTreeCtrl* pTb=(CTreeCtrl*)GetDlgItem(IDC_CLASSDLGTREE);

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
		
		CpropertyDlg* pa=m_parent;
	
		pa->ChangeClassName(pTb->GetItemData(hItem),pTb->GetItemText(hItem));
		this->ShowWindow(SW_HIDE);
			
	}

	*pResult = 0;
}

void CmyClassTreeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow)
	{
		if(!m_classIsInit)
		{
			m_classInit();
			m_classIsInit=TRUE;
		}
	}
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}
