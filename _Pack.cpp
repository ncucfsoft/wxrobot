#include "stdafx.h"
#include "publicshare.h"

#include "_Pack.h"

unsigned long long ntohll(unsigned long long val)
{
   
        return (((unsigned long long )htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
  
}

unsigned long long htonll(unsigned long long val)
{
    
        return (((unsigned long long )htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
  
}
 vectorbyte CopyAndRemoveFromVect(vectorbyte& orgvect,int len)
{

vectorbyte bin;
if(len!=orgvect.size())
{
int minlen=min(len,orgvect.size());
bin.assign(orgvect.begin(),orgvect.begin()+minlen);
vectorbyte restbin;
restbin.assign(orgvect.begin()+minlen,orgvect.end());
orgvect=restbin;
}
else
{
bin=orgvect;
orgvect.clear();
}

return bin;
 }
void setInt(vector<byte>& m_bin,int value)
{
	//1413795694
	int anothervalue=value;//htonl(value);
byte* pbyte=(byte*)&anothervalue;
for(int i=0;i<4;i++)
{
m_bin.push_back(*(pbyte+i));
}

}
void setInt64(vector<byte>& m_bin,INT64 value)
{
	INT64 anothervalue=value;//htonll(value);
byte* pbyte=(byte*)&anothervalue;
for(int i=0;i<8;i++)
{
m_bin.push_back(*(pbyte+i));
}

}

void setShort(vector<byte>& m_bin,short value)
{//1234
	short anothervalue=value;//htons(value);//3412
byte* pbyte=(byte*)&anothervalue;
for(int i=0;i<2;i++)
{
m_bin.push_back(*(pbyte+i));
}

}
void setByte(vector<byte>& m_bin,byte value)
{

m_bin.push_back(value);


}

vectorbyte setHex(const char* value)
{
	vectorbyte m_bin;
	CString szRecValue=(CString)(CStringA)value;
	szRecValue.Trim();
	CStringvect vectemp;
	if(szRecValue.Find(' ')!=-1)
	{
		::function::getmytoken(szRecValue,_T(" \r\n"),vectemp);
		for(int i=0;i<vectemp.size();i++)
		{
			m_bin.push_back(_tcstol(vectemp[i],0,16));
		}
	}
	else//6a3b4f
	{
		CStringA tempstr;

		int len=strlen(value);

		for(int i=0;i<len;i+=2)
		{
			if(i+1<len)
			{
				tempstr.Format("%c%c",value[i],value[i+1]);
				m_bin.push_back(strtol(tempstr,0,16));
			}
		}


	}
	return m_bin;
}
void setHex(vector<byte>& m_bin,const char* value)
{
	vectorbyte tempadd=setHex(value);
m_bin.insert(m_bin.end(),tempadd.begin(),tempadd.end());
//
//	CString szRecValue=(CString)(CStringA)value;
//CStringvect vectemp;
//	::function::getmytoken(szRecValue,_T(" \r\n"),vectemp);
//	for(int i=0;i<vectemp.size();i++)
//	{
//	m_bin.push_back(_tcstol(vectemp[i],0,16));
//	}
	

}

void setBin(vector<byte>& m_bin,const byte* value,int size)
{

	
	for(int i=0;i<size;i++)
	{
	m_bin.push_back(*(value+i));
	}
	

}
void setBin(vector<byte>& m_bin,const_vectorbyte value)
{

	
	m_bin.insert(m_bin.end(),value.begin(),value.end());
	

}

void setStr(vector<byte>& m_bin,const char* value)
{


for(int i=0;i<strlen(value);i++)
{
m_bin.push_back(*(value+i));
}

}
void setToken(vector<byte>& m_bin,const vectorbyte& value)
{

	setShort(m_bin,short(value.size()));

m_bin.insert(m_bin.end(),value.begin(),value.end());

}
void AndroidSetToken(vector<byte>& m_bin,const vectorbyte& value)
{

	setShort(m_bin,short(value.size()+4));

m_bin.insert(m_bin.end(),value.begin(),value.end());

}

void AndroidsetShortString(vector<byte>& m_bin,const_vectorbyte& value)
{
int len=value.size();

	setByte(m_bin,len);
	if(len)
	setBin(m_bin,&value[0],value.size());
}
void AndroidsetString(vector<byte>& m_bin,const_vectorbyte& value)
{
setHex(m_bin,"00 00 04 00 00");
AndroidSetToken(m_bin,value);
setByte(m_bin,0);
}

void AndroidSetShortToken(vector<byte>& m_bin,const_vectorbyte& value)
{

	setByte(m_bin,byte(value.size()));

setBin(m_bin,value);

}
byte * memstr(const byte * str1, const byte * str2, int nLen1,int nLen2)
{
	/*CString str222;
	str222.Format(_T("memstr"));
	
		Write2MagnetLog(str222,&magnetfile,8);*/

    if ((NULL == str1) || (NULL == str2) || (nLen1 <= 0)) 
        return NULL;
 

    long ls1 = nLen1;
	if(nLen2==0)
		nLen2=strlen((char*)str2);

    byte *cp = (byte *) str1;
    byte *s1, *s2;



    while (ls1 > 0)
    {
        s1 = cp;
        s2 = (byte *) str2;

       while (!(*s1-*s2))//  while ( *s1 && *s2 && !(*s1-*s2) )
	   {
		
		   s1++, s2++;
		  
	/*str222.Format(_T("s1:%d  s2:%d  len1:%d len2:%d"),s1-str1,s2-str2,nLen1,nLen2);
	
		Write2MagnetLog(str222,&magnetfile,8);*/
		   if(s1-str1>=nLen1||s2-str2>=nLen2)
		   {
		 /* str222.Format(_T("±ÀÀ£%d  ±ÀÀ£%d  len1:%d len2:%d"),s1-str1>nLen1,s2-str2>nLen2,nLen1,nLen2);
	
	
		Write2MagnetLog(str222,&magnetfile,8);*/
			   break;
		   }
	   }
     if(s2-str2==nLen2) //  if (!*s2)
	 {
   /*  str222.Format(_T("return cp:%d"),cp-str1);
	
		Write2MagnetLog(str222,&magnetfile,8);*/
		 return(cp);
	 }
	    //str222.Format(_T("not return cp-str1:%d  s2-str2:%d   ls1:%d"),cp-str1,s2-str2,ls1);
	
		//Write2MagnetLog(str222,&magnetfile,8);
        cp++;
        ls1--;
    }

    return(NULL);
}
byte * memstrrev(const byte * str1, const byte * str2,int nLen1,int nLen2=0 )
{
    if ((NULL == str1) || (NULL == str2) || (nLen1 <= 0)) 
        return NULL;

		if(nLen2==0)
		nLen2=strlen((char*)str2);

    long ls2 = nLen2;
    long ls1 = nLen1 - ls2;

    byte *cp = (byte *)(str1 + ls1);
    byte *s1, *s2;
 
   
    while (ls1 > 0)
    {
        s1 = cp;
        s2 = (byte *)str2;

     
   while (!(*s1-*s2))//  while ( *s1 && *s2 && !(*s1-*s2) )
   {    
	   s1++, s2++;
	      if(s1-str1>=nLen1||s2-str2>=nLen2)
		   {
		  /* 	CString str222;
	str222.Format(_T("±ÀÀ£%d  ±ÀÀ£%d"),s1-str1>nLen1,s2-str2>nLen2);
		Write2MagnetLog(str222,&magnetfile,8);*/
		break;
		   }
   }
     if(s2-str2==nLen2) //  if (!*s2)
		 return (cp);
        cp--;
        ls1--;
    }

    return(NULL);
}
bool bcompi(BYTE byA, BYTE byB)
{
    if (byA - byB == 0) 
    {
        return true;
    }
    else
    {
        if (!((byA >= 'A' && byA <= 'Z') || (byA >= 'a' && byA <= 'z'))) 
            return false;

        if (!((byB >= 'A' && byB <= 'Z') || (byB >= 'a' && byB <= 'z'))) 
            return false;

        if ((byA - byB == 'a' - 'A') || (byB - byA == 'a' - 'A')) 
            return true;
        else
            return false;
    }
}
byte * memistr(const byte * str1, const byte * str2, int nLen1,int nLen2=0)
{
    if ((NULL == str1) || (NULL == str2) || (nLen1 <= 0)) 
        return NULL;
 
		if(nLen2==0)
		nLen2=strlen((char*)str2);

    long ls1 = nLen1;
    byte *cp = (byte *) str1;
    byte *s1, *s2;
 
   
 
    while (ls1 > 0)
    {
        s1 = cp;
        s2 = (byte *) str2;
 
        while (bcompi(*s1, *s2) )// *s1 && *s2 && 
            s1++, s2++;
 
        if (s2-str2==nLen2)
            return(cp);
  
        cp++;
        ls1--;
    }
 
    return(NULL);
}


int Fun_send(const_vectorbyte& bin)
{
	int	outlen=0;
//int	outlen=.Send(&bin[0],bin.size());
	//increase_ssoSeq();
	
	if(bin.size())
 outlen= send (g_socket,(char*)&bin[0],bin.size(), 0);  

    if (outlen == SOCKET_ERROR)  
    {  
      
    }  
	else if(outlen<bin.size())
	{
	Sleep(20);
	int restlen=send (g_socket,(char*)&bin[0]+outlen,bin.size()-outlen, 0);  
	outlen+=restlen;

	}



//pos++;
//lastId++;
return outlen;
}

int Receive(char *pch, const int nSize, const int nSecs)
{
assert(g_socket != NULL);
FD_SET fd = {1, g_socket};
TIMEVAL tv = {nSecs, 0};
if(select(0, &fd, NULL, NULL, &tv) == 0)
{
return -1;
}

int nBytesReceived;
if((nBytesReceived = recv(g_socket, pch, nSize, 0)) == SOCKET_ERROR) 
{
	int lastdd=GetLastError();

	
		//if(lastdd)//lastdd==0x2745||lastdd==0x2746)
		//{
		//	bNeedResetconnect=true;
		//}

return -1;
}
if(nBytesReceived>0)
{
//CString strtext;
//SYSTEMTIME time1;
//GetLocalTime(&time1);

	//strtext.Format(_T("%2d:%2d %2d ÊÕµ½%d¸ö×Ö½Ú"),time1.wHour,time1.wMinute,time1.wSecond,nBytesReceived);
	//SetWindowText(mainhwnd,strtext);
}
return nBytesReceived;
}

int ReceiveAll(char *pch, const int nSize, const int nSecs)
{
int nBytesReceive = 0;
int nBytesThisTime;
    char* pch1 = pch;
do {
nBytesThisTime = Receive(pch1, nSize - nBytesReceive, nSecs);
if (nBytesThisTime == -1)
return -1;

nBytesReceive += nBytesThisTime;
pch1 += nBytesThisTime;
} while(nBytesReceive < nSize);
return nBytesReceive;
}
const_vectorbyte char2vector(const char* str,int len)
{
	vectorbyte tempvect;

 if(len)
 {
tempvect.resize(len);

 memcpy(&tempvect[0],str,len);
 }
 return tempvect;
}
const_vectorbyte char2vector(const char* str)
{
	vectorbyte tempvect;
	int len=strlen(str);
 if(len)
 {
tempvect.resize(len);

 memcpy(&tempvect[0],str,strlen(str));
 }
 return tempvect;
}
const_vectorbyte string2vector(const string str)
{
	vectorbyte tempvect;
	int len=str.size();
 if(len)
 {
tempvect.assign(str.begin(),str.end());

 }
 return tempvect;
}

CStringA vector2char(const_vectorbyte orgstr)
{
	CStringA rtstr;
	
	for(int i=0;i<orgstr.size();i++)
	{
	rtstr.AppendChar(orgstr[i]);
	}
 return rtstr;
}

 void _Pack::Empty()
 {
	 m_bin.clear();
 }
 void _Pack::SetInt(int value)
 {
 setInt(m_bin,value);
 }
 void _Pack::SetShort(short value)
 {
 setShort(m_bin,value);
 }
 void _Pack::SetInt64(INT64 value)
 {
 setInt64(m_bin,value);
 }
 void _Pack::SetLong(INT64 value)
 {
 setInt64(m_bin,value);
 }

 void _Pack::SetData(const_vectorbyte& value)
 {

	 m_bin=value;


 }

 const_vectorbyte& _Pack::GetAll()
 {
 return m_bin;
 }
 void _Pack::SetHex(const char* hexstr)
 {
	 setHex(m_bin,hexstr);
 }
 void _Pack::SetByte(byte bvalue)
 {
 setByte(m_bin,bvalue);
 }
 void _Pack::AndroidSetString(const_vectorbyte& str)
 {
::AndroidsetString(m_bin,str);
 }
 void _Pack::SetToken(const_vectorbyte& value)
{
	setToken(m_bin,value);

}
 void _Pack::SetToken(const TCHAR* value)
{
	 vectorbyte temp1v;
temp1v.assign((char*)value,(char*)value+lstrlen(value)*2);
	temp1v.push_back(0);
	temp1v.push_back(0);

	SetToken(temp1v);

}

void _Pack::AndroidSetShortToken(const_vectorbyte& value)
{
::AndroidSetShortToken(m_bin,value);
}
void _Pack::AndroidSetShortString(const_vectorbyte& value)
{
::AndroidsetShortString(m_bin,value);
}
 void _Pack::SetStr(const char* str)
 {
	 setStr(m_bin,str);
 }

 void _Pack::SetBin(byte* addbin,int len)
 {
	 m_bin.insert(m_bin.end(),addbin,addbin+len);
 }
 void _Pack::SetBin(const_vectorbyte& addbin)
 {
	 m_bin.insert(m_bin.end(),addbin.begin(),addbin.end());
 }
