// toolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "toolDlg.h"

#include "itbookDoc.h"
#include "itbookView.h"
#include "Conndb.h"

// CtoolDlg 对话框

IMPLEMENT_DYNAMIC(CtoolDlg, CDialogBar)

CtoolDlg::CtoolDlg()
{
}

CtoolDlg::~CtoolDlg()
{
	delete m_pLocalResult;
}

void CtoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CtoolDlg, CDialogBar)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_UPDATE_COMMAND_UI(IDC_LOCALSER, OnUpdateButton)
	ON_BN_CLICKED(IDC_LOCALSER, &CtoolDlg::OnBnClickedLocalser)
	ON_MESSAGE(WM_GOODBYELOCAL,OngoodByeLocal)
END_MESSAGE_MAP()


// CtoolDlg 消息处理程序

 LONG CtoolDlg::OnInitDialog(UINT wParam, LONG lParam){
              BOOL bRet = HandleInitDialog(wParam, lParam);
              if(!UpdateData(FALSE)){
                     TRACE0("Warning, Unalbe to init update.\n");
              }
              // Add Code here
			 CRect rc;
			 GetClientRect(rc);
			 rc.top+=150;
			// rc.bottom-=700;
			 m_classTree.Create( WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|WS_VISIBLE,rc,this,IDC_TOOLTREE);
              return bRet;
}

//CSize CtoolDlg::CalcDynamicLayout(int nLength, DWORD nMode){
//              CRect rcFrame;
//              GetDockingFrame()->GetClientRect(&rcFrame);
//			  rcFrame.top=0;
//			  rcFrame.bottom=400;
//			  rcFrame.left=0;
//			  rcFrame.right=300;
//			  //if(rcFrame.Width()>200)
//			  //{
//				 // rcFrame.left=0;
//				 // rcFrame.right=200;
//			  //}
//			  //if(rcFrame.Width()<100 || rcFrame.Height()<100)
//			  //{
//				 // rcFrame.left=0;
//				 // rcFrame.right=150;
//				 // rcFrame.top=0;
//				 // rcFrame.bottom=500;
//			  //}
//			 
//			  return CSize(rcFrame.Width(),rcFrame.Height());
//}

 void CtoolDlg::OnUpdateButton(CCmdUI * pCmdUI){
         pCmdUI ->Enable(TRUE);
    }

 void CtoolDlg::OnBnClickedLocalser()
 {



	
	 // TODO: 在此添加控件通知处理程序代码
	// CString title;
	// GetDlgItem(IDC_TITLE)->GetWindowTextW(title);

	//if(m_pLocalResult->GetSafeHwnd())
	//	{
	//		CRect drc;
	//		CRect frc;
	//		AfxGetMainWnd()->GetClientRect(&frc);
	//		m_pLocalResult->ShowWindow(TRUE);
	//		m_pLocalResult->GetClientRect(&drc);
	//		
	//		//drc.left+=frc.Width()/2;
	//		//drc.right+=frc.Width()/2;
	//		//drc.top+=frc.Height()/2;
	//		//drc.bottom+=frc.Height()/2+30;			 
	//		// m_pLocalResult->MoveWindow(drc);
	//		if(!title.IsEmpty())
	//		 m_pLocalResult->searchArticle(title);
	//		 
	//}
	//else
	//{
	//		//m_pLocalResult=new ClocalSearchResult();
	//		m_pLocalResult->Create();
	//		m_pLocalResult->ShowWindow(SW_SHOW);
	//		if(!title.IsEmpty())
	//			m_pLocalResult->searchArticle(title);
	//}
	 
 }
 LRESULT CtoolDlg::OngoodByeLocal(WPARAM wParam,LPARAM lParam)
 {
	TRACE("CEx07aView::OnGoodbye %x, %lx\n", wParam, lParam);	
	m_pLocalResult->ShowWindow(FALSE);
	return 0L;
 }
