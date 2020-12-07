/**                                                                             
 *  @file device_driver_modbus_proto.h                                                    
 *                                                                              
 *  @date 2020年12月01日 14:08:52 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_MODBUS_PROTO_H                                                          
#define DEVICE_DRIVER_MODBUS_PROTO_H                                                          
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
/** Private defines ----------------------------------------------------------*/
                                                                     
/** Exported typedefines -----------------------------------------------------*/
/*modbus报文解析状态码*/
typedef enum
{
    MODBUS_OK = 0,          /**< 校验正确*/
    MODBUS_FRAME_ERR,       /**< 帧错误*/
    MODBUS_CRC_ERR,         /**< crc错误*/
    MODBUS_FRAME_MISS,      /**< 帧不完整*/
    MODBUS_TIMEOUT,         /**< 响应超时*/
}MODBUS_PARSE_CODE_Typedef_t;                                                                    
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*modbus主站协议栈初始化*/
void device_driver_modbus_master_stack_init(void);

/*modbus主站读,使用后释放reg_value指向的地址内存*/
MODBUS_PARSE_CODE_Typedef_t device_driver_modbus_master_read(uint8_t addr, uint16_t reg_s, uint16_t reg_n,
                                      uint16_t **reg_value);
/*modbus主站写*/
MODBUS_PARSE_CODE_Typedef_t device_driver_modbus_master_write(uint8_t addr, uint16_t reg_s, 
                                                          uint16_t reg_n, uint16_t *reg_d);
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                