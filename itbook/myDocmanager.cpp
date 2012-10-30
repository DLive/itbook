// myDocmanager.cpp : 实现文件
//

#include "stdafx.h"
#include "itbook.h"
#include "myDocmanager.h"


// CmyDocmanager


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// MultiExt: needed for a bunch of MFC private constants and functions
#include "afxpriv.h"

// determine number of elements in an array (not bytes)
//#define _countof(array) (sizeof(array)/sizeof(array[0]))


//////////////////////////////////////////////////////////////////////
//
// MultiExt: Next block copy and paste from DOCMGR.CPP
//

#ifndef _MAC
static const TCHAR szShellOpenFmt[] = _T("%s\\shell\\open\\%s");
static const TCHAR szShellPrintFmt[] = _T("%s\\shell\\print\\%s");
static const TCHAR szShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
static const TCHAR szDefaultIconFmt[] = _T("%s\\DefaultIcon");
static const TCHAR szShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

static const TCHAR szIconIndexFmt[] = _T(",%d");
static const TCHAR szCommand[] = _T("command");
static const TCHAR szOpenArg[] = _T(" \"%1\"");
static const TCHAR szPrintArg[] = _T(" /p \"%1\"");
static const TCHAR szPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
static const TCHAR szDDEArg[] = _T(" /dde");

static const TCHAR szDDEExec[] = _T("ddeexec");
static const TCHAR szDDEOpen[] = _T("[open(\"%1\")]");
static const TCHAR szDDEPrint[] = _T("[print(\"%1\")]");
static const TCHAR szDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

static const TCHAR szShellNewValueName[] = _T("NullFile");
static const TCHAR szShellNewValue[] = _T("");

static BOOL AFXAPI _AfxDeleteRegKey(LPCTSTR lpszKey)
{
	LPTSTR lpszKeyCopy = _tcsdup(lpszKey);
	LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

	while (lpszLast != NULL)
	{
		*lpszLast-- = '\0';
		HKEY hKey;
		if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
			break;

		TCHAR szScrap[_MAX_PATH+1];
		DWORD dwLen = _countof(szScrap);
		BOOL bItExists = FALSE;

		if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
			bItExists = TRUE;
		::RegCloseKey(hKey);

		if (bItExists)
			break;
		::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);

		lpszLast = _tcsrchr(lpszKeyCopy, '\\');
	}
	free(lpszKeyCopy);
	return TRUE;
}

static BOOL AFXAPI SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}
#endif

//
// MultiExt: EOBlock
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CmyDocmanager::CmyDocmanager()
{

}

CmyDocmanager::~CmyDocmanager()
{

}

// Document functions
void CmyDocmanager::RegisterShellFileTypes(BOOL bCompat)
{
    ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

    CString strPathName, strTemp;

    AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

    POSITION pos = m_templateList.GetHeadPosition();
    for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
    {
        CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

        CString strOpenCommandLine = strPathName;
        CString strPrintCommandLine = strPathName;
        CString strPrintToCommandLine = strPathName;
        CString strDefaultIconCommandLine = strPathName;

        if (bCompat)
        {
            CString strIconIndex;
            HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strPathName, nTemplateIndex);
            if (hIcon != NULL)
            {
                strIconIndex.Format(szIconIndexFmt, nTemplateIndex);
                DestroyIcon(hIcon);
            }
            else
            {
                strIconIndex.Format(szIconIndexFmt, DEFAULT_ICON_INDEX);
            }
            strDefaultIconCommandLine += strIconIndex;
        }

        CString strMultiFilterExt, strFileTypeId, strFileTypeName;
        if (pTemplate->GetDocString(strFileTypeId,
           CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
        {
            // if enough info to register it
            if (!pTemplate->GetDocString(strFileTypeName,
               CDocTemplate::regFileTypeName))
                strFileTypeName = strFileTypeId;    // use id name

            ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

            // first register the type ID of our server
            if (!SetRegKey(strFileTypeId, strFileTypeName))
                continue;       // just skip it

            if (bCompat)
            {
                // path\DefaultIcon = path,1
                strTemp.Format(szDefaultIconFmt, (LPCTSTR)strFileTypeId);
                if (!SetRegKey(strTemp, strDefaultIconCommandLine))
                    continue;       // just skip it
            }

            // If MDI Application
            if (!pTemplate->GetDocString(strTemp, CDocTemplate::windowTitle) ||
                strTemp.IsEmpty())
            {
                // path\shell\open\ddeexec = [open("%1")]
                strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
                    (LPCTSTR)szDDEExec);
                if (!SetRegKey(strTemp, szDDEOpen))
                    continue;       // just skip it

                if (bCompat)
                {
                    // path\shell\print\ddeexec = [print("%1")]
                    strTemp.Format(szShellPrintFmt, (LPCTSTR)strFileTypeId,
                        (LPCTSTR)szDDEExec);
                    if (!SetRegKey(strTemp, szDDEPrint))
                        continue;       // just skip it

                    // path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
                    strTemp.Format(szShellPrintToFmt, (LPCTSTR)strFileTypeId,
                        (LPCTSTR)szDDEExec);
                    if (!SetRegKey(strTemp, szDDEPrintTo))
                        continue;       // just skip it

                    // path\shell\open\command = path /dde
                    // path\shell\print\command = path /dde
                    // path\shell\printto\command = path /dde
                    strOpenCommandLine += szDDEArg;
                    strPrintCommandLine += szDDEArg;
                    strPrintToCommandLine += szDDEArg;
                }
                else
                {
                    strOpenCommandLine += szOpenArg;
                }
            }
            else
            {
                // path\shell\open\command = path filename
                // path\shell\print\command = path /p filename
                // path\shell\printto\command = path /pt filename printer driver port
                strOpenCommandLine += szOpenArg;
                if (bCompat)
                {
                    strPrintCommandLine += szPrintArg;
                    strPrintToCommandLine += szPrintToArg;
                }
            }

            // path\shell\open\command = path filename
            strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
                (LPCTSTR)szCommand);
            if (!SetRegKey(strTemp, strOpenCommandLine))
                continue;       // just skip it

            if (bCompat)
            {
                // path\shell\print\command = path /p filename
                strTemp.Format(szShellPrintFmt, (LPCTSTR)strFileTypeId,
                    (LPCTSTR)szCommand);
                if (!SetRegKey(strTemp, strPrintCommandLine))
                    continue;       // just skip it

                // path\shell\printto\command = path /pt filename printer driver port
                strTemp.Format(szShellPrintToFmt, (LPCTSTR)strFileTypeId,
                    (LPCTSTR)szCommand);
                if (!SetRegKey(strTemp, strPrintToCommandLine))
                    continue;       // just skip it
            }

            pTemplate->GetDocString(strMultiFilterExt, CDocTemplate::filterExt);

            // MultiExt: strMultiFilterExt is formatted as: ".ext1;.ext2;.ext3".
            if (!strMultiFilterExt.IsEmpty())
            {
                CString strFilterExt;
                // for all extensions in the filter
                for (int i=0; AfxExtractSubString(strFilterExt, strMultiFilterExt, i, ';'); i++)
                {
                    ASSERT(strFilterExt[0] == '.');

                    LONG lSize = _MAX_PATH * 2;
                    LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
                        strTemp.GetBuffer(lSize), &lSize);
                    strTemp.ReleaseBuffer();

                    if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
                        strTemp == strFileTypeId)
                    {
                        // no association for that suffix
                        if (!SetRegKey(strFilterExt, strFileTypeId))
                            continue;

                        if (bCompat)
                        {
                            strTemp.Format(szShellNewFmt, strFilterExt);
                            (void)SetRegKey(strTemp, szShellNewValue, szShellNewValueName);
                        }
                    }
                }   // for all extensions in the filter
            }   // if (!strMultiFilterExt.IsEmpty())
        }   // if enough info to register
    }   // for all templates in the template list
}


void CmyDocmanager::UnregisterShellFileTypes()
{
    ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

    CString strPathName, strTemp;

    AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

    POSITION pos = m_templateList.GetHeadPosition();
    for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
    {
        CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

        CString strMultiFilterExt, strFileTypeId, strFileTypeName;
        if (pTemplate->GetDocString(strFileTypeId,
           CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
        {
            // if enough info to register it
            if (!pTemplate->GetDocString(strFileTypeName,
               CDocTemplate::regFileTypeName))
                strFileTypeName = strFileTypeId;    // use id name

            ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

            strTemp.Format(szDefaultIconFmt, (LPCTSTR)strFileTypeId);
            _AfxDeleteRegKey(strTemp);

            // If MDI Application
            if (!pTemplate->GetDocString(strTemp, CDocTemplate::windowTitle) ||
                strTemp.IsEmpty())
            {
                // path\shell\open\ddeexec = [open("%1")]
                strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
                    (LPCTSTR)szDDEExec);
                _AfxDeleteRegKey(strTemp);

                // path\shell\print\ddeexec = [print("%1")]
                strTemp.Format(szShellPrintFmt, (LPCTSTR)strFileTypeId,
                    (LPCTSTR)szDDEExec);
                _AfxDeleteRegKey(strTemp);

                // path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
                strTemp.Format(szShellPrintToFmt, (LPCTSTR)strFileTypeId,
                    (LPCTSTR)szDDEExec);
                _AfxDeleteRegKey(strTemp);
            }

            // path\shell\open\command = path filename
            strTemp.Format(szShellOpenFmt, (LPCTSTR)strFileTypeId,
                (LPCTSTR)szCommand);
            _AfxDeleteRegKey(strTemp);

            // path\shell\print\command = path /p filename
            strTemp.Format(szShellPrintFmt, (LPCTSTR)strFileTypeId,
                (LPCTSTR)szCommand);
            _AfxDeleteRegKey(strTemp);

            // path\shell\printto\command = path /pt filename printer driver port
            strTemp.Format(szShellPrintToFmt, (LPCTSTR)strFileTypeId,
                (LPCTSTR)szCommand);
            _AfxDeleteRegKey(strTemp);

            pTemplate->GetDocString(strMultiFilterExt, CDocTemplate::filterExt);

            // MultiExt: strMultiFilterExt is formatted as: ".ext1;.ext2;.ext3".
            if (!strMultiFilterExt.IsEmpty())
            {
                CString strFilterExt;
                // for all extensions in the filter
                for (int i=0; AfxExtractSubString(strFilterExt, strMultiFilterExt, i, ';'); i++)
                {
                    ASSERT(strFilterExt[0] == '.');

                    LONG lSize = _MAX_PATH * 2;
                    LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
                        strTemp.GetBuffer(lSize), &lSize);
                    strTemp.ReleaseBuffer();

                    if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
                        strTemp == strFileTypeId)
                    {
                        strTemp.Format(szShellNewFmt, (LPCTSTR)strFilterExt);
                        _AfxDeleteRegKey(strTemp);

                        // no association for that suffix
                        _AfxDeleteRegKey(strFilterExt);
                    }
                }   // for all extensions in the filter
            }   // if (!strMultiFilterExt.IsEmpty())
        }   // if enough info to register
    }   // for all templates in the template list
}

void CmyDocmanager::OnFileNew()
{
	if (m_templateList.IsEmpty())
	{
		TRACE0("Error: no document templates registered with CWinApp.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}

	CDocTemplate* pTemplate;
	POSITION pz;
	pz=m_templateList.GetHeadPosition();
	pTemplate= (CDocTemplate*)m_templateList.GetHead();

	for(int i=0;i<m_templateList.GetCount();i++)
	{
		pTemplate=(CDocTemplate*)m_templateList.GetNext(pz);
	}
	//if (m_templateList.GetCount() > 1)
	//{
	//	// more than one document template to choose from
	//	// bring up dialog prompting user
	//	CNewTypeDlg dlg(&m_templateList);
	//	int nID = dlg.DoModal();
	//	if (nID == IDOK)
	//		pTemplate = dlg.m_pSelectedTemplate;
	//	else
	//		return;     // none - cancel operation
	//}

	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	pTemplate->OpenDocumentFile(NULL);
		// if returns NULL, the user has already been alerted
}
//static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
//	                           CDocTemplate* pTemplate, CString* pstrDefaultExt);
//
//// helper for standard commdlg dialogs
//BOOL CmyDocmanager::DoPromptFileName(CString& fileName, UINT nIDSTitle,
//                                     DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
//{
//	CMultiExtFilterFileDialog dlgFile(bOpenFileDialog);
//
//	CString title;
//	VERIFY(title.LoadString(nIDSTitle));
//
//	dlgFile.m_ofn.Flags |= lFlags;
//
//	CString strFilter;
//	CString strDefault;
//	if (pTemplate != NULL)
//	{
//		ASSERT_VALID(pTemplate);
//		AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
//	}
//	else
//	{
//		// do for all doc template
//		POSITION pos = m_templateList.GetHeadPosition();
//		BOOL bFirst = TRUE;
//		while (pos != NULL)
//		{
//			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
//			AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
//				bFirst ? &strDefault : NULL);
//			bFirst = FALSE;
//		}
//	}
//
//	// append the "*.*" all files filter
//	CString allFilter;
//	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
//	strFilter += allFilter;
//	strFilter += (TCHAR)'\0';   // next string please
//#ifndef _MAC
//	strFilter += _T("*.*");
//#else
//	strFilter += _T("****");
//#endif
//	strFilter += (TCHAR)'\0';   // last string
//	dlgFile.m_ofn.nMaxCustFilter++;
//
//	dlgFile.m_ofn.lpstrFilter = strFilter;
//#ifndef _MAC
//	dlgFile.m_ofn.lpstrTitle = title;
//#else
//	dlgFile.m_ofn.lpstrPrompt = title;
//#endif
//	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
//
//    if (!bOpenFileDialog)
//    {
//        // MultiExt: when saving Untitled documents the CDocument::DoSave() function 
//        //           appends the whole filter; we'll only keep the first extension
//        LPTSTR szFileEnd = _tcschr (dlgFile.m_ofn.lpstrFile, (TCHAR)';');
//        if (szFileEnd)
//            *szFileEnd = (TCHAR)'\0';
//    } else {
//        // MultiExt: reset default extension if more than one extension per file type
//        //           so our custom FileDialog will try all extensions in the current filter
//        if (_tcsrchr (dlgFile.m_ofn.lpstrDefExt, (TCHAR)';'))
//            dlgFile.m_ofn.lpstrDefExt = NULL;
//    }
//
//	BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
//    fileName.ReleaseBuffer();
//	return 0;//bResult;
//}




//#include <..\src\mfc\afximpl.h>
//
//#ifndef _MAC
//CDocTemplate::Confidence MatchDocType(CDocTemplate *pTemplate, LPCTSTR lpszPathName,
//                                      CDocument*& rpDocMatch);
//#else
//CDocTemplate::Confidence MatchDocType(CDocTemplate *pTemplate, LPCTSTR lpszPathName,
//                                      DWORD dwFileType, CDocument*& rpDocMatch);
//#endif
//
//
//CDocument* CmyDocmanager::OpenDocumentFile(LPCTSTR lpszFileName)
//{
//	// find the highest confidence
//	POSITION pos = m_templateList.GetHeadPosition();
//	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
//	CDocTemplate* pBestTemplate = NULL;
//	CDocument* pOpenDocument = NULL;
//
//	TCHAR szPath[_MAX_PATH];
//	ASSERT(lstrlen(lpszFileName) < _countof(szPath));
//#ifndef _MAC
//	TCHAR szTemp[_MAX_PATH];
//	if (lpszFileName[0] == '\"')
//		++lpszFileName;
//	lstrcpyn(szTemp, lpszFileName, _MAX_PATH);
//	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
//	if (lpszLast != NULL)
//		*lpszLast = 0;
//	AfxFullPath(szPath, szTemp);
//	TCHAR szLinkName[_MAX_PATH];
//	if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
//		lstrcpy(szPath, szLinkName);
//
//#else
//	lstrcpyn(szPath, lpszFileName, _MAX_PATH);
//	WIN32_FIND_DATA fileData;
//	HANDLE hFind = FindFirstFile(lpszFileName, &fileData);
//	if (hFind != INVALID_HANDLE_VALUE)
//		VERIFY(FindClose(hFind));
//	else
//		fileData.dwFileType = 0;    // won't match any type
//#endif
//
//	while (pos != NULL)
//	{
//		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
//		ASSERT_KINDOF(CDocTemplate, pTemplate);
//
//		CDocTemplate::Confidence match;
//		ASSERT(pOpenDocument == NULL);
//
//        // MultiExt: call our own MatchDocType() function
//#ifndef _MAC
//		match = MatchDocType(pTemplate, szPath, pOpenDocument);
//#else
//		match = MatchDocType(pTemplate, szPath, fileData.dwFileType, pOpenDocument);
//#endif
//
//        if (match > bestMatch)
//		{
//			bestMatch = match;
//			pBestTemplate = pTemplate;
//		}
//		if (match == CDocTemplate::yesAlreadyOpen)
//			break;      // stop here
//	}
//
//	if (pOpenDocument != NULL)
//	{
//		POSITION pos = pOpenDocument->GetFirstViewPosition();
//		if (pos != NULL)
//		{
//			CView* pView = pOpenDocument->GetNextView(pos); // get first one
//			ASSERT_VALID(pView);
//			CFrameWnd* pFrame = pView->GetParentFrame();
//			if (pFrame != NULL)
//				pFrame->ActivateFrame();
//			else
//				TRACE0("Error: Can not find a frame for document to activate.\n");
//			CFrameWnd* pAppFrame;
//			if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
//			{
//				ASSERT_KINDOF(CFrameWnd, pAppFrame);
//				pAppFrame->ActivateFrame();
//			}
//		}
//		else
//		{
//			TRACE0("Error: Can not find a view for document to activate.\n");
//		}
//		return pOpenDocument;
//	}
//
//	if (pBestTemplate == NULL)
//	{
//		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
//		return NULL;
//	}
//
//	return pBestTemplate->OpenDocumentFile(szPath);
//}
//
//
//static void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
//	                           CDocTemplate* pTemplate, CString* pstrDefaultExt)
//{
//	ASSERT_VALID(pTemplate);
//	ASSERT_KINDOF(CDocTemplate, pTemplate);
//
//	CString strMultiFilterExt, strFilterName;
//	if (pTemplate->GetDocString(strMultiFilterExt, CDocTemplate::filterExt) &&
//	 !strMultiFilterExt.IsEmpty() &&
//	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
//	 !strFilterName.IsEmpty())
//	{
//		// a file based document template - add to filter list
//
//    	// add to filter
//		filter += strFilterName;
//		ASSERT(!filter.IsEmpty());  // must have a file type name
//		filter += (TCHAR)'\0';      // next string please
//
//		if (pstrDefaultExt != NULL)
//		{
//    		// set the default extension
//#ifndef _MAC
//			*pstrDefaultExt = ((LPCTSTR)strMultiFilterExt) + 1;  // skip the '.'
//#else
//			*pstrDefaultExt = strMultiFilterExt;
//#endif
//			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
//			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
//		}
//
//        CString strFilterExt;
//        for (int i = 0; AfxExtractSubString(strFilterExt, strMultiFilterExt, i, (TCHAR)';'); i++)
//        {
//            if (i > 0)
//                filter += (TCHAR)';';
//
//#ifndef _MAC
//		    ASSERT(strFilterExt[0] == '.');
//		    filter += (TCHAR)'*';
//#endif
//    	    filter += strFilterExt;
//		    ofn.nMaxCustFilter++;
//        }
//
//        filter += (TCHAR)'\0';  // next string please
//	}
//}
//
//
//// MultiExt: our own MatchDocType() function
//#ifndef _MAC
//CDocTemplate::Confidence MatchDocType(CDocTemplate *pTemplate, LPCTSTR lpszPathName,
//                                      CDocument*& rpDocMatch)
//#else
//CDocTemplate::Confidence MatchDocType(CDocTemplate *pTemplate, LPCTSTR lpszPathName,
//                                      DWORD dwFileType, CDocument*& rpDocMatch)
//#endif
//{
//	ASSERT(lpszPathName != NULL);
//	rpDocMatch = NULL;
//
//	// go through all documents
//	POSITION pos = pTemplate->GetFirstDocPosition();
//	while (pos != NULL)
//	{
//		CDocument* pDoc = pTemplate->GetNextDoc(pos);
//		if (AfxComparePath(pDoc->GetPathName(), lpszPathName))
//		{
//			// already open
//			rpDocMatch = pDoc;
//            return CDocTemplate::yesAlreadyOpen;
//		}
//	}
//
//    // MultiExt: deal w/ multiple extensions in the template filter
//	// see if it matches our default suffix
//	CString strMultiFilterExt;
//	if (pTemplate->GetDocString(strMultiFilterExt, CDocTemplate::filterExt) &&
//	    !strMultiFilterExt.IsEmpty())
//	{
//#ifndef _MAC
//        CString strFilterExt;
//        for (int i = 0; AfxExtractSubString(strFilterExt, strMultiFilterExt, i, (TCHAR)';'); i++)
//        {
//		    // see if extension matches
//		    ASSERT(strFilterExt[0] == '.');
//		    LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
//		    if (lpszDot != NULL && lstrcmpi(lpszDot, strFilterExt) == 0)
//                return CDocTemplate::yesAttemptNative; // extension matches, looks like ours
//        }
//#else
//		// see if file type matches
//		ASSERT(strFilterExt.GetLength() == 4);
//		if (dwFileType == *(DWORD*)(const char*)strFilterExt)
//            return CDocTemplate::yesAttemptNative; // file type matches, looks like ours
//#endif
//	}
//
//	// otherwise we will guess it may work
//	return CDocTemplate::yesAttemptForeign;
//}

IMPLEMENT_DYNAMIC(CmyDocmanager, CDocManager)