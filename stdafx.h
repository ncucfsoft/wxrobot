
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif
//#define CuteMode
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif	
#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����
#include <afxhtml.h>                    // MFC HTML ��ͼ֧��




#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <vector>
#include <set>
#include <map>
#include <list>
#include <shlwapi.h>
#include "algorithm"
using namespace std;
//#include "zlib.h"
#include "function.h"
#include "KeepMap.h"
#include "���ܽ��ܿ�.h"
#include "MySortListCtrl.h"
#include "MyBGCDlg.h"
#include "FreeWareDlg.h"
#include "ReadAUStdioFile.h"
#include "RegMySoft.h"
#include <stdio.h>
//using namespace Gdiplus;
extern bool Global_bNotSimCh;
#include <afxinet.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/fstream.hpp>
//#include <boost/bind.hpp>
#include <boost/regex.hpp>
//#pragma comment( lib,"libeay32.lib" )
//#pragma comment( lib,"ssleay32.lib" )

#include <afxdhtml.h>
#include "publichttp.h"
#include "cJSON.h"
#include "BASE64_API.h"
#include "WeChatAPI.h"
#ifdef _DEBUG           
//#pragma comment( lib,"zlibD.lib")
#pragma comment( lib,"libcurlD.lib")
#pragma comment( lib,"curlppD.lib")
#pragma comment( lib,"CommonFunLibD.lib")
//#pragma comment( lib,"libprotobufD.lib")
#pragma comment( lib,"C:\\��Ŀ����\\silk-arm-ios-master\\src\\SKP_Silk_FIX_Win32_debug.lib")
#else   
//#pragma comment( lib,"zlib.lib")
#pragma comment( lib,"libcurl.lib")
#pragma comment( lib,"curlpp.lib")
#pragma comment( lib,"CommonFunLib.lib")
//#pragma comment( lib,"libprotobuf.lib")
#pragma comment( lib,"C:\\��Ŀ����\\silk-arm-ios-master\\src\\SKP_Silk_FIX_Win32_mt.lib")
#endif

//#define CHECKWRONG 1



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

const INT64 MAXSCORE=8000000000000000;
extern const CString default_g_checkinformatstr;
extern const CString default_g_equipformatstr;
extern const CString default_g_gamroboformatstr;
extern const CString default_g_storeprice;
extern CString g_groupimgdir;
extern CString g_voicedir;
const int ID_ShowMainWindow=WM_USER+199;
const int  WM_SHOWTASK=WM_USER+200;
const int  ID_AutoRun=WM_USER+201;
const int WM_SHOWTASKLIST=WM_USER+243;
const int WM_SHOWHIDELIST=WM_USER+244;
const int WM_DownloadZip=WM_USER+245;
const int WM_SWITCHqq=WM_USER+246;
const int WM_NEWMSG=WM_USER+248;
const int ID_MYOK=WM_USER+249;
const int WM_ANSWER=WM_USER+250;
const int WM_TIMENOTICE=WM_USER+251;
const int WM_PLUGRESPONSE=WM_USER+252;
const int WM_THREADDONE=WM_USER+253;
const int WM_GETALLGROUPOVER=WM_USER+254;
const int WM_RELOGIN=WM_USER+255;
const int WM_GETALLFRIENDOVER=WM_USER+256;
const int WM_RESTART=WM_USER+257;
const int WM_LoginNextqq=WM_USER+258;
const int WM_NEWJOIN=WM_USER+259;
const int WM_NEWQUIT=WM_USER+260;
const int WM_NEWCHANGE=WM_USER+261;
const int WM_REFRESHLIST=WM_USER+262;
const int WM_ONLINE=WM_USER+263;
const int WM_SHOWALLFRIENDOVER=WM_USER+264;
const int WM_NEWUPDATEADMIN=WM_USER+265;
const int WM_ADD2DATA=WM_USER+266;
const int WM_ALREADYWELCOME=WM_USER+267;
const int WM_ReLoadPri=WM_USER+268;
const int WM_ReLoadDefGame=WM_USER+269;
const int WM_UPDATESTORE=WM_USER+270;
const int WM_ACROSSMSG=WM_USER+271;
const int WM_SLEEPPLUGRESPONSE=WM_USER+272;
const int WM_LoadDllPath=WM_USER+273;
const int WM_RELOADPLUG=WM_USER+274;
const int WM_GetBaiduVoiceToken=WM_USER+275;
const int WM_NEWWITHDRAW=WM_USER+276;
const int WM_EXTRESPONSE=WM_USER+277;
const int WM_CREATEqqDLG=WM_USER+278;
const int WM_UPDATERANKFILE=WM_USER+279;
const int WM_CLEARNOTINGROUP=WM_USER+280;
const int WM_NEWWEBRESULT = WM_USER + 281;
const int WM_SIGNEDIN = WM_USER + 282;
const int WM_EXITMYSELF = WM_USER + 283;
const int WM_NEXTRUSH = WM_USER + 284;
const int WM_SHOWNEWMSG = WM_USER + 285;
const int WM_SHOWMYSELF = WM_USER + 286;
const int WM_HIDEMYSELF = WM_USER + 287;
const int WM_CREATEqqDLGBYQQ=WM_USER+288;
//const int WM_UNINSTALL=WM_USER+247;
const int WM_STOPMSG=WM_USER+1000;
 const int ID_Dynamicplugs=WM_USER+800;
 const  int CARDLEN=14;
 typedef vector<byte> vectorbyte;
extern  BOOL Android_Send1Msg(INT64 m_realgroupnumber,INT64 uin,const CString& oldstr,INT64 backupuin=0);
extern void Write2MagnetLog(LPCTSTR str,CReadAUStdioFile* file1,int IsDebug);
extern int getfriendcountonce;
extern CString g_selfusername;
typedef struct _privateimginf
{
	int filesize;
	int width;
	int height;
	int filetype;
	string blocksign;
	CStringA despstr;
	CStringA imgIP;
	int port;
}privateimginf;

typedef struct _daren_rank
{
	int darendays;
	int ranknum;
	_daren_rank()
	{
	ranknum=0;
	darendays=false;
	}
}daren_rank;

typedef struct _UpdateSingleRankfile
{
INT64 groupnum;
INT64 qqnum;
INT64 jointime;
INT64 lastactivetime;
int qqscore;
_UpdateSingleRankfile()
{
	groupnum=0;
	qqnum=0;
	jointime=0;
	lastactivetime=0;
	qqscore=0;
}

}UpdateSingleRankfile;
typedef struct struct_qqnum_msg 
{
	//"age":116,"birthday":"day":25,"month":1,"year":1900,"college":"����ѧ","company":"","constellation":1,"email":"ncucf@hotmail.com","gender":1,"hometown_area":"","hometown_city":"",
	//"hometown_country":"","hometown_state":"","location_area":"","location_city":"�ϲ�","location_country":"�й�","location_state":"����","nick":"&nbsp;����","occupation":0,"personal":"�þ���ѧ˼����װ�Լ��������"] 
struct_qqnum_msg()
	{

	}
CString age;
CString day;
CString month;
CString year;
CString college;
CString company;
CString constellation;
CString email;
CString gender;
CString headimgurl;
CString hometown_area;
CString hometown_city;
CString hometown_country;
CString hometown_state;
CString location_area;
CString location_city;
CString location_country;
CString location_state;
CString nick;
CString occupation;
CString personal;
}; 
 typedef struct _NoticeWithDrawStruct
{

CString content;//���ص�ʱ���ǳ��ص����ݣ�ǩ������ǩ���ĵص㣬�����ϴ��ļ������ļ���
CString posstr;//�����3��
INT64 qqgroupid;
INT64 opqqnum;
INT64 qqnum;
INT64 msgid;

int sign;//�����ǡ�3073����������[4352]��ǩ���ǡ�4096����16�����ϴ��ļ��ǡ�1000�����Լ����õģ�
int shuttime;
int withdrawtime;
int time;
_NoticeWithDrawStruct()
{
qqgroupid=0;
opqqnum=0;
qqnum=0;
sign=0;//�����ǡ�3073����������[4352]��ǩ���ǡ�4096�����ϴ��ļ��ǡ�1000�����Լ����õģ�
shuttime=0;
withdrawtime=0;
msgid=0;
time=0;

}
 bool operator==(const _NoticeWithDrawStruct other1)
       {
            return (other1.content==content &&other1.qqgroupid==qqgroupid && other1.opqqnum==opqqnum && other1.qqnum==qqnum && other1.sign==sign && other1.shuttime==shuttime&& other1.withdrawtime==withdrawtime&& other1.msgid==msgid&& other1.time==time);
       };
}NoticeWithDrawStruct;
typedef struct _NickSex
 {
	CString nickname;
	BOOL bMale;
CString area;
CString oldcardname;
CString nowcardname;
int overdaysjointime;
int overdayslastspeaktime;
int birth;
int age;
int level;
_NickSex()
{
overdaysjointime=0;
overdayslastspeaktime=0;
birth=0;
age=0;
level=0;
bMale=-1;
}
 }NickSex;
 typedef struct group_memberinf 
{
INT64 qq;
int age;
int bMale;
CString nickname;
CString cardname;
INT64 jointime;
INT64 lastactivetime;
int overdaysjointime;
int overdayslastspeaktime;
int qqscore;
group_memberinf()
{
overdaysjointime=0;
overdayslastspeaktime=0;
lastactivetime=0;
jointime=0;
bMale=-1;
age=0;
qq=0;
qqscore=0;
}

};
 typedef struct qqlevel_dareninf 
{
int qqlevel;
int qqdarenlevel;
int qqlikenum;
BOOL bNeedfetchflag;
qqlevel_dareninf()
{
qqlevel=0;
qqdarenlevel=0;
qqlikenum=0;
bNeedfetchflag=true;
}

};
  typedef struct _SingleUpload
{
	
	INT64 groupid;
	CString httpaddress;
	HWND m_hwnd;
}SingleUpload;
 typedef struct _UploadGroupImgStruct
{
	INT64 realqq;
	INT64 realqqgroup;
	BOOL bLocalFileflag;
	int typefilevalue;
	CString httpaddress;
	
}UploadGroupImgStruct;

 typedef struct _UploadGroupwaitImgStruct
{
	INT64 toqq;
	INT64 backupuin;
	INT64 m_realgroupnumber;
	CString defnumstr;
	set<CString> needwaitimgpathset;
}UploadGroupwaitImgStruct;


 typedef struct _GroupCardName
{
	INT64 realqq;
	INT64 realqqgroup;
	TCHAR cardnamebuf[256];
}GroupCardName;
 typedef struct pbmsg_retryinf
 {

	 INT64 toqq;
	 INT64 groupuin;
	 CString orgstr;
	 vectorbyte bin;

 };
 typedef struct _qqretdata
{
	CString polltype;
	  CString Content;
	  CString typestr;//group_request_join;
	  CString file_name;
	  CString mode;
	  CString sendname;
	  int retcode;
        INT64 FromUin;
		INT64 ToUin;
       INT64 GroupCode;
	   INT64 request_uin;
      INT64 ReplyIp;
        INT64 MsgId;
        INT64 MsgId2;
		int lc_id; 
		int InfoSeq;
        int MsgType;
        
		int rtstatus;
		INT64 GroupID;
		INT64 RealUin;
       int service_type;
	   INT64 account;
       INT64 SendUin;
       int Seq;
        int Time;
		CString Timestr;
 _qqretdata()
  {
	  lc_id=0;
	  request_uin=0;
	  GroupID=0;
	  RealUin=0;
	  request_uin=0;
account=0;
rtstatus=99;
  retcode=0;
  FromUin=0;
  ToUin=0;
  GroupCode=0;
  InfoSeq=0;
  MsgId=0;
  MsgId2=0;
  MsgType=0;
  ReplyIp=0;
  SendUin=0;
  Seq=0;
  Time=0;
  }
}qqretdata;
 typedef struct _Wxretdata
 {

	 CString Content;
	 CString MsgId;
	CString FromGroupName;
	CString FromUserName;
	CString ToUserName;
	 int MsgType;
	 int Status;
	 int Imgstatus;
	 int CreateTime;
	 int VoiceLength;
	 int PlayLength;
	 CString FileName;
	 int FileSize;
	 CString MediaId;
	 CString Url;
	 int AppMsgType;
	 int StatusNotifyCode;
	 CString StatusNotifyUserName;
	 int HasProductId;
	 CString Ticket;
	 int ImgHeight;
	 int ImgWidth;
	 int SubMsgType;
	 INT64 NewMsgId;
	 _Wxretdata()
	 {
		 MsgType=0;
		 Status=0;
		 Imgstatus=0;
		 CreateTime=0;
		 VoiceLength=0;
		 PlayLength=0;
		 FileSize=0;
		 AppMsgType=0;
		 StatusNotifyCode=0;
	 }
	 bool operator==(const _Wxretdata other1)
	 {
		
		 return (other1.Content==Content && other1.FromUserName==FromUserName && other1.ToUserName==ToUserName && other1.FromGroupName==FromGroupName&& (((other1.FromUserName==g_selfusername)&&CreateTime-other1.CreateTime<10)||(other1.CreateTime==CreateTime &&other1.NewMsgId==NewMsgId)));
	 };

 }Wxretdata;
 typedef struct struct_wxgroup_msg
 {
	 struct_wxgroup_msg()
	 {
		 picsrc=0;
		 maxlen=0;
	 } 
	 byte* picsrc;
	 UINT maxlen;
	 INT64 sendqq;
	 Wxretdata m_wxretmsg;
	 CString mp3path;
 };

 typedef struct struct_wxnewestgroup_msg
 {
	 struct_wxnewestgroup_msg()
	 {
		 sendqq = 0;
		 groupuin = 0;
		 filesize = 0;
		
	 }
	
	 INT64 groupuin;
	 INT64 sendqq;
	 UINT filesize;
	 CString md5str;	
	 CString defaultmsg; 
	 Wxretdata m_wxretmsg;
 };

 typedef struct struct_wxsingleuserinf
 {
	 struct_wxsingleuserinf()
	 {
		 type=0;
	 } 
	 CString nickname;
	 CString user;
	 CString wxid;
	 CString v1;
	 CString remark;
	 CString HeadImgUrl;
	 int type;
	
 };
 typedef vector<byte> vectorbyte;
typedef const vector<byte> const_vectorbyte;
  typedef CKeepMap<CString,LPCTSTR,CString,LPCTSTR> CKeepStrStrMap;
 typedef CKeepMap<CStringA,LPCSTR,CStringA,LPCSTR> CKeepStrAStrAMap;
  typedef CKeepMap<INT64,INT64,CString,LPCTSTR> CKeepINT64StrMap;
typedef CKeepMap<INT64,INT64,CStringA,LPCSTR> CKeepINT64StrAMap;
 extern BOOL GetUinSingleWholeqqInf(INT64 groupid,INT64 uin,CString& nickname,int& bMale,NickSex* rttempnicksex,int timeout,BOOL bAnyway=false);
extern CKeepStrStrMap g_qqserveripmap;
extern CKeepMap<INT64,INT64,CString,LPCTSTR> g_sourcetofindgroup;
extern INT64 GetGCodeFromGid(INT64 gid);
extern INT64 GetGidFromGCode(INT64 gcode);
extern BOOL RtCStringSendWeb(LPCTSTR realhostname, LPCTSTR processpath, CString* WebrtStr,byte *imagebytebuf, const char* postdata, UINT& ddlen, BOOL bHttps = false, LPCTSTR additionalhead = NULL, TCHAR* getsetcookies = NULL, int timeout = 0,BOOL bRedirect = true);
extern BOOL RtCStringSendWebForIE(LPCTSTR realhostname, LPCTSTR processpath, CString* WebrtStr,byte *imagebytebuf, const char* postdata, UINT& ddlen, BOOL bHttps = false, LPCTSTR additionalhead = NULL, TCHAR* getsetcookies = NULL, int timeout = 0,BOOL bRedirect = true);
extern const_vectorbyte char2vector(const char* str,int len);
extern CStringA vector2char(const_vectorbyte orgstr);
extern vectorbyte setHex(const char* value);
typedef CKeepMap<INT64,INT64,struct_wxsingleuserinf,struct_wxsingleuserinf> CKeepWxInfMap;