#pragma once
#include "MyBGCDlg.h"
#include "����΢�Ż�����Dlg.h"


// CStandUpDlg �Ի���

class CStandUpDlg : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CStandUpDlg)

public:
	CStandUpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStandUpDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL CStandUpDlg::OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
