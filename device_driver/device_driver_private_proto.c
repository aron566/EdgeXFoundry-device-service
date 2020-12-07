/**                                                                             
 *  @file device_driver_private_proto.c                                                   
 *                                                                              
 *  @date 2020年12月01日 12:19:39 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 私有协议功能.
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
#include "device_driver_private_proto.h"
#include "device_driver_list.h"
#include "device_driver_parse_par.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                                
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
  * @brief   主站读
  * @param   [in]req 工作句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
static void private_master_read_port(uv_work_t *req) 
{
    if(req == NULL)
    {
        return;
    }
    /*发送读取命令*/

    /*等待应答*/

    /*返回数据*/

}

/**
  ******************************************************************
  * @brief   驱动升级
  * @param   [in]interface 设备资源接口
  * @param   [in]out_data 二进制流
  * @return  0写入成功、-1失败
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
static int private_proto_update_driver(DEV_DRIVER_INTERFACE_Typedef_t *interface, const void *out_data)
{
    // iot_data_alloc_array_from_base64 (const char * value);

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
  * @brief   读
  * @param   [in]addr 设备地址
  * @param   [in]reg_s 读取寄存器起始地址
  * @param   [in]reg_n 读取寄存器数量
  * @param   [in]req uv工作句柄
  * @param   [in]buf 返回消息存储地址
  * @param   [in]done_cb 任务完成消息返回回调
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
void device_driver_private_read(uint8_t addr, uint16_t reg_s, uint16_t reg_n, 
                                    uv_work_t *req, uint8_t *buf, uv_after_work_cb done_cb)
{
    if(req == NULL || buf == NULL || done_cb == NULL)
    {
        return;
    }
    /*调用uv线程池，处理数据发送与接收*/
    uv_queue_work(uv_default_loop(), req, private_master_read_port, done_cb);
}
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                