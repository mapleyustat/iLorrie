// DlgEvent.cpp : implementation file
//

#include "stdafx.h"
#include "iLorrie.h"
#include "DlgEvent.h"


// CDlgEvent dialog

IMPLEMENT_DYNAMIC(CDlgEvent, CDialog)

CDlgEvent::CDlgEvent( CEvent *pEvent,BOOL bCreate /*= FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEvent::IDD, pParent), m_pEvent(pEvent), m_bCreate(bCreate)
	, m_strTitle(_T(""))
	, m_ctDate(0)
	, m_ctTime(0)
{
	ASSERT(pEvent);
}

CDlgEvent::~CDlgEvent()
{
}

void CDlgEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
	DDX_DateTimeCtrl(pDX, IDC_TMPK_DATE, m_ctDate);
	DDX_DateTimeCtrl(pDX, IDC_TMPK_TIME, m_ctTime);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_EDIT_DETAIL, m_editDetail);
}


BEGIN_MESSAGE_MAP(CDlgEvent, CDialog)
END_MESSAGE_MAP()


// CDlgEvent message handlers

BOOL CDlgEvent::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_bCreate)
	{
		InitCreateControls();
	}else
	{
		InitUpdateControls();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEvent::InitCreateControls()
{
	m_strTitle = "";
	m_ctDate = CTime::GetCurrentTime();
	m_ctTime = m_ctDate;
	m_editDetail.SetWindowText("");

	InitEventTypeCmb();
	m_cmbType.SetCurSel(0);

	UpdateData(FALSE);
}

void CDlgEvent::InitUpdateControls()
{
	m_strTitle = m_pEvent->GetTitle().c_str();
	m_ctDate = m_pEvent->GetEventTime();
	m_editDetail.SetWindowText( m_pEvent->GetDetail().c_str());

	InitEventTypeCmb();
	int nIndex = m_cmbType.FindString( 0, m_pEvent->GetType().c_str());
	m_cmbType.SetCurSel( nIndex);

	UpdateData(FALSE);
}

void CDlgEvent::InitEventTypeCmb()
{
	m_cmbType.Clear();
	vector<string>	vecEventTypes = CConfigData::Instance().GetEventTypes();
	int nCount = CConfigData::Instance().GetEventTypeCnt();
	for ( int i = 0; i < CConfigData::Instance().GetEventTypeCnt(); i++)
	{
		m_cmbType.AddString( vecEventTypes[i].c_str());
	}

}

void CDlgEvent::OnOK()
{
	UpdateData(TRUE);

	CString strTmp;
	if ( !CheckInputValid( strTmp))
	{
		MessageBox( strTmp, "提示", MB_OK | MB_ICONWARNING );
		return;
	}

	CTime ctOcur( m_ctDate.GetYear(), m_ctDate.GetMonth(), m_ctDate.GetDay(), m_ctTime.GetHour(), m_ctTime.GetMinute(), m_ctTime.GetSecond());
	m_pEvent->SetEventTime( ctOcur.GetTime());

	strTmp = "";
	m_cmbType.GetLBText( m_cmbType.GetCurSel(), strTmp);
	m_pEvent->SetType( (LPCTSTR)strTmp);

	m_pEvent->SetTitle( (LPCTSTR)m_strTitle);

	strTmp = "";
	m_editDetail.GetWindowText( strTmp);
	m_pEvent->SetDetail( (LPCTSTR)strTmp);


	CDialog::OnOK();
}

BOOL CDlgEvent::CheckInputValid( CString &strErr)
{
	UpdateData(TRUE);

	// 标题不能为空
	if ( m_strTitle.GetLength() == 0)
	{
		strErr += "标题不能为空";
		return FALSE;
	}


	return TRUE;
}