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
#include <fcntl.h>
#include <termios.h>
/* Private includes ----------------------------------------------------------*/
#include "device_driver_port.h"
/** Private typedef ----------------------------------------------------------*/
/*设备通讯fd*/
typedef struct
{
    int fd;
    PROTOCOL_Type_t protocol_type;
    void (*read)(uint8_t *buf, uint32_t *size);
    void (*write)(uint8_t *data, uint32_t len);
}DEVICE_COM_FD_Typedef_t;
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
static DEVICE_COM_FD_Typedef_t device_com_par[PROTO_MAX] = {0};/**< FD集合*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                                
/** Private function prototypes ----------------------------------------------*/
static int usart_init(const char *ttyx, int baudrate);/**< 初始化串口*/
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
  * @brief   串口设备初始化
  * @param   [in]ttyx 串口文件地址.
  * @param   [in]baudrate 波特率115200/9600.
  * @retval  文件描述符.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-30
  ******************************************************************
  */
static int usart_init(const char *ttyx, int baudrate)
{
  int file_id = -1;
  if((file_id = open(ttyx, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
  {
    perror("USART: Failed to open the usart port!\n");
    return -1;
  }

  /*配置串口*/
  struct termios options;
  tcgetattr(file_id, &options);

  /*Set To Raw Mode*/
  /*输入模式标志*/
  options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  
  /*输出模式标志*/
  options.c_oflag  &= ~OPOST;   
  /*控制模式标志*/
  /*8-bit, enable receiver, no modem control lines*/
  options.c_cflag = CS8 | CREAD | CLOCAL;
  /*通讯波特率*/
  switch(baudrate)
  {
    case 115200:
      options.c_cflag |= B115200;
      break;
    case 19200:
      options.c_cflag |= B19200;
    break;
    case 9600:
      options.c_cflag |= B9600;
      break;
    default:
      options.c_cflag |= B9600;
      break;
  }

  /*本地模式标志*/
  /*ignore partity errors, CR -> newline*/
  options.c_iflag = IGNPAR | ICRNL; 

  /*discard file information not transmitted刷清输入、输出队列*/
  tcflush(file_id, TCIFLUSH); 

  /*changes occur immmediately立即执行而不等待数据发送或者接受完成*/
  tcsetattr(file_id, TCSANOW, &options); 

  return file_id;
}

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
  device_com_par[MODBUS_RTU_PROTO].fd = usart_init("/dev/ttyS2", 9600);
  device_com_par[MODBUS_RTU_PROTO].protocol_type = MODBUS_RTU_PROTO;
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
  return device_com_par[protocol_type].fd;
}

/**
  ******************************************************************
  * @brief   向指定通讯口发送数据
  * @param   [in]data 数据.
  * @param   [in]len 数据长度
  * @param   [in]protocol_type 通讯口.
  * @retval  -1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
int device_driver_send_data_port(uint8_t *data, uint32_t len, PROTOCOL_Type_t protocol_type)
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
  return close(device_com_par[protocol_type].fd);
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
                                                                                