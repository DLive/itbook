#pragma once


typedef struct WORDINFO
{
	DWORD      WordLen;   //�ʵĳ��ȣ��������ٱȽ�
	LPTSTR     pszWord;   //�ʵ�ָ��
	COLORREF   Color;   //��ɫֵ
	WORDINFO * NextLink;     //��һ�� WORDINFO �ṹ
}WORDINFO,*pWORDINFO;

class DLsyntaxHightLight
{
public:
	DLsyntaxHightLight(void);
	~DLsyntaxHightLight(void);

	void ReadKeyWord(int keyClassID);

	//����
public:
	DWORD m_SyntaxArray[256];
};
