/**                                                                             
 *  @file device_driver_update.h                                                    
 *                                                                              
 *  @date 2020年12月15日 00:05:05 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_UPDATE_H                                                          
#define DEVICE_DRIVER_UPDATE_H                                                          
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
#include <limits.h> /**< need variable max value    */                          
/** Private includes ---------------------------------------------------------*/
#include "../service_component/custom-common.h"                                                                 
/** Private defines ----------------------------------------------------------*/
                                                 
/** Exported typedefines -----------------------------------------------------*/                                                  
/*升级记录表*/
typedef struct 
{
    uint16_t package_total;         /**< 数据总包数*/
    uint16_t package_sub;           /**< 数据子包号 start at zero*/
    uint8_t *data;                  /**< 数据地址*/
    bool     is_empty;              /**< 当前包数据是否为空*/
}UPDATE_DATA_RECORD_Typedef;                                                                      
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*更新表初始化*/
void update_data_table_init(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*更新表数据内容释放*/
void update_data_table_free(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*向更新记录表添加数据*/
void update_data_table_add(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint8_t *package, 
                              uint16_t package_num, uint16_t package_total);

/*移除更新记录表中指定package*/
void update_data_table_remove(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint16_t package_num);

/*检查更新记录表是否已满*/
bool update_data_table_is_full(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*显示进度条*/
void update_data_table_show_progressbar(size_t process, size_t total);

/*重置进度条*/
void update_data_table_reset_progressbar(void);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                