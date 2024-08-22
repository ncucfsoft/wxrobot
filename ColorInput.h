#pragma once
#include "PictureEx.h"

// CColorInput �Ի���

class CColorInput : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CColorInput)

public:
CColorInput(LPCTSTR captchaurl,CWnd* pParent = NULL); 
	CColorInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CColorInput();
DWORD colorcode;
int fontsize;
CString m_captcha;
char *m_rtvalue;
UINT m_img_size;
CPictureEx m_mypic;
// �Ի�������
	enum { IDD = IDD_DIALOG3 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_captchastr;
	CImage m_image;
	HICON m_hIcon;
	afx_msg void OnBnClickedOk();
	void OnPaint();
		afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	void DrawPic();
	LRESULT onTakeCapcha(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
