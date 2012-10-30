// txtView.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "txtView.h"
#include "txtDoc.h"

#include "MainFrm.h"
// CtxtView

IMPLEMENT_DYNCREATE(CtxtView, CEditView)

CtxtView::CtxtView()
{

}

CtxtView::~CtxtView()
{
}

BEGIN_MESSAGE_MAP(CtxtView, CEditView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CtxtView 诊断

#ifdef _DEBUG
void CtxtView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CtxtView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CtxtView 消息处理程序
CtxtDoc* CtxtView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtxtDoc)));
	return (CtxtDoc*)m_pDocument;
}

void CtxtView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(bActivate)
	{
		
		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
		CpropertyDlg* pDlg=&pMain->m_propertyBar;
		pDlg->SendMessage(WM_MY_CHANGEINFO,(WPARAM)(CtxtView*)pActivateView,1);
		
	}
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CtxtView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
	
	// TODO: 在此添加专用代码和/或调用基类
	//设置默认字体
	LOGFONT lf;
	this->GetEditCtrl().GetFont()->GetLogFont(&lf);
	lf.lfWeight=400;
	wcscpy(lf.lfFaceName,_T("Tahoma"));
	lf.lfHeight=18;
	m_font.CreateFontIndirectW(&lf);
	if (m_font.m_hObject == NULL)
	{
		 m_font.CreatePointFont(120,L"Tahoma");
	}
	
	if (m_font.m_hObject != NULL)
		 SetFont (&m_font);
	SetTabStops(16);
	
	// 设置打印边距(720 缇 = 1/2 英寸)
}

BOOL CtxtView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//cs.style &= ~(ES_AUTOVSCROLL|WS_VSCROLL);
	//return CEditView::PreCreateWindow(cs);
	BOOL   bPreCreated   =   CEditView::PreCreateWindow(cs); 
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// 启用换行
	//cs.style   &=   ~(ES_AUTOHSCROLL);	//   Enable   word-wrapping 
	return   bPreCreated; 
}

int CtxtView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}
