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
typedef struct DEVICE_COM_FD_Typedef
{
    const char *dev_name;
    int fd;
    PROTOCOL_Type_t protocol_type;
    ssize_t (*read_callback)(struct DEVICE_COM_FD_Typedef *, void *);
    ssize_t (*write_callback)(struct DEVICE_COM_FD_Typedef *, void *);
}DEVICE_COM_FD_Typedef_t;

/** Private macros -----------------------------------------------------------*/
                                                                                
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                                
/** Private function prototypes ----------------------------------------------*/
static int usart_init(const char *ttyx, int baudrate);/**< 初始化串口*/
static void modbus_rtu_com_init(void);
static void mqtt_com_init(void);
static int get_protocol_type_index(PROTOCOL_Type_t protocol_type);
static ssize_t tty_read_port(struct DEVICE_COM_FD_Typedef *par, void *buf);
static ssize_t tty_write_port(struct DEVICE_COM_FD_Typedef *par, void *buf);
static ssize_t udp_read_port(struct DEVICE_COM_FD_Typedef *par, void *buf);
static ssize_t udp_write_port(struct DEVICE_COM_FD_Typedef *par, void *buf);    
static ssize_t tcp_read_port(struct DEVICE_COM_FD_Typedef *par, void *buf);
static ssize_t tcp_write_port(struct DEVICE_COM_FD_Typedef *par, void *buf);   
/** Private constants --------------------------------------------------------*/
static DEVICE_COM_FD_Typedef_t device_com_par[] = 
{
  {
    .dev_name       = "/dev/ttyS2",
    .fd             = -1,
    .protocol_type  = MODBUS_RTU_PROTO,
    .read_callback  = tty_read_port,
    .write_callback = tty_write_port,
  },
  {
    .dev_name       = NULL,
    .fd             = -1,
    .protocol_type  = MQTT_PROTO,
    .read_callback  = tcp_read_port,
    .write_callback = tcp_write_port,
  },
  {
    .dev_name       = NULL,
    .fd             = -1,
    .protocol_type  = PRIVATE_PROTO,
    .read_callback  = udp_read_port,
    .write_callback = udp_write_port,
  },
  {
    .dev_name       = NULL,
    .fd             = -1,
    .protocol_type  = UNKNOW_PROTO,
    .read_callback  = NULL,
    .write_callback = NULL,
  },
  {
    .dev_name       = NULL,
    .fd             = -1,
    .protocol_type  = PROTO_MAX,
    .read_callback  = NULL,
    .write_callback = NULL,
  },
};                                          
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
  * @brief   获取协议参数所在索引号
  * @param   [in]protocol_type 协议类型
  * @return  index
  * @author  aron566
  * @version V1.0
  * @date    2020-12-05
  ******************************************************************
  */
inline static int get_protocol_type_index(PROTOCOL_Type_t protocol_type)
{
  if(protocol_type >= UNKNOW_PROTO)
  {
      return -1;
  }
  for(int index = 0; device_com_par[index].protocol_type != PROTO_MAX; index++)
  {
    if(device_com_par[index].protocol_type == protocol_type)
    {
      return index;
    }
  }
  return -1;
}

/**
  ******************************************************************
  * @brief   tty读取
  * @param   [in]par 通讯参数
  * @param   [in]buf 存储区
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static ssize_t tty_read_port(struct DEVICE_COM_FD_Typedef *par, void *buf)
{
  BASE_BUF_STRUCT_Typedef_t *base_buf = (BASE_BUF_STRUCT_Typedef_t *)buf;
  return read(par->fd, base_buf->buf, base_buf->buf_len);
}

/**
  ******************************************************************
  * @brief   tty写入
  * @param   [in]par 通讯参数
  * @param   [in]buf 数据区
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static ssize_t tty_write_port(struct DEVICE_COM_FD_Typedef *par, void *buf)
{
  BASE_BUF_STRUCT_Typedef_t *base_buf = (BASE_BUF_STRUCT_Typedef_t *)buf;
  return write(par->fd, base_buf->buf, base_buf->buf_len);
}

/**
  ******************************************************************
  * @brief   udp读取
  * @param   [in]par 通讯参数
  * @param   [in]buf 存储区
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static ssize_t udp_read_port(struct DEVICE_COM_FD_Typedef *par, void *buf)
{
  PORT_BUF_STRUCT_Typedef_t *udp_buf = (PORT_BUF_STRUCT_Typedef_t *)buf;
  socklen_t len = sizeof(struct sockaddr);
  return recvfrom(par->fd, udp_buf->base_buf.buf ,udp_buf->base_buf.buf_len ,0 ,(struct sockaddr*)&udp_buf->remote_host_addr, &len);
}

/**
  ******************************************************************
  * @brief   udp写入
  * @param   [in]par 通讯参数
  * @param   [in]buf 数据区
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static ssize_t udp_write_port(struct DEVICE_COM_FD_Typedef *par, void *buf)
{
  PORT_BUF_STRUCT_Typedef_t *udp_buf = (PORT_BUF_STRUCT_Typedef_t *)buf;
  return sendto(par->fd ,udp_buf->base_buf.buf ,udp_buf->base_buf.buf_len ,0 ,(struct sockaddr*)&udp_buf->remote_host_addr ,sizeof(struct sockaddr_in));
}

/**
  ******************************************************************
  * @brief   tcp读取
  * @param   [in]par 通讯参数
  * @param   [in]buf 存储区
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static ssize_t tcp_read_port(struct DEVICE_COM_FD_Typedef *par, void *buf)
{
  BASE_BUF_STRUCT_Typedef_t *tcp_buf = (BASE_BUF_STRUCT_Typedef_t *)buf;
  return recv(par->fd, tcp_buf->buf, tcp_buf->buf_len, MSG_WAITALL);
}

/**
  ******************************************************************
  * @brief   tcp写入
  * @param   [in]par 通讯参数
  * @param   [in]buf 数据区
  * @param   [in]size 数据大小
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static ssize_t tcp_write_port(struct DEVICE_COM_FD_Typedef *par, void *buf)
{
  BASE_BUF_STRUCT_Typedef_t *tcp_buf = (BASE_BUF_STRUCT_Typedef_t *)buf;
  return send(par->fd, tcp_buf->buf, tcp_buf->buf_len, MSG_WAITALL);
}

/**
  ******************************************************************
  * @brief   串口设备初始化
  * @param   [in]ttyx 串口文件地址.
  * @param   [in]baudrate 波特率115200/9600.
  * @return  文件描述符.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-30
  ******************************************************************
  */
static int usart_init(const char *ttyx, int baudrate)
{
  int fd = -1;
  if((fd = open(ttyx, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
  {
    perror("USART: Failed to open the usart port!\n");
    return -1;
  }

  /*配置串口*/
  struct termios options;
  tcgetattr(fd, &options);

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
  tcflush(fd, TCIFLUSH); 

  /*changes occur immmediately立即执行而不等待数据发送或者接受完成*/
  tcsetattr(fd, TCSANOW, &options); 

  return fd;
}

/**
  ******************************************************************
  * @brief   modbus-rtu通讯接口初始化
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
static void modbus_rtu_com_init(void)
{
  int index = get_protocol_type_index(MODBUS_RTU_PROTO);
  if(index == -1)
  {
    return;
  }
  device_com_par[index].fd = usart_init(device_com_par[index].dev_name, 9600);

}     

/**
  ******************************************************************
  * @brief   mqtt通讯接口初始化
  * @param   [in]None.
  * @return  None.
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
  * @return  -1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
int device_driver_com_get_fd(PROTOCOL_Type_t protocol_type)
{
  int index = get_protocol_type_index(protocol_type);
  if(index == -1)
  {
    return -1;
  }
  return device_com_par[index].fd;

}

/**
  ******************************************************************
  * @brief   向指定通讯口发送数据
  * @param   [in]data 数据.
  * @param   [in]len 数据长度
  * @param   [in]protocol_type 通讯口.
  * @return  -1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
ssize_t device_driver_send_data_port(PORT_BUF_STRUCT_Typedef_t *data, PROTOCOL_Type_t protocol_type)
{
  int index = get_protocol_type_index(protocol_type);
  if(index == -1)
  {
    return -1;
  }
  return device_com_par[index].write_callback(&device_com_par[index], data);;
}

/**
  ******************************************************************
  * @brief   向指定通讯口读取数据
  * @param   [in]buf 数据存入地址.
  * @param   [in]len 数据存入长度
  * @param   [in]protocol_type 通讯口.
  * @return  -1失败.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
ssize_t device_driver_read_data_port(PORT_BUF_STRUCT_Typedef_t *buf, PROTOCOL_Type_t protocol_type)
{
  int index = get_protocol_type_index(protocol_type);
  if(index == -1)
  {
    return -1;
  }
  return device_com_par[index].read_callback(&device_com_par[index], buf);;
}

/**
  ******************************************************************
  * @brief   关闭指定通讯口
  * @param   [in]Nprotocol_type 通讯口.
  * @return  -1打开失败，0打开成功.
  * @author  aron566
  * @version V1.0
  * @date    2020-11-25
  ******************************************************************
  */
int device_driver_com_close(PROTOCOL_Type_t protocol_type)
{
  int index = get_protocol_type_index(protocol_type);
  if(index == -1)
  {
    return -1;
  }
  return close(device_com_par[index].fd);
}

/**
  ******************************************************************
  * @brief   通讯接口初始化
  * @param   [in]None.
  * @return  None.
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
                                                                                