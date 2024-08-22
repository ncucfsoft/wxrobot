#include "stdafx.h"  
#include <afxsock.h>
#include <iostream>  
#include <string>  
#include <vector>  
#include "openssl/rc4.h"
#include "publicshare.h"  
#include "encrypt.h"  
#include "_pack.h"
#include "_unpack.h"
#include <detours.h>  
#pragma comment(lib,"libeay32.lib")
#pragma comment(lib,"ssleay32.lib" 
//#include <Tlhelp32.h>
//#include <Psapi.h>
//#pragma comment(lib, "Psapi.lib")
//#pragma comment(lib,"ws2_32.lib")
using namespace std;  
CEvent FindOverevent;
CString g_selfwxid;
extern BOOL g_usingremarkname;
extern void SendFromCMsgDlg(INT64 qqaccount,const CString& backstr);
extern BOOL ExstractOneNum(LPCTSTR wstr,INT64 &qqnum);
extern int g_autopassjoinpaymentvalue;
extern CString g_agreeinviteweb;
extern CWinThread *mywinthread5;
extern UINT proPbSendMsgFun(LPVOID lp);
extern CString g_plusdir;
extern BOOL g_newestversion;
extern BOOL g_versionnewest27;
extern BOOL g_forbidautorec;
extern CStringvect backbufvect;
extern CCriticalSection lockqqimgres;
extern void TranslateUbuffer(LPCTSTR orgbuf,OUT TCHAR *newbuf);
extern CKeepMap<INT64,INT64,CString,LPCTSTR> g_qq2lastimgmap;
extern   int CheckWhetherAgreeinvite(INT64 groupuin,INT64 opuin,INT64 robotqqnum);
extern BOOL IsDefaultGroupOwner(INT64 uin,BOOL bTrueqq);
extern BOOL g_autoagreeinvite;
extern BOOL g_maxgroupnumflag;
CKeepMap<CString,LPCTSTR,CString,LPCTSTR> g_wxidjoinstrpay_map;
CCriticalSection lockjoinstrres;
void SendpackForGetA8Key(LPCTSTR wxid,LPCTSTR url);
void FunForRecognize(CString& localfile,INT64 groupuin,INT64 sendqq,Wxretdata& mywxretdata);
BOOL g_latestwechatflag=true;
int g_readportnum=7087;
	BOOL bShowatmsg=false;
PROCESS_INFORMATION g_wxpi;
DWORD g_wxpid=0;
	char g_WxInstallPath[MAX_PATH] = {0};
//extern BOOL SetPrivilege();
vectorbyte RC4_Encrypt(const vectorbyte cleartext, const vectorbyte key)  
{  
    RC4_KEY rc4key;  
    unsigned char* tmp = new unsigned char[cleartext.size() + 1];  
    memset(tmp, 0, cleartext.size() + 1);  
  
    RC4_set_key(&rc4key, key.size(), (const unsigned char*)&key[0]);  
    RC4(&rc4key, cleartext.size(), (const unsigned char*)&cleartext[0], tmp);  
  
	vectorbyte str;
	//= (unsigned char*)tmp;
	
	str.assign((byte*)tmp,(byte*)tmp+cleartext.size());

    delete [] tmp;  
  
    return str;  
}  
  
string RC4_Decrypt(const string ciphertext, const string key)  
{  
    RC4_KEY rc4key;  
    unsigned char* tmp = new unsigned char[ciphertext.length() + 1];  
    memset(tmp, 0, ciphertext.length() + 1);  
  
    RC4_set_key(&rc4key, key.length(), (const unsigned char*)key.c_str());  
    RC4(&rc4key, ciphertext.length(), (const unsigned char*)ciphertext.c_str(), tmp);  
  
    string str = (char*)tmp;  
  
    delete [] tmp;  
  
    return str;  
}

SOCKET listen_sock;  
//SOCKET sock;
const int maxportnum=30;
int FindAvailablePort(int port)
{
	int rtport=0;
  SOCKADDR_IN local_addr;  
     SOCKADDR_IN client_addr;  
     int iaddrSize = sizeof(SOCKADDR_IN);  
   
   //  CServerDlg * dlg = (CServerDlg *)AfxGetApp()->GetMainWnd();  
     local_addr.sin_family = AF_INET;  
    
     local_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//htonl(INADDR_ANY);  
   
     if( (listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )  
     {  
      //  dlg->Update("创建监听失败");  
     }  
	 int nowport=port;
	 BOOL bFindOK=false;
	 for(int i=0;i<maxportnum;i++)
	 {
		 local_addr.sin_port = htons(nowport);  //7071
		 if(::bind(listen_sock,(struct sockaddr*) &local_addr,sizeof(SOCKADDR_IN)))
		 {
			 nowport++;
			 //  dlg->Update("绑定错误");  
		 }
		 else
		 {
			 bFindOK=true;
			 rtport=nowport;
			 break;
		 
		 }
	 }

	 if(!bFindOK)
	 {
		 rtport=0;
		 return rtport;
	 
	 }
	 else
	 {
		 CString  nowportstr=::function::CFSoft_path(_T("communication.bin"),NULL,0);//::function::complete_path(_T("机器人插件\\communication.bin"));
		 CFile cook_file1;

		 g_readportnum=rtport;
		 if(cook_file1.Open(nowportstr,CFile::modeWrite|CFile::modeCreate))
		 {
			 cook_file1.Write(&g_readportnum,sizeof(int));

			 cook_file1.Close();
		 }
		 else
		 {

			 MessageBox(mainhwnd,_T("未能写入配置文件communication.bin！"),_T("无法同步通讯"),0);


		 }

	 }
	 FindOverevent.SetEvent();
    listen(listen_sock, 1);  
     if( (g_socket = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)  
     {  
       // dlg->Update("accept 失败");  
     }  
	 return rtport;
}
extern int ReceiveAll(char *pch, const int nSize, const int nSecs);
BOOL Fun_Org_recv(vectorbyte& tempbytev)
{
	//tempbytev.resize(2000*1024);
	tempbytev.resize(4);

	int outlen=ReceiveAll((char*)&tempbytev[0],4,5);

if(outlen!=-1)
{
	int len=*((int*)&tempbytev[0]);
	//len=htonl(len);
	int maxlen=15000*1024;
	if(!g_resetmaxlen)
	{
		if(g_maxgroupnumflag)
maxlen=15000*1024;
		else
		maxlen=1000*1024;
	}

	if(len<maxlen)
	{
		tempbytev.resize(len);
	outlen=ReceiveAll((char*)&tempbytev[0],len,5);
	}
	else
	{
			static int count=0;
		if(count<3)
		{
			count++;
			
			CString lenstr;
		lenstr.Format(_T("数据通讯异常，此机器人加的微信群或好友是不是过多？可以尝试关闭微信客户端后重新启动机器人软件（如果微信客户端自动升级到最新版了，请重新执行安装微信电脑客户端2.5版）"),len);//，有空的话可以顺手告知作者QQ：13402429  字节大小:%d
		CString logpathmax;
		logpathmax.Format(_T("%lld微信群和好友数目超多标志.txt"),g_interfaceuin);
		logpathmax=::function::complete_path(logpathmax);


	CReadAUStdioFile file1;
	if(file1.Open(logpathmax,CFile::modeCreate|CFile::modeWrite))
	{
		file1.WriteLineStr(_T("可能你的当前微信号加了很多好友或微信群，导致每次通讯的数据量很大，需要每次接收超多数据，所以会自动创建此文件，增加每次通讯字节数上限。"));
		file1.Close();
	}
			AfxMessageBox(lenstr);

		}
	
	
	tempbytev.clear();
	return 0;
	}
	if(outlen!=-1)
	{
//tempbytev.resize(len);
return true;
	}
	else
	{
tempbytev.clear();
	
	}

	}
	else
	{
	tempbytev.clear();
	}
	return false;
	
}
void Transfer2InitOver(_Unpack& unPack)
{
	if(g_latestwechatflag)
	{
		g_selfwxname=unPack.GetTokenstr();		
		g_selfwxphone=unPack.GetTokenstr();
		//g_selfwxuser=unPack.GetTokenstr();
		//g_selfwxid=unPack.GetTokenstr();
	}
	else
	{
		g_selfwxname=unPack.GetTokenstr();
		g_selfwxuser=unPack.GetTokenstr();
		g_selfwxid=unPack.GetTokenstr();
		g_selfwxphone=unPack.GetTokenstr();
	
	}
		g_interfaceuin=StrToHash(g_selfwxid);
		m_ownqqnumber.Format(_T("%lld"),g_interfaceuin);
		g_lastownqq.Format(_T("%lld"),g_interfaceuin);
		g_wxuin.Format(_T("%lld"),g_interfaceuin);
		g_selfusername=g_selfwxid;
		Client_Wxid=g_selfwxid;
		g_selfnickname=g_selfwxname;

		

		SendMessage(mainhwnd,WM_SIGNEDIN,0,0);
}
BOOL IsPublicWxNum(const CString& wxid,BOOL bAllType)
{
	if(bAllType&&wxid==_T("fmessage"))
		return true;

	if(wxid==_T("floatbottle"))
		return true;
	
	if(wxid==_T("filehelper"))
		return true;
	
	if(wxid==_T("qmessage"))
		return true;
	
	if(wxid==_T("qqmail"))
		return true;
	
	if(wxid==_T("tmessage"))
		return true;
if(wxid==_T("mphelper"))
		return true;

if(wxid.Find(_T("gh_3dfda90e39d6"))==0)
return false;
if(wxid.Find(_T("gh_"))==0)
return true;

return false;
}
void Transfer2FrendList(cJSON* RecvJson)
{
	
					UserInfo uInfo = {0};
				char* RecvWxid=NULL;

				cJSON* item=NULL;
				item= cJSON_GetObjectItem(RecvJson,"mywxid");
				if(item)
					RecvWxid= item->valuestring;
				
				cJSON* UserArray = cJSON_GetObjectItem(RecvJson,"list");
				int uCount=0;
				if(UserArray)
				uCount= cJSON_GetArraySize(UserArray);
				//for (int i=0;i<uCount;i++)
				//{
				//	
				//	if (uInfo.Type == 8 || uInfo.Type == 24 || uInfo.Type == 29)
				//	{
				//		int Item = ListFriend_GH->InsertItem(ListFriend_GH->GetItemCount(),uInfo.Nick);
				//		ListFriend_GH->SetItemText(Item,1,uInfo.Wxid);
				//		ListFriend_GH->SetItemText(Item,2,uInfo.WxNo);
				//		ListFriend_GH->SetItemText(Item,3,uInfo.HImg);
				//	}
				//	else if (strstr(uInfo.Wxid,"@chatroom") != NULL)
				//	{
				//		if (strcmp(uInfo.Nick,"") == 0)
				//			uInfo.Nick = "群聊";
				//		int Item = ListGroup->InsertItem(ListGroup->GetItemCount(),uInfo.Nick);
				//		ListGroup->SetItemText(Item,1,uInfo.Wxid);
				//		ListGroup->SetItemText(Item,2,uInfo.WxNo);
				//		ListGroup->SetItemText(Item,3,uInfo.Int2str(uInfo.List));
				//		ListGroup->SetItemText(Item,4,uInfo.HImg);
				//	}
				//	else if (uInfo.List != 2)
				//	{
				//		int Item = ListFriend->InsertItem(ListFriend->GetItemCount(),uInfo.Nick);
				//		ListFriend->SetItemText(Item,1,uInfo.Mark);
				//		ListFriend->SetItemText(Item,2,uInfo.Wxid);
				//		ListFriend->SetItemText(Item,3,uInfo.WxNo);
				//		ListFriend->SetItemText(Item,4,uInfo.HImg);
				//	}
				//	//cJSON_Delete(uUser);
				//}
		

		int nowfriendpos=0;
		int bChatroomflag=false;
		struct_wxsingleuserinf tempsingleinf;
		for(int i=0;i<uCount;i++)
		{
			cJSON* uUser = cJSON_GetArrayItem(UserArray,i);
			if(!uUser)
			continue;

			 cJSON* item= cJSON_GetObjectItem(uUser,"Type");
				if(item)
					uInfo.Type= item->valueint;

					//uInfo.Type = cJSON_GetObjectItem(uUser,"Type")->valueint;
				item= cJSON_GetObjectItem(uUser,"List");
				if(item)
					uInfo.List= item->valueint;
					//uInfo.List = cJSON_GetObjectItem(uUser,"List")->valueint;

				item= cJSON_GetObjectItem(uUser,"Wxid");
				if(item)
					uInfo.Wxid= item->valuestring;

				//	uInfo.Wxid = cJSON_GetObjectItem(uUser,"Wxid")->valuestring;

				item= cJSON_GetObjectItem(uUser,"WxNo");
				if(item)
					uInfo.WxNo= item->valuestring;

					//uInfo.WxNo = cJSON_GetObjectItem(uUser,"WxNo")->valuestring;

				item= cJSON_GetObjectItem(uUser,"Nick");
				if(item)
					uInfo.Nick= item->valuestring;

					//uInfo.Nick = cJSON_GetObjectItem(uUser,"Nick")->valuestring;

				item= cJSON_GetObjectItem(uUser,"Mark");
				if(item)
					uInfo.Mark= item->valuestring;
					//uInfo.Mark = cJSON_GetObjectItem(uUser,"Mark")->valuestring;

					item= cJSON_GetObjectItem(uUser,"HImg");
				if(item)
					uInfo.HImg= item->valuestring;
					//uInfo.HImg = cJSON_GetObjectItem(uUser,"HImg")->valuestring;

					if (uInfo.Type == 8 || uInfo.Type == 24 || uInfo.Type == 29)
						continue;
					if(uInfo.Nick)
					{

			tempsingleinf.nickname=uInfo.Nick;
			tempsingleinf.nickname.Replace(0xc2a0,' ');
			tempsingleinf.nickname.Trim();

				if(tempsingleinf.nickname.Find(_T("\\u"))!=-1)
		{
	TCHAR newbuf[2048*10];
TranslateUbuffer(tempsingleinf.nickname,newbuf);
tempsingleinf.nickname=newbuf;
		}

					}
			if(uInfo.WxNo)
			tempsingleinf.user=uInfo.WxNo;
			if(uInfo.Wxid)
			tempsingleinf.wxid=uInfo.Wxid;
			//tempsingleinf.v1=unPack.GetTokenstr();
			if(g_usingremarkname)//uInfo.Mark)
			{
			tempsingleinf.remark=uInfo.Mark;

				if(tempsingleinf.remark.Find(_T("\\u"))!=-1)
		{
	TCHAR newbuf[2048*10];
TranslateUbuffer(tempsingleinf.remark,newbuf);
tempsingleinf.remark=newbuf;
		}

			}	
			if(uInfo.HImg)
			tempsingleinf.HeadImgUrl=uInfo.HImg;

			tempsingleinf.type=uInfo.Type;
			
			
			if(::function::TrimRight(tempsingleinf.wxid,_T("@chatroom"),true))
				bChatroomflag=true;
			else
				bChatroomflag=false;

			/*if(!bChatroomflag&&tempsingleinf.type==2)
				continue;*/

			if(IsPublicWxNum(tempsingleinf.wxid,true))
				continue;

			CString rtnickname;
			rtnickname=tempsingleinf.nickname;
			if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
			{
				if(g_usingremarkname)
			rtnickname=tempsingleinf.remark;
				if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
				{
					if(!tempsingleinf.user.IsEmpty())
						tempsingleinf.nickname=tempsingleinf.user;
					else
						tempsingleinf.nickname=tempsingleinf.wxid;

				}
				else
					tempsingleinf.nickname=rtnickname;

			}
			//g_wxfriend_singleinf[tempsingleinf.wxid]=tempsingleinf;
			UpdateInfFromWxid(0,tempsingleinf);
			
			if(bChatroomflag)
			{
				g_wxgroupusername2nickname[tempsingleinf.wxid]=tempsingleinf.nickname;
				g_wxgroupnickname2username[tempsingleinf.nickname]=tempsingleinf.wxid;
			}
			else
			{
				g_wxfriendusername2nickname[tempsingleinf.wxid]=tempsingleinf.nickname;
			}

		}

		if(1)
		{
			CFile file1;

			if(file1.Open(::function::complete_path(_T("wx好友列表.txt")),CFile::modeCreate|CFile::modeWrite))
			{
				CStringA totalstr;
				CStringA tempstr;
				

				file1.Write(RecvJson,sizeof(*RecvJson));
				file1.Close();
			}
		}
		g_readFriendOver=true;
		SendMessage(mainhwnd,WM_GETALLGROUPOVER,0,0);

		

}

void Transfer2FrendList(_Unpack& unPack)
{
	
	CString curwxid;
	if(!g_latestwechatflag)
		curwxid=unPack.GetTokenstr();
		int	friendnum=unPack.GetInt();

		//g_wxfriendusername2nickname
		//g_wxfriendusernamewxnsc
		//g_wxfriendwxuser2wxid

		int nowfriendpos=0;
		int bChatroomflag=false;
		struct_wxsingleuserinf tempsingleinf;
		for(int i=0;i<friendnum;i++)
		{
			
			nowfriendpos=unPack.GetInt();
			if(nowfriendpos==0)
				break;
			tempsingleinf.nickname=unPack.GetTokenstr();
			tempsingleinf.nickname.Replace(0xc2a0,' ');
			tempsingleinf.nickname.Trim();
			tempsingleinf.user=unPack.GetTokenstr();
			tempsingleinf.wxid=unPack.GetTokenstr();
			tempsingleinf.v1=unPack.GetTokenstr();
			tempsingleinf.remark=unPack.GetTokenstr();
			if(g_latestwechatflag)
			{
			tempsingleinf.HeadImgUrl=unPack.GetTokenstr();
			tempsingleinf.type=unPack.GetInt();
			
			
			}
			if(::function::TrimRight(tempsingleinf.wxid,_T("@chatroom"),true))
				bChatroomflag=true;
			else
				bChatroomflag=false;

			/*if(!bChatroomflag&&tempsingleinf.type==2)
				continue;*/

			if(IsPublicWxNum(tempsingleinf.wxid,true))
				continue;

			CString rtnickname;
			rtnickname=tempsingleinf.nickname;
			if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
			{
				if(g_usingremarkname)
				rtnickname=tempsingleinf.remark;
				if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
				{
					if(!tempsingleinf.user.IsEmpty())
						tempsingleinf.nickname=tempsingleinf.user;
					else
						tempsingleinf.nickname=tempsingleinf.wxid;

				}
				else
					tempsingleinf.nickname=rtnickname;

			}
			//g_wxfriend_singleinf[tempsingleinf.wxid]=tempsingleinf;
			UpdateInfFromWxid(0,tempsingleinf);
			
			if(bChatroomflag)
			{
				g_wxgroupusername2nickname[tempsingleinf.wxid]=tempsingleinf.nickname;
				g_wxgroupnickname2username[tempsingleinf.nickname]=tempsingleinf.wxid;
			}
			else
			{
				g_wxfriendusername2nickname[tempsingleinf.wxid]=tempsingleinf.nickname;
			}

		}
		g_readFriendOver=true;
		SendMessage(mainhwnd,WM_GETALLGROUPOVER,0,0);

		//if(1)
		//{
		//	CFile file1;

		//	if(file1.Open(::function::complete_path(_T("wx好友列表.txt")),CFile::modeCreate|CFile::modeWrite))
		//	{
		//		CStringA totalstr;
		//		CStringA tempstr;
		//		for(int i=0;i<data.size();i++)
		//		{
		//			tempstr.Format("%02X ",data[i]);
		//			totalstr+=tempstr;
		//		}

		//		file1.Write(totalstr.GetBuffer(0),totalstr.GetLength());
		//		file1.Close();
		//	}
		//}

}
void Transfer2Groupmember(cJSON* RecvJson)
{
	
		CString curgroupwxid;
		struct_wxsingleuserinf tempsingleinf;

		int	friendnum=0;



		CString tempnickname,tempuser,tempwxid,tempv1,tempremark;
		int nowfriendpos=0;
		set<INT64> tempint64vect;
		int actualmembernum=0;
		//set<CString> curgroupsetwxid;

		
		
	UserInfo uInfo = {0};
			//	char* RecvWxid = cJSON_GetObjectItem(RecvJson,"wxid")->valuestring;
				char* RecvGroup=NULL;
				cJSON* item= cJSON_GetObjectItem(RecvJson,"gid");
				if(item)
					RecvGroup=item->valuestring;
				curgroupwxid=RecvGroup;
				INT64 curgroupid=GetGroupIDFromGroupWxid(curgroupwxid);

				cJSON* UserArray = cJSON_GetObjectItem(RecvJson,"list");
				if (UserArray)
				{
					friendnum = cJSON_GetArraySize(UserArray);
				}

				
			
		if(friendnum)
		{
				lockgqqnickmapresource.Lock();
		if(GrouprealqqNickmap.Lookup(curgroupid))
			GrouprealqqNickmap[curgroupid]->RemoveAll();
				lockgqqnickmapresource.Unlock();
		}
		for(int i=0;i<friendnum;i++)
		{
			cJSON* uUser = cJSON_GetArrayItem(UserArray,i);
			cJSON* item=cJSON_GetObjectItem(uUser,"Wxid");
			if(item)
						uInfo.Wxid = item->valuestring;
			item=cJSON_GetObjectItem(uUser,"Nick");
			if(item)
						uInfo.Nick = item->valuestring;

			item=cJSON_GetObjectItem(uUser,"Mark");
			if(item)
						uInfo.Mark = item->valuestring;

			item=cJSON_GetObjectItem(uUser,"HImg");
			if(item)
						uInfo.HImg  = item->valuestring;

						//uInfo.Mark = cJSON_GetObjectItem(uUser,"Mark")->valuestring;
						//uInfo.HImg = cJSON_GetObjectItem(uUser,"HImg")->valuestring;

			actualmembernum++;
			if(uInfo.Nick)
			{
			tempsingleinf.nickname=uInfo.Nick;
				
		if(tempsingleinf.nickname.Find(_T("\\u"))!=-1)
		{
	TCHAR newbuf[2048*10];
TranslateUbuffer(tempsingleinf.nickname,newbuf);
tempsingleinf.nickname=newbuf;
		}
			tempsingleinf.nickname.Trim();
			}
			//tempsingleinf.user=unPack.GetTokenstr();
			if(uInfo.Wxid)
			tempsingleinf.wxid=uInfo.Wxid;
			//tempsingleinf.v1=unPack.GetTokenstr();
			if(g_usingremarkname)//uInfo.Mark)
			tempsingleinf.remark=uInfo.Mark;
			
			if(uInfo.HImg)
			tempsingleinf.HeadImgUrl=uInfo.HImg;
			
			CString rtnickname;
			rtnickname=tempsingleinf.nickname;
			if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
			{
				if(g_usingremarkname)
				rtnickname=tempsingleinf.remark;
				if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
				{
					if(!tempsingleinf.user.IsEmpty())
						tempsingleinf.nickname=tempsingleinf.user;
					else
						tempsingleinf.nickname=tempsingleinf.wxid;

				}
				else
					tempsingleinf.nickname=rtnickname;

			}
			//curgroupsetwxid.insert(tempsingleinf.wxid);

			UpdateInfFromWxid(curgroupid,tempsingleinf);

			tempint64vect.insert(QueryHashValueFromWxid(curgroupid,tempsingleinf.wxid));
			//g_wxgroupid_wxid_singleinf[curgroupwxid+tempsingleinf.wxid]=tempsingleinf;


		}

		getonegroupmemberevent.SetEvent();
		lockggroupmembercountsource.Lock();
		g_group_membercountmap[curgroupid]=actualmembernum;
		lockggroupmembercountsource.Unlock();

		lockggroup_realqqsetmap.Lock();
		g_group_realqqvectMap[curgroupid]=tempint64vect;
		lockggroup_realqqsetmap.Unlock();
		//g_wxgroup_setwxid[curgroupwxid]=curgroupsetwxid;


	
}

void Transfer2Groupmember(_Unpack& unPack)
{
	CString curwxid;
	if(!g_latestwechatflag)
	curwxid=unPack.GetTokenstr();

		CString curgroupwxid=unPack.GetTokenstr();
		struct_wxsingleuserinf tempsingleinf;

		int	friendnum=unPack.GetInt();



		CString tempnickname,tempuser,tempwxid,tempv1,tempremark;
		int nowfriendpos=0;
		set<INT64> tempint64vect;
		int actualmembernum=0;
		//set<CString> curgroupsetwxid;

		INT64 curgroupid=GetGroupIDFromGroupWxid(curgroupwxid);
		if(friendnum)
		{
				lockgqqnickmapresource.Lock();
		if(GrouprealqqNickmap.Lookup(curgroupid))
			GrouprealqqNickmap[curgroupid]->RemoveAll();
				lockgqqnickmapresource.Unlock();
		}
		for(int i=0;i<friendnum;i++)
		{
			nowfriendpos=unPack.GetInt();
			if(nowfriendpos==0)
				break;
			actualmembernum++;
			tempsingleinf.nickname=unPack.GetTokenstr();
			tempsingleinf.nickname.Replace(0xc2a0,' ');
			tempsingleinf.nickname.Trim();
			tempsingleinf.user=unPack.GetTokenstr();
			tempsingleinf.wxid=unPack.GetTokenstr();
			tempsingleinf.v1=unPack.GetTokenstr();
			tempsingleinf.remark=unPack.GetTokenstr();
			if(g_latestwechatflag)
			{
			tempsingleinf.HeadImgUrl=unPack.GetTokenstr();
			tempsingleinf.type=unPack.GetInt();
			
			
			}
			CString rtnickname;
			rtnickname=tempsingleinf.nickname;
			if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
			{
				//rtnickname=tempsingleinf.remark;
				if(rtnickname.IsEmpty()||rtnickname[0]==0x3000)
				{
					if(!tempsingleinf.user.IsEmpty())
						tempsingleinf.nickname=tempsingleinf.user;
					else
						tempsingleinf.nickname=tempsingleinf.wxid;

				}
				else
					tempsingleinf.nickname=rtnickname;

			}
			//curgroupsetwxid.insert(tempsingleinf.wxid);

			UpdateInfFromWxid(curgroupid,tempsingleinf);

			tempint64vect.insert(QueryHashValueFromWxid(curgroupid,tempsingleinf.wxid));
			//g_wxgroupid_wxid_singleinf[curgroupwxid+tempsingleinf.wxid]=tempsingleinf;


		}

		getonegroupmemberevent.SetEvent();
		lockggroupmembercountsource.Lock();
		g_group_membercountmap[curgroupid]=actualmembernum;
		lockggroupmembercountsource.Unlock();

		lockggroup_realqqsetmap.Lock();
		g_group_realqqvectMap[curgroupid]=tempint64vect;
		lockggroup_realqqsetmap.Unlock();
		//g_wxgroup_setwxid[curgroupwxid]=curgroupsetwxid;


		/*if(1)
		{
		CFile file1;

		if(file1.Open(::function::complete_path(_T("wx群成员列表.txt")),CFile::modeCreate|CFile::modeWrite))
		{
		CStringA totalstr;
		CStringA tempstr;
		for(int i=0;i<data.size();i++)
		{
		tempstr.Format("%02X ",data[i]);
		totalstr+=tempstr;
		}

		file1.Write(totalstr.GetBuffer(0),totalstr.GetLength());
		file1.Close();
		}
		}*/
}

void GetVectMidParamFromBetweenWebBuf(LPCTSTR buf,LPCTSTR targetstr1,LPCTSTR targetstr2,CStringvect &rtvect)
{
	CString transcationid;
	int startpos=0;
	CString newbuf=buf;
	while(startpos=::function::GetSingleMidParamFromBetweenWebBuf(newbuf,targetstr1,targetstr2,transcationid))
	{
		newbuf=newbuf.Mid(startpos);
		rtvect.push_back(transcationid);
	}


}
HWND creategroupm_hwnd=0;
INT64 curcreategroupuin=0;
void Transfer2windowwxmsgsForCreateGroup(_Unpack& bf)
{
	

	CString strrr = (CString)bf.GetAll_Hex();
CString GroupWxid=bf.GetTokenstr();
if (GroupWxid.Find(_T("@chatroom")) == -1)
{
	CString strtxtcontent=_T("自动创建新群返回结果出错，可能你选择的群友中有已经删除了你好友的人。");

	::SendMessage(creategroupm_hwnd, WM_PLUGRESPONSE, (WPARAM)strtxtcontent.GetBuffer(0), (LPARAM)&curcreategroupuin);

}
else
{

	int num = bf.GetInt();
	for (int i = 0; i < num; i++)
	{
		int nownum = bf.GetInt();
		if (nownum)
		{
			int type = bf.GetInt();
			CString wxid;
			wxid=bf.GetTokenstr();
		}
		else
			break;
	}
	CString strtxtcontent ;
	strtxtcontent.Format(_T("自动创建新群返回结果成功，新群群号为：%s。成员数量为%d。"), GroupWxid,num);

	::SendMessage(creategroupm_hwnd, WM_PLUGRESPONSE, (WPARAM)strtxtcontent.GetBuffer(0), (LPARAM)&curcreategroupuin);

}

}
void Transfer2windowwxmsgsFor2526(int Type,int MsgID,LPCTSTR SendWxid,LPCTSTR RecvWxid,LPCTSTR Msg,LPCTSTR atlist,int Time)
{
	
	

	CString str;
	int paysubtype=0;
	CString transferid;
	CString v1;
	CString v2;
	CString scene;



if(IsPublicWxNum(SendWxid,false))
				return;




CString msgtousername=RecvWxid;
	setvector<Wxretdata> mywxretdatasetvect;
	Wxretdata tempwxretdata;
	tempwxretdata.MsgType=Type;
	tempwxretdata.Content=Msg;
	tempwxretdata.FromUserName=SendWxid;
	tempwxretdata.NewMsgId=(INT64)MsgID;
	tempwxretdata.CreateTime=Time;
	//tempwxretdata.FromGroupName=sendwxid;
	CString msgfromgroupname=SendWxid;
	CString tempcontent=Msg;
	if(msgfromgroupname.Find(_T("@chatroom"))!=-1)
	{
					tempwxretdata.FromGroupName=msgfromgroupname;//"@195bff9b154a53ac4ed5495d2eb3f56c58e60983f40335cd2acf0ed55a36c044:
					if(g_newestversion)
					{
						if(lstrcmp(RecvWxid,_T("-"))!=0)
					tempwxretdata.FromUserName=RecvWxid;

					}
					else
					{
					int pos=tempcontent.Find(':');
					if(pos!=-1)
					{
						tempwxretdata.FromUserName=tempcontent.Left(pos);
					}
					tempwxretdata.Content=tempcontent.Mid(pos+1);
					}

				}
				else
				{
					tempwxretdata.FromUserName=(CString)msgfromgroupname;
					if(g_newestversion)
					{
					if(msgtousername.Find(_T("@chatroom"))!=-1)//如果是自己另外一个客户端发出的消息
						tempwxretdata.FromGroupName=msgtousername;
					}
					else
					{
					if(g_latestwechatflag)
					{
						if(msgtousername.Find(_T("@chatroom"))!=-1)//如果是自己另外一个客户端发出的消息
						tempwxretdata.FromGroupName=msgtousername;
					}
					}
					tempwxretdata.Content=tempcontent;
				}

	if(tempwxretdata.MsgType==49)
	{

		/*.版本 2

			.如果真(strfind(Msg,“邀请你加入群聊”) ≠-1)
			str ＝ 取文本中间内容(Msg,“<url><![CDATA[”,“]]></url>”)
			.如果真(选择框_自动同意进群邀请.选中 ＝ 真)
			功能_GetA8Key(m_Socket,AnsiToUnicode(SendWxid),AnsiToUnicode(str))*/
					if(msgfromgroupname.Find(_T("@chatroom"))==-1&&StrStr(Msg,_T("邀请你加入群聊")))
			{

	//MessageBox(0,_T("收到邀请你加入群聊的消息"),0,0);
								int bAgreeFromWeb=0;
								INT64 opuin=GetFriendUidFromUserName(0,SendWxid);
								//在这里判断有没有收到转账
		
	CString str;
							::function::GetSingleMidParamFromBetweenWebBuf(Msg,_T("<url><![CDATA["),_T("]]></url>"),str);

				if(::function::IsChinese(g_agreeinviteweb))
				{
	INT64 tempvalue=1;
ExstractOneNum(g_agreeinviteweb,tempvalue);
g_autopassjoinpaymentvalue=tempvalue;



SendFromCMsgDlg(opuin,g_agreeinviteweb);

							//Android_Send1Msg(0,opuin,g_agreeinviteweb);
							lockjoinstrres.Lock();
							g_wxidjoinstrpay_map[SendWxid]=str;
							lockjoinstrres.Unlock();
				}
				else
				{
							if(g_autoagreeinvite||IsDefaultGroupOwner(opuin,true))
bAgreeFromWeb=CheckWhetherAgreeinvite(0,opuin,g_interfaceuin);


		if(bAgreeFromWeb>=20)//如果是已经开启的群，自动同意
		{
	
		if(bAgreeFromWeb>=2000)
		{
		
			
						//	MessageBox(0,str,0,0);
							SendpackForGetA8Key(SendWxid,str);

			//Fun_send(Pack_AgreeOrDenyJoinMsg(tempsgmsg.groupUin,tempsgmsg.sendUin,timesignal,NULL,true,true));
		
			

						

		//CString groupnickname;
		//groupnickname.Format(_T("%lld"),tempsgmsg.groupUin);
			//HWND mymsghwnd=GetCMsgDlgHwndFromGroupNum(tempsgmsg.groupUin,true);

	//CreateQQMsgDlg(tempsgmsg.groupUin,0,groupnickname,true,false,0,true);

		}
		}
				}
	
					}
			else if(StrStr(Msg,_T("CDATA[微信转账]")))
			{
			CString paysubtypestr;
			CString transferid;
			CString str;
		//	CString feestr;
			::function::GetSingleMidParamFromBetweenWebBuf(Msg,_T("<paysubtype>"),_T("</paysubtype>"),paysubtypestr);
			int paysubtype=_ttoi(paysubtypestr);
			::function::GetSingleMidParamFromBetweenWebBuf(Msg,_T("<transferid><![CDATA["),_T("]]></transferid>"),transferid);
			::function::GetSingleMidParamFromBetweenWebBuf(Msg,_T("<feedesc><![CDATA[￥"),_T("]]></feedesc>"),str);
			//::function::GetSingleMidParamFromBetweenWebBuf(Msg,_T("<fee>"),_T("</fee>"),feestr);
			
			if(paysubtype==1)
			{
				if(SendWxid!=g_selfwxid)
				{
					
					if (!g_forbidautorec)
					{
						Sleep(1000);
						
			
						SendpackForRecPayment(transferid, SendWxid, Time);
					
					}

				
				}

			}

			}
	}
//        paysubtype ＝ 到整数 (取文本中间内容 (Msg, “<paysubtype>”, “</paysubtype>”))
//        transferid ＝ 取文本中间内容 (Msg, “<transferid><![CDATA[”, “]]></transferid>”)
//        str ＝ 取文本中间内容 (Msg, “<feedesc><![CDATA[￥”, “]]></feedesc>”)
//
//        .如果真 (paysubtype ＝ 1)  ' 实时转账
//            .如果真 (SendWxid ＝ MyData.Wxid)  ' 自己在手机上发的转账不处理
//                返回 ()
//            .如果真结束
//            .如果真 (选择框_自动收款.选中 ＝ 真)
//                功能_接受转账 (m_Socket, AnsiToUnicode (transferid), AnsiToUnicode (SendWxid), Time)  ' 这里time正常来说应该取现行时间戳
//            .如果真结束
	tempwxretdata.Content.TrimLeft(_T("\n"));

		
TCHAR bufblank[]={0x2005,0};

//@爱如初心 @3405882456 jjlk@3405882456 sdfkl
int pos1=tempwxretdata.Content.Find('@');
int pos2=tempwxretdata.Content.Find(bufblank);
if(pos2==-1)
{
	if(g_newestversion)
	lstrcpy(bufblank,_T("#"));
	pos2=tempwxretdata.Content.Find(bufblank);
}
if(pos1!=-1&&pos2==-1)
{
	lstrcpy(bufblank,_T(" "));
pos2=tempwxretdata.Content.Find(bufblank);
}
if(pos1!=-1)
{
	INT64 curgroupid=GetGroupUidFromUserNameOrNickName(tempwxretdata.FromGroupName,0);

	CStringvect transcationidvect;
	CStringvect vectemp;
	CString atliststr;
	::function::GetSingleMidParamFromBetweenWebBuf(atlist,_T("[CDATA["),_T("]]"),atliststr);
	::function::getmytoken(atliststr,',',vectemp);


	
	if(vectemp.size()==1&&vectemp[0].IsEmpty())
		vectemp.clear();

	if(atliststr.IsEmpty()||vectemp.size()==0)
	{
		::function::GetSingleMidParamFromBetweenWebBuf(atlist,_T("<atuserlist>"),_T("</atuserlist>"),atliststr);
		::function::getmytoken(atliststr,',',vectemp);
		if(vectemp.size()==1&&vectemp[0].IsEmpty())
		vectemp.clear();
	}

	CString tttt;
	
	if(bShowatmsg)
	{
		tttt.Format(_T("@的数量：%d"),vectemp.size());
		MessageBox(0,atlist,tttt,0);
	}
	//::function::GetSingleMidParamFromBetweenWebBuf(Msg,_T("@"),bufblank,transcationid);

	if(pos2!=-1)
	{
		GetVectMidParamFromBetweenWebBuf(tempwxretdata.Content,_T("@"),bufblank,transcationidvect);
		CString oldnamestr,newidstr;

		if(bShowatmsg)
		{
			tttt.Format(_T("pos2!=-1：%d"),pos2);
			MessageBox(0,tttt,0,0);
		}
		if(!transcationidvect.size()&&vectemp.size())
		{

			CString transcationidstr;
			::function::GetHalfStr(tempwxretdata.Content,NULL,&transcationidstr,_T("@"),true);

			oldnamestr.Format(_T("@%s"),transcationidstr);
			INT64 sendqq=GetFriendUidFromUserName(curgroupid,vectemp.back());
			newidstr.Format(_T("%lld "),sendqq);

			tempwxretdata.Content.Replace(oldnamestr,newidstr);
			tempwxretdata.Content.TrimRight();


		}
		else
		{
		for(int i=0;i<transcationidvect.size();i++)
		{
			if(i>=vectemp.size())
				break;

			oldnamestr.Format(_T("@%s%s"),transcationidvect[i],bufblank);
			INT64 sendqq=GetFriendUidFromUserName(curgroupid,vectemp[i]);
			newidstr.Format(_T("%lld "),sendqq);

			tempwxretdata.Content.Replace(oldnamestr,newidstr);


		}
		}

	}
	else
	{
		if(bShowatmsg)
		{
			tttt.Format(_T("pos2==-1：%d"),pos2);
			MessageBox(0,tttt,0,0);
		}
	if(tempwxretdata.Content.Find(_T("踢出"))==0||tempwxretdata.Content.Find(_T("踢黑"))==0)
		{
			CString newidstr;
			if(vectemp.size())
			{
				INT64 sendqq=GetFriendUidFromUserName(curgroupid,vectemp[0]);
				if(tempwxretdata.Content.Find(_T("踢出"))==0)
					newidstr.Format(_T("踢出%lld"),sendqq);
				else if(tempwxretdata.Content.Find(_T("踢黑"))==0)
					newidstr.Format(_T("踢黑%lld"),sendqq);
				tempwxretdata.Content=newidstr;
			}

		}
		else
		{
			if(bShowatmsg)
			{
				tttt.Format(_T("if(%d)：%d"),vectemp.size(),pos2);
				MessageBox(0,tttt,0,0);
			}
			if(vectemp.size())
			{
				INT64 sendqq=GetFriendUidFromUserName(curgroupid,vectemp[0]);
				//查看邀请数量@天乐
				CString newidstr=tempwxretdata.Content.Left(pos1);//20190206 应该是截取左边，而不是中间的文字。 .Mid(pos1);
				tempwxretdata.Content.Format(_T("%s%lld"),newidstr,sendqq);
			}
		}



	}
}
//if(tempwxretdata.Content.Find(_T("与群里其他人都不是微信朋友关系"))==-1)
{
	IsNewJoinMsg(tempwxretdata);
}
			//return;

	mywxretdatasetvect.push_back(tempwxretdata);


::SendMessage(mainhwnd,WM_NEWMSG,0,(LPARAM)&mywxretdatasetvect);


}
void Transfer2windowwxmsgsForLatest(_Unpack& bf)
{
	int Type=0;
	int MsgID=0;
	CString SendWxid;
	CString RecvWxid;
	CString Msg;
	CString atlist;
	INT64 Time;
	

	CString strrr = (CString)bf.GetAll_Hex();
Type=bf.GetInt();
MsgID=bf.GetInt();
SendWxid=bf.GetTokenstr();
if(IsPublicWxNum(SendWxid,false))
				return;

RecvWxid=bf.GetTokenstr();
Msg=bf.GetTokenstr();
atlist=bf.GetTokenstr();
Time=bf.GetInt();
Transfer2windowwxmsgsFor2526(Type,MsgID,SendWxid,RecvWxid,Msg,atlist,Time);

}
void Transfer2windowwxmsgsForLatest(cJSON* RecvJson)
{
	int Type=0;
	int MsgID=0;
	CString SendWxid;
	CString RecvWxid;
	CString Msg;
	CString atlist;
	INT64 Time;
	
	MsgInfo mInfo = {0};

	cJSON* item= cJSON_GetObjectItem(RecvJson,"mywxid");
				if(item)
					mInfo.RecvWxid= item->valuestring;
				
				item= cJSON_GetObjectItem(RecvJson,"type");
				if(item)
					mInfo.Type= item->valueint;

				//mInfo.Type			= cJSON_GetObjectItem(RecvJson,"type")->valueint;
				int dwTime=0;		
					item= cJSON_GetObjectItem(RecvJson,"time");
				if(item)
					dwTime= item->valueint;
					//= cJSON_GetObjectItem(RecvJson,"time")->valueint;
				strcpy(mInfo.Time,Timestamp2DataTime(dwTime));

				item= cJSON_GetObjectItem(RecvJson,"isme");
				if(item)
					mInfo.IsSend= item->valueint;

				//mInfo.IsSend		= cJSON_GetObjectItem(RecvJson,"isme")->valueint;
					item= cJSON_GetObjectItem(RecvJson,"msgid");
				if(item)
					mInfo.LocalMsgID= item->valueint;

				//mInfo.LocalMsgID	= cJSON_GetObjectItem(RecvJson,"msgid")->valueint;

				item= cJSON_GetObjectItem(RecvJson,"smsgid");
				if(item)
					mInfo.ServerMsgID= item->valuestring;

				//mInfo.ServerMsgID	= cJSON_GetObjectItem(RecvJson,"smsgid")->valuestring;
				item= cJSON_GetObjectItem(RecvJson,"wxid1");
				if(item)
					mInfo.Wxid_1= item->valuestring;
				//mInfo.Wxid_1		= cJSON_GetObjectItem(RecvJson,"wxid1")->valuestring;
				item= cJSON_GetObjectItem(RecvJson,"wxid2");
				if(item)
					mInfo.Wxid_2= item->valuestring;
				//mInfo.Wxid_2		= cJSON_GetObjectItem(RecvJson,"wxid2")->valuestring;

					item= cJSON_GetObjectItem(RecvJson,"msg");
				if(item)
					mInfo.Msg= item->valuestring;
				//mInfo.Msg			= cJSON_GetObjectItem(RecvJson,"msg")->valuestring;

					item= cJSON_GetObjectItem(RecvJson,"source");
				if(item)
					mInfo.Source= item->valuestring;
				
				//mInfo.Source		= cJSON_GetObjectItem(RecvJson,"source")->valuestring;
				
				int B64_Length = strlen(mInfo.Msg);
				int Byte_Length = ((B64_Length / 4) * 3) + 10;
				int Ret=0;
				if(B64_Length)
				{
				BYTE* mByte = new BYTE[Byte_Length];
				memset(mByte,0,Byte_Length);
				Ret = BASE64_Decode(mInfo.Msg, B64_Length, mByte);
				strcpy(mInfo.Msg,(char*)mByte);
				delete mByte;
				}

				B64_Length = strlen(mInfo.Source);
				if(B64_Length)
				{
				Byte_Length = ((B64_Length / 4) * 3) + 10;
				BYTE* sByte = new BYTE[Byte_Length];
				memset(sByte,0,Byte_Length);
				Ret = BASE64_Decode(mInfo.Source, B64_Length, sByte);
				strcpy(mInfo.Source,(char*)sByte);
				delete sByte;
				}

				
				Type=mInfo.Type;
	MsgID=mInfo.LocalMsgID;
	if(mInfo.IsSend)
	{
		if(!MsgID)
		return;
		else
		{
	SendWxid=g_selfwxid;
	RecvWxid=mInfo.Wxid_1;
		}
	}
	else
	{
SendWxid=mInfo.Wxid_1;
RecvWxid=mInfo.Wxid_2;
	}
	if(mInfo.Msg)
	{
		Msg=mInfo.Msg;
		if(Msg.Find(_T("\\u"))!=-1)
		{
	TCHAR newbuf[2048*10];
TranslateUbuffer(Msg,newbuf);
Msg=newbuf;
		}
	}
	if(mInfo.Source)
	atlist=mInfo.Source;
	Time=dwTime;




Transfer2windowwxmsgsFor2526(Type,MsgID,SendWxid,RecvWxid,Msg,atlist,Time);

}

void Transfer2windowwxmsgs(LPCTSTR sendwxid,LPCTSTR sendmsg,int sendtype)
{
	if(StrStr(sendmsg,_T("<voicemsg")))
		return;
	setvector<Wxretdata> mywxretdatasetvect;
	Wxretdata tempwxretdata;
	tempwxretdata.MsgType=sendtype;
	tempwxretdata.Content=sendmsg;
	tempwxretdata.FromUserName=sendwxid;
	//tempwxretdata.FromGroupName=sendwxid;
	CString msgfromgroupname=sendwxid;
	CString tempcontent=sendmsg;
	if(msgfromgroupname.Find(_T("@chatroom"))!=-1)
				{
					tempwxretdata.FromGroupName=msgfromgroupname;//"@195bff9b154a53ac4ed5495d2eb3f56c58e60983f40335cd2acf0ed55a36c044:
					int pos=tempcontent.Find(':');
					if(pos!=-1)
					{
						tempwxretdata.FromUserName=tempcontent.Left(pos);
					}
					tempwxretdata.Content=tempcontent.Mid(pos+1);


				}
				else
				{
					tempwxretdata.FromUserName=(CString)msgfromgroupname;
					
					tempwxretdata.Content=tempcontent;
				}

	tempwxretdata.Content.TrimLeft(_T("\n"));

		

		if(IsNewJoinMsg(tempwxretdata))
			return;

	mywxretdatasetvect.push_back(tempwxretdata);
::SendMessage(mainhwnd,WM_NEWMSG,0,(LPARAM)&mywxretdatasetvect);
}
INT64 GetGroupIDFromGroupWxid(LPCTSTR wxid)
{
	INT64 tempv=_ttoi64(wxid);
	if(tempv<10000)
		tempv=StrToHash(wxid);

	return tempv;

}
BOOL RtCStringSendWebForWeixin(LPCTSTR weixinurl,byte *imagebytebuf,UINT& ddlen);
BOOL RtCStringSendWebForWeixin2(LPCTSTR weixinurl,CString* WebrtStr,byte *imagebytebuf,UINT& ddlen);
CString g_selfwxname,g_selfwxuser,g_selfwxphone;
void fun_recv_analyse(vectorbyte& data)
	{
	

vectorbyte test;
_Unpack unPack;

if(data.size()==0)
	return;
   


unPack.SetData (data);

BOOL bSaveFile=false;
if(bSaveFile)
{
	CFile file1;

	if(file1.Open(::function::complete_path(_T("fff收到的记录.txt")),CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite))
	{
		CStringA totalstr;
		CStringA tempstr;
		for(int i=0;i<data.size();i++)
		{
			tempstr.Format("%02X ",data[i]);
			totalstr+=tempstr;
		}

		file1.Write(totalstr.GetBuffer(0),totalstr.GetLength());
		file1.Close();
	}
}




CString curwxid,sendwxid,sendmsg;
int sendtype;
int prefix=unPack.GetByte();

if(g_latestwechatflag)
{
	CString tempstr;
	CString wutfstr;
	tempstr=unPack.GetTokenstr(&wutfstr);
	if(!tempstr.IsEmpty())
	{
		
		g_selfwxuser=tempstr;

	}
		tempstr=unPack.GetTokenstr();
if(!tempstr.IsEmpty()&&tempstr.Find(_T("http"))==-1)
		g_selfwxid=tempstr;
}
if(g_latestwechatflag)
{
	CString savefilestr;

	
		if(prefix==10)
		{
		/*	CStringA tips;
			tips.Format("%s",unPack.GetAll_Hex());
			MessageBoxA(0,"登录成功",tips,0);*/

		//登录成功
			Transfer2InitOver(unPack);
		}
		else if(prefix==11)
		{
			
		//收到二维码登录图片
vectorbyte vpng=unPack.GetToken();
		}
		else if(prefix==12)
		{
		//添加好友回调过程
		
		}
		else if(prefix==13)
		{
					
		/*CStringA tips;
			tips.Format("%s",unPack.GetAll_Hex());
			MessageBoxA(0,"好友和群列表",tips,0);*/
		//好友和群列表
			Transfer2FrendList(unPack);
		}
		else if(prefix==14)
		{
		
		//特定群的成员列表
			Transfer2Groupmember(unPack);
		}
		else if(prefix==15)
		{
		//创建群聊
			Transfer2windowwxmsgsForCreateGroup(unPack);
		
		}
		else if(prefix==17)
		{
		
		//收到各类消息
			if(g_readGroupOver&&g_readFriendOver)//在读群列表的时候，暂时不处理消息，避免冲突
		{
			Transfer2windowwxmsgsForLatest(unPack);
		}
			
		}
		else if(prefix==18)
		{
		//获取a8key

	
//
			CStringA urla=vector2char(unPack.GetToken());
		//	MessageBoxA(0,urla,0,0);
			CString url=(CString)urla;
//logdebug (“[GetA8Key] ” ＋ url)
			if(StrStr(url,_T("addchatroombyinvite")))
			{
				UINT ddlen=1*1024*1024;

				byte *localbytebuf;

				localbytebuf=new byte[ddlen];
				CString rtstrrr;
				//::function::DownloadHttpFile(url,localbytebuf,ddlen,false);
				//RtCStringSendWebForWeixin(url,localbytebuf,ddlen);
RtCStringSendWebForWeixin2(url,&rtstrrr,localbytebuf,ddlen);

//MessageBox(0,rtstrrr,0,0);
			//if(1)
				//MessageBoxA(0,(char*)localbytebuf,0,0);
				ddlen++;
				delete[]localbytebuf;
				// Http.Open (“POST”, url)
				// Http.SetRedirect (假)
				// Http.SetRequestHeader (“Accept”, “text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8”)
				// Http.SetRequestHeader (“User-Agent”, “Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36 MicroMessenger/6.5.2.501 NetType/WIFI WindowsWechat QBCore/3.43.556.400 QQBrowser/9.0.2524.400”)
				// Http.SetRequestHeader (“Referer”, url)
				// Http.SetRequestHeader (“Accept-Language”, “zh-CN,zh;q=0.8,en-us;q=0.6,en;q=0.5;q=0.4”)
				// Http.Send ()
				// str ＝ Http.GetAllResponseHeaders ()
				// str ＝ 取文本中间内容 (str, “weixin://jump/mainframe/”, “@chatroom”)
				//    .如果真 (str ≠ “”)
				//        logdebug (“[加群成功] ” ＋ str ＋ “@chatroom”)
			}
		}
		else if(prefix==20)
		{
		//微信退出了

	
		}




if(!g_selfwxid.IsEmpty())
if(prefix!=10&&!g_readFriendOver)
	{

		

			static int once2=0;
			if(!once2)//<10&&backbufvect.size()==0
			{
				once2=1;
				SendpackForGetFriendList();
			}
	}

//新版本解析

}
else
{
	if(prefix==0)//初始化完毕
	{
		//g_selfwxid=unPack.GetTokenstr();
	}
	else if(prefix==1)//收到自己的消息
	{
		Transfer2InitOver(unPack);
	}
	else if(prefix==2)//收到好友的消息
	{

	}
	else if(prefix==3)//收到消息
	{
		CString tempstr;
		tempstr=unPack.GetTokenstr();
		sendtype=unPack.GetInt();
		sendwxid=unPack.GetTokenstr();
		sendmsg=unPack.GetTokenstr();


		if(0)
		{
			CFile file1;

			if(file1.Open(::function::complete_path(_T("收到的消息.txt")),CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite))
			{


				if(0)
					MessageBox(0,sendmsg,0,0);
				file1.SeekToEnd();
				file1.Write(sendmsg.GetBuffer(0),sendmsg.GetLength()*2);
				file1.Close();
			}
		}

		if(g_readGroupOver&&g_readFriendOver)//在读群列表的时候，暂时不处理消息，避免冲突
		{
			if(IsPublicWxNum(sendwxid,false))
				return;
			Transfer2windowwxmsgs(sendwxid,sendmsg,sendtype);
		}



	}
	//CKeepMap<CString,LPCTSTR,struct_wxsingleuserinf,struct_wxsingleuserinf> g_wxfriend_singleinf;
	//CKeepMap<CString,LPCTSTR,set<CString>,set<CString>> g_wxgroup_setwxid;
	//CKeepMap<CString,LPCTSTR,struct_wxsingleuserinf,struct_wxsingleuserinf> g_wxgroupid_wxid_singleinf;
	else if(prefix==4)//收到群成员列表的消息
	{
		Transfer2Groupmember(unPack);

	}
	else if(prefix==5)//收到好友列表信息
	{

		Transfer2FrendList(unPack);

	}

	if(!g_selfwxid.IsEmpty())
	if(prefix!=1&&!g_readFriendOver)
	{
		static int once2=0;
		if(!once2)
		{
			once2=1;
			SendpackForGetFriendList();
		}
	}
}




	}

void SendpackForGetFriendList()
{
		if(g_newestversion)
			{
			Wx_ReFriendList(Socket_Client);
			
			}
			else
			{
	_Pack pack1;
	if(g_latestwechatflag)
pack1.SetByte(13);//#send_好友列表=13
	else
	pack1.SetByte(10);
ServerSendPack(pack1.GetAll());

		}
}
void SendpackForRoomDeleteUser(LPCTSTR groupid,LPCTSTR wxid)
{
	if(g_newestversion)
	{
		CStringA uGroupID;
		uGroupID=groupid;
		CStringA uWxid;
		uWxid=wxid;
	WxGroup_Kick(Socket_Client, uGroupID.GetBuffer(), uWxid.GetBuffer());

	}
	else
	{
	_Pack pack1;

	if(g_latestwechatflag)
pack1.SetByte(17);//#send_群聊踢人=17
	else
	pack1.SetByte(6);


	pack1.SetToken(groupid);
	pack1.SetToken(wxid);

	ServerSendPack(pack1.GetAll());
	}
}
void SendpackForDeleteFriendOrGroup(LPCTSTR wxid,int type)
{

	if(g_newestversion)
	{
		CStringA uGroupID;
		uGroupID=wxid;
		CString wxidroom=wxid;
			if(::function::TrimRight(wxidroom,_T("@chatroom"),true))
				WxGroup_Exit(Socket_Client, uGroupID.GetBuffer());
			else
	Wx_DelFriend(Socket_Client, uGroupID.GetBuffer());
		
	//WxGroup_Kick(Socket_Client, uGroupID.GetBuffer(), uWxid.GetBuffer());

	}
	else
	{
	_Pack pack1;

	if(g_latestwechatflag)
pack1.SetByte(11);//#send_删除好友或群=11
	else
	pack1.SetByte(11);


	
	pack1.SetToken(wxid);
pack1.SetInt(type);
	ServerSendPack(pack1.GetAll());
	}
}

void SendpackForRoomAddUser(LPCTSTR groupid,LPCTSTR wxid,int type)
{
	_Pack pack1;
	if(g_latestwechatflag)
	pack1.SetByte(16);
	else
	pack1.SetByte(7);//#send_群聊拉人=16
	pack1.SetToken(groupid);
	pack1.SetToken(wxid);
	
	pack1.SetInt(type);//=0是直接拉，适合40人以下的群，=1是发邀请链接

	ServerSendPack(pack1.GetAll());

}
void SendpackForAcceptFriend(LPCTSTR v1,LPCTSTR v2)
{
	_Pack bf;
	

	bf.SetByte(3);
	bf.SetToken(v1);
	CStringA vvv2;
	::function::wchrto_utf8(v2,vvv2);

	bf.SetToken(char2vector(vvv2,vvv2.GetLength()));

	ServerSendPack(bf.GetAll());

}
void SendpackForAddFriendOrAccept(LPCTSTR wxid,LPCTSTR msg,int type,int scene)//微信id或v1  单向传空 消息添加传我是xxxx  同意传v2  1=单向添加 2=消息添加 3=同意好友申请 scene等于11，scene=14等于群聊
{

	if(g_newestversion)
	{
		CStringA wxida;
		wxida=wxid;
		CStringA msga;
		msga=msg;
	Wx_AddFriend(Socket_Client, type, scene, wxida.GetBuffer(0), msga.GetBuffer(0), "");
	}
	else
	{
	_Pack bf;
	

		bf.SetByte(12);//#send_添加好友=12
		bf.SetToken(wxid);

		CStringA vvv2;
	::function::wchrto_utf8(msg,vvv2);

	bf.SetToken(char2vector(vvv2,vvv2.GetLength()));

		//bf.SetToken(msg);
		bf.SetInt(type);
		bf.SetInt(scene);

	ServerSendPack(bf.GetAll());
	}
}


void SendpackForGetGroupUserAll(LPCTSTR groupid)
{
	if(g_newestversion)
	{
		CStringA groupida;
		groupida=groupid;
	Wx_ReGroupList(Socket_Client,groupida.GetBuffer(0));
	}
	else
	{
	_Pack pack1;
	//#send_群成员列表=14
	if(g_latestwechatflag)
	pack1.SetByte(14);
	else
	pack1.SetByte(9);
	pack1.SetToken(groupid);
	ServerSendPack(pack1.GetAll());
	}
}
BOOL QueryWxidFromHashValue(INT64 groupid,INT64 wxnum,CString &rtwxid,CString* rtwxnickname)
{
	BOOL bOK=false;
	CKeepWxInfMap* pintstrmap;
lockgqqnickmapresource.Lock();
if(!g_group_wxinfmap.Lookup(groupid,pintstrmap))
		{
pintstrmap=new CKeepWxInfMap;
g_group_wxinfmap[groupid]=pintstrmap;
		}
struct_wxsingleuserinf tempinf;
if((*pintstrmap).Lookup(wxnum,tempinf))
{
	bOK=true;
	rtwxid=tempinf.wxid;
	if(rtwxnickname)
	{
		CString rtnick;
		if(g_usingremarkname)
		rtnick=tempinf.remark;
		if(rtnick.IsEmpty())
			rtnick=tempinf.nickname;
		if(rtnick.IsEmpty())
			rtnick=tempinf.user;

		*rtwxnickname=rtnick;
	}
}
//如果没找到，那么再从所有的map中挑一个


lockgqqnickmapresource.Unlock();
if(!bOK)
{

	bOK=g_hash2wxidmap.Lookup(wxnum,rtwxid);
}
return bOK;
}
void UpdateInfFromWxid(INT64 groupid,const struct_wxsingleuserinf& tempinf)
{
	BOOL bOK=false;
	CKeepWxInfMap* pintstrmap;
lockgqqnickmapresource.Lock();
if(!g_group_wxinfmap.Lookup(groupid,pintstrmap))
		{
pintstrmap=new CKeepWxInfMap;
g_group_wxinfmap[groupid]=pintstrmap;
		}
INT64 fakewxnum=0;

if(::function::TrimRight((*(const_cast<CString*>(&(tempinf.wxid)))),_T("@chatroom"),true))
fakewxnum=GetGroupIDFromGroupWxid(tempinf.wxid);
else
{

	if(!g_hashvaluemap.Lookup(tempinf.wxid,fakewxnum))
	{

		fakewxnum=StrToHash(tempinf.wxid);
		g_hash2wxidmap[fakewxnum]=tempinf.wxid;
		g_hashvaluemap[tempinf.wxid]=fakewxnum;

	}

	CKeepINT64StrMap *pintstrmap2=NULL;

	//	nickname.Format(_T("%I64d"),rprealqq);



	lockgqqnickmapresource.Lock();
	if(!GrouprealqqNickmap.Lookup(groupid,pintstrmap2))
	{
		pintstrmap2=new CKeepINT64StrMap;
		GrouprealqqNickmap[groupid]=pintstrmap2;

	}
	lockgqqnickmapresource.Unlock();
	CString rtnick;
	if(g_usingremarkname)
	rtnick=tempinf.remark;
	if(rtnick.IsEmpty())
		rtnick=tempinf.nickname;
	(*pintstrmap2)[fakewxnum]=rtnick;




}
	

	(*pintstrmap)[fakewxnum]=tempinf;
		lockgqqnickmapresource.Unlock();


}

INT64 QueryHashValueFromWxid(INT64 groupid,LPCTSTR wxid)
{

	BOOL bOK=false;
	CKeepWxInfMap* pintstrmap;
lockgqqnickmapresource.Lock();
if(!g_group_wxinfmap.Lookup(groupid,pintstrmap))
		{
pintstrmap=new CKeepWxInfMap;
g_group_wxinfmap[groupid]=pintstrmap;
		}
INT64 fakewxnum=0;
	if(!g_hashvaluemap.Lookup(wxid,fakewxnum))
		{
		
			fakewxnum=StrToHash(wxid);
	

			g_hashvaluemap[wxid]=fakewxnum;
			g_hash2wxidmap[fakewxnum]=wxid;
		}

if(!(*pintstrmap).Lookup(fakewxnum))
{
	struct_wxsingleuserinf tempinf;

	tempinf.wxid=wxid;
	(*pintstrmap)[fakewxnum]=tempinf;
}
lockgqqnickmapresource.Unlock();

return fakewxnum;
}

INT64 QueryHashValueFromWxid(LPCTSTR groupid,LPCTSTR wxid)
{
	return QueryHashValueFromWxid(GetGroupIDFromGroupWxid(groupid),wxid);
}
void UpdateInfFromWxid(LPCTSTR groupid,const struct_wxsingleuserinf& tempinf)
{
	UpdateInfFromWxid(GetGroupIDFromGroupWxid(groupid),tempinf);
}
BOOL QueryWxidFromHashValue(LPCTSTR groupid,INT64 wxnum,CString &rtwxid)
{
	return QueryWxidFromHashValue(GetGroupIDFromGroupWxid(groupid),wxnum,rtwxid,NULL);
}

// DWORD detaddress=0;
// 
// DWORD ListProcessModules( DWORD dwPID,LPCTSTR moudlename)
//{
//	 DWORD rtbaseaddress=0;
//  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
//  MODULEENTRY32 me32;
//
//  // Take a snapshot of all modules in the specified process.
//  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
//  if( hModuleSnap == INVALID_HANDLE_VALUE )
//  {
//	  Sleep(20);
//   // printError( TEXT("CreateToolhelp32Snapshot (of modules)") );
//   // return( 0 );
//	  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );
//	  if(hModuleSnap==INVALID_HANDLE_VALUE)
//	  {
//		  return(0);
//	  }
//  }
//
//  // Set the size of the structure before using it.
//  me32.dwSize = sizeof( MODULEENTRY32 );
//
//  // Retrieve information about the first module,
//  // and exit if unsuccessful
//  if( !Module32First( hModuleSnap, &me32 ) )
//  {
//  //  printError( TEXT("Module32First") );  // show cause of failure
//    CloseHandle( hModuleSnap );           // clean the snapshot object
//    return( 0 );
//  }
//
//  // Now walk the module list of the process,
//  // and display information about each module
//  do
//  {
//   /* _tprintf( TEXT("\n\n     MODULE NAME:     %s"),   me32.szModule );
//    _tprintf( TEXT("\n     Executable     = %s"),     me32.szExePath );
//    _tprintf( TEXT("\n     Process ID     = 0x%08X"),         me32.th32ProcessID );
//    _tprintf( TEXT("\n     Ref count (g)  = 0x%04X"),     me32.GlblcntUsage );
//    _tprintf( TEXT("\n     Ref count (p)  = 0x%04X"),     me32.ProccntUsage );
//    _tprintf( TEXT("\n     Base address   = 0x%08X"), (DWORD) me32.modBaseAddr );
//    _tprintf( TEXT("\n     Base size      = %d"),             me32.modBaseSize );*/
//	if(StrCmpIW(me32.szModule,moudlename)==0)
//	{
//		rtbaseaddress=(DWORD)me32.modBaseAddr;
//		break;
//	
//	}
//  } while( Module32Next( hModuleSnap, &me32 ) );
//
//  CloseHandle( hModuleSnap );
//  return rtbaseaddress;
//}
// DWORD WINAPI  TimeChangeTimeProFun(LPVOID lp)
// {
//	 DWORD lpBaseOfDll=0;
//
//								DWORD dwProcessId=g_wxpi.dwProcessId; //假设你已经获得了它
//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS ,
//                              FALSE, dwProcessId);
//
//	 while(1)
//	 {
//		 if(!lpBaseOfDll)
//		 {
//			 lpBaseOfDll=ListProcessModules(g_wxpi.dwProcessId,_T("wcinterface.dll"));
//			 // HMODULE interfacehDll = GetModuleHandle(("wcinterface.dll")); 
//			 //	BOOL bok=GetModuleInformation(g_wxpi.hProcess,interfacehDll,&mi,sizeof(MODULEINFO));
//			 //lockinterfacedll.Lock();
//			 //	detaddress=(int*)((byte*)mi.lpBaseOfDll+0x14877);
//			 if(lpBaseOfDll)
//			 detaddress=lpBaseOfDll+0x14877;
//
//		/*	 if(!lpBaseOfDll)
//			 {
//				 MessageBox(0,_T("lpBaseOfDll等于0"),0,0);
//				 detaddress=0x10014877;
//			 }*/
//		 }
//		// lockinterfacedll.Lock();
//		if(detaddress)
//		{
////			 *detaddress=1513008000;
//
//			 int destvalue = 1513008000;//{0x4c,0xff,0x64,0x00,0x2f};
//BOOL fSuccess = WriteProcessMemory(hProcess,     //写入内存的进程句柄
//                                   (LPVOID)detaddress, //目标进程的地址
//                                   (PVOID)&destvalue,//你要写入数据的地址
//                                   4,            //大小，按字节计
//                                   NULL); 	
//if(!fSuccess)
//{
//	static int countt=0;
//	if(!countt)
//	{
//		countt=1;
//		MessageBox(0,_T("写入修改时间失败！"),0,0);
//	}
//}
//
//		 }
//		//lockinterfacedll.Unlock();
//
//		 Sleep(100);
//	 }
//	 return 0;
// }
//


static char data_str[10000];

void __stdcall Server_CallBack(int sPoint, int so, int type, int buf, int buflen, int cso);
void __stdcall Client_CallBack(int so, int sPoint, int type, int buf, int buflen);

//Page_Friend		m_PageFriend;
//Page_Friend_GH	m_PageFriend_GH;
//Page_Group		m_PageGroup;
//Page_Data		m_PageData;

CMutex Log_Lock;
void Log(char* strLOG)
{
	////Log_Lock.Lock();
	//int strLen = strlen(strLOG) + 100;
	//char* outLog = new char[strLen];
	//memset(outLog,0,strLen);
	//sprintf(outLog,"%s\r\n-------------------\r\n",strLOG);

 //	CEdit* Data_ = (CEdit*)m_PageData.GetDlgItem(DATA_TXT);
	//int dataLen = Data_->GetWindowTextLength();
	//if (dataLen > 29000)
	//{
	//	Data_->SetWindowText(NULL);
	//	dataLen = 0;
	//}
	//Data_->SetSel(dataLen,dataLen,TRUE);
	//Data_->ReplaceSel(outLog);
	//delete outLog;
	//Log_Lock.Unlock();
}

CMutex ThreadLock;
BOOL alreadyconnect=false;
void __stdcall Server_CallBack(int sPoint, int so, int type, int buf, int buflen, int cso)
{
	static CStringA tempwxid;

		ThreadLock.Lock();

	if (type == 通信_连接成功) //建立通信
	{
		if(g_versionnewest27)
			Wx_Init(so,"E-575864785");
		else
		Wx_Init(so,"CPlus-1529685359");
//

		alreadyconnect=true;
	}
	else if (type == 通信_连接断开) //退出通信
	{
		char sClient[10] = {0};
		sprintf(sClient,"%d",so);
		alreadyconnect=false;

		
	}
	else if (type == 通信_数据到达)//接收消息
	{

		char* s_Recv = (char*)malloc(buflen+1);
		memset(s_Recv,0,buflen+1);
		memcpy((PVOID)s_Recv,(PVOID)buf, buflen);
		CString str222;
		str222.Format(_T("cJSON_Parse before %d"),buflen);
		 Write2MagnetLog(str222,&magnetfile,16);	
		cJSON* RecvJson = cJSON_Parse(s_Recv);
			str222.Format(_T("cJSON_Parse after %d"),buflen);
		 Write2MagnetLog(str222,&magnetfile,16);	
		//在这里做好logresut日志记录，看看运行到什么一步卡死了

		if ( RecvJson)
		{
			int RecvType = cJSON_GetObjectItem(RecvJson,"packtype")->valueint;
			if (RecvType == 回调_二维码)
			{
				char* QRCode = cJSON_GetObjectItem(RecvJson,"qrcode")->valuestring;
				//ShowImageFromB64(QRCode,(CStatic*)Dlg->GetDlgItem(PIC_HIMG));
			}
			else if (RecvType == 回调_登录成功)//登录成功
			{
				CStringA nickname;
			

				char sClient[10] = {0};
				sprintf(sClient,"%d",so);
				
				/*CListCtrl* LoginList = (CListCtrl*)Dlg->GetDlgItem(LOGIN_LIST);
				int Item = LoginList->InsertItem(LoginList->GetItemCount(),cJSON_GetObjectItem(RecvJson,"nick")->valuestring);
				LoginList->SetItemText(Item,1,cJSON_GetObjectItem(RecvJson,"wxid")->valuestring);
				LoginList->SetItemText(Item,2,cJSON_GetObjectItem(RecvJson,"wxno")->valuestring);
				LoginList->SetItemText(Item,3,cJSON_GetObjectItem(RecvJson,"himg")->valuestring);
				LoginList->SetItemText(Item,4,sClient);*/
				cJSON* item= cJSON_GetObjectItem(RecvJson,"wxid");
				if(item)
					tempwxid=item->valuestring;
				//tempwxid=cJSON_GetObjectItem(RecvJson,"wxid")->valuestring;
				item= cJSON_GetObjectItem(RecvJson,"nick");
				if(item)
					nickname=item->valuestring;
				//nickname=cJSON_GetObjectItem(RecvJson,"nick")->valuestring;
				
				
				CString tempwwxid;
				tempwwxid=tempwxid;
			

			if(tempwwxid!=g_selfwxid&&!g_selfwxid.IsEmpty())
				{
				MessageBox(mainhwnd,_T("你有多个微信客户端未关闭，请退出多余的未和机器人进程关联的微信客户端再重新启动机器人软件。"),_T("请关闭多余的微信客户端再重启软件"),0);
				//return;
				}
				g_selfwxid=tempwwxid;
				g_interfaceuin=StrToHash(g_selfwxid);
		m_ownqqnumber.Format(_T("%lld"),g_interfaceuin);
		g_lastownqq.Format(_T("%lld"),g_interfaceuin);
		g_wxuin.Format(_T("%lld"),g_interfaceuin);
		g_selfusername=g_selfwxid;
		Client_Wxid=g_selfwxid;
		g_selfwxname=nickname;
		g_selfnickname=g_selfwxname;
		TCHAR newbuf[2048*10];
TranslateUbuffer(g_selfnickname,newbuf);
g_selfnickname=newbuf;
		Socket_Client=so;
		

		SendMessage(mainhwnd,WM_SIGNEDIN,0,0);

			}
			else if(RecvType == 回调_退出登陆)
			{
				char sClient[10] = {0};
				sprintf(sClient,"%d",so);
				alreadyconnect=false;

if (so == Socket_Client)
						{
							Socket_Client = 0;
}
			/*	CListCtrl* LoginList = (CListCtrl*)Dlg->GetDlgItem(LOGIN_LIST);
				for (int i=0;i<LoginList->GetItemCount();i++)
				{
					char* tmp = LoginList->GetItemText(i,4).GetBuffer();
					if (strcmp(tmp,sClient) == 0)
					{
						if (so == Socket_Client)
						{
							Socket_Client = 0;
							Dlg->SetDlgItemText(LOGIN_TXT_PORT,"[未选择]");
							Dlg->SetDlgItemText(LOGIN_TXT_NICK,"[未选择]");
							Dlg->SetDlgItemText(LOGIN_TXT_WXID,"[未选择]");
							ShowImageFromURL("", (CStatic*)Dlg->GetDlgItem(PIC_HIMG));
						}
						LoginList->DeleteItem(i);
						break;
					}
				}*/
			}
			else if (RecvType == 回调_好友列表)
			{
				UserInfo uInfo = {0};
				char* RecvWxid = NULL;
			cJSON*	item= cJSON_GetObjectItem(RecvJson,"mywxid");
				if(item)
					RecvWxid=item->valuestring;
				//cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;
if(strcmp(tempwxid,RecvWxid)==0)
{
	Transfer2FrendList(RecvJson);
			//	cJSON* UserArray = cJSON_GetObjectItem(RecvJson,"list");
			//	int uCount = cJSON_GetArraySize(UserArray);

				//Log(s_Recv);
				//CListCtrl* ListFriend = (CListCtrl*)m_PageFriend.GetDlgItem(LIST_FRIEND);
				//CListCtrl* ListFriend_GH = (CListCtrl*)m_PageFriend_GH.GetDlgItem(LIST_FRIEND_GH);
				//CListCtrl* ListGroup = (CListCtrl*)m_PageGroup.GetDlgItem(LIST_GROUP);

				
				//for (int i=0;i<uCount;i++)
				//{
				//	cJSON* uUser = cJSON_GetArrayItem(UserArray,i);
				//	uInfo.Type = cJSON_GetObjectItem(uUser,"Type")->valueint;
				//	uInfo.List = cJSON_GetObjectItem(uUser,"List")->valueint;
				//	uInfo.Wxid = cJSON_GetObjectItem(uUser,"Wxid")->valuestring;
				//	uInfo.WxNo = cJSON_GetObjectItem(uUser,"WxNo")->valuestring;
				//	uInfo.Nick = cJSON_GetObjectItem(uUser,"Nick")->valuestring;
				//	uInfo.Mark = cJSON_GetObjectItem(uUser,"Mark")->valuestring;
				//	uInfo.HImg = cJSON_GetObjectItem(uUser,"HImg")->valuestring;
				//	if (uInfo.Type == 8 || uInfo.Type == 24 || uInfo.Type == 29)
				//	{
				//		int Item = ListFriend_GH->InsertItem(ListFriend_GH->GetItemCount(),uInfo.Nick);
				//		ListFriend_GH->SetItemText(Item,1,uInfo.Wxid);
				//		ListFriend_GH->SetItemText(Item,2,uInfo.WxNo);
				//		ListFriend_GH->SetItemText(Item,3,uInfo.HImg);
				//	}
				//	else if (strstr(uInfo.Wxid,"@chatroom") != NULL)
				//	{
				//		if (strcmp(uInfo.Nick,"") == 0)
				//			uInfo.Nick = "群聊";
				//		int Item = ListGroup->InsertItem(ListGroup->GetItemCount(),uInfo.Nick);
				//		ListGroup->SetItemText(Item,1,uInfo.Wxid);
				//		ListGroup->SetItemText(Item,2,uInfo.WxNo);
				//		ListGroup->SetItemText(Item,3,uInfo.Int2str(uInfo.List));
				//		ListGroup->SetItemText(Item,4,uInfo.HImg);
				//	}
				//	else if (uInfo.List != 2)
				//	{
				//		int Item = ListFriend->InsertItem(ListFriend->GetItemCount(),uInfo.Nick);
				//		ListFriend->SetItemText(Item,1,uInfo.Mark);
				//		ListFriend->SetItemText(Item,2,uInfo.Wxid);
				//		ListFriend->SetItemText(Item,3,uInfo.WxNo);
				//		ListFriend->SetItemText(Item,4,uInfo.HImg);
				//	}
				//	//cJSON_Delete(uUser);
				//}
				//memset(data_str,0,520);
				//sprintf(data_str,"[刷新好友列表][%s]\r\n好友数：%d\r\n公众号：%d\r\n群聊数：%d",RecvWxid,ListFriend->GetItemCount(),ListFriend_GH->GetItemCount(),ListGroup->GetItemCount());
				//Log(data_str);
}

}
			else if (RecvType == 回调_群员列表)
			{
				str222.Format(_T("回调群员列表 before"));
		 Write2MagnetLog(str222,&magnetfile,16);	

				UserInfo uInfo = {0};

				char* RecvWxid =NULL;// cJSON_GetObjectItem(RecvJson,"wxid")->valuestring;
				cJSON*	item= cJSON_GetObjectItem(RecvJson,"wxid");
				str222.Format(_T("回调群员列表 after"));
		 Write2MagnetLog(str222,&magnetfile,16);

				if(item)
					RecvWxid=item->valuestring;
				
				if(strcmp(tempwxid,RecvWxid)==0)
{
	Transfer2Groupmember(RecvJson);
		str222.Format(_T("回调群员列表transfer2Groupmember fater"));
		 Write2MagnetLog(str222,&magnetfile,16);
			/*	CListCtrl* ListMember = (CListCtrl*)m_PageGroup.GetDlgItem(LIST_GROUP_MEMBER);
				
				char* RecvGroup = cJSON_GetObjectItem(RecvJson,"gid")->valuestring;
				cJSON* UserArray = cJSON_GetObjectItem(RecvJson,"list");
				if (UserArray)
				{
					int uCount = cJSON_GetArraySize(UserArray);
					for (int i=0;i<uCount;i++)
					{
						cJSON* uUser = cJSON_GetArrayItem(UserArray,i);
						uInfo.Wxid = cJSON_GetObjectItem(uUser,"Wxid")->valuestring;
						uInfo.Nick = cJSON_GetObjectItem(uUser,"Nick")->valuestring;
						uInfo.Mark = cJSON_GetObjectItem(uUser,"Mark")->valuestring;
						uInfo.HImg = cJSON_GetObjectItem(uUser,"HImg")->valuestring;

						int Item = ListMember->InsertItem(ListMember->GetItemCount(),uInfo.Nick);
						ListMember->SetItemText(Item,1,uInfo.Mark);
						ListMember->SetItemText(Item,2,uInfo.Wxid);
						ListMember->SetItemText(Item,3,uInfo.HImg);
					}
				}

				memset(data_str,0,520);
				sprintf(data_str,"[刷新群员列表][%s]\r\n群聊ID：%s\r\n群人数：%d",RecvWxid,RecvGroup,ListMember->GetItemCount());
				Log(data_str);*/
				}
				}
			else if (RecvType == 回调_接收消息)
			{
					str222.Format(_T("回调群员列表Transfer2windowwxmsgsForLatest before"));
		 Write2MagnetLog(str222,&magnetfile,16);

				Transfer2windowwxmsgsForLatest(RecvJson);

					str222.Format(_T("回调群员列表Transfer2windowwxmsgsForLatest fater"));
		 Write2MagnetLog(str222,&magnetfile,16);

						}
			else if(RecvType == 回调_语音消息 || 
					RecvType == 回调_视频消息 || 
					RecvType == 回调_文件消息 || 
					RecvType == 回调_图片消息)
			{
				MsgInfo mInfo = {0};
				cJSON*	item= cJSON_GetObjectItem(RecvJson,"mywxid");
				if(item)
					mInfo.RecvWxid=item->valuestring;

			//	mInfo.RecvWxid		= cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;
					if(strcmp(tempwxid,mInfo.RecvWxid)==0)
{
	item= cJSON_GetObjectItem(RecvJson,"type");
				if(item)
					mInfo.Type=item->valueint;

				//mInfo.Type			= cJSON_GetObjectItem(RecvJson,"type")->valueint;
				int dwTime=0;
					item= cJSON_GetObjectItem(RecvJson,"time");
				if(item)
					dwTime=item->valueint;

							//= cJSON_GetObjectItem(RecvJson,"time")->valueint;
				strcpy(mInfo.Time,Timestamp2DataTime(dwTime));

				item= cJSON_GetObjectItem(RecvJson,"isme");
				if(item)
					mInfo.IsSend=item->valueint;
				//mInfo.IsSend		= cJSON_GetObjectItem(RecvJson,"isme")->valueint;

				item= cJSON_GetObjectItem(RecvJson,"msgid");
				if(item)
					mInfo.LocalMsgID=item->valueint;
				//mInfo.LocalMsgID	= cJSON_GetObjectItem(RecvJson,"msgid")->valueint;

					item= cJSON_GetObjectItem(RecvJson,"smsgid");
				if(item)
					mInfo.ServerMsgID=item->valuestring;

				//mInfo.ServerMsgID	= cJSON_GetObjectItem(RecvJson,"smsgid")->valuestring;
					item= cJSON_GetObjectItem(RecvJson,"wxid1");
				if(item)
					mInfo.Wxid_1=item->valuestring;

				//mInfo.Wxid_1		= cJSON_GetObjectItem(RecvJson,"wxid1")->valuestring;
				item= cJSON_GetObjectItem(RecvJson,"wxid2");
				if(item)
					mInfo.Wxid_2=item->valuestring;
				//mInfo.Wxid_2		= cJSON_GetObjectItem(RecvJson,"wxid2")->valuestring;
				item= cJSON_GetObjectItem(RecvJson,"msg");
				if(item)
					mInfo.Msg=item->valuestring;

				//mInfo.Msg			= cJSON_GetObjectItem(RecvJson,"msg")->valuestring;
				item= cJSON_GetObjectItem(RecvJson,"source");
				if(item)
					mInfo.Source=item->valuestring;

				//mInfo.Source		= cJSON_GetObjectItem(RecvJson,"source")->valuestring;

				int B64_Length = strlen(mInfo.Msg);
				int Byte_Length=0;
				int Ret=0;
				if(B64_Length)
				{
			Byte_Length = ((B64_Length / 4) * 3) + 10;
				BYTE* mByte = new BYTE[Byte_Length];
				memset(mByte,0,Byte_Length);
				Ret = BASE64_Decode(mInfo.Msg, B64_Length, mByte);
				strcpy(mInfo.Msg,(char*)mByte);
				delete mByte;
				}

				if(RecvType == 回调_文件消息||RecvType == 回调_语音消息 || RecvType == 回调_图片消息)
				{
					B64_Length = strlen(mInfo.Source);
					if(B64_Length)
					{
					Byte_Length = ((B64_Length / 4) * 3) + 10;
					BYTE* sByte = (BYTE*)malloc(Byte_Length);
					memset(sByte,0,Byte_Length);
					Ret = BASE64_Decode(mInfo.Source, B64_Length, sByte);
					if(RecvType != 回调_文件消息)
					strcpy(mInfo.Source,SaveFile(mInfo.RecvWxid, mInfo.Wxid_1, RecvType, mInfo.ServerMsgID, sByte, Ret));
					free(sByte);
					}
Wxretdata tempwxretdata;
	tempwxretdata.MsgType = 111111;
	tempwxretdata.CreateTime=dwTime;
tempwxretdata.NewMsgId=mInfo.LocalMsgID;
	CString SendWxid,RecvWxid;
	
	if(mInfo.IsSend&&!mInfo.LocalMsgID)
	{
	}
	else
	{
	if(mInfo.IsSend)
		{
	SendWxid=g_selfwxid;
	RecvWxid=mInfo.Wxid_1;
		}
	else
	{
	
SendWxid=mInfo.Wxid_1;
RecvWxid=mInfo.Wxid_2;
	}

	tempwxretdata.FromUserName=SendWxid;
	CString msgtousername=RecvWxid;
	//tempwxretdata.FromGroupName=sendwxid;
	CString msgfromgroupname=SendWxid;
	
	if(msgfromgroupname.Find(_T("@chatroom"))!=-1)
				{
					tempwxretdata.FromGroupName=msgfromgroupname;//"@195bff9b154a53ac4ed5495d2eb3f56c58e60983f40335cd2acf0ed55a36c044:
					
						if(lstrcmp(RecvWxid,_T("-"))!=0)
					tempwxretdata.FromUserName=RecvWxid;

					

				}
				else
				{
					tempwxretdata.FromUserName=(CString)msgfromgroupname;
					if(msgtousername.Find(_T("@chatroom"))!=-1)//如果是自己另外一个客户端发出的消息
						tempwxretdata.FromGroupName=msgtousername;
				}

					INT64	sendqq=GetFriendUidFromUserName(GetGroupUidFromUserNameOrNickName(tempwxretdata.FromGroupName,0),tempwxretdata.FromUserName);	//对于群消息，fromuin是群号，对于好友消息和临时消息，fromuin都是对方的qq号	

					setvector<Wxretdata> myqqretdatavect;

		
			CString defaultmsg;
			CString localfile;
			localfile=mInfo.Source;
			if(RecvType == 回调_图片消息)
			{
defaultmsg=_T("收到图片");
			}
			else if(RecvType == 回调_语音消息)
			{
defaultmsg=_T("收到语音");
			}
			else if(RecvType == 回调_文件消息)
			{
defaultmsg=_T("收到文件");
			}

			tempwxretdata.Content.Format(_T("%s%s"), defaultmsg, localfile);
			BOOL repeatflag=false;
			if(mInfo.IsSend)
			{
				static CString lastpath;
				if(lastpath==tempwxretdata.Content)
					repeatflag=true;
			
				lastpath=tempwxretdata.Content;
			}
			if(!repeatflag)
			{
			if(RecvType == 回调_图片消息)
			{
				INT64 groupid=GetGroupIDFromGroupWxid(tempwxretdata.FromGroupName);
					FunForRecognize(localfile,groupid,sendqq,tempwxretdata);

		/*	lockqqimgres.Lock();
			CString relativefile=localfile;

			CString backstr;
			::function::GetHalfStr(relativefile,NULL,&backstr,_T("群图片文件\\"),true);
			relativefile.Format(_T("群图片文件\\%s"),backstr);
			g_qq2lastimgmap[sendqq]=relativefile;
			g_qq2lastimgmap[(INT64)0]=relativefile;
			lockqqimgres.Unlock();*/

			}
			else
			{
			myqqretdatavect.push_back(tempwxretdata);
			::SendMessage(mainhwnd, WM_NEWMSG, 0, (LPARAM)&myqqretdatavect);
			}

			}
			//显示收到一条文件信息
	}
				}
		
				/*memset(data_str,0,10000);
				sprintf(data_str,"[%s][%s]\r\nType：%d   时间：%s\r\n消息ID：%d\r\n服务ID：%s\r\nWxid_1：%s\r\nWxid_2：%s\r\n消息内容：%s\r\n文件路径：%s",
					mInfo.GetSender(mInfo.IsSend),
					mInfo.RecvWxid,
					mInfo.Type,
					mInfo.Time,
					mInfo.LocalMsgID,
					mInfo.ServerMsgID,
					mInfo.Wxid_1,
					mInfo.Wxid_2,
					mInfo.Msg,
					mInfo.Source);

				Log(data_str);*/
					}
			}
			else if(RecvType == 回调_添加好友)
			{
				char* RecvWxid=NULL;
				
				cJSON*	item= cJSON_GetObjectItem(RecvJson,"mywxid");
				if(item)
					RecvWxid=item->valuestring;

			//	= cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;

					if(strcmp(tempwxid,RecvWxid)==0)
{
				char* uWxid	=NULL;
				cJSON*	item= cJSON_GetObjectItem(RecvJson,"wxid");
				if(item)
					uWxid=item->valuestring;
				//= cJSON_GetObjectItem(RecvJson,"wxid")->valuestring;
					char* uData	=NULL;
				item= cJSON_GetObjectItem(RecvJson,"data");
				if(item)
					uData=item->valuestring;
				//char* uData		= cJSON_GetObjectItem(RecvJson,"data")->valuestring;
				int uType	=0;
				item= cJSON_GetObjectItem(RecvJson,"id");
				if(item)
					uType=item->valueint;
				
				//int uType		= cJSON_GetObjectItem(RecvJson,"id")->valueint;
				int uSource		= NULL;//cJSON_GetObjectItem(RecvJson,"scene")->valueint;
				item= cJSON_GetObjectItem(RecvJson,"scene");
				if(item)
					uSource=item->valueint;

				if (uType == -44)
				{
					CStringA uMsg;
					//Dlg->GetDlgItemText(FRI_TXT_TEXT,uMsg);
					Wx_AddFriend(so,2,uSource,uWxid,"",uMsg.GetBuffer());
				}
			/*	memset(data_str,0,520);
				sprintf(data_str,"[添加好友][%s]\r\n添加Wxid：%s\r\n添加来源：%d\r\n返回消息：[%d]%s",RecvWxid,uWxid,uSource,uType,uData);
				Log(data_str);*/
					}
			}
			else if (RecvType == 回调_下载信息)
			{
			/*	UserInfo uInfo = {0};
				char* RecvWxid	= cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;
					if(strcmp(tempwxid,RecvWxid)==0)
{
				cJSON* uUser = cJSON_GetObjectItem(RecvJson,"info");
				if (uUser)
				{
					uInfo.Wxid		= cJSON_GetObjectItem(uUser,"Wxid")->valuestring;
					uInfo.WxNo		= cJSON_GetObjectItem(uUser,"WxNo")->valuestring;
					uInfo.Nick		= cJSON_GetObjectItem(uUser,"Nick")->valuestring;
					uInfo.v1		= cJSON_GetObjectItem(uUser,"v1")->valuestring;
					uInfo.HImg		= cJSON_GetObjectItem(uUser,"HImg")->valuestring;
					uInfo.Sex		= uInfo.GetSex(cJSON_GetObjectItem(uUser,"Sex")->valueint);
					uInfo.Address	= cJSON_GetObjectItem(uUser,"Address")->valuestring;
				}
				memset(data_str,0,5200);
				sprintf(data_str,"[详细信息][%s]\r\nWxid：%s\r\n微信号：%s\r\n昵称：%s\r\n性别：%s\r\n地区：%s\r\nV1：%s\r\n头像：%s",RecvWxid,
					uInfo.Wxid,
					uInfo.WxNo,
					uInfo.Nick,
					uInfo.Sex,
					uInfo.Address,
					uInfo.v1,
					uInfo.HImg);
				Log(data_str);

					}
					*/
			}
			else if (RecvType == 回调_建群返回)
			{	
			/*	char* RecvWxid	= cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;
					if(strcmp(tempwxid,RecvWxid)==0)
{
				char* RecvData  = cJSON_GetObjectItem(RecvJson,"data")->valuestring;
				char* uGroupID  = cJSON_GetObjectItem(RecvJson,"gid")->valuestring;
				memset(data_str,0,2048);
				sprintf(data_str,"[建群返回][%s]\r\n结果：%s\r\n群ID：%s\r\n成员：",RecvWxid,RecvData,uGroupID);
				cJSON* UserArray = cJSON_GetObjectItem(RecvJson,"list");
				if (UserArray)
				{
					int uCount = cJSON_GetArraySize(UserArray);
					for (int i=0;i<uCount;i++)
					{
						cJSON* uUser	= cJSON_GetArrayItem(UserArray,i);
						char* uWxid		= cJSON_GetObjectItem(uUser,"wxid")->valuestring;
						int ustatus		= cJSON_GetObjectItem(uUser,"status")->valueint;
						sprintf(data_str,"%s\r\n%s ---> [%d]Wxid：%s",data_str,ustatus?"失败":"成功",ustatus,uWxid);
					}
				}
				Log(data_str);
					}*/
			}
			else if (RecvType == 回调_A8Key)
			{
		/*		char* RecvWxid	= cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;
					if(strcmp(tempwxid,RecvWxid)==0)
{
 				char* LastURL	= cJSON_GetObjectItem(RecvJson,"lasturl")->valuestring;
				char* RecvURL	= cJSON_GetObjectItem(RecvJson,"url")->valuestring;
				memset(data_str,0,2048);
				sprintf(data_str,"[聊天对象][%s]\r\n请求链接：%s\r\n返回链接：%s",RecvWxid,
					LastURL,
					RecvURL);
				Log(data_str);

				cJSON* Groupid	= cJSON_GetObjectItem(RecvJson,"gid");
				if (Groupid)
				{
					memset(data_str,0,2048);
					sprintf(data_str,"[接受邀请入群][%s]\r\n群ID：%s@chatroom",RecvWxid,
						Groupid->valuestring);
					Log(data_str);
				}
					}*/
			}
			else if (RecvType == 回调_聊天对象)
			{
			/*	char* RecvWxid	= cJSON_GetObjectItem(RecvJson,"mywxid")->valuestring;
	if(strcmp(tempwxid,RecvWxid)==0)
	{
				UserInfo uInfo = {0};
				uInfo.Wxid		= cJSON_GetObjectItem(RecvJson,"Wxid")->valuestring;
				uInfo.WxNo		= cJSON_GetObjectItem(RecvJson,"WxNo")->valuestring;
				uInfo.Nick		= cJSON_GetObjectItem(RecvJson,"Nick")->valuestring;
				uInfo.Mark		= cJSON_GetObjectItem(RecvJson,"mark")->valuestring;
				uInfo.HImg		= cJSON_GetObjectItem(RecvJson,"himg")->valuestring;

				memset(data_str,0,2048);
				sprintf(data_str,"[聊天对象][%s]\r\nWxid：%s\r\n微信号：%s\r\n昵称：%s\r\n备注：%s\r\n头像：%s",RecvWxid,
					uInfo.Wxid,
					uInfo.WxNo,
					uInfo.Nick,
					uInfo.Mark,
					uInfo.HImg);
				Log(data_str);
	}*/
			}	
		}
		free(s_Recv);
		cJSON_Delete(RecvJson);
	}
	ThreadLock.Unlock();
}
BOOL InitWxHook()
{
		
			CString dllpath=::function::complete_path(_T("机器人插件\\kstex.dll"));

	if(!PathFileExists(dllpath))
	{
		MessageBox(mainhwnd,_T("缺少配套的kstex.dll，请到http://www.qqshow123.com/game/kstex.dll下载，同时你可能还缺少配套的kstapi.dll，请到http://www.qqshow123.com/game/kstapi.dll下载、以及配套的kst.dll，请到http://www.qqshow123.com/game/kst.dll下载（有可能需要关闭杀毒软件，因为部分杀毒软件会视此文件为微信外挂），然后手动拷贝这三个文件到机器人安装目录下的“机器人插件”目录下， 或者联系qq：13402429获取！"),_T("缺少配套的kstex.dll等文件"),0);
		return false;
	}
	dllpath=::function::complete_path(_T("机器人插件\\kst.dll"));

	if(!PathFileExists(dllpath))
	{
		MessageBox(mainhwnd,_T("缺少配套的kst.dll，请到http://www.qqshow123.com/game/kst.dll下载，然后把这个文件放到机器人安装目录下的“机器人插件”目录下， 或者联系qq：13402429获取！"),_T("缺少配套的kst.dll"),0);
		return false;
	}
	dllpath=::function::complete_path(_T("机器人插件\\kstapi.dll"));

	if(!PathFileExists(dllpath))
	{
		MessageBox(mainhwnd,_T("缺少配套的kstapi.dll，请到http://www.qqshow123.com/game/kstapi.dll下载，然后把这个文件放到机器人安装目录下的“机器人插件”目录下， 或者联系qq：13402429获取！"),_T("缺少配套的kstapi.dll"),0);
		return false;
	}


			if (!Init_KstAPI())
	{
		MessageBox(mainhwnd,_T("DLL组件加载失败,请检查机器人安装路径下的“机器人插件”目录下有没有Kst.dll、kstapi、kstex.dll三个文件!"),_T(""),0);
		return false;
	}

		
	Kst_GetPath(g_WxInstallPath);
	
	CString windllpath;
	windllpath=g_WxInstallPath;
	windllpath+=_T("\\WeChatWin.dll");
		int wechatsize2=::function::GetFileSizeFromPath(windllpath);

		if(wechatsize2!=21579512&&wechatsize2!=22879728)//22,879,728 21579512
		{
		
	MessageBox(mainhwnd,_T("未发现你电脑上已安装微信电脑版，请先安装电脑微信2.6.8.65版本再使用此软件(请在接下来自动打开的微云盘中下载此电脑微信版，或者联系作者qq：13402429获取此版本微信软件，或自行到百度上搜索“电脑微信2.6.8.65版”，安装好了之后，再重新启动机器人软件。如果你安装了2.5版，请在选项设置——全局设置去掉“使用2.6版微信客户端”的勾选。)"),_T("需要安装2.6.8.65版微信客户端，或者在全局设置去掉“使用2.6版微信客户端”的勾选"),0);
			ShellExecute(NULL,NULL,_T("https://share.weiyun.com/5UkPeg2"),0,0,1);
			return false;
		}
			if(wechatsize2==22879728)
			g_versionnewest27=true;
	//初始化Socket
	kstSocket_Init((int)Server_CallBack, 0, 0);
	Socket_Handle = kstSocket_Create("127.0.0.1", 0, 1);
	if (Socket_Handle == 0)
	{
		AfxMessageBox(_T("通信初始化失败,请重试!"));
		TerminateProcess(GetCurrentProcess(),TRUE);
	}
	Socket_Port = kstSocket_Port(Socket_Handle);

	//memset(data_str,0,520);
	//sprintf(data_str,"[通信初始化]\r\n通信句柄：%d\r\n通信端口：%d",Socket_Handle,Socket_Port);
	//Log(data_str);

	Kst_DllConnect(201906061, Socket_Port);


	if (Socket_Port == 0)
	{
		AfxMessageBox(_T("启动失败,无可用通信端口!"));
		return false;
	}
	
		
	return true;
}

void TryNewestHook()
{
if(g_newestversion)
		{
			BOOL bInitOk=false;
	if(!alreadyconnect||strlen(g_WxInstallPath)==0)
	{
	bInitOk=InitWxHook();
	}
	if(!bInitOk)
		memset(g_WxInstallPath,0,sizeof(g_WxInstallPath));

	if(bInitOk)
	{
		int ncount=0;
		while(ncount<10)
		{
			ncount++;
			if(alreadyconnect)
				break;

		Sleep(100);
		}
		if(!alreadyconnect)
		{
	char savepath[256];
	GetCurrentDirectoryA(255,savepath);
	SetCurrentDirectoryW(g_plusdir);
	g_wxpid = Kst_CreateWx(g_WxInstallPath, "WeChat.exe", "kst.dll", Socket_Port);
		SetCurrentDirectoryA(savepath);
	/*memset(data_str,0,520);
	sprintf(data_str,"[创建微信进程]\r\n进程PID：%d",g_wxpid);
	Log(data_str);*/
		}
	}
		}
		
}

BOOL LaunchWindowsWechat()
{
	
	if(g_newestversion)
	{
		if(!mywinthread5)
	mywinthread5=AfxBeginThread(proPbSendMsgFun,NULL);
	}

	TryNewestHook();
		if(!g_newestversion)
		{
	CString dllpath=::function::complete_path(_T("机器人插件\\hookwechat.dll"));
	CStringA dllpatha=(CStringA)dllpath;
	
	if(!PathFileExists(dllpath))
	{
		MessageBox(mainhwnd,_T("缺少配套的hookwechat.dll，请到http://www.qqshow123.com/game/hookwechat.dll下载，同时你可能还缺少配套的wcinterface.dll，请到http://www.qqshow123.com/game/wcinterface.dll下载（有可能需要关闭杀毒软件，因为部分杀毒软件会视此文件为微信外挂），然后手动拷贝这两个文件到机器人安装目录下的“机器人插件”目录下，或者直接下载绿色免安装的压缩包：http://www.qqshow123.com/wxrobot.rar 或者联系qq：13402429获取！"),_T("缺少配套的hookwechat.dll"),0);
		return false;
	}

	g_latestwechatflag=true;

	CString wcinterfacepath=::function::complete_path(_T("机器人插件\\wcinterface.dll"));
		int wechatsize=::function::GetFileSizeFromPath(wcinterfacepath);

		if(!PathFileExists(wcinterfacepath))
	{
		MessageBox(mainhwnd,_T("缺少配套的wcinterface.dll，请到http://www.qqshow123.com/game/wcinterface.dll下载，然后手动拷贝到机器人安装目录下的“机器人插件”目录下，或者联系qq：13402429获取！"),_T("缺少配套的wcinterface.dll"),0);
		return false;
	}

		if(wechatsize==843776)
		{
			g_latestwechatflag=false;
		}

		bExitPostThread=0;
		FindOverevent.ResetEvent();

	STARTUPINFO si;
	
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&g_wxpi,sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	// CHAR* DirPath = new CHAR[MAX_PATH];     
	// CHAR* DLLPath = new CHAR[MAX_PATH]; //testdll.dll     
	// CHAR* DetourPath = new CHAR[MAX_PATH]; //detoured.dll   
	// GetCurrentDirectoryA(MAX_PATH, DirPath);     
	// sprintf_s(DLLPath, MAX_PATH, "%s\\WeChatHook.dll", DirPath);     
	// sprintf_s(DetourPath, MAX_PATH, "%s\\Win32Project1.dll", DirPath);    
	// sprintf_s(DLLPath, MAX_PATH, "%s\\Win32Project1.dll", DirPath);   
	//strcpy(DLLPath,"C:\\项目代码\\晨风微信机器人\\debug\\Win32Project1.dll");
	
	TCHAR getkeyvalue[1024];//=_T("C:\\项目代码\\晨风微信机器人\\debug\\wechat2\\wechat.exe");
	lstrcpy(getkeyvalue,g_wechatpath);


	if(!lstrlen(getkeyvalue)||!PathFileExists(getkeyvalue))
	{


		HKEY rootkey=HKEY_CURRENT_USER;

		lstrcpy(getkeyvalue,_T("InstallPath"));
		LPCTSTR rtpath=::function::GetRegistryString(rootkey,_T("Software\\Tencent\\WeChat"),getkeyvalue);
		if(rtpath)
		{
			lstrcat(getkeyvalue,_T("\\WeChat.exe"));
		}

		
		if(!rtpath||lstrlen(getkeyvalue)==0||!PathFileExists(getkeyvalue))
		{
			MessageBox(mainhwnd,_T("未发现你电脑上已安装微信电脑版，请先安装电脑微信2.5.0.67版本再使用此软件(请在接下来自动打开的微云盘中下载此电脑微信版，或者联系作者qq：13402429获取此版本微信软件，或自行到百度上搜索“电脑微信2.5.0.67版”，安装好了之后，再重新启动机器人软件。)，如果你已经安装了，请手动选择电脑微信版的可执行文件！"),_T("需要手动选择微信可执行文件"),0);
			ShellExecute(NULL,NULL,_T("https://share.weiyun.com/5lVBk4Q"),0,0,1);

			TCHAR buflan[512]=_T("可执行文件|*.exe||");
			CFileDialog  filedlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,buflan);
			if(filedlg.DoModal()!=IDOK)
			{
				return false;
			}
			lstrcpy(getkeyvalue,filedlg.GetPathName());

		}
	

		//
	}

	//getkeyvalue
	//如果已经启动了，提示对方要先手动退出之前登录的微信客户端

	//487616;
	if(!g_latestwechatflag)
	{
		int wechatsize=::function::GetFileSizeFromPath(getkeyvalue);
		CString windllpath;
		//C:\Program Files (x86)\Tencent\WeChat\WeChat.exe
		//C:\Program Files (x86)\Tencent\WeChat\WeChatWin.dll
		windllpath=::function::Getpathfromname(getkeyvalue);
		windllpath+=_T("WeChatWin.dll");
		int wechatsize2=::function::GetFileSizeFromPath(windllpath);

		if(wechatsize2==21579512||wechatsize2==22879728)
		{
			g_newestversion=true;
		g_latestwechatflag=true;
		if(wechatsize2==22879728)
			g_versionnewest27=true;
		}
		else
		{
		//7,486,144
		if(wechatsize!=487616||wechatsize2!=7486144)
		{
			MessageBox(mainhwnd,_T("你电脑上安装的微信不是2.4.5.73版，你是否无意中选择了升级到最新的电脑微信版？请重新安装电脑微信2.4.5.73版本再使用此软件！(请联系作者qq：13402429获取此版本微信软件，或自行到百度上搜索“电脑微信2.4.5.73版”)"),_T("请联系作者获取微信电脑版2.4.5.73版本"),0);
			return false;
		}
		}
	}
	else//17884352
	{

		if(g_latestwechatflag)
		{
			CString wechat2=::function::complete_path(_T("wechat\\wechat.exe"));
			if(PathFileExists(wechat2))
				lstrcpy(getkeyvalue,wechat2);
		}

		int wechatsize=::function::GetFileSizeFromPath(getkeyvalue);
		CString windllpath;
		//C:\Program Files (x86)\Tencent\WeChat\WeChat.exe
		//C:\Program Files (x86)\Tencent\WeChat\WeChatWin.dll
		windllpath=::function::Getpathfromname(getkeyvalue);
		windllpath+=_T("WeChatWin.dll");
		int wechatsize2=::function::GetFileSizeFromPath(windllpath);
		
		if(wechatsize2==21579512||wechatsize2==22879728)//21579512  22879728
		{
			g_newestversion=true;
		g_latestwechatflag=true;
		if(wechatsize2==22879728)
			g_versionnewest27=true;
		}
		else
		{
		//7,486,144
		if(wechatsize!=490688||wechatsize2!=17884352)
		{
			MessageBox(mainhwnd,_T("你电脑上安装的微信不是2.5.0.67版，你是否无意中选择了升级到最新的电脑微信版？请重新安装电脑微信2.5.0.67版本再使用此软件！"),_T("请联系作者获取微信电脑版2.5.0.67版本"),0);
			ShellExecute(NULL,NULL,_T("https://share.weiyun.com/5lVBk4Q"),0,0,1);
			//ShellExecute(NULL,NULL,_T("https://share.weiyun.com/5UkPeg2"),0,0,1);

			return false;
		}
		}

		//SetPrivilege();
	}

	g_wechatpath=getkeyvalue;

	if(!g_newestversion)
	{

	AfxBeginThread(ProCheckMsg, 0);
	
	if(WaitForSingleObject(FindOverevent,20*1000)==WAIT_TIMEOUT)
	{
		MessageBox(mainhwnd,_T("等待查找可用端口超时！"),0,0);
		return false;
	}
	CString cmdline;
	cmdline.Format(_T("%s --Port:%d"),getkeyvalue,g_readportnum);
	TCHAR cmdbuf[256];
	lstrcpy(cmdbuf,cmdline);
	DetourCreateProcessWithDllEx(getkeyvalue,cmdbuf,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&si,&g_wxpi,dllpatha,NULL);
	}
	else
	{
		TryNewestHook();
	
	

	}
	//if(g_latestwechatflag)
	//::CreateThread(NULL,0,TimeChangeTimeProFun,NULL,0,NULL);

	//DetourCreateProcessWithDllEx( _T("C:\\项目代码\\测试qq通讯\\Debug\\simulator.exe"), NULL,NULL,NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL,&si, &pi, DLLPath, NULL);    
	//DetourCreateProcessWithDllEx( _T("C:\\项目代码\\测试qq通讯\\Debug\\simulator.exe"), NULL,NULL,NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL,&si, &pi, DLLPath, NULL);     
	}
	return true;
}
UINT ProCheckMsg(LPVOID p)  
{  
     WSADATA wsaData;  
  
     WORD wVersion;  
  
     wVersion = MAKEWORD(2,2);  
  
     WSAStartup(wVersion,&wsaData);  
       int res;  
     char msg[1025]; 
	 CString nowportstr;

	 nowportstr=::function::CFSoft_path(_T("communication.bin"),NULL,0);//::function::complete_path(_T("机器人插件\\communication.bin"));
	 CFile cook_file1;
	 int readportnum=7070;
	 if(cook_file1.Open(nowportstr,CFile::modeRead|CFile::shareDenyNone))
	{
		 cook_file1.Read(&readportnum,sizeof(int));
		
		cook_file1.Close();
	}
	 int nextport=(readportnum-7071+1)%30+7071;

	 if(!FindAvailablePort(nextport))//创建成功后必须把成功的端口号写入到配置文件中，下一个进程从+1端口号中找到可用的端口，如果大于7100，则又是从7071开始使用
	 {
		 MessageBox(0,_T("找不到可用的端口，请确认相关端口未被占用！"),_T("端口被占用！"),0);
		 return 0;
	 }
     else  
     {  
     
		 // CString port;  
         // port.Format("%d", int(ntohs(client_addr.sin_port)));  
        //  dlg->Update( "已连接来自：" + CString(inet_ntoa(client_addr.sin_addr)) + "  端口：" +  port);  
     }  

	
	/* CStdioFile filerec;
   	filerec.Open(_T("d:\\http_rec6.log"),CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
  */   //接收数据  
	vectorbyte curbuf;
	while(!bExitPostThread)
	{
		
	BOOL bOK=Fun_Org_recv(curbuf);


	if(bOK)
	{
		::SendMessage(mainhwnd,WM_SETTEXT,0,(LPARAM)_T("有新消息！"));
		m_alreadylogin = true;
		fun_recv_analyse(curbuf);



		if(!g_reporturl.IsEmpty())
		{
			static DWORD lasttime=0;
			DWORD nowrelogintime=GetTickCount();


			if(nowrelogintime-lasttime>100*1000)
			{
				lasttime=nowrelogintime;
				AfxBeginThread(ProReportOnline,NULL);
			}
		}
	}
		//if((res = recv(g_socket,msg,4,0))==-1)
		//{
		//	//    dlg->Update("失去连接");  
		//	break;
		//}
		//else
		//{

		//	vectorbyte tempmsg=char2vector(msg,res);
		//	//ShowArrayByte(tempmsg,tempmsg);
		//	_Unpack unpack1;
		//	
		//	unpack1.SetData(tempmsg);
		//	//msg[res] = '\0';
		//	filerec.Write(msg,res);
		//	filerec.Flush();
		//	// MessageBoxA(0,msg,0,0);
		//	//  dlg->Update("client:" + CString(msg));  
		//}
	}
	// filerec.Close();

    return 0;  
}

extern int Fun_send(const_vectorbyte& bin);

void public_rc4(vectorbyte inbuffer,vectorbyte& outstr)//,string key
{
	//	string cleartext = "中国北京12345$abcde%ABCDE@！！！";  
	// string ciphertext = "";  
	string inbuf2="test";
	vectorbyte key;
	//muzigaiyu

	char keybuf[64];
	char tempbuf1[64]={70,89,50,48,49,55,0};
	char tempbuf2[64]={109,117,122,105,103,97,105,121,117,0};
	if(g_latestwechatflag)
		strcpy(keybuf,tempbuf1);
	else
		strcpy(keybuf,tempbuf2);

	key=char2vector(keybuf,strlen(keybuf));//key=keybuf;
    outstr = RC4_Encrypt(inbuffer, key);  
    //string decrypt = RC4_Decrypt(ciphertext, key);  
}

void ServerSendPack(vectorbyte sendout)
{

	vectorbyte rtout;

	public_rc4(sendout,rtout);
	//ShowArrayByte(rtout,rtout);
	_Pack bf2;
	bf2.SetInt(rtout.size());
	bf2.SetBin(rtout);
	Fun_send(bf2.GetAll());
}
CStringA emmmreplace2face(const CString &msg)
{
	CStringA rtemmmstr;
	rtemmmstr=msg;
if(StrStr(msg,_T("emmm")))
	{
		CString expressstr;
		CStringvect weather6vect;
		expressstr.Format(_T("(?<=emmm)\\d{5,6}"));
		GetRegEX(msg,expressstr,weather6vect,0,true);//need consider of the actual keyname

		if(weather6vect.size())
		{
			CStringA newstr,oldstr;
			char wnum[3]={ 0,0,0 };

			for(int i=0;i<weather6vect.size();i++)
			{
				oldstr.Format("emmm%s",(CStringA)weather6vect[i]);
				if(rtemmmstr.Find(oldstr)!=-1)
				{
					CStringA num1str;
					CStringA num2str;
					num1str=weather6vect[i].Left(3);
					num2str=weather6vect[i].Mid(3);

					int newnum1=atoi(num1str);
					int newnum2=atoi(num2str);

					

					wnum[0]=newnum1;
					wnum[1]=newnum2;
					wnum[2]=0;

					newstr=wnum;

					rtemmmstr.Replace(oldstr,newstr);
				}
			}

		}


}
return rtemmmstr;
}

void SendWeChatMessage(LPCTSTR wxid,LPCTSTR msg,int type)//type=1是文字，=2是图片路径，=3是文件路径
{
	if(g_newestversion)
	{
	//emmm253172

		CStringA wxida;
		CStringA atwxid;
		CStringA msga;
		msga=msg;//emmmreplace2face(msg);
		wxida=wxid;
		if(type==1)
		Wx_SendTextMsg(Socket_Client,wxida.GetBuffer(0),atwxid.GetBuffer(0),msga.GetBuffer(0));
		else if(type==2)
		{

		
		if(StrStrI(msg,_T(".gif")))  
		Wx_SendFileMsg(Socket_Client, 2, wxida.GetBuffer(), msga.GetBuffer());
		else
		Wx_SendFileMsg(Socket_Client, 1, wxida.GetBuffer(), msga.GetBuffer());
		}
		else if(type==3)
		{

		Wx_SendFileMsg(Socket_Client, 3, wxida.GetBuffer(), msga.GetBuffer());
		}
		
	}
	else
	{
	_Pack bf;
	if(g_latestwechatflag)
bf.SetByte(20);//#send_发送消息==20
	else		
bf.SetByte(1); // ' 发送消息
	

	bf.SetToken(wxid);

	if(type==2)
	{
			//Sleep(300);
		if(StrStrI(msg,_T(".gif")))
        bf.SetInt(4);//如果是动画表情，则是4
		else
		bf.SetInt(type);
	}
	else
	bf.SetInt(type);


	bf.SetToken(msg);
	//ServerSendPack(socket,bf.GetBuffer());
	//vectorbyte sendout;
	
	//sendout.assign(bf.GetAll().begin(),bf.GetAll().end());
	ServerSendPack(bf.GetAll());
	}
}
//<msg>
//    <fromusername>微信ID</fromusername>
//    <scene>0</scene>
//    <commenturl></commenturl>
//    <appmsg appid="" sdkver="0">
//        <title>专注PC微信各种操作！</title>
//        <des>这不是吹B</des>
//        <action>view</action>
//        <type>5</type>
//        <showtype>0</showtype>
//        <content></content>
//        <url>https://www.baidu.com/s?wd=%E6%88%91%E5%A6%BB%E7%94%B1%E8%89%AF</url>
//        <dataurl></dataurl>
//        <lowurl></lowurl>
//        <thumburl>https://imgsa.baidu.com/forum/w%3D580/sign=4af80a3b6863f6241c5d390bb745eb32/64495159252dd42a90d380a9003b5bb5c8eab8ba.jpg</thumburl>
//        <recorditem>
//            <![CDATA[]]>
//        </recorditem>
//        <extinfo></extinfo>
//        <sourceusername></sourceusername>
//        <sourcedisplayname></sourcedisplayname>
//       <commenturl></commenturl>
//        <appattach>
// <totallen>0</totallen>
//            <attachid></attachid>
//      <emoticonmd5></emoticonmd5>
//            <fileext></fileext>
//        </appattach>
//    </appmsg>
//    <appinfo>
//        <version>1</version>
//        <appname>Window wechat</appname>
//    </appinfo>
//</msg>

//<?xml version="1.0"?> <msg bigheadimgurl="" smallheadimgurl="" username="微信ID" nickname="我不想装B" fullpy="chulian" shortpy="" alias="" imagestatus="0" scene="0" province="" city="" sign="" sex="0" certflag="0" certinfo="" brandIconUrl="" brandHomeUrl="" brandSubscriptConfigUrl="" brandFlags="0" regionCode="" antispamticket="微信ID" /> 
void SendWeChatMsgEx(LPCTSTR wxgroupid,LPCTSTR msg,LPCTSTR atwxid,int type)//发名片type=42，发@群友type=1
{

	if(g_newestversion)
	{
		if(type==42)
		{
		
		}
		else
		{
	CStringA wxida;
		CStringA atwxida;
		CStringA msga;
		atwxida=atwxid;

		msga=msg;//emmmreplace2face(msg);
		wxida=wxgroupid;
		Wx_SendTextMsg(Socket_Client,wxida.GetBuffer(0),atwxida.GetBuffer(0),msga.GetBuffer(0));
		}
	}
	else
	{
	_Pack bf;

	if(g_latestwechatflag)
		bf.SetByte(22);//#send_发送消息Ex
	else
	bf.SetByte(5);

	bf.SetToken(wxgroupid);
	bf.SetToken(msg);
	bf.SetToken(atwxid);
	bf.SetInt(type);
	ServerSendPack(bf.GetAll());
	}
}
void SendpackForAnnouncement(LPCTSTR chatroom,LPCTSTR msg)//
{
	if(g_newestversion)
	{
		CStringA uGroupID;
		uGroupID=chatroom;
		CStringA uNotice;
		uNotice=msg;//emmmreplace2face(msg);
	WxGroup_SetNotice(Socket_Client, Client_Wxid.GetBuffer(0), uGroupID.GetBuffer(), uNotice.GetBuffer());
	}
	else
	{

	_Pack bf;

	if(g_latestwechatflag)
		bf.SetByte(19);//#send_发送群公告=19
	else
	bf.SetByte(8);
	bf.SetToken(chatroom);
	bf.SetToken(msg);

	ServerSendPack(bf.GetAll());
	}
}
void SendpackForQuitGroup(LPCTSTR chatroom,int type)//
{
	SendpackForDeleteFriendOrGroup(chatroom,type);

}
void SendpackForDeleteFriend(LPCTSTR friendwxid)//
{
	SendpackForDeleteFriendOrGroup(friendwxid,0);

}

//wxidArray ＝ wxidArray ＋ { 94, 0, 71, 0 }  ' ^G 分隔符
void SendpackForCreateGroup(const_vectorbyte chatroommemberlist)//
{
	_Pack bf;

	if(g_latestwechatflag)
		bf.SetByte(15);//#send_创建群聊
	else
	bf.SetByte(15);

	bf.SetToken(chatroommemberlist);


	ServerSendPack(bf.GetAll());

}



void SendpackForGetA8Key(LPCTSTR wxid,LPCTSTR url)//
{
	if(g_newestversion)
	{
		CStringA wxida;
		wxida=wxid;
		CStringA urla;
		urla=url;
	Wx_GetA8Key(Socket_Client, 1, wxida.GetBuffer(), urla.GetBuffer(), 1);
	}
	else
	{
	_Pack bf;

	bf.SetByte(23);//23 #send_GetA8Key
	bf.SetToken(wxid);
		bf.SetToken(url);
		ServerSendPack(bf.GetAll());

	}



}
void SendpackForRecPayment(LPCTSTR transferid,LPCTSTR wxid,int time)//
{
				if(g_newestversion)
						{
							CStringA sendwxida;
							sendwxida=wxid;
							CStringA tranida;
							tranida=transferid;
							Wx_GetMoney(Socket_Client, sendwxida.GetBuffer(0), tranida.GetBuffer(0), time);
						}
	else
	{
	_Pack bf;

	
//bf.CreateBuffer (_add2 (20, 取字节集长度 (transferid), 取字节集长度 (wxid)))
	bf.SetByte(10);//10#send_收款
	bf.SetToken(transferid);
	bf.SetToken(wxid);
	bf.SetInt(time);
	ServerSendPack(bf.GetAll());

				}

}

void SendpackForAtAllGroupPeoPle(LPCTSTR chatroom,LPCTSTR msg)//
{
	_Pack bf;

	if(g_latestwechatflag)
		bf.SetByte(18);//#send_群at所有人=18
	else
	bf.SetByte(18);
	bf.SetToken(chatroom);
	bf.SetToken(msg);

	ServerSendPack(bf.GetAll());

}

void SendWeChatMsgHttp(LPCTSTR wxid,LPCTSTR msg,int type)//发送http链接，type=5
{
	if(g_newestversion)
	{
		CStringA uWxid;
		uWxid=wxid;
		CStringA msga;
		msga=msg;
		//g_selfwxid

	Wx_SendURLMsg(Socket_Client, uWxid.GetBuffer(0),Client_Wxid.GetBuffer(0),msga.GetBuffer(0));
	}
	else
	{
	_Pack bf;
	if(g_latestwechatflag)
	bf.SetByte(21);//#send_发送http消息=21
	else
	bf.SetByte(4);
	bf.SetToken(wxid);
	bf.SetToken(msg);
	bf.SetInt(type);

	ServerSendPack(bf.GetAll());
	}
}

