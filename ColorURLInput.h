#pragma once

// CColorURLInput �Ի���

class CColorURLInput : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CColorURLInput)

public:
CColorURLInput(LPCTSTR captchaurl,CWnd* pParent = NULL); 
	CColorURLInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CColorURLInput();

	enum { IDD = IDD_DIALOG16 };
	
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
