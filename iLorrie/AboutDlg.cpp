#include "stdafx.h"
#include "AboutDlg.h"
#include "PubDefine.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
	, m_strVersion(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VER, m_strVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::GetVersionInfo( CString &strVersion)
{
	// 加载FileVersion
#pragma   comment(lib, "Version.lib ")
	char   szFullPath[MAX_PATH]; 
	DWORD   dwVerInfoSize   =   0; 
	DWORD   dwVerHnd; 
	VS_FIXEDFILEINFO   *   pFileInfo;
	GetModuleFileName(NULL,  szFullPath,   sizeof(szFullPath)); 
	dwVerInfoSize   =   GetFileVersionInfoSize( szFullPath,   &dwVerHnd); 
	if   ( !dwVerInfoSize)
	{
		return FALSE;
	}else
	{ 
		//   If   we   were   able   to   get   the   information,   process   it: 
		HANDLE     hMem; 
		LPVOID     lpvMem; 
		unsigned   int   uInfoSize   =   0; 

		hMem   =   GlobalAlloc(GMEM_MOVEABLE,   dwVerInfoSize); 
		lpvMem   =   GlobalLock(hMem); 
		GetFileVersionInfo( szFullPath,   dwVerHnd,   dwVerInfoSize,   lpvMem); 

		VerQueryValue(lpvMem,  "\\",   (void**)&pFileInfo,   &uInfoSize); 

		WORD   nProdVersion[4] = {0}; 

		//   Product   version   from   the   FILEVERSION   of   the   version   info   resource   
		nProdVersion[0]   =   HIWORD(pFileInfo-> dwProductVersionMS);   
		nProdVersion[1]   =   LOWORD(pFileInfo-> dwProductVersionMS); 
		nProdVersion[2]   =   HIWORD(pFileInfo-> dwProductVersionLS); 
		nProdVersion[3]   =   LOWORD(pFileInfo-> dwProductVersionLS);   

		strVersion.Format(_T( "iLorrie ( 版本：%d.%d.%d.%d ) "),nProdVersion[0], 
			nProdVersion[1],nProdVersion[2],nProdVersion[3]); 

		GlobalUnlock(hMem); 
		GlobalFree(hMem); 						
	}

	// 设置版本信息
	strVersion += "\n\nCopyright (C) 2012 .THE GRAPES";
	strVersion += "\n\nContact: grapeandlorrie@gmail.com";

	return TRUE;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTmp;
	GetVersionInfo( m_strVersion);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
