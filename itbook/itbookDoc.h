// itbookDoc.h : CitbookDoc 类的接口
//


#pragma once


class CitbookDoc : public CRichEditDoc
{
protected: // 仅从序列化创建
	CitbookDoc();
	DECLARE_DYNCREATE(CitbookDoc)

// 属性
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
	BOOL m_isSavetoDb;//是否存入数据库
	BOOL m_isSavetoLocal;
	
// 操作
public:
	//bool openFromData();
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual CRichEditCntrItem* CreateClientItem(REOBJECT* preo) const;
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace=TRUE );

// 实现
public:
	virtual ~CitbookDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnFileOpen();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};


