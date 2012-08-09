// iLorrieDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <string>
#include "afxcmn.h"

using namespace std;


class CRecord;

// CiLorrieDlg 对话框
class CiLorrieDlg : public CDialog
{
// 构造
public:
	CiLorrieDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CiLorrieDlg();

// 对话框数据
	enum { IDD = IDD_ILORRIE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void OnCancel();

	BOOL LoadBreifInfo();
	int ShowMsgBox( LPCTSTR strMsg, BOOL bCanCancel = FALSE);		// 显示MessageBox信息
	void InitBreifCmb();			// 更新“选择记录”控件
	void UpdateRecordControls();	// 更新右侧的各个详细控件

	// 详细记录控件信息输出
	void RcdSelfInfoDesc( CRecord *pRecord);
	void EventDesc( CRecord *pRecord);
	void FeedbackDesc( CRecord *pRecord);
	void EffectDesc( CRecord *pRecord);

private:

	afx_msg void OnBnClickedBtnEvent();
	afx_msg void OnBnClickedBtnFeedback();
	afx_msg void OnBnClickedBtnEffect();
	afx_msg void OnBnClickedBtnAbout();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnCbnSelchangeCmbQuery();

private:
	CComboBox m_cmbQueryTitle;
	// 记录自身信息
	CString m_strRcdSelfInfo;
private:
	CRichEditCtrl m_editEvent;
	CRichEditCtrl m_editFeedback;
	CRichEditCtrl m_editEffect;

	CRecord		* m_pCurRcd;		// 对话框中保存的当前记录指针
};
