// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "itbook.h"

#include "MainFrm.h"
#include "MyPropertySheet.h"
#include "DLupClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_MY_NOTIFYICON,OnMyIconNotify)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MAINQUIT, &CMainFrame::OnMainquit)
	ON_COMMAND(ID_MAINSHOW, &CMainFrame::OnMainshow)
	
	ON_COMMAND(ID_SETPROPERTY, &CMainFrame::OnSetproperty)
	ON_COMMAND(ID_MPROPERTY, &CMainFrame::OnMproperty)
	ON_UPDATE_COMMAND_UI(ID_MPROPERTY, &CMainFrame::OnUpdateMproperty)
	//ON_COMMAND(ID_MSEARCH, &CMainFrame::OnMsearch)
	//ON_UPDATE_COMMAND_UI(ID_MSEARCH, &CMainFrame::OnUpdateMsearch)
	ON_COMMAND(ID_EDITBAR, &CMainFrame::OnEditbar)
	ON_UPDATE_COMMAND_UI(ID_EDITBAR, &CMainFrame::OnUpdateEditbar)
	//ON_COMMAND(ID_DSPLLINENUM, &CMainFrame::OnDspllinenum)
	ON_UPDATE_COMMAND_UI(ID_DSPLLINENUM, &CMainFrame::OnUpdateDspllinenum)
	ON_COMMAND(ID_UPCLASSONLINE, &CMainFrame::OnUpclassonline)
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_isDspLineNum=TRUE;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	// TODO: 如果不需要可停靠工具栏，则删除这三行
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	
	//添加格式编辑工具条
	if (!m_editToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("格式编辑工具栏未能创建\n");
		return -1;      // 未能创建
	}
	m_editToolBar.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBarLeftOf(&m_editToolBar,&m_wndToolBar);
	//设置图标

	HICON m_hicon;
	m_hicon=AfxGetApp()->LoadIconW(IDI_EXEICON);
	SetIcon(m_hicon,TRUE);

	////添加可移动工具栏
	//if (!m_tooldlg.Create(this,IDD_TOOLDLG,CBRS_RIGHT|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,777))
 //    {
 //                              TRACE0("Failed to create dialog bar\n");
 //                              return -1;      // fail to create
 //    }
	// m_tooldlg.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
 //    //EnableDocking(CBRS_ALIGN_ANY);
 //    DockControlBar(&m_tooldlg);

	 //添加属性工具条
	if (!m_propertyBar.Create(this,IDD_PROPERTY,CBRS_TOP|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,IDD_PROPERTY))
    {
                               TRACE0("Failed to create dialog bar\n");
                               return -1;      // fail to create
    }
	m_propertyBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
     //EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_propertyBar);

	 //添加分类搜索工具条
	 if (!m_searchBar.Create(this,IDD_LOCALRESULT,CBRS_LEFT|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,IDD_LOCALRESULT))
	 {
		 TRACE0("Failed to create dialog bar\n");
		 return -1;      // fail to create
	 }
	 m_searchBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	 //EnableDocking(CBRS_ALIGN_ANY);
	 DockControlBar(&m_searchBar);

	 //DockControlBarBottomOf(&m_tooldlg,&m_propertyBar);
	//============================================添加托盘图标＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	TaskBarAddIcon(GetSafeHwnd(),120,AfxGetApp()->LoadIcon(IDI_EXEICON),_T("ITbaby笔记软件"));
	//======end=====
	//注册热键
	RegisterHotKey(m_hWnd,WM_MY_OPENITBOOK,MOD_SHIFT|MOD_ALT|MOD_CONTROL,'A');
	//设置主窗口大小
	CRect wrc;
	GetWindowRect(wrc);
	SetWindowPos(&wndTop,300,100,wrc.Width(),738,SWP_NOMOVE);
	return 0;

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

//====================================================================== 任务栏图标处理==============================
BOOL CMainFrame::TaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, LPTSTR lpszTip)
{

	d.cbSize=sizeof(NOTIFYICONDATA);
	d.hWnd=hwnd;
	d.uID=uID;
	d.uFlags=NIF_ICON | NIF_TIP | NIF_MESSAGE;
	d.uCallbackMessage=WM_MY_NOTIFYICON;
	d.hIcon=hicon;	
	if(lpszTip)
		lstrcpy(d.szTip,lpszTip);
	else
		d.szTip[0]='\0';
	return Shell_NotifyIcon(NIM_ADD,&d);
}



BOOL CMainFrame::TaskBarModifyIcon(HWND hwnd, UINT uID, HICON hicon, LPTSTR lpszTip)
{
	
	d.cbSize=sizeof(NOTIFYICONDATA);
	d.hWnd=hwnd;
	d.uID=uID;
	d.uFlags=NIF_ICON | NIF_TIP | NIF_MESSAGE;
	d.uCallbackMessage=WM_MY_NOTIFYICON;
	d.hIcon=hicon;
	if(lpszTip)
		lstrcpy(d.szTip,lpszTip);
	else
		d.szTip[0]='\0';
	return Shell_NotifyIcon(NIM_MODIFY,&d);

}
LRESULT CMainFrame::OnMyIconNotify(WPARAM wParam,LPARAM lParam)
{
	UINT uMouseMsg=LOWORD(lParam);
	switch(uMouseMsg)
	{
	case WM_LBUTTONUP:
		{
			ShowWindow(IsWindowVisible()?SW_HIDE:SW_SHOW);
			SetForegroundWindow();
			//SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}break;
	case WM_LBUTTONDBLCLK:
		{			
			ShowWindow(IsWindowVisible()? SW_HIDE:SW_SHOWNORMAL);
			SetForegroundWindow();
		
		}break;

	case WM_RBUTTONDOWN:
		{
			CPoint point;
			GetCursorPos(&point);
			this->SetForegroundWindow();
			
			CMenu m_menu;
			m_menu.LoadMenu(IDR_ICONMENU);
			//CMenu::SetMenuDrawMode(TYPE_XP); 
			//m_menu.LoadToolbar(IDR_TOOLBAR_MENU);
			
			CMenu *psub = (CMenu *)m_menu.GetSubMenu(0);  			
			DWORD dwID =psub->TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y ,this);
			m_menu.DestroyMenu();			
		}
		break;  
	}
	return 0;
} 
void CMainFrame::OnMainquit()
{
	// TODO: 在此添加命令处理程序代码
	int res=AfxMessageBox(_T("确认退出程序？"),MB_YESNO,0);
	if(res==IDYES){
		PostMessage(WM_CLOSE,0,0);
	}
}

void CMainFrame::OnMainshow()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(IsWindowVisible()?SW_HIDE:SW_SHOWNORMAL);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝ 任务栏图标处理＝＝＝＝＝＝＝＝//

void CMainFrame::OnDestroy()
{
	CMDIFrameWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	Shell_NotifyIcon(NIM_DELETE,&d);  //删除任务栏的图标
	UnregisterHotKey(m_hWnd,WM_MY_OPENITBOOK);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(message)
	{
		case WM_SYSCOMMAND:
			if(wParam==SC_MINIMIZE)
			{
				//接收到最小化消息时主窗口隐藏 
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
				return 0; 
			}
			else if(wParam==SC_CLOSE)
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
				return 0;
			}
			break; 
	}
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

// 停靠工具栏在另一个旁边
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect; //矩形区域定义
	DWORD dw;
	UINT n=0;
	RecalcLayout();//重新显示
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);//设置偏移值以停靠在同一边上
	dw=LeftOf->GetBarStyle();
	n=(dw&CBRS_ALIGN_TOP)?AFX_IDW_DOCKBAR_TOP:n;
	n=(dw&CBRS_ALIGN_BOTTOM &&n==0)?AFX_IDW_DOCKBAR_BOTTOM:n;
	n=(dw&CBRS_ALIGN_LEFT && n==0)?AFX_IDW_DOCKBAR_LEFT:n;
	n=(dw&CBRS_ALIGN_RIGHT && n==0)?AFX_IDW_DOCKBAR_RIGHT:n;
	DockControlBar(Bar,n,&rect);

}
void CMainFrame::DockControlBarBottomOf(CDialogBar* Bar, CDialogBar* LeftOf)
{
	CRect rect; //矩形区域定义
	DWORD dw;
	UINT n=0;
	RecalcLayout();//重新显示
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(-1,0);//设置偏移值以停靠在同一边上
	dw=LeftOf->GetBarStyle();
	n=(dw&CBRS_ALIGN_TOP)?AFX_IDW_DOCKBAR_TOP:n;
	n=(dw&CBRS_ALIGN_BOTTOM &&n==0)?AFX_IDW_DOCKBAR_BOTTOM:n;
	n=(dw&CBRS_ALIGN_LEFT && n==0)?AFX_IDW_DOCKBAR_LEFT:n;
	n=(dw&CBRS_ALIGN_RIGHT && n==0)?AFX_IDW_DOCKBAR_RIGHT:n;
	DockControlBar(Bar,n,&rect);

}
void CMainFrame::OnSetproperty()
{
	// TODO: 在此添加命令处理程序代码
	CMyPropertySheet propertyset(_T("设置"),this,0);
	propertyset.AddPage(&m_baseSet);
	propertyset.AddPage(&m_classset);
	propertyset.DoModal();

}

void CMainFrame::OnMproperty()
{
	// TODO: 在此添加命令处理程序代码
	if(m_propertyBar.IsWindowVisible())
	 {
		 m_propertyBar.ShowWindow(SW_HIDE);
	 }
	 else
	 {
		m_propertyBar.ShowWindow(SW_SHOW);
	 }
}

void CMainFrame::OnUpdateMproperty(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_propertyBar.IsWindowVisible());
}

//void CMainFrame::OnMsearch()
//{
//	 TODO: 在此添加命令处理程序代码
//	if(m_tooldlg.IsWindowVisible())
//		 m_tooldlg.ShowWindow(SW_HIDE);
//	 else
//		m_tooldlg.ShowWindow(SW_SHOW);
//}

//void CMainFrame::OnUpdateMsearch(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//	//pCmdUI->SetCheck(m_tooldlg.IsWindowVisible());
//}

void CMainFrame::OnEditbar()
{
	// TODO: 在此添加命令处理程序代码
	if(m_editToolBar.IsWindowVisible())
		 m_editToolBar.ShowWindow(SW_HIDE);
	 else
		m_editToolBar.ShowWindow(SW_SHOW);
}

void CMainFrame::OnUpdateEditbar(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_editToolBar.IsWindowVisible());
}


//void CMainFrame::OnDspllinenum()
//{
//	// TODO: 在此添加命令处理程序代码
//	if(m_isDspLineNum)
//		m_isDspLineNum=FALSE;
//	else
//		m_isDspLineNum=TRUE;
//}

//切换显示行号
void CMainFrame::OnUpdateDspllinenum(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_isDspLineNum);  //
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_searchBar.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)){ 
		return   TRUE; 
	}
	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnUpclassonline()
{
	// TODO: 在此添加命令处理程序代码
	CDLupClass upc;
	upc.DoModal();
}
LRESULT CMainFrame::OnHotKey(WPARAM wParam,LPARAM lParam){
	if(wParam==WM_MY_OPENITBOOK){
		ShowWindow(IsWindowVisible()?SW_HIDE:SW_SHOW);
		SetForegroundWindow();
		m_searchBar.GetDlgItem(IDC_SEARCHKEY)->SetFocus();
	}
	return 0;
}