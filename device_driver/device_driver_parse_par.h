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
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/
                                                                       
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*注册设备驱动*/
void register_device_driver(void);

/*获取设备协议类型*/
PROTOCOL_Type_t get_device_protocol_type(DEV_INFO_Typedef_t *dev_info);

/*获取设备类型*/
DEVICE_Typedef_t get_device_type(DEV_INFO_Typedef_t *dev_info);  

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                