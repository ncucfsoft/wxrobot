
// MockHtmlDlgDlg.h : ͷ�ļ�
//
#include "����΢�Ż�����Dlg.h"
#pragma once


// CMockHtmlDlg �Ի���
class CMockHtmlDlg : public CDHtmlDialog
{
// ����
public:
	CMockHtmlDlg(LPCTSTR cururl=NULL,CWnd* pParent = NULL);	// ��׼���캯��
	void ExecuteScript(CString &strScript, CString &strLanguage);
	void CMockHtmlDlg::SuppressScriptError();
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
// �Ի�������
	enum { IDD = IDD_DIALOG6, IDH = 0};//IDR_HTML_MY_DIALOG };//
	CString m_saveCurrentUrl;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	afx_msg void OnBnClickedButton1();
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

	virtual BOOL CanAccessExternal();
// ʵ��
protected:
	HICON m_hIcon;
	void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	STDMETHODIMP GetHostInfo(DOCHOSTUIINFO* pInfo);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void On32794();
		afx_msg void OnClose();
};
