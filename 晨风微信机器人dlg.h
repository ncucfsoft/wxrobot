
// 晨风微信机器人Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "MsgDlg.h"

#include "PureAcceptRequestDlg.h"

#include "IgnoreDlg.h"
#include "ModifyTitle.h"

// C晨风微信机器人Dlg 对话框
class CDownloadSkinDlg;
class CMsgDlg;

class CPureAcceptRequestDlg;

class CIgnoreDlg;
class CModifyTitle;

class C晨风微信机器人Dlg : public CFreeWareDlg
{
// 构造
public:
	C晨风微信机器人Dlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_Main };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	CButtonST		m_restorebutton;
	CButtonST		m_modifybutton;
	CButtonST		m_loginqqbt;
	CImage m_bkgimage;
	CShadeButtonST		m_basicbutton[3];
	CString m_addtitle;
	CString g_replaceskintitle;
	CString g_openqqlisttext;
	CString g_loginshowtext;

	HBITMAP m_bkghBitmap;
	DWORD m_lastnewmsgtime;
	CString m_lasttenmsg[100];
	BOOL init_hide;
	HICON m_hIcon;
	void ontuopan();
void RestoreWindow();
void VisitQSpace();
BOOL IsBelongLastTenMsg(const qqretdata& myqqretdata);
BOOL IsBelongLastTenWxMsg(const Wxretdata& mywxretdata);
BOOL CheckqqAccount();
BOOL NoNeedTypeVerifycode();
CString m_ptvfsessionstr;
void executecmdfun(LPCTSTR cmdstr);
void GetCaptchaImg(LPCTSTR url);
//void DeleteWholedir(LPCTSTR dirstr);
	virtual BOOL OnInitDialog();
	void ReleaseAllFile();
	void PopUpRobotAd();
	void UpdateSkinModeMenu();
	void EasyExit();
	//void loglasterror(LPCTSTR saveerror);
	void Getnfo(DWORD dwProcID);
	BOOL Quitqq();
	BOOL Change_status();
	//BOOL CheckExpire();
	//void AppendPlugsMenu();
	void LoadOrSaveWXImgId(BOOL bLoad);
	void LoadPassword(LPCTSTR curqqnumber,CString& curpassword);
	void SavePassword(LPCTSTR curqqnumber,LPCTSTR password,BOOL bSaveSession=true);
BOOL LoadUserSetting(BOOL bLoad);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
afx_msg void OnDestroy();
void UpdateLastInf();

void OnMyOK();
void OnMyOKNotCloseWx();
void LaunchWebWechat();
int offlineproblemcount;
void QuitAndSavehistory(BOOL bJustLookflag=false);
BOOL CheckUpdateAndPrompt();
BOOL CheckUpdateNow();
BOOL TakeEveryMsg(int rtint,const qqretdata& myqqretdata,BOOL bVoice=false);
BOOL TakeEverywxMsg(int rtint,Wxretdata& mywxretdata,BOOL bVoice=false);
void NeedReLogin(LPCTSTR msgtips);
void StopPostMsgshare();
	afx_msg void OnPaint();
	void LoadPriviliegeFile(int mode=0);
	afx_msg HCURSOR OnQueryDragIcon();
	
	void SetButtonBackGrounds(CDC *pDC);
	void LoadBkImage();
	void rerangeButton();
	void reloadinterfaceandlayout();
	BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	void ButtonConfig(CWnd *mywnd, BOOL bShowTextflag, BOOL bTransparrent, const CString& frontstr, int cid);
	// 实现
	LRESULT onSTOPMSG(WPARAM wParam,LPARAM lParam);
	LRESULT onShowTask(WPARAM wParam,LPARAM lParam);
	LRESULT onSWITCHqq(WPARAM wParam,LPARAM lParam);
	LRESULT onTaskList(WPARAM wParam,LPARAM lParam);
	LRESULT onHideList(WPARAM wParam,LPARAM lParam);
	void HotkeyCheck();
	int Getg_tkfromskey(const CString& skey);
	//void GetSkey(CString &skey,CString &ptwebqqhash);
	BOOL Loginqq();
	void AfterLoginOK();
	//void SaveLastCookies(const CString& g_wxuin);
	void LoadLastCookies(const CString& g_wxuin);
	BOOL RetrySdkLogin();
	DECLARE_MESSAGE_MAP()
public:
	void C晨风微信机器人Dlg::Saveinvitelog();
	afx_msg void OnTimer(UINT nIDEvent);
	void TakeTimeTask();
	BOOL m_timetask_opened;
	BOOL m_last_timetask_hour;
	BOOL m_last_timetask_day;
	BOOL m_divinetimetask_opened;
	BOOL m_notcanceltop;
	void TimeSend();
protected:
	void ReShowAllwindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	LRESULT onShowtext(WPARAM wParam,LPARAM lParam);
	LRESULT onShowBaiduVoiceToken(WPARAM wParam,LPARAM lParam);
	LRESULT onNewMsg(WPARAM wParam,LPARAM lParam);
	LRESULT onLoadDllPath(WPARAM wParam,LPARAM lParam);
	LRESULT onTimeNotice(WPARAM wParam,LPARAM lParam);
	LRESULT onGetAllGroupOver(WPARAM wParam,LPARAM lParam);
	LRESULT onGetAllFriendOver(WPARAM wParam,LPARAM lParam);
		void AutoOpenAllGroupMsgDlg();
	LRESULT onRelogin(WPARAM wParam,LPARAM lParam);
LRESULT OnCreateqqdlg(WPARAM wParam,LPARAM lParam);
LRESULT OnCreateqqdlgbyqq(WPARAM wParam,LPARAM lParam);
LRESULT OnUpdaterankfile(WPARAM wParam,LPARAM lParam);

LRESULT onReloadplug(WPARAM wParam,LPARAM lParam);
LRESULT onReStart(WPARAM wParam,LPARAM lParam);
LRESULT onExitMyself(WPARAM wParam,LPARAM lParam);
LRESULT onShowMyself(WPARAM wParam,LPARAM lParam);
LRESULT onHideMyself(WPARAM wParam,LPARAM lParam);
LRESULT onExternResponse(WPARAM wParam,LPARAM lParam);
LRESULT onSignedIn(WPARAM wParam,LPARAM lParam);
LRESULT onOnline(WPARAM wParam,LPARAM lParam);
LRESULT onLoginNextqq(WPARAM wParam,LPARAM lParam);
LRESULT OnLoadPriviliegeFile(WPARAM wParam,LPARAM lParam);
LRESULT OnLoadDefGame(WPARAM wParam,LPARAM lParam);
LRESULT onReFreshList(WPARAM wParam,LPARAM lParam);

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DWORD recmsgtime;
	afx_msg void OnBnClickedOver();
		afx_msg void OnBnClickedButton2();
		void StartingMsg();
		void SuspendMsg();
		void SwitchAccount();
		void LeaveMessage();
	void RemindClick();
	CIgnoreDlg *m_ignoredlg;
	void OnDelIgnoreDlg();
	CModifyTitle *m_replyconfigdlg;
	void SetReplyConfig();
	void OnDelReplyconfigDlg();
	void OnDropFiles(HDROP hDropInfo);
BOOL CreateCimage(LPCTSTR pString,int xsize,int ysize);
void LoadRdbResourceFile(LPCTSTR resdefrdbpath);

BOOL SelectApicFile(LPTSTR picpath);
void StartPostMsg();
void StartVisitOnce();
void StopPostMsg();
afx_msg void OnBnClickedButton1();
void CleanAndClear();
afx_msg void OnCbnKillfocusCombo4();
CComboBox m_combox_ownqq;
afx_msg void OnBnClickedButton4();
afx_msg void OnBnClickedButton3();
//void GetKeepMapFromFile(CKeepMap<CString,LPCTSTR,CString,LPCTSTR> &g_othermap,LPCTSTR filepath);
afx_msg void OnCbnSelchangeCombo4();
afx_msg void OnBnClickedCheck2();

afx_msg void OnEnKillfocusEdit1();
afx_msg void OnBnClickedCheck5();
afx_msg void OnBnClickedCheck6();
afx_msg void OnCbnSetfocusCombo4();
afx_msg void OnEnKillfocusEdit5();
afx_msg void OnBnClickedButton5();
afx_msg void OnCbnSelchangeCombo5();
afx_msg void On32877();
afx_msg void On32891();
afx_msg void On32892();
afx_msg void On32878();
afx_msg void On32879();
afx_msg void On32881();
afx_msg void On32888();
afx_msg void On32889();
afx_msg void On32890();
afx_msg void On32894();
afx_msg void On32895();
afx_msg void On32909();
afx_msg void On32910();
afx_msg void On32896();
afx_msg void On32882();
afx_msg void On32883();
afx_msg void On32884();
afx_msg void On32885();
afx_msg void On32906();
afx_msg void On32902();
afx_msg void On32907();
afx_msg void On32903();
afx_msg void On32904();
afx_msg void On32886();
afx_msg void On32887();
afx_msg void On32908();
afx_msg void On32893();
afx_msg void On32897();
afx_msg void On32898();
afx_msg void On32899();
afx_msg void On32900();
afx_msg void On32901();
afx_msg void On32905();

void EncodeGameFile(LPCTSTR uploadfilename, CStringvect& setenvevent, BOOL bBatchflag=false);

afx_msg LRESULT OnNcHitTest(CPoint point);
afx_msg void OnBnClickedCheck3();
void NavigateOnce();
afx_msg void On32911();
afx_msg void On32912();
afx_msg void On32913();
afx_msg void On32916();

afx_msg void On32914();
afx_msg void On32915();

};
