
// WinMfcD.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWinMfcDApp:
// �йش����ʵ�֣������ WinMfcD.cpp
//

class CWinMfcDApp : public CWinApp
{
public:
	CWinMfcDApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWinMfcDApp theApp;