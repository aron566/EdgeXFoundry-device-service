/**                                                                             
 *  @file device_driver_port.h                                                    
 *                                                                              
 *  @date 2020年11月25日 16:04:14 星期三
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_PORT_H                                                          
#define DEVICE_DRIVER_PORT_H                                                          
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
/*设备数据读取格式*/
typedef struct 
{
    uint8_t *buf;
    size_t buf_len;
}BASE_BUF_STRUCT_Typedef_t;

typedef struct 
{
    BASE_BUF_STRUCT_Typedef_t base_buf;
    struct sockaddr_in remote_host_addr;
}PORT_BUF_STRUCT_Typedef_t;

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*通讯初始化*/
void device_driver_com_init_port(void);

/*获得指定通讯口fd*/
int device_driver_com_get_fd(PROTOCOL_Type_t protocol_type);

/*向指定通讯口发送数据*/
ssize_t device_driver_send_data_port(PORT_BUF_STRUCT_Typedef_t *data, PROTOCOL_Type_t protocol_type);

/*向指定通讯口读取数据*/
ssize_t device_driver_read_data_port(PORT_BUF_STRUCT_Typedef_t *buf, PROTOCOL_Type_t protocol_type);

/*关闭指定通讯口*/
int device_driver_com_close(PROTOCOL_Type_t protocol_type);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                