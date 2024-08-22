#pragma once
#include "stdafx.h"
const int login_state_logining=0;
const int login_state_veriy=1;
const int  login_state_success=2;
const int  TYPE_BYTE=0;
const int  TYPE_DOUBLE=5;
const int  TYPE_FLOAT=4;
const int  TYPE_INT=2;
const int  TYPE_JCE_MAX_STRING_LENGTH=104857600;
const int  TYPE_LIST=9;
const int  TYPE_LONG=3;
const int  TYPE_MAP=8;
const int  TYPE_SHORT=1;
const int  TYPE_SIMPLE_LIST=13;
const int  TYPE_STRING1=6;
const int  TYPE_STRING4=7;
const int  TYPE_STRUCT_BEGIN=10;
const int  TYPE_STRUCT_END=11;
const int  TYPE_ZERO_TAG=12;
const int  Event_Get_Firends=1;
const int  Event_Get_Neighbor=2;
extern void GetArrayByteFromMd5(const TCHAR *szRecValue,byte *instr);
extern void setHex(vector<byte>& m_bin,const char* value);
extern CKeepMap<CString,LPCTSTR,set<CString>,set<CString>> g_wxgroup_setwxid;
extern CKeepMap<CString,LPCTSTR,struct_wxsingleuserinf,struct_wxsingleuserinf> g_wxgroupid_wxid_singleinf;
extern unsigned long long htonll(unsigned long long val);
extern unsigned long long ntohll(unsigned long long val);
//extern CString GetSingleMidParamFromBetweenWebBuf(LPCTSTR buf,LPCTSTR targetstr1,LPCTSTR targetstr2);
extern void SendWeChatMessage(LPCTSTR wxid,LPCTSTR msg,int type);//type=1是文字，=2是图片路径，=3是文件路径
extern SOCKET g_socket;
extern BOOL bExitPostThread;
extern SOCKET g_imgsocket;
extern void ServerSendPack(vectorbyte sendout);
//extern BOOL bNeedResetconnect;
typedef struct qq_info 
{
	void clearAllData()
	{
	Account="";
	QQ=0;
	user.clear();
	caption.clear();
	pass.clear();
	md5.clear();
	md52.clear();
	time.clear();
	key.clear();
	nick="";
	Token002C.clear();
	Token004C.clear();
	Token0058.clear();
	TGTKey.clear();
	shareKey.clear();
	pub_key.clear();
	_ksid.clear();
	randKey.clear();
	mST1Key.clear();
	stweb="";
	skey.clear();
	pskey.clear();
	superkey.clear();
	vkey.clear();
	sid.clear();
	sessionKey.clear();
	loginState=login_state_logining;
	VieryToken1.clear();
	VieryToken2.clear();
	Viery.clear();
	}
    CStringA Account;// qq
    INT64 QQ;// qq 10
    vectorbyte user;//// qq_hex
    vectorbyte caption;//// qq_utf-8
    vectorbyte pass;
    vectorbyte md5;
    vectorbyte md52;
    vectorbyte time;
    vectorbyte key;
    CString nick;
    vectorbyte Token002C;
    vectorbyte Token004C;//// A2
    vectorbyte Token0058;
    vectorbyte TGTKey;
    vectorbyte shareKey;
    vectorbyte pub_key;
    vectorbyte _ksid;
    vectorbyte randKey;
    vectorbyte mST1Key;//// transport秘药
    CStringA stweb;
    vectorbyte skey;
    vectorbyte pskey;//// 01 6C 暂时没返回
    vectorbyte superkey;//// 01 6D 暂时没返回
    vectorbyte vkey;
    vectorbyte sid;
    vectorbyte sessionKey;
    int loginState;// 登录是否验证成功
    vectorbyte VieryToken1;//// 验证码token
    vectorbyte VieryToken2;//// 验证码token
    vectorbyte Viery;//// y验证码

};
typedef struct qq_global 
{
    CStringA imei;
    vectorbyte imei_;
    vectorbyte ver;
    int appId;
    CStringA pc_ver;
    CStringA os_type;
    CStringA os_version;
    int _network_type;
    CStringA _apn;
    CStringA device;
    CStringA _apk_id;
    CStringA _apk_v;
    vectorbyte _apk_sig;
   vectorbyte _phone194sig;
 vectorbyte _phone202sig;
};

//typedef struct pbmsg_retryinf 
//{
//
//INT64 toqq;
//INT64 groupuin;
//CString orgstr;
//vectorbyte bin;
//
//};

typedef struct HeadData 
{
	HeadData()
	{
	type=0;
    tag=0;
	}
    char type;
    int tag;// 1-10

};
typedef struct JceMap 
{
	JceMap()
	{
	key_type=0;
	val_type=0;
	}
    char key_type;
    vectorbyte key;
    char val_type;
    vectorbyte val;

};
typedef struct JceStruct_RequestPacket 
{
    short iversion;
    short cPacketType;
    short iMessageType;
    int iRequestId;
    CStringA sServantName;
    CStringA sFuncName;
    vectorbyte sBuffer;
    int iTimeout;
    vector<JceMap> context;//, JceMap, , "1", 重定义下
    vector<JceMap> status;//, JceMap, , "1", 重定义下
	JceStruct_RequestPacket()
	{
	iversion=0;
	cPacketType=0;
	iMessageType=0;
	iRequestId=0;
	iTimeout=0;
	}
};
typedef struct JceStruct_SvcReqRegister 
{
	JceStruct_SvcReqRegister()
	{
		lUin=0;
		lBid=0;

	cConnType=0;
	iStatus=0;
	bOnlinePush=0;
	bIsOnline=0;
	bIsShowOnline=0;
	bKikPC=0;
	bKikWeak=0;
	timeStamp=0;
	_11=0;
	_12=0;

	_14=0;
	
	_17=0;
	_18=0;
	}
    INT64 lUin;// 0
    INT64 lBid;// 1
    char cConnType;// 2
    CStringA sOther;// 3
    int iStatus;// 4
    char bOnlinePush;// 5
    char bIsOnline;// 6
    char bIsShowOnline;// 7
    char bKikPC;// 8
    char bKikWeak;// 9
    INT64 timeStamp;// 10
    char _11;// 11
    char _12;
    CStringA _13;
    char _14;
    vectorbyte _imei_;
    short _17;
    char _18;
    CStringA _19_device;
    CStringA _20_device;
    CStringA _21_sys_ver;


};
typedef struct JceStruct_FSOLREQ 
{
	JceStruct_FSOLREQ()
	{
	 luin=0;
     _1=0;
     _2=0;
     _3=0;
     _4=0;
     _5=0;
     _6=0;
	}
    INT64 luin;
    short _1;
    short _2;
    short _3;
    short _4;
    short _5;
    short _6;

};
typedef struct JceStruct_FriendListResp 
{
	JceStruct_FriendListResp()
	{
		INT64 uin=0;// 2
	reqtype=0;// , 0
    ifReflush=0;// , 1
   	startIndex=0;// , 3
    getfriendCount=0;// , 4
    totoal_friend_count=0;// , 5
    friend_count=0;// , 6
    vecFriendInfo=0;
    groupid=0;
    ifGetGroupInfo=0;
    groupstartIndex=0;
    getgroupCount=0;
    totoal_group_count=0;
    group_count=0;
    vecGroupInfo=0;
    result=0;
    errorCode=0;
    online_friend_count=0;
    serverTime=0;
    sqqOnLine_count=0;
    
	}
    int reqtype;// , 0
    int ifReflush;// , 1
    INT64 uin;// 2
	int startIndex;// , 3
    int getfriendCount;// , 4
    int totoal_friend_count;// , 5
    int friend_count;// , 6
    int vecFriendInfo;
    int groupid;
    int ifGetGroupInfo;
    int groupstartIndex;
    int getgroupCount;
    int totoal_group_count;
    int group_count;
    int vecGroupInfo;
    int result;
    int errorCode;
    int online_friend_count;
    int serverTime;
    int sqqOnLine_count;
    vectorbyte cache_vecFriendInfo;
    vectorbyte cache_vecGroupInfo;

};
typedef struct JceStruct_FriendInfo 
{
JceStruct_FriendInfo()
{
   friendUin=0;
   groupId=0;
   faceId=0;
   
   sqqtype=0;
   status=0;
   memberLevel=0;
   isMqqOnLine=0;
   sqqOnLineState=0;
   isIphoneOnline=0;
   detalStatusFlag=0;
   sqqOnLineStateV2=0;
   
   isRemark=0;
   
   cSpecialFlag=0;
}
    INT64 friendUin;
    int groupId;
    int faceId;
   
    int sqqtype;
    int status;
    int memberLevel;
    int isMqqOnLine;
    int sqqOnLineState;
    int isIphoneOnline;
    int detalStatusFlag;
    int sqqOnLineStateV2;
    
    int isRemark;
   
    int cSpecialFlag;
    vectorbyte VecIMGroupID;
    vectorbyte VecMSFGroupID;
 CStringA nick;
 CStringA sShowName;
 CString name;
};
typedef struct JceStruct_GroupInfo 
{
	JceStruct_GroupInfo()
	{
	groupId=0;
	friend_count=0;
	online_friend_count=0;
	gcodeId=0;
	sqqOnLine_count=0;
	ninevalue=0;
	}
    INT64 groupId;
    CString groupname;
	CString groupintroduce;
    int friend_count;
    int online_friend_count;
	INT64 ninevalue;
    INT64 gcodeId;
    int sqqOnLine_count;


};
typedef struct JceStruct_GL 
{
	JceStruct_GL()
	{
     _1=0;
   _2=0;
     _3=0;
    _4_rand4=0;
   
  
    _8=0;
	}

    short _1;
    short _2;
    short _3;
    int _4_rand4;
    CStringA _5_objname;
    CStringA _6_servname;
    vectorbyte _7;
    short _8;
    vector<JceMap> _9;
    vector<JceMap>  _10;


};
typedef struct JceStruct_FL 
{
	JceStruct_FL()
	{
	_0_reqtype=0;
    _1_ifReflush=0;
    luin=0;
    _3_startIndex=0;
   _4_getfriendCount=0;
    _5_totoal_friend_count=0;
    _6_friend_count=0;
    _7=0;
    _8=0;
   _9=0;
   _10=0;
  _11=0;
	}
    short _0_reqtype;
    short _1_ifReflush;
    INT64 luin;
    short _3_startIndex;
    short _4_getfriendCount;
    short _5_totoal_friend_count;
    short _6_friend_count;
    short _7;
    short _8;
    short _9;
    short _10;
    short _11;


};
typedef struct JceStruct_ReqHeader 
{
	JceStruct_ReqHeader()
	{
	_0_shVersion=0;
    _1_lMID=0;// qq
    _2_appid=0;
   _3_eBusiType=0;
    _4_eMqqSysType=0;
   _5=0;
    _6=0;
	}
    short _0_shVersion;
    INT64 _1_lMID;// qq
    int _2_appid;
    short _3_eBusiType;
    short _4_eMqqSysType;
    short _5;
    short _6;

};
typedef struct RespEncounterInfo 
{
RespEncounterInfo()
	{
		  lEctID=0;
   iDistance=0;
   lTime=0;
  
   wFace=0;
    cSex=0;
    cAge=0;
   
    cGroupId=0;
   
   nFaceNum=0;
   
    shIntroType=0;
   
   eMerchantType=0;
   iVoteIncrement=0;
    bIsSingle=0;
   iLat=0;
   iLon=0;
   iRank=0;
   lTotalVisitorsNum=0;
    cSpecialFlag=0;
	}
    INT64 lEctID;
    int iDistance;
    int lTime;
    CStringA strDescription;
    short wFace;
    char cSex;
    char cAge;
    CStringA strNick;
    char cGroupId;
    CStringA strPYFaceUrl;
    CStringA strSchoolName;
    CStringA strCompanyName;
    CStringA strPYName;
    int nFaceNum;
    CStringA strCertification;
    short shIntroType;
    vectorbyte vIntroContent;
    vectorbyte vFaceID;
    int eMerchantType;
    int iVoteIncrement;
    char bIsSingle;
    int iLat;
    int iLon;
    int iRank;
    int lTotalVisitorsNum;
    char cSpecialFlag;

};
typedef struct JceStruct_ReqUserInfo 
{
	JceStruct_ReqUserInfo()
	{
	  _0_vCells=0;//, , "4"
   
     _5_eListType=0;
    _6=0;
	}
    short _0_vCells;//, , "4"
    CStringA _3_strAuthName;
    CStringA _4_strAuthPassword;
    short _5_eListType;
    short _6;

};
typedef struct JceStruct_GpsInfo 
{
	JceStruct_GpsInfo()
	{
	lat=0;
    lon=0;
    alt=0;
    eType=0;
    accuracy=0;//, , "1"
	}
    int lat;
    int lon;
    int alt;
    int eType;
    float accuracy;//, , "1"

};
typedef struct JceStruct_GPS 
{
	JceStruct_GPS()
	{
	 _0_time=0;// time
   _1_luin=0;// qq
    _2=0;// 114372142
   _3=0;// 4738116111738333504
   _4=0;// 126427945827296800
  
   _6=0;
	}
    int _0_time;// time
    INT64 _1_luin;// qq
    int _2;// 114372142
    INT64 _3;// 4738116111738333504
    INT64 _4;// 126427945827296800
    CStringA _5;// 湖北省
    int _6;

};
typedef struct 数据类型1 
{
	数据类型1()
	{
	_0=0;
     _1=0;
    _2=0;// 126431811297864200
     _3=0;
     _4=0;
    _5=0;// 1007531172
    _6=0;// 1506
	}
    int _0;
    short _1;
    INT64 _2;// 126431811297864200
    short _3;
    short _4;
    int _5;// 1007531172
    int _6;// 1506

};
typedef struct struct_msg 
{
	struct_msg()
	{
	type=0;
	}
    int type;
    CStringA text;

};

typedef struct struct_group_msg 
{
	struct_group_msg()
	{
	groupUin=0;
    sendUin=0;
	opUin=0;//操作者
	u_5=0;
	time=0;
	rtstatus=99;
   // sendName=0;
   // arr=0;
	}
    INT64 groupUin;
    INT64 sendUin;
	INT64 opUin;
    CString sendName;
	int time;
	int u_5;//消息序号
	int rtstatus;
   // char *arr;
	CString msgvector;
	CString voicemsgvector;
	CString voicesign;
	CString voicefilemd5;

};
extern vectorbyte inflate_compress(vectorbyte bin);
extern vectorbyte inflate_Decompress(vectorbyte bin);
extern vectorbyte inflate_httpDecompress(vectorbyte bin);
extern const bool __stdcall UnPackPbGetMsg(const void * data ,int size,vector<struct_group_msg> * msgGroup,BOOL *bJoinMsg);
extern const bool __stdcall UnPackPbPushGroupMsg(const void * data ,int size,vector<struct_group_msg> * msgGroup);
//extern const bool __stdcall PackPbSend (INT64 toGroupUin,INT64 toRecUin,int sendpos,int randomnum,int tailnum,const WCHAR * msg, byte* &data,int&  len);
extern const bool __stdcall PackPullPbSend (BYTE* &data,int&  len);
extern const bool __stdcall PackBackKickInf2 (INT64 toGroupUin,INT64 toRecUin,int sendpos,BYTE* &data,int&  len);

extern void GetqqlevelFromMap(INT64 finalqqnum,int& qqlevel,int& qqdarenlevel,int &qqlikenum,int timeout,BOOL retryflag);

//extern const char * __stdcall pBUtf8DebugString(const void * data ,int size);