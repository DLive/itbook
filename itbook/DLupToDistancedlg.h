#pragma once

// CDLupToDistancedlg 对话框

class CDLupToDistancedlg : public CDialog
{
	DECLARE_DYNAMIC(CDLupToDistancedlg)

public:
	CDLupToDistancedlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLupToDistancedlg();

	CStringA m_status;
// 对话框数据
	enum { IDD = IDD_UPTODISTANCEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnBnClickedOk();
};
