// CntrItem.cpp : CitbookCntrItem 类的实现
//

#include "stdafx.h"
#include "itbook.h"

#include "itbookDoc.h"
#include "itbookView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CitbookCntrItem 的实现

IMPLEMENT_SERIAL(CitbookCntrItem, CRichEditCntrItem, 0)

CitbookCntrItem::CitbookCntrItem(REOBJECT* preo, CitbookDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: 在此添加一次性构造代码
}

CitbookCntrItem::~CitbookCntrItem()
{
	// TODO: 在此处添加清理代码
}


// CitbookCntrItem 诊断

#ifdef _DEBUG
void CitbookCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CitbookCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

