// IgnoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "晨风微信机器人.h"
#include "IgnoreDlg.h"
#include "AdminList.h"

// CIgnoreDlg 对话框

IMPLEMENT_DYNAMIC(CIgnoreDlg, CDialog)

const TCHAR cneng_Ignore_folder[2][256]={_T("群名称（备注名称）"),_T("群名称（备注名称）")};
const TCHAR cneng_Ignore_state[2][256]={_T("状态"),_T("状态")};
const TCHAR cneng_overtime[2][256]={_T("到期年月日(为空则永不过期)"),_T("到期年月日(为空则永不过期)")};
void LoadIgnoreFile(BOOL bLoad);
int grouptimegap=2;
extern void SendpackForQuitGroup(LPCTSTR chatroom,int type);
INT64 AddValidTime(INT64 daystime,INT64 oldtime)
{
	SYSTEMTIME curSystemTime;
			GetLocalTime(&curSystemTime);
			INT64 t1=ConvertLocalTimeToInt(curSystemTime);		
			if(oldtime>t1)
		oldtime+=daystime*60*60*24;
			else
		oldtime=daystime*60*60*24+t1;
			return oldtime;
}
void GetValidTime(INT64 nowtime,CString& nowtimestr)
{
	if(!nowtime)
	{
	nowtimestr=_T("永久");
	}
	else
	{
SYSTEMTIME tempcurtime=TimetToLocalTime(nowtime);
		
		nowtimestr.Format(_T("%d年%d月%d日"),tempcurtime.wYear,tempcurtime.wMonth,tempcurtime.wDay);
		}
}
CIgnoreDlg::CIgnoreDlg(CWnd* pParent /*=NULL*/)
	: CMyBGCDlg(CIgnoreDlg::IDD, pParent)
{
	m_parentwnd=(C晨风微信机器人Dlg*)pParent;
	bNomultLanguage=true;
		wndIdentification=_T("IgnoreFoloderDlg");
		//m_clrBkgnd=RGB(128,170,40);
		m_clrBkgnd=RGB(180,172,160);
		m_lastrow=-1;
		m_lastremarkrow=-1;
	m_lastsubcolumn=-1;
m_lastremarksubcolumn=-1;
lastfindpos=0;
}
void CIgnoreDlg::SaveEditValue()
{
	CString cstrItemTextEdit;
	GetDlgItemText(IDC_EDIT2,cstrItemTextEdit);
	 m_list1.SetItemText(m_lastrow,m_lastsubcolumn,cstrItemTextEdit); 
	 m_list1.Invalidate();
	 CString filenamebuf2=m_list1.GetItemText(m_lastrow,0);  
      //  m_list1.SetWindowText(L"");  
        m_SubItemEdit.ShowWindow(SW_HIDE); 
	
		INT64 nowtime;
		ConvertAndCompareTime(cstrItemTextEdit,nowtime);

		//filenamebuf2 = filenamebuf2;// Getfinalmergegroupname(filenamebuf2);
		 if(g_bywindowswechatflag)
			 filenamebuf2= Getfinalmergegroupname(filenamebuf2);//filenamebuf2;// 
		 else
			 filenamebuf2=filenamebuf2;

		lockdislikevectres.Lock();
  dislkbackbufvect[filenamebuf2]=nowtime;
  lockdislikevectres.Unlock();
  IsIgnoreOrShutupGroupForName(filenamebuf2);

   m_list1.Invalidate();  
}
void CIgnoreDlg::SaveRemarkEditValue()
{
	CString cstrItemTextEdit;
	GetDlgItemText(IDC_EDIT5,cstrItemTextEdit);
	 m_list2.SetItemText(m_lastremarkrow,m_lastremarksubcolumn,cstrItemTextEdit); 
	 m_list2.Invalidate();
	 CString filenamebuf2=m_list2.GetItemText(m_lastremarkrow,0);  
      //  m_list1.SetWindowText(L"");  

		m_remarkSubItemEdit.ShowWindow(SW_HIDE);
	

		LockGroupName *pqqgroup=new LockGroupName;
		pqqgroup->realqqgroup=_ttoi64(filenamebuf2);
		pqqgroup->lockname=cstrItemTextEdit;
		AfxBeginThread(ProSetFriendRemarkName,(LPVOID)pqqgroup);
		CString uid=GetFriendUsernameFromHashvalue(0,pqqgroup->realqqgroup);
		g_wxfriendusernamewaitchange2remark.RemoveKey(uid);
		//在这里修改备注

   m_list2.Invalidate();  
}
BOOL CIgnoreDlg::OnInitDialog()
{
	CMyBGCDlg::OnInitDialog();
	
	m_SubItemEdit.ShowWindow(SW_HIDE);
	m_remarkSubItemEdit.ShowWindow(SW_HIDE);
	CStringvect m_ziduan;
m_ziduan.push_back(_T("群名称（机器人在此群的备注名称）"));//cneng_Ignore_folder[Global_bNotSimCh]
	m_ziduan.push_back(cneng_Ignore_state[Global_bNotSimCh]);
m_ziduan.push_back(cneng_overtime[Global_bNotSimCh]);
m_ziduan.push_back(_T("群号"));
m_ziduan.push_back(_T("当前群名称"));

	if(g_bIgnoreAll)
		SetDlgItemText(HIDE_BUTTON10,_T("开启/取消开启选中的群"));
	else
		SetDlgItemText(HIDE_BUTTON10,_T("屏蔽/解除屏蔽选中的群"));
	int lenauto;
	intvect m_ziduanlen;
	m_ziduanlen.resize(m_ziduan.size());
	m_ziduanlen[0]=220;
	m_ziduanlen[1]=80;
	m_ziduanlen[2]=180;
	m_ziduanlen[3]=90;
	m_ziduanlen[4]=120;

		if(m_list1.GetSafeHwnd())
			for(int i=0;i<m_ziduan.size();i++)
			{
			lenauto=70;//max(1,m_ziduan[i].GetLength()/4.0)*70;
	
				
					m_list1.InsertColumn(i,m_ziduan[i],0,max(m_ziduanlen[i],lenauto));
			

			}

m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_list2.InsertColumn(0,_T("微信号码"),0,150);

				m_list2.InsertColumn(1,_T("昵称"),0,250);
m_list2.InsertColumn(2,_T("备注名称"),0,250);
SetDlgItemText(IDC_EDIT1,g_hellowstr);	
int itemcount;
		CString struin;

	onFriendOver(0,0);
	if(m_list2.GetItemCount())
	{
	SetDlgItemText(IDC_ReadingFriend,_T("好友列表获取完毕！"));
	}

	LoadIgnoreFile(true);

	UpdateList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void SaveFriendAndGroupNamelist()
{
	CReadAUStdioFile m_file1(true);
	CString Ignorefilename;
	CString tempstr;
	tempstr.Format(_T("(%s)微信好友列表.ini"),m_ownqqnumber);
	Ignorefilename=SaveAs_complete_path(tempstr);

	m_file1.SetUnicodeFile(true);

	int wxncount=0;
	lockfriendusername2nickres.Lock();
	wxncount=g_wxfriendusername2nickname.GetCount();
	lockfriendusername2nickres.Unlock();
	if(wxncount>0)
		if(m_file1.Open(Ignorefilename,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{

		CString totalwritestr;

		if(g_bywindowswechatflag)
		{
			tempstr.Format(_T("微信号\t\t微信昵称\t\t微信ID"));

		}
		else
		{
			tempstr.Format(_T("微信号\t\t微信昵称\t\t头像id\t\t备注名称"));
		}
		totalwritestr+=tempstr;
		totalwritestr+=_T("\r\n");

		//m_file1.WriteLineStr(tempstr);
		if(g_bywindowswechatflag)
		{
			tempstr.Format(_T("%lld\t\t%s\t\t%s"),g_interfaceuin,g_selfnickname,g_selfwxid);

		}
		else
		{
			tempstr.Format(_T("%lld\t\t%s\t\t0\t\t"),g_interfaceuin,g_selfnickname);

		}
		totalwritestr+=tempstr;
		totalwritestr+=_T("\r\n");
		//m_file1.WriteLineStr(tempstr);

		//m_file1.Flush();

		for(UINT i=0;i<wxncount;i++)
		{

			INT64 qqnum=0;
			int tempseq=0;
			lockfriendusername2nickres.Lock();
			g_wxfrienduserwxheadseq.Lookup(g_wxfriendusername2nickname.m_mapstr[i],tempseq);


			qqnum=GetFriendUidFromUserName(0,g_wxfriendusername2nickname.m_mapstr[i]);
			if(g_bywindowswechatflag)
			{
				tempstr.Format(_T("%lld\t\t%s\t\t%s"),qqnum,g_wxfriendusername2nickname[i],g_wxfriendusername2nickname.m_mapstr[i]);
			}
			else
			{
				tempstr.Format(_T("%lld\t\t%s\t\t%d\t\t%s"),qqnum,g_wxfriendusername2nickname[i],tempseq,GetLatestNSCFromHashvalue(0,qqnum));
			}
			lockfriendusername2nickres.Unlock();


				lockfriendresource.Lock();

FriendUinNickNameMap[qqnum]=g_wxfriendusername2nickname[i];
	lockfriendresource.Unlock();


			totalwritestr+=tempstr;
			totalwritestr+=_T("\r\n");
			//m_file1.WriteLineStr(tempstr);
			//m_file1.Flush();


		}
		m_file1.WriteString(totalwritestr);
		totalwritestr=_T("");
		int numbergroup = 0;
		if(g_bywindowswechatflag)
		{
			lockgroupnameresource.Lock();
			numbergroup= GidNickNamemap.GetCount();
			if(GidNickNamemap.GetCount()>0)
			{
				tempstr.Format(_T("-------------------------------------------"));
				totalwritestr+=tempstr;
				totalwritestr+=_T("\r\n");
				//m_file1.WriteLineStr(tempstr);
				tempstr.Format(_T("微信群号\t\t群名称"));
				totalwritestr+=tempstr;
				totalwritestr+=_T("\r\n");
				//m_file1.WriteLineStr(tempstr);

			}
			for(auto it = GidNickNamemap.m_mapstr.begin(); it!=GidNickNamemap.m_mapstr.end(); it++)
			{

				if(!StrStr(GidNickNamemap[*it],_T("@chatroom")))
				{

					tempstr.Format(_T("%lld\t\t%s"),*it,GidNickNamemap[*it]);


					totalwritestr+=tempstr;
					totalwritestr+=_T("\r\n");
					//m_file1.WriteLineStr(tempstr);
					//m_file1.Flush();
				}
			}

			lockgroupnameresource.Unlock();

		/*	if (g_interfaceuin&&!numbergroup)
			{
				MessageBox(mainhwnd, _T("群列表为空，请确认你是通过机器人启动的微信客户端，而不是自己手动打开的微信客户端。"), _T("请确认你是通过机器人软件自动打开的微信客户端"), 0);
			
			}*/
		}
		m_file1.WriteString(totalwritestr);

		m_file1.Close();


		}
}
extern BOOL QueryWxidFromHashValue(INT64 groupid,INT64 wxnum,CString &rtwxid,CString* rtwxnickname);
LRESULT CIgnoreDlg::onFriendOver(WPARAM wParam,LPARAM lParam)
{
	int itemcount;
	CString struin;
	m_list2.DeleteAllItems();

	SaveFriendAndGroupNamelist();

	int wxfncount=0;
lockfriendusername2nickres.Lock();
wxfncount=g_wxfriendusername2nickname.GetCount();
lockfriendusername2nickres.Unlock();



		lockfriendresource.Lock();

		for(UINT i = 0;i<wxfncount;i++)
		{
			itemcount=m_list2.GetItemCount();
			INT64 qqnum;
			lockfriendusername2nickres.Lock();

			if(g_bywindowswechatflag)
			{
				qqnum=GetFriendUidFromUserName(0,g_wxfriendusername2nickname.m_mapstr[i]);
			}
			else
				qqnum=GetFriendUidFromUserName(0,g_wxfriendusername2nickname.m_mapstr[i]);
			lockfriendusername2nickres.Unlock();
			struin.Format(_T("%lld"),qqnum);
			m_list2.InsertItem(itemcount,struin);
			lockfriendusername2nickres.Lock();
			m_list2.SetItemText(itemcount,1,g_wxfriendusername2nickname[i]);

			CString nsckeystr;
			if(g_bywindowswechatflag)
			{
				CString rtwxid;
				QueryWxidFromHashValue(0,qqnum,rtwxid,&nsckeystr);
				if(nsckeystr==g_wxfriendusername2nickname[i])
					nsckeystr=_T("");
			}
			else
			{
				g_wxfriendusernamewxnsc.Lookup(g_wxfriendusername2nickname.m_mapstr[i],nsckeystr);
			}


			m_list2.SetItemText(itemcount,2,nsckeystr);

			lockfriendusername2nickres.Unlock();
		}
		m_list2.Invalidate();
			lastfindpos=m_list2.GetItemCount();
		if(lParam==1)
		SetDlgItemText(IDC_ReadingFriend,_T("好友列表获取完毕！"));
		return 0;
}
CIgnoreDlg::~CIgnoreDlg()
{
}

void CIgnoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, HIDE_LIST1, m_list1);
	DDX_Control(pDX, IDC_LIST1, m_list2);
	DDX_Control(pDX, IDC_EDIT2, m_SubItemEdit);
	DDX_Control(pDX,IDC_EDIT5,m_remarkSubItemEdit);
}
void CIgnoreDlg::OnDestroy()
{
	LoadIgnoreFile(false);
	
	CDialog::OnDestroy();


	// TODO: 在此处添加消息处理程序代码
}
void CIgnoreDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR *szFileName = new TCHAR[255];
int FileNum = DragQueryFile(hDropInfo,0,szFileName,255);//得到拖动文件个数
//for ( int i =0 ; i < FileNum ; i++ )
{
   DragQueryFile(hDropInfo,0,szFileName,255); //szFileName第i个文件名
    //处理文件

}


	//MessageBox(szFileName);
	delete []szFileName;
	::DragFinish(hDropInfo);


	CDialog::OnDropFiles(hDropInfo);
}
BEGIN_MESSAGE_MAP(CIgnoreDlg, CDialog)
	ON_WM_CTLCOLOR()
		ON_WM_ERASEBKGND()
		ON_WM_DESTROY()
		ON_WM_DROPFILES()
ON_BN_CLICKED(HIDE_BUTTON7, &CIgnoreDlg::OnBnClickedButton7)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CIgnoreDlg::OnNMDblclkList2)
	ON_NOTIFY(NM_DBLCLK, HIDE_LIST1, &CIgnoreDlg::OnNMDblclkList1)
	ON_NOTIFY(LVN_ITEMCHANGED, HIDE_LIST1, &CIgnoreDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(HIDE_BUTTON10, &CIgnoreDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON1, &CIgnoreDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CIgnoreDlg::OnBnClickedButton2)
	ON_BN_CLICKED(HIDE_BUTTON11, &CIgnoreDlg::OnBnClickedButtonHide11)
	ON_BN_CLICKED(IDC_BUTTON11, &CIgnoreDlg::OnBnClickedButton11)
	ON_BN_CLICKED(HIDE_BUTTON12, &CIgnoreDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_CHECK2, &CIgnoreDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CIgnoreDlg::OnBnClickedCheck1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CIgnoreDlg::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CIgnoreDlg::OnEnKillfocusEdit2)
	ON_MESSAGE(WM_SHOWALLFRIENDOVER,onFriendOver)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CIgnoreDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CIgnoreDlg::OnBnClickedButton6)
	ON_BN_CLICKED(HIDE_BUTTON13, &CIgnoreDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON8,&CIgnoreDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9,&CIgnoreDlg::OnBnClickedButton9)
	ON_EN_KILLFOCUS(IDC_EDIT5,&CIgnoreDlg::OnEnKillfocusEdit5)
	ON_BN_CLICKED(IDC_BUTTON22,&CIgnoreDlg::OnBnClickedButton22)
END_MESSAGE_MAP()


// CIgnoreDlg 消息处理程序
BOOL IsSameGroupqqNum(LPCTSTR str1,LPCTSTR str2)
{
	if(lstrcmp(str1,str2)==0)
		return true;
CString expressstr;
	 	expressstr.Format(_T("(?<=\\()\\d+(?=\\))"));
	CString rtmatchstr;
	INT64 groupnum1=0;
	INT64 groupnum2=0;


BOOL	bGetmatch=GetFirstRegMatch(str1,expressstr,rtmatchstr,false);
if(bGetmatch)
groupnum1=_ttoi64(rtmatchstr);
else
{
	expressstr.Format(_T("\\d{5,10}"));
	


bGetmatch=GetFirstRegMatch(str1 ,expressstr,rtmatchstr,false);
if(bGetmatch)
groupnum1=_ttoi64(rtmatchstr);

}
bGetmatch=GetFirstRegMatch(str2,expressstr,rtmatchstr,false);
if(bGetmatch)
groupnum2=_ttoi64(rtmatchstr);
else
{
expressstr.Format(_T("\\d{5,10}"));
	


bGetmatch=GetFirstRegMatch(str2,expressstr,rtmatchstr,false);
if(bGetmatch)
groupnum2=_ttoi64(rtmatchstr);

}

        return (groupnum1==groupnum2)&&groupnum1;
 


}
void CIgnoreDlg::NormalSomeItem(int item)
{
	TCHAR filenamebuf2[256];
	
setvector<CString>::iterator it;
	if(g_bIgnoreAll)
	{
	
			m_list1.SetItemText(item,1,cneng_Ignore_avoided[g_bIgnoreAll]);
			m_list1.Invalidate();
			m_list1.GetItemText(item,0,filenamebuf2,256);
			CString cstrItemTextEdit=m_list1.GetItemText(item,2);  
	INT64 nowtime;

	CString	finalfilenamebuf2 ;
	//= Getfinalmergegroupname(filenamebuf2);
	 if(g_bywindowswechatflag)
			 finalfilenamebuf2= Getfinalmergegroupname(filenamebuf2);//filenamebuf2;// 
		 else
			 finalfilenamebuf2=filenamebuf2;

			if(ConvertAndCompareTime(cstrItemTextEdit,nowtime))
			{
nowtime=AddValidTime(30,nowtime);
			}
			
				
			
				lockdislikevectres.Lock();
			dislkbackbufvect[finalfilenamebuf2]=nowtime;
			lockdislikevectres.Unlock();
			  IsIgnoreOrShutupGroupForName(finalfilenamebuf2);
			
	}
	else
	{
		m_list1.SetItemText(item,1,cneng_Ignore_normal[g_bIgnoreAll]);
		m_list1.Invalidate();
			m_list1.GetItemText(item,0,filenamebuf2,256);
			CString rmit;
			lockdislikevectres.Lock();
			it=find_if(dislkbackbufvect.m_mapstr.begin(),dislkbackbufvect.m_mapstr.end(),SameqqGroupNumfun(filenamebuf2));
			if(it!=dislkbackbufvect.m_mapstr.end())
			{
			rmit=*it;
			dislkbackbufvect.RemoveKey(*it);
			  	
			}
			lockdislikevectres.Unlock();
			if(!rmit.IsEmpty())
			IsIgnoreOrShutupGroupForName(rmit);

	}
}
void CIgnoreDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码



	// TODO: 在此添加控件通知处理程序代码  
	NM_LISTVIEW *pNMListCtrl=(NM_LISTVIEW *)pNMHDR;
	if(pNMListCtrl->iItem!=-1)
	{
		if(pNMListCtrl->iSubItem!=2)
		{
			int istat=phdr->iItem;
			if(istat!=-1)
			{
				OpenFriendMsgDlg(istat);
			}
		
		}
		else 
		{
			CRect rect,dlgRect;
			//获得当前列的宽度以适应如果用户调整宽度  
			//此处不用获得的子项rect矩形框来设置宽度  
			//是因第一列时返回的宽度是整行的宽度，我也不知道为啥  
			int width=m_list2.GetColumnWidth(pNMListCtrl->iSubItem);
			m_list2.GetSubItemRect(pNMListCtrl->iItem,pNMListCtrl->iSubItem,LVIR_BOUNDS,rect);
			//保存选择的列表项索引  
			//这个因为我是用了两个列表控件一个CEdit  
			//所以需要保存列表的索引  
			//以及子项相对应的行列号索引  
			m_lastremarkrow=pNMListCtrl->iItem;
			m_lastremarksubcolumn=pNMListCtrl->iSubItem;
			//获得listctrl矩形框  
			//获得父对话框的位置以调整CEdit的显示位置，具体见下面代码  
			m_list2.GetWindowRect(&dlgRect);
			//调整与父窗口对应  
			ScreenToClient(&dlgRect);
			int height=rect.Height();
			rect.top+=(dlgRect.top+1);
			rect.left+=(dlgRect.left+1);
			rect.bottom=rect.top+height+2;
			rect.right=rect.left+width+2;
			m_remarkSubItemEdit.MoveWindow(&rect);
			m_remarkSubItemEdit.ShowWindow(SW_SHOW);
			CString lasttext=m_list2.GetItemText(m_lastremarkrow,m_lastremarksubcolumn);
			m_remarkSubItemEdit.SetWindowText(lasttext);
			m_remarkSubItemEdit.SetFocus();
			//m_SubItemEdit.ShowCaret();
			m_remarkSubItemEdit.SetSel(-1);
			m_remarkSubItemEdit.SetWindowPos((CWnd*)HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			m_list2.SetItemState(m_lastremarkrow,0,-1);
		}
	}
	*pResult=0;
	
}
void CIgnoreDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	
	

	 // TODO: 在此添加控件通知处理程序代码  
    NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;  
    if(pNMListCtrl->iItem != -1)  
    {  
		if(pNMListCtrl->iSubItem!=2)
		{
			int istat=pNMListCtrl->iItem;
			NormalSomeItem(istat);
	OpenGroupMsgDlg(istat);	
		}
		else if(g_bIgnoreAll)
		{
        CRect rect,dlgRect;  
        //获得当前列的宽度以适应如果用户调整宽度  
        //此处不用获得的子项rect矩形框来设置宽度  
        //是因第一列时返回的宽度是整行的宽度，我也不知道为啥  
        int width = m_list1.GetColumnWidth(pNMListCtrl->iSubItem);  
        m_list1.GetSubItemRect(pNMListCtrl->iItem,pNMListCtrl->iSubItem,LVIR_BOUNDS,rect);  
        //保存选择的列表项索引  
        //这个因为我是用了两个列表控件一个CEdit  
        //所以需要保存列表的索引  
        //以及子项相对应的行列号索引  
    m_lastrow=pNMListCtrl->iItem;
	m_lastsubcolumn=pNMListCtrl->iSubItem;
        //获得listctrl矩形框  
        //获得父对话框的位置以调整CEdit的显示位置，具体见下面代码  
        m_list1.GetWindowRect(&dlgRect);  
        //调整与父窗口对应  
        ScreenToClient(&dlgRect);  
        int height = rect.Height();  
        rect.top += (dlgRect.top+1);  
        rect.left += (dlgRect.left+1);  
        rect.bottom = rect.top+height+2;  
        rect.right = rect.left+width+2;  
        m_SubItemEdit.MoveWindow(&rect);  
        m_SubItemEdit.ShowWindow(SW_SHOW);  
		CString lasttext=m_list1.GetItemText(m_lastrow,m_lastsubcolumn);
		m_SubItemEdit.SetWindowText(lasttext);
        m_SubItemEdit.SetFocus();  
		//m_SubItemEdit.ShowCaret();
		m_SubItemEdit.SetSel(-1);
		m_SubItemEdit.SetWindowPos((CWnd*)HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		m_list1.SetItemState(m_lastrow,0,-1);
		}
    }  
	*pResult = 0;
}
void CIgnoreDlg::OnBnClickedCancel()
{
	OnOK();
}
void CIgnoreDlg::Insert2List(LPCTSTR fpath,LPCTSTR hshowstr)
{
	if(g_creategrouponceflag||!StrStr(fpath,_T("@chatroom")))//==过滤那些@chatroom,但如果自己创建了那些临时群的话，就允许显示
	{
		int itemcount=m_list1.GetItemCount();
		m_list1.InsertItem(itemcount,fpath);

		m_list1.SetItemText(itemcount,1,hshowstr);
		CString groupnumstr;
		INT64 tempgroupnum=0;
		if(g_bywindowswechatflag)
		tempgroupnum=ExtractGroupidFromStr(fpath);
		else
		tempgroupnum=GetGroupUidFromUserNameOrNickName(fpath,1);
		groupnumstr.Format(_T("%lld"),tempgroupnum);
		m_list1.SetItemText(itemcount,3,groupnumstr);

		CString tempusername=GetGroupUserNameFromHashvalue(tempgroupnum);
		CString realnickname;
		g_wxrealusername2nickname.Lookup(tempusername,realnickname);
		if(realnickname.IsEmpty())
			realnickname=fpath;
		m_list1.SetItemText(itemcount,4,realnickname);

	}

}
void CIgnoreDlg::ModifyList(LPCTSTR fpath,LPCTSTR hshowstr)
{

		int itemcount=m_list1.GetItemCount();
		CString tempstr;
		CString getvalidtime;
		for(int i=0;i<itemcount;i++)
		{
		tempstr=m_list1.GetItemText(i,0);
		if(IsSameGroupqqNum(tempstr,fpath))//lstrcmpi(tempstr,fpath)==0)
		{
			INT64 tempgroupvalue=0;
			lockdislikevectres.Lock();
			tempgroupvalue=dislkbackbufvect[fpath];
			lockdislikevectres.Unlock();
			if(IsTimeOver(tempgroupvalue))
		m_list1.SetItemText(i,1,cneng_Ignore_overtime[g_bIgnoreAll]);
			else
		m_list1.SetItemText(i,1,cneng_Ignore_avoided[g_bIgnoreAll]);

		if(g_bIgnoreAll&&tempgroupvalue!=0)
		{
		GetValidTime(tempgroupvalue,getvalidtime);

		m_list1.SetItemText(i,2,getvalidtime);
		}
	
		break;
		}

		}

}
void CIgnoreDlg::UpdateList()
{
	m_list1.DeleteAllItems();
	for(int i=0;i<backbufvect.size();i++)
	{
	Insert2List(backbufvect[i],cneng_Ignore_normal[g_bIgnoreAll]);

	}
	lockdislikevectres.Lock();
	for(auto it=dislkbackbufvect.m_mapstr.begin();it!=dislkbackbufvect.m_mapstr.end();++it)
	{
		ModifyList((*it),NULL);
	}
	lockdislikevectres.Unlock();
	//		tasklist.push_back(tempitem);
		//	ModifyList(filepathstr,Ignoreorshowstr);

}
INT64 GetTimeFromStr(LPCTSTR timestr)
{	
	if(lstrlen(timestr)==0)
		return 0;
	SYSTEMTIME tempcurtime;
		memset(&tempcurtime,0,sizeof(SYSTEMTIME));

	CString expressstr;
	 	expressstr.Format(_T("\\d+"));
	CString rtmatchstr;
	INT64 groupnum1=0;
	INT64 groupnum2=0;
	INT64 nowtime=0;
	CStringvect getuinbackstr;
GetRegEX(timestr,expressstr,getuinbackstr);
if(getuinbackstr.size()>=3)
{
	tempcurtime.wYear=_ttoi(getuinbackstr[0]);
	tempcurtime.wMonth=_ttoi(getuinbackstr[1]);
	tempcurtime.wDay=_ttoi(getuinbackstr[2]);
nowtime=ConvertLocalTimeToInt(tempcurtime);

}
else if(getuinbackstr.size()==0)
{
	nowtime=0;
}

return nowtime;
}
BOOL IsTimeOver(INT64 nowtime)
{

	//SYSTEMTIME tempcurtime=TimetToSystemTime(nowtime);
	
if(nowtime==0)
	return false;

SYSTEMTIME curSystemTime;
			GetLocalTime(&curSystemTime);
			INT64 t1=ConvertLocalTimeToInt(curSystemTime);		
	
return t1>nowtime;
		//nowtimestr.Format(_T("%d年%d月%d日"),tempcurtime.wYear,tempcurtime.wMonth,tempcurtime.wDay);

}

BOOL ConvertAndCompareTime(LPCTSTR timestr,INT64& nowtime)
{

	
nowtime=GetTimeFromStr(timestr);	

return IsTimeOver(nowtime);
		//nowtimestr.Format(_T("%d年%d月%d日"),tempcurtime.wYear,tempcurtime.wMonth,tempcurtime.wDay);

}

void LoadIgnoreFile(BOOL bLoad)
{
	
	
		CString tempstr;
		TCHAR encodebuf[1024];
		CReadAUStdioFile m_file1(true);
		CString Ignorefilename;
	//::function::GetLanuageFromCFSoftPath(Ignorefilename,_T("屏蔽群消息列表.txt"),true);
	if(g_bIgnoreAll)
	{
		Ignorefilename=SaveAs_complete_path(_T("手动开启群消息列表.txt"));
		if(!PathFileExists(Ignorefilename))
		Ignorefilename=SaveAs_complete_path(_T("开启群消息列表.txt"));
		else
		{
		g_bManqqgroupListflag=true;
		}
	}
	else
			Ignorefilename=SaveAs_complete_path(_T("屏蔽群消息列表.txt"));
	
		if(bLoad)
{
	lockdislikevectres.Lock();
		dislkbackbufvect.RemoveAll();
		lockdislikevectres.Unlock();
		lockgroupbeignoreresource.Lock();
		g_groupbeIgnoreMap.RemoveAll();
		lockgroupbeignoreresource.Unlock();
		set<INT64> alreadygroupset;
	if(m_file1.Open(Ignorefilename,CFile::modeRead))
	{

	

	CString filepathstr,Ignoreorshowstr;
	CStringvect vectemp;
			//tempstr.Format(_T("%s|%d|%d|%d|%d"),(*it).taskstr,(*it).month,(*it).day,(*it).hour,(*it).minute);
			while(m_file1.ReadString(tempstr))
			{


			CString frontstr,backstr;
				frontstr=tempstr;
				backstr=_T("永久");
		//	::function::getmytoken(tempstr,'|',vectemp);
				::function::GetHalfStr(tempstr,&frontstr,&backstr,'|',true);
				//vectemp.push_back(frontstr);
				//vectemp.push_back(backstr);

			//if(vectemp.size()>=1)
			{
			filepathstr=frontstr;
			//Ignoreorshowstr=vectemp[1];
		BOOL bOvertime=false;
		
		INT64 nowtime=0;				
			//if(vectemp.size()>=2)
			{
			bOvertime=ConvertAndCompareTime(backstr,nowtime);
			}

		if(!bOvertime)
		{

			INT64 groupnumtemp=0;
			if(g_bywindowswechatflag)
				groupnumtemp=ExtractGroupidFromStr(filepathstr);
			else
			{
				CString truename;
				ExtractGroupidFromStr(filepathstr,&truename);
				groupnumtemp=GetGroupUidFromUserNameOrNickName(truename,1);
			}
			lockdislikevectres.Lock();
			dislkbackbufvect[filepathstr]=nowtime;
			lockdislikevectres.Unlock();
			if(alreadygroupset.find(groupnumtemp)==alreadygroupset.end())//微信机器人名字里面不包含群号，所以很可能有多个群名字
			{
				if(g_bywindowswechatflag)
				IsIgnoreOrShutupGroupForName(filepathstr);
				else
			  IsIgnoreOrShutupGroup(groupnumtemp);

			alreadygroupset.insert(groupnumtemp);
			}
		}
			
	//		tasklist.push_back(tempitem);
		//	ModifyList(filepathstr,Ignoreorshowstr);
			}

			}

		
	m_file1.Close();	
	}

}
else
{
	m_file1.SetUnicodeFile(true);
	if(m_file1.Open(Ignorefilename,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyNone))
	{
		m_file1.SetLength(0);

		TCHAR tempbuf1[256];
		TCHAR tempbuf2[256];
		CString overtimestr;
		lockdislikevectres.Lock();
		for(auto it=dislkbackbufvect.m_mapstr.begin();it!=dislkbackbufvect.m_mapstr.end();++it)
		{
			if(g_bIgnoreAll)
				GetValidTime(dislkbackbufvect[*it],overtimestr);
			else
				overtimestr=_T("");

			tempstr.Format(_T("%s|%s"),*it,overtimestr);
			m_file1.WriteLineStr(tempstr);
			m_file1.Flush();

		}
		lockdislikevectres.Unlock();
		m_file1.Close();
	}

	}
}



INT64 GetGidFromGname(LPCTSTR gnamestr)
{
	INT64 rtid=0;
	lockgroupnameresource.Lock();
	for(int i=0;i<GidNickNamemap.GetCount();i++)
	{
	if(GidNickNamemap[(UINT)i]==gnamestr)
	{
		rtid=GidNickNamemap.m_mapstr[i];
	break;
	}
	}
	lockgroupnameresource.Unlock();

	return rtid;
}
void CIgnoreDlg::OpenGroupMsgDlg(int item)
{
	
CString gnamestr;
gnamestr=m_list1.GetItemText(item,0);	
CString nicknamestr=gnamestr;
INT64 tempgid=0;
if(g_bywindowswechatflag)
tempgid=ExtractGroupidFromStr(gnamestr);
else
tempgid=GetGroupUidFromUserNameOrNickName(gnamestr,1);//GetGidFromGname(gnamestr);
INT64 curgcode=0;
		CreateqqMsgDlg(tempgid,0,nicknamestr,true,true,0);

}
void CIgnoreDlg::OpenFriendMsgDlg(int item)
{
	CString nicknamestr;
CString gnamestr;
gnamestr=m_list2.GetItemText(item,0);	
nicknamestr=m_list2.GetItemText(item,1);	
INT64 tempid=_ttoi64(gnamestr);//GetGroupUidFromUserNameOrNickName(nicknamestr,1);//_ttoi64(gnamestr);
INT64 curgcode=0;
//GetthreeNickName(1,0,0,tempid,nicknamestr);
CreateqqMsgDlg(0,tempid,nicknamestr,false,true,curgcode);
}

void CIgnoreDlg::OnBnClickedButton7()
{


	intvect selectvect;
	GetSelectIntvect(m_list1,selectvect);
	if(!selectvect.size())
	{
		AfxMessageBox(_T("请先用鼠标单击选中一个群！"));
		return;
	}
	
	for(int i=0;i<selectvect.size();i++)
	{

		NormalSomeItem(selectvect[i]);

		OpenGroupMsgDlg(selectvect[i]);

	}
	//HideIgnoreWindow();
	//OnCancel();
	
}





BOOL CIgnoreDlg::PreTranslateMessage(MSG* pMsg)
{
	LRESULT presult;
if((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_SPACE)&&pMsg->hwnd==GetDlgItem(HIDE_LIST1)->GetSafeHwnd())
{
	OnBnClickedButton10();
		return true;

}
 else if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )   
    {     
        if( GetFocus()->GetDlgCtrlID() == IDC_EDIT2)  
        {  
          GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
               // m_list1.SetFocus();//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息   
           
        }
	 if( GetFocus()->GetDlgCtrlID() == IDC_EDIT6)  
        {  
		 OnBnClickedButton22();
               // m_list1.SetFocus();//使列表控件获得焦点，则CEdit会自动失去焦点，触发EN_KILLFOCUS消息   
           
        }
        return TRUE;  
    }  
presult=CDialog::PreTranslateMessage(pMsg);

return presult;
}
LRESULT CIgnoreDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{



	return CDialog::WindowProc(message, wParam, lParam);
}


void CIgnoreDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);


	int istat=pNMLV->iItem;
	


	*pResult = 0;
}

void CIgnoreDlg::OnBnClickedButton10()
{
intvect selectvect;
	GetSelectIntvect(m_list1,selectvect);
		TCHAR filenamebuf2[257];
		TCHAR filenamebuf[257];

	setvector<CString>::iterator it;
	for(int i=selectvect.size()-1;i>=0;i--)
	{
			m_list1.GetItemText(selectvect[i],1,filenamebuf,256);
			m_list1.GetItemText(selectvect[i],0,filenamebuf2,256);

			if(lstrcmpi(filenamebuf,cneng_Ignore_normal[g_bIgnoreAll])==0)
			{
			m_list1.SetItemText(selectvect[i],1,cneng_Ignore_avoided[g_bIgnoreAll]);
			m_list1.Invalidate();
		
				
			CString cstrItemTextEdit=m_list1.GetItemText(selectvect[i],2);  
	INT64 nowtime;
	CString	finalfilenamebuf2;//=filenamebuf2;//Getfinalmergegroupname(filenamebuf2);
	 if(g_bywindowswechatflag)
			 finalfilenamebuf2= Getfinalmergegroupname(filenamebuf2);//filenamebuf2;// 
		 else
			 finalfilenamebuf2=filenamebuf2;

			if(ConvertAndCompareTime(cstrItemTextEdit,nowtime))
			{
nowtime=AddValidTime(30,nowtime);
			}
			lockdislikevectres.Lock();
			dislkbackbufvect[finalfilenamebuf2]=nowtime;
			lockdislikevectres.Unlock();
			  IsIgnoreOrShutupGroupForName(finalfilenamebuf2);

			if(g_bIgnoreAll)
			{
				//INT64 GGGID=GetGidFromGname(filenamebuf2);
			//SetGroupPolicy(GGGID,1);//1=接收消息；3=屏蔽消息
			}
			}
			else
			{
			m_list1.SetItemText(selectvect[i],1,cneng_Ignore_normal[g_bIgnoreAll]);
			m_list1.Invalidate();
CString rmit;
			lockdislikevectres.Lock();
			it=find_if(dislkbackbufvect.m_mapstr.begin(),dislkbackbufvect.m_mapstr.end(),SameqqGroupNumfun(filenamebuf2));
			if(it!=dislkbackbufvect.m_mapstr.end())
			{ 
			rmit=*it;
			dislkbackbufvect.RemoveKey(*it);
			  	
			}

			lockdislikevectres.Unlock();
			if(!rmit.IsEmpty())
			IsIgnoreOrShutupGroupForName(rmit);
			
				if(g_bIgnoreAll==0)
			{
				//INT64 GGGID=GetGidFromGname(filenamebuf2);
			//SetGroupPolicy(GGGID,1);//1=接收消息；3=屏蔽消息
			}
			}
		
		
		
	}


}

void CIgnoreDlg::GetSelectIntvect(const CMySortListCtrl& m_list,intvect& selectvect)
{

	for(int i=0;i<m_list.GetItemCount();i++)
	{
		int   nState   =   m_list.GetItemState(i,LVIS_SELECTED);   
		if(nState   !=0)   
		{   
			selectvect.push_back(i);
				
		}  

	}
}
void CIgnoreDlg::OnBnClickedButton1()
{
	intvect selectvect;
	GetSelectIntvect(m_list2,selectvect);
	if(!selectvect.size())
	{
		AfxMessageBox(_T("请先用鼠标单击选中一个好友！"));
		return;
	}
	for(int i=0;i<selectvect.size();i++)
OpenFriendMsgDlg(selectvect[i]);	
	//GetthreeNickName(rtint,myqqretdata.RealUin,groupuin,sendqq,nicknamestr);
		//	CreateqqMsgDlg(groupuin,sendqq,nicknamestr,qunflag);

	// TODO: 在此添加控件通知处理程序代码
}


void CIgnoreDlg::OnBnClickedButton2()
{
	m_parentwnd->OnDelIgnoreDlg();
}

vector<INT64> qgroupuinvect;
UINT proSendQGroupmsg(LPVOID lp)
{
	int rtflagok=0;
	int sumint=qgroupuinvect.size();
	BOOL rtflag;
	for(int i=0;i<sumint;i++)
	{
	rtflag=SendGroupqqMsg(qgroupuinvect[i],(LPCTSTR)lp);
	if(rtflag)
	{
		rtflagok++;
		if(i!=sumint-1)
	Sleep(grouptimegap*1000);
	}
	else
	{
	break;
	}
	}
	CString fmtstr;
	fmtstr.Format(_T("总计划发送%d条群消息，成功发送%d条！成功率为%d%%,如果发送成功率太小，请检查你发送的内容是否被腾讯禁止或者请尝试分批多次发送！"),sumint,rtflagok,rtflagok*100/sumint);
	::MessageBox(0,fmtstr,_T("发送群消息报告"),0);
return 0;
}
void CIgnoreDlg::OnBnClickedButton11()
{
	
	TCHAR buflan[256]=_T("包含昵称和备注名对应关系的文件|*.ini||");
	CFileDialog  filedlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,buflan);
	if(filedlg.DoModal()!=IDOK)
	{
		return;
	}

	CString importfilename=filedlg.GetPathName();

	CReadAUStdioFile filepwd(true);
	CString tempstr;
	CKeepStrStrMap *ptempnickremarkmap=new CKeepStrStrMap;
	if(filepwd.Open(importfilename,CFile::modeRead|CFile::shareDenyNone))
	{
		CStringvect vectemp;
		while(filepwd.ReadString(tempstr))
		{
			::function::getmytoken(tempstr,_T("\t "),vectemp);
			if(vectemp.size()==4)
			{
				(*ptempnickremarkmap)[vectemp[1]]=vectemp[3];
			}
			else if(vectemp.size()==2)
			{
				(*ptempnickremarkmap)[vectemp[0]]=vectemp[1];
			}
		}
		filepwd.Close();
	}
	if((*ptempnickremarkmap).GetCount())
	{

	
		AfxBeginThread(ProBatchSetFriendRemarkName,(LPVOID)ptempnickremarkmap);
	
	}

}


void CIgnoreDlg::OnBnClickedButtonHide11()
{
	
	if(!Isggged)
	{
	AfxMessageBox(_T("未注册版不能给选中的群批量发消息，请点上面的“打开选中的群聊天窗口”按钮，然后手动发消息！"));
	return;
	}
	
	grouptimegap=GetDlgItemInt(IDC_EDIT6);
	if(grouptimegap<2)
		grouptimegap=2;

	intvect selectvect;
	GetSelectIntvect(m_list1,selectvect);
	if(!selectvect.size())
	{
		AfxMessageBox(_T("请先用鼠标单击选中一个群！"));
		return;
	}
	static TCHAR content[1024];
	GetDlgItemText(IDC_EDIT1,content,1024);
	CString gnamestr;
	INT64 tempid;
	qgroupuinvect.clear();
	for(int i=0;i<selectvect.size();i++)
	{
gnamestr=m_list1.GetItemText(selectvect[i],0);	

if(g_bywindowswechatflag)
tempid=ExtractGroupidFromStr(gnamestr);
else
tempid=GetGroupUidFromUserNameOrNickName(gnamestr,1);

if(tempid)
	qgroupuinvect.push_back(tempid);
	
	}

	BOOL bIskeyword=false;
	if(qgroupuinvect.size()<MAXGROUPNUM&&qgroupuinvect.size()>0)
	{
			for(int i=0;i<qgroupuinvect.size();i++)
	{
	
	CMsgDlg* tempmsgdlg;
	if(m_mapqqmsgdlg.Lookup(qgroupuinvect[i],tempmsgdlg))
	{
		CString msgcontent=content;
		msgcontent.Trim();

		CString cmdtemplastmsg=msgcontent;
		BOOL bMatchReserveflag=false;
		for(auto it=g_reservecmdset.begin();it!=g_reservecmdset.end();it++)
		{
			if(MatchStar(*it,cmdtemplastmsg))
			{
				bMatchReserveflag=true;
				break;
			}
		}
		if(!bMatchReserveflag)
		{
			for(auto it=g_inputreplacemap.m_mapstr.rbegin();it!=g_inputreplacemap.m_mapstr.rend();it++)
			{
				CString temprp=*it;
				if(_tcsncmp(cmdtemplastmsg,_T("成语接龙"),lstrlen(_T("成语接龙")))==0)
				{
					if(_tcscmp(cmdtemplastmsg,*it)==0)
					{
						CString tempreplacefind;
						GetRandAnswer(tempreplacefind,g_inputreplacemap[*it]);
						cmdtemplastmsg.Replace(*it,tempreplacefind);
						break;
					}

				}
				else
					if(_tcsncmp(cmdtemplastmsg,*it,lstrlen(*it))==0)
					{
						CString tempreplacefind;
						GetRandAnswer(tempreplacefind,g_inputreplacemap[*it]);
						cmdtemplastmsg.Replace(*it,tempreplacefind);
						break;
					}


			}
		}

		if(!bIskeyword)
		{
			if(tempmsgdlg->IsKeyWord(msgcontent)||tempmsgdlg->IsKeyWord(cmdtemplastmsg))
			{
				bIskeyword=true;

			}
			else
			{
				bIskeyword=false;
				break;
			}
		}

		tempmsgdlg->ShowNewMsg(_T("批量后台发送"),msgcontent,0,g_interfaceuin);

	}
			
			}
			if(bIskeyword)
AfxMessageBox(_T("批量后台发送指令完成！"));
	}
	if(!g_defaultqqgrouprule.bTatalSlient&&!g_totalsilent)
	{
	if(!bIskeyword)
	{
	if(qgroupuinvect.size())
	AfxBeginThread(proSendQGroupmsg,(LPVOID)content);
	else
	{
	AfxMessageBox(_T("你尚未选择任何一个群，请先选中一个群再执行此操作！"));
	}

	}
	}
	else
	{
	AfxMessageBox(_T("彻底静默模式下无法批量发送普通消息（各种指令除外）！"));
	}
}
vector<INT64> qquinvect;
UINT proSendqqmsg(LPVOID lp)
{
	int rtflagok=0;
	int sumint=qquinvect.size();
	BOOL rtflag;
	for(int i=0;i<sumint;i++)
	{
	rtflag=SendqqMsg(qquinvect[i],(LPCTSTR)lp);
	if(rtflag)
	{
		rtflagok++;
		if(i!=sumint-1)
	Sleep(grouptimegap*1000);

	}
	else
	{
	break;
	}
	}
	if(sumint)
	{
		CString fmtstr;
		fmtstr.Format(_T("总计划发送%d条好友消息，成功发送%d条！成功率为%d%%,如果发送成功率太小，请检查你发送的内容是否被腾讯禁止或者请尝试分批多次发送！"),sumint,rtflagok,rtflagok*100/sumint);
		::MessageBox(0,fmtstr,_T("发送好友消息报告"),0);
	}
return 0;
}
void CIgnoreDlg::OnBnClickedButton12()
{
	if(!Isggged)
	{
	AfxMessageBox(_T("未注册版不能给选中的好友批量发消息，请点下面的“与选中的好友聊天”按钮，然后手动发消息！"));
	return;
	}
	if(g_defaultqqgrouprule.bTatalSlient||g_totalsilent)
	{
	
	AfxMessageBox(_T("彻底静默模式下无法批量给好友发送消息！"));
	
		return;
	}

	grouptimegap=GetDlgItemInt(IDC_EDIT6);
	if(grouptimegap<2)
		grouptimegap=2;

	intvect selectvect;
	GetSelectIntvect(m_list2,selectvect);
	if(!selectvect.size())
	{
		AfxMessageBox(_T("请先用鼠标单击选中一个好友！"));
		return;
	}
	static TCHAR content[1024];
	GetDlgItemText(IDC_EDIT1,content,1024);
	CString gnamestr;
	INT64 tempid;
	qquinvect.clear();
	if(!selectvect.size())
	{
		AfxMessageBox(_T("你尚未选择任何一个好友，请先选中一个好友再执行此操作！"));
		return;
	}
	for(int i=0;i<selectvect.size();i++)
	{
gnamestr=m_list2.GetItemText(selectvect[i],0);	

tempid=_ttoi64(gnamestr);
if(tempid!=13402429)
qquinvect.push_back(tempid);
	
	}
	AfxBeginThread(proSendqqmsg,(LPVOID)content);
}


void CIgnoreDlg::OnBnClickedCheck2()
{
	if(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		for(int i=0;i<m_list2.GetItemCount();i++)
	m_list2.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	}
	else
{
	for(int i=0;i<m_list2.GetItemCount();i++)
	m_list2.SetItemState(i,0,-1);
	
	}
}


void CIgnoreDlg::OnBnClickedCheck1()
{
	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		for(int i=0;i<m_list1.GetItemCount();i++)
	m_list1.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	}
	else
{
	for(int i=0;i<m_list1.GetItemCount();i++)
	m_list1.SetItemState(i,0,-1);
	
	}

	// TODO: 在此添加控件通知处理程序代码
}


void CIgnoreDlg::OnEnKillfocusEdit1()
{
	lockhellowres.Lock();
	GetDlgItemText(IDC_EDIT1,g_hellowstr);
	lockhellowres.Unlock();
}

void CIgnoreDlg::OnEnKillfocusEdit2()
{
	SaveEditValue();
}




void CIgnoreDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		m_parentwnd->OnDelIgnoreDlg();
	//CMyBGCDlg::OnClose();
}




void CIgnoreDlg::OnBnClickedButton3()
{
	intvect selectvect;
	GetSelectIntvect(m_list1,selectvect);
	if(!selectvect.size())
	{
		AfxMessageBox(_T("请先用鼠标单击选中一个群！"));
		return;
	}
	if(selectvect.size()>0)
	{
		CString gnamestr;
gnamestr=m_list1.GetItemText(selectvect[0],0);	

INT64 gid=0;
if(g_bywindowswechatflag)
gid=ExtractGroupidFromStr(gnamestr);
else
gid=GetGroupUidFromUserNameOrNickName(gnamestr,1);//GetGidFromGname(gnamestr);
INT64 temprealnum=GetRealqqnumber(gid,true);
if(temprealnum)
{

	if(!GRealNumRulemap.Lookup(temprealnum))
	{
		qqgrouprule temprule;
			BOOL rtflag=GetRuleFromFile(true,temprealnum,temprule);
			if(rtflag)
			GRealNumRulemap[temprealnum]=temprule;
			else
			{
GRealNumRulemap[temprealnum]=g_defaultqqgrouprule;
			}
GRealNumRulemap[temprealnum].m_selfrealqqgroupnum=temprealnum;
			

	}

CAdminList dlg(GRealNumRulemap[temprealnum]);
	dlg.DoModal();
}

	}
	else
	{
	
AfxMessageBox(_T("请先选中一个群，再进行设置！"));
	}

}
extern BOOL g_latestwechatflag;
extern UINT ProQuitGroup(LPVOID lp);
void CIgnoreDlg::OnBnClickedButton6()
{
	

	if(!g_latestwechatflag&&!g_newestversion)
	{
	MessageBox(_T("只有配合windows微信客户端2.5版以上的方式才能退群！"),_T("当前模式下的机器人无法退出群"),MB_ICONWARNING);
	
	return;
	}
	
	
	INT64 groupuin;
		intvect selectvect;
	GetSelectIntvect(m_list1,selectvect);
	
//	CString gnamestr;
//	for(int i=0;i<selectvect.size();i++)
//	{
//gnamestr=m_list1.GetItemText(selectvect[i],1);	
//
//groupuin=_ttoi64(gnamestr);//GetGidFromGname(gnamestr);
//		//第一次获取群信息
//				{
//				//GetLatestGroupInf(groupuin);
//
//
//				}
//				//GetGroupUinNickName(groupuin);
//		
//	}

	if(IDOK!=MessageBox(_T("为避免误操作，请再次确认是否要退出这些群，你可以按住Ctrl键一次退出多个群！"),_T("是否确认退出这些群！"),MB_OKCANCEL))
	{
	return;
	}
	
	

	INT64 gid;
	INT64 gcode=0;
	CString gnamestr;
	for(int i=0;i<selectvect.size();i++)
	{
gnamestr=m_list1.GetItemText(selectvect[i],3);	

gid=_ttoi64(gnamestr);//GetGidFromGname(gnamestr);

lockgroupnameresource.Lock();
GidNickNamemap.RemoveKey(gid);
lockgroupnameresource.Unlock();
//	lockgidgcoderesource.Lock();
//gcode=GidGcodemap[gid];
//	lockgidgcoderesource.Unlock();
	INT64 *pgcode=new INT64;
	*pgcode=gid;//gcode;
	
	AfxBeginThread(ProQuitGroup,(LPVOID)pgcode);
	Sleep(200);
	//if(m_mapqqmsgdlg.Lookup(gid))
	//{
	//	CMsgDlg *dlg=m_mapqqmsgdlg[gid];
	//m_mapqqmsgdlg.RemoveKey(gid);
	//delete dlg;
	//}
	//g_realgroup_hwndMap.RemoveKey(gid);

	}
for(int i=selectvect.size()-1;i>=0;i--)
	{
	m_list1.DeleteItem(selectvect[i]);
	}


}
CString Getfinalmergegroupname(LPCTSTR groupname)
{
CString mergegroupname;
vector<CString>::iterator it;
	lockdislikevectres.Lock();
					it=find_if(dislkbackbufvect.m_mapstr.begin(),dislkbackbufvect.m_mapstr.end(),SameqqGroupNumfun(groupname));
					
					if(it==dislkbackbufvect.m_mapstr.end())
					{
						if(g_bywindowswechatflag)
								 mergegroupname=groupname;
						else
						{
						if(_ttoi64(groupname))
							mergegroupname=GetNickNameFromHashvalue(0,_ttoi64(groupname));
						else
							mergegroupname=groupname;
						}
						//mergegroupname=groupname;
					}
					else
					{
						mergegroupname=*it;
					}
						lockdislikevectres.Unlock();
					return mergegroupname;
}
void AddValidDaysforSomeGroup(LPCTSTR groupname,int days,CString& lastvalidtime)
{
	if(!g_bIgnoreAll)
		{
		//AfxMessageBox(_T("只有在“选项设置”——“全局设置”里勾选了“默认屏蔽所有群，仅在手动开启的群里才能使用机器人”才可以设置有效期年月日信息。"));
		lastvalidtime=_T("只有在“选项设置”——“全局设置”里勾选了“默认屏蔽所有群，仅在手动开启的群里才能使用机器人”才可以设置有效期年月日信息。");
			return;
		}
	CString nowvalidtime;
	CString mergegroupname = Getfinalmergegroupname(groupname);
					INT64 overtimevalue=0;
					lockdislikevectres.Lock();
					dislkbackbufvect.Lookup(mergegroupname,overtimevalue);
					lockdislikevectres.Unlock();
GetValidTime(overtimevalue,nowvalidtime);
if(days>0)
{
	INT64 nowtime=GetTimeFromStr(nowvalidtime);	
nowtime=AddValidTime(days,nowtime);
GetValidTime(nowtime,nowvalidtime);
lastvalidtime.Format(_T("已成功增加群%s有效期至%s"),groupname,nowvalidtime);
lockdislikevectres.Lock();
dislkbackbufvect[mergegroupname]=nowtime;
lockdislikevectres.Unlock();
 IsIgnoreOrShutupGroupForName(mergegroupname);
			LoadIgnoreFile(false);
}	
else
{
lastvalidtime.Format(_T("群%s有效期至%s"),groupname,nowvalidtime);

}

		 
			
}
void CIgnoreDlg::OnBnClickedButton13()
{
		if(!g_bIgnoreAll)
		{
		AfxMessageBox(_T("只有在“选项设置”——“全局设置”里勾选了“默认屏蔽所有群，仅在手动开启的群里才能使用机器人”才可以设置有效期年月日信息。"));
		return;
		}
	intvect selectvect;
	GetSelectIntvect(m_list1,selectvect);
	static TCHAR content[1024];
	GetDlgItemText(IDC_EDIT1,content,1024);
	CString gnamestr;
	INT64 tempid;
	qgroupuinvect.clear();
	for(int i=0;i<selectvect.size();i++)
	{

	CString cstrItemTextEdit;
		cstrItemTextEdit=m_list1.GetItemText(selectvect[i],2);  
	INT64 nowtime=GetTimeFromStr(cstrItemTextEdit);	
nowtime=AddValidTime(30,nowtime);
GetValidTime(nowtime,cstrItemTextEdit);
		 m_list1.SetItemText(selectvect[i],2,cstrItemTextEdit);  
		 m_list1.Invalidate();
		 
		 CString filenamebuf2=m_list1.GetItemText(selectvect[i],0);  

		 CString	finalfilenamebuf2 ;
		 if(g_bywindowswechatflag)
			 finalfilenamebuf2= Getfinalmergegroupname(filenamebuf2);//filenamebuf2;// 
		 else
			 finalfilenamebuf2=filenamebuf2;
			
		 lockdislikevectres.Lock();
			dislkbackbufvect[finalfilenamebuf2]=nowtime;
			lockdislikevectres.Unlock();
			 IsIgnoreOrShutupGroupForName(finalfilenamebuf2);
	}

		//
}

extern void SendpackForCreateGroup(const_vectorbyte chatroommemberlist);
void CIgnoreDlg::OnBnClickedButton9()
{

	
	if(!g_latestwechatflag&&!g_newestversion)
	{
	MessageBox(_T("只有配合windows微信客户端2.5版以上的方式才能删除好友！"),_T("当前模式下的机器人无法删除好友"),MB_ICONWARNING);
	
	return;
	}
	
	
	INT64 groupuin;
		intvect selectvect;
	GetSelectIntvect(m_list2,selectvect);
	
//	CString gnamestr;
//	for(int i=0;i<selectvect.size();i++)
//	{
//gnamestr=m_list1.GetItemText(selectvect[i],1);	
//
//groupuin=_ttoi64(gnamestr);//GetGidFromGname(gnamestr);
//		//第一次获取群信息
//				{
//				//GetLatestGroupInf(groupuin);
//
//
//				}
//				//GetGroupUinNickName(groupuin);
//		
//	}

	if(IDOK!=MessageBox(_T("为避免误操作，请再次确认是否要删除这些好友，你可以按住Ctrl键一次删除多个好友！"),_T("是否确认删除这些好友！"),MB_OKCANCEL))
	{
	return;
	}
	
	

	INT64 gid;
	INT64 gcode=0;
	CString gnamestr;
	for(int i=0;i<selectvect.size();i++)
	{
gnamestr=m_list2.GetItemText(selectvect[i],0);	

gid=_ttoi64(gnamestr);//GetGidFromGname(gnamestr);


//	lockgidgcoderesource.Lock();
//gcode=GidGcodemap[gid];
//	lockgidgcoderesource.Unlock();
	CString rtwxid;
QueryWxidFromHashValue(0,gid,rtwxid,NULL);
	SendpackForQuitGroup(rtwxid,0);//type=0删除好友
	//Sleep(200);
	//if(m_mapqqmsgdlg.Lookup(gid))
	//{
	//	CMsgDlg *dlg=m_mapqqmsgdlg[gid];
	//m_mapqqmsgdlg.RemoveKey(gid);
	//delete dlg;
	//}
	//g_realgroup_hwndMap.RemoveKey(gid);

	}
for(int i=selectvect.size()-1;i>=0;i--)
	{
	m_list2.DeleteItem(selectvect[i]);
	}


//
//	intvect selectvect;
//	GetSelectIntvect(m_list2,selectvect);
//
//	CString gnamestr;
//	INT64 tempid;
//
//	if(!selectvect.size())
//	{
//		AfxMessageBox(_T("你尚未选择任何一个好友，请先选中一个好友再执行此操作！"));
//		return;
//	} 
//	CString totaltempstr;
//	CString tempstr;
//	CString fengestr;
//	vectorbyte memberlist;
//	vectorbyte tempfenge;
//
//	tempfenge.push_back(0x5e);
//	tempfenge.push_back(0);
//tempfenge.push_back(0x47);
//tempfenge.push_back(0);
//
//for(int i=0;i<selectvect.size();i++)
//{
//	gnamestr=m_list2.GetItemText(selectvect[i],0);
//
//	tempid=_ttoi64(gnamestr);
//	CString rtwxid;
//	QueryWxidFromHashValue(0,tempid,rtwxid,NULL);
//
//
//	/*tempstr.Format(_T("%s"),rtwxid);
//	vectorbyte temp1v;
//	temp1v.assign((char*)rtwxid.GetBuffer(),(char*)rtwxid.GetBuffer()+lstrlen(rtwxid)*2);
//	temp1v.push_back(0);
//	temp1v.push_back(0);
//
//	memberlist.insert(memberlist.end(),temp1v.begin(),temp1v.end());
//
//	if(i!=selectvect.size()-1)
//	{
//
//		memberlist.insert(memberlist.end(),tempfenge.begin(),tempfenge.end());
//	}*/
//
//
//}
/*
	 vectorbyte temp1v;
temp1v.assign((char*)value,(char*)value+lstrlen(value)*2);
	temp1v.push_back(0);
	temp1v.push_back(0)
*/
	//SendpackForCreateGroup(memberlist);

	/*for(UINT i=0;i<curgroupuser2nickvect.size();i++)
	{
		tempstr.Format(_T("%s,"),curgroupuser2nickvect[i]);
		userlist+=tempstr;
	}
	::function::TrimRight(userlist,_T(","));
	CString deletedmestr;*/
	//CreateGroupForDefPeople(topicname,userlist,NULL,&deletedmestr);
	

//	int maxcount=35;
//int times=0;
//times=(g_wxfriendusername2nickname.GetCount()+maxcount-1)/maxcount;
//
//
//int ncount=0;
//
//CString tempstr;
//while(ncount<times)
//{
//CStringvect curgroupuser2nickvect;
//	if(ncount==times-1)
//		curgroupuser2nickvect.assign(g_wxfriendusername2nickname.m_mapstr.begin()+ncount*maxcount,g_wxfriendusername2nickname.m_mapstr.end());
//	else
//		curgroupuser2nickvect.assign(g_wxfriendusername2nickname.m_mapstr.begin()+ncount*maxcount,g_wxfriendusername2nickname.m_mapstr.begin()+(ncount+1)*maxcount);
//
//	CString topicname;
//	CString userlist;
//	for(UINT i=0;i<curgroupuser2nickvect.size();i++)
//	{
//		tempstr.Format(_T("%s,"),curgroupuser2nickvect[i]);
//		userlist+=tempstr;
//	}
//	::function::TrimRight(userlist,_T(","));
//	CString deletedmestr;
//	int ret=CreateGroupForDefPeople(topicname,userlist,NULL,&deletedmestr);
//	if(ret!=0)
//	{
//
//		AfxMessageBox(_T("读取错误，不知道是不是短期内操作太频繁导致的，如果是的话，需要联系软件作者加大每批次检测之后的延时时间，避免短时间内操作太多！"));
//		break;
//	}
//if(ncount<times)
//	ncount++;
//}
//


}
void CIgnoreDlg::OnBnClickedButton8()
{
		 CString str; 
			 if(str.IsEmpty())
			 {
			int istat=0;
			POSITION pos = m_list2.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("未选中任何项，请重试！"));
			return;
			//TRACE(_T("No items were selected!\n"));
		}
		else
		{
			//while (pos)
			{
				istat= m_list2.GetNextSelectedItem(pos);
				
			}
		}
				
		CString	newmsg=m_list2.GetItemText(istat,0);
		//newmsg+=_T("           ");
				//newmsg+=m_list2.GetItemText(istat,1);
			
		str=newmsg;
	

			 }
        HGLOBAL hClip; 
        //定义一个HGLOBAL句柄变量用来指向分配的内存块
        if (OpenClipboard())
        {
            EmptyClipboard();                            //将剪贴板内容清空
            hClip=GlobalAlloc(GMEM_MOVEABLE,(str.GetLength()+1)*2); 
            //在堆上分配可移动的内存块，程序返回一个内存句柄
            TCHAR * buff;                                 //定义指向字符型的指针变量
            buff=(TCHAR*)GlobalLock(hClip);
            //对分配的内存块进行加锁，将内存块句柄转化成一个指针,并将相应的引用计数器加1
            lstrcpy(buff,str);
            //将用户输入的数据复制到指针变量中，实际上就是复制到分配的内存块中
            GlobalUnlock(hClip);
            //数据写入完毕，进行解锁操作，并将引用计数器数字减1
            SetClipboardData(CF_UNICODETEXT,hClip);
            //将存放有数据的内存块放入剪贴板的资源管理中
            CloseClipboard();
            //关闭剪贴板，释放剪贴板资源的占用权
			//AfxMessageBox(_T("已成功复制到系统的剪贴板中，你可以直接粘贴到记事本或者群规设置中的主人列表中。"));
        }
		
}


void CIgnoreDlg::OnEnKillfocusEdit5()
{
	SaveRemarkEditValue();
}


void CIgnoreDlg::OnBnClickedButton22()
{
			int itemcount=m_list2.GetItemCount();
	CString qqstr,qqpasswordstr;
	CString strfmt;
	CString findstr;
	GetDlgItemText(IDC_EDIT6,findstr);

		if(lastfindpos>itemcount)
			lastfindpos=itemcount;
		
int i=0;
	for(i=lastfindpos-1;i>=0;i--)
	{
		
		qqstr=m_list2.GetItemText(i,1);
		qqpasswordstr=m_list2.GetItemText(i,0);
	if(StrStrI(qqstr,findstr)||StrStrI(qqpasswordstr,findstr))
	{

		if(lastfindpos<itemcount)
m_list2.SetItemState(lastfindpos,0,-1);
	
	m_list2.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	lastfindpos=i;
	m_list2.EnsureVisible(i,false);
m_list2.SetFocus();
	break;
	}

	}
	if(i==-1)
		lastfindpos=itemcount;

}
