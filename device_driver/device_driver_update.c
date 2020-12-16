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
/*更新文件数据结构*/
typedef struct 
{
  UPDATE_DATA_RECORD_Typedef *table;
  uint16_t table_size;
  const char *filename;
  int (*update_data_is_ok_callback)(void);
}WRITE_DATA_Typedef_t;
                                                                              
/** Private macros -----------------------------------------------------------*/
#define USE_NORMAL_PROGRESS_BAR 0                                                                     
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static char progressbar[302+4]={'\0'};       
static uv_work_t uv_write_data_handle;                  
/** Private function prototypes ----------------------------------------------*/
static int printf_progress_bar(size_t process, size_t total);

static void write_binary_task_cb(uv_work_t *handler);      
static void write_binary_task_ok_cb(uv_work_t* handler, int status);

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
  * @return  进度.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
static int printf_progress_bar(size_t process, size_t total)
{
  static int last_count = -1;
  const char* stat = {"-\\|/"};
  int current_count = (process * 100) / total;
  if (current_count < 0)
      return 0;

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
  return current_count;
} 

/**
  ******************************************************************
  * @brief   写入二进制文件任务处理回调
  * @param   [in]req 句柄.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-15
  ******************************************************************
  */
static void write_binary_task_cb(uv_work_t *handler)
{
  WRITE_DATA_Typedef_t *wire_info = (WRITE_DATA_Typedef_t *)handler->data;
  uint16_t package_num = 0;
  uint16_t package_total = 0;
  uint32_t package_size = 0;
  for(uint16_t seq = 0; seq < wire_info->table[0].package_total; seq++)
  {
    if(wire_info->table[seq].is_empty == false)
    {
      /*取出参数*/
      if(update_data_get_head_info(wire_info->table[seq].data, UPDATE_DATA_HEAD_SIZE+1, &package_total, 
                                      &package_num, &package_size) == true)
      {
        file_write(wire_info->filename ,wire_info->table[seq].data+UPDATE_DATA_HEAD_SIZE ,1 ,(size_t)package_size ,READ_WRITE_APPEND_CREAT_FILE_B);
      }
    }
  }
}

/**
  ******************************************************************
  * @brief   写入二进制文件任务完成回调
  * @param   [in]req 句柄.
  * @param   [in]status 状态.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-15
  ******************************************************************
  */
static void write_binary_task_ok_cb(uv_work_t* handler, int status)
{
  UNUSED(status);
  WRITE_DATA_Typedef_t *wire_info = (WRITE_DATA_Typedef_t *)handler->data;
  wire_info->update_data_is_ok_callback();
  free(handler->data);
  printf("\n\n\nUPDATE THE DEVICE DRIVER DYNAMIC LIB OK!\n\n\n");
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
    table[index].package_size = 0;
  }
  /*进度条重置*/
  update_data_table_reset_progressbar();
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
      table[index].package_size = 0;
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
  * @param   [in]package_size 数据大小.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void update_data_table_add(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint8_t *package, 
                              uint16_t package_num, uint16_t package_total, uint32_t package_size)
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
  table[package_num].package_size = package_size;
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
  table[package_num].package_size = 0;
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
    if(table[index].is_empty == true || index >= table[index].package_total)
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
  * @brief   检查更新记录表有效数据包数量
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @param   [in]package_total 目标数据表总数
  * @return  有效数据包数量.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-15
  ******************************************************************
  */
uint16_t update_data_table_get_package_num(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, uint16_t package_total)
{
  uint16_t num = 0;
  if(table == NULL)
  {
    return num;
  }
  for(uint16_t index = 0; index < table_size; index++)
  {
    if(index >= package_total)
    {
      break;
    }
    if(table[index].package_total == package_total)
    {
      num++;
    }
  }
  return num;
}

/**
  ******************************************************************
  * @brief   获得更新数据头部信息
  * @param   [in]package 数据区域.
  * @param   [in]size 数据总大小.
  * @param   [out]package_total 头部信息描述数据总包数.
  * @param   [out]package_num 头部信息描述包号.
  * @param   [out]package_size 头部信息描述更新数据大小（去除头部信息大小）.
  * @return  true 已满.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-15
  ******************************************************************
  */
bool update_data_get_head_info(uint8_t *package, uint32_t size, uint16_t *package_total, 
                                uint16_t *package_num, uint32_t *package_size)
{
  if(package == NULL || size <= UPDATE_DATA_HEAD_SIZE)
  {
    return false;
  }
  *package_total  = common_get_modbus_u16_data(package, 0);
  *package_num    = common_get_modbus_u16_data(package, 2);
  *package_size   = common_get_modbus_u32_data(package, 4);
  return true;
}

/**
  ******************************************************************
  * @brief   更新记录表数据写入二进制到文件
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @param   [in]filename 文件名称.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
int update_data_table_write_binary(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size, const char *filename, int (*update_data_is_ok_callback)(void))
{
  if(filename == NULL)
  {
    return -1;
  }
  WRITE_DATA_Typedef_t *write_info = (WRITE_DATA_Typedef_t *)calloc(1, sizeof(WRITE_DATA_Typedef_t));
  write_info->filename = filename;
  write_info->table = table;
  write_info->table_size = table_size;
  write_info->update_data_is_ok_callback = update_data_is_ok_callback;
  uv_write_data_handle.data = write_info;
  uv_queue_work(uv_default_loop(), &uv_write_data_handle, write_binary_task_cb, write_binary_task_ok_cb);

  return 0;
}

/**
  ******************************************************************
  * @brief   显示进度条
  * @param   [in]table 更新数据记录表.
  * @param   [in]table_size 数据表大小.
  * @return  进度.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
int update_data_table_show_progressbar(UPDATE_DATA_RECORD_Typedef *table, uint16_t table_size)
{
  if(table == NULL)
  {
    return 0;
  }
  size_t package_total = 0;
  for(uint16_t index = 0; index < table_size; index++)
  {
    if(table[index].is_empty == false)
    {
      package_total = (size_t)table[index].package_total;  
    }
  }
  size_t process = (size_t)update_data_table_get_package_num(table, table_size, package_total);

  return printf_progress_bar(process, package_total);
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
                                                                                