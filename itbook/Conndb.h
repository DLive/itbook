#pragma once
#include "CppSQLite3U.h"
class CConndb
{
public:
	CConndb(void);
	~CConndb(void);
	
	//方法
	int insert(CString &comText);
	//属性
	//_ConnectionPtr m_pConnection;
	//_RecordsetPtr	m_pRecordset;//结果集
	void search(CString &sqlcmd);
	void getarticle(CString &cmdsql);
	int m_geteffectnum(CString &cmdsql);
	//_variant_t RecordsAffecteds; //影响条数
	BOOL excuteSql(CString &cmdsql);
	int excuteSql2(CString &cmdsql);
	int getCount(CString &cmdsql);
	// 删除记录
	bool deleteRecord(CString no);
	
	CppSQLite3DB *m_db;
	CppSQLite3Query *m_query;
	//分页数据
	//int		m_recodeCount; //总条数
	//int		m_paperCoutn;	//总页数
	//int		m_paperNum;		//每页条数

	int		setPageInfo(CString &sqlcmd,int pageSize);

};
