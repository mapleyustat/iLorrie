#pragma once


// CDlgEffect dialog

class CDlgEffect : public CDialog
{
	DECLARE_DYNAMIC(CDlgEffect)

public:
	CDlgEffect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEffect();

// Dialog Data
	enum { IDD = IDD_DLG_EFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
