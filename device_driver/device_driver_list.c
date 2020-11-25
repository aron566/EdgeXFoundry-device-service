/**                                                                             
 *  @file device_driver_list.c                                                   
 *                                                                              
 *  @date 2020年11月10日 10:20:06 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 设备驱动链表.
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
#include "device_driver_list.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
static NODE_Typedef_t *all_list_head[LIST_TYPE_MAX];
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
  * @date    2020-11-12
  ******************************************************************
  */
void list_table_init(void)
{
    for(int i = 0;i < LIST_TYPE_MAX;i++)
    {
      all_list_head[i] = NULL;
    }
}

/**
  ******************************************************************
  * @brief   增加一条数据到链表
  * @param   [in]node 需挂接的数据
  * @param   [in]type 链表类型
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
void list_add_to_list(NODE_Typedef_t *node ,LIST_Type_t type)
{
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return;
    }
    NODE_Typedef_t *it = all_list_head[type];
    if(it == NULL)
    {
        all_list_head[type] = node;
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
  * @param   [in]type 链表类型
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
void list_push_back(NODE_Typedef_t *node ,LIST_Type_t type)
{
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return;
    }
    NODE_Typedef_t *it = all_list_head[type];
    NODE_Typedef_t *pnode = (NODE_Typedef_t *)malloc(sizeof(NODE_Typedef_t));
    memcpy(&pnode->node ,node ,sizeof(NODE_Typedef_t));
    if(it == NULL)
    {
        all_list_head[type] = pnode;
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
  * @param   [in]type 链表类型
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
void list_push_front(NODE_Typedef_t *node ,LIST_Type_t type)
{
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return;
    }
    NODE_Typedef_t *it = all_list_head[type];
    NODE_Typedef_t *pnode = (NODE_Typedef_t *)malloc(sizeof(NODE_Typedef_t));
    memcpy(&pnode->node ,node ,sizeof(NODE_Typedef_t));
    
    /*头部节点挂接*/
    pnode->next = it;
    all_list_head[type] = pnode;
}

/**
  ******************************************************************
  * @brief   插入一条数据到链表
  * @param   [in]node 需挂接的数据
  * @param   [in]type 链表类型
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
void list_insert_node(NODE_Typedef_t *node ,LIST_Type_t type ,int index)
{
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return;
    }
    NODE_Typedef_t *it = all_list_head[type];
    NODE_Typedef_t *last_it = it;
    NODE_Typedef_t *pnode = (NODE_Typedef_t *)malloc(sizeof(NODE_Typedef_t));
    memcpy(&pnode->node ,node ,sizeof(NODE_Typedef_t));

    if(list_get_size(type) == 0)
    {
        all_list_head[type] = pnode;
        pnode->next = NULL;
        return;
    }

    if(index == 0 || index == 1)
    {
        /*头部节点挂接*/
        pnode->next = it;
        all_list_head[type] = pnode;
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
  * @param   [in]type 链表类型
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
int list_get_size(LIST_Type_t type)
{
    int cnt = 0;
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return 0;
    }
    NODE_Typedef_t *it = all_list_head[type];
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
  * @param   [in]type 链表类型
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
void list_dele_list(LIST_Type_t type)
{
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return;
    }
    NODE_Typedef_t *it = all_list_head[type];
    NODE_Typedef_t *del = it;
    while(it != NULL)
    {
      it = it->next;     
      free(del);
      del = it;
    }
    all_list_head[type] = NULL;
}

/**
  ******************************************************************
  * @brief   找到指定类型的节点数据
  * @param   [in]type 链表类型
  * @param   [in]major_key 唯一键值
  * @author  aron566
  * @version V1.0
  * @date    2020-11-12
  ******************************************************************
  */
NODE_TYPE_STRUCT *list_find_node(LIST_Type_t type ,MAJOR_KEY_1 major_key_1, MAJOR_KEY_2 major_key_2)
{
    if(type >= LIST_TYPE_MAX || type < 0)
    {
        return NULL;
    }
    NODE_Typedef_t *it = all_list_head[type];
    while(it != NULL)
    {
        if(it->node.major_key_1 == major_key_1 && it->node.major_key_2 == major_key_2)
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
                                                                                