#pragma once


// DLlistdrop

class DLlistdrop : public CListCtrl
{
	DECLARE_DYNAMIC(DLlistdrop)

public:
	DLlistdrop();
	virtual ~DLlistdrop();


	CComboBox	*m_pSelClass;
	int			m_iItem;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};