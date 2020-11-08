/**                                                                            
 *  @file custom-device.c                                                   
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
/* Private includes ----------------------------------------------------------*/
#include "devsdk/devsdk.h"
#include "custom-device.h"
/** Private typedef ----------------------------------------------------------*/
typedef struct custom_device_driver
{
  iot_logger_t * lc;
} custom_device_driver;
/** Private macros -----------------------------------------------------------*/
#define ERR_CHECK(x) if(x.code){fprintf(stderr, "Error: %d: %s\n", x.code, x.reason); devsdk_service_free (service); free (impl); return x.code;}
#define ERR_BUFSZ 1024
#define ERR_CUSTOM_DEVICE_WRITE "PUT called for custom_device device. This is a read-only device."
#define ERR_CUSTOM_DEVICE_NO_PARAM "No parameter attribute in GET request."

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/

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
/** Private user code --------------------------------------------------------*/                                                                         
/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/
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
  custom_device_driver * driver = (custom_device_driver *) impl;
  char * buff;

  for (uint32_t i = 0; i < nreadings; i++)
  {
    /*获取GET参数*/
    const char * param = devsdk_nvpairs_value (requests[i].attributes, "parameter");
    if (param == NULL)
    {
      iot_log_error (driver->lc, ERR_CUSTOM_DEVICE_NO_PARAM);
      * exception = iot_data_alloc_string (ERR_CUSTOM_DEVICE_NO_PARAM, IOT_DATA_REF);
      return false;
    }

    /*参数来自.yaml中deviceResources列表中attributes项parameter键值*/

    if (strcmp (param, "xrot") == 0)
    {
    //   readings[i].value = iot_data_alloc_i32 ((random () % 501) - 250);
    // }
    // else if (strcmp (param, "yrot") == 0)
    // {
    //   readings[i].value = iot_data_alloc_i32 ((random () % 501) - 250);
    // }
    // else if (strcmp (param, "zrot") == 0)
    // {
    //   readings[i].value = iot_data_alloc_i32 ((random () % 501) - 250);
      
    }
    else
    {
      buff = malloc (ERR_BUFSZ);
      snprintf (buff, ERR_BUFSZ, "Unknown parameter %s requested", param);
      iot_log_error (driver->lc, buff);
      * exception = iot_data_alloc_string (buff, IOT_DATA_TAKE);
      return false;
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
  custom_device_driver * driver = (custom_device_driver *) impl;
  iot_log_error (driver->lc, ERR_CUSTOM_DEVICE_WRITE);
  * exception = iot_data_alloc_string (ERR_CUSTOM_DEVICE_WRITE, IOT_DATA_REF);
  return false;
}

/**
 * @brief Function called when configuration is updated.
 * @param impl The context data passed in when the service was created.
 * @param config A string map containing the new configuration.
 */
static void custom_device_reconfigure(void *impl, const iot_data_t *config)
{

}

/**
 * @brief Optional callback for dynamic discovery of devices. The implementation should detect devices and register them using
 *        the devsdk_add_device API call.
 * @param impl The context data passed in when the service was created.
 */
static void custom_device_discover(void *impl) 
{

}

/**
 * @brief Callback issued during device service shutdown. The implementation should stop processing and release any resources that were being used.
 * @param impl The context data passed in when the service was created.
 * @param force A 'force' stop has been requested. An unclean shutdown may be performed if necessary.
 */
static void custom_device_stop(void * impl, bool force)
{
  /* Stop performs any final actions before the device service is terminated */
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
  
  custom_device_driver * impl = malloc (sizeof (custom_device_driver));
  impl->lc = NULL;

  devsdk_error e;
  e.code = 0;

  devsdk_callbacks custom_deviceImpls =
  {
    custom_device_init,         /* Initialize */
    custom_device_reconfigure,  /* Reconfigure */
    custom_device_discover,     /* Discovery */
    custom_device_get_handler,  /* Get */
    custom_device_put_handler,  /* Put */
    custom_device_stop          /* Stop */
  };

  devsdk_service_t * service = devsdk_service_new("device-custom_device", "1.0", impl, custom_deviceImpls, &argc, argv, &e);
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
