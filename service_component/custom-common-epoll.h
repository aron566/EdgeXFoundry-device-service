/**                                                                             
 *  @file custom-common-epoll.h                                                    
 *                                                                              
 *  @date 2020年12月03日 00:57:56 星期四
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COMMON_EPOLL_H                                                          
#define CUSTOM_COMMON_EPOLL_H                                                          
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
#include "custom-common-listen-list.h"                                                                               
/** Private defines ----------------------------------------------------------*/
                                                                    
/** Exported typedefines -----------------------------------------------------*/
                                                                       
/** Exported constants -------------------------------------------------------*/
                                                                                
/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*epoll监测创建*/
bool epoll_listen_create(void);

/*获取建立的epollFD*/
int get_epoll_fd(void);

/*向epoll注册监听FD*/
void epoll_listen_register(FD_Typedef_t type ,FD_FUNC_MAP_Typedef_t *node);
/*epoll监测列表中注销FD*/
void epoll_listen_unregister(SOCKET_FD_Typedef_t type ,int fd);

/*启动epoll监听线程*/
void epoll_stander_alone_start(void *data);
/*停止epoll监听线程*/
void epoll_stander_alone_stop(void);

/*停止epoll监听*/
void epoll_listen_stop(void);

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                