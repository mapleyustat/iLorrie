/***********************
/*	CAboutDlg.h
/*	about对话框
************************/
#pragma once

#include "resource.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	DECLARE_MESSAGE_MAP()

	BOOL GetVersionInfo( CString &strVersion);
private:
	// 版本号
	CString m_strVersion;
public:
	virtual BOOL OnInitDialog();
};