#pragma once

// DLsyntaxHighlight 命令目标


typedef struct WORDINFO
{
 DWORD      WordLen;   //词的长度，用来快速比较
 LPTSTR     pszWord;   //词的指针
 COLORREF   Color;   //颜色值
 WORDINFO * NextLink;     //下一个 WORDINFO 结构
}WORDINFO;


class DLsyntaxHighlight : public CObject
{
public:
	DLsyntaxHighlight();
	virtual ~DLsyntaxHighlight();

	void readKeyWord();

//变量
public:
	DWORD CppSyntaxArray[256];
};


