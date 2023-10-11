#include "http.h"
#include "网络协议提交.h"

char *到期时间 = NULL;
//-----------------------------------------@OQMODs---------------------------------------------
#ifndef MD5_H
#define MD5_H

typedef struct
{
  unsigned int count[2];
  unsigned int state[4];
  unsigned char buffer[64];
} MD5_CTX;

#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))
#define ROTATE_LEFT(x, n) ((x << n) | (x >> (32 - n)))
#define FF(a, b, c, d, x, s, ac) \
  {                              \
    a += F(b, c, d) + x + ac;    \
    a = ROTATE_LEFT(a, s);       \
    a += b;                      \
  }
#define GG(a, b, c, d, x, s, ac) \
  {                              \
    a += G(b, c, d) + x + ac;    \
    a = ROTATE_LEFT(a, s);       \
    a += b;                      \
  }
#define HH(a, b, c, d, x, s, ac) \
  {                              \
    a += H(b, c, d) + x + ac;    \
    a = ROTATE_LEFT(a, s);       \
    a += b;                      \
  }
#define II(a, b, c, d, x, s, ac) \
  {                              \
    a += I(b, c, d) + x + ac;    \
    a = ROTATE_LEFT(a, s);       \
    a += b;                      \
  }
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen);
void MD5Final(MD5_CTX *context, unsigned char digest[16]);
void MD5Transform(unsigned int state[4], unsigned char block[64]);
void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len);
void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len);

#endif

#include <string.h>

unsigned char PADDING[] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void MD5Init(MD5_CTX *context)
{
  context->count[0] = 0;
  context->count[1] = 0;
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
}
void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen)
{
  unsigned int i = 0, index = 0, partlen = 0;
  index = (context->count[0] >> 3) & 0x3F;
  partlen = 64 - index;
  context->count[0] += inputlen << 3;
  if (context->count[0] < (inputlen << 3))
    context->count[1]++;
  context->count[1] += inputlen >> 29;

  if (inputlen >= partlen)
  {
    memcpy(&context->buffer[index], input, partlen);
    MD5Transform(context->state, context->buffer);
    for (i = partlen; i + 64 <= inputlen; i += 64)
      MD5Transform(context->state, &input[i]);
    index = 0;
  }
  else
  {
    i = 0;
  }
  memcpy(&context->buffer[index], &input[i], inputlen - i);
}
void MD5Final(MD5_CTX *context, unsigned char digest[16])
{
  unsigned int index = 0, padlen = 0;
  unsigned char bits[8];
  index = (context->count[0] >> 3) & 0x3F;
  padlen = (index < 56) ? (56 - index) : (120 - index);
  MD5Encode(bits, context->count, 8);
  MD5Update(context, PADDING, padlen);
  MD5Update(context, bits, 8);
  MD5Encode(digest, context->state, 16);
}
void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len)
{
  unsigned int i = 0, j = 0;
  while (j < len)
  {
    output[j] = input[i] & 0xFF;
    output[j + 1] = (input[i] >> 8) & 0xFF;
    output[j + 2] = (input[i] >> 16) & 0xFF;
    output[j + 3] = (input[i] >> 24) & 0xFF;
    i++;
    j += 4;
  }
}
void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len)
{
  unsigned int i = 0, j = 0;
  while (j < len)
  {
    output[i] = (input[j]) |
                (input[j + 1] << 8) |
                (input[j + 2] << 16) |
                (input[j + 3] << 24);
    i++;
    j += 4;
  }//-----------------------------------------@OQMODs---------------------------------------------
}
void MD5Transform(unsigned int state[4], unsigned char block[64])
{
  unsigned int a = state[0];
  unsigned int b = state[1];
  unsigned int c = state[2];
  unsigned int d = state[3];
  unsigned int x[64];
  MD5Decode(x, block, 64);
  FF(a, b, c, d, x[0], 7, 0xd76aa478);
  FF(d, a, b, c, x[1], 12, 0xe8c7b756);
  FF(c, d, a, b, x[2], 17, 0x242070db);
  FF(b, c, d, a, x[3], 22, 0xc1bdceee);
  FF(a, b, c, d, x[4], 7, 0xf57c0faf);
  FF(d, a, b, c, x[5], 12, 0x4787c62a);
  FF(c, d, a, b, x[6], 17, 0xa8304613);
  FF(b, c, d, a, x[7], 22, 0xfd469501);
  FF(a, b, c, d, x[8], 7, 0x698098d8);
  FF(d, a, b, c, x[9], 12, 0x8b44f7af);
  FF(c, d, a, b, x[10], 17, 0xffff5bb1);
  FF(b, c, d, a, x[11], 22, 0x895cd7be);
  FF(a, b, c, d, x[12], 7, 0x6b901122);
  FF(d, a, b, c, x[13], 12, 0xfd987193);
  FF(c, d, a, b, x[14], 17, 0xa679438e);
  FF(b, c, d, a, x[15], 22, 0x49b40821);

  GG(a, b, c, d, x[1], 5, 0xf61e2562);
  GG(d, a, b, c, x[6], 9, 0xc040b340);
  GG(c, d, a, b, x[11], 14, 0x265e5a51);
  GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
  GG(a, b, c, d, x[5], 5, 0xd62f105d);
  GG(d, a, b, c, x[10], 9, 0x2441453);
  GG(c, d, a, b, x[15], 14, 0xd8a1e681);
  GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
  GG(a, b, c, d, x[9], 5, 0x21e1cde6);
  GG(d, a, b, c, x[14], 9, 0xc33707d6);
  GG(c, d, a, b, x[3], 14, 0xf4d50d87);
  GG(b, c, d, a, x[8], 20, 0x455a14ed);
  GG(a, b, c, d, x[13], 5, 0xa9e3e905);
  GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
  GG(c, d, a, b, x[7], 14, 0x676f02d9);
  GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

  HH(a, b, c, d, x[5], 4, 0xfffa3942);
  HH(d, a, b, c, x[8], 11, 0x8771f681);
  HH(c, d, a, b, x[11], 16, 0x6d9d6122);
  HH(b, c, d, a, x[14], 23, 0xfde5380c);
  HH(a, b, c, d, x[1], 4, 0xa4beea44);
  HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
  HH(c, d, a, b, x[7], 16, 0xf6bb4b60);
  HH(b, c, d, a, x[10], 23, 0xbebfbc70);
  HH(a, b, c, d, x[13], 4, 0x289b7ec6);
  HH(d, a, b, c, x[0], 11, 0xeaa127fa);
  HH(c, d, a, b, x[3], 16, 0xd4ef3085);
  HH(b, c, d, a, x[6], 23, 0x4881d05);
  HH(a, b, c, d, x[9], 4, 0xd9d4d039);
  HH(d, a, b, c, x[12], 11, 0xe6db99e5);
  HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
  HH(b, c, d, a, x[2], 23, 0xc4ac5665);

  II(a, b, c, d, x[0], 6, 0xf4292244);
  II(d, a, b, c, x[7], 10, 0x432aff97);
  II(c, d, a, b, x[14], 15, 0xab9423a7);
  II(b, c, d, a, x[5], 21, 0xfc93a039);
  II(a, b, c, d, x[12], 6, 0x655b59c3);
  II(d, a, b, c, x[3], 10, 0x8f0ccc92);
  II(c, d, a, b, x[10], 15, 0xffeff47d);
  II(b, c, d, a, x[1], 21, 0x85845dd1);
  II(a, b, c, d, x[8], 6, 0x6fa87e4f);
  II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
  II(c, d, a, b, x[6], 15, 0xa3014314);
  II(b, c, d, a, x[13], 21, 0x4e0811a1);
  II(a, b, c, d, x[4], 6, 0xf7537e82);
  II(d, a, b, c, x[11], 10, 0xbd3af235);
  II(c, d, a, b, x[2], 15, 0x2ad7d2bb);
  II(b, c, d, a, x[9], 21, 0xeb86d391);
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
}
//-----------------------------------------@OQMODs---------------------------------------------
static char find_pos(char ch);

const char *getHEX(const char *string)
{
    char chs;
    char *ret;
    char *str;
    if (!string || (ret = str = (char *)malloc(strlen(string) * 2 + 1)) == NULL)
        return NULL;
    while (*string)
    {
        chs = (*string & 0XF0) >> 4;
        if (chs > 9)
            *str = chs - 10 + 'A'; //chs - 10 + 'A'
        else
            *str = chs + '0';
        str++;
        chs = *string & 0X0F;
        if (chs > 9)
            *str = chs - 10 + 'A'; //chs - 10 + 'A'
        else
            *str = chs + '0';
        str++;
        string++;
    }
    *str = '\0';
    return ret;
}

//-----------------------------------------@OQMODs---------------------------------------------
char *加64(const char* data) 
{ 
    int data_len = strlen(data); 
    int prepare = 0; 
    int ret_len; 
    int temp = 0; 
    char *ret = NULL; 
    char *f = NULL; 
    int tmp = 0; 
    char changed[4]; 
    int i = 0; 
    ret_len = data_len / 3; 
    temp = data_len % 3; 
    if (temp > 0) 
    { 
        ret_len += 1; 
    } 
    ret_len = ret_len*4 + 1; 
    ret = (char *)malloc(ret_len); 
      
    if ( ret == NULL) 
    { 
        printf("No enough memory.\n"); 
        exit(0); 
    } 
    memset(ret, 0, ret_len); 
    f = ret; 
    while (tmp < data_len) 
    { 
        temp = 0; 
        prepare = 0; 
        memset(changed, '\0', 4); 
        while (temp < 3) 
        { 
            //printf("tmp = %d\n", tmp); 
            if (tmp >= data_len) 
            { 
                break; 
            } 
            prepare = ((prepare << 8) | (data[tmp] & 0xFF)); 
            tmp++; 
            temp++; 
        } 
        prepare = (prepare<<((3-temp)*8)); 
        //printf("before for : temp = %d, prepare = %d\n", temp, prepare); 
        for (i = 0; i < 4 ;i++ ) 
        { 
            if (temp < i) 
            { 
                changed[i] = 0x40; 
            } 
            else 
            { 
                changed[i] = (prepare>>((3-i)*6)) & 0x3F; 
            } 
            *f = BQAQSQEQ[changed[i]]; 
            //printf("%.2X", changed[i]); 
            f++; 
        } 
    } 
    *f = '\0'; 
    char *a[3]={"","==","="};
    sprintf(ret,"%s%s",ret,a[strlen(data)%3]);
    return ret; 
      
} 
/* */ 
static char find_pos(char ch)   
{ 
    char *ptr = (char*)strrchr(BQAQSQEQ, ch);//the last position (the only) in base[] 
    return (ptr - BQAQSQEQ); 
} 
/* */ 
char *解64(const char *data) 
{ 
    int data_len = strlen(data); 
    int ret_len = (data_len / 4) * 3; 
    int equal_count = 0; 
    char *ret = NULL; 
    char *f = NULL; 
    int tmp = 0; 
    int temp = 0; 
    char need[3]; 
    int prepare = 0; 
    int i = 0; 
    if (*(data + data_len - 1) == '=') 
    { 
        equal_count += 1; 
    } 
    if (*(data + data_len - 2) == '=') 
    { 
        equal_count += 1; 
    } 
    if (*(data + data_len - 3) == '=') 
    {//seems impossible 
        equal_count += 1; 
    } 
    switch (equal_count) 
    { 
    case 0: 
        ret_len += 4;//3 + 1 [1 for NULL] 
        break; 
    case 1: 
        ret_len += 4;//Ceil((6*3)/8)+1 
        break; 
    case 2: 
        ret_len += 3;//Ceil((6*2)/8)+1 
        break; 
    case 3: 
        ret_len += 2;//Ceil((6*1)/8)+1 
        break; 
    } 
    ret = (char *)malloc(ret_len); 
    if (ret == NULL) 
    { 
        printf("No enough memory.\n"); 
        exit(0); 
    } 
    memset(ret, 0, ret_len); 
    f = ret; 
    while (tmp < (data_len - equal_count)) 
    { 
        temp = 0; 
        prepare = 0; 
        memset(need, 0, 4); 
        while (temp < 4) 
        { 
            if (tmp >= (data_len - equal_count)) 
            { 
                break; 
            } 
            prepare = (prepare << 6) | (find_pos(data[tmp])); 
            temp++; 
            tmp++; 
        } 
        prepare = prepare << ((4-temp) * 6); 
        for (i=0; i<3 ;i++ ) 
        { 
            if (i == temp) 
            { 
                break; 
            } 
            *f = (char)((prepare>>((2-i)*8)) & 0xFF); 
            f++; 
        } 
    } 
    *f = '\0'; 
    return ret; 
}


//-----------------------------------------@OQMODs---------------------------------------------
char *itoa(int num, char *str, int radix)
{
    char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;
    int i = 0, j, k;                
    if (radix == 10 && num < 0)
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
        unum = (unsigned)num; 
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;

    } while (unum);

    str[i] = '\0';

    //将顺序调整过来
    if (str[0] == '-')
        k = 1;
    else
        k = 0;

    char temp;
    for (j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }

    return str;
}


const char *获取时间戳( char sj[13])
{
        time_t t = time(NULL);
        struct tm *mtime = localtime(&t);
        //char sj[13];  
        char *sjzz = sj;
        itoa(mtime->tm_year + 1900, sjzz, 10);
        sjzz += 4;
        if (mtime->tm_mon + 1 < 10)
        { //月
            *sjzz = '0';
            sjzz++;
            itoa(mtime->tm_mon + 1, sjzz, 10);
            sjzz++;
        }
        else
        {
            itoa(mtime->tm_mon + 1, sjzz, 10);
            sjzz += 2;
        }
        if (mtime->tm_mday < 10)
        { //日
            *sjzz = '0';
            sjzz++;
            itoa(mtime->tm_mday, sjzz, 10);
            sjzz++;
        }
        else
        {
            itoa(mtime->tm_mday, sjzz, 10);
            sjzz += 2;
        }
        if (mtime->tm_hour < 10)
        { //小时
            *sjzz = '0';
            sjzz++;
            itoa(mtime->tm_hour, sjzz, 10);
            sjzz++;
        }
        else
        {
            itoa(mtime->tm_hour, sjzz, 10);
            sjzz += 2;
        }
        if (mtime->tm_min < 10)
        { //分钟
            *sjzz = '0';
            sjzz++;
            itoa(mtime->tm_min, sjzz, 10);
            sjzz++;
        }
        else
        {
            itoa(mtime->tm_min, sjzz, 10);
            sjzz += 2;
        }
    return sj;
}

char *获取MD5(char yz[128],char yzmd5[32])
{
    MD5_CTX md5c;
    MD5Init(&md5c);     
    unsigned char decrypt[16];
    MD5Update(&md5c, (unsigned char*)yz, strlen(yz));
    MD5Final(&md5c, decrypt);
    //char yzmd5[32] = {0};
    for (int i = 0; i < 16; i++)
    {
        sprintf(&yzmd5[i * 2], "%02x", decrypt[i]);
    }
    return yzmd5;       
}//-----------------------------------------@OQMODs---------------------------------------------