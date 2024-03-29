/**                                                                             
 *  @file temperature_device.h                                                    
 *                                                                              
 *  @date 2020年11月08日 20:25:24 星期天
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief 温湿度设备驱动.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef TEMPERATURE_DEVICE_H                                                          
#define TEMPERATURE_DEVICE_H                                                          
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
#include "device_driver_list_config.h"                                                                                 
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*温湿度设备驱动注册*/
int temperature_device_driver_register(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par,
                                        DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par);    

/*获取温湿度设备资源表*/
const DEV_DRIVER_INTERFACE_Typedef_t *get_temperature_device_resource(void);

/*温湿度设备事件上报确认*/
bool temperature_device_report_event_confirm(const char *dev_name, const char *param_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, 
                                        void *data);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                