// SelectUpImg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelectUpImg.h"
//#include "afxdialogex.h"


// CSelectUpImg �Ի���

IMPLEMENT_DYNAMIC(CSelectUpImg, CDialog)
void PreviewPaintDCBy(CDC& myDC,int xsize,int ysize,int dcx,int dcy,HDC m_hMemDC,int scx,int scy,int srcsizex,int srcsizey,int pmode,int bwpstyle)
{

		::SetStretchBltMode(myDC.m_hDC,HALFTONE);
		::StretchBlt(myDC.m_hDC,dcx,dcy,xsize,ysize,m_hMemDC,scx,scy,srcsizex,srcsizey,pmode);
}
CSelectUpImg::CSelectUpImg(LPCTSTR uploadimgpath,CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CSelectUpImg::IDD, pParent)
{
	m_uploadimgpath=uploadimgpath;
		bNomultLanguage=true;
		wndIdentification=_T("OtherConfigDlg");
		m_clrBkgnd=RGB(180,172,160);

		m_imgxuhao=0;
}

CSelectUpImg::~CSelectUpImg()
{
}

void CSelectUpImg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectUpImg, CDialog)
	ON_WM_CTLCOLOR()
		ON_WM_ERASEBKGND()
		ON_BN_CLICKED(IDOK, &CSelectUpImg::OnBnClickedOk)
		ON_CBN_SELCHANGE(IDC_COMBO1, &CSelectUpImg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CSelectUpImg ��Ϣ�������


void CSelectUpImg::OnBnClickedOk()
{
	m_imgxuhao=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if(m_imgxuhao==((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCount()-1)
	{
	m_imgxuhao=-1;
	}
	OnOK();
}

BOOL CSelectUpImg::OnInitDialog()
{
	CMyBGCDlg::OnInitDialog();

CComboBox* tempcombo=(CComboBox*)GetDlgItem(IDC_COMBO1);
CStringvect vectemp;
			::function::getmytoken(g_groupimgurl,';',vectemp);
			CString fmtstr;
		for(int i=0;i<vectemp.size();i++)
		{
			fmtstr.Format(_T("%d"),i+1);
tempcombo->AddString(fmtstr);
		}
tempcombo->AddString(_T("�����ǣ����ϴ�һ��ͼƬ"));
tempcombo->SetCurSel(tempcombo->GetCount()-1);
	return true;
}
void CSelectUpImg::OnCbnSelchangeCombo1()
{
	m_imgxuhao=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
		CString fmtstr;
	fmtstr.Format(_T("Ⱥ�Զ��ظ������ļ�\\upgroupimage%d.jpg"),m_imgxuhao+1);

	CString newfilename=SaveAs_complete_path(fmtstr);
	
	if(m_imgxuhao==((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCount()-1)
	{
	/*	CRect myrect;
				  CWnd* tempwnd=GetDlgItem(IDC_SMALLPIC);
				   tempwnd->GetClientRect(myrect);
				   tempwnd->ClientToScreen(myrect);
				   ScreenToClient(myrect);
		InvalidateRect(myrect);
	return;*/
newfilename=m_uploadimgpath;
	}

		 CImage m_image;
			HRESULT hResult=m_image.Load(newfilename);
			   if (SUCCEEDED(hResult)) 
			   {
				   CRect myrect;
				  CWnd* tempwnd=GetDlgItem(IDC_SMALLPIC);
				   tempwnd->GetClientRect(myrect);
				 CDC* temphdc=tempwnd->GetDC();		
				
				 PreviewPaintDCBy(*temphdc,myrect.Width(),myrect.Height(),0,0,m_image.GetDC(),0,0,m_image.GetWidth(),m_image.GetHeight(),SRCCOPY,2);
				m_image.ReleaseDC();
					ReleaseDC(temphdc);
				   m_image.Destroy();
			   }
}
