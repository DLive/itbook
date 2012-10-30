#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Cclassset 对话框

class Cclassset : public CPropertyPage
{
	DECLARE_DYNAMIC(Cclassset)

public:
	Cclassset();
	virtual ~Cclassset();

// 对话框数据
	enum { IDD = IDD_CLASSSET };

	bool classinit();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 类树
	virtual BOOL OnInitDialog();
	// 添加分类的父类
	CComboBox m_addFather;
	// 删除分类
	CComboBox m_delclass;
	// 修改分类
	CComboBox m_changeClassname;
	// 父类修改
	CComboBox m_fatherChangeName;
	CComboBox m_fatherFather;
	afx_msg void OnBnClickedAddclass();
	afx_msg void OnBnClickedChange();
	afx_msg void OnCbnSelchangeChangeclass();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedChangefather();
	afx_msg void OnCbnSelchangeFatherchang();
};
