/**                                                                             
 *  @file custom-common.h                                                    
 *                                                                              
 *  @date 2020年11月09日 10:46:21 星期一
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_H                                                          
#define CUSTOM_COMMON_H                                                          
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
#include "devsdk/devsdk.h"
#include "3third_party/libsqlite/sqlite3.h"
#include "3third_party/tomlc99/toml.h"
#include "3third_party/libyaml/yaml.h"
#include "3third_party/libuv/include/uv.h"
#include "custom-common-circularqueue.h"
#include "custom-common-utilities.h"
#include "custom-common-file.h"    
#include "custom-common-crc.h"
#include "custom-common-epoll.h"
#include "custom-common-listen-list.h"
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/

/*toml文件读取方式*/
typedef enum
{
    FP_TYPE = 0,                /**< 文件流方式*/
    STR_TYPE,                   /**< 字符串方式*/
}CAT_TOML_TYPE;
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*toml文件转为json-非安全*/
int toml_file2json(char *toml_fileanme, char *outbuf);

/*toml字符串转为json-非安全*/
int toml2json(char *toml_str, char *outbuf);

/*toml文件打印输出*/
int toml_file_cat(const char *toml_filename);

/*过滤指定字符*/
int common_filter_special_char(char ch, const char *str, char *out_str, int size);

/*解析32位数据-低位在前*/
uint32_t common_get_u32_data(uint8_t *data ,int start_index);

/*解析16位数据-低位在前*/
uint16_t common_get_u16_data(uint8_t *data ,int start_index);

/*解析浮点数数据-低位在前*/
float common_get_float_data(uint8_t *data ,int start_index);

/*解析32位数据-高位在前*/
uint32_t common_get_modbus_u32_data(uint8_t *data ,int start_index);

/*解析16位数据-高位在前*/
uint16_t common_get_modbus_u16_data(uint8_t *data ,int start_index);

/*解析浮点数数据-高位在前*/
float common_get_modbus_float_data(uint8_t *data ,int start_index);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                