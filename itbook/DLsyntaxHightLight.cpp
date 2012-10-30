#include "StdAfx.h"
#include "DLsyntaxHightLight.h"

#include "Conndb.h"

DLsyntaxHightLight::DLsyntaxHightLight(void)
{
	RtlZeroMemory(m_SyntaxArray,sizeof(m_SyntaxArray));
}

DLsyntaxHightLight::~DLsyntaxHightLight(void)
{
}

void DLsyntaxHightLight::ReadKeyWord(int keyClassID)
{
	CString sqlcmd;
	sqlcmd.Format(_T("select k.keyword,c.color from keyWord k,keyWordColor c where k.colorID=c.ID and k.keyWordClassID=%d"),keyClassID);
	
	CConndb getKeys;
	getKeys.search(sqlcmd);
	

	
	if(!getKeys.m_query->eof())
	{
		_variant_t var;
		_variant_t varColor;
		//CString value;
		LPTSTR		pTmpKey=NULL;
		pWORDINFO	pTmpWordInfo;
		DWORD		keyLen;
		int			keyIndex;
		HANDLE		heapHandle=GetProcessHeap();
		while(!getKeys.m_query->eof())
		{
			var=getKeys.m_query->getStringField(_T("keyword"));
			varColor=getKeys.m_query->getStringField(_T("color"));
			if(var.vt!=VT_NULL)
			{
				keyLen=strlen(_bstr_t(var));
				pTmpKey=(LPTSTR)HeapAlloc(heapHandle,HEAP_ZERO_MEMORY,keyLen+1);
				if(pTmpKey==NULL)
					return;
				lstrcpy((LPTSTR)pTmpKey,(LPTSTR)_bstr_t(var));
				pTmpWordInfo=(pWORDINFO)HeapAlloc(heapHandle,HEAP_ZERO_MEMORY,sizeof(WORDINFO));
				pTmpWordInfo->pszWord=pTmpKey;
				pTmpWordInfo->WordLen=keyLen;
				pTmpWordInfo->Color=varColor.ulVal;

				keyIndex=*pTmpKey;
				if(m_SyntaxArray[keyIndex]==0)
					m_SyntaxArray[keyIndex]=(DWORD)pTmpWordInfo;
				else
				{
					pTmpWordInfo->NextLink=(pWORDINFO)m_SyntaxArray[keyIndex];
					m_SyntaxArray[keyIndex]=(DWORD)pTmpWordInfo;
					
				}
					
			}
			
		}
		getKeys.m_query->nextRow();
	}


}