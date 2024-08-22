// AcceptRequestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PureAcceptRequestDlg.h"
//#include "afxdialogex.h"


extern CKeepMap<INT64,INT64,CPureAcceptRequestDlg*,CPureAcceptRequestDlg*> m_mapqqacdlg;
extern void LoadIgnoreqqlist(BOOL bLoad);
extern CCriticalSection lockqqacdlg;
extern CKeepMap<CString,LPCTSTR,CString,LPCTSTR> g_joinqq_qqword_map;//��Ⱥʱ��д����֤����
extern CKeepMap<CString,LPCTSTR,INT64,INT64> g_joinqq_passqqnum_map;//��Ⱥʱ��д������
extern CCriticalSection lockgopuinres;
extern BOOL ExstractOneNum(LPCTSTR wstr,INT64 &qqnum);
CPureAcceptRequestDlg::CPureAcceptRequestDlg(INT64 qqgroupid,INT64 realgroupid,INT64 requestuin,INT64 qqaccount,const CString& strangerstr,const CString& timesignal,LPCTSTR nickname,LPCTSTR content,int bMale,qqgrouprule tempgrule,CWnd* pParent /*=NULL*/)
	{

	m_bMale=bMale;
	m_qqgroupid=qqgroupid;
	m_requestuin=requestuin;
	m_qqaccount=qqaccount;
	m_ticket=timesignal;
	m_nickname=nickname;
	m_content=content;
	m_stranger=strangerstr;

	m_grule=tempgrule;
	m_realgroupnumber=realgroupid;
}

CPureAcceptRequestDlg::~CPureAcceptRequestDlg()
{
}



// CPureAcceptRequestDlg ��Ϣ�������



void CPureAcceptRequestDlg::OnDelSelf()
{


}

extern BOOL AgreeFriendRequest(INT64 qqaccount,const CString&stranger,const CString& timesignal);
extern BOOL AgreeGroupRequest(INT64 groupuin,INT64 requestuin,const CString& timesignal);
extern BOOL DenyGroupRequest(INT64 groupuin,INT64 requestuin,const CString& timesignal,LPCTSTR reason);
extern BOOL DenyFrendRequest(INT64 qqaccount,const CString& timesignal,LPCTSTR reason);

extern UINT SendWelcome(LPVOID lp);
int CPureAcceptRequestDlg::GetCheckedRadioButton(int radio1,int radio2)
{
return m_radiovalue;
}
void CPureAcceptRequestDlg::CheckRadioButton(int radio1,int radio2,int radio3)
{
	m_radiovalue=radio3;
}

void CPureAcceptRequestDlg::OnBnClickedOk()
{
	BOOL bAgreeFriendRequestflag=false;
	//CString str222;
	//str222.Format(_T("OnBnClickedOkbegin"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

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
		//::function::GetHalfStr(m_content,NULL,&contentbackstr,_T("�������ɣ�"),false);
contentbackstr=m_content;
		contentbackstr.TrimRight(_T("\\\\r"));

	CString admininvitestr;
		CString keystr;
		keystr.Format(_T("%lld_%lld"),m_realgroupnumber,m_qqaccount);
		INT64 realinviteuin=0;
		ExstractOneNum(contentbackstr,realinviteuin);
			lockgopuinres.Lock();
			g_joinqq_qqword_map[keystr]=contentbackstr;

			{
				INT64 tempm_realgroupnumber=0;
				INT64 tempm_qqaccount=0;

					keystr.Format(_T("%lld_%lld"),tempm_realgroupnumber,tempm_qqaccount);
				g_joinqq_qqword_map[keystr]=contentbackstr;
			}

	if(realinviteuin>10000)
	{
	if(realinviteuin>10000||!g_joinqq_passqqnum_map.Lookup(keystr))
	g_joinqq_passqqnum_map[keystr]=realinviteuin;
	}
	lockgopuinres.Unlock();

	if(m_realgroupnumber&&m_qqaccount)
	{



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
			if(FindAlreadyExistqq(m_qqaccount))
			{
				bAlwaysDeny=true;
				denyreason=_T("��ֹ�ظ���Ⱥ");
			}
		}

		if(find(g_forever_blacklist.begin(),g_forever_blacklist.end(),m_qqaccount)!=g_forever_blacklist.end())
		{

			bAlwaysDeny=true;
			denyreason=_T("���Ѿ��������ú��������޷�����");
		}
		if(find(m_grule.g_blacklist.begin(),m_grule.g_blacklist.end(),m_qqaccount)!=m_grule.g_blacklist.end())
		{

			bAlwaysDeny=true;
			denyreason=_T("���Ѿ�����Ⱥ���������޷�����");
		}
		if(!m_grule.g_onlyself)
		{
			if(find(g_defaultqqgrouprule.g_blacklist.begin(),g_defaultqqgrouprule.g_blacklist.end(),m_qqaccount)!=g_defaultqqgrouprule.g_blacklist.end())
			{

				bAlwaysDeny=true;
				denyreason=_T("���Ѿ�������Ⱥ���������޷�����");
			}


		}
	}
if(m_realgroupnumber==0)
{

denyreason=g_friend_welcomemsg;
welcomestr=g_defaultqqgrouprule.g_welcomestr;
	if(find(g_defaultqqgrouprule.g_blacklist.begin(),g_defaultqqgrouprule.g_blacklist.end(),m_qqaccount)!=g_defaultqqgrouprule.g_blacklist.end())
	{
	bAlwaysDeny=true;
	denyreason=_T("���Ѿ�������������޷���Ϊ����");
	}	


}

	//str222.Format(_T("balwaysdenybegin"));
		
	//Write2MagnetLog(str222,&magnetfile,11);
if(bAlwaysDeny)
{
	if(m_requestuin)
	{
		
		DenyGroupRequest(m_qqgroupid,m_requestuin,m_ticket,denyreason);
	}
else
{
	DenyFrendRequest(m_qqaccount,m_ticket,denyreason);
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

	denyreasonstr=g_friend_welcomemsg;
	//2016.12.18,�Ӻ��Ѿܾ����ɶ��ǿ�

	denyreasonstr=_T("");
		codecontent=g_friend_pincodestr;
		}
			BOOL bSameqq=false;
		BOOL regqqnumflag=false;
		BOOL bDirectlyAgree=false;
		BOOL bneedmanualAgree=false;
		BOOL bDirectDeny=false;
		CString rtdenyreason;
		//http://www.qqshow123.com/qqnum?
		//http://buy.cfxy.me/recharge.php?cardseq=
		//�Զ�����robotqqnum��opqqnum��version��
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

	/*	INT64 *pqqnum=new INT64;
*pqqnum=m_requestuin;*/
//AfxBeginThread(ProGetqqMoreProfileInf,pqqnum);

		//str222.Format(_T("progetqqmoreprofileinfbegin"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

//ProGetqqMoreProfileInf(pqqnum);//΢�Ų��ò�

//str222.Format(_T("progetqqmoreprofileinfafter"));
		
	//Write2MagnetLog(str222,&magnetfile,11);
//NickSex rttempnicksex;
if(m_bMale==-1)
{
			CString qqnickname;
	



BOOL bGetGenderok=false;
//if(opvalue==3)
//bGetGenderok=GetUinSingleWholeqqInf(m_qqgroupid,m_requestuin,qqnickname,m_bMale,NULL,50);
//else
//bGetGenderok=GetUinSingleWholeqqInf(m_qqgroupid,m_requestuin,qqnickname,m_bMale,NULL,10);
//
//if(!bGetGenderok)
//{
//	Sleep(1000);
//bGetGenderok=GetUinSingleWholeqqInf(m_qqgroupid,m_requestuin,qqnickname,m_bMale,NULL,5);
//}

//str222.Format(_T("GetUinSingleWholeqqInfafter"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

	}

struct_qqnum_msg tempqqnumpro;
lockqqmoreprofile.Lock();
	g_qqnum_structmoreprofile_map.Lookup(m_requestuin,tempqqnumpro);
	lockqqmoreprofile.Unlock();

	CString localtionstr;
		
		localtionstr.Format(_T("%s %s %s %s"),tempqqnumpro.location_country,tempqqnumpro.location_state,tempqqnumpro.location_city,tempqqnumpro.location_area);
		localtionstr.Trim();


		CString cfqqheadimgurl=tempqqnumpro.headimgurl;

		CString agestr;

		agestr.Format(_T("%s"),tempqqnumpro.age);
		agestr.Trim();
		CString personalstr;
		personalstr.Format(_T("%s"),tempqqnumpro.personal);
		
		if(m_bMale!=1&&m_bMale!=0)
		{

			if(tempqqnumpro.gender==_T("1"))
		m_bMale=1;
	else if(tempqqnumpro.gender==_T("2"))
		m_bMale=0;

		}

	//	if(agestr.IsEmpty())
		//	agestr.Format(_T("%d"),rttempnicksex.age);

//����Ҫ��Ҫ��ȡqq�ȼ���qq������Ϣ
int requireminqqlevel=0;
int requireminqqdarenlevel=0;
requireminqqlevel=m_grule.g_group_pinqqlevel;
requireminqqdarenlevel=m_grule.g_group_pinqqdarenlevel;
	int qqlevel=0;
	int qqdarenlevel=0;
	int qqlikenum=0;
if(requireminqqlevel||requireminqqdarenlevel||bRechargeflag)
{
//��ѯqq�ȼ���qq������Ϣ
		
	
//str222.Format(_T("GetqqlevelFromMapbefore"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

//GetqqlevelFromMap(m_requestuin,qqlevel,qqdarenlevel,qqlikenum,5,false);
//
//if(qqlevel==0&&qqdarenlevel==0)
//{
//GetqqlevelFromMap(m_requestuin,qqlevel,qqdarenlevel,qqlikenum,5,true);
//
//}

//str222.Format(_T("GetqqlevelFromMapafter"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

}

if(IsDefaultGroupOwner(m_qqaccount,true))
{
	regqqnumflag=true;

bDirectlyAgree=true;
	
}
	if(!bDenyflag)
	{
	
		//
	
		if(_tcsicmp(codecontent,_T("ע��qq"))==0||StrStrI(codecontent,_T("http://")))
		{
		//g_additionalsourceweb
		CString Getverifierstr;
		CString tempstr2;

		contentbackstr.TrimLeft(_T("��ע��"));
		contentbackstr.TrimLeft(_T("ע��"));
		contentbackstr.TrimLeft(_T("qq"));
		contentbackstr.TrimLeft(_T("qq"));
		INT64 qqregnum=0;
		if(!bRechargeflag)
		qqregnum=_ttoi64(contentbackstr);
	
		if(qqregnum==0&&!bRechargeflag)
		{
		CString expressstr;
CStringvect temp2vect;

expressstr.Format(_T("\\d{5,20}"));
GetRegEX(contentbackstr,expressstr,temp2vect);//need consider of the actual keyname
if(temp2vect.size()>0)
	qqregnum=_ttoi64(temp2vect[0]);

		}
	/*	if(m_realgroupnumber==229375129)
		if(qqregnum==m_qqaccount)
			bSameqq=true;*/

		if(qqregnum>10000)
		{
			if(_tcsicmp(codecontent,_T("ע��qq"))==0)
			{
		if(m_realgroupnumber!=229375129&&m_realgroupnumber!=226519623&&m_realgroupnumber!=477633103&&m_realgroupnumber!=285960406&&m_realgroupnumber!=555421587)
			Getverifierstr.Format(_T("http://www.qqshow123.com/cfweixinrobot.php?qqnum=%I64d&pppflag=1"),qqregnum);
		else
			Getverifierstr.Format(_T("http://www.qqshow123.com/cfweixinrobot.php?qqnum=%I64d&joincheck=%lld"),qqregnum,m_qqaccount);
			}
			else
			{
				//http://www.qqshow123.com/broadcast3.txt.php?word=
				
			Getverifierstr.Format(_T("%s%lld"),codecontent,qqregnum);
	
			}
		}
		else
		{
		if(_tcsicmp(codecontent,_T("ע��qq"))==0)
						Getverifierstr.Format(_T("http://www.qqshow123.com/cfweixinrobot.php?qqnum=%s"),contentbackstr);
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

CString sendcfqqheadimgurl;

::function::wchrto_utf8(cfqqheadimgurl,msgA);
	
	ddd=msgA.GetLength();
	
		strcpy(bufvalue,(CStringA)msgA);
		
	::function::urlencode(bufvalue);
	
	
sendcfqqheadimgurl=(CString)(CStringA)bufvalue;


	HWND inhwnd=0;
	if(m_realgroupnumber)
		inhwnd=GetCMsgDlgHwndFromGroupNum(m_realgroupnumber,false);
	else
		inhwnd=mainhwnd;
	str222.Format(_T("GetCMsgDlgHwndFromGroupNumafter"));
		
	//Write2MagnetLog(str222,&magnetfile,11);
	CString rtwxid;
	QueryWxidFromHashValue(m_realgroupnumber, m_qqaccount, rtwxid, NULL);
				Getverifierstr.Format(_T("%s%s&cfrobotselfnum=%s&cfrobotwxid=%s&cfrobotselfwxid=%s&cfrobotqqnum=%lld&cfrobotgroupnum=%lld&cfqqheadimgurl=%s&cfqqlevel=%d&cfqqdarenlevel=%d&cfqqlikenum=%d&cfgender=%d&cfrobotnickname=%s&cfrobotloc=%s&cfrobotage=%s&cfrobotpersonal=%s&myhwnd=%d&version=%s&checksign=%s"),codecontent,tbufv,m_ownqqnumber,rtwxid,g_selfwxid,m_qqaccount,m_realgroupnumber,sendcfqqheadimgurl,qqlevel,qqdarenlevel,qqlikenum,m_bMale,sendnickname,sendlocationstr,agestr,sendpersonalstr,inhwnd,g_purenumberversion,g_ksidmd5);
			
			}
				else
	Getverifierstr.Format(_T("%s%s"),codecontent,contentbackstr);
		
		}
		}
			str222.Format(_T("LocalPlugFunctionbefore"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

	if(bLocaldllplugflag)
		LocalPlugFunction(Getverifierstr,tempstr2);
	else
DownloadWebResultTxt(Getverifierstr,tempstr2);

		str222.Format(_T("LocalPlugFunctionafter"));
		
	//Write2MagnetLog(str222,&magnetfile,11);

	int rtvalueint=_ttoi(tempstr2);
if(rtvalueint>=2000)
{
	regqqnumflag=true;

bDirectlyAgree=true;
	
}
else if(rtvalueint>=20&&m_realgroupnumber!=229375129&&m_realgroupnumber!=226519623&&m_realgroupnumber!=477633103&&m_realgroupnumber!=285960406&&m_realgroupnumber!=555421587)
	regqqnumflag=true;
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
if(StrStrI(contentbackstr,_T("ע��qq")))
{
contentbackstr=_T("");
}

if(StrStrI(contentbackstr,codecontent))
{
contentbackstr=_T("");
}


}


		}


		BOOL bMatchRegxflag=false;

		if(IsRegxReply(codecontent))
		{
			if(StrStrI(contentbackstr,codecontent))
				contentbackstr=_T("");

			bMatchRegxflag=MatchStar(codecontent,contentbackstr);
		}
		if(!codecontent.IsEmpty()&&!StrStrI(contentbackstr,codecontent)&&(!bMatchRegxflag)&&(!regqqnumflag))
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
		
		if(m_realgroupnumber&&(qqlevel<requireminqqlevel||qqdarenlevel<requireminqqdarenlevel))
		{
		
		if(!denyreasonstr.IsEmpty())
		{
			bDenyflag=true;
		if(denyreasonstr==_T("�Զ������ܾ�����"))
		{

			bDenyflag=true;

			if(qqlevel==0&&qqdarenlevel==0)
{
	denyreason.Format(_T("δʶ���qq�ȼ�����Ϣ�����Ժ����Լ�Ⱥ��"));
			}
			else
			{
			if(qqlevel<requireminqqlevel&&qqdarenlevel<requireminqqdarenlevel)
denyreason.Format(_T("qq�ȼ�Ҫ����%d����������Ҫ����%d"),requireminqqlevel,requireminqqdarenlevel);
			else if(qqlevel<requireminqqlevel)
			denyreason.Format(_T("qq�ȼ�Ҫ����%d"),requireminqqlevel);
			else if(qqdarenlevel<requireminqqdarenlevel)
				denyreason.Format(_T("qq��������Ҫ����%d"),requireminqqdarenlevel );
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

				//str222.Format(_T("BOOL rtflag=AgreeGroupRequestbefore"));

				//Write2MagnetLog(str222,&magnetfile,11);

				BOOL rtflag=AgreeGroupRequest(m_qqgroupid,m_requestuin,m_ticket);
				CString tempstr=welcomestr;
				tempstr.Trim();
				//str222.Format(_T("BOOL rtflag=AgreeGroupRequestafter"));

				//Write2MagnetLog(str222,&magnetfile,11);
				static INT64 lastqqacount=0;
				if(rtflag)//&&lastqqacount!=m_qqaccount)
				{
					lastqqacount=m_qqaccount;
					BOOL alreadywelcomeflag=false;
					//if(!g_Androidflag)
					{
						Sleep(1500);
						//lockwelcomeqqnumres.Lock();
						//if(g_alreadywelcomeqqnum.find(m_qqaccount)!=g_alreadywelcomeqqnum.end())
						//	alreadywelcomeflag=true;
						//if(!alreadywelcomeflag)
						//g_alreadywelcomeqqnum.insert(m_qqaccount);
						//lockwelcomeqqnumres.Unlock();

						HWND mymsghwnd=GetCMsgDlgHwndFromGroupNum(m_qqgroupid);

						//str222.Format(_T("HWND mymsghwnd=GetCMsgDlgafter"));

						//Write2MagnetLog(str222,&magnetfile,11);

						if(m_qqaccount)
						{
							CString cardname;
							GetNickNameFromGroupMap(m_realgroupnumber,m_qqaccount,cardname);
							::SendMessage(mymsghwnd,WM_NEWJOIN,(WPARAM)cardname.GetBuffer(0),(LPARAM)&m_qqaccount);
						}
						//		alreadywelcomeflag=::SendMessage(mymsghwnd,WM_ALREADYWELCOME,(WPARAM)1,(LPARAM)&m_qqaccount);

						//if(1)//!alreadywelcomeflag)
						//{
						//GetGlobalMyRank(m_realgroupnumber,m_qqaccount);
						//SetProbationTime(m_realgroupnumber,m_qqaccount);
						//
						//
						//}



					}
				}
			}
	else
	{
		bAgreeFriendRequestflag=true;
	AgreeFriendRequest(m_qqaccount,m_stranger,m_ticket);
	
	}
		}
	}

	if(bDenyflag)
	{
if(m_qqgroupid)
{
		
		//str222.Format(_T("lastDenyGroupRebefore"));
		
	//Write2MagnetLog(str222,&magnetfile,11);
		DenyGroupRequest(m_qqgroupid,m_requestuin,m_ticket,denyreason);

	//str222.Format(_T("lastDenyGroupReafter"));
		
	//Write2MagnetLog(str222,&magnetfile,11);
}
else
{
	
	DenyFrendRequest(m_qqaccount,m_ticket,denyreason);
}	
	//�ܾ�
	}
}


if(!bAgreeFriendRequestflag)
{
	CString username=GetFriendUsernameFromHashvalue(0,m_qqaccount);
	lockfriendusername2nickres.Lock();
	g_wxfriendusername2nickname.RemoveKey(username);
	lockfriendusername2nickres.Unlock();
//���Ǹ�ɾ��
}
	//g_welcomestr
	
	//str222.Format(_T("okdone"));
		
	//Write2MagnetLog(str222,&magnetfile,11);
	//OnDelSelf();
}

BOOL CPureAcceptRequestDlg::OnInitDialog()
{
	
	if(m_realgroupnumber)
{
	qqgrouprule m_grule;
	if(!GRealNumRulemap.Lookup(m_realgroupnumber,m_grule))
m_grule=g_defaultqqgrouprule;

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
	if(1)
	{
	
	OnBnClickedOk();
	}
	

return true;
}
