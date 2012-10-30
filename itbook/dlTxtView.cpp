// dlTxtView.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "dlTxtView.h"

#include "txtDoc.h"
#include "MainFrm.h"

// CdlTxtView

IMPLEMENT_DYNCREATE(CdlTxtView, CRichEditView)

CdlTxtView::CdlTxtView()
{
	m_LineNumWidth=30;
}

CdlTxtView::~CdlTxtView()
{
}

BEGIN_MESSAGE_MAP(CdlTxtView, CRichEditView)
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_VSCROLL()

	ON_NOTIFY(FN_SETFORMAT,ID_VIEW_TOOLBAR,&CdlTxtView::OnSetSize)
	ON_COMMAND(ID_EDIT_PASTE, &CdlTxtView::OnEditPaste)
END_MESSAGE_MAP()


// CdlTxtView 诊断

#ifdef _DEBUG
void CdlTxtView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CdlTxtView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CdlTxtView 消息处理程序
CtxtDoc* CdlTxtView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtxtDoc)));
	return (CtxtDoc*)m_pDocument;
}
void CdlTxtView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(bActivate)
	{
		
		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
		CpropertyDlg* pDlg=&pMain->m_propertyBar;
		pDlg->SendMessage(WM_MY_CHANGEINFO,(WPARAM)(CdlTxtView*)pActivateView,1); //**
		
	}
	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CdlTxtView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CRichEditCtrl& rCtrl = GetRichEditCtrl();
	CHARFORMAT2 cf;
	//CHARFORMAT cf;
	cf.cbSize=sizeof(CHARFORMAT2);
	rCtrl.GetSelectionCharFormat(cf);
	cf.dwMask|=CFM_BOLD;
	cf.dwEffects&=~CFE_BOLD;//设置粗体，取消用cf.dwEffects&=~CFE_BOLD;
	cf.dwMask|=CFM_ITALIC;
	cf.dwEffects&=~CFE_ITALIC;//设置斜体，取消用cf.dwEffects&=~CFE_ITALIC;
	cf.dwMask|=CFM_UNDERLINE;
	cf.dwEffects&=~CFE_UNDERLINE;//设置斜体，取消用cf.dwEffects&=~CFE_UNDERLINE;
	//cf.dwMask|=CFM_COLOR;
	//cf.crTextColor = RGB(255,0,0);//设置颜色
	cf.dwMask|=CFM_SIZE;
	cf.yHeight =200;//设置高度
	cf.dwMask|=CFM_FACE;
	cf.yOffset=0;
	cf.dwMask|=CFM_OFFSET;
	cf.sSpacing=0;
	cf.dwMask|=CFM_SPACING;
	cf.bPitchAndFamily=0;
	::lstrcpy(cf.szFaceName ,_T("新宋体"));//设置字体
	//rich.SetSelectionCharFormat(cf);
	rCtrl.SetDefaultCharFormat(cf);

	//cfm.cbSize = sizeof(CHARFORMAT2);
	//cfm.dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD |CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED|CFM_WEIGHT;
	//cfm.dwEffects =0;
	//cfm.yHeight = 240;
	//::lstrcpy(cfm.szFaceName, _T("Arial"));
	//rCtrl.SetDefaultCharFormat(cfm);
	PARAFORMAT2 pf2; 
	//this->GetRichEditCtrl().SetDefaultCharFormat
	pf2.cbSize = sizeof(PARAFORMAT2); 
	pf2.dyLineSpacing=220;
	pf2.dwMask = PFM_LINESPACING|PFM_SPACEAFTER|PFM_SPACEBEFORE|PFM_OFFSET; 
	pf2.bLineSpacingRule = 4; //1.5倍行距 
	//pf2.dxOffset=100;
	pf2.dySpaceAfter=0;
	pf2.dySpaceBefore=0;
	pf2.bOutlineLevel=0;
	pf2.dxOffset=0;
	//bLineSpacingRule 设为3、4、5，dyLineSpacing才有效
	this->GetRichEditCtrl().SetParaFormat(pf2);

	//CFont cfs;
	//
	//cfs.CreateFontW(12,                        // nHeight
	//				0,                         // nWidth
	//			  0,                         // nEscapement
	//			  0,                         // nOrientation
	//			  400,                 // nWeight
	//			  FALSE,                     // bItalic
	//			  FALSE,                     // bUnderline
	//			  0,                         // cStrikeOut
	//			  ANSI_CHARSET,              // nCharSet
	//			  OUT_DEFAULT_PRECIS,        // nOutPrecision
	//			  CLIP_DEFAULT_PRECIS,       // nClipPrecision
	//			  DEFAULT_QUALITY,           // nQuality
	//			  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	//			  _T("新宋体"));
	//rCtrl.SetFont(&cfs);
}


BOOL CdlTxtView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nFlags==0)
	{
		int FirstLine = ::SendMessage( m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0 );
		if(zDelta>0)
			FirstLine=-1;
		else
			FirstLine=1;	
		this->GetRichEditCtrl().LineScroll(FirstLine);
		//this->GetRichEditCtrl().LineIndex(FirstLine);
		 TRACE(_T("nFlags=%d zDelta=%d x=%d y=%d   line=%d\n"), nFlags, zDelta, pt.x, pt.y, FirstLine);
	}
	return TRUE;
	//return CRichEditView::OnMouseWheel(nFlags, zDelta, pt);
}

void CdlTxtView::OnSize(UINT nType, int cx, int cy)
{
	CRichEditView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(((CMainFrame*)AfxGetMainWnd())->m_isDspLineNum && nType==SIZE_MAXIMIZED)
	{
		CRect rc;
		GetParent()->GetClientRect(rc);
		rc.left=m_LineNumWidth;
		
		::SetWindowPos(this->m_hWnd,HWND_TOP,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOREDRAW);
	}
}

LRESULT CdlTxtView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	if( message == WM_PAINT )
	{
		if(((CMainFrame*)AfxGetMainWnd())->m_isDspLineNum)
			ShowLineNum(this->GetParent()->m_hWnd); 
	}

	return CRichEditView::WindowProc(message, wParam, lParam);
}

//画行号
void CdlTxtView::ShowLineNum( HWND hEdit )
{
	/*
	功能：显示文本的总行数
	参数： 
	hEdit：要显示行号的文本框，普通的Edit控件没有测试过，这里只用RichEdit

	返回值：没有。
	--------------------------------------------------------------------------------
	*/
	RECT     ClientRect;     //RichEdit的客户区大小
	HDC      hdcEdit;         //RichEdit的Dc（设备环境）
	HDC      hdcCpb;          //与RichEdit兼容的Dc
	HBITMAP  hdcBmp;      //RichEdit兼容的位图dc
	//int      CharHeight;      //字符的高度
	int      chHeight;        //字符的高度，常量 
	int      FirstLine;       //文本框中的第一个可见行的行号。
	int      ClientHeight;    //RichEdit的客户区高度 
	int      LineCount;       //文本的总行数 
	int		 width=30;
	RECT	 numrc;
	wchar_t     countBuf[10];   //显示行号的缓冲区
	CHARFORMAT     CharFmt;  //RichEdit中的一个结构，用于获取字符的一系列信息，这里只用它来获取字符高度
	
	//获取RichEdit的Dc
	hdcEdit = ::GetDC( hEdit);
	::GetClientRect( hEdit, &ClientRect);
	//获取RichEdit的客户区高度
	ClientHeight = ClientRect.bottom - ClientRect.top;
	//创建与RichEdit兼容的Dc
	numrc=ClientRect;
	hdcCpb = CreateCompatibleDC( hdcEdit );
	//创建与RichEdit兼容的位图Dc，我们用来显示行号用的。
	hdcBmp = CreateCompatibleBitmap( hdcEdit, width, ClientHeight );
	//将位图dc选入RichEdit环境中
	SelectObject( hdcCpb, hdcBmp );
	//填充显示行号dc的背景颜色。大家可以试试其它颜色
	FillRect( hdcCpb, &ClientRect, CreateSolidBrush(0xc8c8c8) );
	SetBkMode( hdcCpb, TRANSPARENT );
	//获取当前RichEdit文本中的总行数
	LineCount = ::SendMessage( this->m_hWnd, EM_GETLINECOUNT, 0, 0 );

	RtlZeroMemory( &CharFmt, sizeof(CharFmt) );
	CharFmt.cbSize = sizeof( CharFmt );
	CharFmt=this->GetCharFormatSelection();
	//::SendMessage( GetRichEditCtrl().m_hWnd, EM_GETCHARFORMAT, TRUE, (long)&CharFmt );
	//获取字符高度，以英寸为单位，需转化为磅，只要除以20就得到磅尺寸。
	chHeight = abs(CharFmt.yHeight) / 20; 
	CFont *fn;
	fn=this->GetFont();
	//fn=this->GetRichEditCtrl().GetFont();
	LOGFONT lfn;
	if(fn)
	{
		fn->GetLogFont(&lfn);
	}
	TEXTMETRIC tm;
	//GetTextMetrics(::GetDC(this->GetRichEditCtrl().m_hWnd),&tm);
	//设置显示行号的前景色
	SetTextColor( hdcCpb, 0x000000 );
	//获取文本框中第一个可见的行的行号，没有这个行号，显示不会跟着文本的滚动而滚动。
	FirstLine = ::SendMessage( m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0 );
	FirstLine++;
	//在位图dc中循环输出行号
	numrc.top=2;
	numrc.bottom=numrc.top+chHeight+chHeight/2;
	//this->GetRichEditCtrl().LineScroll(FirstLine);
	//PARAFORMAT2 pf2;
	//pf2.cbSize = sizeof(PARAFORMAT2);
	//pf2.dwMask=PFM_LINESPACING|PFM_STYLE|PFM_SHADING;
	//this->GetRichEditCtrl().GetParaFormat(pf2);
	
	//TRACE(_T("tmWei=%d,tmItalic=%d tmdestcent=%d,tmshadowWei=%d chHeight=%d \n"),tm.tmWeight,tm.tmItalic,tm.tmDescent,pf2.wShadingWeight,chHeight);
	//TRACE(_T("charH=%d,charH=%d,tmHeitht=%d ,tmLeading=%d tmEleading=%d rcHeight=%d lineSpacing=%d linerule=%d \n"),CharFmt.yHeight,chHeight,tm.tmHeight,tm.tmInternalLeading,tm.tmExternalLeading,numrc.bottom-numrc.top,pf2.dyLineSpacing,pf2.bLineSpacingRule);
	//int color=40;
	//int col2=50;
	
	while( FirstLine <= LineCount )
	{
		//COLORREF col=RGB(100,col2,color);
		//color+=5;
		//col2+=7;
		//FillRect( hdcCpb, &numrc,CreateSolidBrush(col) );
		DrawText(hdcCpb,countBuf,wsprintf( countBuf, TEXT("%4u"), FirstLine++ ),&numrc,DT_VCENTER|DT_SINGLELINE);
		//TextOut( hdcCpb,0, CharHeight, countBuf, wsprintf( countBuf, TEXT("%4u"), FirstLine++ ));
		numrc.top+=chHeight+chHeight/2;
		numrc.bottom+=chHeight+chHeight/2;
		if(numrc.bottom > ClientRect.bottom ) break;
	}
	//将已"画好"的位图真正"贴"到RichEdit中
	BitBlt( hdcEdit, 0, 0, width, ClientHeight, hdcCpb, 0, 0, SRCCOPY );
	DeleteDC( hdcCpb );
	::ReleaseDC( hEdit, hdcEdit );
	DeleteObject( hdcBmp );

}
void CdlTxtView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CRichEditView::OnVScroll(nSBCode, nPos, pScrollBar);

	SCROLLINFO si;
	this->GetScrollInfo(SB_VERT,&si);
	
	if(nPos!=si.nPos && nPos!=0)
	{
		int count=this->GetRichEditCtrl().GetLineCount();
		int lineAvg=si.nMax/count;

		//TRACE(_T("npos=%d pbarpos=%d npage=%d mi=%d,mx=%d count=%d avg=%d \n"), nPos,si.nPos,si.nPage,si.nMin,si.nMax,count,(si.nMax)/count);
		int dir=nPos-si.nPos;
		int mov=abs(dir/lineAvg);
		if(abs(dir)%lineAvg>=lineAvg/2)
			mov++;
		if(dir>0){

			this->GetRichEditCtrl().LineScroll(mov);
		}
		else if(dir<0)
		{
			this->GetRichEditCtrl().LineScroll(-mov);
		}
	}
}

void CdlTxtView::OnSetSize(NMHDR* pNMHDR, LRESULT* pRes)
{
	
	CFont cfont;
	cfont.CreateFontW(-((CHARNMHDR*)pNMHDR)->cf.yHeight/20,                        // nHeight
					0,                         // nWidth
				  0,                         // nEscapement
				  0,                         // nOrientation
				  400,                 // nWeight
				  FALSE,                     // bItalic
				  FALSE,                     // bUnderline
				  0,                         // cStrikeOut
				  ANSI_CHARSET,              // nCharSet
				  OUT_DEFAULT_PRECIS,        // nOutPrecision
				  CLIP_DEFAULT_PRECIS,       // nClipPrecision
				  DEFAULT_QUALITY,           // nQuality
				  DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				  _T("Arial"));
	GetRichEditCtrl().SetFont(&cfont);

	*pRes =1;
}
void CdlTxtView::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	CHARFORMAT cf; 
	memset(&cf,   0,   sizeof(CHARFORMAT)); 
	//得到设定的字体
	
	this->GetRichEditCtrl().GetDefaultCharFormat(cf); 
	this->GetRichEditCtrl().Paste(); 
	//得到当前的光标的位置 
	LONG   iStart,iEnd; 
	this->GetRichEditCtrl().GetSel(iStart,iEnd); 
	//   为选定的内容设定所选字体 
	this->GetRichEditCtrl().SetSel(0,-1); 
	this->GetRichEditCtrl().SetSelectionCharFormat(cf); 
	//   重新设置光标位置 
	this->GetRichEditCtrl().SetSel(iStart,iEnd);

	//CRichEditView::OnEditPaste();
}

BOOL CdlTxtView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CRichEditView::PreCreateWindow(cs);
	 if (LoadLibraryA("RICHED20.DLL") == NULL)       
	 {          
		 AfxMessageBox(_T("Fail to load riched20.dll"),MB_OK | MB_ICONERROR);          
		 PostMessage(WM_QUIT,0,0);         
		 return FALSE; 
	}
}