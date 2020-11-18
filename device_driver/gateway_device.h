/**                                                                             
 *  @file gateway_device.h                                                    
 *                                                                              
 *  @date 2020年11月10日 10:23:35 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief 网关设备驱动.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef GATEWAY_DEVICE_H                                                          
#define GATEWAY_DEVICE_H                                                          
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

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                