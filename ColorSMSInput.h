#pragma once

// CColorSMSInput �Ի���

class CColorSMSInput : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CColorSMSInput)

public:
CColorSMSInput(LPCTSTR captchaurl,CWnd* pParent = NULL); 
	CColorSMSInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CColorSMSInput();

	enum { IDD = IDD_DIALOG15 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_smsstr;
	HICON m_hIcon;
	afx_msg void OnBnClickedOk();
	void OnPaint();
	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
