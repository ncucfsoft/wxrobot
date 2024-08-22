#ifndef _CRYPTOTEST_H_  
#define _CRYPTOTEST_H_  
  
#include <string>  
  
using namespace std;  
  
typedef enum {  
    GENERAL = 0,  
    ECB,  
    CBC,  
    CFB,  
    OFB,  
    TRIPLE_ECB,  
    TRIPLE_CBC  
}CRYPTO_MODE;  
  
string DES_Encrypt(const string cleartext, const string key, CRYPTO_MODE mode);  
string DES_Decrypt(const string ciphertext, const string key, CRYPTO_MODE mode);  
  
vectorbyte RC4_Encrypt(const vectorbyte cleartext, const vectorbyte key);  
string RC4_Decrypt(const string ciphertext, const string key);  
extern void SendWeChatMessage(LPCTSTR wxid,LPCTSTR msg,int type);//type=1是文字，=2是图片路径，=3是文件路径
extern void SendWeChatMsgEx(LPCTSTR wxgroupid,LPCTSTR msg,LPCTSTR atwxid,int type);//可发名片 (名片), atwxid等于空, 42)
extern void SendWeChatMsgHttp(LPCTSTR wxid,LPCTSTR msg,int type);
extern void SendpackForAtAllGroupPeoPle(LPCTSTR chatroom,LPCTSTR msg);
extern void SendpackForAnnouncement(LPCTSTR chatroom,LPCTSTR msg);
extern void SendpackForQuitGroup(LPCTSTR chatroom,int type);
extern void SendpackForRecPayment(LPCTSTR transferid,LPCTSTR wxid,int time);
//extern BOOL QueryWxidFromHashValue(INT64 groupid,INT64 wxnum,CString &rtwxid);
extern BOOL QueryWxidFromHashValue(INT64 groupid,INT64 wxnum,CString &rtwxid,CString* rtwxnickname);
extern CCriticalSection lockggroup_realqqsetmap;
extern BOOL g_latestwechatflag;
 extern CKeepMap<INT64,INT64,set<INT64>,set<INT64>> g_group_realqqvectMap;
extern CKeepMap<INT64,INT64,CKeepINT64StrMap*,CKeepINT64StrMap*> GrouprealqqNickmap;
extern CKeepMap<INT64,INT64,CKeepINT64StrMap*,CKeepINT64StrMap*> bfGrouprealqqNickmap;
extern CKeepMap<INT64,INT64,int,int> g_group_membercountmap;
extern void SendpackForGetFriendList();
extern CKeepMap<INT64,INT64,CString,LPCTSTR> g_hash2wxidmap;
extern CKeepMap<INT64,INT64,CKeepWxInfMap*,CKeepWxInfMap*> g_group_wxinfmap;
extern CCriticalSection lockggroupmembercountsource;
extern BOOL IsNewJoinMsg(Wxretdata &tempwxretdata);
extern INT64 GetGroupUidFromUserNameOrNickName(LPCTSTR groupuidstr,int UidOrNickNameOrComBostr);
extern INT64 GetFriendUidFromUserName(INT64 groupid,LPCTSTR str);
extern CEvent getonegroupmemberevent;
extern void SendpackForGetGroupUserAll(LPCTSTR groupid);
extern void SendpackForRoomDeleteUser(LPCTSTR groupid,LPCTSTR wxid);
extern void SendpackForRoomAddUser(LPCTSTR groupid,LPCTSTR wxid,int type);
extern void SendpackForAcceptFriend(LPCTSTR v1,LPCTSTR v2);
extern void SendpackForAddFriendOrAccept(LPCTSTR wxid,LPCTSTR msg,int type,int scene);
extern void SendpackForAddFriend(LPCTSTR wxid,LPCTSTR msg,int type);//type=12等于11，type=14等于群聊
extern INT64 QueryHashValueFromWxid(INT64 groupid,LPCTSTR wxid);
extern void UpdateInfFromWxid(INT64 groupid,const struct_wxsingleuserinf& tempinf);
extern INT64 GetGroupIDFromGroupWxid(LPCTSTR wxid);
extern BOOL m_alreadylogin;
extern CString g_reporturl;
extern UINT ProReportOnline(LPVOID lp);
extern CKeepMap<CString,LPCTSTR,INT64,INT64> g_hashvaluemap;
extern CKeepStrStrMap g_wxgroupusername2nickname;
extern CKeepStrStrMap g_wxgroupnickname2username;
extern CKeepStrStrMap g_wxfriendusername2nickname;
extern CCriticalSection lockgqqnickmapresource;
extern CString g_wxuin;
extern CString m_ownqqnumber;
extern CString g_lastownqq;
extern CString g_selfnickname;
  extern UINT ProCheckMsg(LPVOID p);
  extern BOOL LaunchWindowsWechat();
  extern CString g_wechatpath;
  extern CString g_selfwxid;
  extern CString g_selfwxname;
  extern CString g_selfwxuser;
  extern CString g_selfwxphone;
extern INT64 g_interfaceuin;
extern INT64 StrToHash(LPCTSTR str);
extern BOOL g_readGroupOver;
extern BOOL g_resetmaxlen;
extern BOOL g_readFriendOver;
#endif //_CRYPTOTEST_H_  

