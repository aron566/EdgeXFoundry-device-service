/**                                                                             
 *  @file device_driver_event_db.h                                                    
 *                                                                              
 *  @date 2020年12月06日 22:46:56 星期天
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_EVENT_DB_H                                                          
#define DEVICE_DRIVER_EVENT_DB_H                                                          
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
typedef int (*QUERY_CALLBACK)(char** pr, int row, int column, void *callback_par);

/*基类数据*/
typedef struct 
{
    char table_name[64];    /**< 表名*/
    uint32_t addr;          /**< 地址号 */
    char param_name[64];    /**< 参数名 */
}BASE_DATA_Typedef_t;

/* 数据插入 */      
/*地址号,参数名,当前值,时间戳 PRIMARY KEY*/                                                          
typedef struct insert_data                                                               
{
    char table_name[64];    /**< 表名*/
    uint32_t addr;          /**< 地址号 */
    char param_name[64];    /**< 参数名 */
    char value_current[64]; /**< 当前值 */
    uint64_t time_stamp;    /**< 时间戳 */
}INSERT_DATA_Typedef_t;

/*数据更新*/
/*条件：地址号,参数名 更新字段：当前值,时间戳*/
typedef struct update_data
{
    char table_name[64];    /**< 表名*/
    uint32_t addr;          /**< 地址号*/
    char param_name[64];    /**< 参数名 */
    char value_current[64]; /**< 当前值*/
    uint64_t time_stamp;    /**< 时间戳 */
}UPDATE_DATA_Typedef_t;

/*customize set*/
/*condition: condition_name1 == condition_value1,condition_name2 == condition_value2 
            SET param_name = value1*/
typedef struct customize_set
{
    char table_name[64];
    char condition_name1[64];
    char condition_name2[64];
    char condition_value1[64];
    char condition_value2[64];
    char param_name1[64];
    char value1[64];
}CUSTOMIZE_SET_Typedef_t;

/*查询数据*/
typedef struct query_data
{
    char table_name[64];    /**< 表名*/
    uint32_t addr;          /**< 地址号*/
    char param_name[64];    /**< 参数名*/
}QUERY_DATA_Typedef_t;

/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*初始化数据库*/
void dev_driver_event_db_init(iot_logger_t *lc);

/*数据录入*/
int dev_driver_event_db_record_insert(INSERT_DATA_Typedef_t *data);

/*更新数据库*/
int dev_driver_event_db_record_update(UPDATE_DATA_Typedef_t *data);

/*自定义更新字段*/
int dev_driver_event_db_record_customize_set(CUSTOMIZE_SET_Typedef_t *data);

/*查询数据*/
int dev_driver_event_db_record_query(QUERY_DATA_Typedef_t *data, QUERY_CALLBACK get_result, void *callback_par);

/*查询自定义数据*/
int dev_driver_event_db_record_customize_query(const char *sql, QUERY_CALLBACK get_result, void *callback_par);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                