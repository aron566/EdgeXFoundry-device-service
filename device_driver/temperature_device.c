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
/*事件上报确认*/
static bool dev_online_confirm(const char *dev_name, const char *param_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data);
static bool temperature_hi_confirm(const char *dev_name, const char *param_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data);
static bool humidity_hi_confirm(const char *dev_name, const char *param_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data);
static bool temperature_low_confirm(const char *dev_name, const char *param_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data);
static bool humidity_low_confirm(const char *dev_name, const char *param_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data);

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
    .report_event_confirm   = NULL,
    .start_time             = 0,
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
    .report_event_confirm   = NULL,
    .start_time             = 0,
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
    .report_event_confirm   = NULL,
    .start_time             = 0,
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
    .report_event_confirm   = NULL,
    .start_time             = 0,
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
    .report_event_confirm   = NULL,
    .start_time             = 0,
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
    .report_event_confirm   = NULL,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },   
  {         
    .par_name               = "temperatureHI",
    .command                = 0x03,
    .command_addr           = 0x0006,
    .value_type             = FLOAT32,
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_READ|CONFIRM,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .report_event_confirm   = temperature_hi_confirm,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  }, 
  {         
    .par_name               = "humidityHI",
    .command                = 0x03,
    .command_addr           = 0x0007,
    .value_type             = FLOAT32,
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_READ|CONFIRM,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .report_event_confirm   = humidity_hi_confirm,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },
  {         
    .par_name               = "temperatureLOW",
    .command                = 0x03,
    .command_addr           = 0x0008,
    .value_type             = FLOAT32,
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_READ|CONFIRM,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .report_event_confirm   = temperature_low_confirm,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  }, 
  {         
    .par_name               = "humidityLOW",
    .command                = 0x03,
    .command_addr           = 0x0009,
    .value_type             = FLOAT32,
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_READ|CONFIRM,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .report_event_confirm   = humidity_low_confirm,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },  
  {         
    .par_name               = "online_state",
    .command                = 0x03,
    .command_addr           = 0x000A,
    .value_type             = UINT64,
    .default_value          = 30,
    .permissions            = PRIVATE_READ|CONFIRM,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .report_event_confirm   = dev_online_confirm,
    .start_time             = 0,
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
    .report_event_confirm   = NULL,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },
};
/** Public variables ---------------------------------------------------------*/
/** Private function prototypes ----------------------------------------------*/
/*协议-get-put接口*/
static int get_mqtt_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type);
static int set_mqtt_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type);
static int get_modbus_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type);
static int set_modbus_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type);
static int get_private_dev_value(const char *dev_name, const void *input_data, void *out_data, VALUE_Type_t *type);
static int set_private_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type);

static SET_DEV_VALUE_CALLBACK get_set_callback(PROTOCOL_Type_t protocol_type);
static GET_DEV_VALUE_CALLBACK get_get_callback(PROTOCOL_Type_t protocol_type);
static int get_param_index(const char *param_name);
static int recovery_query_callback(char** pr, int row, int column, void *callback_par);
static void device_event_recovery(DEV_INFO_Typedef_t *dev_info, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par);
static int update_db_data(DEV_INFO_Typedef_t *dev_info, const char *param_name, const iot_data_t *data);
static int online_query_callback(char** pr, int row, int column, void *callback_par);
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
  * @param   [in]param_name 参数名
  * @return  索引号.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-06
  ******************************************************************
  */
inline static int get_param_index(const char *param_name)
{
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(param_name, resources_interface_par[index].par_name) == 0)
    {
      return index;
    }
  }
  return -1;
}

/**
  ******************************************************************
  * @brief   设备在线事件参数查询结果回调
  * @param   [in]pr 表.
  * @param   [in]row 行数
  * @param   [in]column 列数
  * @param   [in]callback_par 参数
  * @return  0 正常 -1错误.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static int online_query_callback(char** pr, int row, int column, void *callback_par)
{
	if(row < 1)
	{
		printf("%s %s can't find this param name.\n", __FILE__, __FUNCTION__);
		return -1;
	}
  uint64_t *time = (uint64_t *)callback_par;
  if(time == NULL)
  {
    return -1;
  }
  /*0地址号,1参数名,2当前值,3时间戳*/
	int i, j;
  uint64_t time_stamp = 0;
	for(i = 1; i <= row; i++)
	{
    j = i * column;
    (*time) = (uint64_t)atoi(pr[j+3]);
    break;
	}
  return 0;
}

/**
  ******************************************************************
  * @brief   确认在线事件
  * @param   [in]dev_name 设备名.
  * @param   [in]param_name 参数名.
  * @param   [in]dev_resource 设备总资源.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static bool dev_online_confirm(const char *dev_name, const char *param_name, 
                                DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data) 
{
  devsdk_commandresult *results = (devsdk_commandresult *)data;
  int index = get_param_index(param_name);
  /*获取当前数值更新时间*/
  /*读取数据库*/
  QUERY_DATA_Typedef_t query_data;
  DEV_INFO_Typedef_t dev_info;
  uint64_t time = 0;
  int ret = parse_dev_name(dev_name, &dev_info);
  if(ret != 0)
  {
    return false;
  }
  query_data.addr = (uint32_t)get_device_addr(&dev_info);
  strncopy(query_data.param_name, "temperature", 64);
  strncopy(query_data.table_name, dev_info.dev_type_name, 64);
  ret = dev_driver_event_db_record_query(&query_data, online_query_callback, &time);
  if(ret != 0)
  {
    return false;
  }

  /*计算时间差*/
  uint64_t current_time = get_current_time_s(CURRENT_TIME);
  uint64_t diff = (uint64_t)abs(current_time - time);

  INTERVAL_TIME_Typedef_t interval_data;
  interval_data.interval_time = dev_resource[index].interval_time;
  interval_data.unit = dev_resource[index].unit;
  uint64_t diff_min = time_base2ms(&interval_data);

  /*更新，距离最近一次更新的时间*/
  dev_resource[index].default_value = diff;

  /*离线上报*/
  if(diff > diff_min)
  {
    if(results->value != NULL)
    {
      iot_data_free(results->value);
    }
    results->value = common_value2iot_data(&diff, dev_resource[index].value_type);
    return true;
  }
  return false;
}

/**
  ******************************************************************
  * @brief   确认高温事件
  * @param   [in]dev_name 设备名.
  * @param   [in]param_name 参数名.
  * @param   [in]dev_resource 设备总资源.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static bool temperature_hi_confirm(const char *dev_name, const char *param_name, 
                                    DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data) 
{
  devsdk_commandresult *results = (devsdk_commandresult *)data;
  /*获取当前温度数值*/
  int index = get_param_index("temperature");
  if(index == -1)
  {
    return true;
  }
  
  int hi_index = get_param_index("temperaturemax");
  if(hi_index == -1)
  {
    return true;
  }

  /*比较*/
  if(dev_resource[index].default_value > dev_resource[hi_index].default_value)
  {
    /*出现高值更新*/
    if(results->value != NULL)
    {
      iot_data_free(results->value);
    }
    results->value = common_value2iot_data(&dev_resource[index].default_value, dev_resource[index].value_type);
    return true;
  }
  return false;
}

/**
  ******************************************************************
  * @brief   确认高湿事件
  * @param   [in]dev_name 设备名.
  * @param   [in]param_name 参数名.
  * @param   [in]dev_resource 设备总资源.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static bool humidity_hi_confirm(const char *dev_name, const char *param_name, 
                                  DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data) 
{
  /*获取当前湿度数值*/
  devsdk_commandresult *results = (devsdk_commandresult *)data;
  /*获取当前温度数值*/
  int index = get_param_index("humidity");
  if(index == -1)
  {
    return true;
  }
  
  int hi_index = get_param_index("humiditymax");
  if(hi_index == -1)
  {
    return true;
  }

  /*比较*/
  if(dev_resource[index].default_value > dev_resource[hi_index].default_value)
  {
    /*出现高值更新*/
    if(results->value != NULL)
    {
      iot_data_free(results->value);
    }
    results->value = common_value2iot_data(&dev_resource[index].default_value, dev_resource[index].value_type);
    return true;
  }
  return false;
}

/**
  ******************************************************************
  * @brief   确认低温事件
  * @param   [in]dev_name 设备名.
  * @param   [in]param_name 参数名.
  * @param   [in]dev_resource 设备总资源.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static bool temperature_low_confirm(const char *dev_name, const char *param_name, 
                                      DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data) 
{
  /*获取当前温度数值*/
  devsdk_commandresult *results = (devsdk_commandresult *)data;
  /*获取当前温度数值*/
  int index = get_param_index("temperature");
  if(index == -1)
  {
    return true;
  }
  
  int low_index = get_param_index("temperaturemin");
  if(low_index == -1)
  {
    return true;
  }

  /*比较*/
  if(dev_resource[index].default_value < dev_resource[low_index].default_value)
  {
    /*出现低值更新*/
    if(results->value != NULL)
    {
      iot_data_free(results->value);
    }
    results->value = common_value2iot_data(&dev_resource[index].default_value, dev_resource[index].value_type);
    return true;
  }
  return false;
}

/**
  ******************************************************************
  * @brief   确认低湿事件
  * @param   [in]dev_name 设备名.
  * @param   [in]param_name 参数名.
  * @param   [in]dev_resource 设备总资源.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static bool humidity_low_confirm(const char *dev_name, const char *param_name, 
                                  DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data) 
{
  /*获取当前湿度数值*/
  devsdk_commandresult *results = (devsdk_commandresult *)data;
  /*获取当前温度数值*/
  int index = get_param_index("humidity");
  if(index == -1)
  {
    return true;
  }
  
  int low_index = get_param_index("humiditymin");
  if(low_index == -1)
  {
    return true;
  }

  /*比较*/
  if(dev_resource[index].default_value < dev_resource[low_index].default_value)
  {
    /*出现低值更新*/
    if(results->value != NULL)
    {
      iot_data_free(results->value);
    }
    results->value = common_value2iot_data(&dev_resource[index].default_value, dev_resource[index].value_type);
    return true;
  }
  return false;
}

/**
  ******************************************************************
  * @brief   更新数据库数据
  * @param   [in]dev_info 设备信息
  * @param   [in]param_name 参数名称
  * @param   [in]data 数值
  * @return  0成功.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
static int update_db_data(DEV_INFO_Typedef_t *dev_info, const char *param_name, const iot_data_t *data)
{
  if(dev_info == NULL || data == NULL)
  {
    return -1;
  }
  MAJOR_KEY_1 major_key_1 = get_device_protocol_type(dev_info);
  MAJOR_KEY_2 major_key_2 = (uint32_t)get_device_addr(dev_info);
  DEVICE_Typedef_t dev_type = get_device_type(dev_info);
  NODE_TYPE_STRUCT *p_node = list_find_node(dev_type, major_key_1, major_key_2);
  if(p_node == NULL)
  {
    return -1;
  }
  int index = get_param_index(param_name);
  if(index == -1)
  {
    return -1;
  }
  /*写入数据*/
  p_node->dev_resource_par[index].default_value = common_iot_data2u64(data, p_node->dev_resource_par[index].value_type);
  
  /*写入数据库*/
  if(STORE_READ & p_node->dev_resource_par[index].permissions)
  {
    UPDATE_DATA_Typedef_t update_data;
    update_data.addr = (uint32_t)get_device_addr(dev_info);
    strncopy(update_data.param_name, param_name, 64);
    strncopy(update_data.table_name, dev_info->dev_type_name, 64);
    get_value_str(update_data.value_current, &p_node->dev_resource_par[index].default_value, 
                    64, p_node->dev_resource_par[index].value_type);
    update_data.time_stamp = get_current_time_s(CURRENT_TIME);
    return dev_driver_event_db_record_update(&update_data);
  }
  return 0;
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
  UNUSED(type);
  const char *param_name = (const char *)input_data;
  devsdk_commandresult *return_value = (devsdk_commandresult *)out_data;
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(param_name, resources_interface_par[index].par_name) == 0)
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
  UNUSED(type);
  const char *param_name = (const char *)input_data;
  const iot_data_t *set_value = (const iot_data_t *)set_data;
  for(int index = 0; resources_interface_par[index].par_name != NULL; index++)
  {
    if(strcmp(param_name, resources_interface_par[index].par_name) == 0)
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
  UNUSED(type);
  MODBUS_PARSE_CODE_Typedef_t state = MODBUS_OK;
  const char *param_name = (const char *)input_data;
  devsdk_commandresult *out_value = (devsdk_commandresult *)out_data;
  DEV_INFO_Typedef_t dev_info;

  uint16_t reg_n = 1;
  uint16_t *reg_value = NULL;
  int ret = parse_dev_name(dev_name, &dev_info);
  if(ret != 0)
  {
    goto __error;
  }
  uint8_t addr = (uint8_t)get_device_addr(&dev_info);
  int index = get_param_index(param_name);
  if(index == -1)
  {
    goto __error;
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
        out_value->origin = get_current_time_s(CURRENT_TIME);
        out_value->value = iot_data_alloc_f32((float)(*reg_value)/10.0);

        /*写入新数据*/
        update_db_data(&dev_info, param_name, out_value->value);

        free(reg_value);
      }
      return 0;
    }
    else
    {
      goto __error;
    }
  }
  else if(PRIVATE_READ & resources_interface_par[index].permissions)
  {
    /*读内部参数*/
    ret = get_private_dev_value(dev_name, input_data, out_data, type);
    if(ret != 0)
    {
      goto __error;
    }
  }
  return 0;
__error:
  out_value->value = iot_data_alloc_f32(-999.9);
  return -1;
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
  const char *param_name = (const char *)input_data;
  const iot_data_t *data = (const iot_data_t *)set_data;
  uint8_t addr = get_modbus_device_addr(dev_name);
  uint16_t reg_n = 1;

  int index = get_param_index(param_name);
  if(index == -1)
  {
    return -1;
  }

  /*检测权限*/
  if(WRITE_ONLY & resources_interface_par[index].permissions)
  {
    /*写入设备*/
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
  UNUSED(type);
  const char *param_name = (const char *)input_data;
  devsdk_commandresult *return_value = (devsdk_commandresult *)out_data;

  /*获得该设备在内存中的数据*/
  DEV_INFO_Typedef_t dev_info;
  int ret = parse_dev_name(dev_name, &dev_info);
  if(ret == -1)
  {
    return -1;
  }

  MAJOR_KEY_1 major_key_1 = get_device_protocol_type(&dev_info);
  MAJOR_KEY_2 major_key_2 = (uint32_t)get_device_addr(&dev_info);
  DEVICE_Typedef_t dev_type = get_device_type(&dev_info);
  NODE_TYPE_STRUCT *p_node = list_find_node(dev_type, major_key_1, major_key_2);
  if(p_node == NULL)
  {
    return -1;
  }

  /*取出参数索引号*/
  int index = get_param_index(param_name);
  if(index == -1)
  {
    return -1;
  }
  return_value->value = iot_data_alloc_ui64(p_node->dev_resource_par[index].default_value);
  return_value->origin = get_current_time_s(CURRENT_TIME);
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
  UNUSED(type);
  if(dev_name == NULL || input_data == NULL || set_data == NULL)
  {
    return -1;
  }
  DEV_INFO_Typedef_t dev_info;
  int ret = parse_dev_name(dev_name, &dev_info);
  if(ret == -1)
  {
    return -1;
  }
  const char *param_name = (const char *)input_data;
  const iot_data_t *set_par = (const iot_data_t *)set_data;

  /*写入数据*/
  return update_db_data(&dev_info, param_name, set_par);
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
  * @return  0 正常 -1错误.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-08
  ******************************************************************
  */
static int recovery_query_callback(char** pr, int row, int column, void *callback_par)
{
	if(row < 1)
	{
		printf("%s %s can't find this param name.\n", __FILE__, __FUNCTION__);
		return -1;
	}
  DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par = (DEV_DRIVER_INTERFACE_Typedef_t *)callback_par;
  if(dev_resource_par == NULL)
  {
    return -1;
  }
  /*0地址号,1参数名,2当前值,3时间戳*/
	int i, j;
	for(i = 1; i <= row; i++)
	{
    j = i * column;
    dev_resource_par->default_value = (uint64_t)atoi(pr[j+2]);
    break;
	}
  return 0;
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
  int ret = dev_driver_event_db_record_query(&data, recovery_query_callback, dev_resource_par);
  if(ret != 0)
  {
    /*查询失败，重建设备事件参数表*/
    INSERT_DATA_Typedef_t insert_data;
    memmove(&insert_data, &data, sizeof(BASE_DATA_Typedef_t));
    get_value_str(insert_data.value_current, &dev_resource_par->default_value, 64, dev_resource_par->value_type);
    insert_data.time_stamp = get_current_time_s(CURRENT_TIME);
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
  * @brief   设备事件上报确认
  * @param   [in]dev_name 设备名.
  * @param   [in]param_name 参数名.
  * @param   [in]dev_resource 设备总资源.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
bool temperature_device_report_event_confirm(const char *dev_name, const char *param_name, 
                                          DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, void *data)
{
  if(dev_name == NULL || param_name == NULL || dev_resource == NULL || data == NULL)
  {
    return false;
  }
  int temp_index = 0;

  /*检查权限*/
  if((dev_resource->permissions & CONFIRM) == 0)
  {
    return true;
  }

  devsdk_commandresult *value = (devsdk_commandresult *)data;
  /*取出参数索引号*/
  int index = get_param_index(param_name);
  if(index == -1)
  {
    return true;
  }
  if(dev_resource[index].report_event_confirm == NULL)
  {
    return true;
  }

  /*调用注册时各参数确认接口*/
  return dev_resource[index].report_event_confirm(dev_name, param_name, 
                                              dev_resource, data);
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
    printf("[%s][%s] can't get the dev type or protocol.\n", __FILE__, __FUNCTION__);
    return -1;
  }
  p_node = list_find_node(dev_type, major_key_1, major_key_2);
  if(p_node == NULL)
  {
    node_p = (NODE_Typedef_t *)calloc(1, sizeof(NODE_Typedef_t)+(sizeof(DEV_DRIVER_INTERFACE_Typedef_t)*sizeof(resources_interface_par)));
    if(node_p == NULL)
    {
      printf("[%s][%s] can't calloc memory for temperature dev.\n", __FILE__, __FUNCTION__);
      return -1;
    }
    p_node = &node_p->node;
    p_node->major_key_1 = major_key_1;
    p_node->major_key_2 = major_key_2;
    snprintf(p_node->dev_name, 256, "%s-%s-%s-%s", dev_info->protocol_str, dev_info->location_str,
                        dev_info->dev_type_name, dev_info->dev_address);
    memmove(p_node->dev_resource_par, resources_interface_par, sizeof(resources_interface_par));
    memmove(&p_node->communication_par, communication_par, sizeof(DEV_COMMUNICATION_PAR_Typedef_t));
    p_node->get_dev_value_callback = get_get_callback(communication_par->protocol_type);
    p_node->set_dev_value_callback = get_set_callback(communication_par->protocol_type);

    /*注册*/
    list_add_to_list(node_p ,dev_type);

    printf("temperature device register successful num %u.\n", list_get_size(dev_type));
  }

  /*修改事件参数*/
  for(int index = 0; p_node->dev_resource_par[index].par_name != NULL; index++)
  {
    if(strcmp(p_node->dev_resource_par[index].par_name, dev_resource_par->par_name) == 0)
    {
      /*私有事件参数从数据库中恢复*/
      if(p_node->dev_resource_par[index].permissions & STORE_READ)
      {
        device_event_recovery(dev_info, dev_resource_par);
      }
      memmove(&p_node->dev_resource_par[index], dev_resource_par, sizeof(DEV_DRIVER_INTERFACE_Typedef_t));
    }
  }

  return 0;
}   

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                