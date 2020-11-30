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
#include "device_driver_parse_par.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
 
/** Private constants --------------------------------------------------------*/
static DEV_DRIVER_INTERFACE_Typedef_t temperature_interface_par[] = 
{
  {
    .par_name               = "temperature",
    .command                = 0x03,
    .command_addr           = 0x0000,
    .value_type             = FLOAT32,
    .permissions            = READ_ONLY,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },
  {
    .par_name               = "humidity",
    .command                = 0x03,
    .command_addr           = 0x0001,
    .value_type             = FLOAT32,
    .permissions            = READ_ONLY,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },          
  {         
    .par_name               = "temperaturemin",
    .command                = 0x03|0x10,
    .command_addr           = 0x0002,
    .value_type             = FLOAT32,
    .permissions            = READ_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },          
  {         
    .par_name               = "humiditymin",
    .command                = 0x03|0x10,
    .command_addr           = 0x0003,
    .value_type             = FLOAT32,
    .permissions            = READ_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },  
  {         
    .par_name               = "temperaturemax",
    .command                = 0x03|0x10,
    .command_addr           = 0x0004,
    .value_type             = FLOAT32,
    .permissions            = READ_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  }, 
  {         
    .par_name               = "humiditymax",
    .command                = 0x03|0x10,
    .command_addr           = 0x0005,
    .value_type             = FLOAT32,
    .permissions            = READ_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },         
  {         
    .par_name               = NULL,
    .command                = 0x00,
    .command_addr           = 0x0000,
    .value_type             = VALUE_TYPE_MAX,
    .permissions            = UNKNOW,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },
};
/** Public variables ---------------------------------------------------------*/
/** Private function prototypes ----------------------------------------------*/
static void get_modbus_dev_value(const void *input_data, void *out_data, VALUE_Type_t *type);
static void set_modbus_dev_value(const void *input_data, void *out_data, VALUE_Type_t *type);
static SET_DEV_VALUE_CALLBACK get_set_callback(PROTOCOL_Type_t protocol_type);
static GET_DEV_VALUE_CALLBACK get_get_callback(PROTOCOL_Type_t protocol_type);   
/** Private variables --------------------------------------------------------*/
/*协议解析回调映射*/
static PROTOCOL_DECODE_CALLBACK_Typedef_t protocol_decoder_map[] = 
{
  {
    .protocol_type = MQTT_PROTO,
    .get_callback = NULL,
    .set_callback = NULL
  },
  {
    .protocol_type = MODBUS_RTU_PROTO,
    .get_callback = get_modbus_dev_value,
    .set_callback = set_modbus_dev_value
  },
  {
    .protocol_type = PRIVATE_PROTO,
    .get_callback = NULL,
    .set_callback = NULL
  },
  {
    .protocol_type = UNKNOW_PROTO,
    .get_callback = NULL,
    .set_callback = NULL
  },
};   
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
static void get_modbus_dev_value(const void *input_data, void *out_data, VALUE_Type_t *type)
{
  const char *parm = (const char *)input_data;
  devsdk_commandresult *out_value = (devsdk_commandresult *)out_data;
  for(int index = 0; temperature_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(parm, temperature_interface_par[index].par_name) == 0)
    {
      /*获取设备数据*/

      out_value->value = iot_data_alloc_i32 ((random () % 501) - 250);
    }
  }
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
static void set_modbus_dev_value(const void *input_data, void *out_data, VALUE_Type_t *type)
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


/**
  ******************************************************************
  * @brief   分配协议解析器-set
  * @param   [in]protocol_type 协议类型
  * @retval  解析器.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
static SET_DEV_VALUE_CALLBACK get_set_callback(PROTOCOL_Type_t protocol_type)
{
  if(protocol_type == UNKNOW_PROTO)
  {
      return NULL;
  }
  for(int index = 0; protocol_decoder_map[index].protocol_type != UNKNOW_PROTO; index++)
  {
    if(protocol_type == protocol_decoder_map[index].protocol_type)
    {
      return protocol_decoder_map[index].set_callback;
    }
  }
  return NULL;
}

/**
  ******************************************************************
  * @brief   分配协议解析器-get
  * @param   [in]protocol_type 协议类型
  * @retval  解析器.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
static SET_DEV_VALUE_CALLBACK get_get_callback(PROTOCOL_Type_t protocol_type)
{
  if(protocol_type == UNKNOW_PROTO)
  {
      return NULL;
  }
  for(int index = 0; protocol_decoder_map[index].protocol_type != UNKNOW_PROTO; index++)
  {
    if(protocol_type == protocol_decoder_map[index].protocol_type)
    {
      return protocol_decoder_map[index].get_callback;
    }
  }
  return NULL;
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
  * @brief   获取温湿度设备数值
  * @param   [in]dev_name 设备名
  * @param   [in]req_param 请求参数
  * @author  aron566
  * @version V1.0
  * @date    2020-11-09
  ******************************************************************
  */
void *get_temperature_device_value(const char *dev_name, const char *req_param)
{
  return NULL;
}

/**
  ******************************************************************
  * @brief   获取温湿度设备资源表
  * @param   [in]None.
  * @retval  DEV_DRIVER_INTERFACE_Typedef_t.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
DEV_DRIVER_INTERFACE_Typedef_t *get_temperature_device_resource(void)
{
  return temperature_interface_par;
}
/**
  ******************************************************************
  * @brief   温湿度设备驱动注册
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
int temperature_device_driver_register(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par)
{
  NODE_TYPE_STRUCT node;
  node.major_key_1 = communication_par->protocol_type;
  node.major_key_2 = (uint32_t)atoi(dev_info->dev_address);
  DEVICE_Typedef_t dev_type = get_device_type(dev_info);
  if(dev_type == DEV_TYPE_MAX || node.major_key_1 == UNKNOW_PROTO)
  {
      printf("can't get the dev type or protocol.\n");
      return -1;
  }
  memcpy(&node.communication_par, communication_par, sizeof(DEV_COMMUNICATION_PAR_Typedef_t));
  node.get_dev_value_callback = get_get_callback(communication_par->protocol_type);
  node.set_dev_value_callback = get_set_callback(communication_par->protocol_type);

  /*注册*/
  list_push_front(&node, TEMPERATURE_DEV_TYPE);
  printf("temperature device register.\n");
  return 0;
}   
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                