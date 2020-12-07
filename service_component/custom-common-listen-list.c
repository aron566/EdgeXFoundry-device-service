/**                                                                             
 *  @file custom-common-listen-list.c                                                   
 *                                                                              
 *  @date 2020年12月03日 00:59:46 星期四
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief None.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "custom-common-listen-list.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
static Listen_Node_Parent_Typedef_t *all_fd_type_map[FD_TYPE_MAX];
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
  
/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/                                                                        

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/                                                                              
                                                                                
/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/
/**
  ******************************************************************
  * @brief   初始化链表
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
void fd_list_table_init(void)
{
    for(int i = FILE_FD;i < FD_TYPE_MAX;i++)
    {
      all_fd_type_map[i] = NULL;
    }
}

/**
  ******************************************************************
  * @brief   增加一条数据到链表
  * @param   [in]node 需挂接的数据
  * @param   [in]type fd类型
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
void fd_list_add_to_list(Listen_Node_Parent_Typedef_t *node ,FD_Typedef_t type)
{
    if(type >= FD_TYPE_MAX || type < FILE_FD)
    {
        return;
    }
    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    if(it == NULL)
    {
        all_fd_type_map[type] = node;
        node->next = NULL;
        return;
    }
    while(it->next != NULL)
    {
      it = it->next;
    }
    it->next = node;
    node->next = NULL;
}

/**
  ******************************************************************
  * @brief   增加一条数据到链表尾部
  * @param   [in]node 需挂接的数据
  * @param   [in]type fd类型
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
void fd_list_push_back(FD_FUNC_MAP_Typedef_t *node ,FD_Typedef_t type)
{
    if(type >= FD_TYPE_MAX || type < FILE_FD)
    {
        return;
    }
    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    Listen_Node_Parent_Typedef_t *pnode = (Listen_Node_Parent_Typedef_t *)malloc(sizeof(Listen_Node_Parent_Typedef_t));
    memcpy(&pnode->node ,node ,sizeof(FD_FUNC_MAP_Typedef_t));
    if(it == NULL)
    {
        all_fd_type_map[type] = pnode;
        pnode->next = NULL;
        return;
    }

    /*找到尾部节点挂接*/
    while(it->next != NULL)
    {
      it = it->next;
    }
    it->next = pnode;
    pnode->next = NULL;
}

/**
  ******************************************************************
  * @brief   增加一条数据到链表头部
  * @param   [in]node 需挂接的数据
  * @param   [in]type fd类型
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
void fd_list_push_front(FD_FUNC_MAP_Typedef_t *node ,FD_Typedef_t type)
{
    if(type >= FD_TYPE_MAX || type < FILE_FD)
    {
        return;
    }
    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    Listen_Node_Parent_Typedef_t *pnode = (Listen_Node_Parent_Typedef_t *)malloc(sizeof(Listen_Node_Parent_Typedef_t));
    memcpy(&pnode->node ,node ,sizeof(FD_FUNC_MAP_Typedef_t));
    
    /*头部节点挂接*/
    pnode->next = it;
    all_fd_type_map[type] = pnode;
}

/**
  ******************************************************************
  * @brief   插入一条数据到链表
  * @param   [in]node 需挂接的数据
  * @param   [in]type fd类型
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
void fd_list_insert_node(FD_FUNC_MAP_Typedef_t *node ,FD_Typedef_t type ,int index)
{
    if(type >= FD_TYPE_MAX || type < FILE_FD)
    {
        return;
    }
    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    Listen_Node_Parent_Typedef_t *last_it = it;
    Listen_Node_Parent_Typedef_t *pnode = (Listen_Node_Parent_Typedef_t *)malloc(sizeof(Listen_Node_Parent_Typedef_t));
    memcpy(&pnode->node ,node ,sizeof(FD_FUNC_MAP_Typedef_t));

    if(fd_list_get_size(type) == 0)
    {
        all_fd_type_map[type] = pnode;
        pnode->next = NULL;
        return;
    }

    if(index == 0 || index == 1)
    {
        /*头部节点挂接*/
        pnode->next = it;
        all_fd_type_map[type] = pnode;
        return;
    }

    /*查找插入点*/
    int cnt = 1;
    while(it->next != NULL)
    {
      last_it = it;
      it = it->next;
      cnt++;
      if(cnt == index)
      {
          last_it->next = pnode;
          pnode->next = it;
          return;
      }
    }

    /*未找到加入到尾部*/
    it->next = pnode;
    pnode->next = NULL;
}

/**
  ******************************************************************
  * @brief   计算链表长度
  * @param   [in]type fd类型
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
int fd_list_get_size(FD_Typedef_t type)
{
    int cnt = 0;
    if(type >= FD_TYPE_MAX || type < FILE_FD)
    {
        return 0;
    }
    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    while(it != NULL)
    {
        cnt++;
        it = it->next;
    }
    return cnt;
}

/**
  ******************************************************************
  * @brief   删除指定类型链表
  * @param   [in]type fd类型
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
void fd_list_dele_list(FD_Typedef_t type)
{
    if(type >= FD_TYPE_MAX || type < FILE_FD)
    {
        return;
    }
    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    Listen_Node_Parent_Typedef_t *del = it;
    while(it != NULL)
    {
      it = it->next;     
      free(del);
      del = it;
    }

    all_fd_type_map[type] = NULL;
}

/**
  ******************************************************************
  * @brief   删除指定类节点
  * @param   [in]type fd细分类型
  * @param   [in]fd 描述符
  * @author  aron566
  * @version V1.1
  * @date    2020-10-12
  ******************************************************************
  */
void fd_list_dele_node(SOCKET_FD_Typedef_t fd_type ,int fd)
{
    Listen_Node_Parent_Typedef_t *it        = NULL;
    Listen_Node_Parent_Typedef_t *p_next    = NULL;
    if(fd_type < TCP_SERVER_SOCKET || fd_type > SOCKET_FD_TYPE_MAX)
    {
        return;
    }
    /*轮询所有类型FD*/
    for(int i = FILE_FD;i < FD_TYPE_MAX;i++)
    {
        if(all_fd_type_map[i] == NULL)
        {
            continue;
        }
        it = all_fd_type_map[i];
        if(it->node.fd_type == fd_type && it->node.node_fd == fd)
        {
            all_fd_type_map[i] = it->next;/**< 第二个节点挂接到首个节点下*/
            free(it);
            return;
        }
        else
        {
            /*自第二个节点轮询*/
            p_next = it->next;
            while(p_next != NULL)
            {
                if(p_next->node.fd_type == fd_type && p_next->node.node_fd == fd)
                {
                    it->next = p_next->next;/**< 第三个节点挂接到第一个下*/
                    free(p_next);
                    return;
                }
                it = p_next;
                p_next = p_next->next;
            }
        }
    }
}

/**
  ******************************************************************
  * @brief   找到指定类型的节点数据
  * @param   [in]type fd类型
  * @param   [in]fd fd描述符
  * @author  aron566
  * @version V1.0
  * @date    2020-09-22
  ******************************************************************
  */
FD_FUNC_MAP_Typedef_t *fd_list_find_node(FD_Typedef_t type ,int fd)
{
    if(fd < 0 || type < FILE_FD || type >= FD_TYPE_MAX)
    {
        return NULL;
    }

    Listen_Node_Parent_Typedef_t *it = all_fd_type_map[type];
    while(it != NULL)
    {
        if(it->node.node_fd == fd)
        {
            return &it->node;
        }
        it = it->next;
    }
    return NULL;
}                                                                                
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                