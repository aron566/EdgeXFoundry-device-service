/**                                                                             
 *  @file device_driver_opt.c                                                   
 *                                                                              
 *  @date 2020年11月10日 10:11:36 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 设备驱动操作.
 *
 *  @details 设备驱动将依据设备名查找设备,特别指定设备名（协议-位置信息(应包含上级网关信息+地理位置)-设备名(可重复)-地址号或者序号(同名设备不可重复)）.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "device_driver_opt.h"
#include "device_driver_list.h"
#include "device_driver_port.h"
#include "device_driver_auto_event.h"
#include "device_driver_parse_par.h"
/** Private typedef ----------------------------------------------------------*/
                                                              
/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                    
/** Private function prototypes ----------------------------------------------*/
static NODE_TYPE_STRUCT *device_driver_opt_get_interface(const char *devname, iot_logger_t *lc);

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
  * @brief   获取设备驱动接口
  * @param   [in]devname 设备名.
  * @param   [in]lc 日志记录.
  * @return  pnode 设备驱动节点
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
static NODE_TYPE_STRUCT *device_driver_opt_get_interface(const char *devname, iot_logger_t *lc)
{
  NODE_TYPE_STRUCT *pnode = NULL;

  /*解析设备类型*/
  DEV_INFO_Typedef_t dev_info;
  int ret = parse_dev_name(devname, &dev_info);
  if(ret != 0)
  {
    iot_log__warn(lc, "can't parse dev name %s.", devname);
    return pnode;
  }
  DEVICE_Typedef_t dev_type = get_device_protocol_type(&dev_info);
  if(dev_type == DEV_TYPE_MAX)
  {
    iot_log__warn(lc, "can't parse dev type %s.", devname);
    return pnode;
  }

  /*查找设备地址*/
  MAJOR_KEY_1 major_key_1 = get_device_protocol_type(&dev_info);
  MAJOR_KEY_2 major_key_2 = (uint32_t)atoi(dev_info.dev_address);
  pnode = list_find_node(dev_type ,major_key_1, major_key_2);

  if(pnode == NULL)
  {
    iot_log__warn(lc, "no find this device in the listtable.");
    return pnode;
  }

  return pnode;
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
  * @brief   设备驱动get接口
  * @param   [in]devname 设备名.
  * @param   [in]param 请求参数.
  * @param   [in]readings 返回结果内存区.
  * @param   [in]lc 日志记录.
  * @return  0请求成功 ，-1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
int device_driver_opt_get(const char *devname, const char *param, devsdk_commandresult *readings, iot_logger_t *lc)
{
  iot_log_info(lc, "call device_driver_opt_get par %s.", param);
  NODE_TYPE_STRUCT *pnode = NULL;

  pnode = device_driver_opt_get_interface(devname, lc);
   /*防止访问无效内存区域*/
  if(pnode == NULL)
  {
    return -1;
  }
  if(pnode->get_dev_value_callback == NULL)
  {
    iot_log__warn(lc, "device have not get interface.");
    return -1;
  }
  VALUE_Type_t value_type = VALUE_TYPE_MAX;

  return pnode->get_dev_value_callback(devname, param, readings, &value_type);    
}

/**
  ******************************************************************
  * @brief   设备驱动set接口
  * @param   [in]devname 设备名.
  * @param   [in]param 请求参数.
  * @param   [in]values 设置值.
  * @param   [in]lc 日志记录.
  * @return  0请求成功 ，-1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
int device_driver_opt_set(const char *devname, const char *param, const iot_data_t *values, iot_logger_t *lc)
{
  iot_log_info(lc, "call device_driver_opt_set.");
  NODE_TYPE_STRUCT *pnode = NULL;

  pnode = device_driver_opt_get_interface(devname, lc);
  /*防止访问无效内存区域*/
  if(pnode == NULL)
  {
    return -1;
  }
  if(pnode->get_dev_value_callback == NULL)
  {
    iot_log__warn(lc, "device have not get interface.");
    return -1;
  }
  VALUE_Type_t value_type = VALUE_TYPE_MAX;
  return pnode->set_dev_value_callback(devname, param, values, &value_type);
}

/**
  ******************************************************************
  * @brief   设备驱动重配置接口
  * @param   [in]lc 日志记录.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
void device_driver_opt_discover(iot_logger_t *lc)
{
  iot_log_info(lc, "call device_driver_opt_discover.");
}

/**
  ******************************************************************
  * @brief   设备驱动重配置接口
  * @param   [in]lc 日志记录.
  * @param   [in]config 配置数据
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
void device_driver_opt_stop(iot_logger_t *lc, bool force)
{
  iot_log_info(lc, "call device_driver_opt_stop.");
}

/**
  ******************************************************************
  * @brief   设备驱动重配置接口
  * @param   [in]lc 日志记录.
  * @param   [in]config 配置数据
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
void device_driver_opt_reconfigure(iot_logger_t *lc, const iot_data_t *config)
{
  iot_log_info(lc, "call device_driver_opt_reconfigure.");
}

/**
  ******************************************************************
  * @brief   设备驱动初始化
  * @param   [in]lc 日志记录器.
  * @param   [in]config 配置数据.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
void device_driver_opt_init(iot_logger_t *lc, const iot_data_t *config)
{
  iot_log_info(lc, "start device driver now.");

  /*初始化链表*/
  list_table_init();

  /*初始化数据库*/
  dev_driver_event_db_init(lc);

  /*通讯端口初始化*/
  device_driver_com_init_port();

  /*创建自定义监听*/
  epoll_listen_create();

  /*modbus主站协议栈初始化*/
  device_driver_modbus_master_stack_init();

  /*注册设备驱动*/
  register_device_driver(lc);

  /*启动uv事件检测*/
  device_driver_uv_handler_start(lc);
}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                