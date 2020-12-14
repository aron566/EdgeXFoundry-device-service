/**
 *  @file device_driver_cfg.h
 *
 *  @date Aug 31, 2020
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 配置参数读写接口.
 *
 *  @details None.
 *
 *  @version V1.0
 */

#ifndef DEVICE_DRIVER_CFG_H
#define DEVICE_DRIVER_CFG_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
/** Private includes ---------------------------------------------------------*/
#include "../service_component/custom-common.h"
/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/
typedef enum{
	PARA_INT,
	PARA_DOUBLE,
	PARA_STRING
}PARA_DATA_Type_t;
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*修改json配置文件数据*/
int midify_config(const char * const identifier ,const void *value ,PARA_DATA_Type_t data_type);

/*读取json配置中键值到value中*/
int read_config(const char * const identifier ,void *value ,PARA_DATA_Type_t data_type);

/*重置配置文件*/
bool reset_config_file(void);

/*初始化配置文件*/
void config_file_init(void);

/*读取软件版本*/
const char *GetSoftwareVersion(void);

/*设置软件版本*/
void SetConfigFileSoftwareVersion(const char *ver);

/*获得日志输出开关*/
bool GetLogOutputEnableState(void);

/*获得配置文件有效性*/
bool GetConfigFileEnableState(void);

/*设置配置文件启用状态*/
void SetConfigFileState(bool state);

/*设置udp服务器端口号*/
void SetudpServerPortNum(int num);

/*获取udp服务器端口号*/
int GetUdpServerPort(void);

/*获取tcp服务器端口号*/
int GetTCPServerPort(void);

/*获取远程udp服务器端口号*/
int GetudpRemotePortNum(void);
/*设置远程udp服务器端口号*/
void SetudpRemotePortNum(int num);
/*获取远程udp服务器IP*/
char *GetUdpRemoteServerIP(void);

/*获取远程tcp服务器IP*/
char *GetTcpRemoteServerIP(void);
/*设置远程tcp服务器端口号*/
void SettcpRemotePortNum(int num);
/*获取远程tcp服务器端口号*/
int GettcpRemotePortNum(void);

/*获取mqtt客户端ID*/
char *GetMqttClentID(void);

/*获取mqtt服务器地址*/
char *GetMqttServerAddr(void);

/*获取mqtt服务器端口*/
char *GetMqttServerPort(void);

/*获取mqtt登录用户名*/
char *GetMqttLoginName(void);

/*获取mqtt登录密码*/
char *GetMqttLoginKey(void);

/*获取扩展订阅主题*/
char *GetMqttSubExternTopic(void);

/*获取扩展发布主题*/
char *GetMqttPublishExternTopic(void);

/*获取服务订阅主题*/
char *GetMqttSubServiceTopic(void);

/*获取服务发布主题*/
char *GetMqttPublishServiceTopic(void);

/*获取控制订阅主题*/
char *GetMqttSubControlTopic(void);

/*获取控制发布主题*/
char *GetMqttPublishControlTopic(void);

/*获取响应订阅主题*/
char *GetMqttSubResponseTopic(void);

/*获取响应发布主题*/
char *GetMqttPublishResponseTopic(void);

/*获取主动上报发布主题*/
char *GetMqttPublishReportTopic(void);

/*获取设备驱动下载文件名*/
char *GetDeviceDriver_DownloadFileName(void);

/*获得设备驱动升级状态*/
bool GetDeviceDriverUpdateState(void);

/*设置设备驱动升级状态*/
void SetDeviceDriverUpdateState(bool state);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif /* CONFIG_OPT_H_ */
/******************************** End of file *********************************/
