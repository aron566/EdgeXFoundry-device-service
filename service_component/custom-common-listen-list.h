/**                                                                             
 *  @file custom-common-listen-list.h                                                    
 *                                                                              
 *  @date 2020年12月03日 00:59:14 星期四
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_LISTEN_LIST_H                                                          
#define CUSTOM_COMMON_LISTEN_LIST_H                                                          
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
#include <netinet/in.h>                                                         
/** Private includes ---------------------------------------------------------*/
                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                       
/** Exported typedefines -----------------------------------------------------*/
/*FD分类*/
typedef enum
{
    FILE_FD = 0,        /**< 文件类*/
    SOCKET_FD,          /**< 网络类*/
    SERIAL_FD,          /**< 串口类*/
    FD_TYPE_MAX,
}FD_Typedef_t;

/*细分类*/
typedef enum
{
    /**< 网络类*/
    TCP_SERVER_SOCKET = 0,  /**< tcp服务器的套接字*/   
    TCP_CLIENT_SOCKET,      /**< tcp客户端的套接字*/         
    UDP_SERVER_SOCKET,      /**< udp服务器的套接字*/   
    UDP_CLIENT_SOCKET,      /**< udp客户端的套接字*/ 
    /**< 串口类*/
    SERIAL_ID_1,            /**< 串口1*/ 
    SERIAL_ID_2, 
    SERIAL_ID_3,
    SERIAL_ID_4,
    SERIAL_ID_5,
    SERIAL_ID_6,
    SERIAL_ID_7,
    SERIAL_ID_8,
    SERIAL_ID_9,        
    SOCKET_FD_TYPE_MAX,
}SOCKET_FD_Typedef_t;

typedef void (*p_write)(int epollfd ,int fd);
typedef void (*p_read)(int epollfd ,int fd);

typedef struct sockaddr_in SOCKET_ADDR_Typedef_t;
/*FD事件处理节点*/
typedef struct 
{
    int node_fd;
    SOCKET_FD_Typedef_t fd_type;
    SOCKET_ADDR_Typedef_t udp_src_addr;/**< 存放udp客户端IP*/
    p_write slot_write;
    p_read  slot_read;
}FD_FUNC_MAP_Typedef_t;

/*父节点信息*/
typedef struct Listen_Node_Parent
{  
    FD_FUNC_MAP_Typedef_t node;
	struct Listen_Node_Parent *next;
}Listen_Node_Parent_Typedef_t;                                                                       
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*链表初始化*/
void fd_list_table_init(void);  

/*向链表添加节点，需手动申请内存*/
void fd_list_add_to_list(Listen_Node_Parent_Typedef_t *node ,FD_Typedef_t type);

/*删除指定链表*/
void fd_list_dele_list(FD_Typedef_t type);

/*删除指定节点*/
void fd_list_dele_node(SOCKET_FD_Typedef_t fd_type ,int fd);

/*找到指定节点*/
FD_FUNC_MAP_Typedef_t *fd_list_find_node(FD_Typedef_t type ,int fd); 

/*获取链表长度*/
int fd_list_get_size(FD_Typedef_t type);

/*插入数据到链表*/
void fd_list_insert_node(FD_FUNC_MAP_Typedef_t *node ,FD_Typedef_t type ,int index);

/*加入数据到链表头部*/
void fd_list_push_front(FD_FUNC_MAP_Typedef_t *node ,FD_Typedef_t type);

/*加入数据到链表尾部*/
void fd_list_push_back(FD_FUNC_MAP_Typedef_t *node ,FD_Typedef_t type); 

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                