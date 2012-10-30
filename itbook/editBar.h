#pragma once


// CeditBar
struct CHARNMHDR : public NMHDR
{
	CHARFORMAT2 cf;
	CHARNMHDR() {cf.cbSize = sizeof(CHARFORMAT);}
};

// Define format notifications constant

class CeditBar : public CToolBar
{
	DECLARE_DYNAMIC(CeditBar)

public:
	CeditBar();
	virtual ~CeditBar();
	CComboBox m_fontSizeBox;

protected:
	afx_msg void OnSelectFontSize();
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


