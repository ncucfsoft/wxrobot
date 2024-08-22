#pragma once
#include "晨风微信机器人.h"

// CPureAcceptRequestDlg 对话框
class C晨风微信机器人Dlg;
class CPureAcceptRequestDlg
{
	

public:
	CPureAcceptRequestDlg(INT64 qqgroupid,INT64 realgroupid,INT64 requestuin,INT64 qqaccount,const CString& strangerstr,const CString& timesignal,LPCTSTR nickname,LPCTSTR content,int bMale,qqgrouprule tempgrule,CWnd* pParent=NULL);   // 标准构造函数
	virtual ~CPureAcceptRequestDlg();
	INT64 m_realgroupnumber;
	INT64 m_qqgroupid;
	INT64 m_requestuin;
	INT64 m_qqaccount;
	int m_bMale;
	CString m_ticket;
	CString m_stranger;
	CString m_nickname;
	CString m_content;
	qqgrouprule m_grule;
	int m_radiovalue;
	int GetCheckedRadioButton(int radio1,int radio2);
void CheckRadioButton(int radio1,int radio2,int radio3);
	void OnDelSelf();
	BOOL OnInitDialog();
void OnBnClickedOk();
	
};
