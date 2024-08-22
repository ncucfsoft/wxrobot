// ColorInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "����΢�Ż�����.h"
//#include "formatstruct.h"
#include "ColorInput.h"
#ifndef _DEBUG
#include "uuwisecdef.h"
#endif
//#include "afxdialogex.h"


// CColorInput �Ի���
BOOL bLastGifOver=true;
int switchrate=2;
BOOL bNoSpecialEffect=false;
BOOL bShowpwding=0;
IMPLEMENT_DYNAMIC(CColorInput, CDialog)
	CCriticalSection lockbytes;
CString temptextoutword;
CString textoutword;
int bShowBubbles;
int ExitFlag;

CColorInput::CColorInput(CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CColorInput::IDD, pParent)
{
	fontsize=9;
	colorcode=0x00fc0606;//��ɫ
}
CColorInput::CColorInput(LPCTSTR captchaurl,CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CColorInput::IDD, pParent)
{bNomultLanguage=true;
	m_captcha=captchaurl;
	fontsize=9;
	m_rtvalue=NULL;
	colorcode=0x00fc0606;//��ɫ
}
CColorInput::~CColorInput()
{
	lockbytes.Lock();
	if(m_rtvalue)
	{
	delete []m_rtvalue;
	m_rtvalue=NULL;
	}
	lockbytes.Unlock();
}

void CColorInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
const int WM_TypeCapcha=WM_USER+1258;

BEGIN_MESSAGE_MAP(CColorInput, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CColorInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CColorInput::OnBnClickedCancel)
	ON_MESSAGE(WM_TypeCapcha,onTakeCapcha)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CColorInput ��Ϣ�������
LRESULT CColorInput::onTakeCapcha(WPARAM wParam,LPARAM lParam)
{
	BOOL IsOk=(BOOL)wParam;
	TCHAR rtcmcode[32];
	lstrcpy(rtcmcode,(LPCTSTR)lParam);
	  if(IsOk)
  {

SetDlgItemText(IDC_EDIT2,rtcmcode);
	OnBnClickedOk();
  }
 else
 {
	 GetDlgItem(IDC_EDIT2)->EnableWindow(1);  
 }

	return 0;
}
void CColorInput::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	
	}
	else
	{
		CPaintDC dc(this);
		
	
	/*	CRect myrect;
		GetDlgItem(IDC_yanzhengpic)->GetWindowRect(myrect);
		ScreenToClient(myrect);
 m_image.Draw(dc, myrect.left, myrect.top, m_image.GetWidth(), m_image.GetHeight(), 0, 0, m_image.GetWidth(), m_image.GetHeight());
*/
			CMyBGCDlg::OnPaint();
	}
}
void MySendWeb2(LPCTSTR realhostname,LPCTSTR processpath,byte *imagebytebuf,char* sttt,UINT& ddlen,LPCTSTR headstr,TCHAR* getsetcookies)
{
	CHttpConnection* pConnection;	
CInternetSession *p_session;
p_session=new CInternetSession;
	pConnection = p_session->GetHttpConnection(realhostname,(DWORD)INTERNET_FLAG_KEEP_CONNECTION); 

		CString rgfilename=_T("imagebyte");
				CString	strHeaders;
				if(headstr!=NULL&&lstrlen(headstr))
				strHeaders.Format(_T("Accept: */*\r\n%s\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)\r\n"),headstr);
				else
				strHeaders.Format(_T("Accept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)\r\n"));
				
				//strHeaders.Format(_T("Accept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)\r\n"));
				
				
				
			
				//strcpy(pattachm,"replycampus=0&content=�������ԣ�&uin=35376062&ouin=13402429&g_tk=1662176677");
				//ddlen=strlen(pattachm);
				rgfilename=_T("login.html");
				Send2web(pConnection,strHeaders,processpath,sttt,ddlen,(byte*)imagebytebuf,NULL,rgfilename);

	

				delete pConnection;
				delete p_session;
	
}
float GetRandomTimeStr(CString &str)
{
srand(time(NULL));
		long ddd=rand();
			long l = rand() | ((long)rand() << 15) | (((long)rand() & 0x0001) << 30);
			double ff=(double)l/0x7FFFFFFF;
			ff=fabs(ff);
		
			str.Format(_T("%.16f"),(float)ff);
			return ff;
}
void GetIntRandomClientMsgIdStr(CString &timestr2)
{

	SYSTEMTIME st;
	GetLocalTime(&st);
	//get_friend_uin2
	CTime time1(st);
	CString fourrandstr;
	fourrandstr.Format(_T("%04d"),rand());
	fourrandstr=fourrandstr.Left(4);
	timestr2.Format(_T("%I64d%03d%s"),time1.GetTime(),st.wMilliseconds,fourrandstr);
		
}
void GetIntRandomTimeStr(CString &timestr2)
{

	SYSTEMTIME st;
	GetLocalTime(&st);
	//get_friend_uin2
	CTime time1(st);
	timestr2.Format(_T("%I64d%03d"), time1.GetTime(), st.wMilliseconds);
		
}

void CColorInput::DrawPic()
{
	

				SetTimer(95,1000*1000,NULL);
		
BOOL tpb;
		CString strpath=::function::complete_path(_T("captcha.gif"));
  if(tpb=m_mypic.Load(strpath))
  {


	  int tempstyle=2;
	  m_mypic.Draw(tempstyle,-1);

	
  }

	
		
	

}
BOOL CColorInput::OnInitDialog()
{
	this->ModifyStyleEx(0,WS_EX_APPWINDOW);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CMyBGCDlg::OnInitDialog();


	//CRect rect;
	CRect myrect;
		GetDlgItem(IDC_yanzhengpic)->GetWindowRect(myrect);
		//GetDlgItem(IDC_yanzhengpic)->GetClientRect(myrect);
//ClientToScreen(myrect);
		ScreenToClient(myrect);
	m_mypic.Create(NULL,WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE,myrect,this);
	m_mypic.ShowWindow(true);
	screen_xsize=myrect.Width();
screen_ysize=myrect.Height();
	SetIcon(m_hIcon, TRUE);
		m_img_size=0;
		
		{
	/*		if(qq.Viery.size())
			{
			GetCImageFromByte(m_image,(byte*)&qq.Viery[0],qq.Viery.size());
			if(qq.Viery.size())
			{
			m_rtvalue=new char[1024*1024];
			memcpy(m_rtvalue,(byte*)&qq.Viery[0],qq.Viery.size());
			m_img_size=qq.Viery.size();
			}
		CString strpath=::function::complete_path(_T("captcha.gif"));
		CFile file1;
		if(file1.Open(strpath,CFile::modeCreate|CFile::modeWrite))
		{
			if(qq.Viery.size())
			file1.Write((byte*)&qq.Viery[0],qq.Viery.size());
		file1.Close();
		}
			}*/
		//qq.Viery;
		}
		DrawPic();
	  SetTimer(1,100000,NULL);
	  if(!uu_username.IsEmpty())
	  {
	
	  SetTimer(2,5*1000,NULL);
	  }
	  SetWindowText(_T("���qq��¼����������֤�룡"));

	

 return true;
}
BOOL RTbackConfirmcode(byte* buf,UINT len,CString& confirmcode,int decaptchid)
{
	TCHAR bufresult[128]=_T("");

	if(LocalPlugFunction(uu_username,confirmcode))
	{
		if(confirmcode.IsEmpty())
			return false;
		else
			return true;
	}
	else
	{
if(decaptchid==1)
{
	#ifndef _DEBUG
	uu_easyRecognizeBytesW(93631,_T("13062b3c3085413aaf9d008fe08ca941"),uu_username,uu_pwd,(CHAR*)buf,len,1105,bufresult);
	#endif
	
	int resultd=_ttoi(bufresult);
	if(resultd==-1001||resultd==-1002)
	{
	#ifndef _DEBUG
	uu_easyRecognizeBytesW(93631,_T("13062b3c3085413aaf9d008fe08ca941"),uu_username,uu_pwd,(CHAR*)buf,len,1105,bufresult);
	#endif
	resultd=_ttoi(bufresult);
	}
		
	if(resultd<0)
	{
		CString fmtstr;
fmtstr.Format(_T("%s:%s"),bufresult,_T("���ֵ"));
		MessageBox(0,_T("����������ʺ����޻��ֻ������Ʒ�����ʱ�����ã�ֻ���ֶ�������֤�룬����㲻���ֵҲ�����ٿ��������ʾ����ɾ���������ƴ����û����������ļ�.txt���ļ���"),fmtstr,0);
	
		return false;
	}
	else
if(lstrlen(bufresult))
{
	confirmcode=bufresult;
	return true;
}

}
	}
	  return false;
}

UINT GetResultFromDecaptcha(LPVOID lp)
{
	CColorInput* myyzwnd=(CColorInput*)lp;

int decpid=1;
BOOL IsOk=0;
CString rtcmcode;
lockbytes.Lock();
if(myyzwnd->m_rtvalue)
 IsOk=RTbackConfirmcode((byte*)&(*myyzwnd->m_rtvalue),myyzwnd->m_img_size,rtcmcode,decpid);
 lockbytes.Unlock();

 if(myyzwnd->m_hWnd)
  { 
	  if(!ExitFlag)
	  ::SendMessage(myyzwnd->m_hWnd,WM_TypeCapcha,IsOk,(LPARAM)rtcmcode.GetBuffer(0));
	
  }
 

return 0;
}

void CColorInput::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1)
	{
	OnOK();
	}
		if(nIDEvent==2)
	{
		KillTimer(2);
	//		  LONG score=0;
	//#ifndef _DEBUG
	//score=uu_getScoreW(uu_username,uu_pwd);
	//if(score<10)
	//{
	//MessageBox(_T("����������ʺ����޻��֣�ֻ���ֶ�������֤�룬����㲻���ֵҲ�����ٿ��������ʾ����ɾ���������ƴ����û����������ļ�.txt���ļ���"),_T("���ֵ"),0);
	//}
	//#endif
	//if(score>=10)
		AfxBeginThread(GetResultFromDecaptcha,this);

	}
	if(nIDEvent==95)
	{
		

		if(bLastGifOver)
		{

			
	DrawPic();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CColorInput::OnBnClickedOk()
{
	
	GetDlgItemText(IDC_EDIT2,m_captchastr);
	ExitFlag=true;
	OnOK();
}


void CColorInput::OnBnClickedCancel()
{
	ExitFlag=true;
	EndDialog(1000);
}


void CColorInput::OnClose()
{
	ExitFlag=true;
	//
	
		EndDialog(1000);
	
	CMyBGCDlg::OnClose();
}
