/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/** Private includes ---------------------------------------------------------*/
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
/** Private defines ----------------------------------------------------------*/
#ifndef OUT_FILE_NAME
	#define OUT_FILE_NAME "default.file"
#endif
/** Exported typedefines -----------------------------------------------------*/

/*文件打开权限*/
typedef enum
{
	READ_ONLY = 0,
	READ_WRITE_ONLY,
	WRITE_CREAT_CLEAR,
	READ_WRITE_CREAT_CLEAR,
	WRITE_APPEND_CREAT,
	READ_WRITE_APPEND_CREAT,
	READ_WRITE_APPEND_CREAT_FILE_B,
}FILE_OPEN_MODE;

/*文件分割方式*/
typedef enum
{
	SPACE_SPLIT = 0,
	COMMA_SPLIT,
}FILE_SPLIT_MODE;

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
#define PRINT_ERRMSG(STR) fprintf(stderr,"line:%d,msg:%s,eMsg:%s\n", __LINE__, STR, strerror(errno))
#define GET_U16_HI_BYTE(data)   ((uint8_t)((data>>8)&0x00FF))/**< 获得u16数据高字节*/
#define GET_U16_LOW_BYTE(data)  ((uint8_t)(data&0x00FF))/**< 获得u16数据低字节*/
#define GET_U32_HI_HALF_WORD(data)  ((uint16_t)((data>>16)&0xFFFF))/**< 获得u32数据高半字*/
#define GET_U32_LOW_HALF_WORD(data)  ((uint16_t)(data&0xFFFF))/**< 获得u32数据低半字*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
/*返回指定文件是否存在*/
int file_is_exist(const char *fimename);

/*打开指定文件返回文件描述符*/
FILE *file_open(const char *filename ,FILE_OPEN_MODE mode);

/*读取指定打开的文件，返回总行数*/
int file_get_line_cnt(const char *filename);

/*读取指定打开的文件指定行的内容到缓冲区*/
size_t file_read(const char *filename ,char *destbuf ,size_t size ,int linenum);

/*替换字符*/
size_t file_replace_ch(char *sourcebuf ,char sourcech,char destch);

/*清除字符串空格*/
char *strtriml(char *pstr);
char *strtrimr(char *pstr);
char *strtrim(char *pstr);

/*清除文本中空格，忽略注释标识行*/
bool file_strip_comments(char *string, char comment);
/**
  ******************************************************************
  * @brief   判断文件是否存在
  * @param   [in]fimename 文件名
  * @retval  返回0文件存在
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
int file_is_exist(const char *fimename)
{
	return access(fimename ,F_OK | W_OK | R_OK);
}

/**
  ******************************************************************
  * @brief   打开指定文件返回文件描述符,追加模式下fseek(fp, 0, SEEK_SET)无效
  * @param   [in]fimename 文件名
  * @param   [in]mode 打开文件的模式选择
  * @retval  返回0文件存在
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
FILE *file_open(const char *filename ,FILE_OPEN_MODE mode)
{
	FILE *fp = NULL;
	switch(mode)
	{
	case READ_ONLY:
		fp = fopen(filename ,"r");
		break;
	case READ_WRITE_ONLY:
		fp = fopen(filename ,"r+");
		break;
	case WRITE_CREAT_CLEAR:
		fp = fopen(filename ,"w");
		break;
	case READ_WRITE_CREAT_CLEAR:
		fp = fopen(filename ,"w+");
		break;
	case WRITE_APPEND_CREAT:
		fp = fopen(filename ,"a");
		break;
	case READ_WRITE_APPEND_CREAT:
		fp = fopen(filename ,"a+");/**< 首次读取时，从文件头部开始读*/
		break;
	case READ_WRITE_APPEND_CREAT_FILE_B:
		fp = fopen(filename, "a+b");/**< 首次读取时，从文件头部开始读*/
		break;
	}
	return fp;
}

/**
  ******************************************************************
  * @brief   打开读取指定打开的文件，返回总行数
  * @param   [in]fp 文件指针
  * @param   [in]filename 文件名称
  * @retval  返回-1读取失败
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
int file_get_line_cnt(const char *filename)
{
	int cnt = 0;
	char buf[256];
	FILE *fp = file_open(filename ,READ_ONLY);
	if(fp == NULL)
	{
		PRINT_ERRMSG("fopen");
		printf("read file name :%s error.\n" ,filename);
		return -1;
	}

	char *ret = NULL;
	/*读取文件流中的内容*/
	while((fgets(buf ,256 ,fp)) != NULL)
	{
		ret = strchr(buf ,'\n');
		if(ret != NULL)
		{
			cnt++;
		}
	}

	/*关闭文件*/
	fclose(fp);
	return cnt;
}

/**
  ******************************************************************
  * @brief   读取指定打开的文件指定行的内容到缓冲区
  * @param   [in]filename 文件名称
  * @param   [in]读取到的数据存储区
  * @param   [in]限制长度
  * @param 	 [in]需读取的行
  * @retval  执行结果，读取到字节数
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
size_t file_read(const char *filename ,char *destbuf ,size_t size ,int linenum)
{
	int cnt = 0;
	char buf[1024];

	/*初始化缓冲区*/
	if(strlen(destbuf) > 0)
	{
		destbuf[0] = '\0';
	}

	/*打开文件流*/
	FILE *fp = file_open(filename ,READ_ONLY);
	if(fp == NULL)
	{
		PRINT_ERRMSG("fopen");
		return -1;
	}

	/*读取文件流中的内容*/
	char *ret = NULL;
	size_t len = 0;
	while((fgets(buf ,1024 ,fp)) != NULL)
	{
		ret = strchr(buf ,'\n');
		if(ret != NULL)
		{
			cnt++;
			if(cnt == linenum)
			{
				if(len == 0)
				{
					strncpy(destbuf ,buf ,1024);
					if(size > 1024)
					{
						destbuf[1024] = '\0';
					}
					else
					{
						destbuf[1023] = '\0';
					}
				}
				else
				{
					/*追加字符串*/
					len += strlen(buf);
					if(len > size)
					{
						break;
					}
					else
					{
						strcat(destbuf ,buf);
					}
				}
				fclose(fp);
				return strlen(destbuf);
			}//cnt == linenum
		}
		else
		{
			/*判断是否超出缓冲区大小*/
			if(cnt == linenum-1)
			{
				/*目标缓冲区过小直接退出*/
				if(size <= 1024)
				{
					break;
				}
				len = strlen(destbuf);
				if(len == 0)
				{
					strncpy(destbuf ,buf ,1024);
					destbuf[1024] = '\0';
				}
				else
				{
					/*追加字符串*/
					len += strlen(buf);
					if(len > size)
					{
						break;
					}
					else
					{
						strcat(destbuf ,buf);
					}
				}
			}
		}
	}
	fclose(fp);
	return strlen(destbuf);
}

/**
  ******************************************************************
  * @brief   写入指定的内容到文件
  * @param   [in]filename 文件名称
  * @param   [in]buffer数据存储区
  * @param   [in]size写入的元素占总字节数
  * @param 	 [in]count写入元素数目
  * @param 	 [in]mode文件写入模式
  * @retval  执行结果，写入元素的总数
  * @author  aron566
  * @version V1.0
  * @date    2020-10-09
  ******************************************************************
  */
size_t file_write(const char *filename ,const void* buffer ,size_t size ,size_t count ,FILE_OPEN_MODE mode)
{
	/*打开文件流*/
	FILE *fp = file_open(filename ,mode);
	if(fp == NULL)
	{
		return 0;
	}
	size_t cnt = fwrite(buffer ,size ,count ,fp);
	/*同步到文件中*/
	fflush(fp);
	fclose(fp);
	return cnt;
}

/**
  ******************************************************************
  * @brief   读取文件内容到指定的存储区
  * @param   [in]filename 文件名称
  * @param   [in]buffer数据存储区
  * @param   [in]size写入的元素数据结构占总字节数
  * @param 	 [in]count写入元素数目
  * @param 	 [in]mode文件写入模式
  * @return  执行结果，读取元素的总数
  * @author  aron566
  * @version V1.0
  * @date    2020-12-15
  ******************************************************************
  */
size_t file_read_with_mode(const char *filename, void* buffer, size_t size, size_t count, FILE_OPEN_MODE mode)
{
  FILE *fp = file_open(filename, mode);
  if(fp == NULL)
  {
    return 0;
  }
  return fread(buffer, size, count, fp);
}

/**
  ******************************************************************
  * @brief   替换目标字符串中字符
  * @param   [in]目标字符串
  * @param   [in]源字符
  * @param   [in]目标字符
  * @retval  执行结果，当前字符长度
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
size_t file_replace_ch(char *sourcebuf ,char sourcech,char destch)
{
    int i;
    i = strlen(sourcebuf) - 1;
    while (sourcebuf[i] == sourcech && (i >= 0))
    	sourcebuf[i--] = destch;
    return strlen(sourcebuf);
}

/**
  ******************************************************************
  * @brief   去除字符串右端空格
  * @param   [in]字符串指针
  * @retval  修剪后的字符串地址
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
char *strtrimr(char *pstr)
{
    int i;
    i = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}

/**
  ******************************************************************
  * @brief   去除字符串左端空格
  * @param   [in]字符串指针
  * @retval  修剪后的字符串地址
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
char *strtriml(char *pstr)
{
    int i = 0,j;
    j = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i <= j))
        i++;
    if (0<i)
        strcpy(pstr, &pstr[i]);
    return pstr;
}

/**
  ******************************************************************
  * @brief   去除字符串两端空格
  * @param   [in]字符串指针
  * @retval  修剪后的字符串地址
  * @author  aron566
  * @version V1.0
  * @date    2020-08-28
  ******************************************************************
  */
char *strtrim(char *pstr)
{
    char *p;
    p = strtrimr(pstr);
    return strtriml(p);
}

/**
  ******************************************************************
  * @brief   去掉字符串内所有空白，且忽略注释部分，最终得到没有空白的字符串
  * @param   [in]string：字符串
  * @param   [in]comment：注释标识
  * @retval  true表示数据可用
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
bool file_strip_comments(char *string, char comment)
{
  if (NULL == string || '\n' == *string || '\r' == *string) {
    return false; /* 第一个字符为回车或换行,表示空行 */
  }

  char *p, *q;
  /* 下面去掉字符串中所有空白字符 */
  for (p = q = string; *p != '\0' && *p != comment; p++) {
	  /* 不是空白字符则重写字符串 */
    if (0 == isspace(*p)) {
      *q++ = *p;
    }
  }
  *q = '\0';

  return 0 != strlen(string); /* 字符串长度不为0,表示数据可用 */
}

/**
 * @brief 16进制字符转为数值
 * 
 * @param ch 16进制字符
 * @return uint8_t 数值
 */
uint8_t hex_char_to_value(uint8_t ch)
{
	uint8_t result = 0;
	/*获取16进制的高字节位数据*/
	if (ch >= '0' && ch <= '9')
	{
		result = ch - '0';
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		result = ch - 'a' + 10;
	}
	else if (ch >= 'A' && ch <= 'Z')
	{
		result = ch - 'A' + 10;
	}
	else
	{
		result = 0;
	}
	return result;
}

/**
 * @brief 将大写字母转换成小写字母
 * 
 * @param ch 大写字母
 * @return uint8_t 小写字母
 */
uint8_t ch_tolower(uint8_t ch)
{
    if(ch >= 'A' && ch <= 'Z')
    {
        return ch + 'a' - 'A';
    }
    else
    {
        return ch;
    }
}

/**
 * @brief 16进制的字符串转换成整数
 * 
 * @param s 16进制字符串
 * @return int 数值
 */
int hextoi(char s[])
{
    int i = 0;
    int ret = 0;
    if(s[0] == '0' && (s[1]=='x' || s[1]=='X'))
    {  
        i = 2;
    }
    else
    {
        i = 0;
    }
    for(;(s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)
    {
        if(ch_tolower(s[i]) > '9')
        {
            ret = 16 * ret + (10 + ch_tolower(s[i]) - 'a');
        }
        else
        {
            ret = 16 * ret + (ch_tolower(s[i]) - '0');
        }
    }
    return ret;
}

/*=================================BASE64========================================*/
#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66   
static const unsigned char dec[] =
{
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 64, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 62, 66, 66, 66, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60,
  61, 66, 66, 66, 65, 66, 66, 66,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
  11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 66, 66, 66, 66,
  66, 66, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
  43, 44, 45, 46, 47, 48, 49, 50, 51, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
  66, 66, 66, 66, 66, 66, 66, 66, 66
};

static const char enc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * @brief Get the base64 encode size of the specified binary data
 *
 * @param binsize Size of binary data we wish to encode
 * @return        Size of base64 encoded binary data
 */
size_t common_b64_encodesize (size_t binsize)
{
  size_t result = binsize / 3 * 4;    // Four chars per three bytes
  if (binsize % 3) { result += 4; }   // Another four for trailing one or two
  return result + 1;                  // Plus '\0'
}

/**
 * @brief Get the maximum base64 decode size of the specified base64 encoded string
 *
 * @param in Pointer to base64 encoded string
 * @return   Maximum size of base64 decode
 */
size_t common_b64_maxdecodesize (const char *in)
{
  size_t inLen = strlen (in);
  return (inLen % 4) ? inLen / 4 * 3 + 2 : inLen / 4 * 3;
}

/**
 * @brief Decode base64 (encoded) string
 *
 * @param in     Pointer to base64 encoded string
 * @param out    General purpose pointer to the output of this base64 decode function
 * @param outLen Length of the decoded output
 * @return       'true' if decode successful, 'false' if decode in error
 */
bool common_b64_decode (const char *in, void *outv, size_t *outLen)
{
  uint8_t *out = (uint8_t *) outv;
  int iter = 0;
  uint32_t buf = 0;
  size_t len = 0;

  while (*in)
  {
    unsigned char c = dec[(unsigned char) (*in++)];

    if (c == WHITESPACE) continue;   // skip whitespace
    if (c == INVALID) return false;  // invalid input, return error
    if (c == EQUALS) break;          // pad character, end of data

    buf = buf << 6 | c;

    /* Every four symbols we will have filled the buffer. Split it into bytes */

    if (++iter == 4)
    {
      if ((len += 3) > *outLen) { return false; } // buffer overflow
      *(out++) = (buf >> 16) & 255;
      *(out++) = (buf >> 8) & 255;
      *(out++) = buf & 255;
      buf = 0;
      iter = 0;
    }
  }
  if (iter == 3)
  {
    if ((len += 2) > *outLen) { return false; } // buffer overflow
    *(out++) = (buf >> 10) & 255;
    *(out++) = (buf >> 2) & 255;
  }
  else if (iter == 2)
  {
    if (++len > *outLen) { return false; } // buffer overflow
    *(out++) = (buf >> 4) & 255;
  }

  *outLen = len; // modify outLen to reflect the actual output size
  return true;
}

/**
 * @brief Encode input into base64 encoded string
 *
 * @param in     General purpose pointer to input
 * @param inLen  Size of input
 * @param out    Pointer to output
 * @param outLen Length of the base64 encoded string
 * @return       'true' if encode successful, 'false' if encode in error
 */
bool common_b64_encode (const void *in, size_t inLen, char *out, size_t outLen)
{
  const uint8_t *data = (const uint8_t *) in;
  size_t resultIndex = 0;
  size_t x;
  uint32_t n = 0;
  uint8_t n0, n1, n2, n3;

  if (outLen < common_b64_encodesize (inLen))
  {
    return false;
  }

  /* iterate over the length of the string, three characters at a time */
  for (x = 0; x < inLen; x += 3)
  {
    /* combine up to three bytes into 24 bits */

    n = ((uint32_t) data[x]) << 16;
    if ((x + 1) < inLen)
    {
      n += ((uint32_t) data[x + 1]) << 8;
    }
    if ((x + 2) < inLen)
    {
      n += data[x + 2];
    }

    /* split into four 6-bit numbers */

    n0 = (uint8_t) (n >> 18) & 63;
    n1 = (uint8_t) (n >> 12) & 63;
    n2 = (uint8_t) (n >> 6) & 63;
    n3 = (uint8_t) n & 63;

    /* One byte -> two characters */

    out[resultIndex++] = enc[n0];
    out[resultIndex++] = enc[n1];

    /* Two bytes -> three characters */

    if ((x + 1) < inLen)
    {
      out[resultIndex++] = enc[n2];
    }

    /* Three bytes -> four characters */

    if ((x + 2) < inLen)
    {
      out[resultIndex++] = enc[n3];
    }
  }

  /* Pad to multiple of four characters */

  while (resultIndex % 4)
  {
    out[resultIndex++] = '=';
  }

  /* Terminate string */

  out[resultIndex] = 0;
  return true;
}

/**
 ******************************************************************
 * @brief   array转为base64,使用完需释放
 * @param   [in]str 原始数据
 * @return  解码后数据
 * @author  aron566
 * @version V1.0
 * @date    2020-12-14
 ******************************************************************
 */
uint8_t *base64_encode(const uint8_t *buf, uint64_t size)
{
    size_t str_len = common_b64_encodesize((size_t)size);
    uint8_t *str = (uint8_t *)calloc(str_len, sizeof(uint8_t));
    if(str == NULL)
    {
        return NULL;
    }
    bool ret = common_b64_encode(buf, (size_t)size, str, str_len);
    if(ret == false)
    {
        return NULL;
    }
    return str;
}

/**
 ******************************************************************
 * @brief   base64转为array,使用完需释放
 * @param   [in]base64数据
 * @param   [out]size 解码字节长度
 * @return  解码后数据
 * @author  aron566
 * @version V1.0
 * @date    2020-12-14
 ******************************************************************
 */
uint8_t *base64_decode(const char *base64str, uint64_t *size)
{
    size_t data_len = common_b64_maxdecodesize(base64str);
    uint8_t *binary_data = (uint8_t *)calloc(data_len, sizeof(uint8_t));
    if(binary_data == NULL)
    {
        return NULL;
    }
    bool ret = common_b64_decode(base64str, binary_data, &data_len);
    if(ret == false)
    {
        return NULL;
    }
    *size = (uint64_t)data_len;
    return binary_data;
}

/**
  ******************************************************************
  * @brief   获取文件大小
  * @param   [in]fileName
  * @return  文件大小字节数
  * @author  aron566
  * @version V1.0
  * @date    2020-12-13
  ******************************************************************
  */
int get_file_size(const char *filename)
{ 
	// struct stat st;
	// stat(filename, &st);
	// return st.st_size;
  FILE *fd = file_open(filename, READ_ONLY);
  long size = 0;
  if(fd)
  {
    fseek (fd, 0L, SEEK_END);
    size = ftell(fd);
    rewind (fd);
  }
  return (int)size;
}

/**
 * @brief 16进制格式调试打印
 * 
 * @param msg 
 * @param msg_len 
 */
void debug_print(uint8_t *msg ,uint32_t msg_len)
{
  for (uint32_t i = 0; i < msg_len; i++)
  printf("%02X ",
      msg[i]); //打印2位的16进制数，不足位0补齐。
  printf("\n");
}

/**
 * @brief main入口
 * 
 * @param argc 参数个数
 * @return argv[] 参数
 */
int main(int argc ,char *argv[])
{
	if(argc < 2)
	{
		printf("Usage:%s filename \n" ,argv[0]);
		return -1;
	}
	const char *filename = argv[1];
	int size = get_file_size(filename);
	uint8_t *buf = (uint8_t *)calloc(size+8, sizeof(uint8_t));
	if(buf == NULL)
	{
		printf("calloc error.\n");
	}

	/*打开文件*/
	size_t len = file_read_with_mode(filename, buf+8, 1, size, READ_WRITE_APPEND_CREAT_FILE_B);
	buf[0] = GET_U16_HI_BYTE(1);
	buf[1] = GET_U16_LOW_BYTE(1);
	buf[2] = GET_U16_HI_BYTE(0);
	buf[3] = GET_U16_LOW_BYTE(0);
	buf[4] = GET_U16_HI_BYTE(GET_U32_HI_HALF_WORD(len));
	buf[5] = GET_U16_LOW_BYTE(GET_U32_HI_HALF_WORD(len));
	buf[6] = GET_U32_LOW_HALF_WORD(GET_U16_HI_BYTE(len));
	buf[7] = GET_U32_LOW_HALF_WORD(GET_U16_LOW_BYTE(len));
	
	uint8_t *base64_str = base64_encode(buf, size+8);
	if(base64_str == NULL)
	{
		printf("encode error.\n");
	}

	printf("%s\n", base64_str);
	printf("total:%d pack:%ld\n",size+8, len);
	uint64_t filesize;
	uint8_t *out = base64_decode(base64_str, &filesize);
	printf("decode total:%lu pack:%lu\n",filesize, filesize-8);
	file_write("outfile.c" ,(uint8_t*)out+8 ,1 ,filesize-8 ,READ_WRITE_CREAT_CLEAR);
	free(base64_str);
	free(buf);
	return 0;
}
