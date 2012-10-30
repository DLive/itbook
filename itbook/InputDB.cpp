// InputDB.cpp : 实现文件
//
#include "stdafx.h"
#include "itbook.h"
#include "InputDB.h"
#include "itbookView.h"
#include "Conndb.h"
#include <string.h>
#include <AtlConv.h>

#include "itbookDoc.h"
#include "txtDoc.h"
// CInputDB
#include "CppSQLite3U.h"

IMPLEMENT_DYNAMIC(CInputDB, CWnd)

CInputDB::CInputDB()
{

}
CInputDB::CInputDB(CDocument *pDoc)
{
	m_pDoc=pDoc;
}

CInputDB::~CInputDB()
{
	m_pDoc=NULL;
}


BEGIN_MESSAGE_MAP(CInputDB, CWnd)
END_MESSAGE_MAP()



// CInputDB 消息处理程序
bool CInputDB::getText(CString pathname,CString content)
{
	CString contenttext=content;
	//if(!pathname.IsEmpty()){
	//	CFile bb;
	//	if(!bb.Open(pathname,CFile::modeRead,NULL))
	//	{
	//		AfxMessageBox(_T("error,file is not find"));
	//	}
	//	byte head[2];
	//	bb.Read(head,2);

	//	if(((head[0]==0xff&&head[1]==0xfe)||(head[0]==0xfe&&head[1]==0xff)))
	//	{
	//		CString contemp;
	//		bb.Seek(2,CFile::begin);
	//		wchar_t* pbuf;
	//		ULONGLONG l=bb.GetLength();
	//		pbuf=new wchar_t[l];
	//		memset(pbuf,'\0',l);
	//		bb.Read(pbuf,l);
	//		pbuf[(l/2)]='\0';
	//		contenttext.Format(_T("%s"),pbuf);
	//		//this->GB2312ToUTF8(contemp,contenttext);
	//		delete []pbuf;
	//	}
	//	else
	//	{
	//		bb.SeekToBegin();
	//		LPSTR pbuf;
	//		ULONGLONG l=bb.GetLength();
	//		pbuf=new char[l+1];
	//		memset(pbuf,'\0',l+1);
	//		bb.Read(pbuf,l);
	//		//ansi to unicode
	//		//size_t len=strlen(pbuf)+1;
	//		int wlen=MultiByteToWideChar(CP_ACP,NULL,pbuf,-1,NULL,NULL);

	//		wchar_t *pwText=new wchar_t[wlen];
	//		if(!pwText)
	//		{
	//			delete []pwText;
	//		}

	//		MultiByteToWideChar(CP_ACP,0,pbuf,-1,pwText,wlen);
	//		contenttext.Format(_T("%s"),pwText);
	//		delete []pwText;
	//		delete []pbuf;
	//	}
	//	bb.Close();
	//}
	//else{
	//	contenttext=content;
	//}
	BOOL m_isFromDb;
	CString m_classid,m_oldPath,m_author,m_keyword,artype,difficulty,uniquetag;
	int m_aid;
	bool m_isTemp;
	//if(type==_T("tit"))
	//{
	//	CitbookDoc *pitbook=(CitbookDoc*)m_pDoc;
	//	m_isFromDb=pitbook->m_isFromDb;
	//	m_classid=pitbook->m_classid;
	//	m_aid=pitbook->m_aid;
	//	m_isTemp=pitbook->m_isTemp;
	//	m_oldPath=pitbook->m_oldPath;
	//	m_author=pitbook->m_author;
	//	m_keyword=pitbook->m_keyword;
	//	difficulty=pitbook->m_difficulty;
	//}
	//else
	//{
	CtxtDoc *pitbook=(CtxtDoc*)m_pDoc;
	m_isFromDb=pitbook->m_isFromDb;
	m_classid=pitbook->m_classid;
	m_aid=pitbook->m_aid;
	m_isTemp=pitbook->m_isTemp;
	m_oldPath=pitbook->m_oldPath;
	m_author=pitbook->m_author;
	m_keyword=pitbook->m_keyword;
	difficulty=pitbook->m_difficulty;
	if(pitbook->m_bRTF==SF_RTF)
		artype=_T("tit");
	else
		artype=_T("txt");
	//}
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CTime m_tTime(sysTime);
	time_t unixTime = m_tTime.GetTime();
	uniquetag.Format(_T("%d"),unixTime);
	//判断是保存，还是另存为
	bool isSave;//true is save. false is save as
	if(pathname.CompareNoCase(m_oldPath)==0)
		isSave=true;
	else
		isSave=false;
	//取得文件名
	int p=pathname.ReverseFind('\\');
	CString titlename;
	if(!pathname.IsEmpty()){
		titlename=pathname.Right(pathname.GetLength()-p-1);
		p=titlename.ReverseFind('.');
		titlename=titlename.Left(p);
	}else{
		titlename=pitbook->m_title;
	}
	//得到时间
	CTime time=CTime::GetCurrentTime();
	CString data=time.Format("%Y-%m-%d");
	//取得路径
	pathname.Replace(_T("'"),_T("''"));
	contenttext.Replace(_T("'"),_T("''"));
	CString addText;
	CConndb add;//sql链接对象

	if(m_isFromDb && isSave==true)
	{
		CString updatasql;
		//如何缓存在temp 则不更新txtaddress
		if(m_isTemp)
			updatasql.Format(_T("update articles set title='%s',content='%s',classid=%s,addtime='%s',artitletype='%s',author='%s',keyword='%s',difficulty=%s where id=%d"),titlename,contenttext,m_classid,data,artype,m_author,m_keyword,difficulty,m_aid);
		else
			updatasql.Format(_T("update articles set title='%s',content='%s',classid=%s,addtime='%s',txtaddress='%s',artitletype='%s',author='%s',keyword='%s',difficulty=%s where id=%d"),titlename,contenttext,m_classid,data,pathname,artype,m_author,m_keyword,difficulty,m_aid);
		if(add.excuteSql2(updatasql)!=-1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		addText.Format(_T("insert into articles (title,content,classid,addtime,txtaddress,artitletype,author,keyword,difficulty,uniquetag) values('%s','%s',%s,'%s','%s','%s','%s','%s',%s,'%s')"),titlename,contenttext,m_classid,data,pathname,artype,m_author,m_keyword,difficulty,uniquetag);
		//得到插入ID


		if(add.excuteSql2(addText)!=-1)
		{
			CString sqlget;
			int insertid;
			sqlget.Format(_T("select max(id) as maxid from articles"));
			add.search(sqlget);
			if(!add.m_query->eof())
			{
				_variant_t var;
				var=add.m_query->getIntField(_T("maxid"));
				if(var.vt!=VT_NULL)
					insertid=var.ulVal;
			}
			if(artype==_T("tit"))
			{
				CitbookDoc *pitbook=(CitbookDoc*)m_pDoc;
				pitbook->m_aid=insertid;
			}
			else
			{
				CtxtDoc *pitbook=(CtxtDoc*)m_pDoc;
				pitbook->m_aid=insertid;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

}

BOOL CInputDB::m_getinfo(CString pathname,CString type)
{
	CString text;
	text.Format(_T("SELECT a.id, a.title,a.classid,a.author,a.keyword,a.difficulty,a.artitletype,c.classname FROM articles AS a, class AS c WHERE a.classid=c.id AND a.txtaddress='%s'"),pathname);
	CConndb setinfo;
	setinfo.getarticle(text);
	
	
	CString title,cid,author,classname,keyword,difficulty;
	difficulty=_T("0");
	int aid;
	if(!setinfo.m_query->eof())
	{
		_variant_t var;
		CString value;
		while(!setinfo.m_query->eof())
		{
			var=setinfo.m_query->getStringField(_T("title"));
			if(var.vt!=VT_NULL)
				title=(LPCSTR)_bstr_t(var);
			var=setinfo.m_query->getIntField(_T("id"));
			if(var.vt!=VT_NULL)
				aid=var.intVal;
			var=setinfo.m_query->getIntField(_T("classid"));
			if(var.vt!=VT_NULL)
				cid=(LPCSTR)_bstr_t(var);
			var=setinfo.m_query->getStringField(_T("classname"));
			if(var.vt!=VT_NULL)
				classname=(LPCSTR)_bstr_t(var);
			var=setinfo.m_query->getStringField(_T("keyword"));
			if(var.vt!=VT_NULL)
				keyword=(LPCSTR)_bstr_t(var);
			var=setinfo.m_query->getStringField(_T("author"));
			if(var.vt!=VT_NULL)
				author=(LPCSTR)_bstr_t(var);
			var=setinfo.m_query->getIntField(_T("difficulty"));
			if(var.vt!=VT_NULL)
				difficulty=(LPCSTR)_bstr_t(var);

			setinfo.m_query->nextRow();
		}
		if(type==_T("tit"))
		{
			CitbookDoc *pitbook=(CitbookDoc*)m_pDoc;
			pitbook->m_nowPath=pathname;
			pitbook->m_isFromDb=true;
			pitbook->m_aid=aid;
			pitbook->m_classid=cid;
			pitbook->m_classname=classname;
			pitbook->m_keyword=keyword;
			pitbook->m_author=author;
			pitbook->m_difficulty=difficulty;
			pitbook->SetModifiedFlag(FALSE);
		}
		else
		{
			CtxtDoc *pitbook=(CtxtDoc*)m_pDoc;
			pitbook->m_nowPath=pathname;
			pitbook->m_isFromDb=true;
			pitbook->m_aid=aid;
			pitbook->m_classid=cid;
			pitbook->m_classname=classname;
			pitbook->m_keyword=keyword;
			pitbook->m_author=author;
			pitbook->m_difficulty=difficulty;
			pitbook->SetModifiedFlag(FALSE);
		}
		
	}

	return TRUE;
}
