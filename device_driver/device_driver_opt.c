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
  * @brief   设备驱动get接口
  * @param   [in]devname 设备名.
  * @param   [in]param 请求参数.
  * @param   [in]readings 返回结果内存区.
  * @param   [in]lc 日志记录.
  * @retval  0请求成功 ，-1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
int device_driver_opt_get(const char *devname, const char *param, devsdk_commandresult *readings, iot_logger_t *lc)
{
  iot_log_info(lc, "call device_driver_opt_get.");
    // if (strcmp (param, "xrot") == 0)
    // {
      // readings[i].value = iot_data_alloc_i32 ((random () % 501) - 250);
    // }
    // else if (strcmp (param, "yrot") == 0)
    // {
    //   readings[i].value = iot_data_alloc_i32 ((random () % 501) - 250);
    // }
    // else if (strcmp (param, "zrot") == 0)
    // {
      // readings[0].value = iot_data_alloc_i32 (250);
    // 
    // }
    // else
    // {
    //   buff = malloc (ERR_BUFSZ);
    //   snprintf (buff, ERR_BUFSZ, "Unknown parameter %s requested", param);
    //   iot_log_error (driver->lc, buff);
    //   * exception = iot_data_alloc_string (buff, IOT_DATA_TAKE);
    //   return false;
    // }
    
    return 0;    
}

/**
  ******************************************************************
  * @brief   设备驱动set接口
  * @param   [in]devname 设备名.
  * @param   [in]param 请求参数.
  * @param   [in]values 设置值.
  * @param   [in]lc 日志记录.
  * @retval  0请求成功 ，-1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-27
  ******************************************************************
  */
int device_driver_opt_set(const char *devname, const char *param, const iot_data_t *values, iot_logger_t *lc)
{
  iot_log_info(lc, "call device_driver_opt_set.");
  //  = iot_data_bool (values[i]);
  return 0;
}

/**
  ******************************************************************
  * @brief   设备驱动重配置接口
  * @param   [in]lc 日志记录.
  * @retval  None.
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
  * @retval  None.
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
  * @retval  None.
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
  * @retval  None.
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

    /*通讯端口初始化*/
    device_driver_com_init_port();

    /*注册设备驱动*/
    register_device_driver();

    /*启动事件检测*/
    device_driver_event_report_handler_start();
}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                