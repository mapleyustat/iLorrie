#pragma once
#include "afxwin.h"


// CDlgEvent dialog
class CEvent;
class CDlgEvent : public CDialog
{
	DECLARE_DYNAMIC(CDlgEvent)

public:
	CDlgEvent( CEvent * pEvent, BOOL bCreate = FALSE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEvent();

// Dialog Data
	enum { IDD = IDD_DLG_EVENT };
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	void	InitCreateControls();	// ��ʼ�������¼�ʱ�Ŀؼ�
	void	InitUpdateControls();	// ��ʼ�������¼�ʱ�Ŀؼ�
	void	InitEventTypeCmb();		// ��ʼ���¼����Ϳؼ�

	BOOL	CheckInputValid( CString &strErr);

private:
	CEvent * m_pEvent;				// ���洫����������ָ��
	BOOL	m_bCreate;

	// ����
	CString m_strTitle;
private:
	// �¼�����ʱ���������
	CTime m_ctDate;
private:
	// �¼�������ʱ����
	CTime m_ctTime;
private:
	// �¼�����
	CComboBox m_cmbType;
private:
	// �¼�����
	CEdit m_editDetail;
};
