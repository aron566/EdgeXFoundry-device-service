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
/*设备类型列表*/
typedef int (*REGISTER_DEV_FUNC)(DEV_INFO_Typedef_t *, DEV_COMMUNICATION_PAR_Typedef_t *, DEV_DRIVER_INTERFACE_Typedef_t *);
typedef struct 
{
    const char* const type_name;    /**< 类型名称*/
    DEVICE_Typedef_t dev_type;      /**< 类型*/
    REGISTER_DEV_FUNC register_func;/**< 设备注册函数*/
}DEVICE_TYPE_MAP_Typedef_t;                                                                       
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

/*获取设备类型表*/
DEVICE_TYPE_MAP_Typedef_t *get_device_type_list(void);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                