/**                                                                             
 *  @file custom-common.h                                                    
 *                                                                              
 *  @date 2020年11月09日 10:46:21 星期一
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_H                                                          
#define CUSTOM_COMMON_H                                                          
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
#include "3third_party/cJSON/cJSON.h"
#include "3third_party/libsqlite/sqlite3.h"
#include "3third_party/tomlc99/toml.h"
#include "3third_party/tomlc99/toml_utilities.h"
#include "3third_party/libyaml/yaml.h"
#include "3third_party/libuv/include/uv.h"
#include "custom-common-circularqueue.h"
#include "custom-common-utilities.h"
#include "custom-common-file.h"    
#include "custom-common-crc.h"
#include "custom-common-epoll.h"
#include "custom-common-listen-list.h"
/** Private defines ----------------------------------------------------------*/
                                                                      
/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*数值转为iot_data格式*/
iot_data_t *common_value2iot_data(const void *data, VALUE_Type_t type);

/*iot_data转为uint64_t格式*/
uint64_t common_iot_data2u64(const iot_data_t *data, VALUE_Type_t type);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                