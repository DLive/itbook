#pragma once

// DLsyntaxHighlight ����Ŀ��


typedef struct WORDINFO
{
 DWORD      WordLen;   //�ʵĳ��ȣ��������ٱȽ�
 LPTSTR     pszWord;   //�ʵ�ָ��
 COLORREF   Color;   //��ɫֵ
 WORDINFO * NextLink;     //��һ�� WORDINFO �ṹ
}WORDINFO;


class DLsyntaxHighlight : public CObject
{
public:
	DLsyntaxHighlight();
	virtual ~DLsyntaxHighlight();

	void readKeyWord();

//����
public:
	DWORD CppSyntaxArray[256];
};


