// AcceptRequestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AcceptRequestDlg.h"
//#include "afxdialogex.h"


// CAcceptRequestDlg �Ի���

IMPLEMENT_DYNAMIC(CAcceptRequestDlg, CDialog)
extern CKeepMap<INT64,INT64,CAcceptRequestDlg*,CAcceptRequestDlg*> m_mapQQacdlg;
extern void LoadIgnoreQQlist(BOOL bLoad);
extern CCriticalSection lockQQacdlg;
extern CKeepMap<CString,LPCTSTR,CString,LPCTSTR> g_joinQQ_QQword_map;//��Ⱥʱ��д����֤����
extern CKeepMap<CString,LPCTSTR,INT64,INT64> g_joinQQ_passQQnum_map;//��Ⱥʱ��д������
extern CCriticalSection lockgopuinres;
extern BOOL ExstractOneNum(LPCTSTR wstr,INT64 &QQnum);
CAcceptRequestDlg::CAcceptRequestDlg(INT64 QQgroupid,INT64 realgroupid,INT64 requestuin,INT64 QQaccount,INT64 timesignal,LPCTSTR nickname,LPCTSTR content,int bMale,QQgrouprule tempgrule,CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CAcceptRequestDlg::IDD, pParent)
{
	bNomultLanguage=true;
	m_bMale=bMale;
	m_QQgroupid=QQgroupid;
	m_requestuin=requestuin;
	m_QQaccount=QQaccount;
	m_timesignal=timesignal;
	m_nickname=nickname;
	m_content=content;
	if(g_slientmode)
	ncount=1;
	else
	ncount=5;

	m_grule=tempgrule;
	m_realgroupnumber=realgroupid;
}

CAcceptRequestDlg::~CAcceptRequestDlg()
{
}

void CAcceptRequestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAcceptRequestDlg, CDialog)
ON_WM_CTLCOLOR()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_TIMER()
ON_BN_CLICKED(IDOK, &CAcceptRequestDlg::OnBnClickedOk)
ON_WM_CLOSE()
ON_BN_CLICKED(IDCANCEL, &CAcceptRequestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAcceptRequestDlg ��Ϣ�������



void CAcceptRequestDlg::OnDelSelf()
{
	//OnOK();
	
	

	
	

{
	PostQuitMessage(0);
}

}
void CAcceptRequestDlg::OnDestroy()
{

	CMyBGCDlg::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
}

extern BOOL AgreeFriendRequest(INT64 QQaccount,INT64 timesignal);
extern BOOL AgreeGroupRequest(INT64 groupuin,INT64 requestuin,INT64 timesignal);
extern BOOL DenyGroupRequest(INT64 groupuin,INT64 requestuin,INT64 timesignal,LPCTSTR reason);
extern BOOL DenyFrendRequest(INT64 QQaccount,INT64 timesignal,LPCTSTR reason);

UINT SendWelcome(LPVOID lp)
{
	Sleep(2000);
	
	welcomestruct *pwelcome=(welcomestruct*)lp;
	CString tempstr=pwelcome->welcomestr;
	CString tempshowstr;
	
	
	CString opuincardname,operationname,realcardname;
				LPCTSTR findpos1=StrStr(pwelcome->nickname,_T("����"));
			LPCTSTR findpos2=StrStr(pwelcome->nickname,_T("��׼"));
			opuincardname=_T("����Ա");
			operationname=_T("��׼");
			realcardname=pwelcome->nickname;
if(findpos1||findpos2)
{
	if(findpos1)
	{
		opuincardname=pwelcome->nickname;
		opuincardname=opuincardname.Left(findpos1-pwelcome->nickname.GetBuffer(0));
		operationname=_T("����");
	realcardname=findpos1+lstrlen(_T("����"));

	}
	else
	{
		opuincardname=pwelcome->nickname;
		opuincardname=opuincardname.Left(findpos2-pwelcome->nickname.GetBuffer(0));
		operationname=_T("��׼");
	realcardname=findpos2+lstrlen(_T("��׼"));
	}

}
	tempshowstr.Format(_T("%s"),realcardname);

		tempstr.Replace(_T("xxx"),tempshowstr);
	tempstr.Replace(_T("XXX"),tempshowstr);
tempshowstr.Format(_T("%lld "),pwelcome->otherQQnum);

	tempstr.Replace(_T("$QQ$"),tempshowstr);
	tempstr.Replace(_T("$QQ$"),tempshowstr);

	tempstr.Replace(_T("$������$"),opuincardname);
	tempstr.Replace(_T("$������Ϊ$"),operationname);

	tempstr.Replace(_T("xxx"),tempshowstr);
	tempstr.Replace(_T("XXX"),tempshowstr);
	ReplaceRandomText(tempstr);

	tempshowstr=_T("");
	if(pwelcome->bMale>0)
	{
	if(pwelcome->bMale==0)
	tempshowstr.Format(_T("Ů"));
	else if(pwelcome->bMale==1)
	tempshowstr.Format(_T("��"));
	else
	tempshowstr.Format(_T("δ֪"));
	}
	if(tempshowstr.IsEmpty())
	{
	tempstr.Replace(_T("��$�Ա�$��"),tempshowstr);
	tempstr.Replace(_T("($�Ա�$)"),tempshowstr);
	}

	tempstr.Replace(_T("$�Ա�$"),tempshowstr);
	

	INT64	m_realgroupnumber=GetRealGroupnumberFromGid(pwelcome->pro_m_QQgroupid);

		CStringvect getgroupQQbackstr;
	CStringvectA getutfnamebackstr;
	GetGroupQQNickName(m_realgroupnumber,getgroupQQbackstr,getutfnamebackstr,NULL);

		lockggroupmembercountsource.Lock();
		if(!bCheckingmemebercount)
	g_group_membercountmap[m_realgroupnumber]=getgroupQQbackstr.size();
	lockggroupmembercountsource.Unlock();
ConvertVariousValue(m_realgroupnumber,tempstr,pwelcome->otherQQnum);

CString answerstr;
		for(auto it=g_outputreplacemap.m_mapstr.rbegin();it!=g_outputreplacemap.m_mapstr.rend();it++)
{
	if(tempstr.Find(*it)!=-1)
	{
		GetRandAnswer(answerstr,g_outputreplacemap[*it]);
	tempstr.Replace(*it,answerstr);
	//break;
	}
}

	CStringvect welcomevect;
	GetTotal_content_vect(welcomevect,tempstr);
	

	CString publicwelcomestr,sessionwelcomestr;
	publicwelcomestr=tempstr;
	if(welcomevect.size()>0)
		publicwelcomestr=welcomevect[0];
	if(welcomevect.size()>1)
	{
		sessionwelcomestr=welcomevect[1];
		sessionwelcomestr.Trim();
	}
	BOOL rtflag=false;

	if((!pwelcome->bTatalSlient&&!g_totalsilent)||publicwelcomestr.Find(_T("������Ϸ����Ĭ��"))!=-1||sessionwelcomestr.Find(_T("������Ϸ����Ĭ��"))!=-1)
	{

	rtflag=SendGroupQQMsg(pwelcome->pro_m_QQgroupid,publicwelcomestr+_T("������Ϸ�����ա�"));
	if(rtflag==false)
	{
		Sleep(2000);
		rtflag=SendGroupQQMsg(pwelcome->pro_m_QQgroupid,publicwelcomestr+_T("������Ϸ�����ա�"));
	}

	if(!sessionwelcomestr.IsEmpty())
	{
		rtflag=SendSessionQQMsg(pwelcome->pro_m_QQgroupid,pwelcome->cur_uin,sessionwelcomestr+_T("������Ϸ�����ա�"));
	if(rtflag==false)
	{
		Sleep(2000);
		rtflag=SendSessionQQMsg(pwelcome->pro_m_QQgroupid,pwelcome->cur_uin,sessionwelcomestr+_T("������Ϸ�����ա�"));
	}
	}
	}

				if(m_realgroupnumber&&(Isggged||g_alreadyhomepage))
				{

			CString cardname=pwelcome->nickname;
				cardname.Trim();

					CString tempprestr;

				if(pwelcome->bMale==1)
				{
					tempprestr=pwelcome->precardname;
				}
				else
					tempprestr=pwelcome->femaleprecardname;

		BOOL	bIncludeDDDflag=false;
		if(StrStrI(tempprestr,_T("DDD")))
		{
		bIncludeDDDflag=true;
		}
tempprestr.Replace(_T("DDD"),pwelcome->titlestr);
tempprestr.Replace(_T("ddd"),pwelcome->titlestr);

					
					if(StrStrI(tempprestr,_T("xxx")))
					{
						GetPerfectnamecard(tempprestr,cardname,true);
	//					int restlen=2;
	//						CStringA tempprestrA=(CStringA)tempprestr;

	//						restlen=(14-(tempprestrA.GetLength()-3))/2;//7-(tempprestr.GetLength()-3);
	//					
	//					cardname=cardname.Right(restlen);
	//tempprestr.Replace(_T("xxx"),cardname);
	//tempprestr.Replace(_T("XXX"),cardname);
	//cardname=tempprestr;
					}
					else
					{
						GetPerfectnamecard(tempprestr,cardname,false);
				//		int restlen=2;
				//			CStringA tempprestrA=(CStringA)tempprestr;

				//			restlen=(14-tempprestrA.GetLength())/2;//7-(tempprestr.GetLength()-3);
				//		
				//		cardname=cardname.Right(restlen);

				//cardname.Insert(0,tempprestr);
					}
//Sleep(10*1000);			
				if(cardname!=pwelcome->nickname)
				{
	ChangebusinessCard(m_realgroupnumber,pwelcome->otherQQnum,cardname);
	//cardname=cardname.Left(7);
				}
	CKeepINT64StrMap *pintstrmap=NULL;
	lockgnameresource.Lock();
	if(g_group_uinNameMap.Lookup(pwelcome->pro_m_QQgroupid,pintstrmap))
	(*pintstrmap)[pwelcome->cur_uin]=cardname;
lockgnameresource.Unlock();

lockgQQnickmapresource.Lock();
if(GrouprealQQNickmap.Lookup(m_realgroupnumber,pintstrmap))
(*pintstrmap)[pwelcome->otherQQnum]=cardname;
lockgQQnickmapresource.Unlock();


lockuinrealQQres.Lock();
UinRealNumMap[pwelcome->cur_uin]=pwelcome->otherQQnum;
lockuinrealQQres.Unlock();
				}
	delete pwelcome;
return 0;
}
void CAcceptRequestDlg::OnBnClickedOk()
{
	
	BOOL bDenyflag;
	BOOL bAlwaysDeny=false;
	if(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO2)
	{
	bDenyflag=true;
	}
	else
	bDenyflag=false;

	int opvalue=-1;
	if(1)//g_slientmode)
	{
	if(m_realgroupnumber)
	{


opvalue=m_grule.g_group_request_method;
	}
	else
opvalue=g_friend_request_method;
	}
	
if(!g_totalsilent&&GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO1&&(opvalue==0||opvalue==2))
	opvalue=1;


CString denyreason;
CString welcomestr;
CString precardname;
CString femaleprecardname;
CString nnnnprecardname;
CString adminprecardname;

CString g_ranktitlestr;
INT64 ruleinitscore;
BOOL bTatalSlient;

CString contentbackstr;
		::function::GetHalfStr(m_content,NULL,&contentbackstr,_T("�������ɣ�"),false);
		contentbackstr.TrimRight(_T("\\\\r"));

	

if(m_realgroupnumber&&m_QQaccount)
{

		CString admininvitestr;
		CString keystr;
		keystr.Format(_T("%lld_%lld"),m_realgroupnumber,m_QQaccount);
		INT64 realinviteuin=0;
		ExstractOneNum(contentbackstr,realinviteuin);
			lockgopuinres.Lock();
	g_joinQQ_QQword_map[keystr]=contentbackstr;

	if(realinviteuin>10000)
	{
	if(realinviteuin>10000||!g_joinQQ_passQQnum_map.Lookup(keystr))
	g_joinQQ_passQQnum_map[keystr]=realinviteuin;
	}
	lockgopuinres.Unlock();
	
		if(g_bNewUserGuestPlusTime)
		{
			SYSTEMTIME mySystemTime;
	GetLocalTime(&mySystemTime);
			CString timestr;
			timestr.Format(_T("�ο�%d.%d"),mySystemTime.wMonth,mySystemTime.wDay);
precardname=timestr;
femaleprecardname=timestr;
nnnnprecardname=timestr;
adminprecardname=timestr;
		}
		else
		{

precardname=m_grule.precardstr;
femaleprecardname=m_grule.femaleprecardstr;
nnnnprecardname=m_grule.nnnnprecardstr;
adminprecardname=m_grule.adminprecardstr;
		}
g_ranktitlestr=m_grule.g_ranktitlestr;
denyreason=m_grule.g_group_denyreason;
ruleinitscore=m_grule.initscore;
bTatalSlient=m_grule.bTatalSlient;
if(!m_grule.g_welcomestr.IsEmpty()&&m_grule.lastwelcomestr.IsEmpty())
{
InsertExternalData(m_grule.g_welcomestr,m_grule.lastwelcomestr);
}
welcomestr=m_grule.lastwelcomestr;

		//InsertExternalData(welcomestr,welcomestr);
if(m_grule.g_group_denyreason==_T("��ֹ�ظ���Ⱥ")||g_bForbidReJoin)
		{
			if(FindAlreadyExistQQ(m_QQaccount))
			{
	bAlwaysDeny=true;
	denyreason=_T("��ֹ�ظ���Ⱥ");
			}
	}

if(find(g_forever_blacklist.begin(),g_forever_blacklist.end(),m_QQaccount)!=g_forever_blacklist.end())
	{
	
	bAlwaysDeny=true;
	denyreason=_T("���Ѿ��������ú��������޷�����");
	}	
	if(find(m_grule.g_blacklist.begin(),m_grule.g_blacklist.end(),m_QQaccount)!=m_grule.g_blacklist.end())
	{
	
	bAlwaysDeny=true;
	denyreason=_T("���Ѿ�����Ⱥ���������޷�����");
	}	
	if(!m_grule.g_onlyself)
	{
		if(find(g_defaultQQgrouprule.g_blacklist.begin(),g_defaultQQgrouprule.g_blacklist.end(),m_QQaccount)!=g_defaultQQgrouprule.g_blacklist.end())
	{
	
	bAlwaysDeny=true;
	denyreason=_T("���Ѿ�������Ⱥ���������޷�����");
	}	

	
	}
}
if(m_realgroupnumber==0)
{

denyreason=g_friend_denyreason;
welcomestr=g_defaultQQgrouprule.g_welcomestr;
	if(find(g_defaultQQgrouprule.g_blacklist.begin(),g_defaultQQgrouprule.g_blacklist.end(),m_QQaccount)!=g_defaultQQgrouprule.g_blacklist.end())
	{
	bAlwaysDeny=true;
	denyreason=_T("���Ѿ�������������޷���Ϊ����");
	}	


}

if(bAlwaysDeny)
{
	if(m_requestuin)
	{
		
	DenyGroupRequest(m_QQgroupid,m_requestuin,m_timesignal,denyreason);
	}
else
{
	DenyFrendRequest(m_QQaccount,m_timesignal,denyreason);
}	

}
else if(opvalue)
{
		CString codecontent;
		CString denyreasonstr;
		if(m_realgroupnumber)
		{
	
	denyreasonstr=m_grule.g_group_denyreason;
					codecontent=m_grule.g_group_pincodestr;
		}
		else
		{

	denyreasonstr=g_friend_denyreason;
	//2016.12.18,�Ӻ��Ѿܾ����ɶ��ǿ�

	denyreasonstr=_T("");
		codecontent=g_friend_pincodestr;
		}
			BOOL bSameQQ=false;
		BOOL regQQnumflag=false;
		BOOL bDirectlyAgree=false;
		BOOL bneedmanualAgree=false;
		BOOL bDirectDeny=false;
		CString rtdenyreason;
		//http://www.cfxy.me/QQnum?
		//http://buy.cfxy.me/recharge.php?cardseq=
		//�Զ�����robotQQnum��opQQnum��version��
		BOOL bRechargeflag=false;
		BOOL bLocaldllplugflag=false;
		if(codecontent.Find(_T("recharge"))!=-1)
				{
		bRechargeflag=true;
		}
		if(codecontent.Find(_T(".dll?function="))!=-1)
				{
		bLocaldllplugflag=true;
		}
	INT64 *pQQnum=new INT64;
*pQQnum=m_requestuin;
//AfxBeginThread(ProGetQQMoreProfileInf,pQQnum);
ProGetQQMoreProfileInf(pQQnum);
//NickSex rttempnicksex;
if(m_bMale==-1)
{
			CString QQnickname;
	



BOOL bGetGenderok=false;
if(opvalue==3)
bGetGenderok=GetUinSingleWholeQQInf(m_QQgroupid,m_requestuin,QQnickname,m_bMale,NULL,50);
else
bGetGenderok=GetUinSingleWholeQQInf(m_QQgroupid,m_requestuin,QQnickname,m_bMale,NULL,10);

if(!bGetGenderok)
{
	Sleep(1000);
bGetGenderok=GetUinSingleWholeQQInf(m_QQgroupid,m_requestuin,QQnickname,m_bMale,NULL,5);
}
	}

struct_QQnum_msg tempQQnumpro;
lockQQmoreprofile.Lock();
	g_QQnum_structmoreprofile_map.Lookup(m_requestuin,tempQQnumpro);
	lockQQmoreprofile.Unlock();
		CString localtionstr;
		
		localtionstr.Format(_T("%s %s %s"),tempQQnumpro.location_state,tempQQnumpro.location_city,tempQQnumpro.location_area);
		localtionstr.Trim();

		CString agestr;

		agestr.Format(_T("%s"),tempQQnumpro.age);
		agestr.Trim();
		CString personalstr;
		personalstr.Format(_T("%s"),tempQQnumpro.personal);
		
		if(m_bMale!=1&&m_bMale!=0)
		{

			if(tempQQnumpro.gender==_T("1"))
		m_bMale=1;
	else if(tempQQnumpro.gender==_T("2"))
		m_bMale=0;

		}

	//	if(agestr.IsEmpty())
		//	agestr.Format(_T("%d"),rttempnicksex.age);

//����Ҫ��Ҫ��ȡQQ�ȼ���QQ������Ϣ
int requireminQQlevel=0;
int requireminQQdarenlevel=0;
requireminQQlevel=m_grule.g_group_pinQQlevel;
requireminQQdarenlevel=m_grule.g_group_pinQQdarenlevel;
	int QQlevel=0;
	int QQdarenlevel=0;
	int QQlikenum=0;
if(requireminQQlevel||requireminQQdarenlevel||bRechargeflag)
{
//��ѯQQ�ȼ���QQ������Ϣ
		

GetQQlevelFromMap(m_requestuin,QQlevel,QQdarenlevel,QQlikenum,5,false);

if(QQlevel==0&&QQdarenlevel==0)
{
GetQQlevelFromMap(m_requestuin,QQlevel,QQdarenlevel,QQlikenum,5,true);

}

}

if(IsDefaultGroupOwner(m_QQaccount,true))
{
	regQQnumflag=true;

bDirectlyAgree=true;
	
}
	if(!bDenyflag)
	{
	
		//
	
		if(_tcsicmp(codecontent,_T("ע��QQ"))==0||StrStrI(codecontent,_T("http://")))
		{
		//g_additionalsourceweb
		CString Getverifierstr;
		CString tempstr2;

		contentbackstr.TrimLeft(_T("��ע��"));
		contentbackstr.TrimLeft(_T("ע��"));
		contentbackstr.TrimLeft(_T("QQ"));
		contentbackstr.TrimLeft(_T("QQ"));
		INT64 QQregnum=0;
		if(!bRechargeflag)
		QQregnum=_ttoi64(contentbackstr);
	
		if(QQregnum==0&&!bRechargeflag)
		{
		CString expressstr;
CStringvect temp2vect;

expressstr.Format(_T("\\d{5,20}"));
GetRegEX(contentbackstr,expressstr,temp2vect);//need consider of the actual keyname
if(temp2vect.size()>0)
	QQregnum=_ttoi64(temp2vect[0]);

		}
	/*	if(m_realgroupnumber==229375129)
		if(QQregnum==m_QQaccount)
			bSameQQ=true;*/

		if(QQregnum>10000)
		{
			if(_tcsicmp(codecontent,_T("ע��QQ"))==0)
			{
		if(m_realgroupnumber!=229375129&&m_realgroupnumber!=226519623&&m_realgroupnumber!=477633103&&m_realgroupnumber!=285960406&&m_realgroupnumber!=555421587)
			Getverifierstr.Format(_T("http://www.cfxy.me/cfweixinrobot.php?QQnum=%I64d&pppflag=1"),QQregnum);
		else
			Getverifierstr.Format(_T("http://www.cfxy.me/cfweixinrobot.php?QQnum=%I64d&joincheck=%lld"),QQregnum,m_QQaccount);
			}
			else
			{
				//http://www.cfxy.me/broadcast3.txt.php?word=
				
			Getverifierstr.Format(_T("%s%lld"),codecontent,QQregnum);
	
			}
		}
		else
		{
		if(_tcsicmp(codecontent,_T("ע��QQ"))==0)
						Getverifierstr.Format(_T("http://www.cfxy.me/cfweixinrobot.php?QQnum=%s"),contentbackstr);
		else
		{
			if(bRechargeflag)
				{
						CStringA msgA;
	::function::wchrto_utf8(contentbackstr,msgA);
	char bufvalue[2048*10];
	int ddd=msgA.GetLength();
	
		strcpy(bufvalue,(CStringA)msgA);
		int ddd3=strlen(bufvalue);
	::function::urlencode(bufvalue);
	CString tbufv;//=trimusermsg;
	tbufv=(CString)(CStringA)bufvalue;

	CString sendnickname;


	::function::wchrto_utf8(m_nickname,msgA);
	
	ddd=msgA.GetLength();
	
		strcpy(bufvalue,(CStringA)msgA);
		
	::function::urlencode(bufvalue);
	
	sendnickname=(CString)(CStringA)bufvalue;

	CString sendlocationstr;
CString sendpersonalstr;


	::function::wchrto_utf8(localtionstr,msgA);
	
	ddd=msgA.GetLength();
	
		strcpy(bufvalue,(CStringA)msgA);
		
	::function::urlencode(bufvalue);
	
	
sendlocationstr=(CString)(CStringA)bufvalue;

::function::wchrto_utf8(personalstr,msgA);
	
	ddd=msgA.GetLength();
	
		strcpy(bufvalue,(CStringA)msgA);
		
	::function::urlencode(bufvalue);
	
	
sendpersonalstr=(CString)(CStringA)bufvalue;


	HWND inhwnd=0;
	inhwnd=GetCMsgDlgHwndFromGroupNum(m_realgroupnumber,false);
				Getverifierstr.Format(_T("%s%s&cfrobotselfnum=%s&cfrobotQQnum=%lld&cfrobotgroupnum=%lld&cfQQlevel=%d&cfQQdarenlevel=%d&cfQQlikenum=%d&cfgender=%d&cfrobotnickname=%s&cfrobotloc=%s&cfrobotage=%s&cfrobotpersonal=%s&myhwnd=%d&version=%s&checksign=%s"),codecontent,tbufv,m_ownQQnumber,m_QQaccount,m_realgroupnumber,QQlevel,QQdarenlevel,QQlikenum,m_bMale,sendnickname,sendlocationstr,agestr,sendpersonalstr,inhwnd,g_purenumberversion,g_ksidmd5);
			

			}
				else
	Getverifierstr.Format(_T("%s%s"),codecontent,contentbackstr);
		
		}
		}

	if(bLocaldllplugflag)
		LocalPlugFunction(Getverifierstr,tempstr2);
	else
DownloadWebResultTxt(Getverifierstr,tempstr2);

	int rtvalueint=_ttoi(tempstr2);
if(rtvalueint>=2000)
{
	regQQnumflag=true;

bDirectlyAgree=true;
	
}
else if(rtvalueint>=20&&m_realgroupnumber!=229375129&&m_realgroupnumber!=226519623&&m_realgroupnumber!=477633103&&m_realgroupnumber!=285960406&&m_realgroupnumber!=555421587)
	regQQnumflag=true;
else
{
	if(rtvalueint==2)
	{
		bDenyflag=true;
		bDirectDeny=true;
	}
	else if(rtvalueint>2)
	{
	bneedmanualAgree=true;
	}
	else if(rtvalueint==0)
	{
		if(tempstr2.GetLength()>1)
		{
bDenyflag=true;
	rtdenyreason=tempstr2;
		}
	}
if(StrStrI(contentbackstr,_T("ע��QQ")))
{
contentbackstr=_T("");
}

if(StrStrI(contentbackstr,codecontent))
{
contentbackstr=_T("");
}


}


		}

		if(!codecontent.IsEmpty()&&!StrStrI(contentbackstr,codecontent)&&(!regQQnumflag))
		{
		

		if(denyreasonstr.IsEmpty()||denyreasonstr==_T("��ֹ�ظ���Ⱥ"))
		{

			//if(!m_realgroupnumber)//�Ӻ��ѣ���������Ͼ�Ҫ�ֶ���׼
		bneedmanualAgree=true;
			
			//denyreason=_T("��֤��Ϣ����ȷ�������²�����ȷ����֤��Ϣ�������룡");//�ĳɲ���������Ⱥ���ֶ�����
		}
		else
		{
		//�������ַ���ر�2�󣬱�20С�����֣���ô���ǲ���������������Ա�ֶ�����
if(bneedmanualAgree)
{

}
		else
		{
			
			bDenyflag=true;
		
		if(denyreasonstr==_T("�Զ������ܾ�����"))
			{
		
				if(bDirectDeny)
				{
				denyreason=_T("���Ѿ���������������");
				}
				else
			denyreason=_T("��֤��Ϣ����ȷ�������²�����ȷ����֤��Ϣ�������룡");
			}
			else
			denyreason=denyreasonstr;


}
		}

		}
		

		if(!bDirectlyAgree)
		{
		if(m_bMale!=0&&opvalue==3)
		{
	
			if(!denyreasonstr.IsEmpty()&&m_bMale==1)
			{
		bDenyflag=true;
		if(denyreasonstr==_T("�Զ������ܾ�����"))
			{
			
			denyreason=_T("ֻ��Ů�Բſɼ����Ⱥ");
		}
		else
			denyreason=denyreasonstr;
		//denyreason=denyreasonstr;
			}
		

		}
		
		if(m_realgroupnumber&&(QQlevel<requireminQQlevel||QQdarenlevel<requireminQQdarenlevel))
		{
		
		if(!denyreasonstr.IsEmpty())
		{
			bDenyflag=true;
		if(denyreasonstr==_T("�Զ������ܾ�����"))
		{

			bDenyflag=true;

			if(QQlevel==0&&QQdarenlevel==0)
{
	denyreason.Format(_T("δʶ���QQ�ȼ�����Ϣ�����Ժ����Լ�Ⱥ��"));
			}
			else
			{
			if(QQlevel<requireminQQlevel&&QQdarenlevel<requireminQQdarenlevel)
denyreason.Format(_T("QQ�ȼ�Ҫ����%d����������Ҫ����%d"),requireminQQlevel,requireminQQdarenlevel);
			else if(QQlevel<requireminQQlevel)
			denyreason.Format(_T("QQ�ȼ�Ҫ����%d"),requireminQQlevel);
			else if(QQdarenlevel<requireminQQdarenlevel)
				denyreason.Format(_T("QQ��������Ҫ����%d"),requireminQQdarenlevel );
			}
				
		}
		else
		{
			
			denyreason=denyreasonstr;
			
		}
		}
		else		
		bneedmanualAgree=true;


		}
		}

		if(!rtdenyreason.IsEmpty())
		{
		denyreason=rtdenyreason;
		}
		
		if(!bDenyflag&&!bneedmanualAgree)
		{
	if(m_realgroupnumber)
	{

	BOOL rtflag=AgreeGroupRequest(m_QQgroupid,m_requestuin,m_timesignal);
		CString tempstr=welcomestr;
		tempstr.Trim();
		
		static INT64 lastQQacount=0;
		if(rtflag)//&&lastQQacount!=m_QQaccount)
		{
		lastQQacount=m_QQaccount;
		BOOL alreadywelcomeflag=false;
		//if(!g_Androidflag)
		{
		Sleep(1500);
//lockwelcomeQQnumres.Lock();
//if(g_alreadywelcomeQQnum.find(m_QQaccount)!=g_alreadywelcomeQQnum.end())
//	alreadywelcomeflag=true;
//if(!alreadywelcomeflag)
//g_alreadywelcomeQQnum.insert(m_QQaccount);
//lockwelcomeQQnumres.Unlock();

HWND mymsghwnd=GetCMsgDlgHwndFromGroupNum(m_QQgroupid);

	if(m_QQaccount)
	{
		CString cardname;
	GetNickNameFromGroupMap(m_realgroupnumber,m_QQaccount,cardname);
		::SendMessage(mymsghwnd,WM_NEWJOIN,(WPARAM)cardname.GetBuffer(0),(LPARAM)&m_QQaccount);
	}
	//		alreadywelcomeflag=::SendMessage(mymsghwnd,WM_ALREADYWELCOME,(WPARAM)1,(LPARAM)&m_QQaccount);
	
//if(1)//!alreadywelcomeflag)
//{
//GetGlobalMyRank(m_realgroupnumber,m_QQaccount);
//SetProbationTime(m_realgroupnumber,m_QQaccount);
//
//
//}

	
	
	}
		}
	}
	else
	{
	AgreeFriendRequest(m_QQaccount,m_timesignal);
	
	}
		}
	}

	if(bDenyflag)
	{
if(m_requestuin)
	{
		
	DenyGroupRequest(m_QQgroupid,m_requestuin,m_timesignal,denyreason);
}
else
{
	
	DenyFrendRequest(m_QQaccount,m_timesignal,denyreason);
}	
	//�ܾ�
	}
}

	//g_welcomestr
	
	OnDelSelf();
}

BOOL CAcceptRequestDlg::OnInitDialog()
{
	CMyBGCDlg::OnInitDialog();
	SetDlgItemText(IDC_content,m_content);
	if(m_realgroupnumber)
{
	QQgrouprule m_grule;
	if(!GRealNumRulemap.Lookup(m_realgroupnumber,m_grule))
m_grule=g_defaultQQgrouprule;

	if(m_grule.g_group_request_method>0)
	{
		if(m_grule.g_group_request_method==2)
CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
		else
CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
	}
}
	else
	{
		if(g_friend_request_method>0)
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1+(g_friend_request_method-1));
	}
	if(g_slientmode)
	{
	
	OnBnClickedOk();
	}
	else
	SetTimer(1,1000,NULL);

return true;
}
void CAcceptRequestDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1)
	{
	
		CString strfmt;
strfmt.Format(_T("������ֶ������Ļ���%d����֮��������Զ���ȷ�����رմ���"),ncount);
	SetDlgItemText(IDC_tenagree,strfmt);
if(ncount<=0)
{	KillTimer(1);
	
}
if(ncount==0)
{
OnBnClickedOk();
return;
}
ncount--;

	}
	

	CDialog::OnTimer(nIDEvent);
}



//
void CAcceptRequestDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnDelSelf();
	//CMyBGCDlg::OnClose();
}


void CAcceptRequestDlg::OnBnClickedCancel()
{
		OnDelSelf();
}
