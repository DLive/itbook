#pragma once

// CtxtDoc 文档

class CtxtDoc : public CDocument
{
	DECLARE_DYNCREATE(CtxtDoc)

public:
	CtxtDoc();
	virtual ~CtxtDoc();
//#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
//#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	bool m_isFromDb;//true 为是从数据库取出数据
	int m_aid;  //ID号
	CString m_oldPath;//打开之前保存的路径
	CString m_nowPath;//现在的路存
	CString m_classid; //分类号
	CString m_title; //标题
	CString m_classname; //分类名称
	bool m_isTemp;//true 为缓存
	CString m_author;//作者
	CString m_difficulty;//文章难度
	CString m_artitletype;//文本类型
	CString m_keyword;//关键字
	BOOL m_infoIsFi;//属性信息是否已更改
	BOOL m_isANSI;//ture is ansi;
	BOOL m_isSavetoDb;//是否存入数据库  默认存入
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BOOL selfSaveFile(CString& content,CString path);
	BOOL SaveAsAnsi(LPCTSTR pathname);
	virtual void OnCloseDocument();
};
