// ChildFrm.cpp : CChildFrame 类的实现
//
#include "stdafx.h"
#include "itbook.h"

#include "ChildFrm.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_DSPLLINENUM, &CChildFrame::OnDspllinenum)
END_MESSAGE_MAP()


// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame 消息处理程序

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
		//设置图标

	HICON m_hicon;
	m_hicon=AfxGetApp()->LoadIconW(IDR_itbookTYPE);
	SetIcon(m_hicon,TRUE);
	return 0;
}

void CChildFrame::OnDspllinenum()
{
	// TODO: 在此添加命令处理程序代码
	CRect rc;
	
	int left=0;
	if(((CMainFrame*)AfxGetMainWnd())->m_isDspLineNum)
	{
		((CMainFrame*)AfxGetMainWnd())->m_isDspLineNum=FALSE;
		left=0;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_isDspLineNum=TRUE;
		left=30;
	}

	POSITION   posTemp=AfxGetApp()->m_pDocManager-> GetFirstDocTemplatePosition(); 
	while(posTemp) 
	{ 
		//遍历文档模板 
		CMultiDocTemplate   *pDocTemp=(CMultiDocTemplate*)AfxGetApp()->m_pDocManager-> GetNextDocTemplate(posTemp); 
		if(pDocTemp) 
		{	
			POSITION   posDoc=pDocTemp-> GetFirstDocPosition(); 
			while(posDoc) 
			{	
				//遍历文档模板对应的文档 
				CDocument   *pDoc=pDocTemp-> GetNextDoc(posDoc); 
				if(pDoc) 
				{ 
					POSITION   posView=pDoc-> GetFirstViewPosition(); 
					while(posView) 
					{ 
					//遍历文档对应的视图 
						CView   *pView=pDoc-> GetNextView(posView); 
						if(pView) 
						{ 
					    pView->GetParent()->GetClientRect(rc);
						rc.left+=left;
						pView->SetWindowPos(FromHandle(HWND_TOP),rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOREDRAW);
						//得到view对应的父窗口childframe， 
						//CWnd   *pFrame=pView-> GetParent(); 
						//if(pFrame) 
						//	pFrame-> SendMessage(WM_CLOSE);//关闭childframe,也就关闭了view 
						}	
					} 
				} 
			} 
		}
	}

	//::SetWindowPos(this->m_hWnd,HWND_TOP,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOREDRAW);
}
