#pragma once
#include "MyBGCDlg.h"
#include "����΢�Ż�����Dlg.h"
// COtherConfig �Ի���

class COtherConfig : public CMyBGCDlg
{
	DECLARE_DYNAMIC(COtherConfig)

public:
	COtherConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COtherConfig();
	virtual BOOL OnInitDialog();
		void OnDestroy();
// �Ի�������
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck5();
};
