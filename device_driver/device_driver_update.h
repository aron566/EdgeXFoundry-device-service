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
    uint16_t package_size;          /**< 不含包信息数据包大小*/
    uint8_t *data;                  /**< 数据地址*/
    bool     is_empty;              /**< 当前包数据是否为空*/
}UPDATE_DATA_RECORD_Typedef;                                                                      
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
#define UPDATE_DATA_HEAD_SIZE 6U/**< 数据包桢头信息长度*/
#define GET_UPDATE_DATA_SIZE(total_size) (total_size-UPDATE_DATA_HEAD_SIZE)/**< 获得真实数据包大小*/
#define GET_UPDATE_DATA_TOTAL_SIZE(valid_data_size) (valid_data_size+UPDATE_DATA_HEAD_SIZE)
#define UPDATE_DATA_IS_CHECK_OK(total_size,package_size) \
        (GET_UPDATE_DATA_SIZE(total_size) == package_size) /**< 判断数据包是否完整*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*更新表初始化*/
void update_data_table_init(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*更新表数据内容释放*/
void update_data_table_free(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*向更新记录表添加数据*/
void update_data_table_add(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint8_t *package, 
                            uint16_t package_num, uint16_t package_total, uint16_t package_size);

/*移除更新记录表中指定package*/
void update_data_table_remove(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, 
                            uint16_t package_num);

/*检查更新记录表是否已满*/
bool update_data_table_is_full(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*检查更新记录表有效数据包数量*/
uint16_t update_data_table_get_package_num(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, 
                                    uint16_t package_total);

/*获得更新数据头部信息*/
bool update_data_get_head_info(uint8_t *package, uint16_t size, uint16_t *package_total, 
                                uint16_t *package_num, uint16_t *package_size);

/*更新记录表数据写入二进制到文件*/
int update_data_table_write_binary(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, 
                                    const char *filename, int (*update_data_is_ok_callback)(void));

/*显示进度条*/
int update_data_table_show_progressbar(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size);

/*重置进度条*/
void update_data_table_reset_progressbar(void);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                