// QR2CodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "晨风微信机器人.h"
#include "QR2CodeDlg.h"
#include "zbase64.h"
//#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CQR2CodeDlg, CDialog)

extern BOOL retryloginflag2;
BOOL exitgetwebthreadflag=false;
HWND qr2codehwnd = NULL;
CEvent getwebevent;
UINT progetweb(LPVOID lp);

CQR2CodeDlg::CQR2CodeDlg(CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CQR2CodeDlg::IDD, pParent)
{
	fontsize=9;
	colorcode=0x00fc0606;//蓝色
m_rtvalue=NULL;
m_bstopdrawflag=false;
m_img_size=0;
m_validstate=true;
m_lastrtcode = -1;
exitgetwebthreadflag=false;
}

CQR2CodeDlg::~CQR2CodeDlg()
{
	
	if(m_rtvalue)
	{
	delete []m_rtvalue;
	m_rtvalue=NULL;
	}

}

void CQR2CodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
const int WM_TypeCapcha=WM_USER+1258;

BEGIN_MESSAGE_MAP(CQR2CodeDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_NEWWEBRESULT, onWebNewResult)
	ON_BN_CLICKED(IDOK, &CQR2CodeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CQR2CodeDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CQR2CodeDlg 消息处理程序
LRESULT CQR2CodeDlg::onWebNewResult(WPARAM wParam, LPARAM lParam)
{
	char* imagebytebuf = (char*)wParam;

	int recode = GetWebBufWindowCode((char*)imagebytebuf);
	m_jumpurl = (CString)GetWebBufUUID((char*)imagebytebuf, ".redirect_uri");
	//m_jumpurl = (char*)imagebytebuf;
	if (recode == 201)
	{
		CStringA imgstr = GetWebBufUUID((char*)imagebytebuf, ".userAvatar");
		int posb = imgstr.Find("base64,");
		CStringA pureimgstr = imgstr.Mid(posb + strlen("base64,"));
		int outlen = 0;
		ZBase64 base641;
		string imguser = base641.Decode(pureimgstr.GetBuffer(0), pureimgstr.GetLength(), outlen);

		if (!m_image.IsNull())
		{
			m_image.Detach();
			
		}
		if (imguser.size())
		{
			GetCImageFromByte(m_image, (byte*)imguser.data(), imguser.size());

			//m_myrect.SetRect(m_myrect.left, m_myrect.top, m_myrect.left + m_image.GetWidth(), m_myrect.top + m_image.GetHeight());



			InvalidateRect(m_myrect);
			m_validstate = true;
		}
		//window.userAvatar = 'data:img/jpg;base64,/9j/4AAQ';
	}

	if (0)
		MessageBox(m_jumpurl, 0, 0);
	//delete[]imagebytebuf;

	if (m_jumpurl.Find(_T("https://")) != -1)
		m_lastrtcode= 0;
	else if (recode == 201)
		m_lastrtcode= 4;
	else if (recode == 408)
		m_lastrtcode= 2;
	else
		m_lastrtcode= 3;

	return m_lastrtcode;
}
void CQR2CodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	
	}
	else
	{

		if(m_bstopdrawflag)
		{
		//CDC* pDC = this->GetDC();
		CPaintDC dc(this);
 TCHAR buffer[32] = _T("二维码已失效，请点击刷新");
 dc.SetBkMode(TRANSPARENT);
 dc.Rectangle(m_myrect);
 dc.TextOut(m_myrect.left, m_myrect.top, buffer, lstrlen(buffer));
 m_lastrtcode = -1;
 m_bstopdrawflag=false;

 	CMyBGCDlg::OnPaint();
		}
		else
		{
		CPaintDC dc(this);
		
	
		if (!m_image.IsNull())
		{
			if (m_image.GetWidth()>200)
			m_image.Draw(dc, m_myrect.left, m_myrect.top, m_image.GetWidth() / 2, m_image.GetHeight() / 2, 0, 0, m_image.GetWidth(), m_image.GetHeight());
			else
				m_image.Draw(dc, m_myrect.left, m_myrect.top, m_image.GetWidth() , m_image.GetHeight() , 0, 0, m_image.GetWidth(), m_image.GetHeight());
		}
			CMyBGCDlg::OnPaint();
		}
	}
}

extern float GetRandomTimeStr(CString &str);
//window.QRLogin.code = 200; window.QRLogin.uuid = "ofOxT_ZGxQ==";

//window.code=201;window.userAvatar = '
//window.code=200;
int GetWebBufWindowCode(char* buf)
{
	int rtcode = 0;
	CStringA rtbufstr = buf;
	int pos = rtbufstr.Find(".code");
	if (pos != -1)
	{
		rtbufstr = rtbufstr.Mid(pos + strlen(".code"));
		rtbufstr.TrimLeft(" =");
		rtcode = atoi(rtbufstr);
	}
	return rtcode;
}
CStringA GetWebBufUUID(char* buf,const char *targetstr)
{
	CStringA rtcode;
	CStringA rtbufstr = buf;
	int pos = rtbufstr.Find(targetstr);
	if (pos != -1)
	{
		rtbufstr = rtbufstr.Mid(pos + strlen(targetstr));
		rtbufstr.TrimLeft(" =\"'");
		int pos2=rtbufstr.Find("\"");
		if (pos2 != -1)
		{
			rtcode = rtbufstr.Left(pos2);
		}
		else
		{
pos2=rtbufstr.Find("'");
		if (pos2 != -1)
		{
			rtcode = rtbufstr.Left(pos2);
		}

		}

	}
	return rtcode;
}
extern int getrtime();
void CQR2CodeDlg::LoadOneQRImage()
{
	CString urldownstr;
		CString realhostname=_T("login.wx2.qq.com");
		CString processpath;
		CString randtimestr;
GetIntRandomTimeStr(randtimestr);
processpath.Format(_T("/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%%3A%%2F%%2Fwx2.qq.com%%2Fcgi-bin%%2Fmmwebwx-bin%%2Fwebwxnewloginpage&fun=new&lang=zh_CN&_=%s"),randtimestr);
//https://login.wx2.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx2.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=zh_CN&_=1489646611370 HTTP/1.1

		//urldownstr.Format(_T("https://ssl.captcha.qq.com%s"),processpath);
		//::function::DownloadHttpFile(urldownstr,(byte*)m_rtvalue,m_img_size);
		//MySendWeb2(realhost,processpath,(byte*)m_rtvalue,NULL,m_img_size,NULL);
m_img_size=0;
RtCStringSendWeb(realhostname,processpath,NULL,(byte*)m_rtvalue,NULL,m_img_size,true);
	//	MySendWeb(realhostname,processpath,(byte*)m_rtvalue,0,m_img_size,true);
		int recode = GetWebBufWindowCode(m_rtvalue);
		CStringA uuid = GetWebBufUUID(m_rtvalue,".uuid");
		m_uuid = (CString)uuid;
		//https://login.weixin.qq.com/qrcode/ofOxT_ZGxQ==
realhostname=_T("login.weixin.qq.com");
processpath.Format(_T("/qrcode/%s"), m_uuid);
m_img_size=0;
RtCStringSendWeb(realhostname,processpath,NULL,(byte*)m_rtvalue,NULL,m_img_size,true);
//MySendWeb(realhostname,processpath,(byte*)m_rtvalue,0,m_img_size,true);
if (!m_image.IsNull())
{
	m_image.Detach();
	
}
		m_bstopdrawflag=false;
			if(retryloginflag2)
	{
	
		SetWindowText(_T("之前的登录失败了，需要重新扫描一次才能登录上！"));
	}
			else
SetWindowText(_T("微信机器人必须用手机扫描二维码才能登录，请用手机扫二维码进行登录"));

CPoint pt1(0,0);
::ClientToScreen(GetSafeHwnd(),&pt1);
static int ncount=0;
if(!ncount)
{
	ncount=1;
	SetCursorPos(pt1.x,pt1.y);
}
if(GetCImageFromByte(m_image,(byte*)m_rtvalue,m_img_size))		
if (m_img_size)
{
	static BOOL onceflag = 0;
	if (onceflag == 0)
	{
		onceflag = 1;
		m_myrect.SetRect(m_myrect.left, m_myrect.top, m_myrect.left + m_image.GetWidth() / 2, m_myrect.top + m_image.GetHeight() / 2);

	}

	InvalidateRect(m_myrect);
	m_validstate = true;
}
			

}

BOOL CQR2CodeDlg::OnInitDialog()
{
	this->ModifyStyleEx(0,WS_EX_APPWINDOW);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CMyBGCDlg::OnInitDialog();


		GetDlgItem(IDC_2code)->GetWindowRect(m_myrect);
		//GetDlgItem(IDC_yanzhengpic)->GetClientRect(myrect);
//ClientToScreen(myrect);
		ScreenToClient(m_myrect);
	//m_mypic.Create(NULL,WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE,myrect,this);
	//m_mypic.ShowWindow(true);

	SetIcon(m_hIcon, TRUE);
		m_img_size=0;
	m_rtvalue=new char[1024*1024];
LoadOneQRImage();
qr2codehwnd = GetSafeHwnd();
AfxBeginThread(progetweb, &m_uuid);

		SetTimer(1,1000,NULL);
	//CClientDC dc(this);

 //m_image.Draw(dc, myrect.left, myrect.top, m_image.GetWidth(), m_image.GetHeight(), 0, 0, m_image.GetWidth(), m_image.GetHeight());

 	 // int tempstyle=2;
	 // m_mypic.Draw(tempstyle,-1);
 return true;
}

UINT progetweb(LPVOID lp)
{
	CString *puuid = (CString*)lp;
	CString localpuuid=*puuid;
	UINT rtcode = 0;

CString realhostname=_T("login.wx2.qq.com");
		CString skeyvalue;
				
				
	CString processpath;
byte *imagebytebuf;
imagebytebuf=new byte[1024*512];	
UINT ddlen=0;
//https://login.wx2.qq.com/cgi-bin/mmwebwx-bin/login?loginicon=true&uuid=ofOxT_ZGxQ==&tip=0&r=707024749&_=1489646611372
//	processpath.Format(_T("/ptqrlogin?webqq_type=10&remember_uin=1&login2qq=1&aid=501004106&u1=http%%3A%%2F%%2Fw.qq.com%%2Fproxy.html%%3Flogin2qq%%3D1%%26webqq_type%%3D10&pttype=1&ptredirect=0&ptlang=2052&daid=164&from_ui=1&pttype=1&dumy=&fp=loginerroralert&action=0-0-2095&mibao_css=m_webqq&t=undefined&g=1&js_type=0&js_ver=10141&login_sig=&pt_randsalt=0"));
	//processpath.Format(_T("/login?u=%s&p=%s&verifycode=%s&webqq_type=10&remember_uin=1&login2qq=1&aid=1003903&u1=http%%3A%%2F%%2Fweb.qq.com%%2Floginproxy.html%%3Flogin2qq%%3D1%%26webqq_type%%3D10&h=1&pttype=1&ptredirect=0&ptlang=2052&daid=164&from_ui=1&dumy=&fp=loginerroralert&action=1-16-24797&mibao_css=m_webqq&t=1&g=1&js_type=0&js_ver=10037&login_sig=3w7ek7oQbOzdtCFW92wOacUgeAdjIs1WhLwIVjn-sz4PAGVrnuRdSPPLmf9hRnQl&pt_randsalt=0&pt_vcode_v1=0&pt_verifysession_v1=%s"),m_ownqqnumber,pstr,m_verifycode,fvsession);
CString randtimestr;

while (1)
{
	WaitForSingleObject(getwebevent,INFINITE);

		if (exitgetwebthreadflag)
			break;
	GetIntRandomTimeStr(randtimestr);
	processpath.Format(_T("/cgi-bin/mmwebwx-bin/login?loginicon=true&uuid=%s&tip=0&r=%d&_=%s"), localpuuid, getrtime(), randtimestr);
	ddlen=0;
	RtCStringSendWeb(realhostname,processpath,NULL,(byte*)imagebytebuf,NULL,ddlen,true);

	imagebytebuf[ddlen]=0;
	//MySendWeb(realhostname, processpath, imagebytebuf, 0, ddlen, true);

	::SendMessage(qr2codehwnd, WM_NEWWEBRESULT, (WPARAM)imagebytebuf, 0);
}
delete []imagebytebuf;
	return rtcode;
}
int CQR2CodeDlg::TestLoginState()
{
	
	//CString str222;
//::function::utf8_wchar((char*)imagebytebuf,str222);
	return 0;
}

void CQR2CodeDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1)
	{
		int rtstate = m_lastrtcode;// TestLoginState();

	if(rtstate==0)
	{
	OnOK();

	exitgetwebthreadflag = true;
	getwebevent.SetEvent();

	}
	else
	{
		
getwebevent.SetEvent();
		if (rtstate == 2)
		{
			if (m_validstate)
			{
				//m_bstopdrawflag = true;
				//m_validstate = false;
				if (!m_image.IsNull())
				{
					m_image.Detach();
					m_img_size = 0;
				}
				InvalidateRect(m_myrect);

				
				SetWindowText(_T("扫码超时，二维码已失效，软件将自动刷新"));
				SetDlgItemText(IDC_STATICnotice,_T("扫码超时，之前二维码已失效，软件已自动刷新了一个最新的二维码，扫码之后请耐心等待十秒！"));
				LoadOneQRImage();
				getwebevent.SetEvent();
				m_lastrtcode=-1;
			}
			//m_image.Load();
		}
		else if (rtstate == 4)
		{

			SetWindowText(_T("二维码已扫描，请在手机上点“确认登录”"));

			//m_image.Load();
		}
	}
	
	}
	
	
	CDialog::OnTimer(nIDEvent);
}

void CQR2CodeDlg::OnBnClickedOk()
{
	
	
	OnOK();
}


void CQR2CodeDlg::OnBnClickedCancel()
{
	
	EndDialog(1000);
}


void CQR2CodeDlg::OnClose()
{
		int rtstate=TestLoginState();
		if(rtstate==0)
		EndDialog(IDOK);
		else
		EndDialog(1000);
		//OnOK();
	CMyBGCDlg::OnClose();
}


BOOL CQR2CodeDlg::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message ==WM_MOUSEMOVE))
{
HCURSOR hcur;
POINT pt=pMsg->pt;
ScreenToClient(&pt);


//CWnd *pwnd=ChildWindowFromPoint(pt);
//int nid=pwnd->GetDlgCtrlID();
if(m_myrect.PtInRect(pt))
{

hcur=AfxGetApp()->LoadStandardCursor(IDC_HAND);
::SetCursor(hcur);

}
else
{
hcur=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
::SetCursor(hcur);
}
	}
	else if((pMsg->message ==WM_LBUTTONDOWN))
{
HCURSOR hcur;
POINT pt=pMsg->pt;
ScreenToClient(&pt);


//CWnd *pwnd=ChildWindowFromPoint(pt);
//int nid=pwnd->GetDlgCtrlID();
if(m_myrect.PtInRect(pt))
{

LoadOneQRImage();
getwebevent.SetEvent();

}
else
{

}
	}

	return CMyBGCDlg::PreTranslateMessage(pMsg);
}
