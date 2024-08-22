#pragma once
typedef __int32 int32;
class Crypt 
{
// 构造
public:
	Crypt();	// 标准构造函数
~Crypt();  
 
  
static void teaEncipher(const unsigned int *const v, const unsigned int *const k, unsigned int *const w); 
static void teaDecipher(const unsigned int *const v, const unsigned int *const k, unsigned int *const w); 
static void encrypt(const unsigned char* instr, int32 instrlen, const unsigned char* key,unsigned char* outstr, int32* outstrlen_ptr);  
static int rand(void);
static int decrypt(const unsigned char* instr, int32 instrlen, const unsigned char* key, unsigned char*  outstr, int32* outstrlen_ptr);  
static int tencentt ( unsigned char   flag, const unsigned char*  instr,  int32  instrlen,const unsigned char*  key, unsigned char*  outstr, int32* outstrlen_ptr);
};