/**                                                                             
 *  @file device_driver_modbus_proto.c                                                   
 *                                                                              
 *  @date 2020年12月01日 14:08:21 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief Modbus主站功能.
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
#include "device_driver_modbus_proto.h"
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
  * @brief   组合主站读数据报文结构
  * @param   [in]buf 数据存储区
  * @param   [in]addr 设备地址
  * @param   [in]reg_s 读取寄存器起始地址
  * @param   [in]reg_n 读取寄存器数量
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */                                                                              
inline static void master_read_frame_joint(uint8_t *buf, uint8_t addr, uint16_t reg_s, uint16_t reg_n)
{
    if(buf == NULL)
    {
        return;
    }
    buf[0] = addr;
    buf[1] = 0x03;
    buf[2] = GET_U16_HI_BYTE(reg_s);
    buf[3] = GET_U16_LOW_BYTE(reg_s);
    buf[4] = GET_U16_HI_BYTE(reg_n);
    buf[5] = GET_U16_LOW_BYTE(reg_n);
    uint16_t crc = modbus_crc_return(buf, 6);
    buf[6] = GET_U16_HI_BYTE(crc);
    buf[7] = GET_U16_LOW_BYTE(crc);
}                                                                      

/**
  ******************************************************************
  * @brief   主站读
  * @param   [in]req 工作句柄
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
void modbus_master_read_port(uv_work_t *req) 
{
    if(req == NULL)
    {
        return;
    }
    /*发送读取命令*/

    /*等待应答*/

    /*返回数据*/

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
  * @brief   modebus主站读
  * @param   [in]addr 设备地址
  * @param   [in]reg_s 读取寄存器起始地址
  * @param   [in]reg_n 读取寄存器数量
  * @param   [in]req uv工作句柄
  * @param   [in]buf 返回消息存储地址
  * @param   [in]done_cb 任务完成消息返回回调
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
void device_driver_modbus_master_read(uint8_t addr, uint16_t reg_s, uint16_t reg_n, 
                                    uv_work_t *req, uint8_t *buf, uv_after_work_cb done_cb)
{
    if(req == NULL || buf == NULL || done_cb == NULL)
    {
        return;
    }
    /*调用uv线程池，处理数据发送与接收*/
    uv_queue_work(uv_default_loop(), reg, modbus_master_read_port, done_cb);
}   

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                