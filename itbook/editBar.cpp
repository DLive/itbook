// editBar.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "editBar.h"

#include "MainFrm.h"
#include "itbookView.h"

// CeditBar
const static int nFontSizes[] = 
	{8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};

IMPLEMENT_DYNAMIC(CeditBar, CToolBar)

CeditBar::CeditBar()
{

}

CeditBar::~CeditBar()
{
}


BEGIN_MESSAGE_MAP(CeditBar, CToolBar)
	ON_CBN_SELENDOK(ID_FONTSIZE, OnSelectFontSize)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CeditBar 消息处理程序

void CeditBar::OnSelectFontSize()
{
	TCHAR szSize[5];
	int index = m_fontSizeBox.GetCurSel();
	if( index != CB_ERR )
		m_fontSizeBox.GetLBText(index, szSize );
	else
		m_fontSizeBox.GetWindowText(szSize, 5);

	// Get size in Twips
	int nSize = _ttoi( szSize ) * 20;			// atoi for tchar

	if( !nSize )
		return;

	CHARNMHDR fh;
	CHARFORMAT &cf = fh.cf;
	fh.hwndFrom = m_hWnd;
	fh.idFrom = GetDlgCtrlID();
	fh.code = FN_SETFORMAT;

	cf.dwMask = CFM_SIZE;
	cf.yHeight = nSize;
   
	if(AfxGetApp()->m_pDocManager->GetOpenDocumentCount()<=0)
		return;
	VERIFY(::SendMessage(AfxGetMainWnd()->m_hWnd,WM_NOTIFY,(WPARAM)fh.idFrom, (LPARAM)&fh));
	

	//GetOwner()->SendMessage(WM_NOTIFY, fh.idFrom, (LPARAM)&fh);
}

//BOOL CeditBar::Create(CWnd* pParentWnd, DWORD dwStyle , UINT nID)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CToolBar::Create(pParentWnd, dwStyle, nID);
//}

int CeditBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(!LoadToolBar(IDR_EDITBAR))
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics( &tm );
	int cxChar = tm.tmAveCharWidth;
	int cyChar = tm.tmHeight + tm.tmExternalLeading;

	CRect rect; 
	GetItemRect( CommandToIndex(ID_FONTSIZE), &rect );
	rect.right = rect.left + 8*cxChar;
	rect.bottom = rect.top + 16*cyChar;

	SetButtonInfo( CommandToIndex(ID_FONTSIZE), ID_FONTSIZE,TBBS_SEPARATOR,rect.Width() );

	if(!m_fontSizeBox.Create(WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS,rect,this,ID_FONTSIZE))
	{
		TRACE0("FAILED TO CREATE COMBO-BOX\n");
		return 0;
	}
	m_fontSizeBox.ShowWindow(SW_SHOW);
	
	CString sSize;
	int nSizeCount = sizeof(nFontSizes) / sizeof(nFontSizes[0]);
	for( int i=0; i < nSizeCount; i++ )
	{
		sSize.Format(_T("%d"), nFontSizes[i] );
		m_fontSizeBox.AddString( sSize );
	}
    m_fontSizeBox.SetCurSel(4); 


	return 0;
}
