// itbook.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "itbook.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "itbookDoc.h"
#include "itbookView.h"

#include "txtDoc.h"
#include "txtView.h"
#include "dlTxtView.h"

#include "Ccommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CitbookApp

BEGIN_MESSAGE_MAP(CitbookApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CitbookApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CitbookApp::seltNewfile)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_NEWTITFILE, &CitbookApp::OnNewtitfile)
	ON_COMMAND(ID_NEWTXTFILE, &CitbookApp::OnNewtxtfile)
END_MESSAGE_MAP()



//删除temp线程主函数
UINT DeleteTemp(LPVOID lpParam)
{
	Ccommon getfile;
	CString tempPath;
	getfile.getExeUrl(&tempPath);
	tempPath+=_T("Temp\\");
	Ccommon deletefile;
	deletefile.DeleteDirectory(tempPath);
	
	return 0;
}

// CitbookApp 构造
CitbookApp::CitbookApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CitbookApp 对象

CitbookApp theApp;


// CitbookApp 初始化

BOOL CitbookApp::InitInstance()
{
	//--------程序只运行一个实例-------------------------

	HANDLE m_hMutex = ::CreateMutex(NULL, TRUE, _T("itbookone"));
    if (GetLastError() == ERROR_ALREADY_EXISTS) //程序已经运行
    {
        HWND   oldHWnd = NULL;   
        oldHWnd = ::FindWindow(NULL, _T("ItBaby笔记管理")); //查找已经运行的程序
        if (oldHWnd)   
        {   
			  AfxMessageBox(_T("程序已运行"));
            ::ShowWindow(oldHWnd, SW_SHOWNORMAL); //激活显示找到的已运行的程序
            ::SetForegroundWindow(oldHWnd);       //将已运行的程序设置为当前窗口
        }  
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
        return FALSE;
    }
	//-------------------------------------------------------------

	//创建删除线程
	AfxBeginThread(DeleteTemp,(LPVOID)123);
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("ItBook笔记管理"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_itbookTYPE,
		RUNTIME_CLASS(CitbookDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CitbookView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_itbookTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);
	
	pDocTemplate=new CMultiDocTemplate(IDR_txtTYPE,
			RUNTIME_CLASS(CtxtDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CdlTxtView));
	pDocTemplate->SetContainerInfo(IDR_itbookTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();


	return TRUE;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CitbookApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



// CitbookApp 消息处理程序


void CitbookApp::OnNewtitfile()
{
	CDocTemplate* pTemplate;
	// TODO: 在此添加命令处理程序代码
	POSITION pz=m_pDocManager->GetFirstDocTemplatePosition();
	CString ext;
	while(pz)
	{
		pTemplate=m_pDocManager->GetNextDocTemplate(pz);
		pTemplate->GetDocString(ext,CDocTemplate::filterExt);
		if(ext==_T(".txt"))
			break;
	}
	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);
	
	pTemplate->OpenDocumentFile(NULL);
	((CtxtDoc*)pTemplate->OpenDocumentFile(NULL))->m_bRTF=SF_RTF;//只是让文档在保存时，选择该类型
	//OnNewtxtfile();
	
}

void CitbookApp::OnNewtxtfile()
{
	// TODO: 在此添加命令处理程序代码
		CDocTemplate* pTemplate;
	// TODO: 在此添加命令处理程序代码
	POSITION pz=m_pDocManager->GetFirstDocTemplatePosition();
	CString ext;
	while(pz)
	{
		pTemplate=m_pDocManager->GetNextDocTemplate(pz);
		pTemplate->GetDocString(ext,CDocTemplate::filterExt);
		if(ext==_T(".txt"))
			break;
	}
	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);
	((CtxtDoc*)pTemplate->OpenDocumentFile(NULL))->m_bRTF=SF_TEXT;

}
void CitbookApp::seltNewfile()
{

}