/**                                                                             
 *  @file device_driver_opt.h                                                    
 *                                                                              
 *  @date 2020年11月10日 10:12:01 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_OPT_H                                                          
#define DEVICE_DRIVER_OPT_H                                                          
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
#include "../service_component/custom-common.h"                                                                                
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
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
                                                                                
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                