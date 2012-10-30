#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CbaseSet 对话框

class CbaseSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CbaseSet)

public:
	CbaseSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CbaseSet();

// 对话框数据
	enum { IDD = IDD_BASEPROPERTY };
	
	BOOL CheckAutorun();
	BOOL setAutorun();
	BOOL delAutorun();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAutorun();
	afx_msg void OnBnClickedRelationtxt();
	// 检查文件是否关联
	BOOL CheckFileRelation(const TCHAR* strExt, const TCHAR* strAppKey);
	void RegisterFileRelation(TCHAR *strExt, TCHAR *strAppName, TCHAR *strAppKey, TCHAR *strDefaultIcon, TCHAR *strDescribe);
	void RegisterFileRelationtxt(TCHAR *strExt, TCHAR *strAppKey);
};
