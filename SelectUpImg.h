#pragma once
#include "MyBGCDlg.h"
#include "����΢�Ż�����Dlg.h"

// CSelectUpImg �Ի���

class CSelectUpImg : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CSelectUpImg)

public:
	CSelectUpImg(LPCTSTR uploadimgpath,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectUpImg();
	int m_imgxuhao;
	CString m_uploadimgpath;
// �Ի�������
	enum { IDD = IDD_DIALOG13 };
virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();
};
