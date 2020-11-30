/**                                                                             
 *  @file custom-common-file.c                                                   
 *                                                                              
 *  @date 2020年11月30日 16:26:12 星期一
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief None.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "custom-common-file.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                                
/** Private function prototypes ----------------------------------------------*/
                                                                                
/** Private user code --------------------------------------------------------*/                                                                      
                                                                                
/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/                                                                              

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/ 
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
	case READ_FILE_ONLY:
		fp = fopen(filename ,"r");
		break;
	case READ_WRITE_FILE_ONLY:
		fp = fopen(filename ,"r+");
		break;
	case WRITE_CREAT_CLEAR_FILE:
		fp = fopen(filename ,"w");
		break;
	case READ_WRITE_CREAT_CLEAR_FILE:
		fp = fopen(filename ,"w+");
		break;
	case WRITE_APPEND_CREAT_FILE:
		fp = fopen(filename ,"a");
		break;
	case READ_WRITE_APPEND_CREAT_FILE:
		fp = fopen(filename ,"a+");/**< 首次读取时，从文件头部开始读*/
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

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
                                                                            