/**                                                                             
 *  @file device_driver_port.c                                                   
 *                                                                              
 *  @date 2020年11月25日 15:50:10 星期三
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 设备驱动接口.
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <unistd.h>
/* Private includes ----------------------------------------------------------*/
#include "device_driver_port.h"
/** Private typedef ----------------------------------------------------------*/
/*设备通讯fd*/
typedef struct
{
    int fd;
    PROTOCOL_Type_t protocol_type;
}DEVICE_COM_FD_Typedef_t;
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
static DEVICE_COM_FD_Typedef_t device_com_fd[PROTO_MAX] = {0};/**< FD集合*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                                
/** Private function prototypes ----------------------------------------------*/
static void modbus_rtu_com_init(void);
static void mqtt_com_init(void);                                                                                
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
  * @brief   modbus-rtu通讯接口初始化
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
static void modbus_rtu_com_init(void)
{

}     

/**
  ******************************************************************
  * @brief   mqtt通讯接口初始化
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
static void mqtt_com_init(void)
{

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
  * @brief   获得指定通讯口fd
  * @param   [in]protocol_type 通讯口.
  * @retval  -1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
int device_driver_com_get_fd(PROTOCOL_Type_t protocol_type)
{
    if(protocol_type >= UNKNOW_PROTO)
    {
        return -1;
    }
    return device_com_fd[protocol_type].fd;
}

/**
  ******************************************************************
  * @brief   向指定通讯口发送数据
  * @param   [in]data 数据.
  * @param   [in]protocol_type 通讯口.
  * @retval  -1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
int device_driver_send_data_port(uint8_t *data, PROTOCOL_Type_t protocol_type)
{
    return 0;
}

/**
  ******************************************************************
  * @brief   关闭指定通讯口
  * @param   [in]Nprotocol_type 通讯口.
  * @retval  -1打开失败，0打开成功.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
int device_driver_com_close(PROTOCOL_Type_t protocol_type)
{
    if(protocol_type >= UNKNOW_PROTO)
    {
        return -1;
    }
    return close(device_com_fd[protocol_type].fd);
}

/**
  ******************************************************************
  * @brief   通讯接口初始化
  * @param   [in]None.
  * @retval  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
void device_driver_com_init_port(void)
{
    modbus_rtu_com_init();

    mqtt_com_init();
}  

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                