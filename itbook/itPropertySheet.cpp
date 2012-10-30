// itPropertySheet.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "itPropertySheet.h"



// CitPropertySheet

IMPLEMENT_DYNAMIC(CitPropertySheet, CPropertySheet)

CitPropertySheet::CitPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_dfCaption=new TCHAR[256];
	m_dfListFont.CreatePointFont(30,_T("宋体"));
}

CitPropertySheet::CitPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_dfCaption=new TCHAR[256];
	m_dfListFont.CreatePointFont(30,_T("宋体"));
}

CitPropertySheet::~CitPropertySheet()
{
	delete [] m_dfCaption;
}


BEGIN_MESSAGE_MAP(CitPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CitPropertySheet 消息处理程序

BOOL CitPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  在此添加您的专用代码

	CRect rect, rectPage, rectTab;
	GetPage(0)->GetWindowRect(&rectPage);

	GetWindowRect(&rect);
	rect.right+=150;
	
	int nWidth=rectPage.Width();
	rectPage.right=rect.right-20;
	rectPage.left=rect.right-nWidth;
	ScreenToClient(&rectPage);
	m_rectPage=rectPage;
	GetPage(0)->MoveWindow(&rectPage);

	//隐藏属性页原来的TabControl
	CTabCtrl* pTab=GetTabControl();
	pTab->GetWindowRect(&rectTab);
	ScreenToClient(&rectTab);
	if(!pTab->ShowWindow(SW_HIDE))
		return FALSE;

	//创建列表控件
	if(!m_wndList.Create(WS_CHILD | WS_VISIBLE |  LVS_REPORT | LVS_NOCOLUMNHEADER , CRect(10 ,rectTab.top,150,rectPage.bottom ),this,35000))
		return FALSE;
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	listinit();

	//扩大行高度
	CFont font;
	font.CreatePointFont(240,_T("宋体"));
	m_wndList.SetFont(&font);
	
	CString strCaption;
	GetPage(0)->GetWindowText(strCaption);
	_tcscpy(m_dfCaption, strCaption.GetBuffer(strCaption.GetLength()));	

	return bResult;
}
void CitPropertySheet::listinit()
{
	LVITEM lvi;
	::ZeroMemory(&lvi,sizeof(lvi));

	CHeaderCtrl* pHeader=m_wndList.GetHeaderCtrl();
	pHeader->ShowWindow(SW_HIDE);

	CRect rectList;
	m_wndList.GetWindowRect(&rectList);
	ScreenToClient(&rectList);
	//添加列表头，前面已经隐藏
	m_wndList.InsertColumn(0,_T("设置"),LVCFMT_CENTER,rectList.Width(),0);
	CString str;
	CTabCtrl* pTab=GetTabControl();
	TCITEM tci;
	::ZeroMemory(&tci,sizeof(tci));
	tci.mask=TCIF_TEXT;
	tci.cchTextMax=256;
	TCHAR szBuf[256]={0};
	tci.pszText=szBuf;
	for(int idxPge=0;idxPge<GetPageCount();idxPge++)
	{
		if(pTab->GetItem(idxPge,&tci))
		{
			lvi.iItem=idxPge;
			lvi.iSubItem=0;
			//lvi.iImage=idxPge;
			lvi.mask=LVIF_TEXT; //LVIF_IMAGE;
			lvi.pszText=tci.pszText;
			m_wndList.InsertItem(&lvi);
		}
	}
}