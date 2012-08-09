// iLorrieDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <string>
#include "afxcmn.h"

using namespace std;


class CRecord;

// CiLorrieDlg �Ի���
class CiLorrieDlg : public CDialog
{
// ����
public:
	CiLorrieDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CiLorrieDlg();

// �Ի�������
	enum { IDD = IDD_ILORRIE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void OnCancel();

	BOOL LoadBreifInfo();
	int ShowMsgBox( LPCTSTR strMsg, BOOL bCanCancel = FALSE);		// ��ʾMessageBox��Ϣ
	void InitBreifCmb();			// ���¡�ѡ���¼���ؼ�
	void UpdateRecordControls();	// �����Ҳ�ĸ�����ϸ�ؼ�

	// ��ϸ��¼�ؼ���Ϣ���
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
	// ��¼������Ϣ
	CString m_strRcdSelfInfo;
private:
	CRichEditCtrl m_editEvent;
	CRichEditCtrl m_editFeedback;
	CRichEditCtrl m_editEffect;

	CRecord		* m_pCurRcd;		// �Ի����б���ĵ�ǰ��¼ָ��
};
