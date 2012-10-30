#include "StdAfx.h"
#include "DLsyn.h"
#include "Conndb.h"
#include "Ccommon.h"
#include "json/json.h"
#include "DLupToDistancedlg.h"

CDLsyn::CDLsyn(void)
{
}

CDLsyn::~CDLsyn(void)
{

}
bool CDLsyn::upFileToLoc(CStringArray &data){
	CConndb db;
	CString gsql;
	CString key;
	for(int j=0;j<data.GetCount();j++){
		key+=data.GetAt(j);
		key+=_T(",");
	}
	key.Delete(key.GetLength()-1,1);
	gsql.Format(_T("select uniquetag from articles where id IN(%s)"),key);
	db.search(gsql);
	if(db.m_query->eof())
	{
		return false;
	}
	int i=0;
	CString uniquetag,uniquetags;
	_variant_t var;
	while(!db.m_query->eof())
	{
		var=db.m_query->getStringField(_T("uniquetag"));
		if(var.vt!=VT_NULL)
			uniquetag=(LPCSTR)_bstr_t(var);
		uniquetags+=_T("'");
		uniquetags+=uniquetag;
		uniquetags+=_T("',");
		db.m_query->nextRow();
	}
	uniquetags.Delete(uniquetags.GetLength()-1,1);
	CString strFormData=_T("ac=upsome&uniques=");
	strFormData+=uniquetags;
	char *tmpdata=unicodeToUtf8(strFormData.GetBuffer());
	BOOL result = m_pFile->SendRequest(m_strHeaders,(LPVOID)tmpdata,strlen(tmpdata));
	if(result == FALSE) return false;

	free(tmpdata);
	DWORD dwRet;
	m_pFile->QueryInfoStatusCode(dwRet);
	//返回错误没处理！ 哈哈
	CStringA m_strHtml="";
	char szBuff[1024];
	UINT nRead;
	while ((nRead = m_pFile->Read(szBuff,1024))>0)
	{
		m_strHtml+=CStringA(szBuff,nRead);
		memset(szBuff,'\0',1024);
	}
	char *retdata=m_strHtml.GetBuffer();//UTF8ToUnicode(m_strHtml.GetBuffer());//"[{\"term_id\":\"1\",\"name\":\"\u672a\u5206\u7c7b\"},{\"term_id\":\"4\",\"name\":\"\u7f51\u7edc\u5b89\u5168\"}]";//unicodeToUtf8(m_strHtml.GetBuffer());
	Json::Reader reader; 
	Json::Value value; 
	//reader.parse(m_strHtml.GetBuffer(), value);
	if(reader.parse(retdata, value)) 
	{ 
		
		CString sql;
		std::string tmpvalue;
		for(int i=0; i<value.size(); i++) 
		{
			tmpvalue = value[i]["post_title"].asString(); 
			wstring titlename=UTF8ToUnicode(tmpvalue);
			tmpvalue = value[i]["post_content"].asString(); 
			CString contenttext=UTF8ToUnicode(tmpvalue).c_str();
			//tmpvalue = value[i]["name"].asString(); 
			//wstring rclassid=UTF8ToUnicode(tmpvalue);
			//tmpvalue = value[i]["name"].asString(); 
			//wstring data=UTF8ToUnicode(tmpvalue);
			tmpvalue = value[i]["uniquetag"].asString(); 
			wstring uniquetag=UTF8ToUnicode(tmpvalue);
			tmpvalue = value[i]["post_date"].asString(); 
			wstring post_date=UTF8ToUnicode(tmpvalue);
			//tmpvalue = value[i]["name"].asString(); 
			//wstring artype=UTF8ToUnicode(tmpvalue);
			//tmpvalue = value[i]["name"].asString(); 
			//wstring m_keyword=UTF8ToUnicode(tmpvalue);
			contenttext.Replace(_T("'"),_T("''"));

			sql.Format(_T("update articles set title='%s',content='%s',addtime='%s' where uniquetag=%s"),
				titlename.c_str(),contenttext.GetBuffer(),post_date.c_str(),uniquetag.c_str());
			db.excuteSql(sql);
		} 
	} 

	if (dwRet == HTTP_STATUS_OK)
	{
		return true;
	}
	return true;
}
BOOL CDLsyn::upClassFileToLocal(CString cid){
	CConndb db;
	CString gsql;
	gsql.Format(_T("select distancecid from class where id=%s"),cid);
	db.search(gsql);
	if(db.m_query->eof())
		return false;
	int i=0;
	_variant_t var;
	CString rid;
	while(!db.m_query->eof())
	{
		var=db.m_query->getStringField(_T("distancecid"));
		if(var.vt!=VT_NULL)
			rid=(LPCSTR)_bstr_t(var);
		db.m_query->nextRow();
	}
	CString strFormData=_T("ac=upclasstoloc&cid=");
	strFormData+=rid;
	char *tmpdata=unicodeToUtf8(strFormData.GetBuffer());
	BOOL result = m_pFile->SendRequest(m_strHeaders,(LPVOID)tmpdata,strlen(tmpdata));
	if(result == FALSE) return false;
	free(tmpdata);
	DWORD dwRet;
	m_pFile->QueryInfoStatusCode(dwRet);
	//返回错误没处理！ 哈哈
	CStringA m_strHtml="";
	char szBuff[1024];
	UINT nRead;
	while ((nRead = m_pFile->Read(szBuff,1024))>0)
	{
		m_strHtml+=CStringA(szBuff,nRead);
		memset(szBuff,'\0',1024);
	}
	char *retdata=m_strHtml.GetBuffer();//UTF8ToUnicode(m_strHtml.GetBuffer());//"[{\"term_id\":\"1\",\"name\":\"\u672a\u5206\u7c7b\"},{\"term_id\":\"4\",\"name\":\"\u7f51\u7edc\u5b89\u5168\"}]";//unicodeToUtf8(m_strHtml.GetBuffer());
	Json::Reader reader; 
	Json::Value value; 
	//reader.parse(m_strHtml.GetBuffer(), value);
	if(reader.parse(retdata, value)) 
	{ 
		CString sql;
		std::string tmpvalue;
		for(int i=0; i<value.size(); i++) 
		{
			tmpvalue = value[i]["post_title"].asString(); 
			wstring titlename=UTF8ToUnicode(tmpvalue);
			tmpvalue = value[i]["post_content"].asString(); 
			CString contenttext=UTF8ToUnicode(tmpvalue).c_str();
			//tmpvalue = value[i]["name"].asString(); 
			//wstring rclassid=UTF8ToUnicode(tmpvalue);
			//tmpvalue = value[i]["name"].asString(); 
			//wstring data=UTF8ToUnicode(tmpvalue);
			tmpvalue = value[i]["uniquetag"].asString(); 
			wstring uniquetag=UTF8ToUnicode(tmpvalue);
			tmpvalue = value[i]["post_date"].asString(); 
			wstring post_date=UTF8ToUnicode(tmpvalue);
			//tmpvalue = value[i]["name"].asString(); 
			//wstring artype=UTF8ToUnicode(tmpvalue);
			//tmpvalue = value[i]["name"].asString(); 
			//wstring m_keyword=UTF8ToUnicode(tmpvalue);
			contenttext.Replace(_T("'"),_T("''"));
			if(uniquetag.empty()){
				SYSTEMTIME sysTime;
				GetLocalTime(&sysTime);
				CTime m_tTime(sysTime);
				time_t unixTime = m_tTime.GetTime();
				CString utag;
				utag.Format(_T("%d"),unixTime);
				sql.Format(_T("insert into articles (title,content,addtime,classid,artitletype,uniquetag) values ('%s','%s','%s','%s','txt','%s')"),
					titlename.c_str(),contenttext.GetBuffer(),post_date.c_str(),cid,utag);
				//db.excuteSql(sql);
				db.insert(sql);
			}else{
				sql.Format(_T("update articles set title='%s',content='%s',addtime='%s' where uniquetag=%s"),
					titlename.c_str(),contenttext.GetBuffer(),post_date.c_str(),uniquetag.c_str());
				db.excuteSql(sql);
			}
			
		} 
	} 

	if (dwRet == HTTP_STATUS_OK)
	{
		return true;
	}
	return true;
}
bool CDLsyn::upFileToSer(CStringArray &data)
{

	//CStringA abc="[{\"term_id\":\"1\",\"name\":\"\\u672a\\u5206\\u7c7b\"},{\"term_id\":\"4\",\"name\":\"\u7f51\u7edc\u5b89\u5168\"}]";
	//CStringA b;
	//b.Format("%s",abc);
	//char *retdata=abc.GetBuffer();//unicodeToUtf8(m_strHtml.GetBuffer());
	//getClass();
	//return true;
	CDLupToDistancedlg updlg;
	if(updlg.DoModal()!=IDOK){
		return false;
	}
	CStringA status=updlg.m_status;
	//((CComboBox*)updlg.GetDlgItem(IDC_STATUS))->GetCurSel()==0?"draft":"public";
	CConndb db;
	
	CString sqltmp=_T("SELECT a.title,a.distanceID,a.distanceClass,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype,a.uniquetag,a.content FROM articles AS a,class AS c where a.classid=c.id and a.id=%s");
	CStringA titles,id,addtime,content,locaddress,classname,author,diffiNum,filetype,rid,rclass,classid,uniquetag;
	CString sqlcmd;
	//CString upDatatmp=_T("title=%s&distanceID=%s&distanceClass=%s&id=%s&classid=%s&addtime=%s&artitletype=%s&uniquetag=%s&content=%s");
	
	CString upData;
	for(int j=0;j<data.GetCount();j++){
		sqlcmd.Format(sqltmp,data.GetAt(j));
		db.search(sqlcmd);
		_variant_t var;
		try
		{
			if(db.m_query->eof())
			{
				AfxMessageBox(_T("没有相关文章，请重新搜索"));
				return false;
			}
			int i=0;
			Json::Value root;

			while(!db.m_query->eof())
			{
				var=db.m_query->getStringField(_T("title"));
				if(var.vt!=VT_NULL)
					titles=(LPCSTR)_bstr_t(var);
				var=db.m_query->getIntField(_T("id"));
				if(var.vt!=VT_NULL)
					id=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("addtime"));
				if(var.vt!=VT_NULL)
					addtime=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("txtaddress"));
				if(var.vt!=VT_NULL)
					locaddress=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("classname"));
				if(var.vt!=VT_NULL)
					classname=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("distanceID"));
				if(var.vt!=VT_NULL)
					rid=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("uniquetag"));
				if(var.vt!=VT_NULL)
					uniquetag=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("distanceClass"));
				if(var.vt!=VT_NULL)
					rclass=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("content"));
				if(var.vt!=VT_NULL)
					content=(LPCSTR)_bstr_t(var);
				var=db.m_query->getStringField(_T("author"));
				if(var.vt!=VT_NULL)
					author=(LPCSTR)_bstr_t(var);
				else
					author=_T("");
				var=db.m_query->getIntField(_T("difficulty"));
				if(var.vt!=VT_NULL)
					diffiNum=(LPCSTR)_bstr_t(var);
				else
					diffiNum=_T("");
				var=db.m_query->getStringField(_T("artitletype"));
				if(var.vt!=VT_NULL)
					filetype=(LPCSTR)_bstr_t(var);
				else
					filetype=_T("tit");
				
				Json::Value sartitle;
				//CString upDatatmp=_T("title=%s&distanceID=%s&distanceClass=%s&id=%s&classid=%s&addtime=%s&artitletype=%s&uniquetag=%s&content=%s");
				sartitle["title"] = titles.GetBuffer();
				sartitle["distanceID"] = rid.GetBuffer();
				sartitle["distanceClass"] = rclass.GetBuffer();
				sartitle["id"] = id.GetBuffer();
				sartitle["classid"] = classid.GetBuffer();
				sartitle["addtime"] = addtime.GetBuffer();
				sartitle["artitletype"] = filetype.GetBuffer();
				sartitle["uniquetag"] = uniquetag.GetBuffer();
				sartitle["content"] = content.GetBuffer();
				sartitle["status"]=status.GetBuffer();
				root.append(sartitle);

				db.m_query->nextRow();
			}
			//upData.Format(upDatatmp,titles,rid,rclass,id,classid,addtime,filetype,uniquetag,content);
			Json::FastWriter writer;
			//CStringA sdata="adata=";
			CStringA sdata=writer.write(root).c_str();
			sendData(sdata);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
			return false;
		}
	}
	return true;
}
bool CDLsyn::upClassFileToSer(CString str)
{
	CDLupToDistancedlg updlg;
	if(updlg.DoModal()!=IDOK){
		return false;
	}
	CStringA status=updlg.m_status;
	CConndb db;

	CString sqltmp=_T("SELECT a.title,a.distanceID,a.distanceClass,a.id,a.classid,a.addtime,a.txtaddress,c.classname,a.author,a.difficulty,a.artitletype,a.uniquetag,a.content FROM articles AS a,class AS c where a.classid=c.id and a.classid=%s");
	CStringA titles,id,addtime,content,locaddress,classname,author,diffiNum,filetype,rid,rclass,classid,uniquetag;
	CString sqlcmd;
	CString upData;
	sqlcmd.Format(sqltmp,str);
	db.search(sqlcmd);
	Json::Value root;
	_variant_t var;
	while(!db.m_query->eof())
	{	
		var=db.m_query->getStringField(_T("title"));
		if(var.vt!=VT_NULL)
			titles=(LPCSTR)_bstr_t(var);
		var=db.m_query->getIntField(_T("id"));
		if(var.vt!=VT_NULL)
			id=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("addtime"));
		if(var.vt!=VT_NULL)
			addtime=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("txtaddress"));
		if(var.vt!=VT_NULL)
			locaddress=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("classname"));
		if(var.vt!=VT_NULL)
			classname=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("distanceID"));
		if(var.vt!=VT_NULL)
			rid=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("uniquetag"));
		if(var.vt!=VT_NULL)
			uniquetag=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("distanceClass"));
		if(var.vt!=VT_NULL)
			rclass=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("content"));
		if(var.vt!=VT_NULL)
			content=(LPCSTR)_bstr_t(var);
		var=db.m_query->getStringField(_T("author"));
		if(var.vt!=VT_NULL)
			author=(LPCSTR)_bstr_t(var);
		else
			author=_T("");
		var=db.m_query->getIntField(_T("difficulty"));
		if(var.vt!=VT_NULL)
			diffiNum=(LPCSTR)_bstr_t(var);
		else
			diffiNum=_T("");
		var=db.m_query->getStringField(_T("artitletype"));
		if(var.vt!=VT_NULL)
			filetype=(LPCSTR)_bstr_t(var);
		else
			filetype=_T("tit");

		Json::Value sartitle;
		//CString upDatatmp=_T("title=%s&distanceID=%s&distanceClass=%s&id=%s&classid=%s&addtime=%s&artitletype=%s&uniquetag=%s&content=%s");
		sartitle["title"] = titles.GetBuffer();
		sartitle["distanceID"] = rid.GetBuffer();
		sartitle["distanceClass"] = rclass.GetBuffer();
		sartitle["id"] = id.GetBuffer();
		sartitle["classid"] = classid.GetBuffer();
		sartitle["addtime"] = addtime.GetBuffer();
		sartitle["artitletype"] = filetype.GetBuffer();
		sartitle["uniquetag"] = uniquetag.GetBuffer();
		sartitle["content"] = content.GetBuffer();
		sartitle["status"]=status.GetBuffer();
		root.append(sartitle);

		db.m_query->nextRow();
	}
	//upData.Format(upDatatmp,titles,rid,rclass,id,classid,addtime,filetype,uniquetag,content);
	Json::FastWriter writer;
	//CStringA sdata="adata=";
   	CStringA sdata=writer.write(root).c_str();
	sendData(sdata);	
	return true;
}
BOOL CDLsyn::openSession(LPCTSTR host,LPCTSTR object)
{
	CString strHeaders = _T("Content-Type:application/x-www-form-urlencoded\r\n");
	strHeaders += "Accept-Language:zh-CN\r\n";
	strHeaders += "User-Agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; EmbeddedWB 14.52 from: CIBA; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)\r\n";
	strHeaders += "Host:%s\r\n";
	strHeaders += "Connection:Keep-Alive\r\n";
	strHeaders += "Cache-Control:no-cache\r\n";
	m_strHeaders.Format(strHeaders,host);
	//m_strHeaders = _T("Content-Type:application/x-www-form-urlencoded\r\n");
	//m_strHeaders += "Accept-Language:zh-CN\r\n";
	//m_strHeaders += "User-Agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; EmbeddedWB 14.52 from: CIBA; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)\r\n";
	//m_strHeaders += "Host:dlive.sinaapp.com\r\n";
	//m_strHeaders += "Connection:Keep-Alive\r\n";
	//m_strHeaders += "Cache-Control:no-cache\r\n";

	m_session;
	m_pConnection = m_session.GetHttpConnection(host);
	if(m_pConnection == NULL) return FALSE;
	m_pFile = m_pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,object);
	if(m_pFile == NULL) return false;
	return TRUE;
}
BOOL CDLsyn::sendData(CStringA strFormData)
{
	//strFormData.Replace("&","\\&");
	string td=strFormData.GetBuffer();

	string rtd=ansiToUtf8(td);//unicodeToUtf8(strFormData.GetBuffer());
	BOOL result = m_pFile->SendRequest(m_strHeaders,(LPVOID)rtd.c_str(),rtd.length());
	if(result == FALSE) return false;
	//pFile->SendRequestEx(strFormData.GetLength()); 
	//pFile->WriteString(strFormData);
	//pFile->EndRequest();
	//free(tmpdata);
	DWORD dwRet;
	m_pFile->QueryInfoStatusCode(dwRet);

	CString m_strHtml=_T("");
	char szBuff[1024];
	UINT nRead;
	//pFile->Read(szBuff,1023);
	while ((nRead = m_pFile->Read(szBuff,1023))>0)
	{
		m_strHtml+=CString(szBuff,nRead);
	}
	if (dwRet == HTTP_STATUS_OK)
	{
		return true;
	}
	return true;
}
BOOL CDLsyn::getClass(LPVOID val)
{
	CString strFormData=_T("ac=getclass");
	char *tmpdata=unicodeToUtf8(strFormData.GetBuffer());
	BOOL result = m_pFile->SendRequest(m_strHeaders,(LPVOID)tmpdata,strlen(tmpdata));
	if(result == FALSE) return false;

	free(tmpdata);
	DWORD dwRet;
	m_pFile->QueryInfoStatusCode(dwRet);

	CStringA m_strHtml="";
	char szBuff[1024];
	UINT nRead;
	while ((nRead = m_pFile->Read(szBuff,1024))>0)
	{
		m_strHtml+=CStringA(szBuff,nRead);
		memset(szBuff,'\0',1024);
	}
	char *retdata=m_strHtml.GetBuffer();//UTF8ToUnicode(m_strHtml.GetBuffer());//"[{\"term_id\":\"1\",\"name\":\"\u672a\u5206\u7c7b\"},{\"term_id\":\"4\",\"name\":\"\u7f51\u7edc\u5b89\u5168\"}]";//unicodeToUtf8(m_strHtml.GetBuffer());
	Json::Reader reader; 
	//Json::Value value; 
	//reader.parse(m_strHtml.GetBuffer(), value);
	if(reader.parse(retdata,(*(Json::Value *)val)))
	{
		return TRUE;
		//std::string out = value["key1"].asString(); 
		//std::cout << out << std::endl; 
		//const Json::Value arrayObj = value["array"]; 
		//std::string abc;
		//for(int i=0; i<value.size(); i++) 
		//{ 
		//	string abc = value[i]["name"].asString(); 
		//	wstring test=UTF8ToUnicode(abc);
		//	
		//} 
	} 

	if (dwRet == HTTP_STATUS_OK)
	{
		return true;
	}
	return true;
}
bool CDLsyn::PostData(LPCTSTR host, LPCTSTR object, LPCTSTR postdata, LPCTSTR refererlink, int port)
{
	CString strHeaders = _T("Content-Type:application/x-www-form-urlencoded\r\n");
	strHeaders += "Accept-Language:zh-CN\r\n";
	strHeaders += "User-Agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; EmbeddedWB 14.52 from: CIBA; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)\r\n";
	strHeaders += "Host:dlive.sinaapp.com\r\n";
	strHeaders += "Connection:Keep-Alive\r\n";
	strHeaders += "Cache-Control:no-cache\r\n";
	//if(refererlink)
	//{
	//	strHeaders += "\r\nReferer:";
	//	strHeaders += refererlink;
	//}
	CStringA strFormData ="name=Value1&userid=Value2"; 
	CInternetSession session;
	CHttpConnection* pConnection = session.GetHttpConnection(host);
	if(pConnection == NULL) return false;
	CHttpFile* pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,object);
	if(pFile == NULL) return false;
	BOOL result = pFile->SendRequest(strHeaders,(LPVOID)"nnnnnn=tt&name=Value1&userid=Value2",lstrlenA("nnnnnn=tt&name=Value1&userid=Value2"));
	if(result == FALSE) return false;
	//pFile->SendRequestEx(strFormData.GetLength()); 
	//pFile->WriteString(strFormData);
	//pFile->EndRequest();
	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);

	CString m_strHtml=_T("");
	char szBuff[1024];
	UINT nRead;
	pFile->Read(szBuff,1023);
	//while ((nRead = pFile->Read(szBuff,1023))>0)
	//{
	//	m_strHtml+=CString(szBuff,nRead);
	//}
	FILE *fp = fopen("C:\\11.html", "w");
	fwrite(m_strHtml, 1, m_strHtml.GetLength(), fp);
	fclose(fp);

	if (dwRet == HTTP_STATUS_OK)
	{
		return true;
	}


	//LPCSTR lpszAccept[]={"*/*"};// 响应头
	//char *szHeader=new char[3072];
	//memset(szHeader,'\0',3072);
	//char szHeader2[] =
	//{
	//		// 如果提交的是表单,那么这个 MIME 一定要带!
	//		"Content-Type: application/x-www-form-urlencoded\r\n"
	//};

	//strcat(szHeader,"Content-Type:application/x-www-form-urlencoded\r\n");
	//strcat(szHeader,"Accept-Language:zh-CN\r\n");
	////strcat(szHeader,"Referer:http://img.weisanguo.cn/swf/1.0.11120501/WeiSanGuo.swf\r\n");
	////strcat(szHeader,"x-flash-version:11,1,102,55\r\n");
	////strcat(szHeader,"Content-Length:60\r\n");
	////strcat(szHeader,"Accept-Encoding:gzip, deflate\r\n");
	//strcat(szHeader,"User-Agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; QQDownload 538; EmbeddedWB 14.52 from: http://www.bsalsa.com/ EmbeddedWB 14.52; CIBA; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)\r\n");
	//strcat(szHeader,"Host:dlive.sinaapp.com\r\n");
	//strcat(szHeader,"Connection:Keep-Alive\r\n");
	//strcat(szHeader,"Cache-Control:no-cache\r\n");
	////strcat(szHeader,"Cookie:Hm_lvt_b8ba0f59a0da0b7c5da2b95a06d5edf9=1323773260859; Hm_lpvt_b8ba0f59a0da0b7c5da2b95a06d5edf9=1323773260859; saeut=114.93.56.244.1323773244671850; PHPSESSID=65180f5478b3bee238ef9c1b83ebad17\r\n");



 //   // 需要提交的数据就放下面这个变量
 //   char szPostData[] = "name=Value1&userid=Value2";
 //   // 寂寞党可以修改一下 UserAgent 哈哈,我喜欢 Chrome !
 //   HINTERNET hInet = InternetOpenA("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1;)", INTERNET_OPEN_TYPE_DIRECT, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
 //   // 第二个参数是主机的地址
 //   HINTERNET hConn = InternetConnectA(hInet, "dlive.sinaapp.com"/*www.weisanguo.cn*/, INTERNET_DEFAULT_HTTP_PORT,"", "", INTERNET_SERVICE_HTTP, 0, 1);
 //   // 第三个参数是 URL 的路径部分 你懂的,第五个参数是Referer,有些站判断来源地址,修改这里就好啦
 //   HINTERNET hGETs = HttpOpenRequestA(hConn, "POST","/test.php", HTTP_VERSIONA, "http://img.weisanguo.cn/swf/1.0.11120501/WeiSanGuo.swf", lpszAccept, INTERNET_FLAG_DONT_CACHE, 1);
	//												 
	//// 发送HTTP请求
	////HINTERNET hGETs = HttpOpenRequest(hConn, "POST","/ibaby/test.php", HTTP_VERSION, "http://img.weisanguo.cn/swf/1.0.11120501/WeiSanGuo.swf", lpszAccept, INTERNET_FLAG_DONT_CACHE, 1);

 //   BOOL bRequest = HttpSendRequestA(hGETs, szHeader, lstrlenA(szHeader),szPostData,lstrlenA(szPostData));
 //   // 不需要接受回应的忽略下面的东东...
	//char *szBuffer =new char[3072];
 //   DWORD dwByteRead = 0;
 //   // 防止乱码的方法就是建立完变量立即清空
 //   ZeroMemory(szBuffer, 3072);
 //   // 循环读取缓冲区内容直到结束
 //   while (InternetReadFile(hGETs, szBuffer, 3072, &dwByteRead) && dwByteRead > 0){
 //       // 加入结束标记
 //       szBuffer[dwByteRead] = '\0';
 //       // 应该用变长字符串的 比如 AnsiString
 //       //MessageBox(szBuffer, "1312312c",MB_OK);
 //       // 清空缓冲区以备下一次读取
 //       //ZeroMemory(szBuffer, sizeof(szBuffer));
 //   }
 //   // 清理现场
 //   InternetCloseHandle(hGETs);
 //   InternetCloseHandle(hConn);
 //   InternetCloseHandle(hInet);
	return false;
}
