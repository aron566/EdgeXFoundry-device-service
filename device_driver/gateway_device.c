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
#include "device_driver_event_db.h"
/** Private typedef ----------------------------------------------------------*/
                                                                               
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
static DEV_DRIVER_INTERFACE_Typedef_t resources_interface_par[] = 
{
  {
    .par_name               = "version",
    .command                = 0x03,
    .command_addr           = 0x0000,
    .value_type             = STRING,
    .default_value          = 0,
    .permissions            = CONFIG_READ|PRIVATE_READ,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },
  {
    .par_name               = "temperature",
    .command                = 0x03,
    .command_addr           = 0x0001,
    .value_type             = FLOAT32,
    .default_value          = 0,
    .permissions            = STORE_READ|PRIVATE_READ,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },          
  {         
    .par_name               = "run_state",
    .command                = 0x03|0x10,
    .command_addr           = 0x0002,
    .value_type             = UINT32,
    .default_value          = 0,
    .permissions            = PRIVATE_READ|PRIVATE_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },          
  {         
    .par_name               = "update_progress",
    .command                = 0x03|0x10,
    .command_addr           = 0x0003,
    .value_type             = UINT32,
    .default_value          = 0,
    .permissions            = PRIVATE_READ,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
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
    .permissions            = STORE_READ|PRIVATE_READ|PRIVATE_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },   
  {         
    .par_name               = "upgrade_file",
    .command                = 0x03|0x10,
    .command_addr           = 0x0005,
    .value_type             = UINT8,
    .default_value          = 0,
    .permissions            = PRIVATE_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
    .start_time             = 0,
    .interval_time          = 0,
    .unit                   = T_S
  },  
  {         
    .par_name               = "cmd",
    .command                = 0x03|0x10,
    .command_addr           = 0x0006,
    .value_type             = STRING,
    .default_value          = 0,
    .permissions            = PRIVATE_WRITE,
    .enable_event_flag      = false,
    .enable_on_change_flag  = false,
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
    .start_time             = 0,
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
  * @brief   获取参数所在资源索引号
  * @param   [in]parm 参数名
  * @return  索引号.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
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
  * @brief   网关更新监测任务
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-09
  ******************************************************************
  */
static void edge_gateway_upgruade_monitor(void)
{
    /*读取升级文件大小*/

    /*上报*/
  /* Load the file contents */
  // uint8_t *data = file_readfile (fname, &size);
  // if (data)
  // {
  //   /* Set up a commandresult. The deviceResource for our profiles is "File" */
  //   devsdk_commandresult results[1];
  //   iot_log_info (impl->lc, "File size: %" PRIu32, size);
  //   results[0].origin = 0;
  //   results[0].value = iot_data_alloc_array (data, size, IOT_DATA_UINT8, IOT_DATA_TAKE);

  //   /* Trigger an event */
  //   devsdk_post_readings (service, dname, "File", results);

  //   /* Cleanup the value. Note that as we used IOT_DATA_TAKE, the buffer allocated in file_readfile is free'd here */
  //   iot_data_free (results[0].value);
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
  UNUSED(type);
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
  UNUSED(type);
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
  * @brief   设置modbus设备数值接口
  * @param   [in]input_data 设置参数
  * @param   [out]out_data 返回数据
  * @param   [out]type 数据类型
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-13
  ******************************************************************
  */
static int set_modbus_dev_value(const char *dev_name, const void *input_data, const void *set_data, VALUE_Type_t *type)
{
  UNUSED(type);
  return 0;
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
  UNUSED(type);
  return 0;
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
  * @brief   获取网关设备运行状态
  * @param   [in]None.
  * @return  EdgeGatewayRUN_SATE_Typedef_t.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-07
  ******************************************************************
  */
EdgeGatewayRUN_SATE_Typedef_t get_gateway_device_run_state(void)
{
  int index = get_param_index("run_state");
  if(index == -1)
  {
    return UNKNOW_STATE;
  }
  return (EdgeGatewayRUN_SATE_Typedef_t)resources_interface_par[index].default_value;
}

/**
  ******************************************************************
  * @brief   获取网关设备资源表
  * @param   [in]None.
  * @return  DEV_DRIVER_INTERFACE_Typedef_t.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
const DEV_DRIVER_INTERFACE_Typedef_t *get_gateway_device_resource(void)
{
  return (const DEV_DRIVER_INTERFACE_Typedef_t *)resources_interface_par;
}

/**
  ******************************************************************
  * @brief   设备事件上报确认
  * @param   [in]param_name 参数名.
  * @param   [in]data 此参数获取的数据. 
  * @return  true 上报允许 false 不上报.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-10
  ******************************************************************
  */
bool gateway_device_report_event_confirm(const char *dev_name, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource, const void *data)
{
  if(dev_name == NULL || dev_resource == NULL || data == NULL)
  {
    return false;
  }

  /*检查权限*/
  if((dev_resource->permissions & CONFIRM) == 0)
  {
    return true;
  }

  const devsdk_commandresult *value = (const devsdk_commandresult *)data;
  return true;
}

/**
  ******************************************************************
  * @brief   网关设备驱动注册
  * @param   [in]dev_info 设备信息
  * @param   [in]communication_par 设备通讯参数
  * @param   [in]dev_resource_par 设备资源信息
  * @return  None.
  * @author  aron566
  * @version V1.1
  * @date    2020-12-02
  ******************************************************************
  */
int gateway_device_driver_register(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par,
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
  p_node = list_find_node(GATEWAY_DEV_TYPE, major_key_1, major_key_2);
  if(p_node == NULL)
  {
    node_p = (NODE_Typedef_t *)calloc(1, sizeof(NODE_Typedef_t)+(sizeof(DEV_DRIVER_INTERFACE_Typedef_t)*sizeof(resources_interface_par)));
    if(node_p == NULL)
    {
      printf("can't calloc memory for gatway dev.\n");
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
    list_add_to_list(node_p ,GATEWAY_DEV_TYPE);

    printf("gatway device register successful.\n");
  }

  /*修改事件参数*/
  for(int index = 0; p_node->dev_resource_par[index].par_name != NULL; index++)
  {
    if(strcmp(p_node->dev_resource_par[index].par_name, dev_resource_par->par_name) == 0)
    {
      /*私有事件参数从数据库中恢复*/
      if(dev_resource_par->permissions & STORE_READ)
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
                                                                                