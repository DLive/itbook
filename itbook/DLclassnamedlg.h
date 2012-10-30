#pragma once


// CDLclassnamedlg 对话框

class CDLclassnamedlg : public CDialog
{
	DECLARE_DYNAMIC(CDLclassnamedlg)

public:
	CDLclassnamedlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLclassnamedlg();

// 对话框数据
	enum { IDD = IDD_CLASSNAME };

	CString m_name;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
