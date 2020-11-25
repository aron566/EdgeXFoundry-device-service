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
#include "device_driver_list_config.h"
#include "../service_component/custom-common.h"                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                     
/** Exported typedefines -----------------------------------------------------*/
/*设备名称解析*/
typedef struct 
{
    char protocol_str[64];      /**< 协议名*/
    char location_str[128];     /**< 设备位置信息*/
    char dev_type_name[64];     /**< 设备类型名称*/
    char dev_address[16];       /**< 设备地址号*/
}DEV_INFO_Typedef_t;

/*设备接口数据结构*/
typedef enum
{
    READ_ONLY = 0,
    WRITE_ONLY,
    READ_WRITE,
    UNKNOW,
}PERMISSIONS_TYPE;

/*设备服务*/
typedef enum
{
    T_MS = 0,                           /**< 毫秒单位*/
    T_S,                                /**< 秒*/
    T_MIN,                              /**< 分*/
    T_H,                                /**< 时*/
    T_D,                                /**< 天*/
    T_MTH,                              /**< 月*/
    T_YER,                              /**< 年*/
    T_MAX
}EVENT_REPORT_TIME_UNIT;
typedef struct 
{
    const char *const par_name;         /**< 设备服务名称*/
    uint8_t command;                    /**< 设备控制命令*/
    uint16_t command_addr;              /**< 设备参数地址*/
    PERMISSIONS_TYPE permissions;       /**< 命令权限*/
    bool enable_event_flag;             /**< 开始事件标识*/
    bool enable_on_change_flag;         /**< 数值改变时事件上报*/
    uint64_t interval_time;             /**< 事件上报频率*/
    EVENT_REPORT_TIME_UNIT unit;        /**< 事件上报时基*/
}DEV_DRIVER_INTERFACE_Typedef_t;    

typedef struct 
{
    uint64_t interval_time;             /**< 事件上报频率*/
    EVENT_REPORT_TIME_UNIT unit;        /**< 事件上报时基*/    
}INTERVAL_TIME_Typedef_t;

/*协议解析器映射*/
typedef struct 
{
  PROTOCOL_Type_t protocol_type;
  GET_DEV_VALUE_CALLBACK get_callback;
  SET_DEV_VALUE_CALLBACK set_callback;
}PROTOCOL_DECODE_CALLBACK_Typedef_t;

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
                                                                                
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                