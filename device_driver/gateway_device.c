/**                                                                             
 *  @file gateway_device.c                                                   
 *                                                                              
 *  @date 2020年11月10日 10:23:10 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 网关设备驱动.
 *
 *  @details 云端控制更新设备驱动及动态增删设备，并且更新服务配置文件中的设备数目下发.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "gateway_device.h"
#include "device_driver_list.h"
/** Private typedef ----------------------------------------------------------*/
                                                                               
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
DEV_DRIVER_INTERFACE_Typedef_t gateway_interface_par[] = 
{
  {
    .par_name     = "r_service_ver",
    .command      = 0x03,
    .command_addr = 0x0000,
    .permissions  = READ_ONLY
  },
  {
    .par_name     = "r_driver_ver",
    .command      = 0x03,
    .command_addr = 0x0001,
    .permissions  = READ_ONLY
  },
  {
    .par_name     = "rw_driver",
    .command      = 0x10,
    .command_addr = 0x0002,
    .permissions  = READ_WRITE
  },
  {
    .par_name     = "rw_service_config",
    .command      = 0x03|0x10,
    .command_addr = 0x0002,
    .permissions  = READ_WRITE
  },
  {
    .par_name     = NULL,
    .command      = 0x00,
    .command_addr = 0x0000,
    .permissions  = UNKNOW
  },
};
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                                
/** Private function prototypes ----------------------------------------------*/
static void get_mqtt_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_mqtt_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void get_modbus_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_modbus_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void get_customize_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_customize_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
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
  * @brief   读取mqtt设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void get_mqtt_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   设置mqtt设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void set_mqtt_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   读取modbus设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void get_modbus_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   设置modbus设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void set_modbus_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   读取customize设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void get_customize_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   设置customize设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void set_customize_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

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
  * @brief   网关设备驱动注册
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
void gateway_device_driver_register(void)
{
  NODE_TYPE_STRUCT node;
  node.major_key = MQTT_PROTO;
  node.get_dev_value_callback = get_mqtt_dev_value;
  node.set_dev_value_callback = set_mqtt_dev_value;
  strncpy(node.dev_name, "common" ,128);
  node.dev_name[127] = '\0';
  
}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                