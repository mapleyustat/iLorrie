// iLorrie.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CiLorrieApp:
// �йش����ʵ�֣������ iLorrie.cpp
//

class CiLorrieApp : public CWinApp
{
public:
	CiLorrieApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

protected:
	BOOL	InitMyComponents();		// ��ʼ�������Զ������

};

extern CiLorrieApp theApp;

