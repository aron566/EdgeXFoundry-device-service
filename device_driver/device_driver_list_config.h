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
#include "../service_component/custom-common.h"                                                         
/** Private defines ----------------------------------------------------------*/
/*Modbus-RTU Master读指令0x03功能码*/
/*
    /----------/---------/---------/---------/---------/
   /   地址    /  功能码  /  REG-S  /  REG-N  /  CRC16  /  
  /----------/---------/---------/---------/---------/
 /   1Byte  /  1Byte  /  2Byte  /  2Byte  /  2Byte  / 
/----------/---------/---------/---------/---------/
校         验        数        据        区
PS:
1、地址信息，由设备名读出解析
2、REG-S：需读取寄存器的起始地址、REG-N：读取寄存器数目，高字节在前低字节在后
3、CRC16：低字节在前、高字节在后
*/

/*从机响应 Master读指令0x03功能码*/
/*
    /----------/---------/---------/---------/---------/---------/---------/
   /   地址    /  功能码  /  字节数  / REG-D1  /  REG-D2 /  REG-D3 /  CRC16  /  
  /----------/---------/---------/---------/---------/---------/---------/
 /   1Byte  /  1Byte  /  1Byte  /  2Byte  /  2Byte  /  2Byte  /  2Byte  /
/----------/---------/---------/---------/---------/---------/---------/
    校           验            数             据             区
1、REG-D1：第一个寄存器的数据、REG-D2：第二个寄存器的数据，高字节在前低字节在后
2、字节数：描述寄存器返回的数据总大小（寄存器数目*2）
*/

/*Modbus-RTU Master写指令0x10功能码*/
/*
    /----------/---------/---------/---------/---------/---------/---------/---------/
   /   地址    /  功能码  /  REG-S  /  REG-N  /  字节数  /  REG-D1 /  REG-D1 /  CRC16  /  
  /----------/---------/---------/---------/---------/---------/---------/---------/
 /   1Byte  /  1Byte  /  2Byte  /  2Byte  /  1Byte  /  2Byte  /  2Byte  /  2Byte  / 
/----------/---------/---------/---------/---------/---------/---------/---------/
      校             验             数              据             区
PS:
1、地址信息，由设备名读出解析
2、REG-S：需设置寄存器的起始地址、REG-N：设置寄存器数目，高字节在前低字节在后
3、字节数：设置寄存器数据总大小（寄存器数目*2）
4、REG-D1：第一个寄存器写入的数值，REG-D2：第二个寄存器写入的数值
5、CRC16：低字节在前、高字节在后
*/

/*从机响应 Master写指令0x10功能码*/
/*
    /----------/---------/---------/---------/---------/
   /   地址    /  功能码  /  REG-S  /  REG-N  /  CRC16  /  
  /----------/---------/---------/---------/---------/
 /   1Byte  /  1Byte  /  2Byte  /  2Byte  /  2Byte  /
/----------/---------/---------/---------/---------/
校         验        数        据        区
*/                                                                   
/** Exported typedefines -----------------------------------------------------*/
/*设备分类*/
typedef enum
{
    GATEWAY_DEV_TYPE = 0,   /**< 网关设备类*/
    TEMPERATURE_DEV_TYPE,   /**< 温湿度设备类*/
    RELAY_DEV_TYPE,         /**< 继电器设备类*/
    DEV_TYPE_MAX,
}DEVICE_Typedef_t; 

/*协议类型*/
typedef enum
{
    MQTT_PROTO = 0,
    MODBUS_RTU_PROTO,
    PRIVATE_PROTO,
    UNKNOW_PROTO,
    PROTO_MAX,
}PROTOCOL_Type_t;

/*边缘网关运行状态*/
typedef enum
{
    NORMAL_RUNNING_STATE = 0,     /**< 正常运行状态*/
    UPDATING_STATE,               /**< 更新状态*/
    UNKNOW_STATE,                 /**< 未知状态*/
}EdgeGatewayRUN_SATE_Typedef_t;

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

/**                  
 * @name 设备接口数据结构
 * @brief 只读只写用来说明设备内部参数，私有代表自定义的参数，配置读代表需读配置文件，需确认用于事件上报，存储读代表参数存储在数据库并周期更新
 * @{                
 */                  
typedef enum
{
    READ_ONLY       = 1<<0,             /**< 只读*/
    WRITE_ONLY      = 1<<1,             /**< 只写*/
    READ_WRITE      = 1<<2,             /**< 读写*/
    PRIVATE_WRITE   = 1<<3,             /**< 似有写*/
    PRIVATE_READ    = 1<<4,             /**< 私有读*/
    STORE_READ      = 1<<5,             /**< 存储读*/
    CONFIG_READ     = 1<<6,             /**< 配置读*/
    CONFIRM         = 1<<7,             /**< 需确认*/     
    UNKNOW,                             /**< 未知*/
}PERMISSIONS_TYPE;
/** @}*/  

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
    VALUE_Type_t value_type;            /**< 数值类型*/
    uint64_t default_value;             /**< 参数默认值*/
    PERMISSIONS_TYPE permissions;       /**< 命令权限*/
    bool enable_event_flag;             /**< 开始事件标识*/
    bool enable_on_change_flag;         /**< 数值改变时事件上报*/
    uint64_t start_time;                /**< 事件上报开始时间*/
    uint64_t interval_time;             /**< 事件上报频率*/
    EVENT_REPORT_TIME_UNIT unit;        /**< 事件上报时基*/
}DEV_DRIVER_INTERFACE_Typedef_t;    

typedef struct 
{
    uint64_t interval_time;             /**< 事件上报频率*/
    EVENT_REPORT_TIME_UNIT unit;        /**< 事件上报时基*/    
}INTERVAL_TIME_Typedef_t;

/*设备驱动同一接口*/
typedef int (*GET_DEV_VALUE_CALLBACK)(const char *, const void *input_data, void *out_data, VALUE_Type_t *type);/**< 设备读取接口*/
typedef int (*SET_DEV_VALUE_CALLBACK)(const char *, const void *input_data, const void *out_data, VALUE_Type_t *type);/**< 设备设置接口*/
typedef struct dev_node_func { 
    GET_DEV_VALUE_CALLBACK get_dev_value_callback;/**< 设备读取接口*/
    SET_DEV_VALUE_CALLBACK set_dev_value_callback;/**< 设备设置接口*/
    DEV_COMMUNICATION_PAR_Typedef_t communication_par;/**< 设备通讯参数信息*/
    PROTOCOL_Type_t major_key_1;  /**< 设备采用的协议类型*/
    // char dev_name[256];         /**< 设备完整名称*/
    uint32_t major_key_2;         /**< 设备地址或者序号，此值在同类设备中唯一*/
    DEV_DRIVER_INTERFACE_Typedef_t dev_resource_par[];/**< 设备资源表*/
}DEV_NODE_FUNC_Type_t;

/*设备名称解析*/
typedef struct 
{
    char protocol_str[64];      /**< 协议名*/
    char location_str[128];     /**< 设备位置信息*/
    char dev_type_name[64];     /**< 设备类型名称*/
    char dev_address[16];       /**< 设备地址号*/
}DEV_INFO_Typedef_t;

/*协议解析器映射*/
typedef struct 
{
  PROTOCOL_Type_t protocol_type;
  GET_DEV_VALUE_CALLBACK get_callback;
  SET_DEV_VALUE_CALLBACK set_callback;
}PROTOCOL_DECODE_CALLBACK_Typedef_t;

/*线程 Used to squelch a -Wcast-function-type warning*/
typedef void *(*THREAD_FUNC)(void *par);
typedef void (*THREAD_FUNC_)(void *par);
typedef union 
{
    THREAD_FUNC_ thread_func_;  /**< 用户线程赋值*/
    THREAD_FUNC thread_func;    /**< 提供线程创建*/
}THREAD_FUNC_UNION_Type_t;

/*csdk user data*/
typedef struct custom_device_driver
{
  iot_logger_t * lc;
  devsdk_service_t * service;
}custom_device_driver;

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
#define DEV_EVENT_DB_NAME    "dev_driver_event.db"  /**< 设备事件数据库*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
                                                                                
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                