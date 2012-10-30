#pragma once
#pragma warning(disable : 4996)

// CitPropertySheet

class CitPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CitPropertySheet)

public:
	CitPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CitPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CitPropertySheet();

	void listinit();
protected:
	CImageList m_imgList;
	CListCtrl m_wndList;
	int  m_nSelectedItem;
	LPTSTR m_dfCaption;
	CFont m_dfListFont;
	CRect m_rectPage;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};


