/**                                                                             
 *  @file custom-common-file.h                                                    
 *                                                                              
 *  @date 2020年11月30日 16:25:40 星期一
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_FILE_H                                                          
#define CUSTOM_COMMON_FILE_H                                                          
#ifdef __cplusplus ///<use C compiler                                           
extern "C" {                                                                   
#endif                                                                          
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */                          
#include <stddef.h> /**< need definition of NULL    */                          
#include <stdbool.h>/**< need definition of BOOL    */                        
#include <stdio.h>  /**< if need printf             */                          
#include <stdlib.h>                                                             
#include <string.h>                                                             
/** Private includes ---------------------------------------------------------*/
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/
/*文件打开权限*/
typedef enum
{
	READ_FILE_ONLY = 0,
	READ_WRITE_FILE_ONLY,
	WRITE_CREAT_CLEAR_FILE,
	READ_WRITE_CREAT_CLEAR_FILE,
	WRITE_APPEND_CREAT_FILE,
	READ_WRITE_APPEND_CREAT_FILE,
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

/*写入指定的内容到文件*/
size_t file_write(const char *filename ,const void* buffer ,size_t size ,size_t count ,FILE_OPEN_MODE mode);

/*替换字符*/
size_t file_replace_ch(char *sourcebuf ,char sourcech,char destch);

/*清除字符串空格*/
char *strtriml(char *pstr);
char *strtrimr(char *pstr);
char *strtrim(char *pstr);

/*清除文本中空格，忽略注释标识行*/
bool file_strip_comments(char *string, char comment);
                                                                             
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                