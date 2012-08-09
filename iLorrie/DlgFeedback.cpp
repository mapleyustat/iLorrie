// DlgFeedback.cpp : implementation file
//

#include "stdafx.h"
#include "iLorrie.h"
#include "DlgFeedback.h"


// CDlgFeedback dialog

IMPLEMENT_DYNAMIC(CDlgFeedback, CDialog)

CDlgFeedback::CDlgFeedback(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFeedback::IDD, pParent)
{

}

CDlgFeedback::~CDlgFeedback()
{
}

void CDlgFeedback::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFeedback, CDialog)
END_MESSAGE_MAP()


// CDlgFeedback message handlers
