// itbook.h : itbook 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"  // 主符号
#include "CommResours.h" 

// CitbookApp:
// 有关此类的实现，请参阅 itbook.cpp
//


class CitbookApp : public CWinApp
{
public:
	CitbookApp();
	
	
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNewtitfile();
	//多扩展名

	afx_msg void OnNewtxtfile();
	afx_msg void seltNewfile();
};

extern CitbookApp theApp;