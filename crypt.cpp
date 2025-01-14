#include "stdafx.h"  
#include "crypt.h"  
#include <string.h>  
#include <cstring>  
Crypt::Crypt()  
{  
}  
  
Crypt::~Crypt()  
{  
}  

void Crypt::teaEncipher(const unsigned int *const v, const unsigned int *const k,  
            unsigned int *const w)  
{  
    register unsigned int  
        y     = ntohl(v[0]),  
        z     = ntohl(v[1]),  
        a     = ntohl(k[0]),  
        b     = ntohl(k[1]),  
        c     = ntohl(k[2]),  
        d     = ntohl(k[3]),  
        n     = 0x10,       /* do encrypt 16 (0x10) times */  
        sum   = 0,  
        delta = 0x9E3779B9; /*  0x9E3779B9 - 0x100000000 = -0x61C88647 */  
  
    while (n-- > 0) {  
        sum += delta;  
        y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);  
        z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);  
    }  
  
    w[0] = htonl(y); w[1] = htonl(z);  

}  
void Crypt::teaDecipher(const unsigned int *const v, const unsigned int *const k,  
            unsigned int *const w)  
{  
    register unsigned int  
        y     = ntohl(v[0]),  
        z     = ntohl(v[1]),  
        a     = ntohl(k[0]),  
        b     = ntohl(k[1]),  
        c     = ntohl(k[2]),  
        d     = ntohl(k[3]),  
        n     = 0x10,  
        sum   = 0xE3779B90,  
        /* why this ? must be related with n value*/  
        delta = 0x9E3779B9;  
  
    /* sum = delta<<5, in general sum = delta * n */  
    while (n-- > 0) {  
        z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);  
        y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);  
        sum -= delta;  
    }  
  
    w[0] = htonl(y); w[1] = htonl(z);  
}  
  
void Crypt::encrypt(const unsigned char* instr, int32 instrlen,const unsigned char* key,  
            unsigned char*  outstr, int32* outstrlen_ptr)  
{  
    unsigned char  
        plain[8],         /* plain text buffer*/  
        plain_pre_8[8],   /* plain text buffer, previous 8 bytes*/  
        * crypted,        /* crypted text*/  
        * crypted_pre_8;  /* crypted test, previous 8 bytes*/  
      const unsigned char * inp;            /* current position in instr*/  
    int  
        pos_in_byte = 1,  /* loop in the byte */  
        is_header=1,      /* header is one byte*/  
        count=0,          /* number of bytes being crypted*/  
        padding = 0;      /* number of padding stuff*/  
  
    //void encrypt_every_8_byte (void);  
  
      
    /*** we encrypt every eight byte ***/  
#define encrypt_every_8_byte()  \
    {\
    for(pos_in_byte=0; pos_in_byte<8; pos_in_byte++) {\
    if(is_header) { plain[pos_in_byte] ^= plain_pre_8[pos_in_byte]; }\
   else { plain[pos_in_byte] ^= crypted_pre_8[pos_in_byte]; }\
    }\
    teaEncipher( (unsigned int *) plain,\
	(unsigned int *) key, \
    (unsigned int *) crypted);   /* encrypt it*/\
    \
    for(pos_in_byte=0; pos_in_byte<8; pos_in_byte++) {\
    crypted[pos_in_byte] ^= plain_pre_8[pos_in_byte]; \
    } \
    memcpy(plain_pre_8, plain, 8);     /* prepare next*/\
    \
    crypted_pre_8   =   crypted;       /* store position of previous 8 byte*/\
    crypted         +=  8;             /* prepare next output*/\
    count           +=  8;             /* outstrlen increase by 8*/\
    pos_in_byte     =   0;             /* back to start*/\
    is_header       =   0;             /* and exit header*/\
    }/* encrypt_every_8_byte*/  
  
      
    pos_in_byte = (instrlen + 0x0a) % 8; /* header padding decided by instrlen*/  
    if (pos_in_byte) {  
        pos_in_byte = 8 - pos_in_byte;  //求出要padding多少字节  
    }  
    plain[0] = (rand() & 0xf8) | pos_in_byte; //头部第一个字节  
  
    memset(plain+1, rand()&0xff, pos_in_byte++); //padding n 个字节  
    memset(plain_pre_8, 0x00, sizeof(plain_pre_8)); //前8个字节清零  
  
    crypted = crypted_pre_8 = outstr;  
  
    padding = 1; /* pad some stuff in header*/    //pad 2 bytes  
    while (padding <= 2) { /* at most two byte */  
        if(pos_in_byte < 8) { plain[pos_in_byte++] = rand() & 0xff; padding ++; }  
        if(pos_in_byte == 8){
			encrypt_every_8_byte(); 
		}  
    }  
  
    inp = instr;  
    while (instrlen > 0) {  
        if (pos_in_byte < 8) { plain[pos_in_byte++] = *(inp++); instrlen --; } //复制明文至待加密数据区  
        if (pos_in_byte == 8){ encrypt_every_8_byte(); } //满8个字节则加密  
    }  
  
    padding = 1; /* pad some stuff in tailer*/           //pad 7 bytes 00  
    while (padding <= 7) { /* at most sever byte*/  
        if (pos_in_byte < 8) { plain[pos_in_byte++] = 0x00; padding ++; }  
        if (pos_in_byte == 8){ encrypt_every_8_byte(); }  
    }  
  
    *outstrlen_ptr = count;  
  
}  
  
int Crypt::rand(void)  
{  
    /* it can be the real random seed function*/  
	return ::rand();
    //return 0xad; /* override with number, convenient for debug*/  
}  
  
int Crypt::decrypt(const unsigned char* instr, int32 instrlen,const unsigned char* key,  
            unsigned char*  outstr, int32* outstrlen_ptr)  
{  
    unsigned char  
        decrypted[8], m[8],  
             * outp; 
	const unsigned char * crypt_buff_pre_8;
	const unsigned char* crypt_buff;
    int  
        count,           //明文数据长度  
        context_start,  //长度临时变量  
        pos_in_byte,  
        padding;  
  
#define decrypt_every_8_byte()  {\
    bool bNeedRet = false;\
    for (pos_in_byte = 0; pos_in_byte < 8; pos_in_byte ++ ) {\
        if (context_start + pos_in_byte >= instrlen) \
        {\
            bNeedRet = true;\
            break;\
        }\
        decrypted[pos_in_byte] ^= crypt_buff[pos_in_byte];\
    }\
    if( !bNeedRet ) { \
        teaDecipher( (unsigned int *) decrypted, \
        (unsigned int *) key, \
        (unsigned int *) decrypted);\
    \
        context_start +=  8;\
        crypt_buff    +=  8;\
        pos_in_byte   =   0;\
    }\
}/* decrypt_every_8_byte*/  
  
    /* at least 16 bytes and %8 == 0*/  
    if ((instrlen % 8) || (instrlen < 16)) return 0;  
    /* get information from header*/  
    teaDecipher( (unsigned int *) instr,  
        (unsigned int *) key,  
        (unsigned int *) decrypted);   //解出头部，然后decrypted缓冲区就有初值了  
    pos_in_byte = decrypted[0] & 0x7;  
    count = instrlen - pos_in_byte - 10; /* this is the plaintext length*/  
    /* return if outstr buffer is not large enought or error plaintext length*/  
   // if (*outstrlen_ptr < count || count < 0) return 0;  //没必要考虑返回的缓冲区大小
   if ( count < 0) return 0;  
  
    memset(m, 0, 8);  
    crypt_buff_pre_8 = m;  
    *outstrlen_ptr = count;   /* everything is ok! set return string length*/  
  
    crypt_buff = instr + 8;   /* address of real data start */  
    context_start = 8;        /* context is at the second 8 byte*/  
    pos_in_byte ++;           /* start of paddng stuffv*/  
  
    padding = 1;              /* at least one in header*/  
    while (padding <= 2) {    /* there are 2 byte padding stuff in header*/  
        if (pos_in_byte < 8) {  /* bypass the padding stuff, none sense data*/  
            pos_in_byte ++; padding ++;  
        }  
        if (pos_in_byte == 8) {  
            crypt_buff_pre_8 = instr;  
            //if (! decrypt_every_8_byte()) return 0;  
            decrypt_every_8_byte();  
        }  
    }/* while*/  
  
    outp = outstr;  
    while(count !=0) {  
        if (pos_in_byte < 8) {  
            *outp = crypt_buff_pre_8[pos_in_byte] ^ decrypted[pos_in_byte]; //输出明文等于前一个密文单元数据异或当前解密后单元  
            outp ++;  
            count --;  
            pos_in_byte ++;  
        }  
        if (pos_in_byte == 8) {  
            crypt_buff_pre_8 = crypt_buff - 8;  
            //if (! decrypt_every_8_byte()) return 0;  
            decrypt_every_8_byte();  
        }  
    }/* while*/  
  
    for (padding = 1; padding < 8; padding ++) {  
        if (pos_in_byte < 8) {  
            if (crypt_buff_pre_8[pos_in_byte] ^ decrypted[pos_in_byte]) return 0; //若是和0异或的话则说明已解密完毕了  
            pos_in_byte ++;  
        }  
        if (pos_in_byte == 8 ) {  
            crypt_buff_pre_8 = crypt_buff;  
            //if (! decrypt_every_8_byte()) return 0;  
            decrypt_every_8_byte();  
        }  
    }/* for*/  
    return 1;  
  
}  
  
  
int Crypt::tencentt ( unsigned char   flag,const  unsigned char*  instr,  int32  instrlen,  
           const unsigned char*  key, unsigned char*  outstr, int32* outstrlen_ptr)  
{  
    if (flag == 2)  
        return decrypt(instr, instrlen, key, outstr, outstrlen_ptr);  
    else  
        if (flag == 1)  
            encrypt(instr, instrlen, key, outstr, outstrlen_ptr);  
  
    return 1; /* flag must be DECRYPT or ENCRYPT*/  
}