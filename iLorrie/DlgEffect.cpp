// DlgEffect.cpp : implementation file
//

#include "stdafx.h"
#include "iLorrie.h"
#include "DlgEffect.h"


// CDlgEffect dialog

IMPLEMENT_DYNAMIC(CDlgEffect, CDialog)

CDlgEffect::CDlgEffect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEffect::IDD, pParent)
{

}

CDlgEffect::~CDlgEffect()
{
}

void CDlgEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffect, CDialog)
END_MESSAGE_MAP()


// CDlgEffect message handlers
