/**                                                                             
 *  @file device_driver_list_config.h                                                    
 *                                                                              
 *  @date 2020年11月10日 11:45:47 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_LIST_CONFIG_H
#define DEVICE_DRIVER_LIST_CONFIG_H
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
/*设备分类*/
typedef enum
{
    GATEWAY_DEV_TYPE = 0,   /**< 网关设备类*/
    TEMPERATURE_DEV_TYPE,   /**< 温湿度设备类*/
    RELAY_DEV_TYPE,         /**< 继电器设备类*/
    DEV_TYPE_MAX,
}DEVICE_Typedef_t;

/*数值类型*/
typedef enum
{
    INT8 = 0,       
    UINT8,
    INT16,
    UINT16,
    UINT32,
    INT32,
    FLOAT32,
    DOUBLE,
    STRING,
    VALUE_TYPE_MAX,
}VALUE_Type_t;  

/*协议类型*/
typedef enum
{
    MQTT_PROTO = 0,
    MODBUS_RTU_PROTO,
    PRIVATE_PROTO,
    UNKNOW_PROTO,
    PROTO_MAX,
}PROTOCOL_Type_t;

/*设备通讯地址*/
typedef struct 
{
    uint8_t addr;
}MODBUS_RTU_Typedef_t;

typedef struct 
{
    char subcribe_topic[128];           /**< 订阅*/
    char publish_topic[128];            /**< 发布*/
}MQTT_Typedef_t;

typedef struct 
{
    char cmd[256];                      /**< 私有命令*/
}PRIVATE_Typedef_t;

typedef struct 
{
    PROTOCOL_Type_t protocol_type;  /**< 设备协议信息*/
    MODBUS_RTU_Typedef_t modbus_par;
    MQTT_Typedef_t mqtt_par;
    PRIVATE_Typedef_t private_par;
}DEV_COMMUNICATION_PAR_Typedef_t;

/*设备驱动同一接口*/
typedef void (*GET_DEV_VALUE_CALLBACK)(void *input_data, void *out_data, VALUE_Type_t type);/**< 设备读取接口*/
typedef void (*SET_DEV_VALUE_CALLBACK)(void *input_data, void *out_data, VALUE_Type_t type);/**< 设备设置接口*/
typedef struct dev_node_func { 
    GET_DEV_VALUE_CALLBACK get_dev_value_callback;/**< 设备读取接口*/
    SET_DEV_VALUE_CALLBACK set_dev_value_callback;/**< 设备设置接口*/
    DEV_COMMUNICATION_PAR_Typedef_t communication_par;/**< 设备通讯参数信息*/
    PROTOCOL_Type_t major_key_1;  /**< 设备采用的协议类型*/
    // char dev_name[256];         /**< 设备完整名称*/
    uint32_t major_key_2;         /**< 设备地址或者序号，此值在同类设备中唯一*/
}DEV_NODE_FUNC_Type_t;

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
                                                                                