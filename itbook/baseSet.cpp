// baseSet.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "baseSet.h"


// CbaseSet 对话框

IMPLEMENT_DYNAMIC(CbaseSet, CPropertyPage)

CbaseSet::CbaseSet(CWnd* pParent /*=NULL*/)
	:  CPropertyPage(CbaseSet::IDD)
{

}

CbaseSet::~CbaseSet()
{
}

void CbaseSet::DoDataExchange(CDataExchange* pDX)
{
	 CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CbaseSet, CDialog)
	ON_BN_CLICKED(IDC_AUTORUN, &CbaseSet::OnBnClickedAutorun)
	ON_BN_CLICKED(IDC_RELATIONTXT, &CbaseSet::OnBnClickedRelationtxt)
END_MESSAGE_MAP()


// CbaseSet 消息处理程序

BOOL CbaseSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	CheckAutorun();

	//判断关联
	if(CheckFileRelation(_T(".txt"),_T("ittit.Document")))
	{
		((CButton*)GetDlgItem(IDC_RELATIONTXT))->SetCheck(true);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RELATIONTXT))->SetCheck(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BOOL CbaseSet::CheckAutorun()
{
	HKEY hKey;
	long rz1=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_READ,&hKey));
	if(rz1!=ERROR_SUCCESS)
	{
		return FALSE;
	}
	DWORD style=REG_SZ;
	DWORD length;
	TCHAR *runpath;
	rz1=(::RegQueryValueExW(hKey,_T("itbook"),NULL,&style,NULL,&length));
	if(rz1==ERROR_SUCCESS)
	{
		runpath=new WCHAR[length];
		rz1=(::RegQueryValueEx(hKey,_T("itbook"),NULL,&style,(LPBYTE)runpath,&length));
	}
	::RegCloseKey(hKey);
	if(rz1!=ERROR_SUCCESS)
	{
		((CButton*)GetDlgItem(IDC_AUTORUN))->SetCheck(false);
		return FALSE;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_AUTORUN))->SetCheck(true);
		TCHAR exepath[MAX_PATH];
		GetModuleFileName(NULL,exepath,MAX_PATH);
		if(wcscmp(exepath,((TCHAR*)runpath))!=0)
		{
			setAutorun();
		}
		return FALSE;
	}
}
BOOL CbaseSet::setAutorun()
{
	TCHAR exepath[MAX_PATH];
	GetModuleFileName(NULL,exepath,MAX_PATH);

	HKEY hKey;
	long rz1=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_WRITE,&hKey));
	rz1=(::RegSetValueEx(hKey,_T("itbook"),0,REG_SZ,(const unsigned char*)exepath,MAX_PATH));
	::RegCloseKey(hKey);
	return TRUE;
}
void CbaseSet::OnBnClickedAutorun()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_AUTORUN))->GetCheck())
	{
		setAutorun();
	}
	else
	{
		delAutorun();
	}
}
BOOL CbaseSet::delAutorun()
{
	TCHAR exepath[MAX_PATH];
	GetModuleFileName(NULL,exepath,MAX_PATH);

	HKEY hKey;
	long rz1=(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_WRITE,&hKey));
	rz1=(::RegDeleteValue(hKey,_T("itbook")));
	//rz1=(::RegSetValueEx(hKey,_T("itbook"),0,REG_SZ,(const unsigned char*)exepath,MAX_PATH));
	::RegCloseKey(hKey);
	return TRUE;
}

void CbaseSet::OnBnClickedRelationtxt()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_RELATIONTXT))->GetCheck())
	{
		RegisterFileRelationtxt(_T(".txt"),_T("ittit.Document"));
	}
	else
	{
		RegisterFileRelationtxt(_T(".txt"),_T("txtfile"));
	}
}
// 检测文件关联情况
// strExt: 要检测的扩展名(例如: ".txt")
// strAppKey: ExeName扩展名在注册表中的键值(例如: "txtfile")
// 返回TRUE: 表示已关联，FALSE: 表示未关联

// 检查文件是否关联
BOOL CbaseSet::CheckFileRelation(const TCHAR* strExt, const TCHAR* strAppKey)
{
	int nRet=FALSE;
    HKEY hExtKey;
    TCHAR szPath[_MAX_PATH];  
    DWORD dwSize=sizeof(szPath);  
    if(RegOpenKey(HKEY_CLASSES_ROOT,strExt,&hExtKey)==ERROR_SUCCESS)
    {
        RegQueryValueEx(hExtKey,NULL,NULL,NULL,(LPBYTE)szPath,&dwSize);
        if(_wcsicmp(szPath,strAppKey)==0)
        {
            nRet=TRUE;
        }
        RegCloseKey(hExtKey);
        return nRet;
    }
    return nRet;
}


// 注册文件关联
// strExe: 要检测的扩展名(例如: ".txt")
// strAppName: 要关联的应用程序名(例如: "C:\MyApp\MyApp.exe")
// strAppKey: ExeName扩展名在注册表中的键值(例如: "txtfile")
// strDefaultIcon: 扩展名为strAppName的图标文件(例如: "C:\MyApp\MyApp.exe,0")
// strDescribe: 文件类型描述
void  CbaseSet::RegisterFileRelation(TCHAR *strExt, TCHAR *strAppName, TCHAR *strAppKey, TCHAR *strDefaultIcon, TCHAR *strDescribe)
{
    TCHAR strTemp[_MAX_PATH];
    HKEY hKey;
    
    RegCreateKey(HKEY_CLASSES_ROOT,strExt,&hKey);
    RegSetValue(hKey,_T(""),REG_SZ,strAppKey,wcslen(strAppKey)+1);
    RegCloseKey(hKey);
    
    RegCreateKey(HKEY_CLASSES_ROOT,strAppKey,&hKey);
    RegSetValue(hKey,_T(""),REG_SZ,strDescribe,wcslen(strDescribe)+1);
    RegCloseKey(hKey);
    
    swprintf_s(strTemp,_T("%s\\DefaultIcon"),strAppKey);
    RegCreateKeyW(HKEY_CLASSES_ROOT,strTemp,&hKey);
    RegSetValue(hKey,_T(""),REG_SZ,strDefaultIcon,wcslen(strDefaultIcon)+1);
    RegCloseKey(hKey);
    
    swprintf_s(strTemp,_T("%s\\Shell"),strAppKey);
    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    RegSetValue(hKey,_T(""),REG_SZ,_T("Open"),wcslen(_T("Open"))+1);
    RegCloseKey(hKey);
    
    swprintf_s(strTemp,_T("%s\\Shell\\Open\\Command"),strAppKey);
    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    swprintf_s(strTemp,_T("%s \"%%1\""),strAppName);
    RegSetValue(hKey,_T(""),REG_SZ,strTemp,wcslen(strTemp)+1);
    RegCloseKey(hKey);
}
void  CbaseSet::RegisterFileRelationtxt(TCHAR *strExt, TCHAR *strAppKey)
{
    HKEY hKey;
    
    RegCreateKey(HKEY_CLASSES_ROOT,strExt,&hKey);
    RegSetValue(hKey,_T(""),REG_SZ,strAppKey,wcslen(strAppKey)+1);
    RegCloseKey(hKey);
    
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_FLUSHNOWAIT,0,0);
    //RegCreateKey(HKEY_CLASSES_ROOT,strAppKey,&hKey);
    //RegSetValue(hKey,_T(""),REG_SZ,strDescribe,wcslen(strDescribe)+1);
    //RegCloseKey(hKey);
    
    //swprintf_s(strTemp,_T("%s\\DefaultIcon"),strAppKey);
    //RegCreateKeyW(HKEY_CLASSES_ROOT,strTemp,&hKey);
    //RegSetValue(hKey,_T(""),REG_SZ,strDefaultIcon,wcslen(strDefaultIcon)+1);
    //RegCloseKey(hKey);
    
    //swprintf_s(strTemp,_T("%s\\Shell"),strAppKey);
    //RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    //RegSetValue(hKey,_T(""),REG_SZ,_T("Open"),wcslen(_T("Open"))+1);
    //RegCloseKey(hKey);
    //
    //swprintf_s(strTemp,_T("%s\\Shell\\Open\\Command"),strAppKey);
    //RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    //swprintf_s(strTemp,_T("%s \"%%1\""),strAppName);
    //RegSetValue(hKey,_T(""),REG_SZ,strTemp,wcslen(strTemp)+1);
    //RegCloseKey(hKey);
}