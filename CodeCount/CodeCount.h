// CodeCount.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCodeCountApp:
// �йش����ʵ�֣������ CodeCount.cpp
//

class CCodeCountApp : public CWinApp
{
public:
	CCodeCountApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCodeCountApp theApp;