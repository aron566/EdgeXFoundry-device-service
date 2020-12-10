/**                                                                             
 *  @file device_driver_parse_par.h                                                    
 *                                                                              
 *  @date 2020年11月20日 15:58:55 星期五
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_PARSE_PAR_H                                                          
#define DEVICE_DRIVER_PARSE_PAR_H                                                          
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
#include "device_driver_list.h"
#include "../service_component/custom-common.h"                                                                         
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*注册设备驱动*/
void register_device_driver(iot_logger_t *lc);

/*获取设备协议类型*/
PROTOCOL_Type_t get_device_protocol_type(DEV_INFO_Typedef_t *dev_info);

/*获取设备类型*/
DEVICE_Typedef_t get_device_type(DEV_INFO_Typedef_t *dev_info);

/*返回设备地址*/
int get_device_addr(DEV_INFO_Typedef_t *dev_info);
uint8_t get_modbus_device_addr(const char *dev_name);

/*解析设备名包含的信息*/
int parse_dev_name(const char *dev_name, DEV_INFO_Typedef_t *dev_info);

/*合成设备名*/
char *jonint_dev_name(char *dest_str, size_t size, PROTOCOL_Type_t protocol_type, const char *location, 
                        DEVICE_Typedef_t dev_type, int address);

/*获取设备类型表*/
const DEVICE_TYPE_MAP_Typedef_t *get_device_type_list(void);

/*获取设备资源表*/
const DEVICE_RESOURCE_MAP_Typedef_t *get_device_resource_list(void);

/*获取设备上报事件确认接口*/
const REPORT_EVENT_CONFIRM_FUNC get_device_event_confirm_func(DEVICE_Typedef_t dev_type);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                