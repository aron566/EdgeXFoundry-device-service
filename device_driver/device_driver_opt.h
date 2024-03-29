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

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*设备驱动初始化*/
void device_driver_opt_init(void *data, const iot_data_t *config);

/*设备驱动启动*/
void device_driver_opt_reconfigure(iot_logger_t *lc, const iot_data_t *config);

/*设备驱动停止*/
void device_driver_opt_stop(iot_logger_t *lc, bool force);

/*设备驱动发现*/
void device_driver_opt_discover(iot_logger_t *lc);

/*设备驱动get接口*/
int device_driver_opt_get(const char *devname, const char *param, devsdk_commandresult *readings, 
                          iot_logger_t *lc);

/*设备驱动set接口*/
int device_driver_opt_set(const char *devname, const char *param, const iot_data_t *values, 
                          iot_logger_t *lc);

/*响应添加设备*/
void device_driver_add_device(void *impl, const char *devname, const devsdk_protocols *protocols, 
                              const devsdk_device_resources *resources, bool adminEnabled);

/*响应更新设备*/
void device_driver_update_device(void *impl, const char *devname, const devsdk_protocols *protocols, 
                                  bool adminEnabled);

/*响应移除设备*/
void device_driver_remove_device(void *impl, const char *devname, const devsdk_protocols *protocols);

/*启动事件*/
void *device_driver_autoevent_start_handler(void *impl, const char *devname, const devsdk_protocols *protocols,
                                            const char *resource_name,
                                            uint32_t nreadings,
                                            const devsdk_commandrequest *requests,
                                            uint64_t interval,
                                            bool onChange);
/*停止事件*/
void device_driver_autoevent_stop_handler(void *impl, void *handle);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                