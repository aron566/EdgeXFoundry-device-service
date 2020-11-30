/**                                                                             
 *  @file custom-common-crc.h                                                    
 *                                                                              
 *  @date 2020年09月10日 17:37:31 星期四
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_CRC_H                                                          
#define CUSTOM_COMMON_CRC_H                                                         
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
                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                       
/** Exported typedefines -----------------------------------------------------*/
                                                                       
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*查表计算CRC16*/
uint16_t get_crc(uint8_t *puchMsg ,uint16_t usDataLen);
/*查表计算CRC16并对比数据包中的CRC，返回结果，数据包含crc计算内容+crc结果，否则可能指针越界访问*/                                                                              
bool get_crc_result(uint8_t *puchMsg ,uint16_t usDataLen);

/*计算CRC*/
uint16_t modbus_crc_return(uint8_t *Crc_Buf ,uint16_t Crc_Len);
/*计算CRC16并对比数据包中的CRC，返回结果，数据包含crc计算内容+crc结果，否则可能指针越界访问*/ 
uint8_t return_check_crc(uint8_t *msg ,uint16_t len);
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                