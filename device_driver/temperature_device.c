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
#include "device_driver_modbus_proto.h"
#include "device_driver_event_db.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
/** Private constants --------------------------------------------------------*/
static DEV_DRIVER_INTERFACE_Typedef_t resources_interface_par[] = 
{
  {
    .par_name               = "temperature",
    .command                = 0x03,
    .command_addr           = 0x0000,
    .value_type             = FLOAT32,
    .default_value          = 0,
    .permissions            = STORE_READ|READ_ONLY,
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
    .default_value          = 0,
    .permissions            = STORE_READ|READ_ONLY,
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
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_WRITE|PRIVATE_READ,
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
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_WRITE|PRIVATE_READ,
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
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_WRITE|PRIVATE_READ,
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
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_WRITE|PRIVATE_READ,
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
    .default_value          = 0,
    .permissions            = UNKNOW,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .interval_time          = 0,
    .unit                   = T_S
  },
};
/** Public variables ---------------------------------------------------------*/
/** Private function prototypes ----------------------------------------------*/
static int get_mqtt_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type);
static int set_mqtt_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type);
static int get_modbus_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type);
static int set_modbus_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type);
static int get_private_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type);
static int set_private_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type);

static SET_DEV_VALUE_CALLBACK get_set_callback(PROTOCOL_Type_t protocol_type);
static GET_DEV_VALUE_CALLBACK get_get_callback(PROTOCOL_Type_t protocol_type);
static int get_param_index(const char *parm);
static void query_callback(char** pr, int row, int column, void *callback_par);
static void device_event_recovery(DEV_INFO_Typedef_t *dev_info, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par);

static void read_msg_callback(uv_work_t *req, int status);
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
  * @brief   获取参数所在资源索引号
  * @param   [in]parm 参数名
  * @return  索引号.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-06
  ******************************************************************
  */
inline static int get_param_index(const char *parm)
{
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(parm, resources_interface_par[index].par_name) == 0)
    {
      return index;
    }
  }
  return -1;
}

/**
  ******************************************************************
  * @brief   轮询设备数据完成回调接口
  * @param   [in]req 请求参数
  * @param   [in]status 状态
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
static void read_msg_callback(uv_work_t *req, int status) 
{
  debug_print((uint8_t *)req->data, 7);
}

/**
  ******************************************************************
  * @brief   读取mqtt设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int get_mqtt_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type)
{
  const char *parm = (const char *)input_data;
  devsdk_commandresult *return_value = (devsdk_commandresult *)out_data;
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(parm, resources_interface_par[index].par_name) == 0)
    {
      /*获取设备数据*/

      // out_value->value = iot_data_alloc_i32 ((random () % 501) - 250);
    }
  }
  return 0;
}

/**
  ******************************************************************
  * @brief   设置mqtt设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int set_mqtt_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type)
{
  const char *parm = (const char *)input_data;
  const iot_data_t *set_value = (const iot_data_t *)set_data;
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(parm, resources_interface_par[index].par_name) == 0)
    {
      /*获取设备数据*/

      // out_value->value = iot_data_alloc_i32 ((random () % 501) - 250);
    }
  }
  return 0;
}

/**
  ******************************************************************
  * @brief   读取modbus设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int get_modbus_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type)
{
  MODBUS_PARSE_CODE_Typedef_t state = MODBUS_OK;
  const char *parm = (const char *)input_data;
  devsdk_commandresult *out_value = (devsdk_commandresult *)out_data;
  
  uint8_t addr = get_modbus_device_addr(dev_name);
  uint16_t reg_n = 1;
  uint16_t *reg_value = NULL;
  int index = get_param_index(parm);
  if(index == -1)
  {
    return -1;
  }

  /*检测权限*/
  if(READ_ONLY & resources_interface_par[index].permissions)
  {
    /*获取设备数据*/
    uint16_t reg_s = resources_interface_par[index].command_addr;
    state = device_driver_modbus_master_read(addr, reg_s, reg_n, &reg_value);
    if(state == MODBUS_OK)
    {
      if(reg_value != NULL)
      {
        out_value->value = iot_data_alloc_f32((float)(*reg_value)/10.0);
        free(reg_value);
      }
      return 0;
    }
  }
  else if(PRIVATE_READ & resources_interface_par[index].permissions)
  {
    /*读内部参数*/
    return get_private_dev_value(dev_name, input_data, out_data, type);
  }
  return -1;
}

/**
  ******************************************************************
  * @brief   读取private设备数值接口
  * @param   [in]input_data 请求参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int get_private_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type)
{
  const char *parm = (const char *)input_data;
  devsdk_commandresult *return_value = (devsdk_commandresult *)out_data;
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(parm, resources_interface_par[index].par_name) == 0)
    {
      /*获取设备数据*/

      // out_value->value = iot_data_alloc_i32 ((random () % 501) - 250);
    }
  }
  return 0;
}

/**
  ******************************************************************
  * @brief   设置private设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int set_private_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type)
{
  return 0;
}

/**
  ******************************************************************
  * @brief   设置modbus设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [in]set_data 返回数据
  * @param   [out]type 数据类型
  * @return  0设置成功.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int set_modbus_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type)
{
  UNUSED(type);
  MODBUS_PARSE_CODE_Typedef_t state = MODBUS_OK;
  const char *parm = (const char *)input_data;
  const iot_data_t *data = (const iot_data_t *)set_data;
  uint8_t addr = get_modbus_device_addr(dev_name);
  uint16_t reg_n = 1;

  int index = get_param_index(parm);
  if(index == -1)
  {
    return -1;
  }

  /*检测权限*/
  if(WRITE_ONLY & resources_interface_par[index].permissions)
  {
    uint16_t reg_s = resources_interface_par[index].command_addr;
    uint16_t reg_d = iot_data_ui16 (data);
    state = device_driver_modbus_master_write(addr, reg_s, reg_n, &reg_d);
    if(state == MODBUS_OK)
    {
      return 0;
    }
    return -1;
  }
  else if(PRIVATE_WRITE & resources_interface_par[index].permissions)
  {
    /*写内部参数*/
    return set_private_dev_value(dev_name, input_data, set_data, type);
  }
  return -1;
}

/**
  ******************************************************************
  * @brief   分配协议解析器-set
  * @param   [in]protocol_type 协议类型
  * @return  解析器.
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
  * @return  解析器.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
static GET_DEV_VALUE_CALLBACK get_get_callback(PROTOCOL_Type_t protocol_type)
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

/**
  ******************************************************************
  * @brief   设备事件参数查询结果回调
  * @param   [in]pr 表.
  * @param   [in]row 行数
  * @param   [in]column 列数
  * @param   [in]callback_par 参数
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-08
  ******************************************************************
  */
static void query_callback(char** pr, int row, int column, void *callback_par)
{
	if(row < 1)
	{
		printf("can't find this param name.\n");
		return;
	}
  DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par = (DEV_DRIVER_INTERFACE_Typedef_t *)callback_par;
  if(dev_resource_par == NULL)
  {
    return;
  }
  /*0地址号,1参数名,2当前值,3时间戳*/
	int i, j;
	for(i = 1; i <= row; i++)
	{
    j = i * column;
    dev_resource_par->default_value = (uint64_t)atoi(pr[j+2]);
	}
}

/**
  ******************************************************************
  * @brief   设备事件参数恢复
  * @param   [in]dev_info 设备信息.
  * @param   [in]dev_resource_par 设备资源
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-08
  ******************************************************************
  */
static void device_event_recovery(DEV_INFO_Typedef_t *dev_info, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par)
{
  if(dev_info == NULL || dev_resource_par == NULL)
  {
    return;
  }

  /*读取数据库*/
  QUERY_DATA_Typedef_t data;
  data.addr = (uint32_t)atoi(dev_info->dev_address);
  strncopy(data.param_name, dev_resource_par->par_name, 64);
  strncopy(data.table_name, dev_info->dev_type_name, 64);
  int ret = dev_driver_event_db_record_query(&data, query_callback, dev_resource_par);
  if(ret != 0)
  {
    /*查询失败，重建设备事件参数表*/
    INSERT_DATA_Typedef_t insert_data;
    memmove(&insert_data, &data, sizeof(BASE_DATA_Typedef_t));
    get_value_str(insert_data.value_current, &dev_resource_par->default_value, 64, dev_resource_par->value_type);
    insert_data.time_stamp = get_curent_time_s(CURRENT_TIME);
    dev_driver_event_db_record_insert(&insert_data);
    return;
  }
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
  * @return  DEV_DRIVER_INTERFACE_Typedef_t.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
const DEV_DRIVER_INTERFACE_Typedef_t *get_temperature_device_resource(void)
{
  return (const DEV_DRIVER_INTERFACE_Typedef_t *)resources_interface_par;
}

/**
  ******************************************************************
  * @brief   温湿度设备驱动注册
  * @param   [in]dev_info 设备信息
  * @param   [in]communication_par 设备通讯参数
  * @param   [in]dev_resource_par 设备资源信息
  * @return  None.
  * @author  aron566
  * @version V1.1
  * @date    2020-12-02
  ******************************************************************
  */
int temperature_device_driver_register(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par,
                                      DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par)
{
  /*查询*/
  NODE_Typedef_t *node_p = NULL;
  NODE_TYPE_STRUCT *p_node = NULL;

  MAJOR_KEY_1 major_key_1 = communication_par->protocol_type;
  MAJOR_KEY_2 major_key_2 = (uint32_t)atoi(dev_info->dev_address);
  DEVICE_Typedef_t dev_type = get_device_type(dev_info);
  if(dev_type == DEV_TYPE_MAX || major_key_1 == UNKNOW_PROTO)
  {
      printf("can't get the dev type or protocol.\n");
      return -1;
  }
  p_node = list_find_node(TEMPERATURE_DEV_TYPE, major_key_1, major_key_2);
  if(p_node == NULL)
  {
    node_p = (NODE_Typedef_t *)calloc(1, sizeof(NODE_Typedef_t)+(sizeof(DEV_DRIVER_INTERFACE_Typedef_t)*sizeof(resources_interface_par)));
    if(node_p == NULL)
    {
      printf("can't calloc memory for temperature dev.\n");
      return -1;
    }
    p_node = &node_p->node;
    p_node->major_key_1 = major_key_1;
    p_node->major_key_2 = major_key_2;
    memmove(p_node->dev_resource_par, resources_interface_par, sizeof(resources_interface_par));
    memmove(&p_node->communication_par, communication_par, sizeof(DEV_COMMUNICATION_PAR_Typedef_t));
    p_node->get_dev_value_callback = get_get_callback(communication_par->protocol_type);
    p_node->set_dev_value_callback = get_set_callback(communication_par->protocol_type);

    /*注册*/
    list_add_to_list(node_p ,TEMPERATURE_DEV_TYPE);

    printf("temperature device register successful.\n");
  }

  /*修改事件参数*/
  for(int index = 0; p_node->dev_resource_par[index].par_name != NULL; index++)
  {
    /*私有事件参数从数据库中恢复*/
    if(dev_resource_par->permissions & STORE_READ)
    {
      device_event_recovery(dev_info, dev_resource_par);
    }
    if(strcmp(p_node->dev_resource_par[index].par_name, dev_resource_par->par_name) == 0)
    {
      memmove(&p_node->dev_resource_par[index], dev_resource_par, sizeof(DEV_DRIVER_INTERFACE_Typedef_t));
    }
  }

  return 0;
}   

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                