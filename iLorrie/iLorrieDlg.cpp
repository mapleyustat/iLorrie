// iLorrieDlg.cpp : ʵ���ļ�
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

// CiLorrieDlg �Ի���

CiLorrieDlg::CiLorrieDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CiLorrieDlg::IDD, pParent)
	, m_strRcdSelfInfo(_T("��¼������Ϣ"))
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


// CiLorrieDlg ��Ϣ�������

BOOL CiLorrieDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// ���ô�������
	SetWindowText( MAIN_DLG_NAME);

	// ��ʼ��
	if ( !LoadBreifInfo())
	{
		return FALSE;
	}

	InitBreifCmb();
	m_cmbQueryTitle.SetCurSel( m_cmbQueryTitle.GetCount() - 1);	// ��ʼ��ѡ�����һ����¼
	OnCbnSelchangeCmbQuery();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CiLorrieDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ��Ӧ���������iLorrie��
void CiLorrieDlg::OnBnClickedBtnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CiLorrieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CiLorrieDlg::OnCancel()
{
	CString strMsg;
// 	strMsg.Format( "ȷ��Ҫ�˳� \'%s\'��", MAIN_DLG_NAME);
// 	if ( ShowMsgBox( strMsg, TRUE) != IDOK)
// 	{
// 		return ;
// 	}

	CDialog::OnCancel();
}

// ��ʾMessageBox
int CiLorrieDlg::ShowMsgBox( LPCTSTR strMsg, BOOL bCanCancel /*= FALSE*/ )
{
	UINT  nType = bCanCancel ? MB_OKCANCEL : MB_OK;
	return MessageBox( strMsg, "��ʾ", nType | MB_ICONWARNING);
}


// ���ؼ�¼ժҪ����������
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

// ���¡�ѡ���¼���ؼ���ʾ���ݣ���Ŀ�󶨼�¼��CreateTime
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

// ��Ӧ������¼��༭��
void CiLorrieDlg::OnBnClickedBtnEvent()
{
	// ��ȡ��ǰ��־�� 
	CDlgEvent dlgEvent();

}

// ��Ӧ�����ִ�б༭��
void CiLorrieDlg::OnBnClickedBtnEffect()
{
	CDlgEffect dlg;
	dlg.DoModal();
}

// ��Ӧ����������༭��
void CiLorrieDlg::OnBnClickedBtnFeedback()
{
	CDlgFeedback dlg;
	dlg.DoModal();
}

// ��Ӧ���������¼�¼��
void CiLorrieDlg::OnBnClickedBtnAdd()
{
	// ����record
	CRecord objRecord;

	// ������Ҫ����event������Ŀ���ͨ���޸����
	// ����event
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

	// ����feed
	CFeedback objFeed;
	objRecord.SetFeedback( objFeed);

	// ����effect
	CEffect objEffect;
	objRecord.SetEffect( objEffect);


	// ���¼�¼������޸�ʱ����޸���
	CTime ctCur = CTime::GetCurrentTime();
	objRecord.SetCreateTime( ctCur.GetTime());
	objRecord.SetSaveTime( ctCur.GetTime());
	objRecord.SetModifier("Will");			// ��Ҫ����û���Ϣ��֤

	// ���浽���ݿ�
	string strErr;
	if ( !CDBInterface::Instance().WriteRecord( objRecord, strErr))
	{
		ShowMsgBox( strErr.c_str());
		return;
	}else
	{
		// log
	}
	
	// ��Ӽ�¼���ڴ�
	m_pCurRcd = CRecordMgr::Instance().AddRecordToEnd( objRecord);
	// ���ժҪ���ڴ�
	CBriefInfo objBrief( &objRecord);
	CRecordMgr::Instance().AddBriefAsc( objBrief);
	
	// ������ʾ
	int nIndex = m_cmbQueryTitle.AddString( objBrief.GetAbstract().c_str());
	m_cmbQueryTitle.SetItemData( nIndex, objBrief.GetCreateTime());
	m_cmbQueryTitle.SetCurSel( nIndex);

	// �����Ҳ�ĸ�����ϸ��¼��
	UpdateRecordControls();

}

// ��Ӧ�����ɾ����¼��
void CiLorrieDlg::OnBnClickedBtnDel()
{
	
}

// ��Ӧѡ���¼�����������
void CiLorrieDlg::OnCbnSelchangeCmbQuery()
{
	// �����ݿ������ѡ����Ŀ��Ӧ�ļ�¼����������
	int nSeq = m_cmbQueryTitle.GetCurSel();
	if ( nSeq == CB_ERR)
	{
		return;
	}

	time_t tCreateTime = m_cmbQueryTitle.GetItemData(nSeq);
	if( tCreateTime == 0)
	{
		ShowMsgBox("��ʼ�����ժҪ��Ϣ���������´򿪳���!");
		return;
	}
	
	// ����ѡ�е���Ŀ��Ӧ������ָ��:
	// ���Ҳ�������˵������Ŀ��δ�����ݿ���أ��������أ������ҵ�ָ�룬��˵��֮ǰ���ع��ˣ�ֱ���ü���
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
	
	// �����Ҳ��¼��ϸ��Ϣ�ؼ���ʾ
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
		m_strRcdSelfInfo = "��¼������Ϣ";
	}else
	{
		CTime ctTime = pRecord->GetSaveTime();
		m_strRcdSelfInfo.Format("    �ü�¼���һ�εı���� %s �� %s ���еġ�", 
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
	strDesc.Format("��    �⣺ %s\r\n����ʱ�䣺 %s\r\n�¼����ͣ� %s\r\n��    �ݣ� %s",
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
	strDesc.Format("�������ǣ� %s\r\n����̬�ȣ� %s\r\n�������ͣ� %s\r\n��    �飺 %s",
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
	strDesc.Format("ִ�����ǣ� %s\r\nִ��Ч���� %s\r\n��    �飺 %s",
		pEffect->GetPerson().c_str(), 
		pEffect->GetLevel().c_str(), 
		pEffect->GetDetail().c_str());

	m_editFeedback.SetWindowText( strDesc);

}
