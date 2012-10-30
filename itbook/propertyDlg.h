#pragma once
#include "localSearchResult.h"
//#include "itbookDoc.h"
// CpropertyDlg 对话框
#include "myClassTreeDlg.h"

class CpropertyDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CpropertyDlg)

public:
	CpropertyDlg();   // 标准构造函数
	virtual ~CpropertyDlg();

// 对话框数据
	enum { IDD = IDD_PROPERTY };
	CDocument* m_pDoc;
	ClocalSearchResult* m_pLocalResult;//查询对话框
	
	
	//分类树
	BOOL m_classdlgIsCreate;//false is not 
	CmyClassTreeDlg* m_classtreedlg;
	
	
	//分类初始化
	void classInfoInit();

	void ChangeClassName(int cid,CString cname);//更改类名
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg LRESULT OnchangeInfo(WPARAM, LPARAM);
	afx_msg LRESULT setClassinit(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnNMDblclkClasstree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeNowclass();

	afx_msg void OnCbnSetfocusNowclass();
	afx_msg void OnEnChangeTitle();
	afx_msg void OnEnKillfocusAuthor();
	afx_msg void OnEnKillfocusKeyword();
	afx_msg void OnCbnSelchangeDifficulty();
	afx_msg void OnBnClickedInserttodb();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnUpdateButtontest(CCmdUI * pCmdUI);
	afx_msg void OnBnClickedSavelocal();
};
