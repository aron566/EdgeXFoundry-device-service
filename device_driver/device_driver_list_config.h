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
    MODBUS_PROTO,
    PRIVATE_PROTO,
    UNKNOW_PROTO,
}PROTOCOL_Type_t;

/*设备驱动同一接口*/
typedef struct dev_node_func { 
    void (*get_dev_value_callback)(void *input_data, void *out_data, VALUE_Type_t type);/**< 设备读取接口*/
    void (*set_dev_value_callback)(void *input_data, void *out_data, VALUE_Type_t type);/**< 设备设置接口*/
    PROTOCOL_Type_t major_key;  /**< 设备协议信息*/
    char dev_name[256];         /**< 设备完整名称*/
}DEV_NODE_FUNC_Type_t;
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
                                                                                
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                