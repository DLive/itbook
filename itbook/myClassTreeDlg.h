#pragma once


// CmyClassTreeDlg 对话框

class CpropertyDlg;
class CmyClassTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CmyClassTreeDlg)

public:
	CmyClassTreeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CmyClassTreeDlg();
	
	BOOL m_classIsInit;//类树是否初始化
	//图标列表
	CImageList Cil1;
	//类树初始化
	BOOL m_classInit(void);
	
	CpropertyDlg* m_parent;
	//遍历树
	HTREEITEM FindItem(CTreeCtrl& tree, HTREEITEM item, DWORD strText,int* finLevel,int* curLevel);
// 对话框数据
	enum { IDD = IDD_CLASSTREEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

//	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnTvnSelchangedClassdlgtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickClassdlgtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
