// itbookView.cpp : CitbookView 类的实现

#include "stdafx.h"
#include "itbook.h"
#include "itbookDoc.h"
#include "CntrItem.h"
#include "itbookView.h"
#include "editBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CitbookView

IMPLEMENT_DYNCREATE(CitbookView, CRichEditView)

BEGIN_MESSAGE_MAP(CitbookView, CRichEditView)
	ON_WM_DESTROY()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRichEditView::OnFilePrintPreview)
	ON_COMMAND(ID_CHAR_COLOR, &CitbookView::OnCharColor)
	ON_NOTIFY(FN_SETFORMAT,ID_VIEW_TOOLBAR,&CitbookView::OnSetCharFormat)
	ON_COMMAND(ID_CHARBOLD, &CitbookView::OnCharbold)
	ON_UPDATE_COMMAND_UI(ID_CHARBOLD, &CitbookView::OnUpdateCharbold)
	ON_COMMAND(ID_CHARITALIC, &CitbookView::OnCharitalic)
	ON_UPDATE_COMMAND_UI(ID_CHARITALIC, &CitbookView::OnUpdateCharitalic)
	ON_COMMAND(ID_CHARSTRIKEOUT, &CitbookView::OnCharstrikeout)
	ON_UPDATE_COMMAND_UI(ID_CHARSTRIKEOUT, &CitbookView::OnUpdateCharstrikeout)
	ON_COMMAND(ID_CHARUNDELINE, &CitbookView::OnCharundeline)
	ON_UPDATE_COMMAND_UI(ID_CHARUNDELINE, &CitbookView::OnUpdateCharundeline)
	ON_COMMAND(ID_PARA_LEFT, &CitbookView::OnParaLeft)
	ON_UPDATE_COMMAND_UI(ID_PARA_LEFT, &CitbookView::OnUpdateParaLeft)
	ON_COMMAND(ID_PARA_CENTER, &CitbookView::OnParaCenter)
	ON_UPDATE_COMMAND_UI(ID_PARA_CENTER, &CitbookView::OnUpdateParaCenter)
	ON_COMMAND(ID_PARA_RIGHT, &CitbookView::OnParaRight)
	ON_UPDATE_COMMAND_UI(ID_PARA_RIGHT, &CitbookView::OnUpdateParaRight)
	ON_COMMAND(ID_INSERT_NUMBERING, &CitbookView::OnInsertNumbering)
	ON_UPDATE_COMMAND_UI(ID_INSERT_NUMBERING, &CitbookView::OnUpdateInsertNumbering)
	ON_COMMAND(ID_INSERT_BULLET, &CitbookView::OnInsertBullet)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BULLET, &CitbookView::OnUpdateInsertBullet)
	ON_COMMAND(ID_CHAR_HIGHLIGHT, &CitbookView::OnCharHighlight)
	ON_WM_SIZE()

	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CitbookView 构造/析构

CitbookView::CitbookView()
{
	// TODO: 在此处添加构造代码
}

CitbookView::~CitbookView()
{
}

BOOL CitbookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CRichEditView::PreCreateWindow(cs);
}

void CitbookView::OnInitialUpdate()
{
	
	CRichEditView::OnInitialUpdate();
	//设置默认字体
	//CHARFORMAT  cf/*  =  GetCharFormatSelection()*/;
	//memset   (&cf,   '\0 ',   sizeof   (CHARFORMAT));
	//cf.cbSize  =  sizeof(CHARFORMAT);  
	//GetRichEditCtrl().GetDefaultCharFormat(cf);  
	//cf.dwMask  =  CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED|CFM_COLOR;
	////cf.dwEffects &=~(CFE_BOLD);
	//cf.dwEffects  =0;
	//cf.yHeight=240;
	//cf.crTextColor   =   RGB(0x00,   0x00,   0x00);
	//wcscpy(cf.szFaceName,_T("Tahoma")); 
	//GetRichEditCtrl().SetDefaultCharFormat(cf); 
	////
	////long   lEventMask   =   GetRichEditCtrl().GetEventMask(); 
	////lEventMask   |=   ENM_SELCHANGE; 
	////GetRichEditCtrl().SetEventMask(lEventMask); 
	////// 设置打印边距(720 缇 = 1/2 英寸)
	////

	////

	CRichEditCtrl& rCtrl = GetRichEditCtrl();
	CHARFORMAT2 cfm;

	cfm.cbSize = sizeof(CHARFORMAT2);
	cfm.dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD |CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_PROTECTED|CFM_WEIGHT;
	cfm.dwEffects =0;
	cfm.yHeight = 240;
	::lstrcpy(cfm.szFaceName, _T("Arial"));
	rCtrl.SetDefaultCharFormat(cfm);
	//PARAFORMAT2 pf2; 
	////this->GetRichEditCtrl().SetDefaultCharFormat
	//pf2.cbSize = sizeof(PARAFORMAT2); 
	//pf2.dyLineSpacing=10;
	//pf2.dwMask = PFM_LINESPACING; 
	//pf2.bLineSpacingRule = 0; //1.5倍行距 
	//pf2.dyLineSpacing = 0; //bLineSpacingRule 设为3、4、5，dyLineSpacing才有效
	//this->GetRichEditCtrl().SetParaFormat(pf2);

	//CFont cf;
	//cf.CreateFontW(-24,                        // nHeight
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
	//			  _T("Arial"));
	//rCtrl.SetFont(&cf);
	//SetMargins(CRect(720, 720, 720, 720));
}


// CitbookView 打印

BOOL CitbookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}


void CitbookView::OnDestroy()
{
	// 析构时停用此项；这在
	// 使用拆分视图时非常重要 
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}



// CitbookView 诊断

#ifdef _DEBUG
void CitbookView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CitbookView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CitbookDoc* CitbookView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CitbookDoc)));
	return (CitbookDoc*)m_pDocument;
}
#endif //_DEBUG



// CitbookView 消息处理程序

//void CitbookView::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{	// storing code
//
//	}
//	else
//	{	// loading code
//	//	CString cc;
//	//	ar >> cc ;
//	//	AfxMessageBox(cc);
//	}
//}

void CitbookView::OnCharColor()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg;
	
	if( dlg.DoModal() == IDOK )
	{
		
		CRichEditView::OnColorPick(dlg.GetColor());
	}
}

void CitbookView::OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes)
{
	SetCharFormat(((CHARNMHDR*)pNMHDR)->cf);
	*pRes =1;
}
// 得到字体信息
void CitbookView::onCharFormatgetEx(CHARFORMAT2& cf, DWORD dwMask,DWORD dwEffects)
{
	cf = GetCharFormatSelection ();   
   
    if (!(cf.dwMask & dwMask) || !(cf.dwEffects & dwEffects))   
        cf.dwEffects = dwEffects;   
    else   
        cf.dwEffects = 0;   
   
    cf.dwMask = dwMask;
}

// 设置字体信息
void CitbookView::onCharFormatsetEx(CHARFORMAT2& cf)
{
	SetCharFormat(cf);
}
void CitbookView::OnCharbold()
{
	// TODO: 在此添加命令处理程序代码
	CHARFORMAT2 cf;   
    onCharFormatgetEx(cf,CFM_BOLD,CFE_BOLD);  
    SetCharFormat (cf);
}

void CitbookView::OnUpdateCharbold(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateCharEffect (pCmdUI, CFM_BOLD, CFE_BOLD);
}

void CitbookView::OnCharitalic()
{
	// TODO: 在此添加命令处理程序代码

	CHARFORMAT2 cf;   
    onCharFormatgetEx(cf,CFM_ITALIC,CFE_ITALIC);  
    SetCharFormat (cf);

}

void CitbookView::OnUpdateCharitalic(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateCharEffect (pCmdUI,CFM_ITALIC,CFE_ITALIC);
}

void CitbookView::OnCharstrikeout()
{
	// TODO: 在此添加命令处理程序代码
	CHARFORMAT2 cf;   
    onCharFormatgetEx(cf,CFM_STRIKEOUT,CFE_STRIKEOUT);  
    SetCharFormat (cf);
}

void CitbookView::OnUpdateCharstrikeout(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateCharEffect (pCmdUI,CFM_STRIKEOUT,CFE_STRIKEOUT);
}

void CitbookView::OnCharundeline()
{
	// TODO: 在此添加命令处理程序代码
	CHARFORMAT2 cf;   
    onCharFormatgetEx(cf,CFM_UNDERLINE,CFE_UNDERLINE);  
    SetCharFormat (cf);
}

void CitbookView::OnUpdateCharundeline(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateCharEffect (pCmdUI,CFM_UNDERLINE,CFE_UNDERLINE);
}

void CitbookView::OnParaLeft()
{
	// TODO: 在此添加命令处理程序代码
	OnParaAlign(PFA_LEFT);
}

void CitbookView::OnUpdateParaLeft(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateParaAlign (pCmdUI, PFA_LEFT);   
}

void CitbookView::OnParaCenter()
{
	// TODO: 在此添加命令处理程序代码
	OnParaAlign (PFA_CENTER); 
}

void CitbookView::OnUpdateParaCenter(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateParaAlign (pCmdUI, PFA_CENTER);
}

void CitbookView::OnParaRight()
{
	// TODO: 在此添加命令处理程序代码
	 OnParaAlign (PFA_RIGHT);   
	 
}

void CitbookView::OnUpdateParaRight(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	OnUpdateParaAlign (pCmdUI, PFA_RIGHT);  
}

// 设置段落属性
void CitbookView::onParaFormatSetEx(PARAFORMAT2& pf)
{
	CRichEditCtrl& rCtrl = GetRichEditCtrl();		
	pf.cbSize = sizeof PARAFORMAT2;

	CHARRANGE cr;
	rCtrl.GetSel(cr);
	if (cr.cpMax != cr.cpMin)
		SendMessage(EM_SETPARAFORMAT, SCF_SELECTION, (LPARAM)&pf);	
	else
		SendMessage(EM_SETPARAFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&pf);
}
// 得到段落信息
void CitbookView::onParaFormatGetEx(PARAFORMAT2& pf)
{
	pf.cbSize = sizeof PARAFORMAT2;
	SendMessage(EM_GETPARAFORMAT, 1, (LPARAM)&pf);
}

void CitbookView::OnInsertNumbering()
{
	// TODO: 在此添加命令处理程序代码
	PARAFORMAT2 pf;
	onParaFormatGetEx(pf);
	if (pf.dwMask & PFM_NUMBERING && pf.wNumbering == 2)
	{
		pf.wNumbering = 0;
		pf.dxOffset = 0;
		pf.dxStartIndent = 0;
		pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{	
		pf.dwMask = PFM_NUMBERING | PFM_NUMBERINGSTART | PFM_NUMBERINGSTYLE | PFM_NUMBERINGTAB;
		pf.wNumbering = 2;
		pf.wNumberingStart = 1;
		pf.wNumberingStyle = 1;
		pf.wNumberingTab = 360;
		if (pf.dxOffset == 0)
		{
			pf.dxOffset = m_nBulletIndent;
			pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	onParaFormatSetEx(pf);
}

void CitbookView::OnUpdateInsertNumbering(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	PARAFORMAT2 pf;
	onParaFormatGetEx(pf);
	pCmdUI->SetRadio( (pf.dwMask & PFM_NUMBERING) ? ((pf.wNumbering & 2) ? 1 : 0) : 2);
}



void CitbookView::OnInsertBullet()
{
	// TODO: 在此添加命令处理程序代码
	PARAFORMAT2 pf;
	onParaFormatGetEx(pf);
	if (pf.dwMask & PFM_NUMBERING && pf.wNumbering == PFN_BULLET)
	{
		pf.wNumbering = 0;
		pf.dxOffset = 0;
		pf.dxStartIndent = 0;
		pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{	
		pf.dwMask = PFM_NUMBERING | PFM_NUMBERINGSTART | PFM_NUMBERINGSTYLE | PFM_NUMBERINGTAB;
		pf.wNumbering =PFN_BULLET;
		//pf.wNumberingStart = 1;
		//pf.wNumberingStyle = 1;
		//pf.wNumberingTab = 360;
		if (pf.dxOffset == 0)
		{
			pf.dxOffset = 20;// 也可以是 m_nBulletIndent
			pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	onParaFormatSetEx(pf);
}

void CitbookView::OnUpdateInsertBullet(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	PARAFORMAT2 pf;
	onParaFormatGetEx(pf);
	pCmdUI->SetRadio( (pf.dwMask & PFM_NUMBERING) ? ((pf.wNumbering & PFN_BULLET) ? 1 : 0) : 2);
}

void CitbookView::OnCharHighlight()
{
	// TODO: 在此添加命令处理程序代码
		CColorDialog dlg;

	if( IDOK == dlg.DoModal() )	
	{	
		CHARFORMAT2 cf;
		::memset( &cf, 0, sizeof( cf ) );

		cf.cbSize = sizeof(cf);

		//SendMessage(EM_GETCHARFORMAT, (WPARAM)SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
		
		cf.dwMask = CFM_BACKCOLOR;
		cf.crBackColor =dlg.GetColor();
		SendMessage(EM_SETCHARFORMAT, (WPARAM) SCF_SELECTION,(LPARAM)&cf);
	}
}

void CitbookView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	
	if(bActivate)
	{
		
		CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
		CpropertyDlg* pDlg=&pMain->m_propertyBar;
		pDlg->SendMessage(WM_MY_CHANGEINFO,(WPARAM)(CitbookView*)pActivateView,0);
		
	}
	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
//void CitbookView::OnSize(UINT nType, int cx, int cy)
//{
//	CRichEditView::OnSize(nType, cx, cy);
//
//	// TODO: 在此处添加消息处理程序代码
//	if(((CMainFrame*)AfxGetMainWnd())->m_isDspLineNum && nType==SIZE_MAXIMIZED)
//	{
//		CRect rc;
//		GetParent()->GetClientRect(rc);
//		rc.left=m_LineNumWidth;
//		
//		::SetWindowPos(this->m_hWnd,HWND_TOP,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOREDRAW);
//	}
//}



//LRESULT CitbookView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	if( message == WM_PAINT )
//	{
//		ShowLineNum(this->GetParent()->m_hWnd); 
//	}
//	return CRichEditView::WindowProc(message, wParam, lParam);
//}
////画行号
//void CitbookView::ShowLineNum( HWND hEdit )
//{
//	/*
//	功能：显示文本的总行数
//	参数： 
//	hEdit：要显示行号的文本框，普通的Edit控件没有测试过，这里只用RichEdit
//
//	返回值：没有。
//	--------------------------------------------------------------------------------
//	*/
//	RECT     ClientRect;     //RichEdit的客户区大小
//	HDC      hdcEdit;         //RichEdit的Dc（设备环境）
//	HDC      hdcCpb;          //与RichEdit兼容的Dc
//	HBITMAP  hdcBmp;      //RichEdit兼容的位图dc
//	//int      CharHeight;      //字符的高度
//	int      chHeight;        //字符的高度，常量 
//	int      FirstLine;       //文本框中的第一个可见行的行号。
//	int      ClientHeight;    //RichEdit的客户区高度 
//	int      LineCount;       //文本的总行数 
//	int		 width=30;
//	RECT	 numrc;
//	wchar_t     countBuf[10];   //显示行号的缓冲区
//	CHARFORMAT     CharFmt;  //RichEdit中的一个结构，用于获取字符的一系列信息，这里只用它来获取字符高度
//	
//	//获取RichEdit的Dc
//	hdcEdit = ::GetDC( hEdit);
//	::GetClientRect( hEdit, &ClientRect);
//	//获取RichEdit的客户区高度
//	ClientHeight = ClientRect.bottom - ClientRect.top;
//	//创建与RichEdit兼容的Dc
//	numrc=ClientRect;
//	hdcCpb = CreateCompatibleDC( hdcEdit );
//	//创建与RichEdit兼容的位图Dc，我们用来显示行号用的。
//	hdcBmp = CreateCompatibleBitmap( hdcEdit, width, ClientHeight );
//	//将位图dc选入RichEdit环境中
//	SelectObject( hdcCpb, hdcBmp );
//	//填充显示行号dc的背景颜色。大家可以试试其它颜色
//	FillRect( hdcCpb, &ClientRect, CreateSolidBrush(0x8080ff) );
//	SetBkMode( hdcCpb, TRANSPARENT );
//	//获取当前RichEdit文本中的总行数
//	LineCount = ::SendMessage( this->m_hWnd, EM_GETLINECOUNT, 0, 0 );
//
//	RtlZeroMemory( &CharFmt, sizeof(CharFmt) );
//	CharFmt.cbSize = sizeof( CharFmt );
//	CharFmt=this->GetCharFormatSelection();
//	//::SendMessage( GetRichEditCtrl().m_hWnd, EM_GETCHARFORMAT, TRUE, (long)&CharFmt );
//	//获取字符高度，以英寸为单位，需转化为磅，只要除以20就得到磅尺寸。
//	chHeight = abs(CharFmt.yHeight) / 20; 
//	CFont *fn;
//	fn=this->GetFont();
//	//fn=this->GetRichEditCtrl().GetFont();
//	LOGFONT lfn;
//	if(fn)
//	{
//		fn->GetLogFont(&lfn);
//	}
//	TEXTMETRIC tm;
//	GetTextMetrics(::GetDC(this->GetRichEditCtrl().m_hWnd),&tm);
//	//设置显示行号的前景色
//	SetTextColor( hdcCpb, 0x000000 );
//	//获取文本框中第一个可见的行的行号，没有这个行号，显示不会跟着文本的滚动而滚动。
//	FirstLine = ::SendMessage( m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0 );
//	FirstLine++;
//	//在位图dc中循环输出行号
//	numrc.top=2;
//	numrc.bottom=numrc.top+chHeight+chHeight/2;
//	//this->GetRichEditCtrl().LineScroll(FirstLine);
//	PARAFORMAT2 pf2;
//	pf2.cbSize = sizeof(PARAFORMAT2);
//	pf2.dwMask=PFM_LINESPACING|PFM_STYLE|PFM_SHADING;
//	this->GetRichEditCtrl().GetParaFormat(pf2);
//	
//	TRACE(_T("tmWei=%d,tmItalic=%d tmdestcent=%d,tmshadowWei=%d \n"),tm.tmWeight,tm.tmItalic,tm.tmDescent,pf2.wShadingWeight);
//	TRACE(_T("charH=%d,charH=%d,tmHeitht=%d ,tmLeading=%d tmEleading=%d rcHeight=%d lineSpacing=%d linerule=%d \n"),CharFmt.yHeight,chHeight,tm.tmHeight,tm.tmInternalLeading,tm.tmExternalLeading,numrc.bottom-numrc.top,pf2.dyLineSpacing,pf2.bLineSpacingRule);
//	int color=40;
//	int col2=50;
//	
//	while( FirstLine <= LineCount )
//	{
//		COLORREF col=RGB(100,col2,color);
//		color+=5;
//		col2+=7;
//		FillRect( hdcCpb, &numrc,CreateSolidBrush(col) );
//		DrawText(hdcCpb,countBuf,wsprintf( countBuf, TEXT("%4u"), FirstLine++ ),&numrc,DT_VCENTER|DT_SINGLELINE);
//		//TextOut( hdcCpb,0, CharHeight, countBuf, wsprintf( countBuf, TEXT("%4u"), FirstLine++ ));
//		numrc.top+=chHeight+chHeight/2;
//		numrc.bottom+=chHeight+chHeight/2;
//		if(numrc.bottom > ClientRect.bottom ) break;
//	}
//	//将已"画好"的位图真正"贴"到RichEdit中
//	BitBlt( hdcEdit, 0, 0, width, ClientHeight, hdcCpb, 0, 0, SRCCOPY );
//	DeleteDC( hdcCpb );
//	::ReleaseDC( hEdit, hdcEdit );
//	DeleteObject( hdcBmp );
//
//}
//
//BOOL CitbookView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	return CRichEditView::OnMouseWheel(nFlags, zDelta, pt);
//}
//
//void CitbookView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	
//	//CRichEditView::OnVScroll(nSBCode, nPos, pScrollBar);
//
//	SCROLLINFO si;
//	this->GetScrollInfo(SB_VERT,&si);
//	
//	if(nPos!=si.nPos && nPos!=0)
//	{
//		int count=this->GetRichEditCtrl().GetLineCount();
//		int lineAvg=si.nMax/count;
//
//		TRACE(_T("npos=%d pbarpos=%d npage=%d mi=%d,mx=%d count=%d avg=%d \n"), nPos,si.nPos,si.nPage,si.nMin,si.nMax,count,(si.nMax)/count);
//		int dir=nPos-si.nPos;
//		if(dir>=lineAvg/2)
//			this->GetRichEditCtrl().LineScroll(1);
//		else if(dir<=-(lineAvg/2) && dir<0)
//			this->GetRichEditCtrl().LineScroll(-1);
//	}
//}
