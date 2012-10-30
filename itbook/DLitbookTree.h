#pragma once


// DLitbookTree

class DLitbookTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(DLitbookTree)

public:
	DLitbookTree();
	virtual ~DLitbookTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//图标列表
	CImageList Cil1;
	 HTREEITEM FindItem(CTreeCtrl& tree, HTREEITEM item, DWORD strText,int* finLevel,int* curLevel); //查找结点
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 初始化
	BOOL classInit(void);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


