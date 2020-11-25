/**                                                                             
 *  @file relay_device.h                                                    
 *                                                                              
 *  @date 2020年11月08日 20:28:50 星期天
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief 继电器设备驱动.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef RELAY_DEVICE_H                                                          
#define RELAY_DEVICE_H                                                          
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
#include "devsdk/devsdk.h"    
#include "service_component/custom-common.h"
#include "device_driver_list_config.h"
#include "device_driver_opt.h"                                      
/** Private defines ----------------------------------------------------------*/
                                                                    
/** Exported typedefines -----------------------------------------------------*/
                                                                      
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*继电器设备驱动注册*/
int relay_device_driver_register(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par);       

/*获取继电器设备资源表*/
DEV_DRIVER_INTERFACE_Typedef_t *get_relay_device_resource(void);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                