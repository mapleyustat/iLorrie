#pragma once


// CDlgFeedback dialog

class CDlgFeedback : public CDialog
{
	DECLARE_DYNAMIC(CDlgFeedback)

public:
	CDlgFeedback(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFeedback();

// Dialog Data
	enum { IDD = IDD_DLG_FEEDBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
