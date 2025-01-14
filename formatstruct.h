#pragma once
#include "publicshare.h"
#include "_pack.h"

extern qq_info qq;
class _Unpack;
class _Tlv;
class _BIN;
class _Other;
class _Hash;
extern _BIN Xbin;
extern _Other Other;
extern _Hash Hash;
extern BOOL g_bQQflag;

class _BIN
{
public:
static vectorbyte GetRandomBin(int len)
{
	vectorbyte temp;
	for(int i=0;i<len;i++)
	{
	temp.push_back(rand()%256);
	}

return temp;
}
static int Random()
{
return (rand() << 16) | rand();
}
static CStringA Bin2Hex(const_vectorbyte& bin)
{
	CStringA totalstr;
	CStringA templine;
	for(int i=0;i<bin.size();i++)
	{
	templine.Format("%02X ",bin[i]);
	totalstr+=templine;
	}
	totalstr.Trim();
	return totalstr;
}
static CStringA Bin2HexNoBlank(const_vectorbyte& bin)
{
	CStringA totalstr;
	CStringA templine;
	for(int i=0;i<bin.size();i++)
	{
	templine.Format("%02X",bin[i]);
	totalstr+=templine;
	}
	totalstr.Trim();
	return totalstr;
}
static CStringA Bin2HexNoBlank(const string& bin)
{
	CStringA totalstr;
	CStringA templine;
	for(auto it=bin.begin();it!=bin.end();it++)
	{
	templine.Format("%02X",(byte)*it);
	totalstr+=templine;
	}
	totalstr.Trim();
	return totalstr;
}


};
class _Other
{
	public:
static INT64 TimeStamp(int bNeedMillOrU=0,CString* rttimestr=NULL/*=NULL*/)
{
	
SYSTEMTIME st;
GetLocalTime(&st);
	CTime time1(st);
	CStringA timestr2;
	if(bNeedMillOrU==1)
		timestr2.Format("%lld%03d",time1.GetTime(),st.wMilliseconds);
if(bNeedMillOrU==2)
{
				
FILETIME beg;
	GetSystemTimeAsFileTime(&beg);

	timestr2.Format("%lld%03d%03d",time1.GetTime(),st.wMilliseconds,beg.dwLowDateTime%1000);

	
}
	else
		timestr2.Format("%I64d",time1.GetTime());

	if(rttimestr)
		*rttimestr=timestr2;

	return _atoi64(timestr2);

}

};
extern const_vectorbyte Unpack(const byte* mybytev,int len);
extern const_vectorbyte Unpack(const_vectorbyte& mybytev);


class _Tlv
{
public:

vectorbyte tlv_pack(CStringA cmd,vectorbyte bin)
{

_Pack pack;

pack.Empty ();
pack.SetHex (cmd);
pack.SetShort (bin.size());
pack.SetBin (bin);

return pack.GetAll ();



}
vectorbyte tlv18(vectorbyte user)//, , hex
{

_Pack pack;

//' 00 18 //tlv18
//' 00 16 //tlv长度22 如果太长不是tlv包
//' 00 01 //_ping_version=1
//' 00 00 06 00 //_sso_version=1536
//' 00 00 00 10 //_appid
//' 00 00 00 00 //_app_client_version
//' 18 B4 A1 BC [QQ号码：414491068]
//' 00 00 //0
//' 00 00 //0
pack.Empty ();
pack.SetHex ("00 01");
pack.SetHex ("00 00 06 00");
pack.SetHex ("00 00 00 10");
pack.SetHex ("00 00 00 00");
pack.SetBin (user);
pack.SetHex ("00 00");
pack.SetHex ("00 00");

return tlv_pack ("00 18", pack.GetAll ());

}
vectorbyte tlv1(vectorbyte user,vectorbyte time)
{

_Pack pack;

//' 00 01 //tlv1
//' 00 14 //长度20
//' 00 01 //ip_ver=1
//' 3F AA 67 F9 //get_rand_32();
//' 18 B4 A1 BC [QQ号码：414491068]
//' 54 09 99 7F //get_server_cur_time
//' 00 00 00 00//_ip_addr
//' 00 00 //0
//' time ＝Xbin.Flip ( 取字节集左边 (到字节集 (Other.TimeStamp ();), 4))

pack.Empty ();
pack.SetHex ("00 01");
pack.SetBin (Xbin.GetRandomBin (4));
pack.SetBin (user);
pack.SetBin (time);
pack.SetHex ("00 00 00 00");
pack.SetHex ("00 00");

return tlv_pack ("00 01", pack.GetAll ());

}
vectorbyte tlv2(CStringA code,vectorbyte VieryToken1)
{

_Pack pack;

//' 00 02
//' 00 0E
//' 00 00 00 04
//' 74 77 79 76
//' 00 04
//' 4B 58 31 35
pack.Empty ();
pack.SetInt (code.GetLength());
pack.SetBin (char2vector(((CStringA)code).GetBuffer(0)));
pack.SetShort (VieryToken1.size());
pack.SetBin (VieryToken1);

return tlv_pack ("00 02", pack.GetAll ());

}
vectorbyte tlv106(vectorbyte user,vectorbyte md5pass,vectorbyte md52pass,vectorbyte _TGTKey,vectorbyte imei_,vectorbyte time,int appId)
{

_Pack pack;

//' 01 06
//' 00 70 [md5(md5(pass)+0 0 0 0+hexQQ)E8 FD 5B 08 BF 42 3C B9 F8 D4 23 30 F2 E2 E3 59 ]
//' 67 A4 4D 1D 59 08 97 15 92 03 BB E3 E8 7F 49 CC 65 A2 F6 E3 4F 68 DA 9E A2 E9 DA 90 DB 26 2D F5 A4 BD C0 52 51 F0 40 77 B5 50 25 42 AC 68 1B 57 35 61 97 65 36 6B AA 35 C5 E1 E6 C8 91 3B 3E 30 84 AA 6F 6C 32 29 97 FB DF 53 CA 3C B5 F8 F3 13 E4 FF AA 58 39 75 81 45 38 4A A2 BE CA 43 E0 7E 0A 83 71 17 5C 88 7C DE DE ED B8 12 E4 D5 C4 22
//' [
//' 00 03 //TGTGTVer=3
//' 29 A5 69 34 rand32
//' 00 00 00 05
//' 00 00 00 10
//' 00 00 00 00
//' 00 00 00 00
//' 18 B4 A1 BC [QQ:414491068]
//' 4D 1F C3 AC //时间
//' 00 00 00 00
//' 01
//' EB E0 80 63 34 8C 9E E1 FD 6B 5E 05 9A 72 84 C6 //MD5PASS
//' C5 2E 0F 5D A6 20 B5 EE 0B 94 F2 6F C3 05 4A 02 //TGTKey
//' 00 00 00
//' 00 01
//' 46 60 1E D3 C6 24 16 BF CA A2 9E 9E B8 9A D2 4E //imei_
//' 20 02 93 92 _sub_appid
//' 00 00 00 01 00 00
//' ]
//' time ＝Xbin.Flip 取字节集左边 (到字节集 (Other.TimeStamp ();), 4))

pack.SetHex ("00 03");
pack.SetBin (Xbin.GetRandomBin (4));
pack.SetHex ("00 00 00 05");
pack.SetHex ("00 00 00 10");
pack.SetHex ("00 00 00 00");
pack.SetHex ("00 00 00 00");
pack.SetBin (user);
pack.SetBin (time);
pack.SetHex ("00 00 00 00 01");
pack.SetBin (md5pass);
pack.SetBin (_TGTKey);
pack.SetHex ("00 00 00 00");
pack.SetHex ("01");
pack.SetBin (imei_);
int subappid=0x2002872E;//企业qqsub appid和appid不同，不知道有没有问题
if(g_bQQflag)
pack.SetInt (subappid);//appId
else
pack.SetInt (appId);

pack.SetHex ("00 00 00 01");
pack.SetHex ("00 00");

//' 调试输出 ("01 06", “TGTKey", 字节集到十六进制文本 (_TGTKey), 字节集到十六进制文本 (pack.GetAll ();))

return tlv_pack ("01 06", Hash.QQTEA (pack.GetAll (), md52pass));

}
vectorbyte tlv116()
{
_Pack pack;

//' 01 16
//' 00 0A
//' 00
//' 00 00 7F 7C /mMiscBitmap
//' 00 01 04 00 mSubSigMap
//' 00 _sub_appid_list.length
pack.Empty ();
pack.SetHex ("00");
pack.SetHex ("00 01 FF 7C");//01 FF 7C//00 7F 7C"); 2015.12.22
pack.SetHex ("00 01 04 00");
pack.SetHex ("01 5F 5E 10 E2");////("00");; 2015.12.22

return tlv_pack ("01 16", pack.GetAll ());

}
vectorbyte tlv100(int appId)
{

_Pack pack;

//' 01 00 //tlv-100
//' 00 16
//' 00 01 //_db_buf_ver=1
//' 00 00 00 05 //_sso_ver=5
//' 00 00 00 10 //appid
//' 20 02 92 13 //_sub_appid
//' 00 00 00 00 //_app_client_version
//' 00 0E 10 E0 //_main_sigmap

pack.Empty ();
pack.SetHex ("00 01");
pack.SetHex ("00 00 00 05");
pack.SetHex ("00 00 00 10");
int subappid=0x2002872E;//企业qqsub appid和appid不同，不知道有没有问题
if(g_bQQflag)
pack.SetInt (subappid);//appId
else
pack.SetInt (appId);
pack.SetHex ("00 00 00 00");
if(g_bQQflag)
pack.SetHex ("00 1E 10 E0");//2016.518//pack.SetHex ("02 1E 10 E0");////00 0E 10 E0 2015.12.22
else
pack.SetHex ("02 1E 10 E0");

return tlv_pack ("01 00", pack.GetAll ());

}
vectorbyte tlv104(vectorbyte VieryToken2)
{

_Pack pack;

//' 01 04
//' 00 14
//' 41 56 51 4F 77 56 78 48 58 78 54 64 43 6F 73 4A
//' 4D 6D 57 53 //VieryToken2
pack.Empty ();
pack.SetBin (VieryToken2);

return tlv_pack ("01 04", pack.GetAll ());

}
vectorbyte tlv107()
{
_Pack pack;

//' 01 07 //tlv-107
//' 00 06
//' 00 00 //_pic_type
//' 00 //0
//' 00 00 //0
//' 01 //1
pack.Empty ();
pack.SetHex ("00 00");
pack.SetHex ("00");
pack.SetHex ("00 00");
pack.SetHex ("01");

return tlv_pack ("01 07", pack.GetAll ());


}
vectorbyte tlv108(vectorbyte& _ksid)
{


//' 01 08 //tlv108
//' 00 10
//' 93 33 4E AD B8 08 D3 42 82 55 B7 EF 28 E7 E8 F5 //request_global._ksid
//_ksid.clear();
return tlv_pack ("01 08", _ksid);


}
vectorbyte tlv144(vectorbyte TGTKey,vectorbyte tlv109,vectorbyte tlv124,vectorbyte tlv128,vectorbyte tlv16e)
{

_Pack pack;

//' 01 44
//' 00 80 (////_tgtgt_key)
//' 60 17 BF D3 F7 A4 7E C5 BC 07 47 98 B3 9B 12 C1
//' CC F6 87 13 7A 28 BB 62 18 3B 1A 43 F8 FE 07 87
//' CB CF 40 3D BD DB 93 0F A7 CC F4 71 67 67 70 9E
//' 33 14 CD E6 D7 CA 62 B4 48 FB 32 21 47 8F 40 B5
//' A0 8E CB 5E 31 70 26 44 EA 79 AD A7 76 00 2A 26
//' 56 92 38 EA 78 BB CC 4E E8 E3 F4 CD FE 19 AB 32
//' A6 BB 31 72 D7 25 93 94 4A EF A7 94 A9 59 B2 73
//' 55 95 4C FC AD C4 1A C2 15 C6 8F A1 39 48 F8 1A
//' [
//' 00 04 //get_tlv_144四个参数byte[]都有数据
//' 01 09 //tlv-109
//' 00 10
//' 46 60 1E D3 C6 24 16 BF CA A2 9E 9E B8 9A D2 4E //_IMEI
//' 01 24 //tlv-124
//' 00 1C
//' 00 07 61 6E 64 72 6F 69 64 00 05 34 2E 30 2E 34
//' 00 01 00 00 00 00 00 04 77 69 66 69
//' 01 28 //tlv-128
//' 00 2B
//' 00 00 //0
//' 00 //request_global._new_install
//' 01 //request_global._read_guid
//' 00 //request_global._guid_chg
//' 01 00 00 00 //request_global._dev_report
//' 00 0C
//' 48 55 41 57 45 49 20 55 39 35 30 38 //request_global._device=HUAWEI U9508
//' 00 10
//' 46 60 1E D3 C6 24 16 BF CA A2 9E 9E B8 9A D2 4E //request_global._IMEI
//' 00 00 //0
//' 01 6E //tlv-16e
//' 00 0C
//' 48 55 41 57 45 49 20 55 39 35 30 38 //request_global._device=HUAWEI U9508
//' ]

pack.Empty ();
pack.SetShort (4);
pack.SetBin (tlv109);
pack.SetBin (tlv124);
pack.SetBin (tlv128);
pack.SetBin (tlv16e);

//' 调试输出 ("01 44", “TGTKey", 字节集到十六进制文本 (TGTKey), 字节集到十六进制文本 (pack.GetAll ();))
return tlv_pack ("01 44", Hash.QQTEA (pack.GetAll (), TGTKey));

}
vectorbyte tlv109(vectorbyte imei_)
{


//' 01 09 //tlv-109
//' 00 10
//' 46 60 1E D3 C6 24 16 BF CA A2 9E 9E B8 9A D2 4E //_IMEI
return tlv_pack ("01 09", imei_);


}
vectorbyte tlv124(CStringA os_type,CStringA os_version,int _network_type,CStringA _apn)
{

_Pack pack;

//' 01 24 //tlv-124
//' 00 1C
//' 00 07 //os_type
//' 61 6E 64 72 6F 69 64
//' 00 05 //os_version
//' 34 2E 30 2E 34
//' 00 01 //_network_type
//' 00 00 //._sim_operator_name
//' 00 00 //0
//' 00 04 //_apn
//' 77 69 66 69
pack.SetShort (os_type.GetLength());
pack.SetStr ((CStringA)os_type);
pack.SetShort (os_version.GetLength());
pack.SetStr ((CStringA)os_version);
pack.SetShort (_network_type);
pack.SetHex ("00 00");
pack.SetHex ("00 00");
pack.SetShort (_apn.GetLength());
pack.SetStr ((CStringA)_apn);

return tlv_pack ("01 24", pack.GetAll ());

}
vectorbyte tlv128(CStringA _device,vectorbyte imei_)
{

_Pack pack;

//' 01 28 //tlv-128
//' 00 2B
//' 00 00 //0
//' 00 //request_global._new_install
//' 01 //request_global._read_guid
//' 00 //request_global._guid_chg
//' 01 00 00 00 //request_global._dev_report
//' 00 0C
//' 48 55 41 57 45 49 20 55 39 35 30 38 //request_global._device=HUAWEI U9508
//' 00 10
//' 46 60 1E D3 C6 24 16 BF CA A2 9E 9E B8 9A D2 4E //request_global._IMEI
//' 00 00 //0
pack.Empty ();
pack.SetHex ("00 00");
pack.SetHex ("00");
pack.SetHex ("01");
pack.SetHex ("01");
pack.SetHex ("01 00 02 00");
pack.SetShort (_device.GetLength());
pack.SetStr ((CStringA)_device);
pack.SetShort (imei_.size());
pack.SetBin (imei_);
pack.SetHex ("00 00");

return tlv_pack ("01 28", pack.GetAll ());

}
vectorbyte tlv16e(CStringA _device)
{

_Pack pack;

//' 01 6E //tlv-16e
//' 00 0C
//' 48 55 41 57 45 49 20 55 39 35 30 38 //request_global._device=HUAWEI U9508
pack.Empty ();
pack.SetBin (char2vector((CStringA)_device));

return tlv_pack ("01 6E", pack.GetAll ());


}
vectorbyte tlv142(CStringA _apk_id)
{

_Pack pack;

//' 01 42 //tlv142
//' 00 16 //len
//' 00 00 00 12 //len
//' 63 6F 6D 2E 74 65 6E 63 65 6E 74 2E 71 71 6C 69
//' 74 65 //request_global._apk_id=com.tencent.qqlite
pack.Empty ();
vectorbyte tttemp=char2vector((CStringA)_apk_id);
pack.SetInt (tttemp.size());
pack.SetBin (tttemp);

return tlv_pack ("01 42", pack.GetAll ());

}
vectorbyte tlv154(int _sso_seq)
{

_Pack pack;

//' 01 54 //tlv-154
//' 00 04
//' 00 01 19 6A //this._g._sso_seq
pack.Empty ();
pack.SetInt (_sso_seq);

return tlv_pack ("01 54", pack.GetAll ());

}
vectorbyte tlv145(vectorbyte imei_)
{

_Pack pack;

//' 01 45 //tlv-145
//' 00 10
//' 46 60 1E D3 C6 24 16 BF CA A2 9E 9E B8 9A D2 4E //request_global._IMEI
pack.Empty ();
pack.SetBin (imei_);

return tlv_pack ("01 45", pack.GetAll ());


}
vectorbyte tlv141(int _network_type,CStringA _apn)
{

_Pack pack;

//' 01 41 //tlv-141
//' 00 0C
//' 00 01 // this._version=1
//' 00 00 //request_global._sim_operator_name
//' 00 01 //request_global._network_type
//' 00 04 //len
//' 77 69 66 69 // request_global._apn

pack.Empty ();
pack.SetHex ("00 01");
pack.SetHex ("00 07 41 6E 64 72 6F 69 64");
pack.SetShort (_network_type);
pack.SetShort (_apn.GetLength());
pack.SetStr ((CStringA)_apn);

return tlv_pack ("01 41", pack.GetAll ());

}
vectorbyte tlv8()
{
_Pack pack;

//' 00 08 //tlv-8
//' 00 08
//' 00 00 //0
//' 00 00 08 04 //request_global._local_id
//' 00 00 //0
pack.SetHex ("00 00 ");
pack.SetHex ("00 00 08 04");
pack.SetHex ("00 00");

return tlv_pack ("00 08", pack.GetAll ());


}
vectorbyte tlv16b()
{
_Pack pack;

//' 01 6B
//' 00 0F
//' 00 01
//' 00 0B
//' 67 61 6D 65 2E 71 71 2E 63 6F 6D [game.qq.com]

pack.Empty ();
pack.SetHex ("00 05");//2015.12.25 00 01
pack.SetHex ("00 0B");
pack.SetHex ("67 61 6D 65 2E 71 71 2E 63 6F 6D 00 0B 6D 61 69 6C 2E 71 71 2E 63 6F 6D 00 0C 71 7A 6F 6E 65 2E 71 71 2E 63 6F 6D 00 09 71 7A 6F 6E 65 2E 63 6F 6D 00 0A 71 75 6E 2E 71 71 2E 63 6F 6D");//201512.25 67 61 6D 65 2E 71 71 2E 63 6F 6D");

return tlv_pack ("01 6B", pack.GetAll ());

}
vectorbyte tlv147(CStringA _apk_v,vectorbyte _apk_sig)
{

_Pack pack;

//' 01 47//tlv-147
//' 00 1D
//' 00 00 00 10 //appid
//' 00 05
//' 33 2E 30 2E 30 // request_global._apk_v
//' 00 10
//' A6 B7 45 BF 24 A2 C2 77 52 77 16 F6 F3 6E B6 8D //request_global._apk_sig
pack.Empty ();
pack.SetHex ("00 00 00 10");
pack.SetShort (_apk_v.GetLength());
pack.SetStr ((CStringA)_apk_v);
pack.SetShort (_apk_sig.size());
pack.SetBin (_apk_sig);

return tlv_pack ("01 47", pack.GetAll ());


}
vectorbyte tlv177()
{
_Pack pack;

//' 01 77
//' 00 0E
//' 01
//' 53 FB 17 9B
//' 00 07
//' 35 2E 32 2E 33 2E 30 [5.2.3.0]
pack.Empty ();
pack.SetHex ("01");
if(g_bQQflag)
{
pack.SetHex ("54 28 CF 45");//企业qq 56 69 53 75");//55 FF C1 EF");//2015.12.25 "53 FB 17 9B");
pack.SetHex ("00 07");
pack.SetHex ("35 2E 32 2E 33 2E 30");//35 2E 34 2E 30 2E 37");//2015.12.25 35 2E 32 2E 33 2E 30");
}
else
{
pack.SetHex ("56 69 53 75");//企业qq 56 69 53 75");//55 FF C1 EF");//2015.12.25 "53 FB 17 9B");
pack.SetHex ("00 07");
pack.SetHex ("35 2E 34 2E 30 2E 37");//35 2E 34 2E 30 2E 37");//2015.12.25 35 2E 32 2E 33 2E 30");

}
return tlv_pack ("01 77", pack.GetAll ());

}
vectorbyte tlv194(vectorbyte _phone194sig)
{
_Pack pack;

//01 94 
//00 10
//5D 0F 45 DF A0 62 FB BC 01 6D 6E A0 CC 06 F3 0B 

pack.Empty ();
pack.SetHex ("5D 0F 45 DF A0 62 FB BC 01 6D 6E A0 CC 06 F3 0B");
//pack.SetBin (_phone194sig);
return tlv_pack ("01 94", pack.GetAll ());

}
vectorbyte tlv202(vectorbyte _phone202sig)
{
_Pack pack;

//02 02 
//00 1D

//00 10 CF 53 37 FA 93 03 09 D8 8A 12 5C EE 9F 9C EA DA 00 09 57 69 72 65 64 53 53 49 44 

pack.Empty ();
pack.SetHex ("00 10");
pack.SetHex ("CF 53 37 FA 93 03 09 D8 8A 12 5C EE 9F 9C EA DA");
//pack.SetBin (_phone202sig);
pack.SetHex ("00 09");//2015.12.25 "53 FB 17 9B");
pack.SetHex ("57 69 72 65 64 53 53 49 44");
return tlv_pack ("02 02", pack.GetAll ());

}
vectorbyte tlv400()
{
_Pack pack;

//02 02 
//00 1D
//00 10 CF 53 37 FA 93 03 09 D8 8A 12 5C EE 9F 9C EA DA 00 09 57 69 72 65 64 53 53 49 44 

pack.Empty ();
pack.SetHex ("00 48");
//pack.SetHex ("CF 53 37 FA 93 03 09 D8 8A 12 5C EE 9F 9C EA DA");
byte buf[0x48];
if(0)
	{
	CFile file1;
	if(file1.Open(::function::complete_path(_T("dfsdl.txt")),CFile::modeRead))
	{
		file1.Read(&buf,0x48);
	file1.Close();
	}
	}
pack.SetBin (buf,0x48);//"49 4D DE 34 90 74 E0 44 37 D6 9A 24 39 43 20 E7 38 80 69 49 E1 38 4A 51 0C EB E6 03 05 24 60 A2 12 19 C9 18 6A 05 A1 69 B9 E8 8A BC 57 8F BB CA 98 17 24 E6 5F E2 38 53 BF D5 94 2E CB A4 0E 69 D3 20 E3 3E 52 44 96 DD");
return tlv_pack ("04 00", pack.GetAll ());

}


vectorbyte tlv114_get0058(vectorbyte bin)
{

_Unpack unPack;
int len;

//' 00 01 54 09 99 81
//' 00 58 //下面的data
//' D1 48 33 52 6C F5 AA 2C 59 2E 7E E0 85 55 39 9E
//' 35 C9 93 1E 1E 71 3F B7 AF 0E 17 D1 C4 13 69 C4
//' A6 75 38 52 BF 0F F1 DA B1 61 B9 A0 39 14 F9 8D
//' 59 6A EB F2 B8 33 66 A1 9E A5 7C 61 0D 0F 28 B6
//' C2 7D 72 1B 5C A5 96 42 80 80 F1 22 DF 5A 8C B8
//' 4A 8E BB F3 56 B4 F4 13
unPack.SetData (bin);
unPack.GetBin (6);
len=unPack.GetShort ();
if(len != 88)
{
   // 调试输出 ("error tlv114_get0058 len!=0058");
//.如果真结束
}
return unPack.GetBin (len);

}
vectorbyte tlv187()
{
_Pack pack;

//' 01 87
//' 00 10
//' F8 FF 12 23 6E 0D AF 24 97 CE 7E D6 A0 7B DD 68

pack.Empty ();
if(g_bQQflag)
pack.SetHex ("A9 B1 31 D4 40 B6 57 C7 01 05 2A EA E8 DD B4");//F8 FF 12 23 6E 0D AF 24 97 CE 7E D6 A0 7B DD 68");
else
pack.SetHex ("F8 FF 12 23 6E 0D AF 24 97 CE 7E D6 A0 7B DD 68");
return tlv_pack ("01 87", pack.GetAll ());

}
vectorbyte tlv188()
{
_Pack pack;

//' 01 88
//' 00 10
//' 4D BF 65 33 D9 08 C2 73 63 6D E5 CD AE 83 C0 43

pack.Empty ();
if(g_bQQflag)
pack.SetHex ("07 A2 E5 5D CD 10 90 71 AC 42 48 9C 22 53 4C F9");//4D BF 65 33 D9 08 C2 73 63 6D E5 CD AE 83 C0 43");
else
	pack.SetHex ("4D BF 65 33 D9 08 C2 73 63 6D E5 CD AE 83 C0 43");
return tlv_pack ("01 88", pack.GetAll ());

}
vectorbyte tlv191()
{
_Pack pack;

//' 01 91
//' 00 01
//' 00

pack.Empty ();
pack.SetHex ("01");//2015.12.25 00

return tlv_pack ("01 91", pack.GetAll ());
}
};


extern _Tlv tlv;
extern qq_info qq;
extern qq_global global;
extern int pc_sub_cmd;
extern INT64 luin;
extern vector<JceStruct_FriendInfo> g_friendlist;
extern vector<JceStruct_GroupInfo> g_grouplist;
typedef CKeepMap<INT64,INT64,CString,LPCTSTR> CKeepINT64StrMap;
extern UINT appSeq;
extern UINT last_Join_appSeq;
extern CKeepMap<int,int,struct_group_msg,struct_group_msg> g_Seq_struct_group_msg_map;
extern void increase_ssoSeq ();
extern CStringA last_error;
extern CKeepMap<int,int,CEvent*,CEvent*> g_socketeventMap;
extern CKeepMap<int,int,CString,LPCTSTR> g_ssomd5postMap;
extern CKeepMap<INT64,INT64,int,int> g_group_membercountmap;
extern CKeepMap<INT64,INT64,INT64,INT64> g_group_ownerqqmap;
extern CKeepMap<INT64,INT64,CKeepINT64StrMap*,CKeepINT64StrMap*> GrouprealqqNickmap; 
extern CKeepMap<INT64,INT64,CKeepINT64StrMap*,CKeepINT64StrMap*> last_GrouprealqqNickmap; 

extern CKeepMap<INT64,INT64,CKeepINT64StrMap*,CKeepINT64StrMap*> Android_GrouprealqqNickmap;
extern CCriticalSection lockandroid_gqqnickmapresource;
extern CCriticalSection lockgqqnickmapresource;
extern CCriticalSection locksocketmapres;
extern CCriticalSection lockqqinformationres;
extern CCriticalSection lockggroupmembercountsource;
extern CCriticalSection lockggroupownerqqsource;

extern CCriticalSection lockgidgcoderesource;
