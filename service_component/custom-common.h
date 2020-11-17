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
#include "3third_party/tomlc99/toml.h"                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/
/*设备名称解析*/
typedef struct 
{
    char protocol_str[16];      /**< 协议名*/
    char location_str[128];     /**< 设备位置信息*/
    char dev_type_name[16];     /**< 设备类型名称*/
    char dev_address[32];       /**< 设备地址号*/
}DEV_INFO_Typedef_t;

/*设备接口数据结构*/
typedef enum
{
    READ_ONLY = 0,
    WRITE_ONLY,
    READ_WRITE,
    UNKNOW,
}PERMISSIONS_TYPE;
typedef struct 
{
    const char *const par_name; /**< 设备服务名称*/
    uint8_t command;            /**< 设备控制命令*/
    uint16_t command_addr;      /**< 设备参数地址*/
    PERMISSIONS_TYPE permissions;/**< 命令权限*/     
}DEV_DRIVER_INTERFACE_Typedef_t;

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
/*解析设备名包含的信息*/
DEV_INFO_Typedef_t *common_parse_dev_name(const char *dev_name ,DEV_INFO_Typedef_t *dev_info);

/*toml文件转为json-非安全*/
int toml_file2json(char *toml_fileanme, char *outbuf);

/*toml字符串转为json-非安全*/
int toml2json(char *toml_str, char *outbuf);

/*toml文件打印输出*/
int toml_file_cat(const char *toml_filename);
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                