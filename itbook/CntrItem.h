// CntrItem.h : CitbookCntrItem 类的接口
//

#pragma once

class CitbookDoc;
class CitbookView;

class CitbookCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CitbookCntrItem)

// 构造函数
public:
	CitbookCntrItem(REOBJECT* preo = NULL, CitbookDoc* pContainer = NULL);
		// 注意: 允许 pContainer 为 NULL 以启用 IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE 要求类具有带零
		//  参数的构造函数。OLE 项通常是用
		//  非 NULL 文档指针构造的

// 属性
public:
	CitbookDoc* GetDocument()
		{ return reinterpret_cast<CitbookDoc*>(CRichEditCntrItem::GetDocument()); }
	CitbookView* GetActiveView()
		{ return reinterpret_cast<CitbookView*>(CRichEditCntrItem::GetActiveView()); }

	public:
	protected:

// 实现
public:
	~CitbookCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

