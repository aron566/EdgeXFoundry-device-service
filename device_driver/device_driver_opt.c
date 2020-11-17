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
#include <errno.h>
/* Private includes ----------------------------------------------------------*/
#include "device_driver_opt.h"
#include "device_driver_list.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
#define SERVICE_CONFIG_FILE "res/configuration.toml"
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
	if (!fp) {
		fprintf(stderr, "ERROR: cannot open configuration.toml - %s\n", strerror(errno));
		exit(1);
	}

	toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
	fclose(fp);

	if (!conf) {
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
	toml_array_t* DeviceList = toml_array_in(conf, "DeviceList");
	if (!DeviceList) {
		fprintf(stderr, "ERROR: missing [DeviceList]\n");
		return -1;
	}

    toml_table_t *tab = NULL;
	for (int i = 0; 0 != (tab = toml_table_at(DeviceList, i)); i++) {
	print_table_title(key);
	print_table(tab);
	printf("\n");
    }
	toml_datum_t host = toml_string_in(server, "host");
	if (!host.ok) {
		fprintf(stderr, "ERROR: cannot read server.host.\n");
		return -1;
	}
	
	toml_datum_t port = toml_int_in(server, "port");
	if (!port.ok) {
		fprintf(stderr, "ERROR: cannot read server.port.\n");
		return -1;
	}

	printf("host: %s, port %d\n", host.u.s, (int)port.u.i);
	free(host.u.s);
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
* @brief   设备驱动初始化
* @param   [in]None.
* @retval  None.
* @author  aron566
* @version V1.0
* @date    2020-11-13
******************************************************************
*/
void device_driver_opt_init(void)
{
    /*初始化链表*/
    list_table_init();

    /*注册设备驱动*/
    

}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                