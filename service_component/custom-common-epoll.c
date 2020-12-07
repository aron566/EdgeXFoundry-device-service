/**                                                                             
 *  @file custom-common-epoll.c                                                   
 *                                                                              
 *  @date 2020年12月03日 00:56:34 星期四
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief None.
 *
 *  @details 
 *          EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
 *          EPOLLOUT：表示对应的文件描述符可以写；
 *          EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
 *          EPOLLERR：表示对应的文件描述符发生错误；
 *          EPOLLHUP：表示对应的文件描述符被挂断；
 *          EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，事件就绪时，假设对事件没做处理，内核不会反复通知事件就绪
 *          EPOLLLT： 将EPOLL设为水平触发(Level Triggered)模式，事件就绪时，假设对事件没做处理，内核会反复通知事件就绪
 *          EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
 *
 *  @version V1.1
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <sys/types.h>      
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
/* Private includes ----------------------------------------------------------*/
#include "custom-common-epoll.h"
#include "custom-common-utilities.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
#define EPOLLEVENTS 100U    /**< 最大就绪事件处理数目*/
#define FDSIZE      1024U   /**< 最大监测fd*/                                                                                 
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static int g_epollfd = 0;/**< epoll描述符*/
static pthread_t epoll_thread_id;/**< epoll线程id*/
static volatile int epoll_listen_fd_cnt = 1;/**< 已在epoll中监控的fd数目统计*/
/** Private function prototypes ----------------------------------------------*/   
static void *epoll_do_epoll_start(void *data);/**< epoll线程*/

static void epoll_handle_events(int epollfd ,struct epoll_event *events ,int num);/**< 处理事件*/
static void epoll_handle_accpet(int epollfd ,int listenfd);/**< 允许tcp客户端连接*/

/*epoll事件增删改接口*/
static void epoll_modify_event(int fd ,uint32_t events);
static void epoll_delete_event(int fd ,uint32_t events);
static void epoll_add_event(int fd ,uint32_t events);
/** Private user code --------------------------------------------------------*/
                                                                                
/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/                                                                              

/**
  ******************************************************************
  * @brief   启动epoll线程
  * @param   [in]data
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
static void *epoll_do_epoll_start(void *data)
{
    UNUSED(data);

    struct epoll_event events[EPOLLEVENTS];
    int ret;

    int epollfd = get_epoll_fd();
    if(epollfd <= 0)
    {
        return NULL;
    }
    /*监测状态*/
    for(;;)
    {
        /*获取已经准备好的描述符事件*/
        ret = epoll_wait(epollfd ,events ,EPOLLEVENTS ,-1);

        /*处理*/
        epoll_handle_events(epollfd ,events ,ret);

        usleep(1000);
    }
    close(epollfd);
    return NULL;
}

/**
 * @brief 读事件
 * 
 * @param epollfd 
 * @param fd 
 * @param buf 
 */
static void epoll_do_read_port(int epollfd ,int fd)
{
    FD_FUNC_MAP_Typedef_t *pread = NULL;
    for(int i = FILE_FD;i < FD_TYPE_MAX;i++)
    {
        pread = fd_list_find_node((FD_Typedef_t)i ,fd);
        if(pread != NULL)
        {
            break;
        }
    }
    
    if(pread == NULL)
    {
        return;
    }
    if(pread->slot_read == NULL)
    {
        return;
    }
    pread->slot_read(epollfd ,fd);
}

/**
 * @brief 写事件
 * 
 * @param epollfd 
 * @param fd 
 * @param buf 
 */
static void epoll_do_write_port(int epollfd ,int fd)
{
    FD_FUNC_MAP_Typedef_t *pwrite = NULL;
    for(int i = FILE_FD;i < FD_TYPE_MAX;i++)
    {
        pwrite = fd_list_find_node((FD_Typedef_t)i ,fd);
        if(pwrite != NULL)
        {
            break;
        }
    }

    if(pwrite == NULL)
    {
        return;
    }
    if(pwrite->slot_write == NULL)
    {
        return;
    }
    pwrite->slot_write(epollfd ,fd);
}

/**
 * @brief 处理事件
 * 
 * @param epollfd 
 * @param events 
 * @param num 
 * @param listenfd 
 * @param buf 
 */
static void epoll_handle_events(int epollfd ,struct epoll_event *events ,int num)
{
    int fd;
    /*就绪列表进行遍历*/
    for(int i = 0;i < num;i++)
    {
        fd = events[i].data.fd;
        /*根据描述符的类型和事件类型进行处理*/
        // if ((fd == tcp_get_listen_fd()) &&(events[i].events & EPOLLIN))
        // {
        //     epoll_handle_accpet(epollfd ,fd);
        // }    
        // else if
        if (events[i].events & EPOLLIN)
        {
            epoll_do_read_port(epollfd ,fd);
        }
        else if (events[i].events & EPOLLOUT)
        {
            epoll_do_write_port(epollfd ,fd);
        }
    }
}

/**
 * @brief tcp链接监听
 * 
 * @param epollfd 
 * @param listenfd 
 */
static void epoll_handle_accpet(int epollfd ,int listenfd)
{
    UNUSED(epollfd);
    UNUSED(listenfd);
    // struct sockaddr_in cliaddr;
    // socklen_t cliaddrlen = (socklen_t)sizeof(struct sockaddr);
    // int clifd = accept(listenfd ,(struct sockaddr*)&cliaddr ,&cliaddrlen);
    // if(clifd == -1)
    // {
    //     perror("accpet error");
    // }   
    // else
    // {
    //     printf("accept a new client:fd-%d %s:%d\n" ,clifd ,inet_ntoa(cliaddr.sin_addr) ,cliaddr.sin_port);
    //     /*添加一个客户端描述符和事件*/
    //     FD_FUNC_MAP_Typedef_t node;
    //     node.node_fd = clifd;
    //     node.fd_type = TCP_CLIENT_SOCKET;
    //     node.slot_read = tcp_server_read_msg;
    //     node.slot_write = NULL;
    //     memcpy(&node.udp_src_addr ,&cliaddr ,sizeof(cliaddr));
    //     epoll_listen_register(SOCKET_FD ,&node);
    // }
}

/**
 * @brief 添加监测fd
 * 
 * @param epollfd 
 * @param fd 
 * @param events 事件 
 */
static void epoll_add_event(int fd ,uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    int epollfd = get_epoll_fd();
    if(epollfd <= 0)
    {
        return;
    }
    epoll_ctl(epollfd ,EPOLL_CTL_ADD ,fd ,&ev);
}

/**
 * @brief 删除监测中的fd
 * 
 * @param epollfd 
 * @param fd 
 * @param events 事件  
 */
static void epoll_delete_event(int fd ,uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    int epollfd = get_epoll_fd();
    if(epollfd <= 0)
    {
        return;
    }
    epoll_ctl(epollfd ,EPOLL_CTL_DEL ,fd ,&ev);
}

/**
 * @brief 修改fd 监测事件
 * 
 * @param epollfd 
 * @param fd 
 * @param events 事件 
 */
static void epoll_modify_event(int fd ,uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    int epollfd = get_epoll_fd();
    if(epollfd <= 0)
    {
        return;
    }
    epoll_ctl(epollfd ,EPOLL_CTL_MOD ,fd ,&ev);
}

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   向epoll注册监听FD
  * @param   [in]type fd分类
  * @param   [in]node node信息
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
void epoll_listen_register(FD_Typedef_t type ,FD_FUNC_MAP_Typedef_t *node)
{
    if(type < FILE_FD || type >= FD_TYPE_MAX || node == NULL)
    {
        return;
    }
    int epollfd = get_epoll_fd();
    if(epollfd <= 0)
    {
        return;
    }

    /*限制监听数量*/
    int cnt = epoll_listen_fd_cnt+1;
    if(cnt >= FDSIZE)
    {
        printf("listen size more than max size\n");
        return;
    }
    else
    {
        epoll_listen_fd_cnt = cnt;
    }
    
    fd_list_push_front(node ,type);
    epoll_add_event(node->node_fd ,EPOLLIN|EPOLLET);
}

/**
  ******************************************************************
  * @brief   epoll监测列表中注销FD
  * @param   [in]type fd分类
  * @param   [in]node node信息
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
void epoll_listen_unregister(SOCKET_FD_Typedef_t type ,int fd)
{
    if(type < FILE_FD || type >= FD_TYPE_MAX || fd <= 0)
    {
        return;
    }
    int epollfd = get_epoll_fd();
    if(epollfd == 0)
    {
        return;
    }
    
    /*限制监听数量*/
    int cnt = epoll_listen_fd_cnt-1;
    if(cnt <= 0)
    {
        printf("listen size less than min size.\n");
        return;
    }
    else
    {
        epoll_listen_fd_cnt = cnt;
    }
    epoll_delete_event(fd ,EPOLLIN|EPOLLET);
    fd_list_dele_node(type ,fd);
}

/**
 * @brief Get the epoll fd object
 * 
 * @return int 
 */
int get_epoll_fd(void)
{
    return g_epollfd;
}

/**
  ******************************************************************
  * @brief   EPOLL监听线程启动
  * @param   [in]data 参数
  * @author  aron566
  * @version V1.0
  * @date    2020-09-25
  ******************************************************************
  */
void epoll_stander_alone_start(void *data)
{
    int res = pthread_create(&epoll_thread_id, NULL, epoll_do_epoll_start, data);
    if(res != 0)
    {
        printf("create epoll monitor thread fail.\n");
        exit(res);
    }
    pthread_detach(epoll_thread_id);
    // pthread_join(epoll_thread_id);
}

/**
  ******************************************************************
  * @brief   EPOLL监听线程停止
  * @param   [in]None
  * @author  aron566
  * @version V1.0
  * @date    2020-09-25
  ******************************************************************
  */
void epoll_stander_alone_stop(void)
{
    epoll_listen_stop();
    pthread_cancel(epoll_thread_id);
}

/**
 * @brief 停止epoll监听
 * 
 */
void epoll_listen_stop(void)
{
    // tcp_server_close();
    // tcp_client_close();
    // udp_server_close();
    // udp_client_close();
    int epollfd = get_epoll_fd();
    if(epollfd == 0)
    {
        return;
    } 
    close(epollfd);
}

/**
 * @brief 创建epollfd描述符
 * 
 * @return true 创建成功
 * @return false 
 */
bool epoll_listen_create(void)
{
    /*创建一个epoll描述符*/
    g_epollfd = epoll_create(FDSIZE);

    if(g_epollfd <= 0)
    {
        printf("epoll create error.\n");
        return false;
    }
    return true;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                