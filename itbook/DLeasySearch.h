#pragma once


// DLeasySearch 对话框

class DLeasySearch : public CDialogBar
{
	DECLARE_DYNAMIC(DLeasySearch)

public:
	DLeasySearch();   // 标准构造函数
	virtual ~DLeasySearch();

// 对话框数据
	enum { IDD = IDD_EASYSEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LONG OnInitDialog ( UINT, LONG );

	DECLARE_MESSAGE_MAP()
};
