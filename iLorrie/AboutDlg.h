/***********************
/*	CAboutDlg.h
/*	about�Ի���
************************/
#pragma once

#include "resource.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

protected:
	DECLARE_MESSAGE_MAP()

	BOOL GetVersionInfo( CString &strVersion);
private:
	// �汾��
	CString m_strVersion;
public:
	virtual BOOL OnInitDialog();
};