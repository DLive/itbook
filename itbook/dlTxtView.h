#pragma once


// CdlTxtView 视图
class CtxtDoc;
class CdlTxtView : public CRichEditView
{
	DECLARE_DYNCREATE(CdlTxtView)

protected:
	CdlTxtView();           // 动态创建所使用的受保护的构造函数
	virtual ~CdlTxtView();

public:
	CtxtDoc* GetDocument() const;


	int			m_LineNumWidth;//行号宽度
	void ShowLineNum( HWND hEdit );//画行号
	afx_msg void OnSetSize(NMHDR* pNMHDR, LRESULT* pRes);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnInitialUpdate();
	afx_msg void OnEditPaste();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


