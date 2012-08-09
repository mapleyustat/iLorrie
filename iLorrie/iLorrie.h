// iLorrie.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CiLorrieApp:
// 有关此类的实现，请参阅 iLorrie.cpp
//

class CiLorrieApp : public CWinApp
{
public:
	CiLorrieApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

protected:
	BOOL	InitMyComponents();		// 初始化各个自定义组件

};

extern CiLorrieApp theApp;

