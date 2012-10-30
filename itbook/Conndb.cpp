#include "StdAfx.h"
#include "Conndb.h"
#include "Ccommon.h"
CConndb::CConndb(void)
{
	//m_pConnection.CreateInstance(__uuidof(Connection));
	//try
	//{
	//	CString dbUrl;
	//	Ccommon db;
	//	db.getExeUrl(&dbUrl);
	//	dbUrl+=_T("data\\notebook.mdb");
	//	//dbUrl=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")+dbUrl;  MSDAORA
	//	dbUrl=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")+dbUrl;
	//	m_pConnection->Open(dbUrl.AllocSysString(),"","",adModeUnknown);
	//}
	//catch(_com_error e)
	//{
	//	AfxMessageBox(_T("数据库连接失败，请确认数据库是否在当前路径下"));
	//}
	//sqlite
	//CString Url;
	//Ccommon dba;
	//dba.getExeUrl(&Url);
	//Url+=_T("data\\notebook.sqlite");
	//sqlite3 *db=NULL;
	//int ret=0;
	//LPCSTR	tUrl;
	//tUrl=unicodeToAnsi(Url.GetBuffer());
	//ret=sqlite3_open(tUrl,&db);
	//delete tUrl;
	//if(ret!=SQLITE_OK)
	//{
	//}
	//createDataBase(db);
	//AfxMessageBox(_T("success"));
	m_db=new CppSQLite3DB();
	m_db->open(getAppDbPath());
	m_query=NULL;
}

CConndb::~CConndb(void)
{
	//if(m_pConnection->State)
	//{
	//	/*if(m_pRecordset->State)
	//		m_pRecordset->Close();*/
	//	m_pConnection->Close();
	//	m_pConnection=NULL;
	//

	//}
	if(m_query){
		m_query->finalize();
		delete m_query;
	}
	if(m_db){
		m_db->close();
		delete m_db;
	}
}
//执行sql命令
int CConndb::insert(CString &comText)
{
	if(m_db->execDML(comText.GetBuffer()))
		return TRUE;
	//MessageBox(NULL,_T("执行出错"),NULL,0);
	return FALSE;
/*
	_variant_t RecordsAffected;
	try{
		m_pConnection->Execute( (_bstr_t)comText,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	TRACE("影响条数:%d",RecordsAffected);
	return (int)RecordsAffected;*/

	
}
int CConndb::m_geteffectnum(CString &cmdsql)
{
	//_variant_t RecordsAffected;
	int num;
	//try{
	//	m_pRecordset=m_pConnection->Execute( (_bstr_t)cmdsql,&RecordsAffected,adCmdText);
	//}
	/*catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	if(!m_pRecordset->BOF)
	{
		_variant_t var;
		CString id;
		m_pRecordset->MoveFirst();
		var=m_pRecordset->GetCollect("id");
		if(var.vt!=VT_NULL)
			id=(LPCSTR)_bstr_t(var);
		num=_wtoi(id.GetBuffer());
		id.ReleaseBuffer();

	}
	else
	{
		num=-1;
	}
	TRACE("影响条数:%d",RecordsAffected);
	*/
	AfxMessageBox(_T("oh no"));
	return num;
}

void CConndb::search(CString& sqlcmd)
{
	//_variant_t RecordsAffected;
	////CString cmdsql;
	//
	//try
	//{
	//	m_pRecordset=m_pConnection->Execute((_bstr_t)sqlcmd,&RecordsAffected,adCmdText);
	//}
	//catch(_com_error e)
	//{
	//	AfxMessageBox(e.ErrorMessage());
	//}
	if(m_query)
		delete m_query;
	m_query=new CppSQLite3Query();
	*m_query=m_db->execQuery(sqlcmd.GetBuffer());
}

void CConndb::getarticle(CString &cmdsql)
{
	//_variant_t RecordsAffected;
	try
	{
		if(m_query)
			delete m_query;
		m_query=new CppSQLite3Query();
		*m_query=m_db->execQuery(cmdsql.GetBuffer());
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	//AfxMessageBox(_T("haha"),0,0);
}
// 删除记录
bool CConndb::deleteRecord(CString no)
{
	/*_variant_t RecordsAffected;
	CString sqlcmd;
	sqlcmd.Format(_T("delete from articles where id=%s"),no);
	try
	{		
		m_pConnection->Execute((_bstr_t)sqlcmd,&RecordsAffected,adCmdText);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
	}
	if((int)RecordsAffected>=1)
		return true;
	else
		return false;*/
	AfxMessageBox(_T("deleteneed"));
	return true;
}
BOOL CConndb::excuteSql(CString &cmdsql)
{
	try
	{
		//m_pRecordset=m_pConnection->Execute((_bstr_t)cmdsql,&RecordsAffecteds,adCmdText); //delete
		return m_db->execDML(cmdsql.GetBuffer());
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
		return FALSE;
	}
}

//返回影响行数
int CConndb::excuteSql2(CString &cmdsql)
{
	try
	{
		return m_db->execDML(cmdsql.GetBuffer());
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
		return -1;
	}
}
int CConndb::getCount(CString &cmdsql){
	try
	{
		return m_db->execScalar(cmdsql.GetBuffer());
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
		return -1;
	}
}
int CConndb::setPageInfo(CString &sqlcmd,int pageSize)
{
		///*int begin=sqlcmd.Find(_T("from"));
		//if(begin==-1)
		//	AfxMessageBox(_T("查询出错"));
		//total.Format(_T("select count(*) %s"),sqlcmd.Mid(begin));*/
	AfxMessageBox(_T("setpageinfo2"));
	//_variant_t RecordsAffected;
	//m_pRecordset=m_pConnection->Execute((_bstr_t)sqlcmd,&RecordsAffected,adCmdText);
	//m_pRecordset->MoveFirst();
	//int count;
	//if(!m_pRecordset->adoEOF)
	//	count=m_pRecordset->GetCollect(_variant_t((long)0)).intVal;
	//int m_paperCount=count/pageSize;
	//if(count%pageSize!=0)
	//	m_paperCount++;
	//m_pRecordset->Close();
	////m_pRecordset.Release();
	//return m_paperCount;
	return 0;

}

/*
Are you sure you want to perform the following operation(s):
Create Table "main"."articles"
SQL:
CREATE  TABLE "main"."articles" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , "title" VARCHAR(100), "content" TEXT, "classid" INTEGER, "addtime" DATETIME, "txtaddress" TEXT, "artitletype" TEXT, "author" VARCHAR(50), "keyword" VARCHAR(200), "difficulty" INTEGER, "issenddistance" INTEGER, "distanceID" INTEGER, "distanceClass" INTEGER)

Are you sure you want to perform the following operation(s):
Create Table "main"."class"
SQL:
CREATE  TABLE "main"."class" ("id" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , "classname" VARCHAR(50), "parentid" INTEGER, "lv" INTEGER, "distancecid" INTEGER, "issenddistance" INTEGER)
*/