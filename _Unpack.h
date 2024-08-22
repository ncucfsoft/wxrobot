#pragma once
class _Unpack
{

public:
	int m_pos;
	_Unpack();
	vector<byte> m_bin;
 int GetInt();
 short GetShort();
 UINT64 GetLong();
 vectorbyte GetAll();
 vectorbyte AndroidGetToken();
 vectorbyte GetShortToken();
 CStringA GetAll_Hex();
 void SetData_Hex(char* hexstr);
 void SetData(vectorbyte value);
 CString _Unpack::GetTokenstr(CString *utfword=NULL);
 CString _Unpack::GetTokenstr2();
 byte GetByte();
 vectorbyte GetToken();
 
 vectorbyte GetBin(int len);
};
  