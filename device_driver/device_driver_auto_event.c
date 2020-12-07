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
/** Private typedef ----------------------------------------------------------*/
typedef struct {
    uv_write_t req;
    uv_buf_t buf;
}write_req_t;
/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
uv_loop_t *loop;
uv_pipe_t stdin_pipe;
uv_pipe_t stdout_pipe;
uv_pipe_t file_pipe;

static pthread_t auto_event_task_thread_id;/**< 自动事件任务句柄*/
/** Private function prototypes ----------------------------------------------*/
/*事件检测轮询*/
static void *polling_uv_event_loop(void *par);
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
    printf("uv indel task is running...\n");
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
    static uv_idle_t idler;
    iot_logger_t *lc = (iot_logger_t *)par;

    iot_log_debug(lc, "start uv idel task...");

    uv_loop_t *loop = uv_default_loop();

    /*init idel*/
    uv_idle_init(loop, &idler);

    /*set task*/
    uv_idle_start(&idler, polling_report_event_task);

    uv_run(loop, UV_RUN_DEFAULT);

    /* never return */
    return NULL;
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    *buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void on_stdout_write(uv_write_t *req, int status) {
    free_write_req(req);
}

void on_file_write(uv_write_t *req, int status) {
    free_write_req(req);
}

void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb) {
    write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
    req->buf = uv_buf_init((char*) malloc(size), size);
    memcpy(req->buf.base, buf.base, size);
    uv_write((uv_write_t*) req, (uv_stream_t*)dest, &req->buf, 1, cb);
}

void read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0){
        if (nread == UV_EOF){
            // end of file
            uv_close((uv_handle_t *)&stdin_pipe, NULL);

            uv_close((uv_handle_t *)&stdout_pipe, NULL);

            uv_close((uv_handle_t *)&file_pipe, NULL);
        }

    } else if (nread > 0) {
        write_data((uv_stream_t *)&stdout_pipe, nread, *buf, on_stdout_write);

        write_data((uv_stream_t *)&file_pipe, nread, *buf, on_file_write);
    }
    // OK to free buffer as write_data copies it.
    if (buf->base)
        free(buf->base);
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
  * @brief   事件检测轮询启动
  * @param   [in]lc log
  * @return  返回0成功，-1失败
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
int device_driver_uv_handler_start(iot_logger_t *lc)
{
    int res = pthread_create(&auto_event_task_thread_id ,NULL ,polling_uv_event_loop ,lc);
    if(res != 0) 
    {
        iot_log_error(lc, "create auto event task thread faild.");
        exit(res);
    }
    return pthread_detach(auto_event_task_thread_id);
}     
                                                                      
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                