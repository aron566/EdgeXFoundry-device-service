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
#include "device_driver_parse_par.h"
/** Private typedef ----------------------------------------------------------*/
                                                                               
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
static DEV_DRIVER_INTERFACE_Typedef_t gateway_interface_par[] = 
{
  {
    .par_name               = "r_service_ver",
    .command                = 0x03,
    .command_addr           = 0x0000,
    .permissions            = READ_ONLY,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },
  {
    .par_name               = "r_driver_ver",
    .command                = 0x03,
    .command_addr           = 0x0001,
    .permissions            = READ_ONLY,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },          
  {         
    .par_name               = "rw_driver",
    .command                = 0x03|0x10,
    .command_addr           = 0x0002,
    .permissions            = READ_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },          
  {         
    .par_name               = "rw_service_config",
    .command                = 0x03|0x10,
    .command_addr           = 0x0002,
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
    .permissions            = UNKNOW,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },
};

/** Public variables ---------------------------------------------------------*/                                                                          
/** Private function prototypes ----------------------------------------------*/
static void get_mqtt_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_mqtt_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void get_modbus_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_modbus_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void get_private_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static void set_private_dev_value(void *input_data, void *out_data, VALUE_Type_t type);
static SET_DEV_VALUE_CALLBACK get_set_callback(PROTOCOL_Type_t protocol_type);
static GET_DEV_VALUE_CALLBACK get_get_callback(PROTOCOL_Type_t protocol_type);
/** Private variables --------------------------------------------------------*/
/*协议解析回调映射*/
static PROTOCOL_DECODE_CALLBACK_Typedef_t protocol_decoder_map[] = 
{
  {
    .protocol_type = MQTT_PROTO,
    .get_callback = get_mqtt_dev_value,
    .set_callback = set_mqtt_dev_value
  },
  {
    .protocol_type = MODBUS_RTU_PROTO,
    .get_callback = get_modbus_dev_value,
    .set_callback = set_modbus_dev_value
  },
  {
    .protocol_type = PRIVATE_PROTO,
    .get_callback = get_private_dev_value,
    .set_callback = set_private_dev_value
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
  * @brief   读取private设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void get_private_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

/**
  ******************************************************************
  * @brief   设置private设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static void set_private_dev_value(void *input_data, void *out_data, VALUE_Type_t type)
{

}

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
  * @brief   获取网关设备资源表
  * @param   [in]None.
  * @retval  DEV_DRIVER_INTERFACE_Typedef_t.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
DEV_DRIVER_INTERFACE_Typedef_t *get_gateway_device_resource(void)
{
  return gateway_interface_par;
}

/**
  ******************************************************************
  * @brief   网关设备驱动注册
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
int gateway_device_driver_register(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par)
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
  list_push_front(&node, GATEWAY_DEV_TYPE);
  printf("gatway device register.\n");
  return 0;
}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                