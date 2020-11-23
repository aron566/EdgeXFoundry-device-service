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
#include "device_driver_list.h"
/** Private typedef ----------------------------------------------------------*/
/*解析通讯参数*/
typedef int (*PARSE_COM_PAR_FUNC)(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
typedef struct 
{
	const char *const communication_name;   /**< 通讯名称*/
    PROTOCOL_Type_t protocol_type;          /**< 协议类型*/
	PARSE_COM_PAR_FUNC parse_func;          /**< 解析通讯参数函数*/
}PARSE_COM_PAR_MAP_Typedef_t;

/*设备类型列表*/
typedef struct 
{
    const char* const type_name;    /**< 类型名称*/
    DEVICE_Typedef_t dev_type;      /**< 类型*/
}DEVICE_TYPE_MAP_Typedef_t;
                                                                          
/** Private macros -----------------------------------------------------------*/
#define SERVICE_CONFIG_FILE "res/configuration.toml"                                                                                
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
                                                                               
/** Private function prototypes ----------------------------------------------*/
/*解析设备名包含的信息*/
static int parse_dev_name(const char *dev_name, DEV_INFO_Typedef_t *dev_info);
/*获取设备协议类型*/
static PROTOCOL_Type_t get_device_protocol_type(DEV_INFO_Typedef_t *dev_info);
/*获取设备类型*/
static DEVICE_Typedef_t get_device_type(DEV_INFO_Typedef_t *dev_info); 

/*解析配置中通讯参数*/
static int parse_mqtt_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
static int parse_modbus_rtu_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
static int parse_private_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par);
/** Private variables --------------------------------------------------------*/
PARSE_COM_PAR_MAP_Typedef_t parse_com_par_map[] = 
{
	{
		.communication_name = "mqtt",
        .protocol_type = MQTT_PROTO,
		.parse_func = parse_mqtt_par
	},
	{
		.communication_name = "modbus-rtu",
        .protocol_type = MODBUS_RTU_PROTO,
		.parse_func = parse_modbus_rtu_par
	},
	{
		.communication_name = "private",
        .protocol_type = PRIVATE_PROTO,
		.parse_func = parse_private_par
	},
	{
		.communication_name = NULL,
        .protocol_type = UNKNOW_PROTO,
		.parse_func = NULL
	},
};   

/*设备类型映射*/
DEVICE_TYPE_MAP_Typedef_t device_type_map[] = 
{
    {
        .type_name = "gateway_device",
        .dev_type = GATEWAY_DEV_TYPE
    },
    {
        .type_name = "temperature_device",
        .dev_type = TEMPERATURE_DEV_TYPE
    },
    {
        .type_name = "relay_device",
        .dev_type = RELAY_DEV_TYPE
    },
    {
        .type_name = NULL,
        .dev_type = DEV_TYPE_MAX
    }
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
 * @retval  返回0成功，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-20
 ******************************************************************
 */
static int parse_mqtt_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par)
{
    if(tab == NULL || par == NULL)
    {
        return -1;
    }
    const char* raw;
    par->protocol_type = MQTT_PROTO;
    if(0 != (raw = toml_raw_in(tab, "subcribe_topic")))
    {
        printf("subcribe_topic = %s\n", raw);
    }
    if(0 != (raw = toml_raw_in(tab, "publish_topic")))
    {
        printf("publish_topic = %s\n", raw);
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   解析modbus-rtu参数
 * @param   [in]tab 参数表
 * @param   [out]par 写入参数的内存区
 * @retval  返回0成功，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-20
 ******************************************************************
 */
static int parse_modbus_rtu_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par)
{
    if(tab == NULL || par == NULL)
    {
        return -1;
    }
    const char* raw;
    par->protocol_type = MODBUS_RTU_PROTO;
    if(0 != (raw = toml_raw_in(tab, "UnitID"))) 
    {
        printf("UnitID = %s\n", raw);
        par->modbus_par.addr = (uint8_t)atoi(raw);
    }
    return 0;
}

/**
 ******************************************************************
 * @brief   解析私有参数
 * @param   [in]tab 参数表
 * @param   [out]par 写入参数的内存区
 * @retval  返回0成功，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-20
 ******************************************************************
 */
static int parse_private_par(toml_table_t *tab, DEV_COMMUNICATION_PAR_Typedef_t *par)
{
    if(tab == NULL || par == NULL)
    {
        return -1;
    }
    const char* raw;
    par->protocol_type = PRIVATE_PROTO;
    if(0 != (raw = toml_raw_in(tab, "cmd"))) 
    {
        /*cmd:[get/put]-type[array,string,int,float]-value*/
        printf("cmd = %s\n", raw);
    }
    return 0;
}
/**
 ******************************************************************
 * @brief   返回协议类型
 * @param   [in]DEV_INFO_Typedef_t 设备信息
 * @retval  PROTOCOL_Type_t 协议类型
 * @author  aron566
 * @version V1.0
 * @date    2020-11-18
 ******************************************************************
 */
static PROTOCOL_Type_t get_device_protocol_type(DEV_INFO_Typedef_t *dev_info)
{
	if(dev_info == NULL)
	{
		return UNKNOW_PROTO;
	}
    for(int index = 0; parse_com_par_map[index].protocol_type != UNKNOW_PROTO; index++)
    {
        if(strcmp(dev_info->protocol_str, parse_com_par_map[index].communication_name) == 0)
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
 * @retval  PROTOCOL_Type_t 协议类型
 * @author  aron566
 * @version V1.0
 * @date    2020-11-18
 ******************************************************************
 */
static DEVICE_Typedef_t get_device_type(DEV_INFO_Typedef_t *dev_info)
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
 * @brief   解析设备名包含的信息
 * @param   [in]dev_name 设备完整名
 * @param   [out]dev_info 设备解析结构信息存储区
 * @retval  解析正确返回0，错误-1
 * @author  aron566
 * @version V1.0
 * @date    2020-11-09
 ******************************************************************
 */
static int parse_dev_name(const char *dev_name, DEV_INFO_Typedef_t *dev_info)
{
    // （协议-位置信息(应包含上级网关信息+地理位置)-设备名(可重复)-地址号或者序号(同名设备不可重复)）
    /*解析设备通讯协议*/
    int ret = sscanf(dev_name, "%[^-]-%[^-]-%[^-]-%s", dev_info->protocol_str, 
                    dev_info->location_str, dev_info->dev_type_name, dev_info->dev_address);
    if(ret != 4)
    {
        printf("parse dev_name error.\r\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 ******************************************************************
 * @brief   解析服务配置文件
 * @param   [in]None.
 * @retval  成功toml_table_t *失败NULL
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
 * @retval  成功0,失败-1
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
        printf("parse config file error.\n");
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
    const char* dev_name;
	DEV_INFO_Typedef_t dev_info;
	DEV_COMMUNICATION_PAR_Typedef_t communication_par;
	int ret = 0;
	for (int i = 0; 0 != (array_of_tab = toml_table_at(DeviceList, i)); i++) 
	{
		if(0 != (dev_name = toml_raw_in(array_of_tab, "Name")))
		{
			printf("Name = %s\n", dev_name);
			ret = parse_dev_name(dev_name, &dev_info);
			if(ret != 0)
			{
				printf("parse dev name error skip one.\n");
				continue;
			}
		}

		/*找到表中的子表*/
		/*解析协议参数*/
        for(int index = 0; parse_com_par_map[index].parse_func != NULL; index++)
        {
            if(0 != (tab_of_array = toml_table_in(array_of_tab, parse_com_par_map[index].communication_name)))
            {
                parse_com_par_map[index].parse_func(tab_of_array, &communication_par);
            }
        }
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
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                