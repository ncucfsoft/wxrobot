#pragma once
#include "MyBGCDlg.h"
#include "����΢�Ż�����Dlg.h"
// CFontInfSetDlg �Ի���

class CFontInfSetDlg : public CMyBGCDlg
{
	DECLARE_DYNAMIC(CFontInfSetDlg)

public:
	CFontInfSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFontInfSetDlg();
CMySortListCtrl m_list1;
CImageList m_imList;
// �Ի�������
	enum { IDD = IDD_DIALOG7 };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
virtual BOOL OnInitDialog();
void Insert2List(COLORREF color1);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEdit6();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton4();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck2();
};
