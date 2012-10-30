// itbookView.h : CitbookView 类的接口
//


#pragma once
//定义视图改变消息
class CitbookDoc;
class CitbookCntrItem;

class CitbookView : public CRichEditView
{
protected: // 仅从序列化创建
	CitbookView();
	DECLARE_DYNCREATE(CitbookView)

// 属性
public:
	CitbookDoc* GetDocument() const;
	//int			m_LineNumWidth;//行号宽度

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// 实现
public:
	virtual ~CitbookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//void ShowLineNum( HWND hEdit );//画行号
// 生成的消息映射函数
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes);
	DECLARE_MESSAGE_MAP()
public:
//	virtual void Serialize(CArchive& ar);
	afx_msg void OnCharColor();
	
	afx_msg void OnCharbold();
	afx_msg void OnUpdateCharbold(CCmdUI *pCmdUI);
	// 得到字体信息
	void onCharFormatgetEx(CHARFORMAT2& cf, DWORD dwMask,DWORD dwEffects);
	// 设置字体信息
	void onCharFormatsetEx(CHARFORMAT2& cf);
	afx_msg void OnCharitalic();
	afx_msg void OnUpdateCharitalic(CCmdUI *pCmdUI);
	afx_msg void OnCharstrikeout();
	afx_msg void OnUpdateCharstrikeout(CCmdUI *pCmdUI);
	afx_msg void OnCharundeline();
	afx_msg void OnUpdateCharundeline(CCmdUI *pCmdUI);
	afx_msg void OnParaLeft();
	afx_msg void OnUpdateParaLeft(CCmdUI *pCmdUI);
	afx_msg void OnParaCenter();
	afx_msg void OnUpdateParaCenter(CCmdUI *pCmdUI);
	afx_msg void OnParaRight();
	afx_msg void OnUpdateParaRight(CCmdUI *pCmdUI);
	// 设置段落属性
	void onParaFormatSetEx(PARAFORMAT2& pf);
	afx_msg void OnInsertNumbering();
	afx_msg void OnUpdateInsertNumbering(CCmdUI *pCmdUI);
	// 得到段落信息
	void onParaFormatGetEx(PARAFORMAT2& pf);
	afx_msg void OnInsertBullet();
	afx_msg void OnUpdateInsertBullet(CCmdUI *pCmdUI);
	afx_msg void OnCharHighlight();
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	//afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#ifndef _DEBUG  // itbookView.cpp 中的调试版本
inline CitbookDoc* CitbookView::GetDocument() const
   { return reinterpret_cast<CitbookDoc*>(m_pDocument); }
#endif

