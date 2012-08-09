// iLorrie.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "iLorrie.h"
#include "iLorrieDlg.h"
#include "DBInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CiLorrieApp

BEGIN_MESSAGE_MAP(CiLorrieApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CiLorrieApp 构造

CiLorrieApp::CiLorrieApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CiLorrieApp 对象

CiLorrieApp theApp;


// CiLorrieApp 初始化

BOOL CiLorrieApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("The iLorrie Software."));

//	_CrtSetBreakAlloc(1353);

	// 初始化日志文件
//	CLogger::SetAttributes(APP_NAME);
	// 初始化各个组件
	if ( !InitMyComponents())
	{
		return FALSE;
	}
	
	CiLorrieDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;

}

BOOL CiLorrieApp::InitMyComponents()
{
	// 初始化数据库接口
	CDBInterface::Instance().InitDBName( DB_NAME);

	// 加载本地配置xml文件
	string strErr;
	//char szPath[MAX_PATH+1];
	//GetCurrentDirectory(MAX_PATH, szPath);
	if ( !CConfigData::Initialize( CONFIG_FILE, strErr))
	{
		AfxMessageBox( strErr.c_str());
		return FALSE;
	}else
	{
		return TRUE;
	}
	
}