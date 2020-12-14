/**                                                                             
 *  @file device_driver_update.c                                                   
 *                                                                              
 *  @date 2020年12月15日 00:04:46 星期二
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
#include "device_driver_update.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
#define USE_NORMAL_PROGRESS_BAR 0                                                                     
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static char progressbar[302+4]={'\0'};                                                                       
/** Private function prototypes ----------------------------------------------*/
                                                                                
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
  * @brief   更新进度展示
  * @param   [in]process 当前进度.
  * @param   [in]total 总进度.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
static void printf_progress_bar(size_t process, size_t total)
{
    static int last_count = -1;
    const char* stat = {"-\\|/"};
    int current_count = (process * 100) / total;
    if (current_count < 0)
        return;

    /*进度不同则刷新输出*/
    if(last_count != current_count)
    {
        last_count = current_count;
#if !USE_NORMAL_PROGRESS_BAR
        /*类python进度条，每打印一个字符补偿2个字符否则无法左对齐103等宽*/
        strcat(progressbar ,"█");
        printf("[%-*s][%%%d]%c\r" ,current_count*2+103 ,progressbar ,current_count ,stat[current_count%4]);
#else
        /*普通进度条，固定左对齐101等宽*/
        progressbar[current_count] = '#';/*替换█--Alt+9609~9600*/
        printf("[%-101s][%%%d]%c\r" ,progressbar ,current_count ,stat[current_count%4]);
#endif
    }

    fflush(stdout);
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
  * @brief   初始化更新文件记录表
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_init(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size)
{
  if(table == NULL)
  {
    return;
  }
  for(uint16_t index = 0; index < table_size; index++)
  {
    table[index].is_empty = true;
    table[index].data = NULL;
    table[index].package_total = 0;
    table[index].package_sub = 0;
  }
}

/**
  ******************************************************************
  * @brief   释放更新文件记录表的数据缓冲区
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_free(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size)
{
  if(table == NULL)
  {
    return;
  }
  for(uint16_t index = 0; index < table_size; index++)
  {
    if(table[index].is_empty == false)
    {
      free(table[index].data);
      table[index].data = NULL;
      table[index].is_empty = true;
      table[index].package_total = 0;
      table[index].package_sub = 0;
    }
  }
}

/**
  ******************************************************************
  * @brief   添加新的更新文件到更新记录表
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @param   [in]package 新的数据.
  * @param   [in]package_num 子包号.
  * @param   [in]package_total 总包号.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_add(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint8_t *package, 
                              uint16_t package_num, uint16_t package_total)
{
  if(table == NULL || package == NULL || package_num > table_size)
  {
    return;
  }
  if(table[package_num].is_empty == false)
  {
    update_data_table_remove(table, table_size, package_num);
  }
  table[package_num].data = package;
  table[package_num].is_empty = false;
  table[package_num].package_sub = package_num;
  table[package_num].package_total = package_total;
}

/**
  ******************************************************************
  * @brief   移除更新记录表指定子包号
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @param   [in]package_num 子包号.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_remove(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint16_t package_num)
{
  if(table == NULL || package_num > table_size)
  {
    return;
  }
  if(table[package_num].is_empty == true || table[package_num].data != NULL)
  {
    return;
  }
  free(table[package_num].data);
  table[package_num].data = NULL;
  table[package_num].is_empty = true;
  table[package_num].package_sub = 0;
  table[package_num].package_total = 0;
}

/**
  ******************************************************************
  * @brief   检查更新记录表是否已满
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @return  true 已满.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
bool update_data_table_is_full(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size)
{
  if(table == NULL)
  {
    return false;
  }
  /*是否只有一包*/
  if(table[0].package_total == table[0].package_sub+1 && table[0].is_empty == false)
  {
    return true;
  }
  for(uint16_t index = 0; index < table_size; index++)
  {
    if(table[index].is_empty == true)
    {
      if(table[index-1].package_total == table[index-1].package_sub+1)
      {
        return true;
      }
      break;
    }
  }
  return false;
}

/**
  ******************************************************************
  * @brief   显示进度条
  * @param   [in]process 当掐进度.
  * @param   [in]total 总进度.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_show_progressbar(size_t process, size_t total)
{
  printf_progress_bar(process, total);
}

/**
  ******************************************************************
  * @brief   重置进度条
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_reset_progressbar(void)
{
  memset(progressbar, '\0', sizeof(progressbar));
}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                