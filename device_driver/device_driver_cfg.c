/**
 *  @file device_driver_cfg.c
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

#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <errno.h>
/* Private includes ----------------------------------------------------------*/
#include "device_driver_cfg.h"
/** Private typedef ----------------------------------------------------------*/
/** Private macros -----------------------------------------------------------*/
#define CONFIG_FILE_PATH	"EdgeXFoundryConfig.json"
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static char Software_Ver[10] = "1.1.1";			/**< 软件版本号*/

/*server config item*/
static int  udpServerPortNum = 2000;/**< UDP服务器端口设置*/
static int	tcpServerPortNum = 3000;/**< TCP服务器端口*/

static char	udpRemoteServerIP[100] = "192.168.10.1";/**< UDP远程服务器IP地址*/
static int 	udpRemotePortNum = 1000;/**< UDP远程服务器端口设置*/
static char	tcpRemoteServerIP[100] = "192.168.10.1";/**< TCP远程服务器IP地址*/
static int 	tcpRemotePortNum = 4000;/**< TCP远程服务器端口*/

static char mqttClientID[100] = "EdgeXFoundryGateway";/**< mqtt客户端id*/
static char mqttServer[100] = "192.168.10.37";/**< mqtt服务器ip*/
static int  mqttPort = 1883;/**< mqtt服务器端口号*/
static char mqttUserName[64] = "usernametest";/**< mqtt用户名*/
static char mqttUserKey[64] = "userkeytest";/**< mqtt用户密码*/

static char mqttSUB_ExternTopic[64] = "extern_testsub";/**< mqtt扩展订阅主题*/
static char mqttSUB_ResponseTopic[64] = "/v1/i1_Protocol/device/response";/**< mqtt响应订阅主题*/
static char mqttSUB_ServiceTopic[64] = "/v1/i1_protocol/service/command";/**< mqtt服务订阅主题*/
static char mqttSUB_ControlTopic[64] = "/v1/i1_protocol/device/command";/**< mqtt控制订阅主题*/

static char mqttPUB_ExternTopic[64] = "extern_testpub";/**< mqtt扩展发布主题*/
static char mqttPUB_ResponseTopic[64] = "/v1/i1_Protocol/device/request";/**< mqtt响应发布主题*/
static char mqttPUB_ServiceTopic[64] = "/v1/i1_protocol/service/reply";/**< mqtt服务发布主题*/
static char mqttPUB_ControlTopic[64] = "/v1/i1_protocol/device/reply";/**< mqtt控制发布主题*/
static char mqttPUB_ReportTopic[64] = "/v1/i1_protocol/service/data";/**< mqtt上报发布主题*/
static char deviceDriver_DownloadFileName[64] = "device_driver.so.download";/**< 设备驱动文件下载名*/
/*设备驱动升级成功状态*/
static int updateSuccessful = 1;        /**< 默认为成功升级状态，若为失败状态则将备份驱动恢复*/
/*配置是否启用日志输出*/
static int enableLogOutput = 1;         /**< 开启日志输出状态标志，默认为开启输出状态*/
/*配置文件启用状态*/
static int enableConfigFile = 0;				/**< 启用配置文件标志，防止意外断电数据不完整*/
/** Private function prototypes ----------------------------------------------*/
/**/
/*创建配置文件*/
static bool creat_config_file(void);
/*启动读取配置信息*/
static bool starup_read_config(void);
/*删除文本中空格*/
static bool strip_comments(char *string, char comment);
/*修改json中字符串键值*/
static void cjson_add_string_to_object(cJSON * const object, const char * const name, const char * const string);
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
  * @brief   创建配置文件
  * @param   [in]None
  * @return  true表示初始化完成
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
static bool creat_config_file(void)
{
  FILE  *fp 	= 	NULL;
  cJSON *root = 	NULL;

  fp = file_open(CONFIG_FILE_PATH, READ_WRITE_CREAT_CLEAR_FILE);
  if(NULL == fp)
  {
    PRINT_ERRMSG("fopen");
    exit(errno);
  }

  root = cJSON_CreateObject();

  /*作为服务器 配置网络参数*/
  cJSON_AddStringToObject(root, "Software_Ver",Software_Ver);
  cJSON_AddNumberToObject(root, "udpServerPortNum", udpServerPortNum);
  cJSON_AddNumberToObject(root, "tcpServerPortNum", tcpServerPortNum);

  /*作为客户端设置远程属性*/
  cJSON_AddStringToObject(root, "udpRemoteServerIP", udpRemoteServerIP);
  cJSON_AddNumberToObject(root, "udpRemotePortNum", udpRemotePortNum);
  cJSON_AddStringToObject(root, "tcpRemoteServerIP", tcpRemoteServerIP);
  cJSON_AddNumberToObject(root, "tcpRemotePortNum", tcpRemotePortNum);

  /*mqtt 配置参数*/
  cJSON_AddStringToObject(root, "mqttClientID", mqttClientID);
  cJSON_AddStringToObject(root, "mqttServer", mqttServer);
  cJSON_AddNumberToObject(root, "mqttPort", mqttPort);
  cJSON_AddStringToObject(root, "mqttUserName", mqttUserName);
  cJSON_AddStringToObject(root, "mqttUserKey", mqttUserKey);

  cJSON_AddStringToObject(root, "mqttSUB_ExternTopic", mqttSUB_ExternTopic);
  cJSON_AddStringToObject(root, "mqttPUB_ExternTopic", mqttPUB_ExternTopic);

  cJSON_AddStringToObject(root, "mqttSUB_ResponseTopic", mqttSUB_ResponseTopic);
  cJSON_AddStringToObject(root, "mqttPUB_ResponseTopic", mqttPUB_ResponseTopic);

  cJSON_AddStringToObject(root, "mqttSUB_ServiceTopic", mqttSUB_ServiceTopic);
  cJSON_AddStringToObject(root, "mqttPUB_ServiceTopic", mqttPUB_ServiceTopic);

  cJSON_AddStringToObject(root, "mqttSUB_ControlTopic", mqttSUB_ControlTopic);
  cJSON_AddStringToObject(root, "mqttPUB_ControlTopic", mqttPUB_ControlTopic);

  cJSON_AddStringToObject(root, "mqttPUB_ReportTopic", mqttPUB_ReportTopic);

  cJSON_AddStringToObject(root, "deviceDriver_DownloadFileName", deviceDriver_DownloadFileName);

  /*设备驱动升级状态*/
  cJSON_AddNumberToObject(root, "updateSuccessful", updateSuccessful);

  /*配置启用日志输出*/
  cJSON_AddNumberToObject(root, "enableLogOutput", enableLogOutput);

  /*配置文件启用状态*/
  cJSON_AddNumberToObject(root, "enableConfigFile", enableConfigFile);

  char *out = cJSON_Print(root);
  fprintf(fp,"%s",out);
  fclose(fp);
  cJSON_free(out);

  if (NULL != root)
  {
    cJSON_Delete(root);
  }
  return true;
}

/**
  ******************************************************************
  * @brief   读取配置信息
  * @param   [in]None
  * @return  true表示读取完成
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
static bool starup_read_config(void)
{
  read_config("Software_Ver", Software_Ver, PARA_STRING);			/**< 读取软件版本信息*/

  read_config("udpServerPortNum", &udpServerPortNum, PARA_INT);	/**< UDP端口号*/

  read_config("tcpServerPortNum", &tcpServerPortNum, PARA_INT);	/**< TCP端口号*/

  read_config("udpRemoteServerIP", udpRemoteServerIP, PARA_STRING);/**< UDP远程服务器IP*/

  read_config("udpRemotePortNum", &udpRemotePortNum, PARA_INT);/**< UDP远程服务器端口*/

  read_config("tcpRemoteServerIP", tcpRemoteServerIP, PARA_STRING);/**< TCP远程服务器IP*/

  read_config("tcpRemotePortNum", &tcpRemotePortNum, PARA_INT);/**< TCP远程服务器端口*/	

  read_config("mqttClientID", mqttClientID, PARA_STRING);			/**< 读取mqtt客户端id*/

  read_config("mqttServer", mqttServer, PARA_STRING);				/**< 读取mqtt服务器ip*/

  read_config("mqttPort", &mqttPort, PARA_INT);					/**< 读取mqtt服务器端口号*/

  read_config("mqttUserName", mqttUserName, PARA_STRING);			/**< 读取mqtt用户名*/

  read_config("mqttUserKey", mqttUserKey, PARA_STRING);			/**< 读取mqtt用户密码*/

  read_config("mqttSUB_ExternTopic", mqttSUB_ExternTopic, PARA_STRING);			/**< 读取mqtt扩展订阅主题*/

  read_config("mqttPUB_ExternTopic", mqttPUB_ExternTopic, PARA_STRING);			/**< 读取mqtt扩展发布主题*/

  read_config("mqttSUB_ResponseTopic", mqttSUB_ResponseTopic, PARA_STRING);			/**< 读取mqtt响应订阅主题*/

  read_config("mqttPUB_ResponseTopic", mqttPUB_ResponseTopic, PARA_STRING);			/**< 读取mqtt响应发布主题*/

  read_config("mqttSUB_ServiceTopic", mqttSUB_ServiceTopic, PARA_STRING);			/**< 读取mqtt服务订阅主题*/

  read_config("mqttPUB_ServiceTopic", mqttPUB_ServiceTopic, PARA_STRING);			/**< 读取mqtt服务发布主题*/

  read_config("mqttSUB_ControlTopic", mqttSUB_ControlTopic, PARA_STRING);			/**< 读取mqtt控制订阅主题*/

  read_config("mqttPUB_ControlTopic", mqttPUB_ControlTopic, PARA_STRING);			/**< 读取mqtt控制发布主题*/

  read_config("mqttPUB_ReportTopic", mqttPUB_ReportTopic, PARA_STRING);			/**< 读取mqtt上报发布主题*/

  read_config("deviceDriver_DownloadFileName", deviceDriver_DownloadFileName, PARA_STRING);   /**< 读取设备驱动下载文件名*/

  read_config("updateSuccessful", &updateSuccessful, PARA_INT);/**< 设备更新状态*/
  
  read_config("enableLogOutput", &enableLogOutput, PARA_INT);	/**< 读取启用日志输出标志*/

  read_config("enableConfigFile", &enableConfigFile, PARA_INT);	/**< 读取启用配置文件标志*/

  return true;
}

/**
  ******************************************************************
  * @brief   去掉字符串内所有空白，且忽略注释部分，最终得到没有空白的字符串
  * @param   [in]string：字符串
  * @param   [in]comment：注释标识
  * @return  true表示数据可用
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
static bool strip_comments(char *string, char comment)
{
  if (NULL == string || '\n' == *string || '\r' == *string) {
    return false; /* 第一个字符为回车或换行,表示空行 */
  }

  char *p, *q;
  /* 下面去掉字符串中所有空白字符 */
  for (p = q = string; *p != '\0' && *p != comment; p++) {
	  /* 不是空白字符则重写字符串 */
    if (0 == isspace(*p)) {
      *q++ = *p;
    }
  }
  *q = '\0';

  return 0 != strlen(string); /* 字符串长度不为0,表示数据可用 */
}

/**
  ******************************************************************
  * @brief   增加字符串键值到json对象中
  * @param   [in]object：json对象
  * @param   [in]name：键名
  * @param   [in]string：键值
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
static void cjson_add_string_to_object(cJSON * const object, const char * const name, const char * const string)
{
    cJSON *item;

    if (object == NULL || name == NULL || string == NULL)
    {
    	printf("error\n");
    	reset_config_file();
        return;
    }

    item = cJSON_GetObjectItemCaseSensitive(object, name);
    /*item not exist*/
    if (item == NULL)
    {
    	printf("error!\n");
    	reset_config_file();
        cJSON_AddStringToObject(object, name, string);
    }
    else
    {
    	cJSON_ReplaceItemInObject(object,name,cJSON_CreateString(string));
/*非原生方式*/
//    	GNNC_DEBUG_INFO("修改配置文件：%s",item->valuestring);
//        cJSON_free(item->valuestring); // free present valuestring
//        GNNC_DEBUG_INFO("释放");
//        item->valuestring = strdup(string); // malloc and init new valuestring
//        GNNC_DEBUG_INFO("新配置文件：%s",item->valuestring);

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
  * @brief   修改配置文件中键值对
  * @param   [in]identifier：键值名称
  * @param   [in]value：值
  * @param   [in]PARA_DATA_Type_t数值类型：０代表整数，１代表double，２代表字符串
  * @return  0表示数据修改正常
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
int midify_config(const char * const identifier, const void *value, PARA_DATA_Type_t data_type)
{
	FILE  *fp 	= 	NULL;
	cJSON *root = 	NULL;
	char str[2048] = {0};
	char txt[64] = {0};
	int int_value;
	double float_value;
	const char *char_value = NULL;

	fp = file_open(CONFIG_FILE_PATH, READ_FILE_ONLY);
	if(NULL == fp) {
      PRINT_ERRMSG("fopen");
      exit(errno);
	}

	while((fgets(txt, 64, fp)) != NULL)
	{
		/*去掉字符串所有空白,注释也忽略*/
		if(strip_comments(txt, ' '))
		{
			  strcat(str,txt);
		}
	}
	fclose(fp);
	usleep(100);

	/*写入新配置*/
	fp = file_open(CONFIG_FILE_PATH, READ_WRITE_CREAT_CLEAR_FILE);
	root = cJSON_Parse(str);
	cJSON *item = cJSON_GetObjectItem(root,identifier);
	switch(data_type)
	{
	case PARA_INT:
//		cJSON_DeleteItemFromObject(root, identifier);
		int_value = *(int*)value;
		printf("设置值：%d\n",int_value);
		cJSON_SetIntValue(item, int_value);
//		cJSON_AddNumberToObject(root,identifier,int_value);
		break;
	case PARA_DOUBLE:
//		cJSON_DeleteItemFromObject(root, identifier);
		float_value = *(double*)value;
		printf("设置值：%.1f\n",float_value);
		cJSON_SetIntValue(item, float_value);
//		cJSON_AddNumberToObject(root,identifier,float_value);
		break;
	case PARA_STRING:
//		cJSON_DeleteItemFromObject(root, identifier);
		char_value = (const char *)value;
		printf("<%s>设置值：%s\n",identifier,char_value);
//		cJSON_AddStringToObject(root,identifier,char_value);
		cjson_add_string_to_object(root, identifier, char_value);
		break;
	default:
		printf("设置ERROR!\n");
		return -1;
		break;
	}
	char *out = cJSON_Print(root);
	fprintf(fp,"%s",out);
	fclose(fp);
	cJSON_free(out);
	if (NULL != root)
	{
		cJSON_Delete(root);
	}
	return 0;
}

/**
  ******************************************************************
  * @brief   读取配置文件中指定键值对
  * @param   [in]identifier：键值名称
  * @param   [out]value：读取的值写入
  * @param   [in]PARA_DATA_Type_t数值类型：０代表整数，１代表double，２代表字符串
  * @return  0表示数据读取正常
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
int read_config(const char * const identifier, void *value, PARA_DATA_Type_t data_type)
{
    FILE  *fp 	= 	NULL;
    cJSON *root = 	NULL;
    char str[1024] = {0};
    char txt[64] = {0};

    fp = file_open(CONFIG_FILE_PATH, READ_FILE_ONLY);
    if(NULL == fp) {
        PRINT_ERRMSG("fopen");
        return -1;
    }

    while((fgets(txt, 64, fp)) != NULL)
    {
      /*去掉字符串所有空白,注释也忽略*/
      if(strip_comments(txt, ' '))
      {
          strcat(str, txt);
      }
    }
    root = cJSON_Parse(str);
    if(root == NULL)
    {
        reset_config_file();
    }
    cJSON *item = cJSON_GetObjectItem(root,identifier);
    switch(data_type)
    {
      case PARA_INT:
        if(item != NULL)
        {
          if(item->type == cJSON_Number)
          {
            *(int*)value = item->valueint;
#if (ENABLE_PAR_SET_DEBUG)
            printf("获取数值：%d\n",item->valueint);
#endif
          }
        }
        break;
      case PARA_DOUBLE:
        if(item != NULL)
        {
          if(item->type == cJSON_Number)
          {
            *(double*)value = item->valuedouble;
          }
        }
        break;
      case PARA_STRING:
        if(item != NULL)
        {
          if(item->type == cJSON_String)
          {
            strcpy((char*)value,item->valuestring);
#if (ENABLE_PAR_SET_DEBUG)
            printf("获取参数：%s\n",(char*)value);
#endif
          }
        }
        break;
      default:
        printf("read ERROR!\n");
        return -1;
        break;
    }
    fclose(fp);
    if (NULL != root)
    {
      cJSON_Delete(root);
    }
    return 0;
}

/**
  ******************************************************************
  * @brief   重置参数配置文件
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
bool reset_config_file(void)
{
  /*清除文件*/
  printf("reset config par... \n");
  int status = unlink(CONFIG_FILE_PATH);
  sleep(3);
  if (0 != status)
  {
    printf("unlink failed \n");
    return false;
  }
  printf("the config file is removed,reset config file now... \n");

  /*参数文件错误，重新恢复参数*/
  return creat_config_file();
}

/**
  ******************************************************************
  * @brief   初始化配置文件,存在则读取，否则创建
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
void config_file_init(void)
{
  if(file_is_exist(CONFIG_FILE_PATH) != 0)
  {
    /*创建配置文件*/
    creat_config_file();
  }
  else
  {
    /*读取配置到内存*/
    starup_read_config();
  }
}

/**
  ******************************************************************
  * @brief   获得软件版本号
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-09-28
  ******************************************************************
  */
const char *GetSoftwareVersion(void)
{
  return (const char *)Software_Ver;
}

/**
  ******************************************************************
  * @brief   获得软件版本号
  * @param   ver 软件版本号
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-09-28
  ******************************************************************
  */
void SetConfigFileSoftwareVersion(const char *ver)
{
  if(ver == NULL)
  {
    return;
  }
  if(strncmp(Software_Ver, ver, 10) == 0)
  {
    return;
  }
  midify_config("Software_Ver", ver, PARA_STRING);
}

/**
  ******************************************************************
  * @brief   获得日志输出开关
  * @return  true表示配置启用
  * @author  aron566
  * @version V1.0
  * @date    2020-09-28
  ******************************************************************
  */
bool GetLogOutputEnableState(void)
{
  if(enableLogOutput == 0)
  {
    printf("set log out false.\n");
    return false;
  }
  printf("set log out true.\n");
  return true;
}

/**
  ******************************************************************
  * @brief   获得配置文件有效性
  * @return  true表示配置启用
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
bool GetConfigFileEnableState(void)
{
  if(enableConfigFile == 0)
  {
    return false;
  }
  return true;
}

/**
  ******************************************************************
  * @brief   设置配置文件为启用状态
  * @param   [in]state：设置状态
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2020-08-31
  ******************************************************************
  */
void SetConfigFileState(bool state)
{
  int state_flag = 0;
  if(state == true)
  {
    state_flag = 1;
  }
  else
  {
    state_flag = 0;
  }

  if(state_flag != enableConfigFile)
  {
    enableConfigFile = state_flag;
    midify_config("enableConfigFile", &enableConfigFile, PARA_INT);
  }
}


/**
  ******************************************************************
  * @brief   设置udp服务器端口号
  * @param   [in]num：服务器端口号
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2020-09-15
  ******************************************************************
  */
void SetudpServerPortNum(int num)
{
  if(num != udpServerPortNum)
  {
    udpServerPortNum = num;
    midify_config("udpServerPortNum", &udpServerPortNum, PARA_INT);
  }
}

/**
  ******************************************************************
  * @brief   获取udp服务器端口号
  * @param   [in]None
  * @return  udp服务器端口号
  * @author  aron566
  * @version V1.0
  * @date    2020-09-15
  ******************************************************************
  */
int GetUdpServerPort(void)
{
  return udpServerPortNum;
}

/**
  ******************************************************************
  * @brief   设置tcp服务器端口号
  * @param   [in]num：服务器端口号
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
void SettcpServerPortNum(int num)
{
  if(num != tcpServerPortNum)
  {
    tcpServerPortNum = num;
    midify_config("tcpServerPortNum", &tcpServerPortNum, PARA_INT);
  }
}

/**
  ******************************************************************
  * @brief   获取tcp服务器端口号
  * @param   [in]None
  * @return  tcp服务器端口号
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
int GetTCPServerPort(void)
{
  return tcpServerPortNum;
}

/**
  ******************************************************************
  * @brief   获取远程tcp服务器IP
  * @param   [in]None
  * @return  远程tcp服务器IP
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
char *GetTcpRemoteServerIP(void)
{
  return tcpRemoteServerIP;
}

/**
  ******************************************************************
  * @brief   设置远程tcp服务器端口号
  * @param   [in]num：服务器端口号
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
void SettcpRemotePortNum(int num)
{
  if(num != tcpRemotePortNum)
  {
    tcpRemotePortNum = num;
    midify_config("tcpRemotePortNum", &tcpRemotePortNum, PARA_INT);
  }
}

/**
  ******************************************************************
  * @brief   获取远程tcp服务器端口号
  * @param   [in]None
  * @return  tcp远程服务器端口号
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
int GettcpRemotePortNum(void)
{
  return tcpRemotePortNum;
}

/**
  ******************************************************************
  * @brief   获取远程udp服务器IP
  * @param   [in]None
  * @return  远程udp服务器IP
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
char *GetUdpRemoteServerIP(void)
{
  return udpRemoteServerIP;
}

/**
  ******************************************************************
  * @brief   设置远程udp服务器端口号
  * @param   [in]num：服务器端口号
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
void SetudpRemotePortNum(int num)
{
  if(num != udpRemotePortNum)
  {
    udpRemotePortNum = num;
    midify_config("udpRemotePortNum", &udpRemotePortNum, PARA_INT);
  }
}

/**
  ******************************************************************
  * @brief   获取远程udp服务器端口号
  * @param   [in]None
  * @return  udp远程服务器端口号
  * @author  aron566
  * @version V1.0
  * @date    2020-09-23
  ******************************************************************
  */
int GetudpRemotePortNum(void)
{
  return udpRemotePortNum;
}

/**
  ******************************************************************
  * @brief   获取mqtt客户端ID
  * @param   [in]None
  * @return  客户端ID
  * @author  aron566
  * @version V1.0
  * @date    2020-09-07
  ******************************************************************
  */
char *GetMqttClentID(void)
{
  return mqttClientID;
}

/**
  ******************************************************************
  * @brief   获取mqtt服务器地址
  * @param   [in]None
  * @return  服务器地址
  * @author  aron566
  * @version V1.0
  * @date    2020-09-07
  ******************************************************************
  */
char *GetMqttServerAddr(void)
{
  return mqttServer;
}

/**
  ******************************************************************
  * @brief   获取mqtt服务器端口
  * @param   [in]None
  * @return  服务器端口
  * @author  aron566
  * @version V1.0
  * @date    2020-09-07
  ******************************************************************
  */
char *GetMqttServerPort(void)
{
  static char port_str[6];
  sprintf(port_str, "%d", mqttPort);
  return port_str;
}

/**
  ******************************************************************
  * @brief   获取mqtt登录用户名
  * @param   [in]None
  * @return  mqtt登录用户名
  * @author  aron566
  * @version V1.0
  * @date    2020-09-07
  ******************************************************************
  */
char *GetMqttLoginName(void)
{
  return mqttUserName;
}

/**
  ******************************************************************
  * @brief   获取mqtt登录密码
  * @param   [in]None
  * @return  mqtt登录密码
  * @author  aron566
  * @version V1.0
  * @date    2020-09-07
  ******************************************************************
  */
char *GetMqttLoginKey(void)
{
  return mqttUserKey;
}

/**
  ******************************************************************
  * @brief   获取扩展订阅主题
  * @param   [in]None
  * @return  订阅主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttSubExternTopic(void)
{
  return mqttSUB_ExternTopic;
}

/**
  ******************************************************************
  * @brief   获取扩展发布主题
  * @param   [in]None
  * @return  发布主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttPublishExternTopic(void)
{
  return mqttPUB_ExternTopic;
}

/**
  ******************************************************************
  * @brief   获取服务订阅主题
  * @param   [in]None
  * @return  订阅主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttSubServiceTopic(void)
{
  return mqttSUB_ServiceTopic;
}

/**
  ******************************************************************
  * @brief   获取服务发布主题
  * @param   [in]None
  * @return  发布主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttPublishServiceTopic(void)
{
  return mqttPUB_ServiceTopic;
}

/**
  ******************************************************************
  * @brief   获取控制订阅主题
  * @param   [in]None
  * @return  订阅主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttSubControlTopic(void)
{
  return mqttSUB_ControlTopic;
}

/**
  ******************************************************************
  * @brief   获取控制发布主题
  * @param   [in]None
  * @return  发布主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttPublishControlTopic(void)
{
  return mqttPUB_ControlTopic;
}

/**
  ******************************************************************
  * @brief   获取响应订阅主题
  * @param   [in]None
  * @return  订阅主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttSubResponseTopic(void)
{
  return mqttSUB_ResponseTopic;
}

/**
  ******************************************************************
  * @brief   获取响应发布主题
  * @param   [in]None
  * @return  发布主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttPublishResponseTopic(void)
{
  return mqttPUB_ResponseTopic;
}

/**
  ******************************************************************
  * @brief   获取主动上报发布主题
  * @param   [in]None
  * @return  发布主题
  * @author  aron566
  * @version V1.0
  * @date    2020-10-16
  ******************************************************************
  */
char *GetMqttPublishReportTopic(void)
{
  return mqttPUB_ReportTopic;
}

/**
  ******************************************************************
  * @brief   获取设备驱动下载文件名
  * @param   [in]None
  * @return  设备驱动下载文件名
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
char *GetDeviceDriver_DownloadFileName(void)
{
  return deviceDriver_DownloadFileName;
}

/**
  ******************************************************************
  * @brief   获得设备驱动升级状态
  * @return  true表示升级成功
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
bool GetDeviceDriverUpdateState(void)
{
  if(updateSuccessful == 0)
  {
    printf("device driver update faild.\n");
    return false;
  }
  printf("device driver update successful.\n");
  return true;
}

/**
  ******************************************************************
  * @brief   设置设备驱动升级状态
  * @param   [in]state：设置状态
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2020-12-14
  ******************************************************************
  */
void SetDeviceDriverUpdateState(bool state)
{
  int state_flag = 0;
  if(state == true)
  {
    state_flag = 1;
  }
  else
  {
    state_flag = 0;
  }

  if(state_flag != updateSuccessful)
  {
    updateSuccessful = state_flag;
    midify_config("updateSuccessful", &updateSuccessful, PARA_INT);
  }
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
