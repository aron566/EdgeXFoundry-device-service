/**                                                                             
 *  @file custom-common-utilities.h
 *                                                                              
 *  @date 2020年09月17日 09:56:44 星期四
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_UTILITIES_H                                                          
#define CUSTOM_COMMON_UTILITIES_H                                                          
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
                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                                                                                                                                           
/** Exported typedefines -----------------------------------------------------*/
typedef enum
{
    CURRENT_TIME = 0,   /**< 当前时间即从UTC1970-1-1 0:0:0开始*/
    RUN_TIME,           /**< 系统启动运行时间*/
    PROCESS_CPUTIME,    /**< 本进程到当前代码系统CPU花费的时间*/
    THREAD_CPUTIME,     /**< 本线程到当前代码系统CPU花费的时间*/
}UTILITIES_TIME_MODE_Typedef_t;

/** Exported constants -------------------------------------------------------*/
                                                                               
/** Exported macros-----------------------------------------------------------*/
/**                                                                             
 * @name 常用工具                                                           
 * @{                                                                           
 */ 
#define UNUSED(x) (void)(x)/**< 消除未使用参数报警*/
#define OFFSETOF(type ,member)  ((size_t)(&(((type*)0)->member)))/**< 求成员偏移字节*/
#define BYTES_TO_U8ARRAY_INNDEX(at_bytes) (at_bytes-1)/**< 字节转为数组中的位置*/
#define GET_U16_HI_BYTE(data)   ((uint8_t)((data>>8)&0x00FF))/**< 获得u16数据高字节*/
#define GET_U16_LOW_BYTE(data)  ((uint8_t)(data&0x00FF))/**< 获得u16数据低字节*/
#define GET_U32_HI_HALF_WORD(data)  ((uint16_t)((data>>16)&0xFFFF))/**< 获得u32数据高半字*/
#define GET_U32_LOW_HALF_WORD(data)  ((uint16_t)(data&0xFFFF))/**< 获得u32数据低半字*/
/** @}*/  
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
/*调试打印*/
void debug_print(uint8_t *msg ,uint32_t msg_len);

/*获取时间*/
uint32_t get_curent_time_s(UTILITIES_TIME_MODE_Typedef_t mode);

/*秒转为时间字符串*/
const char *get_time_str(time_t sec);

/*16进制字符转为数值*/
uint8_t hex_char_to_value(uint8_t ch);

/*16进制数组转字符串*/
void hex_to_str(char *strbuf ,uint8_t *hex_data ,uint32_t len);

/*将大写字母转换成小写字母*/
uint8_t ch_tolower(uint8_t ch);

/*16进制的字符串转换成整数*/
int hextoi(char s[]);

/*获取主机名*/
char *get_host_name(void);

/*获取本地mac地址*/
char *get_local_mac(const char *hardware_name);

/*获取本机内存信息*/
struct sysinfo *get_meminfo(void);

/*获取本机CPU信息*/
int get_cpu_num(void);

/*获取本机CPU频率*/
double get_cpu_frequency(void);
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                