// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "toolDlg.h"
#include "editBar.h"
#include "propertyDlg.h"
#include "classset.h"
#include "baseSet.h"
#include "itbookView.h"
#include "localSearchResult.h"


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:
	CtoolDlg		m_tooldlg;
	CeditBar		m_editToolBar;
	CpropertyDlg	m_propertyBar;
	ClocalSearchResult m_searchBar;  //收搜分类工具条
	NOTIFYICONDATA  d;//托盘图标结构体
	BOOL			m_isDspLineNum;  //是否显示行号

	//设置选项
	Cclassset m_classset;
	CbaseSet m_baseSet;
// 操作
public:
	BOOL TaskBarAddIcon(HWND hwnd, UINT uID, HICON hicon, LPTSTR lpszTip);
	BOOL TaskBarModifyIcon(HWND hwnd, UINT uID, HICON hicon, LPTSTR lpszTip);
	
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
		// 停靠工具栏在另一个旁边
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	void DockControlBarBottomOf(CDialogBar* Bar, CDialogBar* LeftOf);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	
	//添加字体大小工具函数

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnMyIconNotify(WPARAM wParam,LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMainquit();
	afx_msg void OnMainshow();

	afx_msg void OnSetproperty();
	afx_msg void OnMproperty();
	afx_msg void OnUpdateMproperty(CCmdUI *pCmdUI);
	afx_msg void OnMsearch();
	afx_msg void OnUpdateMsearch(CCmdUI *pCmdUI);
	afx_msg void OnEditbar();
	afx_msg void OnUpdateEditbar(CCmdUI *pCmdUI);
	
	//afx_msg void OnDspllinenum();
	afx_msg void OnUpdateDspllinenum(CCmdUI *pCmdUI);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnUpclassonline();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
};


