// iLorrieDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "iLorrie.h"
#include "iLorrieDlg.h"
#include "DlgEvent.h"
#include "DlgEffect.h"
#include "DlgFeedback.h"
#include "DBInterface.h"
#include "RecordData.h"
#include "AboutDlg.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CiLorrieDlg 对话框

CiLorrieDlg::CiLorrieDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CiLorrieDlg::IDD, pParent)
	, m_strRcdSelfInfo(_T("记录自身信息"))
	, m_pCurRcd(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AfxInitRichEdit2();
}


CiLorrieDlg::~CiLorrieDlg()
{
}

void CiLorrieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_QUERY, m_cmbQueryTitle);
	DDX_Text(pDX, IDC_STATIC_RECINFO, m_strRcdSelfInfo);
	DDX_Control(pDX, IDC_EDIT_EVENT, m_editEvent);
	DDX_Control(pDX, IDC_EDIT_FEEDBACK, m_editFeedback);
	DDX_Control(pDX, IDC_EDIT_EFFECT, m_editEffect);
}

BEGIN_MESSAGE_MAP(CiLorrieDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_EVENT, &CiLorrieDlg::OnBnClickedBtnEvent)
	ON_BN_CLICKED(IDC_BTN_FEEDBACK, &CiLorrieDlg::OnBnClickedBtnFeedback)
	ON_BN_CLICKED(IDC_BTN_EFFECT, &CiLorrieDlg::OnBnClickedBtnEffect)
	ON_BN_CLICKED(IDC_BTN_ABOUT, &CiLorrieDlg::OnBnClickedBtnAbout)
	ON_BN_CLICKED(IDC_BTN_ADD, &CiLorrieDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CiLorrieDlg::OnBnClickedBtnDel)
	ON_CBN_SELCHANGE(IDC_CMB_QUERY, &CiLorrieDlg::OnCbnSelchangeCmbQuery)
END_MESSAGE_MAP()


// CiLorrieDlg 消息处理程序

BOOL CiLorrieDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// 设置窗口名称
	SetWindowText( MAIN_DLG_NAME);

	// 初始化
	if ( !LoadBreifInfo())
	{
		return FALSE;
	}

	InitBreifCmb();
	m_cmbQueryTitle.SetCurSel( m_cmbQueryTitle.GetCount() - 1);	// 初始化选中最后一条记录
	OnCbnSelchangeCmbQuery();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CiLorrieDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CiLorrieDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 响应点击“关于iLorrie”
void CiLorrieDlg::OnBnClickedBtnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CiLorrieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CiLorrieDlg::OnCancel()
{
	CString strMsg;
// 	strMsg.Format( "确定要退出 \'%s\'？", MAIN_DLG_NAME);
// 	if ( ShowMsgBox( strMsg, TRUE) != IDOK)
// 	{
// 		return ;
// 	}

	CDialog::OnCancel();
}

// 显示MessageBox
int CiLorrieDlg::ShowMsgBox( LPCTSTR strMsg, BOOL bCanCancel /*= FALSE*/ )
{
	UINT  nType = bCanCancel ? MB_OKCANCEL : MB_OK;
	return MessageBox( strMsg, "提示", nType | MB_ICONWARNING);
}


// 加载记录摘要到数据中心
BOOL CiLorrieDlg::LoadBreifInfo()
{
	vector<CBriefInfo> vecBrief;
	string strErr;
	if ( CDBInterface::Instance().ReadAllRecordAbstract( vecBrief, strErr) == false)
	{
		ShowMsgBox( strErr.c_str());
		return FALSE;
	}else
	{
		for ( int i = 0; i < vecBrief.size(); i++)
		{
			CRecordMgr::Instance().AddBriefAsc( vecBrief[i]);
		}
		return TRUE;
	}
}

// 更新“选择记录”控件显示内容，条目绑定记录的CreateTime
void CiLorrieDlg::InitBreifCmb()
{
	m_cmbQueryTitle.Clear();
	for ( int i = 0; i < (int)CRecordMgr::Instance().GetBriefCnt(); i++)
	{
		CBriefInfo *pBriefItem = CRecordMgr::Instance().GetBrief(i);
		ASSERT( pBriefItem);
		int nIndex = m_cmbQueryTitle.AddString( pBriefItem->GetAbstract().c_str());
		m_cmbQueryTitle.SetItemData( nIndex, pBriefItem->GetCreateTime());
	}
}

// 响应点击“事件编辑”
void CiLorrieDlg::OnBnClickedBtnEvent()
{
	// 获取当前日志的 
	CDlgEvent dlgEvent();

}

// 响应点击“执行编辑”
void CiLorrieDlg::OnBnClickedBtnEffect()
{
	CDlgEffect dlg;
	dlg.DoModal();
}

// 响应点击“反馈编辑”
void CiLorrieDlg::OnBnClickedBtnFeedback()
{
	CDlgFeedback dlg;
	dlg.DoModal();
}

// 响应点击“添加新记录”
void CiLorrieDlg::OnBnClickedBtnAdd()
{
	// 创建record
	CRecord objRecord;

	// 至少需要创建event，其余的可以通过修改添加
	// 创建event
	CEvent objEvent;
	CDlgEvent dlgEvent( &objEvent, TRUE);
	if ( dlgEvent.DoModal() != IDOK)
	{
		return;
	}else
	{
		objRecord.SetEvent( objEvent);
		objRecord.SetEventValid();
	}

	// 创建feed
	CFeedback objFeed;
	objRecord.SetFeedback( objFeed);

	// 创建effect
	CEffect objEffect;
	objRecord.SetEffect( objEffect);


	// 更新记录的最后修改时间和修改人
	CTime ctCur = CTime::GetCurrentTime();
	objRecord.SetCreateTime( ctCur.GetTime());
	objRecord.SetSaveTime( ctCur.GetTime());
	objRecord.SetModifier("Will");			// 需要添加用户信息认证

	// 保存到数据库
	string strErr;
	if ( !CDBInterface::Instance().WriteRecord( objRecord, strErr))
	{
		ShowMsgBox( strErr.c_str());
		return;
	}else
	{
		// log
	}
	
	// 添加记录到内存
	m_pCurRcd = CRecordMgr::Instance().AddRecordToEnd( objRecord);
	// 添加摘要到内存
	CBriefInfo objBrief( &objRecord);
	CRecordMgr::Instance().AddBriefAsc( objBrief);
	
	// 更新显示
	int nIndex = m_cmbQueryTitle.AddString( objBrief.GetAbstract().c_str());
	m_cmbQueryTitle.SetItemData( nIndex, objBrief.GetCreateTime());
	m_cmbQueryTitle.SetCurSel( nIndex);

	// 更新右侧的各个详细记录框
	UpdateRecordControls();

}

// 响应点击“删除记录”
void CiLorrieDlg::OnBnClickedBtnDel()
{
	
}

// 响应选择事件标题下拉框
void CiLorrieDlg::OnCbnSelchangeCmbQuery()
{
	// 从数据库加载所选的条目对应的记录到数据中心
	int nSeq = m_cmbQueryTitle.GetCurSel();
	if ( nSeq == CB_ERR)
	{
		return;
	}

	time_t tCreateTime = m_cmbQueryTitle.GetItemData(nSeq);
	if( tCreateTime == 0)
	{
		ShowMsgBox("初始化添加摘要信息出错，请重新打开程序!");
		return;
	}
	
	// 查找选中的条目对应的数据指针:
	// 若找不到，则说明该条目还未从数据库加载，立即加载；若能找到指针，则说明之前加载过了，直接用即可
	m_pCurRcd = CRecordMgr::Instance().GetRecordByTime( tCreateTime);
	if ( m_pCurRcd == NULL)
	{
		CRecord objRecord;
		string strErr;
		if ( !CDBInterface::Instance().ReadRecord( tCreateTime, objRecord, strErr))
		{
			ShowMsgBox( strErr.c_str());
			return;
		}else
		{
			m_pCurRcd = CRecordMgr::Instance().AddRecordToEnd( objRecord);
			ASSERT(m_pCurRcd);
		}
	}
	
	// 更新右侧记录详细信息控件显示
	UpdateRecordControls();	
}

void CiLorrieDlg::UpdateRecordControls()
{
	RcdSelfInfoDesc( m_pCurRcd);
	EventDesc( m_pCurRcd);
	FeedbackDesc( m_pCurRcd);
	EffectDesc( m_pCurRcd);
}

void CiLorrieDlg::RcdSelfInfoDesc( CRecord *pRecord )
{
	if ( pRecord == NULL)
	{
		m_strRcdSelfInfo = "记录自身信息";
	}else
	{
		CTime ctTime = pRecord->GetSaveTime();
		m_strRcdSelfInfo.Format("    该记录最近一次的变更是 %s 在 %s 进行的。", 
			pRecord->GetModifier().c_str(), ctTime.Format("%Y-%m-%d %H:%M:%S"));
	}

	UpdateData( FALSE);
}

void CiLorrieDlg::EventDesc( CRecord *pRecord )
{
	if ( pRecord == NULL)
	{
		return;
	}

	if ( !pRecord->IsEventValid())
	{
		m_editEvent.SetWindowText("");;
		return;
	}

	CEvent *pEvent = pRecord->GetEventPt();
	ASSERT( pEvent);

	CString strDesc;
	CTime ctTime = pEvent->GetEventTime();
	strDesc.Format("标    题： %s\r\n发生时间： %s\r\n事件类型： %s\r\n内    容： %s",
		pEvent->GetTitle().c_str(), 
		ctTime.Format("%Y-%m-%d %H:%M:%S"), 
		pEvent->GetType().c_str(), 
		pEvent->GetDetail().c_str());

	m_editEvent.SetWindowText( strDesc);
//	m_editEvent.SetBackgroundColor( FALSE, EDIT_BKCLR_NORMAL);
}

void CiLorrieDlg::FeedbackDesc( CRecord *pRecord )
{
	if ( pRecord == NULL)
	{
		return;
	}

	if ( !pRecord->IsFeedbackValid())
	{
		m_editEffect.SetWindowText("");;
		return;
	}

	CFeedback *pFeed = pRecord->GetFeedbackPt();
	ASSERT( pFeed);

	CString strDesc;
	strDesc.Format("反馈主角： %s\r\n反馈态度： %s\r\n建议类型： %s\r\n详    情： %s",
		pFeed->GetPerson().c_str(), 
		pFeed->GetAttitude().c_str(), 
		pFeed->GetType().c_str(), 
		pFeed->GetDetail().c_str());

	m_editFeedback.SetWindowText( strDesc);

}

void CiLorrieDlg::EffectDesc( CRecord *pRecord )
{
	if ( pRecord == NULL)
	{
		return;
	}

	if ( !pRecord->IsEffectValid())
	{
		m_editEffect.SetWindowText("");;
		return;
	}

	CEffect *pEffect = pRecord->GetEffectPt();
	ASSERT( pEffect);

	CString strDesc;
	strDesc.Format("执行主角： %s\r\n执行效果： %s\r\n详    情： %s",
		pEffect->GetPerson().c_str(), 
		pEffect->GetLevel().c_str(), 
		pEffect->GetDetail().c_str());

	m_editFeedback.SetWindowText( strDesc);

}
