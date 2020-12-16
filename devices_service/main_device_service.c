/**                                                                            
 *  @file main_device_service.c                                                   
 *                                                                              
 *  @date 2020年11月08日 11:14:06 星期天
 *
 *  @author aron566
 *
 *  @copyright None
 *
 *  @brief EdgeXFoudry 设备服务驱动.
 *
 *  @details Pseudo-device service illustrating resource aggregation using C SDK .
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <dlfcn.h>
/* Private includes ----------------------------------------------------------*/
#include "devsdk/devsdk.h"
#include "main_device_service.h"
/** Private typedef ----------------------------------------------------------*/

/*设备驱动初始化*/
typedef void (*dl_device_driver_opt_init)(void *data, const iot_data_t *config);

/*设备驱动启动*/
typedef void (*dl_device_driver_opt_reconfigure)(iot_logger_t *lc, const iot_data_t *config);

/*设备驱动停止*/
typedef void (*dl_device_driver_opt_stop)(iot_logger_t *lc, bool force);

/*设备驱动发现*/
typedef void (*dl_device_driver_opt_discover)(iot_logger_t *lc);

/*设备驱动get接口*/
typedef int (*dl_device_driver_opt_get)(const char *devname, const char *param, devsdk_commandresult *readings, 
                          iot_logger_t *lc);

/*设备驱动set接口*/
typedef int (*dl_device_driver_opt_set)(const char *devname, const char *param, const iot_data_t *values, 
                          iot_logger_t *lc);

/*响应添加设备*/
typedef void (*dl_device_driver_add_device)(void *impl, const char *devname, const devsdk_protocols *protocols, 
                              const devsdk_device_resources *resources, bool adminEnabled);

/*响应更新设备*/
typedef void (*dl_device_driver_update_device)(void *impl, const char *devname, const devsdk_protocols *protocols, 
                                  bool adminEnabled);

/*响应移除设备*/
typedef void (*dl_device_driver_remove_device)(void *impl, const char *devname, const devsdk_protocols *protocols);

/*启动事件*/
typedef void *(*dl_device_driver_autoevent_start_handler)(void *impl, const char *devname, const devsdk_protocols *protocols,
                                            const char *resource_name,
                                            uint32_t nreadings,
                                            const devsdk_commandrequest *requests,
                                            uint64_t interval,
                                            bool onChange);
/*停止事件*/
typedef void (*dl_device_driver_autoevent_stop_handler)(void *impl, void *handle);
/** Private macros -----------------------------------------------------------*/
#define ERR_CHECK(x) if(x.code){fprintf(stderr, "Error: %d: %s\n", x.code, x.reason); devsdk_service_free (service); free (impl); return x.code;}
#define ERR_BUFSZ 1024
#define ERR_CUSTOM_DEVICE_WRITE "PUT called for custom_device device. This is a read-only device."
#define ERR_CUSTOM_DEVICE_NO_PARAM "No parameter attribute in GET/PUT request."

#define USE_DL_OPEN 0/**< 不使用dlopen 动态库的静态链接库无法使用，导致断错误*/
#define ERR_EXIT_MSG do{ printf("unknow this func symbol.\n"); \
                         exit(-1); \
                       }while(0)
#define DEVICE_DRIVER_PATH_NAME "/home/aron566/Workspace/custom_device_driver/build/device_driver/libdevice_driver.so"
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/

#if USE_DL_OPEN
/*csdk user data*/
typedef struct custom_device_driver
{
  iot_logger_t * lc;
  devsdk_service_t * service;
  void (*device_driver_reload_callback)(void);
}custom_device_driver;
static void *dl_device_driver_handle = NULL;
static dl_device_driver_opt_init device_driver_opt_init = NULL;
static dl_device_driver_opt_reconfigure device_driver_opt_reconfigure = NULL;
static dl_device_driver_opt_stop device_driver_opt_stop = NULL;
static dl_device_driver_opt_discover device_driver_opt_discover = NULL;
static dl_device_driver_opt_get device_driver_opt_get = NULL;
static dl_device_driver_opt_set device_driver_opt_set = NULL;
static dl_device_driver_add_device device_driver_add_device = NULL;
static dl_device_driver_update_device device_driver_update_device = NULL;
static dl_device_driver_remove_device device_driver_remove_device = NULL;
static dl_device_driver_autoevent_start_handler device_driver_autoevent_start_handler = NULL;
static dl_device_driver_autoevent_stop_handler device_driver_autoevent_stop_handler = NULL;
#endif
/** Private function prototypes ----------------------------------------------*/
/*初始化设备服务*/
static bool custom_device_init(void * impl, struct iot_logger_t * lc, const iot_data_t * config);
/*响应GET请求*/
static bool custom_device_get_handler(void * impl, const char * devname,
                                      const devsdk_protocols * protocols,
                                      uint32_t nreadings,
                                      const devsdk_commandrequest * requests,devsdk_commandresult * readings,
                                      const devsdk_nvpairs * qparams,iot_data_t ** exception);
/*响应PUT请求*/
static bool custom_device_put_handler(void * impl, const char * devname, const devsdk_protocols * protocols,
                                      uint32_t nvalues,
                                      const devsdk_commandrequest * requests,
                                      const iot_data_t * values[],
                                      iot_data_t ** exception);
/*响应重置请求*/
static void custom_device_reconfigure(void *impl, const iot_data_t *config);
/*响应发现请求*/
static void custom_device_discover(void *impl);
/*响应停止请求*/
static void custom_device_stop(void * impl, bool force);
/*响应添加设备*/
static void custom_device_add_device(void *impl, const char *devname, const devsdk_protocols *protocols, const devsdk_device_resources *resources, bool adminEnabled);
/*响应更新设备*/
static void custom_device_update_device(void *impl, const char *devname, const devsdk_protocols *protocols, bool adminEnabled);
/*响应移除设备*/
static void custom_device_remove_device(void *impl, const char *devname, const devsdk_protocols *protocols);
/*启动事件*/
static void * custom_device_autoevent_start_handler(void *impl, const char *devname, const devsdk_protocols *protocols,
                                            const char *resource_name,
                                            uint32_t nreadings,
                                            const devsdk_commandrequest *requests,
                                            uint64_t interval,
                                            bool onChange);
/*停止事件*/
static void custom_device_autoevent_stop_handler(void *impl, void *handle);
/** Private user code --------------------------------------------------------*/                                                                         
/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/
#if USE_DL_OPEN
/**
 * @brief 装载设备驱动.
 */
static void device_driver_load(void)
{
  dl_device_driver_handle = dlopen(DEVICE_DRIVER_PATH_NAME, RTLD_GLOBAL|RTLD_NOW);
  if(dl_device_driver_handle == NULL)
  {
    fprintf(stderr, "%s\n", dlerror());
    exit(-1);
  }
  else
  {
    device_driver_opt_init = (dl_device_driver_opt_init)dlsym(dl_device_driver_handle, "device_driver_opt_init");
    if(!device_driver_opt_init) ERR_EXIT_MSG;
    device_driver_opt_reconfigure = (dl_device_driver_opt_reconfigure)dlsym(dl_device_driver_handle, "device_driver_opt_reconfigure");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_opt_stop = (dl_device_driver_opt_stop)dlsym(dl_device_driver_handle, "device_driver_opt_stop");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_opt_discover = (dl_device_driver_opt_discover)dlsym(dl_device_driver_handle, "device_driver_opt_discover");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_opt_get = (dl_device_driver_opt_get)dlsym(dl_device_driver_handle, "device_driver_opt_get");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_opt_set = (dl_device_driver_opt_set)dlsym(dl_device_driver_handle, "device_driver_opt_set");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_add_device = (dl_device_driver_add_device)dlsym(dl_device_driver_handle, "device_driver_add_device");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_update_device = (dl_device_driver_update_device)dlsym(dl_device_driver_handle, "device_driver_update_device");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_remove_device = (dl_device_driver_remove_device)dlsym(dl_device_driver_handle, "device_driver_remove_device");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_autoevent_start_handler = (dl_device_driver_autoevent_start_handler)dlsym(dl_device_driver_handle, "device_driver_autoevent_start_handler");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
    device_driver_autoevent_stop_handler = (dl_device_driver_autoevent_stop_handler)dlsym(dl_device_driver_handle, "device_driver_autoevent_stop_handler");
    if(!device_driver_opt_reconfigure) ERR_EXIT_MSG;
  }
}
#endif
/**
 * @brief 重装载设备驱动
 * 
 */
static void device_driver_reload(void)
{
  // if(dl_device_driver_handle == NULL)
  // {
  //   return;
  // }
  // dlclose(dl_device_driver_handle);
  // dl_device_driver_handle = NULL;
  // device_driver_load();
  printf("you can press cmd: reboot to update driver\n");
}

/**
 * @brief Function called during service start operation.
 * @param impl The context data passed in when the service was created.
 * @param lc A logging client for the device service.
 * @param config A string map containing the configuration specified in the service's "Driver" section.
 * @return true if the operation was successful, false otherwise.
 */
static bool custom_device_init(void * impl, struct iot_logger_t * lc, const iot_data_t * config)
{
  custom_device_driver * driver = (custom_device_driver *)impl;
  driver->lc = lc;

  /*初始化设备驱动*/
  device_driver_opt_init(impl, config);

  return true;
}

/**
 * @brief Callback issued to handle GET requests for device readings.
 * @param impl The context data passed in when the service was created.
 * @param devname The name of the device to be queried.
 * @param protocols The location of the device to be queried.
 * @param nreadings The number of readings requested.
 * @param requests An array specifying the readings that have been requested.
 * @param readings An array in which to return the requested readings.
 * @param qparams Query Parameters which were set for this request.
 * @param exception Set this to an IOT_DATA_STRING to give more information if the operation fails.
 * @return true if the operation was successful, false otherwise.
 */
static bool custom_device_get_handler(
  void * impl,                            /**< 自定义数据*/
  const char * devname,                   /**< 设备名称*/
  const devsdk_protocols * protocols,     /**< 请求设备所归属的协议*/
  uint32_t nreadings,                     /**< 请求类型的数量*/
  const devsdk_commandrequest * requests, /**< 请求参数列表*/
  devsdk_commandresult * readings,        /**< 返回结果给Edge控制台*/
  const devsdk_nvpairs * qparams,         /**< 请求的附加参数*/
  iot_data_t ** exception)                /**< 返回请求结果说明信息*/
{
  int ret = 0;
  const char *param;

  custom_device_driver * driver = (custom_device_driver *) impl;
  for(uint32_t i = 0; i < nreadings; i++)
  {
    /*获取GET参数*/
    /*参数来自.yaml中deviceResources列表中attributes项parameter键值*/
    param = devsdk_nvpairs_value (requests[i].attributes, "parameter");
    if(param == NULL)
    {
      iot_log_error (driver->lc, ERR_CUSTOM_DEVICE_NO_PARAM);
      * exception = iot_data_alloc_string (ERR_CUSTOM_DEVICE_NO_PARAM, IOT_DATA_REF);
      return false;
    }

    ret = device_driver_opt_get(devname, param, &readings[i], driver->lc);
    if(ret != 0)
    {
      iot_log_error(driver->lc, "[%s][%s] get dev: %s par [%s] error.", __FILE__, __FUNCTION__, devname, param);
    }
  }
  return true;
}

/**
 * @brief Callback issued to handle PUT requests for setting device values.
 * @param impl The context data passed in when the service was created.
 * @param devname The name of the device to be queried.
 * @param protocols The location of the device to be queried.
 * @param nvalues The number of set operations requested.
 * @param requests An array specifying the resources to which to write.
 * @param values An array specifying the values to be written.
 * @param exception Set this to an IOT_DATA_STRING to give more information if the operation fails.
 * @return true if the operation was successful, false otherwise.
 */
static bool custom_device_put_handler(
  void * impl,
  const char * devname,
  const devsdk_protocols * protocols,
  uint32_t nvalues,
  const devsdk_commandrequest * requests,
  const iot_data_t * values[],
  iot_data_t ** exception)
{
  int ret = 0;
  const char *param;

  custom_device_driver * driver = (custom_device_driver *) impl;
  for(uint32_t i = 0; i < nvalues; i++)
  {
    param = devsdk_nvpairs_value (requests[i].attributes, "parameter");
    if(param == NULL)
    {
      iot_log_error (driver->lc, ERR_CUSTOM_DEVICE_NO_PARAM);
      * exception = iot_data_alloc_string (ERR_CUSTOM_DEVICE_NO_PARAM, IOT_DATA_REF);
      continue;
    }

    /*设置请求*/
    ret = device_driver_opt_set(devname, param, values[i], driver->lc);
    if(ret != 0)
    {
      iot_log_error(driver->lc, "[%s][%s] set dev: %s par [%s] error.", __FILE__, __FUNCTION__, devname, param);
    }
    
  }
  return true;
}

/**
 * @brief Function called when configuration is updated.
 * @param impl The context data passed in when the service was created.
 * @param config A string map containing the new configuration.
 */
static void custom_device_reconfigure(void *impl, const iot_data_t *config)
{
  custom_device_driver * driver = (custom_device_driver *) impl;
  device_driver_opt_reconfigure(driver->lc, config);
}

/**
 * @brief Optional callback for dynamic discovery of devices. The implementation should detect devices and register them using
 *        the devsdk_add_device API call.
 * @param impl The context data passed in when the service was created.
 */
static void custom_device_discover(void *impl) 
{
  custom_device_driver * driver = (custom_device_driver *) impl;
  device_driver_opt_discover(driver->lc);
}

/**
 * @brief Callback issued during device service shutdown. The implementation should stop processing and release any resources that were being used.
 * @param impl The context data passed in when the service was created.
 * @param force A 'force' stop has been requested. An unclean shutdown may be performed if necessary.
 */
static void custom_device_stop(void *impl, bool force)
{
  /* Stop performs any final actions before the device service is terminated */
  custom_device_driver * driver = (custom_device_driver *) impl;
  device_driver_opt_stop(driver->lc, force);
}

/**
 * @brief Callback function indicating that a new device has been added.
 * @param impl The context data passed in when the service was created.
 * @param devname The name of the new device.
 * @param protocols The protocol properties that comprise the device's address.
 * @param resources The operations supported by the device.
 * @param adminEnabled Whether the device is administratively enabled.
 */
static void custom_device_add_device(void *impl, const char *devname, const devsdk_protocols *protocols, const devsdk_device_resources *resources, bool adminEnabled)
{
  device_driver_add_device(impl, devname, protocols, resources, adminEnabled);
}

/**
 * @brief Callback function indicating that a device's address or adminstate has been updated.
 * @param impl The context data passed in when the service was created.
 * @param devname The name of the updated device.
 * @param protocols The protocol properties that comprise the device's address.
 * @param state The device's current adminstate.
 */
static void custom_device_update_device(void *impl, const char *devname, const devsdk_protocols *protocols, bool adminEnabled)
{
  device_driver_update_device(impl, devname, protocols, adminEnabled);
}

/**
 * @brief Callback function indicating that a device has been removed.
 * @param impl The context data passed in when the service was created.
 * @param devname The name of the removed device.
 * @param protocols The protocol properties that comprise the device's address.
 */
static void custom_device_remove_device(void *impl, const char *devname, const devsdk_protocols *protocols)
{
  device_driver_remove_device(impl, devname, protocols);
}

/**
 * @brief Callback function requesting that automatic events should begin. These should be generated according to the schedule given,
 *        and posted using devsdk_post_readings().
 * @param impl The context data passed in when the service was created.
 * @param devname The name of the device to be queried.
 * @param protocols The location of the device to be queried.
 * @param resource_name The resource on which autoevents have been configured.
 * @param nreadings The number of readings requested.
 * @param requests An array specifying the readings that have been requested.
 * @param interval The time between events, in milliseconds.
 * @param onChange If true, events should only be generated if one or more readings have changed.
 * @return A pointer to a data structure that will be provided in a subsequent call to the stop handler.
 */
static void * custom_device_autoevent_start_handler
(
  void *impl,
  const char *devname,
  const devsdk_protocols *protocols,
  const char *resource_name,
  uint32_t nreadings,
  const devsdk_commandrequest *requests,
  uint64_t interval,
  bool onChange
)
{
  void *ret = device_driver_autoevent_start_handler(impl, devname, protocols, resource_name,
                                            nreadings, requests, interval, onChange);
  
  return ret;
}

/**
 * @brief Callback function requesting that automatic events should cease.
 * @param impl The context data passed in when the service was created.
 * @param handle The data structure returned by a previous call to the start handler.
 */
static void custom_device_autoevent_stop_handler(void *impl, void *handle)
{
  device_driver_autoevent_stop_handler(impl, handle);
}
/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/
/**
 * @brief 设备服务驱动入口
 */
int main (int argc, char * argv[])
{
  sigset_t set;
  int sigret;
#if USE_DL_OPEN
  /*加载设备驱动*/
  device_driver_load();
#endif

  custom_device_driver * impl = malloc (sizeof (custom_device_driver));
  impl->lc = NULL;
  impl->service = NULL;

  devsdk_error e;
  e.code = 0;

  devsdk_callbacks custom_deviceImpls =
  {
    custom_device_init,         /* Initialize */
    custom_device_reconfigure,  /* Reconfigure */
    custom_device_discover,     /* Discovery */
    custom_device_get_handler,  /* Get */
    custom_device_put_handler,  /* Put */
    custom_device_stop,         /* Stop */
    custom_device_add_device,   /* Add dev*/
    custom_device_update_device,/* Update dev*/
    custom_device_remove_device,/* Remove dev*/
    // custom_device_autoevent_start_handler,/*Autoevent start*/
    // custom_device_autoevent_stop_handler/*Autoevent stop*/
  };

  devsdk_service_t * service = devsdk_service_new("device-custom_device", "1.0", impl, custom_deviceImpls, &argc, argv, &e);
  impl->service = service;
  impl->device_driver_reload_callback = device_driver_reload;

  ERR_CHECK (e);

  int n = 1;
  while (n < argc)
  {
    if (strcmp (argv[n], "-h") == 0 || strcmp (argv[n], "--help") == 0)
    {
      printf ("Options:\n");
      printf ("  -h, --help\t\t\tShow this text\n");
      return 0;
    }
    else
    {
      printf ("%s: Unrecognized option %s\n", argv[0], argv[n]);
      return 0;
    }
  }

  devsdk_service_start (service, NULL, &e);
  ERR_CHECK (e);

  sigemptyset (&set);
  sigaddset (&set, SIGINT);
  sigprocmask (SIG_BLOCK, &set, NULL);
  sigwait (&set, &sigret);
  sigprocmask (SIG_UNBLOCK, &set, NULL);

  devsdk_service_stop (service, true, &e);
  ERR_CHECK (e);

  devsdk_service_free (service);
  free (impl);
  return 0;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/                                                                             
