/**                                                                             
 *  @file device_driver_parse_par.c
 *                                                                              
 *  @date 2020年11月20日 15:58:41 星期五
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 解析设备配置文件相关参数.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <errno.h>
/* Private includes ----------------------------------------------------------*/
#include "device_driver_parse_par.h"
#include "gateway_device.h"
#include "relay_device.h"
#include "temperature_device.h"
/** Private typedef ----------------------------------------------------------*/
/*解析通讯参数*/
typedef int (*PARSE_COM_PAR_FUNC)(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
typedef struct 
{
    const char *const communication_name;   /**< 通讯名称*/
    const char *communication_alias;
    PROTOCOL_Type_t protocol_type;          /**< 协议类型*/
    PARSE_COM_PAR_FUNC parse_func;          /**< 解析通讯参数函数*/
}PARSE_COM_PAR_MAP_Typedef_t;

/*事件时间单位*/
typedef struct 
{
    const char* const unit_name;    /**< 单位名称*/
    EVENT_REPORT_TIME_UNIT unit;    /**< 事件上报时基*/
}EVENT_TIME_UNIT_MAP_Typedef_t;                                                               
/** Private macros -----------------------------------------------------------*/
#define SERVICE_CONFIG_FILE "../../res/configuration.toml"
/** Private constants --------------------------------------------------------*/
static iot_logger_t *log_lc = NULL;
/** Public variables ---------------------------------------------------------*/
                                                                               
/** Private function prototypes ----------------------------------------------*/

/*解析配置中通讯参数*/
static int parse_mqtt_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
static int parse_modbus_rtu_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
static int parse_private_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);

/*解析配置中设备事件*/
static int parse_event_par_update(toml_table_t *tab, DEV_INFO_Typedef_t *dev_info, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par);
static EVENT_REPORT_TIME_UNIT get_event_time_unit(const char *unit_name);
static int get_event_interval(const char *interval_str, INTERVAL_TIME_Typedef_t *time_par);

/*注册*/
static void register_dev_distribution(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par,
                                    DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par);
/** Private variables --------------------------------------------------------*/
static PARSE_COM_PAR_MAP_Typedef_t parse_com_par_map[] = 
{
    {
        .communication_name = "mqtt",
        .communication_alias= "mqtt",
        .protocol_type      = MQTT_PROTO,
        .parse_func         = parse_mqtt_par
    },
    {
        .communication_name = "modbus-rtu",/**< toml中名称*/
        .communication_alias= "modbus_rtu",
        .protocol_type      = MODBUS_RTU_PROTO,
        .parse_func         = parse_modbus_rtu_par
    },
    {
        .communication_name = "private",
        .communication_alias= "private",
        .protocol_type      = PRIVATE_PROTO,
        .parse_func         = parse_private_par
    },
    {
        .communication_name = NULL,
        .communication_alias= "NULL",
        .protocol_type      = UNKNOW_PROTO,
        .parse_func         = NULL
    },
};   

/*设备类型映射/设备资源映射*/
static  DEVICE_TYPE_MAP_Typedef_t device_type_map_resource_map[] = 
{
    {
        .type_name              = "gateway_device",
        .dev_type               = GATEWAY_DEV_TYPE,
        .register_func          = gateway_device_driver_register,
        .get_device_resource    = get_gateway_device_resource,
        .report_event_confirm   = gateway_device_report_event_confirm
    },
    {
        .type_name              = "temperature_device",
        .dev_type               = TEMPERATURE_DEV_TYPE,
        .register_func          = temperature_device_driver_register,
        .get_device_resource    = get_temperature_device_resource,
        .report_event_confirm   = temperature_device_report_event_confirm
    },
    {
        .type_name              = "relay_device",
        .dev_type               = RELAY_DEV_TYPE,
        .register_func          = relay_device_driver_register,
        .get_device_resource    = get_relay_device_resource,
        .report_event_confirm   = relay_device_report_event_confirm
    },
    {
        .type_name              = NULL,
        .dev_type               = DEV_TYPE_MAX,
        .register_func          = NULL,
        .get_device_resource    = NULL,
        .report_event_confirm   = NULL
    }
};
/*设备类型映射*/
#define device_type_map device_type_map_resource_map
/*设备资源列表*/
#define dev_resource_map device_type_map_resource_map


/*事件时间单位查询表*/
static EVENT_TIME_UNIT_MAP_Typedef_t event_interval_unit[] = 
{
    {
        .unit_name  = "ms",
        .unit       = T_MS
    },
    {
        .unit_name  = "s",
        .unit       = T_S
    },
    {
        .unit_name  = "min",
        .unit       = T_MIN
    },
    {
        .unit_name  = "h",
        .unit       = T_H
    },
    {
        .unit_name  = "d",
        .unit       = T_D
    },
    {
        .unit_name  = "mth",
        .unit       = T_MTH
    },
    {
        .unit_name  = "yer",
        .unit       = T_YER
    },
    {
        .unit_name  = NULL,
        .unit       = T_MAX
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
 * @brief   解析mqtt参数
 * @param   [in]tab 参数表
 * @param   [out]par 写入参数的内存区
 * @return  返回0成功，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-20
 ******************************************************************
 */
static int parse_mqtt_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par)
{
    printf("\n*********** parse mqtt ***********\n");
    if(tab == NULL || par == NULL)
    {
        return -1;
    }
    toml_datum_t subcribe_topic = toml_string_in(tab, "subcribe_topic");
    toml_datum_t publish_topic = toml_string_in(tab, "publish_topic");
    par->protocol_type = MQTT_PROTO;
    if(subcribe_topic.ok)
    {
        printf("subcribe_topic = %s\n", subcribe_topic.u.s);
        strncopy(par->mqtt_par.subcribe_topic, subcribe_topic.u.s, 128);
        free(subcribe_topic.u.s);
    }
    if(publish_topic.ok)
    {
        printf("publish_topic = %s\n", publish_topic.u.s);
        strncopy(par->mqtt_par.publish_topic, publish_topic.u.s, 128);
        free(publish_topic.u.s);
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   解析modbus-rtu参数
 * @param   [in]tab 参数表
 * @param   [out]par 写入参数的内存区
 * @return  返回0成功，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-20
 ******************************************************************
 */
static int parse_modbus_rtu_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par)
{
    printf("\n*********** parse modbus ***********\n");
    if(tab == NULL || par == NULL)
    {
        return -1;
    }
    toml_datum_t UnitID = toml_string_in(tab, "UnitID");
    par->protocol_type = MODBUS_RTU_PROTO;
    if(UnitID.ok) 
    {
        printf("UnitID = %s\n", UnitID.u.s);
        par->modbus_par.addr = (uint8_t)atoi(UnitID.u.s);
        free(UnitID.u.s);
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   解析私有参数
 * @param   [in]tab 参数表
 * @param   [out]par 写入参数的内存区
 * @return  返回0成功，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-20
 ******************************************************************
 */
static int parse_private_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par)
{
    printf("\n*********** parse private ***********\n");
    if(tab == NULL || par == NULL)
    {
        return -1;
    }
    toml_datum_t cmd = toml_string_in(tab, "cmd");
    par->protocol_type = PRIVATE_PROTO;
    if(cmd.ok) 
    {
        /*cmd:[get/put]-type[array,string,int,float]-value*/
        printf("cmd = %s\n", cmd.u.s);
        free(cmd.u.s);
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   解析事件时间单位
 * @param   [in]unit_name 单位名
 * @return  EVENT_REPORT_TIME_UNIT
 * @author  aron566
 * @version V1.0
 * @date    2020-11-25
 ******************************************************************
 */
static EVENT_REPORT_TIME_UNIT get_event_time_unit(const char *unit_name)
{
    if(unit_name == NULL)
    {
        return T_MAX;
    }
    for (int index = 0; event_interval_unit[index].unit_name != NULL; index++)
    {
        if(strcmp(unit_name, event_interval_unit[index].unit_name) == 0)
        {
            return event_interval_unit[index].unit;
        }
    }
    return T_MAX;
}

/**
 ******************************************************************
 * @brief   解析设备事件上报间隔参数
 * @param   [in]interval_str间隔时间字符串
 * @param   [in]time_par 时间解析存储区
 * @return  解析正确返回0，错误-1
 * @author  aron566
 * @version V1.0
 * @date    2020-11-25
 ******************************************************************
 */
static int get_event_interval(const char *interval_str, INTERVAL_TIME_Typedef_t *time_par)
{
    if(interval_str == NULL || time_par == NULL)
    {
        return -1;
    }
    char unit_name[8];
    int ret = sscanf(interval_str, "%lu%s", &time_par->interval_time, unit_name);
    if(ret < 2)
    {
        printf("parse time unit error.\n");
        return -1;
    }
    time_par->unit = get_event_time_unit(unit_name);
    if(time_par->unit == T_MAX)
    {
        return -1;
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   解析设备事件参数并更新
 * @param   [in]tab 事件表
 * @param   [in]dev_info 设备解析结构信息
 * @param   [out]dev_resource_par 设备资源信息
 * @return  解析正确返回0，错误-1
 * @author  aron566
 * @version V1.0
 * @date    2020-11-09
 ******************************************************************
 */
static int parse_event_par_update(toml_table_t *tab, DEV_INFO_Typedef_t *dev_info, DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par)
{
    if(dev_info == NULL || tab == NULL || dev_resource_par == NULL)
    {
        return -1;
    }
    const DEV_DRIVER_INTERFACE_Typedef_t *dev_resource = NULL;
    
    int ret = 0;
    toml_datum_t OnChange;
    toml_datum_t Frequency;
    toml_datum_t resource_name;
    INTERVAL_TIME_Typedef_t time_par;
    DEVICE_Typedef_t dev_type = get_device_type(dev_info);

    if(dev_type == DEV_TYPE_MAX)
    {
        return -1;
    }
    resource_name = toml_string_in(tab, "Resource");
    if(resource_name.ok)
    {
        OnChange = toml_bool_in(tab, "OnChange");
        
        Frequency = toml_string_in(tab, "Frequency");

        for(int index = 0; dev_resource_map[index].get_device_resource != NULL; index++)
        {
            /*匹配设备类型*/
            if(dev_type == dev_resource_map[index].dev_type)
            {
                dev_resource = dev_resource_map[index].get_device_resource();
                break;
            }
        }
        if(dev_resource != NULL)
        {
            for(int i = 0; dev_resource[i].par_name != NULL; i++)
            {
                /*匹配到该设备资源名*/
                if(strcmp(dev_resource[i].par_name, resource_name.u.s) == 0)
                {
                    /*copy*/
                    memmove(dev_resource_par, &dev_resource[i], sizeof(DEV_DRIVER_INTERFACE_Typedef_t));
                    if(OnChange.ok) 
                    {
                        dev_resource_par->enable_on_change_flag = OnChange.u.b;
                    }
                    if(Frequency.ok)
                    {
                        ret = get_event_interval(Frequency.u.s ,&time_par);
                        if(ret < 0)
                        {
                            printf("decode interval time error.\n");
                        }
                        else
                        {
                            dev_resource_par->enable_event_flag = true;
                            dev_resource_par->interval_time = time_par.interval_time;
                            dev_resource_par->unit = time_par.unit;
                        }
                        free(Frequency.u.s);
                    }
                }
            }     
        }
        free(resource_name.u.s);
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   注册设备
 * @param   [in]None.
 * @return  成功toml_table_t *失败NULL
 * @author  aron566
 * @version V1.0
 * @date    2020-11-15
 ******************************************************************
 */
static void register_dev_distribution(DEV_INFO_Typedef_t *dev_info, DEV_COMMUNICATION_PAR_Typedef_t *communication_par,
                                        DEV_DRIVER_INTERFACE_Typedef_t *dev_resource_par)
{
    if(dev_info == NULL || communication_par == NULL)
    {
        return;
    }
    for(int index = 0; device_type_map[index].dev_type != DEV_TYPE_MAX; index++)
    {
        if(strcmp(dev_info->dev_type_name, device_type_map[index].type_name) == 0)
        {
            device_type_map[index].register_func(dev_info, communication_par ,dev_resource_par);
            return;
        }
    }
}

/**
 ******************************************************************
 * @brief   解析服务配置文件
 * @param   [in]None.
 * @return  成功toml_table_t *失败NULL
 * @author  aron566
 * @version V1.0
 * @date    2020-11-15
 ******************************************************************
 */
static toml_table_t* load_service_config(void)
{
    FILE* fp;
    char errbuf[200];
    fp = fopen(SERVICE_CONFIG_FILE, "r");
    if (!fp) 
    {
        fprintf(stderr, "ERROR: cannot open configuration.toml - %s\n", strerror(errno));
        exit(1);
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if(!conf)
    {
        fprintf(stderr, "ERROR: cannot parse - %s\n", errbuf);
        return NULL;
    }
    return conf;
}

/**
 ******************************************************************
 * @brief   解析服务配置文件
 * @param   [in]None.
 * @return  成功0,失败-1
 * @author  aron566
 * @version V1.0
 * @date    2020-11-15
 ******************************************************************
 */
static int parse_service_config(void)
{
    toml_table_t* conf = load_service_config();
    if(conf == NULL)
    {
        iot_log_error(log_lc, "parse config file error.");
        return -1;
    }

    /*查找配置中[[DeviceList]]的数组*/
    toml_array_t* DeviceList = toml_array_in(conf, "DeviceList");
    if (!DeviceList) 
    {
        fprintf(stderr, "ERROR: missing [DeviceList]\n");
        return -1;
    }

    /*轮询DeviceList数组中的表*/
    toml_table_t *array_of_tab = NULL;
    toml_table_t *tab_of_array = NULL;
    toml_array_t *sub_array_of_array = NULL;
    toml_table_t *tab_of_sub_array = NULL;
    toml_datum_t dev_name;
    DEV_INFO_Typedef_t dev_info;
    DEV_COMMUNICATION_PAR_Typedef_t communication_par;
    DEV_DRIVER_INTERFACE_Typedef_t dev_resource_par;
    
    int ret = 0;
    for (int i = 0; 0 != (array_of_tab = toml_table_at(DeviceList, i)); i++) 
    {
        dev_name = toml_string_in(array_of_tab, "Name");
        if(dev_name.ok)
        {
            iot_log_debug(log_lc, "Name = %s\n", dev_name.u.s);
            ret = parse_dev_name(dev_name.u.s, &dev_info);
            if(ret != 0)
            {
                iot_log_warn(log_lc, "parse dev name error skip one.");
                continue;
            }
        }
        free(dev_name.u.s);

        /*找到表中的子表*/
        /*解析协议参数*/
        for(int index = 0; parse_com_par_map[index].parse_func != NULL; index++)
        {
            printf("\n*********** search parse proto : %s ***********\n",parse_com_par_map[index].communication_name);
            if(0 != (tab_of_array = toml_table_in(array_of_tab, "Protocols")))
            {
                if(0 != (tab_of_array = toml_table_in(tab_of_array, parse_com_par_map[index].communication_name)))
                {
                    parse_com_par_map[index].parse_func(tab_of_array, &communication_par);     
                }

            }
        }

        /*找到表中设备事件*/
        sub_array_of_array = toml_array_in(array_of_tab, "AutoEvents");
        if (!sub_array_of_array) 
        {
            fprintf(stderr, "ERROR: missing [[AutoEvents]]\n");
        }
        else
        {
            /*更新设备资源表的事件*/
            for (int y = 0; 0 != (tab_of_sub_array = toml_table_at(sub_array_of_array, y)); y++) 
            {
                parse_event_par_update(tab_of_sub_array, &dev_info, &dev_resource_par);
                /*分配注册-更新事件*/
                register_dev_distribution(&dev_info, &communication_par, &dev_resource_par);
            }
        }

        /*分配注册*/
        register_dev_distribution(&dev_info, &communication_par, &dev_resource_par);
    }

    toml_free(conf);
    return 0;
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
 * @brief   返回协议类型
 * @param   [in]DEV_INFO_Typedef_t 设备信息
 * @return  PROTOCOL_Type_t 协议类型
 * @author  aron566
 * @version V1.0
 * @date    2020-11-18
 ******************************************************************
 */
PROTOCOL_Type_t get_device_protocol_type(DEV_INFO_Typedef_t *dev_info)
{
    if(dev_info == NULL)
    {
        return UNKNOW_PROTO;
    }
    for(int index = 0; parse_com_par_map[index].protocol_type != UNKNOW_PROTO; index++)
    {
        if((strcmp(dev_info->protocol_str, parse_com_par_map[index].communication_name) == 0) ||
            (strcmp(dev_info->protocol_str, parse_com_par_map[index].communication_alias) == 0))
        {
            return parse_com_par_map[index].protocol_type;
        }
    }
    return UNKNOW_PROTO;
}

/**
 ******************************************************************
 * @brief   返回设备类型
 * @param   [in]DEV_INFO_Typedef_t 设备信息
 * @return  PROTOCOL_Type_t 协议类型
 * @author  aron566
 * @version V1.0
 * @date    2020-11-18
 ******************************************************************
 */
DEVICE_Typedef_t get_device_type(DEV_INFO_Typedef_t *dev_info)
{
    if(dev_info == NULL)
    {
        return DEV_TYPE_MAX;
    }
    for(int index = 0; device_type_map[index].dev_type != DEV_TYPE_MAX; index++)
    {
        if(strcmp(dev_info->dev_type_name, device_type_map[index].type_name) == 0)
        {
            return device_type_map[index].dev_type;
        }
    }
    return DEV_TYPE_MAX;
}

/**
 ******************************************************************
 * @brief   返回设备地址
 * @param   [in]DEV_INFO_Typedef_t 设备信息
 * @return  -1失败 ，地址
 * @author  aron566
 * @version V1.0
 * @date    2020-12-06
 ******************************************************************
 */
int get_device_addr(DEV_INFO_Typedef_t *dev_info)
{
    if(dev_info == NULL)
    {
        return -1;
    }
    return atoi(dev_info->dev_address);
}

/**
 ******************************************************************
 * @brief   返回设备地址
 * @param   [in]dev_name 设备名
 * @return  地址
 * @author  aron566
 * @version V1.0
 * @date    2020-12-06
 ******************************************************************
 */
uint8_t get_modbus_device_addr(const char *dev_name)
{
  DEV_INFO_Typedef_t dev_info;
  int ret = parse_dev_name(dev_name, &dev_info);
  if(ret != 0)
  {
    return 0;
  }
  return (uint8_t)atoi(dev_info.dev_address);
}

/**
 ******************************************************************
 * @brief   解析设备名包含的信息
 * @param   [in]dev_name 设备完整名
 * @param   [out]dev_info 设备解析结构信息存储区
 * @return  解析正确返回0，错误-1
 * @author  aron566
 * @version V1.0
 * @date    2020-11-09
 ******************************************************************
 */
int parse_dev_name(const char *dev_name, DEV_INFO_Typedef_t *dev_info)
{
    // （协议-位置信息(应包含上级网关信息+地理位置)-设备名(可重复，形式：xxx_device)-地址号或者序号(同名设备不可重复)）
    /*解析设备通讯协议*/
    int ret = sscanf(dev_name, "%[^-]-%[^-]-%[^-]-%s", dev_info->protocol_str, 
                    dev_info->location_str, dev_info->dev_type_name, dev_info->dev_address);
    if(ret != 4)
    {
        iot_log_error(log_lc, "parse dev_name error.");
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 ******************************************************************
 * @brief   合成设备名
 * @param   [out]dest_str 设备名存储区
 * @param   [in]proto_type 协议类型
 * @param   [in]location 位置信息
 * @param   [in]dev_type 设备类型
 * @param   [in]address 设备地址号
 * @return  设备名
 * @author  aron566
 * @version V1.0
 * @date    2020-12-09
 ******************************************************************
 */
char *jonint_dev_name(char *dest_str, size_t size, PROTOCOL_Type_t protocol_type, const char *location, 
                        DEVICE_Typedef_t dev_type, int address)
{
    if(dest_str == NULL || size == 0)
    {
        return dest_str;
    }
    const char *dev_name = NULL;
    const char *protocol_name = NULL;
    for(int index = 0; device_type_map[index].type_name != NULL; index++)
    {
        if(device_type_map[index].dev_type == dev_type)
        {
            /*获得设备类型名*/
            dev_name = device_type_map[index].type_name;
            break;
        }
    }
    for(int i = 0; parse_com_par_map[i].communication_name != NULL; i++)
    {
        if(parse_com_par_map[i].protocol_type == protocol_type)
        {
            /*获得协议名*/
            protocol_name = parse_com_par_map[i].communication_name;
            break;
        }
    }
    if(dev_name == NULL || protocol_name == NULL)
    {
        return dest_str;
    }
    /*协议名-位置-设备类型名-地址号*/
    snprintf(dest_str, size, "%s-%s-%s-%d", protocol_name, location, dev_name, address);
    return dest_str;
}

/**
 ******************************************************************
 * @brief   获取设备类型表
 * @param   [in]None.
 * @return  设备表.
 * @author  aron566
 * @version V1.0
 * @date    2020-12-07
 ******************************************************************
 */
const DEVICE_TYPE_MAP_Typedef_t *get_device_type_list(void)
{
    return device_type_map;
}

/**
 ******************************************************************
 * @brief   获取设备资源表
 * @param   [in]None.
 * @return  设备资源表.
 * @author  aron566
 * @version V1.0
 * @date    2020-12-08
 ******************************************************************
 */
const DEVICE_RESOURCE_MAP_Typedef_t *get_device_resource_list(void)
{
    return dev_resource_map;
}

/**
 ******************************************************************
 * @brief   获取设备上报事件确认接口
 * @param   [in]dev_type 设备类型.
 * @return  设备资源表.
 * @author  aron566
 * @version V1.0
 * @date    2020-12-10
 ******************************************************************
 */
const REPORT_EVENT_CONFIRM_FUNC get_device_event_confirm_func(DEVICE_Typedef_t dev_type)
{
    for(int index = 0; dev_resource_map[index].report_event_confirm != NULL; index++)
    {
        if(dev_resource_map[index].dev_type == dev_type)
        {
            return dev_resource_map[index].report_event_confirm;
        }
    }
    return NULL;
}

/**
 ******************************************************************
 * @brief   注册设备驱动
 * @param   [in]lc log.
 * @return  None.
 * @author  aron566
 * @version V1.0
 * @date    2020-11-25
 ******************************************************************
 */
void register_device_driver(iot_logger_t *lc)
{
    log_lc = lc;
    /*解析设备服务文件*/
    int ret = parse_service_config();
    if(ret != 0)
    {
        iot_log_error(log_lc, "decode device service file faild.");
        exit(-1);
    }
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                