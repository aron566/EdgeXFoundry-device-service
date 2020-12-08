/**                                                                             
 *  @file device_driver_list.h                                                    
 *                                                                              
 *  @date 2020年11月10日 10:19:34 星期二
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.1                                                               
 */                                                                             
#ifndef DEVICE_DRIVER_LIST_H                                                          
#define DEVICE_DRIVER_LIST_H                                                          
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
#include "device_driver_list_config.h"  /**< 链表的数据结构配置文件*/                                                                
/** Private defines ----------------------------------------------------------*/
#define LIST_TYPE_MAX   DEV_TYPE_MAX    /**< 链表的最大分类*/
#define MAJOR_KEY_1     PROTOCOL_Type_t /**< 查找节点的唯一键值标识*/
#define MAJOR_KEY_2     uint32_t        /**< 查找节点的第二键值标识*/
/** Exported typedefines -----------------------------------------------------*/
/*链表分类*/
typedef DEVICE_Typedef_t LIST_Type_t;

/*节点数据结构*/
typedef DEV_NODE_FUNC_Type_t NODE_TYPE_STRUCT;   

/*节点信息*/
typedef struct NODE_TYPE
{  
	struct NODE_TYPE *next;
    NODE_TYPE_STRUCT node;
}NODE_Typedef_t;                                                                       
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
/*链表初始化*/
void list_table_init(void);  

/*向链表添加节点，需手动申请内存*/
void list_add_to_list(NODE_Typedef_t *node ,LIST_Type_t type);

/*删除指定链表*/
void list_dele_list(LIST_Type_t type);

/*找到指定节点*/
NODE_TYPE_STRUCT *list_find_node(LIST_Type_t type ,MAJOR_KEY_1 major_key_1, MAJOR_KEY_2 major_key_2); 

/*找到指定索引号节点*/
NODE_TYPE_STRUCT *list_find_index_node(LIST_Type_t type ,uint32_t index); 

/*获取链表长度*/
uint32_t list_get_size(LIST_Type_t type);

/*插入数据到链表*/
void list_insert_node(NODE_TYPE_STRUCT *node ,LIST_Type_t type ,int index);

/*加入数据到链表头部*/
void list_push_front(NODE_TYPE_STRUCT *node ,LIST_Type_t type);

/*加入数据到链表尾部*/
void list_push_back(NODE_TYPE_STRUCT *node ,LIST_Type_t type);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                