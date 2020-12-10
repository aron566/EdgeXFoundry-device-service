/**                                                                             
 *  @file device_driver_auto_event.c                                                   
 *                                                                              
 *  @date 2020年11月24日 11:33:30 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 主动上报数据-事件的轮询处理.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "device_driver_auto_event.h"
#include "device_driver_parse_par.h"
#include "gateway_device.h"
/** Private typedef ----------------------------------------------------------*/
// typedef struct {
//     uv_write_t req;
//     uv_buf_t buf;
// }write_req_t;
/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
// uv_loop_t *loop;
// uv_pipe_t stdin_pipe;
// uv_pipe_t stdout_pipe;
// uv_pipe_t file_pipe;

static pthread_t auto_event_task_thread_id;/**< 自动事件任务句柄*/
static iot_logger_t *g_lc = NULL;
static devsdk_service_t *g_service = NULL;
/** Private function prototypes ----------------------------------------------*/
/*事件检测轮询*/
static void *polling_uv_event_loop(void *par);
static void polling_report_event_task(uv_idle_t* handle);
static void event_monitor_task(void);
static void dev_event_monitor(NODE_TYPE_STRUCT *p_node, DEVICE_Typedef_t dev_type);
static bool check_interval_is_arrived(INTERVAL_TIME_Typedef_t *dest_time, uint64_t last_time);

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
  * @brief   检测周期时间是否到达
  * @param   [in]dest_time 周期时间.
  * @param   [in]last_time 上次时间.
  * @return  true时间到达，false未到达.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-09
  ******************************************************************
  */
static bool check_interval_is_arrived(INTERVAL_TIME_Typedef_t *dest_time, uint64_t last_time)
{
    uint64_t ms = time_base2ms(dest_time);
    uint64_t current_time = get_current_time_s(CURRENT_TIME_MS);
    if((current_time - last_time) > ms)
    {
        return true;
    }
    return false;
}

/**
  ******************************************************************
  * @brief   设备事件监测
  * @param   [in]p_node 节点数据.
  * @param   [in]dev_type 设备类型.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-09
  ******************************************************************
  */
static void dev_event_monitor(NODE_TYPE_STRUCT *p_node, DEVICE_Typedef_t dev_type)
{
    if(p_node == NULL)
    {
        return;
    }
    DEV_DRIVER_INTERFACE_Typedef_t *dev_resource = p_node->dev_resource_par;
    char dev_name[sizeof(DEV_INFO_Typedef_t)];
    devsdk_commandresult results[1] = {0};
    VALUE_Type_t value_type = VALUE_TYPE_MAX;
    INTERVAL_TIME_Typedef_t interval_time_base;
    bool allow = false;
    int ret;

    if(dev_resource == NULL)
    {
        return;
    }
    for(size_t i = 0; dev_resource[i].par_name != NULL; i++)
    {
        if(dev_resource[i].enable_event_flag == true)
        {
            if(p_node->get_dev_value_callback == NULL)
            {
                continue;
            }

            /*监测周期查询*/
            interval_time_base.interval_time = dev_resource[i].interval_time;
            interval_time_base.unit = dev_resource[i].unit;
            allow = check_interval_is_arrived(&interval_time_base, dev_resource[i].start_time);
            if(allow == false)
            {
                continue;
            }
            dev_resource[i].start_time = get_current_time_s(CURRENT_TIME_MS);

            /*获取该参数数值*/
            ret = p_node->get_dev_value_callback(p_node->dev_name, dev_resource[i].par_name, results, &value_type);
            if(ret != 0)
            {
               iot_log_warn(g_lc, "get event value faild.");     
            }
            
            /*询问设备是否需要上报事件*/
            const REPORT_EVENT_CONFIRM_FUNC event_confirm = get_device_event_confirm_func(dev_type);
            if(event_confirm == NULL)
            {
                continue;
            }
            allow = event_confirm(p_node->dev_name, dev_resource[i].par_name, dev_resource, results);
            if(allow == false)
            {
                continue;
            }
            
            /* Trigger an event */
            devsdk_post_readings(g_service, p_node->dev_name, dev_resource[i].par_name, results);

            /* Cleanup the value. Note that as we used IOT_DATA_TAKE, the buffer allocated in get callback is free'd here */
            iot_data_free(results[0].value);
        }
    }
}

/**
  ******************************************************************
  * @brief   事件监测任务
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-09
  ******************************************************************
  */
static void event_monitor_task(void)
{
    uint32_t len = 0;
    const DEVICE_TYPE_MAP_Typedef_t *device_type_map = get_device_type_list();
    if(device_type_map == NULL)
    {
        return;
    }

    NODE_TYPE_STRUCT *p_node = NULL; 
    for(int index = 0; device_type_map[index].type_name != NULL; index++)
    {
        len = list_get_size(device_type_map[index].dev_type);
        if(len == 0)
        {
            continue;
        }
        for(uint32_t i = 0; i < len; i++)
        {
            p_node = list_find_index_node(device_type_map[index].dev_type ,i);
            if(p_node == NULL)
            {
                continue;
            }
            dev_event_monitor(p_node, device_type_map[index].dev_type);
        }
    }   
}

/**
  ******************************************************************
  * @brief   事件检测上报
  * @param   [in]handle uv句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
static void polling_report_event_task(uv_idle_t* handle)
{
    /*监测运行状态*/
    // iot_log_debug(g_lc, "uv indel task is running...");
    
    /*获取网关运行状态*/
    EdgeGatewayRUN_SATE_Typedef_t run_state = get_gateway_device_run_state();

    switch(run_state)
    {
        case NORMAL_RUNNING_STATE:
        case UPDATING_STATE:
            /*监测上报更新进度*/
            /*监测事件*/
            event_monitor_task();
            break;
        case UNKNOW_STATE:
        default:
            break;
    }
}

/**
  ******************************************************************
  * @brief   uv事件监测
  * @param   [in]par
  * @return  NULL
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
static void *polling_uv_event_loop(void *par)
{
    UNUSED(par);
    static uv_idle_t idler;

    iot_log_debug(g_lc, "start uv idel task...");

    uv_loop_t *loop = uv_default_loop();

    /*init idel*/
    uv_idle_init(loop, &idler);

    /*set task*/
    uv_idle_start(&idler, polling_report_event_task);

    /*start uv loop*/
    uv_run(loop, UV_RUN_DEFAULT);

    /* never return */
    return NULL;
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
  * @brief   任意时间单位转为ms单位数
  * @param   [in]time_base 时间数据.
  * @return  ms.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-09
  ******************************************************************
  */
uint64_t time_base2ms(INTERVAL_TIME_Typedef_t *time_base)
{
    if(time_base == NULL)
    {
        return 0;
    }
    uint64_t ms = 0;
    switch(time_base->unit)
    {
        case T_MS:                           /**< 毫秒单位*/
            ms = time_base->interval_time;
            break;
        case T_S:                            /**< 秒*/
            ms = time_base->interval_time*1000;
            break;
        case T_MIN:                          /**< 分*/
            ms = time_base->interval_time*60*1000;
            break;
        case T_H:                            /**< 时*/
            ms = time_base->interval_time*60*60*1000;
            break;                                
        case T_D:                            /**< 天*/
            ms = time_base->interval_time*24*60*60*1000;
            break;
        case T_MTH:                          /**< 月*/
            ms = time_base->interval_time*30*24*60*60*1000;
            break;
        case T_YER:                          /**< 年*/
            ms = time_base->interval_time*12*30*24*60*60*1000;
            break;
        default:
            break;
    }
    /*检测溢出*/
    if(ms < time_base->interval_time)
    {
        ms = ULONG_MAX;
    }
    return ms;
}

/**
  ******************************************************************
  * @brief   事件检测轮询启动
  * @param   [in]lc log
  * @return  返回0成功，-1失败
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
int device_driver_uv_handler_start(void *data)
{
    custom_device_driver *user_data = (custom_device_driver *)data;
    g_lc = user_data->lc;
    g_service = user_data->service;
    int res = pthread_create(&auto_event_task_thread_id ,NULL ,polling_uv_event_loop ,user_data);
    if(res != 0) 
    {
        iot_log_error(g_lc, "create auto event task thread faild.");
        exit(res);
    }
    return pthread_detach(auto_event_task_thread_id);
}
// void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
//     *buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
// }

// void free_write_req(uv_write_t *req) {
//     write_req_t *wr = (write_req_t*) req;
//     free(wr->buf.base);
//     free(wr);
// }

// void on_stdout_write(uv_write_t *req, int status) {
//     free_write_req(req);
// }

// void on_file_write(uv_write_t *req, int status) {
//     free_write_req(req);
// }

// void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb) {
//     write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
//     req->buf = uv_buf_init((char*) malloc(size), size);
//     memcpy(req->buf.base, buf.base, size);
//     uv_write((uv_write_t*) req, (uv_stream_t*)dest, &req->buf, 1, cb);
// }

// void read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
//     if (nread < 0){
//         if (nread == UV_EOF){
//             // end of file
//             uv_close((uv_handle_t *)&stdin_pipe, NULL);

//             uv_close((uv_handle_t *)&stdout_pipe, NULL);

//             uv_close((uv_handle_t *)&file_pipe, NULL);
//         }

//     } else if (nread > 0) {
//         write_data((uv_stream_t *)&stdout_pipe, nread, *buf, on_stdout_write);

//         write_data((uv_stream_t *)&file_pipe, nread, *buf, on_file_write);
//     }
//     // OK to free buffer as write_data copies it.
//     if (buf->base)
//         free(buf->base);
// }                                                               
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                