
// MockHtmlDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MockHtmlDlg.h"
#include <WinSock2.h>

//#include "detours.h"
#include "deflate.h"
#include "utf8.hpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern int m_bkn;
//extern void GetSkey(CString &skey, CString &ptwebqqhash, CString& verifysessionhash);
//extern CString pgv_pvid,p_uin,g_p_skey,g_pt4_token;
//extern void GetqzoneSkey(CString &p_skey,CString &pt4_token);
extern BOOL MySendWeb(LPCTSTR realhostname,LPCTSTR processpath,byte *imagebytebuf,const char* postdata,UINT& ddlen);
//extern int Getg_tkfromskey(const CString& skey, BOOL bBkn);
void wchrto_utf8(LPCWSTR cbuf,std::string& tranlatedbuf)
{
	std::wstring test1=cbuf;

	tranlatedbuf.clear();
	try{
		libtorrent::detail::wchar_utf8(test1.begin(), test1.end(), std::back_inserter(tranlatedbuf));
	}
	catch(...)
	{
	}
}
void chrto_utf8(LPCSTR cbuf,std::string& tranlatedbuf)
{CStringW wcbuf=(CStringW)cbuf;
	wchrto_utf8(wcbuf,tranlatedbuf);
}

void utf8_wchar(const char* utfbuf, CStringW &widestr,int len)
{
 std::string utf8;
 if(len==0)
	 len=strlen(utfbuf);

 utf8.assign(utfbuf,utfbuf+len);
	wstring wide;
	wide.clear();
	try
	{
	libtorrent::detail::utf8_wchar(utf8.begin(), utf8.end(), std::back_inserter(wide));
	widestr=wide.c_str();
	}
catch(...)
{
}
}
void utf8_char(const char* utfbuf, CStringA &astr,int len=0)
{ if(len==0)
	 len=strlen(utfbuf);

	CStringW widestr;
 utf8_wchar(utfbuf,widestr,len);
astr=(CStringA)widestr;
}
void utf8_wchar(const std::string &utf8, CStringW &widestr)
{
	wstring wide;
	wide.clear();
	try
	{
	libtorrent::detail::utf8_wchar(utf8.begin(), utf8.end(), std::back_inserter(wide));
	widestr=wide.c_str();
	}
catch(...)
{
}
}
void utf8_wchar(const std::string &utf8, std::wstring &wide)
{
	wide.clear();
	libtorrent::detail::utf8_wchar(utf8.begin(), utf8.end(), std::back_inserter(wide));
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

CReadAUStdioFile filerec,filebin;
CReadAUStdioFile filesend(false);
int (WINAPI *pSend)(SOCKET s, const char* buf,int len, int flags) = send;
int WINAPI MySend(SOCKET s, const char* buf,int len, int flags);
//int (WINAPI *pRecv)(SOCKET s, __out_bcount_part(len, return) __out_data_source(NETWORK) char* buf,int len, int flags) = recv;
//int WINAPI MyRecv(IN SOCKET s, __out_bcount_part(len, return) __out_data_source(NETWORK) char FAR * buf, IN int len, IN int flags);
int (WINAPI *pRecv)(SOCKET s, __out_bcount_part(len, return) __out_data_source(NETWORK) char* buf,int len, int flags) = recv;
int WINAPI MyRecv(SOCKET s, __out_bcount_part(len, return) __out_data_source(NETWORK) char* buf,int len, int flag);
int (WINAPI * pWSARecv)(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)=WSARecv;
int WINAPI MyWSARecv(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int WINAPI MySend(SOCKET s, const char* buf, int len, int flags)
{
	//FILE *pSendLogFile;
	//  fopen_s(&pSendLogFile, "C:\\SendLog.txt", "a+");
	// 
	//  fwrite(buf,len,1,pSendLogFile);
	////MessageBoxA(NULL,"Error!send","Error in Detours!",MB_OK);
	//  fclose(pSendLogFile);
		
		char bufr[]="Accept-Encoding: gzip, deflate\r\n";
char bufr2[]="Accept-Encoding: kzip, keflate\r\n";

const char *pf=strstr(buf,bufr);
if(pf)
	memcpy((char*)pf,bufr2, strlen(bufr));
	
if(filesend.m_hFile!=CFile::hFileNull)
{
filesend.Write(buf,len);
filesend.Flush();
}
	return pSend(s, buf, len, flags);

}
	const char signbuf[5][32]={"HTTP/1.1 ","Content-Encoding: gzip","Content-Length: ","Transfer-Encoding: chunked","\r\n\r\n"};

void ModifyLength2permanence(char *buf,int& OUT IN add_len)
{
char buftemp[1024];
	LPSTR pos1,pos2,pos3;
	LPSTR curpos=buf;
	
		pos1=StrStrIA(curpos,signbuf[2]);
		if(pos1)
		{
		
			pos3=StrStrIA(pos1,"\r\n");

			if(pos3&&pos3-pos1<30)
			{
				int tt=atoi(pos1+strlen(signbuf[2]));
				char buf1[32],buf2[32];
				itoa(tt,buf1,10);
				itoa(add_len,buf2,10);
add_len=strlen(buf2)-strlen(buf1);
strcpy(buftemp,pos1+strlen(signbuf[2])+strlen(buf1));
strcpy(pos1+strlen(signbuf[2]),buf2);
strcat(buf,buftemp);


			}
			
		}
	pos1=StrStrIA(curpos,signbuf[1]);
		if(pos1)
		{
		
			pos3=StrStrIA(pos1,"\r\n");

			if(pos3&&pos3-pos1<30)
			{
		
memcpy(pos1,"Content-Encoding: kzip",strlen(signbuf[1]));



			}
			
		}

}
void ModifyCookie2permanence(char *buf)
{
	char rpbuf[]="expires=Tue, 29-Dec-2099 16:31:26 GMT;";
	LPSTR pos1,pos2,pos3;
	LPSTR curpos=buf;
	while(1)
	{
		pos1=StrStrIA(curpos,"Set-Cookie:");
		if(pos1)
		{
			pos2=StrStrIA(pos1,"expires=");
			/*if(pos2==NULL)
				pos2=StrStrIA(pos1,"expires:");*/
			pos3=StrStrIA(pos1,"\r\n");

			if(pos2&&pos2<pos3)
			{
				memcpy(pos2,rpbuf,strlen(rpbuf));

			}
			else
				break;

		}
		else
			break;

		curpos=pos3+2;

	}

}
void dealwith(char *buf,int chunk,BOOL& Isnewheader,BOOL& Isgzip,int& body_len,BOOL& Ischunked,char* &headend_pos)
{
char* temppos,*temppos2;
	if(temppos=strstr(buf,signbuf[0]))
	{
		temppos2=strstr(temppos,"\r\n");
		if(temppos2)
			if(temppos2-temppos<100)
				Isnewheader=true;
	}
	if(temppos=strstr(buf,signbuf[1]))
	{
		Isgzip=true;
	}
	if(temppos=strstr(buf,signbuf[2]))
		body_len=atoi(temppos+strlen(signbuf[2]));
	if(temppos=strstr(buf,signbuf[3]))
	{
		Ischunked=true;
	}
	if(temppos=strstr(buf,signbuf[4]))
	{

		headend_pos=temppos+4;
	}





}
char splitbuf[]="=======================================================\r\n";
void SavetoRecF(const char *buf,int len)
{
CString astr;
//buf[rtint]=0;
if(filerec.m_hFile==CFile::hFileNull)
return;
astr=buf;
		//utf8_wchar(buf,astr,len);
//filebin.Write(buf,len);
//filebin.Flush();
		
	filerec.WriteString(astr);
filerec.Flush();
}

int WINAPI MyRecv(IN SOCKET s, __out_bcount_part(len, return) __out_data_source(NETWORK) char * buf, IN int len, IN int flags)
{
	int rtint=pRecv(s,buf,len,flags);
//	if(rtint>0)
//	{
//	/*	CStringA strold=buf;
//	if(strold.Find("9987k.com")!=-1)
//	{
//	strold.Replace("9987k.com","cfxy.me");
//	strold.Replace("win7","win764");
//	strcpy(buf,strold);
//	}*/
//SavetoRecF(buf,rtint);
//	//ModifyCookie2permanence(buf);
//	}
	//dealwith(buf,len);
	return rtint;
}

int WINAPI MyWSARecv(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
int rtint=pWSARecv(s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,lpFlags,lpOverlapped,lpCompletionRoutine);
//if(rtint==0)
//{
//	//int minn=min(lpBuffers->len,*lpNumberOfBytesRecvd);
//	//SavetoRecF(lpBuffers->buf,minn);
//	//CStringA strold=lpBuffers->buf;
//	//strold.Replace("9987k.com","cfxy.me");
//	//strold.Replace("win7","win764");
//	//strcpy(lpBuffers->buf,strold);
//
//	ModifyCookie2permanence(lpBuffers->buf);
//
//}
return rtint;
}




BEGIN_DHTML_EVENT_MAP(CMockHtmlDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()
BEGIN_INTERFACE_MAP(CMockHtmlDlg, CMockHtmlDlg) 
	//INTERFACE_PART(CMockHtmlDlg, IID_IOleCommandTarget, OleCommandTarget)
	//	BEGIN_INTERFACE_PART(CMockHtmlDlg,   IOleCommandTarget)
	//    STDMETHOD(QueryStatus)(const   GUID*,   ULONG,   OLECMD*,   OLECMDTEXT*); 
	//    STDMETHOD(Exec)(const   GUID*,   DWORD,   DWORD,   VARIANTARG*,   VARIANTARG*); 
	//END_INTERFACE_PART(CMockHtmlDlg) 
END_INTERFACE_MAP() 



CMockHtmlDlg::CMockHtmlDlg(LPCTSTR cururl,CWnd* pParent /*=NULL*/)
: CDHtmlDialog(CMockHtmlDlg::IDD, CMockHtmlDlg::IDH, pParent)
{
	m_strCurrentUrl=cururl;
	m_saveCurrentUrl=cururl;
	//m_alreadyskey=false;
	/*LONG error;
	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	error = DetourAttach(&(PVOID&)pWSARecv, MyWSARecv);
	error = DetourTransactionCommit();
	if(NO_ERROR!=error)
	{
		MessageBoxA(NULL,"Error!","Error in Detours!",MB_OK);
	}
	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	error = DetourAttach(&(PVOID&)pRecv, MyRecv);
	error = DetourTransactionCommit();
	if(NO_ERROR!=error)
	{
		MessageBoxA(NULL,"Error!","Error in Detours!",MB_OK);
	}
	
	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	DetourAttach(&(PVOID&)pSend, MySend);
	error = DetourTransactionCommit();
	if(NO_ERROR!=error)
	{
		MessageBoxA(NULL,"Error!","Error in Detours!",MB_OK);
	}

	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	error = DetourAttach(&(PVOID&)pWSARecv, MyWSARecv);
	error = DetourTransactionCommit();
	if(NO_ERROR!=error)
	{
		MessageBoxA(NULL,"Error!","Error in Detours!",MB_OK);
	}
	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());
	error = DetourAttach(&(PVOID&)pRecv, MyRecv);
	error = DetourTransactionCommit();
	if(NO_ERROR!=error)
	{
		MessageBoxA(NULL,"Error!","Error in Detours!",MB_OK);
	}*/
}

void CMockHtmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMockHtmlDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMockHtmlDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CMockHtmlDlg::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);

	SuppressScriptError(); // ���νű�����
}
void CMockHtmlDlg::SuppressScriptError()
{
	// Ҫִ�е����α���ű�
	CString strScript = _T("window.onerror=function myonerror(){return true}");
	CString strLanguage("JavaScript");
	ExecuteScript(strScript, strLanguage);


}
void CMockHtmlDlg::ExecuteScript(CString &strScript, CString &strLanguage)
{
	IHTMLDocument2* pIHtmlDoc = NULL;
	GetDHtmlDocument(&pIHtmlDoc);
	if (!pIHtmlDoc) return;

	IHTMLWindow2* pIhtmlwindow2 = NULL;
	pIHtmlDoc->get_parentWindow(&pIhtmlwindow2);
	if (!pIhtmlwindow2) return;

	BSTR bstrScript = strScript.AllocSysString();
	BSTR bstrLanguage = strLanguage.AllocSysString();
	VARIANT pRet;

	// ע��ű�����ǰҳ��
	pIhtmlwindow2->execScript(bstrScript, bstrLanguage, &pRet);
	::SysFreeString(bstrScript);
	::SysFreeString(bstrLanguage);
	pIhtmlwindow2->Release();
}

// CMockHtmlDlg ��Ϣ�������
void CMockHtmlDlg::OnBnClickedButton1()
{
		GetDlgItemText(IDC_EDIT1,m_strCurrentUrl);
		Navigate(m_strCurrentUrl);
}
BOOL CMockHtmlDlg::CanAccessExternal()
{
	// we trust all com object (haha, you can make virus)
	return TRUE;
}

STDMETHODIMP CMockHtmlDlg::GetHostInfo(DOCHOSTUIINFO* pInfo)
{
	pInfo->dwFlags = DOCHOSTUIFLAG_THEME;
	return S_OK;
}

BOOL CMockHtmlDlg::OnInitDialog()
{
	//put_Silent();
	SetHostFlags(DOCHOSTUIFLAG_NO3DBORDER);
	//filesend.Open(::function::complete_path(_T("log\\http_send.log")),CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	//filerec.Open(::function::complete_path(_T("log\\http_rec.log")),CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
//filebin.Open(::function::complete_path(_T("log\\http_bin.log")),CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);


	CDHtmlDialog::OnInitDialog();
	if(m_saveCurrentUrl.Find(_T("iwencai"))!=-1)
	SetTimer(1,10000,NULL);
//				CString tempskeyvalue,ptwebqqhash,hashstr;
//				GetSkey(tempskeyvalue,ptwebqqhash,hashstr);
//if(tempskeyvalue.IsEmpty())
//{
//SetTimer(2,1000,NULL);
//SetTimer(1,60000,NULL);
//}
//else
//{
//	
//	//Sleep(3000);
//	//m_alreadyskey=true;
//SetTimer(1,30000,NULL);
//}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CMockHtmlDlg::OnTimer(UINT nIDEvent)
{

	if(m_saveCurrentUrl.Find(_T("iwencai"))!=-1)
	{
	OnOK();
	DestroyModeless();
	}
	// if(nIDEvent==2)
	// {
			// CString ptwebqqhash,hashstr;
				// GetSkey(skeyvalue,ptwebqqhash,hashstr);
// if(!skeyvalue.IsEmpty())
// {
	// //Sleep(3000);

		// OnOK();
// }
	// }
	// if(nIDEvent==1)
	// {
		// static BOOL promptflag=0;
		// if(promptflag==0)
		// {
			// promptflag=true;
		// if(IDOK==MessageBox(_T("���Ƿ��Ѿ�����ҳ�еġ����ٵ�¼�������QQ�ŵ���֤������Ѿ����������QQ�ռ䣬���¼�Ѿ���ɣ��Ƿ������Զ��رմ˴��ڻص���������棿�������ȼ�����ɵ�¼QQ�Ĺ��̡�"),_T("�Ƿ�رմ˴����Իص����������"),MB_OKCANCEL))
		// {
		// OnOK();
		// }
		// else
		// promptflag=false;

		// }
	// }
	
	CDialog::OnTimer(nIDEvent);
}

void CMockHtmlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	CDHtmlDialog::OnSysCommand(nID, lParam);

}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMockHtmlDlg::OnPaint()
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

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMockHtmlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CMockHtmlDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CMockHtmlDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

BOOL CMockHtmlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if  (pMsg->message  ==  WM_KEYDOWN  &&  (pMsg->wParam==VK_RETURN|| pMsg->wParam  ==  VK_ESCAPE)) 
	{ 
		TranslateMessage(pMsg); 
		DispatchMessage(pMsg); 
		return TRUE; 
			}


	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

void CMockHtmlDlg::OnDestroy()
{
	

//		CString realhostname;
//CString processpath;
////realhostname=_T("qun.qq.com");
//realhostname=_T("qun.qq.com");
//
//	processpath.Format(_T("/cgi-bin/qunwelcome/myinfo"));
//	m_bkn = Getg_tkfromskey(skeyvalue, true);
//	CStringA propramstr;
//	propramstr.Format("callback=?&bkn=%d",m_bkn);
//UINT ddlen=1024*1024*2;
//		byte *imagebytebuf=new byte[ddlen];
//		memset(imagebytebuf,0,ddlen);
//		ddlen = propramstr.GetLength();
//
//	//	BOOL rtflag = MySendWeb(realhostname, processpath, imagebytebuf, (char*)propramstr.GetBuffer(0), ddlen);
//		delete []imagebytebuf;
	
	//qun.qq.com/cgi-bin/qunwelcome/myinfo?callback=?&bkn=1079869070

		//GetqzoneSkey(g_p_skey,g_pt4_token);
//filebin.Close();
//if(1)//!Isneedlog)
//{
//     DetourTransactionBegin();
//        DetourUpdateThread(GetCurrentThread());
//        DetourDetach(&(PVOID&)pSend, MySend);
//        DetourDetach(&(PVOID&)pWSARecv, MyWSARecv);
//        DetourDetach(&(PVOID&)pRecv, MyRecv);
//        DetourTransactionCommit();
//}
	CDHtmlDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

extern BOOL Multipartpost(const CString& realhostname,const CString& processpath,const CStringvect& columname_com,const CStringvect& columvalue_com,const CString& columname_pic_col,const byte* picsrc,int len,CString& feedbackstr);
void CMockHtmlDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}
void CMockHtmlDlg::On32794()
{

}
