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
    CURRENT_TIME_MS,    /**< 当前时间即从UTC1970-1-1 0:0:0开始us/ms/s级别*/
    RUN_TIME,           /**< 系统启动运行时间*/
    PROCESS_CPUTIME,    /**< 本进程到当前代码系统CPU花费的时间*/
    THREAD_CPUTIME,     /**< 本线程到当前代码系统CPU花费的时间*/
}UTILITIES_TIME_MODE_Typedef_t;

/*数值类型*/
typedef enum
{
    INT8 = 0,       
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT32,
    DOUBLE,
    STRING,
    VALUE_TYPE_MAX,
}VALUE_Type_t; 
/** Exported constants -------------------------------------------------------*/
                                                                               
/** Exported macros-----------------------------------------------------------*/
/**                                                                             
 * @name 常用工具                                                           
 * @{                                                                           
 */ 
#define UNUSED(x) (void)(x)/**< 消除未使用参数报警*/
#define OFFSETOF(type, member)  ((size_t)(&(((type*)0)->member)))/**< 求成员偏移字节*/
#define BYTES_TO_U8ARRAY_INNDEX(at_bytes) (at_bytes-1)/**< 字节转为数组中的位置*/
#define GET_U16_HI_BYTE(data)   ((uint8_t)((data>>8)&0x00FF))/**< 获得u16数据高字节*/
#define GET_U16_LOW_BYTE(data)  ((uint8_t)(data&0x00FF))/**< 获得u16数据低字节*/
#define GET_U32_HI_HALF_WORD(data)  ((uint16_t)((data>>16)&0xFFFF))/**< 获得u32数据高半字*/
#define GET_U32_LOW_HALF_WORD(data)  ((uint16_t)(data&0xFFFF))/**< 获得u32数据低半字*/
/** @}*/  
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*调试打印*/
void debug_print(uint8_t *msg, uint32_t msg_len);

/*安全字符串拷贝*/
char *strncopy(char *dest_str, const char *src_str, size_t size);

/*获取数值对应的字符串*/
char *get_value_str(char *dest_str, void *data, size_t size, VALUE_Type_t value_type);

/*获取时间*/
uint64_t get_current_time_s(UTILITIES_TIME_MODE_Typedef_t mode);

/*秒转为时间字符串*/
const char *get_time_str(time_t sec);

/*16进制字符转为数值*/
uint8_t hex_char_to_value(uint8_t ch);

/*16进制数组转字符串*/
void hex_to_str(char *strbuf, uint8_t *hex_data, uint32_t len);

/*将大写字母转换成小写字母*/
uint8_t ch_tolower(uint8_t ch);

/*16进制的字符串转换成整数*/
int hextoi(char s[]);

/*过滤指定字符*/
int common_filter_special_char(char ch, const char *str, char *out_str, int size);

/*解析32位数据-低位在前*/
uint32_t common_get_u32_data(uint8_t *data, int start_index);

/*解析16位数据-低位在前*/
uint16_t common_get_u16_data(uint8_t *data, int start_index);

/*解析浮点数数据-低位在前*/
float common_get_float_data(uint8_t *data, int start_index);

/*解析32位数据-高位在前*/
uint32_t common_get_modbus_u32_data(uint8_t *data, int start_index);

/*解析16位数据-高位在前*/
uint16_t common_get_modbus_u16_data(uint8_t *data, int start_index);

/*解析浮点数数据-高位在前*/
float common_get_modbus_float_data(uint8_t *data, int start_index);

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

/**
 * @brief Get the base64 encode size of the specified binary data
 *
 * @param binsize Size of binary data we wish to encode
 * @return        Size of base64 encoded binary data
 */
extern size_t common_b64_encodesize (size_t binsize);

/**
 * @brief Get the maximum base64 decode size of the specified base64 encoded string
 *
 * @param in Pointer to base64 encoded string
 * @return   Maximum size of base64 decode
 */
extern size_t common_b64_maxdecodesize (const char * in);

/**
 * @brief Decode base64 (encoded) string
 *
 * @param in     Pointer to base64 encoded string
 * @param out    General purpose pointer to the output of this base64 decode function
 * @param outLen Length of the decoded output
 * @return       'true' if decode successful, 'false' if decode in error
 */
extern bool common_b64_decode (const char * in, void * out, size_t * outLen);

/**
 * @brief Encode input into base64 encoded string
 *
 * @param in     General purpose pointer to input
 * @param inLen  Size of input
 * @param out    Pointer to output
 * @param outLen Length of the base64 encoded string
 * @return       'true' if encode successful, 'false' if encode in error
 */
extern bool common_b64_encode (const void * in, size_t inLen, char * out, size_t outLen);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                