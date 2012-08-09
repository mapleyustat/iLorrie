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
	void	InitCreateControls();	// 初始化创建事件时的控件
	void	InitUpdateControls();	// 初始化更新事件时的控件
	void	InitEventTypeCmb();		// 初始化事件类型控件

	BOOL	CheckInputValid( CString &strErr);

private:
	CEvent * m_pEvent;				// 保存传进来的数据指针
	BOOL	m_bCreate;

	// 标题
	CString m_strTitle;
private:
	// 事件发生时间的年月日
	CTime m_ctDate;
private:
	// 事件发生的时分秒
	CTime m_ctTime;
private:
	// 事件类型
	CComboBox m_cmbType;
private:
	// 事件详情
	CEdit m_editDetail;
};
