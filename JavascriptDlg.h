#pragma once



// CJavascriptDlg ������ͼ

class CJavascriptDlg : public CHtmlView
{
	DECLARE_DYNCREATE(CJavascriptDlg)

public:
		virtual ~CJavascriptDlg();

public:
	CJavascriptDlg();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

	BOOL CreateFromStatic(CStatic* pStaticWnd, CWnd* pParent);
	int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnNavigateComplete2(LPCTSTR strURL);
};


