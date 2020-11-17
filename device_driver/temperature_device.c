/**                                                                             
 *  @file temperature_device.c                                                   
 *                                                                              
 *  @date 2020年11月08日 20:24:38 星期天
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 温湿度设备驱动.
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
#include "temperature_device.h"
#include "device_driver_list.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
 
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
 
/** Private function prototypes ----------------------------------------------*/
static void get_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_dev_value(void *input_data, void *out_data, VALUE_Type_t type);   
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
  * @brief   读取设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void get_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   设置设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void set_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}
/**
  ******************************************************************
  * @brief   解析设备名称获取相关信息
  * @param   [in]dev_name
  * @retval  解析后数据
  * @author  aron566
  * @version V1.0
  * @date    2020-11-9
  ******************************************************************
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
  * @brief   获取温湿度设备数值
  * @param   [in]dev_name 设备名
  * @param   [in]req_param 请求参数
  * @author  aron566
  * @version V1.0
  * @date    2020-11-09
  ******************************************************************
  */
iot_data_t *get_temperature_device_value(const char *dev_name, const char *req_param)
{

}

/**
  ******************************************************************
  * @brief   温湿度设备驱动注册
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
void temperature_device_driver_register(void)
{

}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                