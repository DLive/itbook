// DLupToDistancedlg.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "DLupToDistancedlg.h"


// CDLupToDistancedlg 对话框

IMPLEMENT_DYNAMIC(CDLupToDistancedlg, CDialog)

CDLupToDistancedlg::CDLupToDistancedlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLupToDistancedlg::IDD, pParent)
{

}

CDLupToDistancedlg::~CDLupToDistancedlg()
{
}

void CDLupToDistancedlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLupToDistancedlg, CDialog)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_BN_CLICKED(IDOK, &CDLupToDistancedlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDLupToDistancedlg 消息处理程序

int CDLupToDistancedlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码

	return 0;
}
LONG CDLupToDistancedlg::OnInitDialog(UINT wParam, LONG lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	if (!UpdateData(FALSE))

	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
	}
	CComboBox *sta=(CComboBox*)GetDlgItem(IDC_STATUS);
	sta->InsertString(0,_T("草稿"));
	sta->InsertString(1,_T("发布"));
	sta->SetCurSel(0);

	return bRet;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDLupToDistancedlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_status=((CComboBox*)GetDlgItem(IDC_STATUS))->GetCurSel()==0?"draft":"public";
	CDialog::OnOK();
}
