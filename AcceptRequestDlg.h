#pragma once
#include "����΢�Ż�����.h"
typedef struct _welcomestruct
{
	CString nickname;
	CString welcomestr;
	CString precardname;
	CString femaleprecardname;
	CString titlestr;
	INT64 pro_m_QQgroupid;
	INT64 cur_uin;
	INT64 otherQQnum;
	BOOL bMale;
	BOOL bTatalSlient;
}welcomestruct;
// CAcceptRequestDlg �Ի���
class C����΢�Ż�����Dlg;
class CAcceptRequestDlg : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CAcceptRequestDlg)

public:
	CAcceptRequestDlg(INT64 QQgroupid,INT64 realgroupid,INT64 requestuin,INT64 QQaccount,INT64 timesignal,LPCTSTR nickname,LPCTSTR content,int bMale,QQgrouprule tempgrule,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAcceptRequestDlg();
	INT64 m_realgroupnumber;
	INT64 m_QQgroupid;
	INT64 m_requestuin;
	INT64 m_QQaccount;
	int m_bMale;
	INT64 m_timesignal;
	CString m_nickname;
	CString m_content;
	QQgrouprule m_grule;
// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcDestroy();
	afx_msg void OnDestroy();
	void OnDelSelf();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	int ncount;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCancel();
};
