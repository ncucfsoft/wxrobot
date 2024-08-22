#include "stdafx.h"
#include "publicshare.h"
#include "_Unpack.h"


_Unpack::_Unpack()
{
m_pos=0;
}
 int _Unpack::GetInt()
 {
	 vectorbyte temp=GetBin(4);
	 if(temp.size()==0)
		 return 0;
	 int tempint2=*(int*)&temp[0];
	 int tempint=tempint2;//ntohl(tempint2);
	 return tempint;
 }
 short _Unpack::GetShort()
 {
 vectorbyte temp=GetBin(2);//1234
  if(temp.size()==0)
		 return 0;

 short tempsss=*(short*)&temp[0];//3412
 short tempint=tempsss;//ntohs(tempsss);//1234
	 return tempint;
 }
 UINT64 _Unpack::GetLong()
 {
 vectorbyte temp=GetBin(8);
  if(temp.size()==0)
		 return 0;
 UINT64 tempint2=*(UINT64*)&temp[0];
 UINT64 tempint=tempint2;//ntohll(tempint2);
	 return tempint;
 }

 vectorbyte _Unpack::GetAll()
 {
	 vectorbyte tempv;
	 tempv.assign(m_bin.begin()+m_pos,m_bin.end());
 return tempv;
 }
 vectorbyte _Unpack::AndroidGetToken()
 {
vectorbyte bin;
int i=GetShort ();
i=i-4;
bin=GetBin (i);
return (bin);

 }
 vectorbyte _Unpack::GetShortToken()
 {
 

int i=GetByte ();
vectorbyte bin=GetBin (i);
return bin;
 }

 CStringA _Unpack::GetAll_Hex()
 {
CStringA totalstr;
CStringA tempstr;
//for(int i=0;i<m_bin.size();i++)
	for(auto it=m_bin.begin()+m_pos;it!=m_bin.end();it++)
{
	tempstr.Format("%02X ",*it);
	totalstr+=tempstr;
}
 return totalstr;
 }

 void _Unpack::SetData_Hex(char* hexstr)
 {
	setHex(m_bin,hexstr);
	m_pos=0;
 }
 void _Unpack::SetData(vectorbyte value)
 {
	 m_bin=value;
	 m_pos=0;
 }

 byte _Unpack::GetByte()
 {
 vectorbyte temp=GetBin(1);
  if(temp.size()==0)
		 return 0;
	 byte tempint=*(byte*)&temp[0];
	 return tempint;
 } 
 vectorbyte _Unpack::GetToken()
{
	 CString str;

short len=GetShort();

return GetBin(len);
}
 CString _Unpack::GetTokenstr(CString *utfword)
{
	 CString str;

vectorbyte tempbytev=GetToken();
if(tempbytev.size())
{
	int len=tempbytev.size()/2+2;
	TCHAR *strbuf=new TCHAR[len];
	ZeroMemory(strbuf,len*sizeof(TCHAR));
	memcpy(strbuf,&tempbytev[0],tempbytev.size());
	if(utfword)
	{
	char *strbuf2=new char[tempbytev.size()+1];
	ZeroMemory(strbuf2,tempbytev.size()+1);
	memcpy(strbuf2,&tempbytev[0],tempbytev.size());
	::function::utf8_wchar(strbuf2,*utfword);
	}
	str=strbuf;
	delete[]strbuf;
}
return str;
}
// CString _Unpack::GetTokenstr2()
//{
//	 CString str;
//
//vectorbyte tempbytev=GetToken();
//int len=tempbytev.size()/2+2;
//TCHAR *strbuf=new TCHAR[len];
//ZeroMemory(strbuf,len*sizeof(TCHAR));
//memcpy(strbuf,&tempbytev[0],tempbytev.size());
//short testzero=GetShort();
//if(testzero)
//m_pos-=2;
//
//str=strbuf;
//return str;
//}
 
 vectorbyte _Unpack::GetBin(int len)
{

vectorbyte bin;
if(len>0)
{
int minlen=min(len,m_bin.size()-m_pos);
bin.assign(m_bin.begin()+m_pos,m_bin.begin()+m_pos+minlen);
//vectorbyte restbin;
//restbin.assign(m_bin.begin()+minlen,m_bin.end());
//m_bin=restbin;
m_pos+=minlen;
}
return bin;
 }

