// AutoReplyRule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoReplyRule.h"
//#include "afxdialogex.h"
#include "ColorInput.h"

// CAutoReplyRule �Ի���
CString g_verify_msg;

IMPLEMENT_DYNAMIC(CAutoReplyRule, CDialog)
void SearchqqGroupByUin(LPCTSTR qqnumber,CString& mytoken)
{
CString realhostname;
CString processpath;
CString codestr;

CString url;
		CString timestr;
	GetRandomTimeStr(timestr);
url.Format(_T("/getimage?aid=1003901&%s"),timestr);
	CString ptwebqqhash,verifysessionhash;

	CColorInput dlg(url);
	int rtflag=dlg.DoModal();
	if(rtflag==1000)
	{
	return;
	}
	else
	{
	codestr=dlg.m_captchastr;
	}
	CString skeyvalue;
	GetSkey(skeyvalue,ptwebqqhash,verifysessionhash);
	realhostname=_T("cgi.web2.qq.com");
CString timestr2;
SYSTEMTIME st;
GetLocalTime(&st);
//get_friend_uin2
	CTime time1(st);
	timestr2.Format(_T("%I64d%03d"),time1.GetTime(),st.wMilliseconds);
	
processpath.Format(_T("/keycgi/qqweb/group/search.do?pg=1&perpage=10&all=%s&c1=0&c2=0&c3=0&st=0&type=1&vfcode=%s&vfwebqq=%s&t=%s"),qqnumber,codestr,vfwebqqstr,timestr2);
	UINT ddlen=0;
	byte buf[1024*5];



MySendWeb(realhostname,processpath,buf,(char*)NULL,ddlen);
//,"allow":1,"	
//,"token":"84792c69580595e5c72204d5b683e164d9befca004fb200e",
CString expressstr;


expressstr.Format(_T("(?<=,\"GEX\":)\\d+\\b"));
CStringvect gettokenbackstr;

GetRegEX((char*)buf,expressstr,gettokenbackstr);//need consider of the actual keyname
if(gettokenbackstr.size())
   mytoken=gettokenbackstr[0];


}
int SearchqqByUin(LPCTSTR qqnumber,CString& mytoken,int *myallow)
{
CString realhostname;
CString processpath;
CString codestr;

CString url;
		CString timestr;
	GetRandomTimeStr(timestr);
url.Format(_T("/getimage?aid=1003901&%s"),timestr);
	CString ptwebqqhash,verifysessionhash;

	CColorInput dlg(url);
	int rtflag=dlg.DoModal();
	if(rtflag==1000)
	{
	return 1000;
	}
	else
	{
	codestr=dlg.m_captchastr;
	}
	CString skeyvalue;
	GetSkey(skeyvalue,ptwebqqhash,verifysessionhash);
	realhostname=_T("s.web2.qq.com");
CString timestr2;
SYSTEMTIME st;
GetLocalTime(&st);
//get_friend_uin2
	CTime time1(st);
	timestr2.Format(_T("%I64d%03d"),time1.GetTime(),st.wMilliseconds);
	
processpath.Format(_T("/api/search_qq_by_uin2?tuin=%s&verifysession=%s&code=%s&vfwebqq=%s&t=%s"),qqnumber,verifysessionhash,codestr,vfwebqqstr,timestr2);
	UINT ddlen=0;
	byte buf[1024*5];



MySendWeb(realhostname,processpath,buf,(char*)NULL,ddlen);
//,"allow":1,"	
//,"token":"84792c69580595e5c72204d5b683e164d9befca004fb200e",
CString expressstr;
expressstr.Format(_T("(?<=,\"allow\":)\\d+\\b"));
CStringvect getgroupidbackstr;
GetRegEX((char*)buf,expressstr,getgroupidbackstr);//need consider of the actual keyname
if(getgroupidbackstr.size())
   *myallow=_ttoi(getgroupidbackstr[0]);


expressstr.Format(_T("(?<=,\"token\":\")\\w+?(?=\")"));
CStringvect gettokenbackstr;
GetRegEX((char*)buf,expressstr,gettokenbackstr);//need consider of the actual keyname
if(gettokenbackstr.size())
   mytoken=gettokenbackstr[0];

if(mytoken.IsEmpty())
	return 0;
else
	return 1;
}

int JoinqqGroupFromVect(LPCTSTR qqnumber)
{
	
	{
		ApplyJoinToGroup(_ttoi64(qqnumber),g_verify_msg);
	str222=_T("�ɹ��������󣬵ȴ��Է�Ⱥ����Ա��׼��");
		
		//Write2MagnetLog(str222,&magnetfile,true);
		AfxMessageBox(str222);
	return 1;
	}

}

int AddqqFromVect(LPCTSTR qqnumber)
{
CString realhostname;
CString processpath;
realhostname=_T("s.web2.qq.com");
CString timestr2;
SYSTEMTIME st;
GetLocalTime(&st);
//get_friend_uin2
	CTime time1(st);
	timestr2.Format(_T("%I64d%03d"),time1.GetTime(),st.wMilliseconds);

	CString mytoken;
	int myallow=1;

	int rtvalue=SearchqqByUin(qqnumber,mytoken,&myallow);
	if(rtvalue==1000)
		return 1000;
	if(myallow==0)
processpath.Format(_T("/api/add_no_verify2"));
	else if(myallow==1)
processpath.Format(_T("/api/add_need_verify2"));

	CString str222;
	str222.Format(_T("qq�ţ�%s"),qqnumber);
	if(myallow<2)
	{
	UINT ddlen=0;
	byte buf[1024*5];
CStringA rstructstr; 
CStringA g_verify_msg_utf;
::function::wchrto_utf8(g_verify_msg,g_verify_msg_utf);
	
	rstructstr.Format("{\"account\":%s,\"myallow\":%d,\"groupid\":0,\"mname\":\"\",\"msg\":\"%s\",\"token\":\"%s\",\"vfwebqq\":\"%s\"}",(CStringA)qqnumber,1,g_verify_msg_utf,(CStringA)mytoken,(CStringA)vfwebqqstr);
	
	
				qqRPost(realhostname,processpath,rstructstr,buf,ddlen);
	
				
				
	if(!StrStrIA((char*)buf,"\"retcode\":0"))
	{
		str222+=_T("���ʧ��");
		str222+=(CString)(CStringA)(char*)buf;

		AfxMessageBox(str222);
		//Write2MagnetLog(str222,&magnetfile,true);
		
		return false;
	}
	else
	{
	str222+=_T("�ɹ������Ӻ������󣬵ȴ��Է�ȷ�ϣ�");
		
		//Write2MagnetLog(str222,&magnetfile,true);
		AfxMessageBox(str222);
	return true;
	}
	}
	else
	{
		str222+=_T("�޷�����ӣ������������ʴ𱣻��������κ�����ӣ�");
		AfxMessageBox(str222);
		//Write2MagnetLog(str222,&magnetfile,true);
	
	return false;
	}
}
CAutoReplyRule::CAutoReplyRule(CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CAutoReplyRule::IDD, pParent)
{
	bNomultLanguage=true;
}

CAutoReplyRule::~CAutoReplyRule()
{
}

void CAutoReplyRule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAutoReplyRule::OnBnClickedCheck8()
{
	g_slientmode=((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();
}

BEGIN_MESSAGE_MAP(CAutoReplyRule, CDialog)
    ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK1, &CAutoReplyRule::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK8, &CAutoReplyRule::OnBnClickedCheck8)

ON_EN_KILLFOCUS(IDC_EDIT5, &CAutoReplyRule::OnEnKillfocusEdit5)
ON_EN_KILLFOCUS(IDC_EDIT7, &CAutoReplyRule::OnEnKillfocusEdit7)
ON_BN_CLICKED(IDC_CHECK10, &CAutoReplyRule::OnBnClickedCheck10)
ON_EN_KILLFOCUS(IDC_EDIT14, &CAutoReplyRule::OnEnKillfocusEdit14)
ON_EN_KILLFOCUS(IDC_EDIT10, &CAutoReplyRule::OnEnKillfocusEdit10)
ON_EN_KILLFOCUS(IDC_EDIT15, &CAutoReplyRule::OnEnKillfocusEdit15)
ON_BN_CLICKED(IDC_RADIO1, &CAutoReplyRule::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &CAutoReplyRule::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_RADIO3, &CAutoReplyRule::OnBnClickedRadio3)
ON_BN_CLICKED(IDC_BUTTON5, &CAutoReplyRule::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON7, &CAutoReplyRule::OnBnClickedButton7)
ON_BN_CLICKED(IDC_CHECK11, &CAutoReplyRule::OnBnClickedCheck11)
ON_EN_KILLFOCUS(IDC_EDIT11, &CAutoReplyRule::OnEnKillfocusEdit11)
ON_BN_CLICKED(IDC_BUTTON8, &CAutoReplyRule::OnBnClickedButton8)
ON_EN_KILLFOCUS(IDC_EDIT35, &CAutoReplyRule::OnEnKillfocusEdit35)
ON_BN_CLICKED(IDC_CHECK13, &CAutoReplyRule::OnBnClickedCheck13)
ON_BN_CLICKED(IDC_CHECK6, &CAutoReplyRule::OnBnClickedCheck6)
ON_BN_CLICKED(IDC_CHECK25, &CAutoReplyRule::OnBnClickedCheck25)
ON_BN_CLICKED(IDC_CHECK26, &CAutoReplyRule::OnBnClickedCheck26)
ON_BN_CLICKED(IDC_CHECK27, &CAutoReplyRule::OnBnClickedCheck27)
ON_BN_CLICKED(IDC_CHECK28, &CAutoReplyRule::OnBnClickedCheck28)
ON_EN_KILLFOCUS(IDC_EDIT16, &CAutoReplyRule::OnEnKillfocusEdit16)
ON_EN_KILLFOCUS(IDC_EDIT39, &CAutoReplyRule::OnEnKillfocusEdit39)
ON_EN_KILLFOCUS(IDC_EDIT40, &CAutoReplyRule::OnEnKillfocusEdit40)
ON_EN_KILLFOCUS(IDC_EDIT43, &CAutoReplyRule::OnEnKillfocusEdit43)
ON_EN_KILLFOCUS(IDC_EDIT18, &CAutoReplyRule::OnEnKillfocusEdit18)
ON_EN_KILLFOCUS(IDC_EDIT44, &CAutoReplyRule::OnEnKillfocusEdit44)
ON_EN_KILLFOCUS(IDC_EDIT45, &CAutoReplyRule::OnEnKillfocusEdit45)
ON_EN_KILLFOCUS(IDC_EDIT20, &CAutoReplyRule::OnEnKillfocusEdit20)
ON_EN_KILLFOCUS(IDC_EDIT21, &CAutoReplyRule::OnEnKillfocusEdit21)
ON_EN_KILLFOCUS(IDC_EDIT22, &CAutoReplyRule::OnEnKillfocusEdit22)
ON_BN_CLICKED(IDC_BUTTON10, &CAutoReplyRule::OnBnClickedButton10)
ON_EN_KILLFOCUS(IDC_EDIT32, &CAutoReplyRule::OnEnKillfocusEdit32)
ON_BN_CLICKED(IDC_CHECK29, &CAutoReplyRule::OnBnClickedCheck29)
ON_EN_KILLFOCUS(IDC_EDIT46, &CAutoReplyRule::OnEnKillfocusEdit46)
ON_EN_KILLFOCUS(IDC_EDIT47, &CAutoReplyRule::OnEnKillfocusEdit47)
ON_EN_KILLFOCUS(IDC_EDIT48, &CAutoReplyRule::OnEnKillfocusEdit48)
ON_BN_CLICKED(IDC_CHECK36, &CAutoReplyRule::OnBnClickedCheck36)
ON_BN_CLICKED(IDC_CHECK37, &CAutoReplyRule::OnBnClickedCheck37)
ON_BN_CLICKED(IDC_BUTTON11, &CAutoReplyRule::OnBnClickedButton11)
ON_EN_KILLFOCUS(IDC_EDIT54, &CAutoReplyRule::OnEnKillfocusEdit54)
ON_BN_CLICKED(IDC_CHECK38, &CAutoReplyRule::OnBnClickedCheck38)
ON_EN_KILLFOCUS(IDC_EDIT26, &CAutoReplyRule::OnEnKillfocusEdit26)
END_MESSAGE_MAP()


// CAutoReplyRule ��Ϣ�������
void CAutoReplyRule::OnBnClickedRadio1()
{
	
	g_friend_request_method=0;
}


void CAutoReplyRule::OnBnClickedRadio3()
{
		g_friend_request_method=2;
}


void CAutoReplyRule::OnBnClickedRadio2()
{
		g_friend_request_method=1;
}
void CAutoReplyRule::OnEnKillfocusEdit5()
{
	g_privatepercent=GetDlgItemInt(IDC_EDIT5);
}






void CAutoReplyRule::OnBnClickedCheck1()
{
	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
	
	g_nohistory=true;
	
	}
	else
	{
	g_nohistory=false;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}







void CAutoReplyRule::OnEnKillfocusEdit7()
{
GetDlgItemText(IDC_EDIT7,g_autoreplycategory);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CAutoReplyRule::OnInitDialog()
{
	CMyBGCDlg::OnInitDialog();

//SetDlgItemInt(IDC_EDIT5,g_privatepercent);
SetDlgItemInt(IDC_EDIT11,g_history_maxsize);
SetDlgItemText(IDC_EDIT7,g_autoreplycategory);
SetDlgItemText(IDC_EDIT35,g_groupfilespath);
SetDlgItemText(IDC_EDIT40,g_specifygrouprankpath);

SetDlgItemText(IDC_EDIT15,g_friend_pincodestr);
SetDlgItemText(IDC_EDIT14,g_friend_welcomemsg);

SetDlgItemText(IDC_EDIT10,g_privatedefaultanswer);
CString g_verify_msg1;
g_verify_msg1.Format(_T("����%s"),g_defaultqqgrouprule.g_robotname);
SetDlgItemText(IDC_EDIT6,g_verify_msg1);
SetDlgItemText(IDC_EDIT26,g_verify_msg1);
((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(g_nohistory);

((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(g_slientmode);




((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(g_bTuopanflag);
((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(g_bApplyAll);
((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(g_bIgnoreAll);

CheckRadioButton(IDC_RADIO1,IDC_RADIO3,IDC_RADIO1+g_friend_request_method);

((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(g_doubletipPrivate);
((CButton*)GetDlgItem(IDC_CHECK29))->SetCheck(g_bShowChineseNum);
((CButton*)GetDlgItem(IDC_CHECK37))->SetCheck(g_autoagreeinvite);
((CButton*)GetDlgItem(IDC_CHECK38))->SetCheck(g_forbidsaveanyrankfile);

((CButton*)GetDlgItem(IDC_CHECK26))->SetCheck(g_newestversion);
((CButton*)GetDlgItem(IDC_CHECK27))->SetCheck(g_smarttipsforsong);
((CButton*)GetDlgItem(IDC_CHECK28))->SetCheck(g_totalsilent);

((CButton*)GetDlgItem(IDC_CHECK36))->SetCheck(!(g_bIgnoreAll^g_temporaryignorall));


SetDlgItemInt(IDC_EDIT47,g_checkgroupmembertime);

SetDlgItemText(IDC_EDIT18,g_sourceweb);
SetDlgItemText(IDC_EDIT44,g_additionalsourceweb);
SetDlgItemText(IDC_EDIT46,g_defgameweb);
SetDlgItemText(IDC_EDIT48,g_buycardweb);
SetDlgItemText(IDC_EDIT54,g_agreeinviteweb);

SetDlgItemText(IDC_EDIT45,g_additionalprewords);
SetDlgItemText(IDC_EDIT20,g_checkinformatstr);
SetDlgItemText(IDC_EDIT21,g_equipformatstr);
SetDlgItemText(IDC_EDIT22,g_gamroboformatstr);
if(!g_verify_msg.IsEmpty())
SetDlgItemText(IDC_EDIT26,g_verify_msg);

return true;
}







void CAutoReplyRule::OnBnClickedCheck10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_bTuopanflag=((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();
}


void CAutoReplyRule::OnEnKillfocusEdit10()
{
	if(!Isggged)
	{
	AfxMessageBox(_T("δע��汾�汾�������ޣ�ֻ���Կհ�ΪĬ�ϻظ���"));
	g_privatedefaultanswer=_T("");
	return;
	}

	GetDlgItemText(IDC_EDIT10,g_privatedefaultanswer);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



void CAutoReplyRule::OnEnKillfocusEdit14()
{
	GetDlgItemText(IDC_EDIT14,g_friend_welcomemsg);
	g_friend_welcomemsg.Trim();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CAutoReplyRule::OnEnKillfocusEdit15()
{
	GetDlgItemText(IDC_EDIT15,g_friend_pincodestr);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
//POST /cgi-bin/mmwebwx-bin/webwxverifyuser?r=1495288476777&pass_ticket=qsQXco%252BWd0xcyjPD3fJF1LzIALBPv5lHRD6R0A11%252Bfb1HBtRhPclWGQuNQwrXz7V HTTP/1.1
//{"BaseRequest":{"Uin":3368653628,"Sid":"hTDOt60JeBpH3hc2","Skey":"@crypt_1334ed51_c624a649123fd21aa2546e4349ee33fc","DeviceID":"e659383328641912"},"Opcode":3,"VerifyUserListSize":1,"VerifyUserList":[{"Value":"@7478785c30619b2daa314462237e0de7bc8c889a2700a447200481b8cc3d96cf","VerifyUserTicket":"v2_1430978e38dd4450e5d20cc23e7093d3d2b3bfbedde4979a77f9e4a72866327afa79cfdb48ed5d68f9851d09c6839b95c7f19ea214028ccde0ae7e80b401665b@stranger"}],"VerifyContent":"","SceneListCount":1,"SceneList":[33],"skey":"@crypt_1334ed51_c624a649123fd21aa2546e4349ee33fc"}
void CAutoReplyRule::OnBnClickedButton5()
{
	if(m_alreadylogin==false)
	{
	MessageBox(_T("���ȵ�¼��Ļ����˺����ٳ��Լ�������Ϊ���ѣ�"));
	return;
	}
	CString otherqqstr;
	GetDlgItemText(IDC_EDIT12,otherqqstr);
	otherqqstr.Trim();
GetDlgItemText(IDC_EDIT6,g_verify_msg);

if(otherqqstr.IsEmpty())
	{
	MessageBox(_T("����������Ҫ�ӵ�qq�ţ�"));
	GetDlgItem(IDC_EDIT12)->SetFocus();
	return;
	}
	//INT64 otherqqnum=_ttoi64(otherqqstr);
	AddqqFromVect(otherqqstr);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAutoReplyRule::OnBnClickedButton7()
{
	if(m_alreadylogin==false)
	{
	MessageBox(_T("���ȵ�¼��Ļ����˺����ٳ��Լ��뵽ĳ��Ⱥ�У�"));
	return;
	}
	CString otherqqstr;
	GetDlgItemText(IDC_EDIT27,otherqqstr);
	otherqqstr.Trim();
GetDlgItemText(IDC_EDIT26,g_verify_msg);

if(otherqqstr.IsEmpty())
	{
	MessageBox(_T("����������Ҫ�����Ⱥ�ţ�"));
	GetDlgItem(IDC_EDIT27)->SetFocus();
	return;
	}
	//INT64 otherqqnum=_ttoi64(otherqqstr);
	JoinqqGroupFromVect(otherqqstr);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAutoReplyRule::OnBnClickedCheck11()
{
		g_bApplyAll=((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAutoReplyRule::OnEnKillfocusEdit11()
{
		g_history_maxsize=GetDlgItemInt(IDC_EDIT11);
}


void CAutoReplyRule::OnBnClickedButton8()
{
	CString lastgroupfilespath;
	GetDlgItemText(IDC_EDIT35,lastgroupfilespath);
	TCHAR programbuf[256]=_T("");
	::function::SelectFolder(programbuf,GetSafeHwnd());
	SetDlgItemText(IDC_EDIT35,programbuf);
	g_groupfilespath=programbuf;
	if(!g_groupfilespath.IsEmpty())
	{
		if(!PathFileExists(g_groupfilespath))
			{
				g_groupfilespath=_T("");
		AfxMessageBox(_T("·�������ڣ��Զ���Ϊ���������Ŀ¼�£�"));
		return;
		}
	if(g_groupfilespath.GetAt(g_groupfilespath.GetLength()-1)!='\\')
g_groupfilespath+=_T("\\");
	

	if(lastgroupfilespath!=g_groupfilespath)
		AfxMessageBox(_T("�޸�Ⱥ���֡�Ⱥ��ȸ����ļ�����·������Ҫ��Ϊ�˷�����qqһ��Ϊ��ͬ��Ⱥ�ṩ�������޸�·������Ҫ�������������˲���ʹ������Ч��"));
	
	}
}


void CAutoReplyRule::OnEnKillfocusEdit35()
{
	GetDlgItemText(IDC_EDIT35,g_groupfilespath);
	if(!g_groupfilespath.IsEmpty())
	{	
		if(!PathFileExists(g_groupfilespath))
			{
				g_groupfilespath=_T("");
		AfxMessageBox(_T("·�������ڣ��Զ���Ϊ���������Ŀ¼�£�"));
		return;
		}
	if(g_groupfilespath.GetAt(g_groupfilespath.GetLength()-1)!='\\')
g_groupfilespath+=_T("\\");

}
}


void CAutoReplyRule::OnBnClickedCheck13()
{
g_bIgnoreAll=((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck();

BOOL bIgnoreFlag=((CButton*)GetDlgItem(IDC_CHECK36))->GetCheck();
if(bIgnoreFlag)
g_temporaryignorall=g_bIgnoreAll;
else
g_temporaryignorall=!g_bIgnoreAll;

backbufvect.clear();
		lockgroupnameresource.Lock();
		for(UINT i=0;i<GidNickNamemap.m_mapstr.size();i++)
		{
			if(g_bywindowswechatflag)
			{
				CString nicknamestr;

				nicknamestr.Format(_T("%s(%lld)"),GidNickNamemap[i],GidNickNamemap.m_mapstr[i]);
				backbufvect.push_back(nicknamestr);
			}
			else
			{
			backbufvect.push_back(GidNickNamemap[i]);
			
			}
	
		}
		lockgroupnameresource.Unlock();
			LoadIgnoreFile(true);
}




void CAutoReplyRule::OnBnClickedCheck6()
{
	g_doubletipPrivate=((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();
}


void CAutoReplyRule::OnBnClickedCheck25()
{
	g_bShowChineseNum=((CButton*)GetDlgItem(IDC_CHECK25))->GetCheck();
}


void CAutoReplyRule::OnBnClickedCheck26()
{
	g_newestversion=((CButton*)GetDlgItem(IDC_CHECK26))->GetCheck();
	if(g_newestversion)
		g_latestwechatflag=true;
}


void CAutoReplyRule::OnBnClickedCheck27()
{
	g_smarttipsforsong=((CButton*)GetDlgItem(IDC_CHECK27))->GetCheck();
}


void CAutoReplyRule::OnBnClickedCheck28()
{
	g_totalsilent=((CButton*)GetDlgItem(IDC_CHECK28))->GetCheck();
	if(g_totalsilent&&!Isggged)
	{
	AfxMessageBox(_T("�������δע��汾���˹��ܽ����ޣ����޷����ó��׾�Ĭģʽ��"));
	}
}


void CAutoReplyRule::OnEnKillfocusEdit16()
{



}


void CAutoReplyRule::OnEnKillfocusEdit39()
{
	
}


void CAutoReplyRule::OnEnKillfocusEdit40()
{
	GetDlgItemText(IDC_EDIT40,g_specifygrouprankpath);
	if(!g_specifygrouprankpath.IsEmpty())
	{	
		if(!PathFileExists(g_specifygrouprankpath))
			{
		CreateDirectory(g_specifygrouprankpath,NULL);
		}

}

}


void CAutoReplyRule::OnEnKillfocusEdit43()
{
	
}


void CAutoReplyRule::OnEnKillfocusEdit18()
{
	GetDlgItemText(IDC_EDIT18,g_sourceweb);

if(m_alreadylogin&&!Isggged&&!g_sourceweb.IsEmpty())
	{
	AfxMessageBox(_T("δע��汾�汾�������ޣ��޷������Զ����������ݽӿڡ�"));

	
	}

	if(g_sourceweb.Find(_T("www.tuling123.com"))!=-1&&g_sourceweb.Find(_T("key="))!=-1&&g_sourceweb.Find(_T("&info=%s"))==-1)
		{
				g_sourceweb.Trim();
				g_sourceweb+=_T("&info=%s");
		}

	if(!g_sourceweb.IsEmpty()&&g_sourceweb.Find(_T("http://"))==-1)
	{
		g_sourceweb=_T("http://")+g_sourceweb;
		
		SetDlgItemText(IDC_EDIT18,g_sourceweb);
	}
	g_sourceweb.Trim();

}


void CAutoReplyRule::OnEnKillfocusEdit44()
{

	GetDlgItemText(IDC_EDIT44,g_additionalsourceweb);

	if(m_alreadylogin&&!Isggged&&!g_additionalsourceweb.IsEmpty())
	{
	AfxMessageBox(_T("δע��汾�������ޣ��޷����ö����ѯ�ӿڡ�"));
	
	}

		
	if(!g_additionalsourceweb.IsEmpty()&&g_additionalsourceweb.Find(_T("http://"))==-1)
	{
		g_sourceweb=_T("http://")+g_additionalsourceweb;
		SetDlgItemText(IDC_EDIT44,g_additionalsourceweb);
	}

	g_additionalsourceweb.Trim();
}


void CAutoReplyRule::OnEnKillfocusEdit45()
{

	
	GetDlgItemText(IDC_EDIT45,g_additionalprewords);
	
		if(g_additionalprewords.IsEmpty())
	{
		AfxMessageBox(_T("�����ѯ�ӿڵĴ����ַ�����Ϊ�գ�Ҳ��Ӧ����#����������������õ����壬����Ķ���"));
	SetDlgItemText(IDC_EDIT45,_T("��ѯ"));
	
	}

}


void CAutoReplyRule::OnEnKillfocusEdit20()
{
	GetDlgItemText(IDC_EDIT20,g_checkinformatstr);
	g_checkinformatstr.Replace(_T("xml��"),_T("xml:"));
}


void CAutoReplyRule::OnEnKillfocusEdit21()
{
	GetDlgItemText(IDC_EDIT21,g_equipformatstr);
		g_equipformatstr.Replace(_T("xml��"),_T("xml:"));
}


void CAutoReplyRule::OnEnKillfocusEdit22()
{
	GetDlgItemText(IDC_EDIT22,g_gamroboformatstr);
g_gamroboformatstr.Replace(_T("xml��"),_T("xml:"));
}


void CAutoReplyRule::OnBnClickedButton10()
{

g_checkinformatstr=default_g_checkinformatstr;
g_equipformatstr=default_g_equipformatstr;
g_gamroboformatstr=default_g_gamroboformatstr;

	SetDlgItemText(IDC_EDIT20,g_checkinformatstr);
	SetDlgItemText(IDC_EDIT21,g_equipformatstr);
	SetDlgItemText(IDC_EDIT22,g_gamroboformatstr);
}


void CAutoReplyRule::OnEnKillfocusEdit32()
{

	
}


void CAutoReplyRule::OnBnClickedCheck29()
{
	g_bShowChineseNum=((CButton*)GetDlgItem(IDC_CHECK29))->GetCheck();
}


void CAutoReplyRule::OnEnKillfocusEdit46()
{

	
	GetDlgItemText(IDC_EDIT46,g_defgameweb);

	
		
	if(!g_defgameweb.IsEmpty()&&g_defgameweb.Find(_T("http://"))==-1)
	{
		g_defgameweb=_T("http://")+g_defgameweb;
		SetDlgItemText(IDC_EDIT46,g_defgameweb);
	}

g_defgameweb.Trim();

}


void CAutoReplyRule::OnEnKillfocusEdit47()
{
	int oldg_checkgroupmembertime = g_checkgroupmembertime;

	g_checkgroupmembertime=GetDlgItemInt(IDC_EDIT47);
	if(g_checkgroupmembertime<0)
		g_checkgroupmembertime=INT_MAX;

	if(g_checkgroupmembertime<5)
	{
	AfxMessageBox(_T("��ʱ���Ⱥ�ڳ�Ա������ʱ�������5�����ϣ�ʱ��Խ�̣�cpu����Խ�أ���������15�����ϣ�"));
	}
	
			g_checkgroupmembertime=max(5,g_checkgroupmembertime);
	SetDlgItemInt(IDC_EDIT47,g_checkgroupmembertime);

	if (m_alreadylogin&&oldg_checkgroupmembertime != g_checkgroupmembertime)
	{
		AfxMessageBox(_T("�������ñ��������������������������Ч��"));
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CAutoReplyRule::OnEnKillfocusEdit48()
{
	GetDlgItemText(IDC_EDIT48,g_buycardweb);

	
		
	if(!g_buycardweb.IsEmpty()&&g_buycardweb.Find(_T("http://"))==-1)
	{
		g_buycardweb=_T("http://")+g_buycardweb;
		SetDlgItemText(IDC_EDIT48,g_buycardweb);
	}


	g_buycardweb.Trim();
	g_buycardweb.TrimRight('/');
	
}
LRESULT CAutoReplyRule::OnNcHitTest(CPoint point)
{
	  CRect rc;
           GetClientRect(&rc);
           ClientToScreen(&rc);
           return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);   //�������ڿͻ��������䵱��������
}


void CAutoReplyRule::OnBnClickedCheck36()
{
	BOOL bIgnoreFlag=((CButton*)GetDlgItem(IDC_CHECK36))->GetCheck();
		if(g_bIgnoreAll^bIgnoreFlag)//Ĭ�Ϲر�����ȺΪ1ʱ���������ʱ��������Ⱥ����ִ���������ִ��else //���Ĭ�Ϲر�����ȺΪ0���������ε�Ⱥ�����������ʱ��������Ⱥ1���ͻ�ִ������ģ�������else
				
			{
				//!(g_bIgnoreAll^g_temporaryignorall) return;(������)
				g_temporaryignorall=false;
			}
			else
			{

			//dislkbackbufvect.RemoveAll();
				g_temporaryignorall=true;

			}
					
}


void CAutoReplyRule::OnBnClickedCheck37()
{
	g_autoagreeinvite=((CButton*)GetDlgItem(IDC_CHECK37))->GetCheck();
}


void CAutoReplyRule::OnBnClickedButton11()
{
	CString lastgroupfilespath;
	GetDlgItemText(IDC_EDIT40,lastgroupfilespath);
	TCHAR programbuf[256]=_T("");
	::function::SelectFolder(programbuf,GetSafeHwnd());
	SetDlgItemText(IDC_EDIT40,programbuf);
	g_specifygrouprankpath=programbuf;
	if(!g_specifygrouprankpath.IsEmpty())
	{
		if(!PathFileExists(g_specifygrouprankpath))
			{
		CreateDirectory(g_specifygrouprankpath,NULL);
		}
	if(g_specifygrouprankpath.GetAt(g_specifygrouprankpath.GetLength()-1)!='\\')
g_specifygrouprankpath+=_T("\\");
	

	if(lastgroupfilespath!=g_specifygrouprankpath)
				AfxMessageBox(_T("�޸�Ⱥ���ֱ���·������Ҫ��Ϊ�˷�����qq����ͬһ�ݻ����ļ������޸�·������Ҫ�������������˲���ʹ������Ч��"));
	
	}
}


void CAutoReplyRule::OnEnKillfocusEdit54()
{
	GetDlgItemText(IDC_EDIT54,g_agreeinviteweb);

	if(!::function::IsChinese(g_agreeinviteweb))
	{
		
	if(!g_agreeinviteweb.IsEmpty()&&g_agreeinviteweb.Find(_T("http://"))==-1)
	{
		if(_ttoi(g_agreeinviteweb)==2000)
		{
		}
		else
		{
		g_agreeinviteweb=_T("http://")+g_agreeinviteweb;
		SetDlgItemText(IDC_EDIT54,g_agreeinviteweb);
		}
	}


	g_agreeinviteweb.Trim();
	g_agreeinviteweb.TrimRight('/');
	}
}


void CAutoReplyRule::OnBnClickedCheck38()
{
	g_forbidsaveanyrankfile=((CButton*)GetDlgItem(IDC_CHECK38))->GetCheck();

}


void CAutoReplyRule::OnEnKillfocusEdit26()
{
GetDlgItemText(IDC_EDIT26,g_verify_msg);
}
