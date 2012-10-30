// DLlistdrop.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "DLlistdrop.h"


// DLlistdrop

IMPLEMENT_DYNAMIC(DLlistdrop, CListCtrl)

DLlistdrop::DLlistdrop()
{
	m_iItem=-1;
}

DLlistdrop::~DLlistdrop()
{
}


BEGIN_MESSAGE_MAP(DLlistdrop, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, &DLlistdrop::OnNMClick)
END_MESSAGE_MAP()



// DLlistdrop 消息处理程序




void DLlistdrop::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项

	if(m_iItem>=0){

		CRect rect;
		this->GetSubItemRect(m_iItem,1, LVIR_BOUNDS,rect);
		// Now scroll if we need to expose the column
		m_pSelClass->MoveWindow(rect,TRUE);
	}
	//CListCtrl::DrawItem(lpDrawItemStruct);
}


BOOL DLlistdrop::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL ret=CListCtrl::Create(dwStyle, rect, pParentWnd, nID);
		
	m_pSelClass =new CComboBox();
	m_pSelClass->Create(WS_CHILD |WS_VISIBLE| WS_VSCROLL |CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL,CRect(0,0,0,0),this,IDC_DISCLASSSELECT);//WS_VISIBLE
	//m_pSelClass->SetParent(CWnd::FromHandle(classlist->m_hWnd));

	return ret;
}


void DLlistdrop::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_iItem=pNMItemActivate->iItem;
	*pResult = 0;
}
