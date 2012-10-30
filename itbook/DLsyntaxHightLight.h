#pragma once


typedef struct WORDINFO
{
	DWORD      WordLen;   //词的长度，用来快速比较
	LPTSTR     pszWord;   //词的指针
	COLORREF   Color;   //颜色值
	WORDINFO * NextLink;     //下一个 WORDINFO 结构
}WORDINFO,*pWORDINFO;

class DLsyntaxHightLight
{
public:
	DLsyntaxHightLight(void);
	~DLsyntaxHightLight(void);

	void ReadKeyWord(int keyClassID);

	//变量
public:
	DWORD m_SyntaxArray[256];
};
