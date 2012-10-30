// DLclassnamedlg.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "DLclassnamedlg.h"


// CDLclassnamedlg 对话框

IMPLEMENT_DYNAMIC(CDLclassnamedlg, CDialog)

CDLclassnamedlg::CDLclassnamedlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLclassnamedlg::IDD, pParent)
{

}

CDLclassnamedlg::~CDLclassnamedlg()
{
}

void CDLclassnamedlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLclassnamedlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDLclassnamedlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDLclassnamedlg 消息处理程序

void CDLclassnamedlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_CLASSNAME,m_name);
	OnOK();
}
