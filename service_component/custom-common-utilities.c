/**                                                                             
 *  @file custom-common-utilities.c                                                   
 *                                                                              
 *  @date 2020年09月17日 11:11:59 星期四
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief None.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>///<for struct ifreq
#include <sys/sysinfo.h>///< sysinfo
#include <unistd.h>
/* Private includes ----------------------------------------------------------*/
#include "custom-common-utilities.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                               
/** Private constants --------------------------------------------------------*/

/** Public variables ---------------------------------------------------------*/
/*设置输出颜色*/
const char *red_color = "\E[1;31m";
const char *color_suffix = "\E[0m";
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
                                                                                
/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/
/**
 * @brief 16进制格式调试打印
 * 
 * @param msg 
 * @param msg_len 
 */
void debug_print(uint8_t *msg ,uint32_t msg_len)
{
  for (uint32_t i = 0; i < msg_len; i++)
  printf("%02X ",
      msg[i]); //打印2位的16进制数，不足位0补齐。
  printf("\n");
}

/**
 * @brief 安全字符串拷贝
 * 
 * @param dest_str 目标存储区
 * @param src_str 源字符串
 * @param size 最大拷贝字符数目
 * @return char* 目标存储区
 */
char *strncopy(char *dest_str, const char *src_str, size_t size)
{
  if(size == 0 || dest_str == NULL || src_str == NULL)
  {
      return dest_str;
  }
  strncpy(dest_str, src_str, size);
  dest_str[size-1] = '\0';
  return dest_str;
}

/**
  ******************************************************************
  * @brief   获取数值对应的字符串
  * @param   [out]dest_str 字符串存储区
  * @param   [in]data 数值
  * @param   [in]size 存储区长度
  * @param   [in]value_type 数值类型
  * @return  str.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-08
  ******************************************************************
  */
char *get_value_str(char *dest_str, void *data, size_t size, VALUE_Type_t value_type)
{
  if(dest_str == NULL || data == NULL || size <= 0 || value_type == VALUE_TYPE_MAX)
  {
    return dest_str;
  }
  switch(value_type)
  {
    case INT8:
      snprintf(dest_str, size, "%hhd", *(int8_t*)data);
      break;
    case INT16:
      snprintf(dest_str, size, "%hd", *(int16_t*)data);
      break;
    case INT32:
      snprintf(dest_str, size, "%d", *(int32_t*)data);
    case INT64:
      snprintf(dest_str, size, "%ld", *(int64_t*)data);
      break;
    case UINT8:
      snprintf(dest_str, size, "%hhu", *(uint8_t*)data);
      break;
    case UINT16:
      snprintf(dest_str, size, "%hu", *(uint16_t*)data);
      break;
    case UINT32:
      snprintf(dest_str, size, "%u", *(uint32_t*)data);
      break;
    case UINT64:
      snprintf(dest_str, size, "%lu", *(uint64_t*)data);
      break;
    case FLOAT32:
      snprintf(dest_str, size, "%f", *(float*)data);
      break;
    case DOUBLE:      
      snprintf(dest_str, size, "%lf", *(double*)data);
      break;
    case STRING:
      snprintf(dest_str, size, "%s", (const char*)data);
      break;
    default:
      break;
  }
  return dest_str;
}
    
/**
 * @brief Get the curent time s object
 * 
 * @return uint64_t 时间秒
 */
uint64_t get_current_time_s(UTILITIES_TIME_MODE_Typedef_t mode)
{
  struct timespec timespe;
  struct timeval tv;
  struct tm nowTime;
  
  char str[256];
#if defined(__linux__)
  switch(mode)
  {
    case CURRENT_TIME:
      clock_gettime(CLOCK_REALTIME ,&timespe);
      break;
    case RUN_TIME:
      clock_gettime(CLOCK_MONOTONIC ,&timespe);
      break;
    case PROCESS_CPUTIME:
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID ,&timespe);
      break;
    case THREAD_CPUTIME:
      clock_gettime(CLOCK_THREAD_CPUTIME_ID ,&timespe);
      break;
    case CURRENT_TIME_MS:
      gettimeofday(&tv,NULL);
      break;
  }
#else
	timespe.tv_sec = time(NULL);
#endif
  if(mode != CURRENT_TIME_MS)
  {
    localtime_r(&timespe.tv_sec, &nowTime);
    sprintf(str ,"GetTime:%04d-%02d-%02d-%02d:%02d:%02d\n"
      ,nowTime.tm_year + 1900 ,nowTime.tm_mon+1 ,nowTime.tm_mday ,nowTime.tm_hour ,nowTime.tm_min ,nowTime.tm_sec);
    // printf("%s", str);
    return (uint64_t)timespe.tv_sec;
  }
  // printf("second:%ld\n", tv.tv_sec);
  // printf("millisecond:%ld\n", tv.tv_sec*1000 + tv.tv_usec/1000);
  // printf("microsecond:%ld\n", tv.tv_sec*1000000 + tv.tv_usec);
  return (uint64_t)(tv.tv_sec*1000 + (tv.tv_usec/1000));
}

/**
 * @brief Get the time str object
 * 
 * @param sec 
 * @return const char* 
 */
const char *get_time_str(time_t sec)
{
  static char date_str[100] = {0};
  struct timespec timespe;
  struct tm nowTime;
  timespe.tv_sec = sec;
  localtime_r(&timespe.tv_sec, &nowTime);
  sprintf(date_str ,"%04d-%02d-%02d-%02d:%02d:%02d"
      ,nowTime.tm_year + 1900 ,nowTime.tm_mon+1 ,nowTime.tm_mday ,nowTime.tm_hour ,nowTime.tm_min ,nowTime.tm_sec);
  return (const char*)date_str;
}

/**
 * @brief 16进制字符转为数值
 * 
 * @param ch 16进制字符
 * @return uint8_t 数值
 */
uint8_t hex_char_to_value(uint8_t ch)
{
  uint8_t result = 0;
  /*获取16进制的高字节位数据*/
  if (ch >= '0' && ch <= '9')
  {
    result = ch - '0';
  }
  else if (ch >= 'a' && ch <= 'z')
  {
    result = ch - 'a' + 10;
  }
  else if (ch >= 'A' && ch <= 'Z')
  {
    result = ch - 'A' + 10;
  }
  else
  {
    result = 0;
  }
  return result;
}

/**
 * @brief 16进制转字符串
 * 
 * @param strbuf 字符串存储区
 * @param hex_data 16进制数据
 * @param len hex数据字节数
 * @return None 
 */
void hex_to_str(char *strbuf ,uint8_t *hex_data ,uint32_t len)
{
  if(strbuf == NULL || hex_data == NULL)
  {
    return;
  }
  uint32_t str_offset = 0;
  for(uint32_t index = 0;index < len;index++)
  {
    snprintf(strbuf+str_offset ,3 ,"%02X" ,hex_data[index]);
    str_offset +=2;
  }
  strbuf[str_offset] = '\0';
}
/**
 * @brief 将大写字母转换成小写字母
 * 
 * @param ch 大写字母
 * @return uint8_t 小写字母
 */
uint8_t ch_tolower(uint8_t ch)
{
  if(ch >= 'A' && ch <= 'Z')
  {
    return ch + 'a' - 'A';
  }
  else
  {
    return ch;
  }
}

/**
 * @brief 16进制的字符串转换成整数
 * 
 * @param s 16进制字符串
 * @return int 数值
 */
int hextoi(char s[])
{
  int i = 0;
  int ret = 0;
  if(s[0] == '0' && (s[1]=='x' || s[1]=='X'))
  {  
    i = 2;
  }
  else
  {
    i = 0;
  }
  for(;(s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)
  {
    if(ch_tolower(s[i]) > '9')
    {
      ret = 16 * ret + (10 + ch_tolower(s[i]) - 'a');
    }
    else
    {
      ret = 16 * ret + (ch_tolower(s[i]) - '0');
    }
  }
  return ret;
}

/**
 ******************************************************************
 * @brief   过滤字符串中指定字符
 * @param   [in]ch 过滤字符
 * @param   [in]str 原字符串
 * @param   [in]out_str 过滤后的字符串
 * @param   [in]size 存储区大小
 * @return  字符串长度，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-24
 ******************************************************************
 */
int common_filter_special_char(char ch, const char *str, char *out_str, int size)
{
  if(str == NULL || out_str == NULL || size <= 1)
  {
    return -1;
  }
  int len = 0;
  size_t total_len = strlen(str);
  for(size_t index = 0; index < total_len; index++)
  {
    if(str[index] != ch)
    {
      out_str[len] = str[index];
      len++;
      if(len >= size)
      {
        break;
      }
    }
  }
  out_str[len-1] = '\0';
  return len;
}

/**
  ******************************************************************
  * @brief   解析浮点数数据
  * @param   [in]data 完整报文，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  float
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
float common_get_float_data(uint8_t *data ,int start_index)
{
  static float result_float = 0.0;
  if(data == NULL)
  {
    return result_float;
  }
  uint32_t result_temp = data[start_index];
  result_temp |= (((uint32_t)data[start_index+1])<<8);
  result_temp |= (((uint32_t)data[start_index+2])<<16);
  result_temp |= (((uint32_t)data[start_index+3])<<24);
  result_float = *(float*)&result_temp;
  return result_float;
}

/**
  ******************************************************************
  * @brief   解析整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint16_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint16_t common_get_u16_data(uint8_t *data ,int start_index)
{
  static uint16_t result_u16 = 0;
  if(data == NULL)
  {
    return result_u16;
  }
  result_u16 = data[start_index];
  result_u16 |= (((uint16_t)data[start_index+1])<<8);
  return result_u16;
}

/**
  ******************************************************************
  * @brief   解析整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint32_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint32_t common_get_u32_data(uint8_t *data ,int start_index)
{
  static uint32_t result_u32 = 0;
  if(data == NULL)
  {
    return result_u32;
  }
  result_u32 = data[start_index];
  result_u32 |= (((uint32_t)data[start_index+1])<<8);
  result_u32 |= (((uint32_t)data[start_index+2])<<16);
  result_u32 |= (((uint32_t)data[start_index+3])<<24);
  return result_u32;
}

/**
  ******************************************************************
  * @brief   解析32位整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint32_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint32_t common_get_modbus_u32_data(uint8_t *data ,int start_index)
{
  static uint32_t result_u32 = 0;
  if(data == NULL)
  {
    return result_u32;
  }
  result_u32 = (((uint32_t)data[start_index])<<24);
  result_u32 |= (((uint32_t)data[start_index+1])<<16);
  result_u32 |= (((uint32_t)data[start_index+2])<<8);
  result_u32 |= (((uint32_t)data[start_index+3]));
  return result_u32;
}

/**
  ******************************************************************
  * @brief   解析16位整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint16_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint16_t common_get_modbus_u16_data(uint8_t *data ,int start_index)
{
  static uint16_t result_u16 = 0;
  if(data == NULL)
  {
    return result_u16;
  }
  result_u16 = (((uint16_t)data[start_index])<<8);
  result_u16 |= (((uint16_t)data[start_index+1]));
  return result_u16;
}

/**
  ******************************************************************
  * @brief   解析浮点数数据
  * @param   [in]data 完整报文，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  float
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
float common_get_modbus_float_data(uint8_t *data ,int start_index)
{
  static float result_float = 0.0;
  if(data == NULL)
  {
    return result_float;
  }
  uint32_t result_temp = (((uint32_t)data[start_index])<<24);
  result_temp |= (((uint32_t)data[start_index+1])<<16);
  result_temp |= (((uint32_t)data[start_index+2])<<8);
  result_temp |= (((uint32_t)data[start_index+3]));
  result_float = *(float*)&result_temp;
  return result_float;
}

/**
  ******************************************************************
  * @brief   获取主机名
  * @param   [in]None
  * @return  char *host_name_str
  * @author  aron566
  * @version V1.0
  * @date    2020-10-10
  ******************************************************************
  */
char *get_host_name(void)
{
  static char host_name_str[64] = {0};
  int ret = gethostname(host_name_str, 64);
  if(ret != 0)
  {
    return NULL;
  }
  return host_name_str;
}

/**
  ******************************************************************
  * @brief   获取本地mac地址
  * @param   [in]hardware_name 网卡名称
  * @return  char *mac地址字符串
  * @author  aron566
  * @version V1.1
  * @date    2020-10-13
  ******************************************************************
  */
char *get_local_mac(const char *hardware_name)
{
  int sock_mac;
  struct ifreq ifr_mac;
  static char mac_addr[30] = {0};

  if(hardware_name == NULL)
  {
    return mac_addr;
  }
  sock_mac = socket(AF_INET ,SOCK_STREAM ,0); 
  if(sock_mac == -1)
  {
    perror("create socket falise...mac");
    return NULL;
  }

  memset(&ifr_mac ,0,sizeof(ifr_mac));
  strncpy(ifr_mac.ifr_name ,hardware_name ,sizeof(ifr_mac.ifr_name)-1);

  if((ioctl(sock_mac ,SIOCGIFHWADDR ,&ifr_mac)) < 0)
  {
    printf("mac ioctl error.");
  if(sock_mac != -1)
  {
    close(sock_mac);	
  }
    return NULL;
  }

  sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",
          (uint8_t)ifr_mac.ifr_hwaddr.sa_data[0],
          (uint8_t)ifr_mac.ifr_hwaddr.sa_data[1],
          (uint8_t)ifr_mac.ifr_hwaddr.sa_data[2],
          (uint8_t)ifr_mac.ifr_hwaddr.sa_data[3],
          (uint8_t)ifr_mac.ifr_hwaddr.sa_data[4],
          (uint8_t)ifr_mac.ifr_hwaddr.sa_data[5]);
  close(sock_mac);
  return mac_addr;
}

/**
  ******************************************************************
  * @brief   获取本机内存信息
  * @param   [in]None
  * @return  struct sysinfo*
  * @author  aron566
  * @version V1.0
  * @date    2020-10-10
  ******************************************************************
  */
struct sysinfo *get_meminfo(void)
{
  static struct sysinfo info;
  // long uptime;          /* 启动到现在经过的时间 */  
  // unsigned long loads[3];    
  // /* 1, 5, and 15 minute load averages */  
  // unsigned long totalram;  /* 总的可用的内存大小 */  
  // unsigned long freeram;   /* 还未被使用的内存大小 */  
  // unsigned long sharedram; /* 共享的存储器的大小*/  
  // unsigned long bufferram; /* 共享的存储器的大小 */  
  // unsigned long totalswap; /* 交换区大小 */  
  // unsigned long freeswap;  /* 还可用的交换区大小 */  
  // unsigned short procs;    /* 当前进程数目 */  
  // unsigned long totalhigh; /* 总的高内存大小 */  
  // unsigned long freehigh;  /* 可用的高内存大小 */  
  // unsigned int mem_unit;   /* 以字节为单位的内存大小 */      
  int ret = -1;
  ret = sysinfo(&info);
  if(ret != 0)
  {
    printf("get meminfo error.");
    return NULL;
  }
  // printf("uptime             : %ld\n",info.uptime);//打印从设备开启到现在的时间，单位为秒    
  // printf("1 min load average : %lu\n",info.loads[0]);    
  // printf("5 min load average : %lu\n",info.loads[1]);    
  // printf("15 min load average: %lu\n",info.loads[2]);//平均负载    
  // printf("totalram           : %lu\n",info.totalram);//总可用内存大小  
  // printf("freeram            : %lu\n",info.freeram); //剩余内存   
  // printf("procs              : %u\n",info.procs);    //进程数
  return &info;
}

/**
  ******************************************************************
  * @brief   获取本机CPU信息
  * @param   [in]None
  * @return  int cpu数目
  * @author  aron566
  * @version V1.0
  * @date    2020-10-10
  ******************************************************************
  */
int get_cpu_num(void)
{
  char buf[16] = {0};
  int num;
  FILE* fp = popen("cat /proc/cpuinfo |grep processor|wc -l", "r");
  if(fp) 
  {                                                                                                                                                                             
    fread(buf, 1, sizeof(buf) - 1 ,fp);
    pclose(fp);
  }   
  num = atoi(buf);
  if(num <= 0)
  { 
    num = 1;
  }
  return num;
}

/**
  ******************************************************************
  * @brief   获取本机CPU频率
  * @param   [in]None
  * @return  double 频率
  * @author  aron566
  * @version V1.0
  * @date    2020-10-10
  ******************************************************************
  */
double get_cpu_frequency(void)
{
  FILE *fp;
  char str[81];
  memset(str ,0 ,81);
  fp = popen("cat /proc/cpuinfo|grep cpu\\ MHz|sed -e 's/.*:[^0-9]//'" ,"r");
  if(fp < 0)
  {
    printf("can't read CPU main frequency.");
    return 0.0;
  }
  fgets(str ,80 ,fp);
  fclose(fp);
  double spd = atof(str);
  return spd;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                