#pragma once
class _Pack
{
	
vector<byte> m_bin;
public:
 void Empty();
 void SetInt(int value);
 void SetShort(short value);
 void SetInt64(INT64 value);
  void SetLong(INT64 value);
 const_vectorbyte& GetAll();
 void SetHex(const char* hexstr);
 void SetByte(byte bvalue);
 void AndroidSetString(const_vectorbyte& str);
 void SetToken(const_vectorbyte& value);
 void SetToken(const TCHAR* value);
void AndroidSetShortToken(const_vectorbyte& value);
void AndroidSetShortString(const_vectorbyte& value);
 void SetStr(const char* str);
  void SetData(const_vectorbyte& value);

 void SetBin(byte* addbin,int len);
 void SetBin(const_vectorbyte& addbin);
 
};
  