/**                                                                             
 *  @file device_driver_auto_event.h                                                    
 *                                                                              
 *  @date 2020年11月24日 11:33:41 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_AUTO_EVENT_H                                                          
#define DEVICE_DRIVER_AUTO_EVENT_H                                                          
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
#include <limits.h> /**< define max value           */                                                        
/** Private includes ---------------------------------------------------------*/
#include "device_driver_list_config.h"
#include "../service_component/custom-common.h"
/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*事件检测轮询启动*/
int device_driver_uv_handler_start(void *data);

/*事件检测轮询启动*/
void device_driver_uv_handler_stop(void);

/*任意时间单位转为ms单位数*/
uint64_t time_base2ms(INTERVAL_TIME_Typedef_t *time_base);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                