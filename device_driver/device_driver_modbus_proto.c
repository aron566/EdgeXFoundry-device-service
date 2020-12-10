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
#include "device_driver_port.h"
/** Private typedef ----------------------------------------------------------*/
                                                                               
/** Private macros -----------------------------------------------------------*/
#define MODBUS_FRAME_LEN_MIN  7U   /**< modbus  报文最小长度*/
#define MODBUS_READ_COMMAND   0x03U/**< modbus  读命令*/
#define MODBUS_WRITE_COMMAND  0x10U/**< modbus  读命令*/
#define GET_READ_RESPONSE_DATA_LEN(reg_n) ((reg_n*2U+5U))/**< modbus 写命令回复数据长度*/
#define GET_WRITE_RESPONSE_DATA_LEN 8U  /**< modbus 写命令回复数据长度*/
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static uv_mutex_t modbus_mutex;
static CQ_handleTypeDef *modbus_cb = NULL;                                                                            
/** Private function prototypes ----------------------------------------------*/
static int master_read_frame_joint(uint8_t *buf, uint8_t addr, 
                                uint16_t reg_s, uint16_t reg_n);    
static int master_write_frame_joint(uint8_t *buf, uint8_t addr,
                                      uint16_t reg_s, uint16_t reg_n, uint16_t *reg_d);
static MODBUS_PARSE_CODE_Typedef_t modbus_read_response_data(uint8_t addr, uint16_t reg_n,
                                          uint16_t **reg_value, uint8_t *data, uint32_t len);
static MODBUS_PARSE_CODE_Typedef_t modbus_write_response_data(uint8_t addr, uint16_t reg_s, uint16_t reg_n,
                                                            uint8_t *data, uint32_t len);
static MODBUS_PARSE_CODE_Typedef_t modbus_wait_new_data(uint8_t addr, uint8_t cmd, uint16_t reg_s,
                                                        uint16_t reg_n, uint16_t **reg_value);
static void modbus_master_read_port(int epoll_fd, int read_fd);

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
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */                                                                              
inline static int master_read_frame_joint(uint8_t *buf, uint8_t addr, 
                                        uint16_t reg_s, uint16_t reg_n)
{
    if(buf == NULL)
    {
        return 0;
    }
    buf[0] = addr;
    buf[1] = MODBUS_READ_COMMAND;
    buf[2] = GET_U16_HI_BYTE(reg_s);
    buf[3] = GET_U16_LOW_BYTE(reg_s);
    buf[4] = GET_U16_HI_BYTE(reg_n);
    buf[5] = GET_U16_LOW_BYTE(reg_n);
    uint16_t crc = modbus_crc_return(buf, 6);
    buf[6] = GET_U16_LOW_BYTE(crc);
    buf[7] = GET_U16_HI_BYTE(crc);
    return 8;
}                                                                      

/**
  ******************************************************************
  * @brief   组合主站写数据报文结构
  * @param   [in]buf 数据存储区
  * @param   [in]addr 设备地址
  * @param   [in]reg_s 写入寄存器起始地址
  * @param   [in]reg_n 写入存器数量
  * @param   [in]reg_d 写入寄存器起始数据地址
  * @return  报文长度.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-02
  ******************************************************************
  */                                                                              
inline static int master_write_frame_joint(uint8_t *buf, uint8_t addr,
                                      uint16_t reg_s, uint16_t reg_n, uint16_t *reg_d)
{
    if(buf == NULL || reg_d == NULL)
    {
        return 0;
    }
    buf[0] = addr;
    buf[1] = MODBUS_WRITE_COMMAND;
    buf[2] = GET_U16_HI_BYTE(reg_s);
    buf[3] = GET_U16_LOW_BYTE(reg_s);
    buf[4] = GET_U16_HI_BYTE(reg_n);
    buf[5] = GET_U16_LOW_BYTE(reg_n);
    buf[6] = (uint8_t)(reg_n*2);
    
    for(uint16_t index = 0; index < reg_n; index++)
    {
      buf[7+index*2] = GET_U16_HI_BYTE(reg_d[index]);
      buf[7+index*2+1] = GET_U16_LOW_BYTE(reg_d[index]);
    }
    uint16_t crc = modbus_crc_return(buf, 7+reg_n*2);
    buf[7+reg_n*2] = GET_U16_LOW_BYTE(crc);
    buf[7+reg_n*2+1] = GET_U16_HI_BYTE(crc);
    return 7+reg_n*2+1+1;
}                                                                      

/**
  ******************************************************************
  * @brief   读命令响应数据解析
  * @param   [in]addr 地址
  * @param   [in]reg_n 寄存器数量
  * @param   [out]reg_value 寄存器数值 使用后释放
  * @param   [in]data 响应数据
  * @param   [in]len 响应数据长度
  * @return  解析状态码.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static MODBUS_PARSE_CODE_Typedef_t modbus_read_response_data(uint8_t addr, uint16_t reg_n,
                                          uint16_t **reg_value, uint8_t *data, uint32_t len)
{
  if(data == NULL || len < MODBUS_FRAME_LEN_MIN)
  {
    return MODBUS_FRAME_MISS;
  }
  /*判断地址与命令*/
  if(addr != data[0] || data[1] != MODBUS_READ_COMMAND)
  {
    return MODBUS_CRC_ERR;
  }
  /*寄存器数量*/
  if((uint8_t)reg_n != (data[2]/2))
  {
    return MODBUS_FRAME_ERR;
  }
  /*判断数据长度*/
  if(len < GET_READ_RESPONSE_DATA_LEN(reg_n))
  {
    return MODBUS_FRAME_MISS;
  }
  /*校验CRC*/
  if(return_check_crc(data, len))
  {
    /*解析数据内容*/
    uint16_t *value_buf = NULL;
    value_buf = (uint16_t *)calloc(reg_n, sizeof(uint16_t));
    if(value_buf == NULL)
    {
      printf("can't malloc modbus rec memory.\n");
      exit(-1); 
    }
    for(uint16_t index = 0; index < reg_n; index++)
    {
      value_buf[index] = common_get_modbus_u16_data(data ,3+index*2);
    }
    *reg_value = value_buf;
    return MODBUS_OK;
  }
  else
  {
    return MODBUS_CRC_ERR;
  }
}

/**
  ******************************************************************
  * @brief   写命令响应数据解析
  * @param   [in]addr 地址
  * @param   [in]reg_s 寄存器起始地址
  * @param   [in]reg_n 寄存器数量
  * @param   [in]data 响应数据
  * @param   [in]len 响应数据长度
  * @return  解析状态码.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static MODBUS_PARSE_CODE_Typedef_t modbus_write_response_data(uint8_t addr, uint16_t reg_s, uint16_t reg_n,
                                                            uint8_t *data, uint32_t len)
{
  if(data == NULL || len < MODBUS_FRAME_LEN_MIN)
  {
    return MODBUS_FRAME_MISS;
  }
  /*判断地址与命令*/
  if(addr != data[0] || data[1] != MODBUS_WRITE_COMMAND)
  {
    return MODBUS_FRAME_ERR;
  }
  /*寄存器地址*/
  uint16_t reg_addr = ((uint16_t)(data[2]<<8))|data[3];
  if(reg_s != reg_addr)
  {
    return MODBUS_FRAME_ERR;
  }
  /*判断数据长度*/
  if(len < GET_WRITE_RESPONSE_DATA_LEN)
  {
    return MODBUS_FRAME_MISS;
  }
  /*校验CRC*/
  if(return_check_crc(data, len))
  {
    return MODBUS_OK;
  }
  else
  {
    return MODBUS_CRC_ERR;
  }
}

/**
  ******************************************************************
  * @brief   modbus新数据接收处理
  * @param   [in]epoll_fd
  * @param   [in]read_fd
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
static void modbus_master_read_port(int epoll_fd, int read_fd) 
{
  UNUSED(epoll_fd);
  static uint8_t modbus_buf[256];
  PORT_BUF_STRUCT_Typedef_t buf;
  buf.base_buf.buf = modbus_buf;
  buf.base_buf.buf_len = 256;

  /*发送读取命令*/
  ssize_t len = device_driver_read_data_port(&buf, MODBUS_RTU_PROTO);
  if(len > 0)
  {
    /*存入缓冲区*/
    if(modbus_cb == NULL)
    {
      printf("the circular buf is null.\n");
      return;
    }
    CQ_putData(modbus_cb ,modbus_buf ,(uint32_t)len);
  }

}

/**
  ******************************************************************
  * @brief   解析modbus数据
  * @param   [in]addr 设备地址
  * @param   [in]cmd 命令
  * @param   [in]reg_s 读取寄存器起始地址
  * @param   [in]reg_n 读取寄存器数量
  * @param   [out]reg_value 寄存器数值存储区 为NULL时解析写命令响应数据
  * @return  解析状态码
  * @author  aron566
  * @version V1.0
  * @date    2020-12-05
  ******************************************************************
  */
static MODBUS_PARSE_CODE_Typedef_t modbus_wait_new_data(uint8_t addr, uint8_t cmd, uint16_t reg_s,
                                                        uint16_t reg_n, uint16_t **reg_value)
{
  uint64_t last_time = get_current_time_s(CURRENT_TIME_MS);
  uint64_t current_time = 0;
  MODBUS_PARSE_CODE_Typedef_t state = MODBUS_OK;
  uint8_t temp_buf[256];
  for(;;)
  {
    current_time = get_current_time_s(CURRENT_TIME_MS);

    /*判断缓冲区是否有新数据*/
    uint32_t buf_len = CQ_getLength(modbus_cb);
    buf_len = buf_len > 256?256:buf_len;
    if(buf_len > MODBUS_FRAME_LEN_MIN)
    {
      /*获取数据*/
      CQ_ManualGetData(modbus_cb, temp_buf, buf_len);

      /*解析响应数据*/
      if(cmd == MODBUS_READ_COMMAND)
      {
        state = modbus_read_response_data(addr, reg_n, reg_value, temp_buf, buf_len);
      }
      else if(cmd == MODBUS_WRITE_COMMAND)
      {
        state = modbus_write_response_data(addr, reg_s, reg_n, temp_buf, buf_len);
      }
      
      switch(state)
      {
        case MODBUS_FRAME_MISS:
          break;
        case MODBUS_FRAME_ERR:
        case MODBUS_CRC_ERR:
          CQ_skipInvaildU8Header(modbus_cb, addr);
          break;
        case MODBUS_OK:
          CQ_ManualOffsetInc(modbus_cb ,buf_len);
          return MODBUS_OK;
        default:
          break;
      }
    }
    /*判断超时*/
    if((current_time - last_time) > 350)
    {
      return MODBUS_TIMEOUT;
    }
  }

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
  * @brief   modbus主站读
  * @param   [in]addr 设备地址
  * @param   [in]reg_s 读取寄存器起始地址
  * @param   [in]reg_n 读取寄存器数量
  * @param   [out]reg_value 寄存器数值存储区
  * @return  state 解析状态码.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-01
  ******************************************************************
  */
MODBUS_PARSE_CODE_Typedef_t device_driver_modbus_master_read(uint8_t addr, uint16_t reg_s, uint16_t reg_n,
                                      uint16_t **reg_value)
{
  uint8_t read_cmd[8];
  MODBUS_PARSE_CODE_Typedef_t state = MODBUS_OK;
  PORT_BUF_STRUCT_Typedef_t data;
  data.base_buf.buf = read_cmd;
  data.base_buf.buf_len = 8;

  /*lock*/
  uv_mutex_lock(&modbus_mutex);

  /*组合读取数据报文*/
  master_read_frame_joint(read_cmd, addr, reg_s, reg_n);

  /*发送读取命令*/
  device_driver_send_data_port(&data, MODBUS_RTU_PROTO);

  /*判断缓冲区是否有新数据*/
  state = modbus_wait_new_data(addr, MODBUS_READ_COMMAND, reg_s, reg_n, reg_value);

  /*unlock*/
  uv_mutex_unlock(&modbus_mutex);
  return state;
}

/**
  ******************************************************************
  * @brief   modbus主站写
  * @param   [in]addr 设备地址
  * @param   [in]reg_s 读取寄存器起始地址
  * @param   [in]reg_n 读取寄存器数量
  * @param   [in]reg_d 寄存器数值
  * @return  state 解析状态码.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-05
  ******************************************************************
  */
MODBUS_PARSE_CODE_Typedef_t device_driver_modbus_master_write(uint8_t addr, uint16_t reg_s, 
                                                          uint16_t reg_n, uint16_t *reg_d)
{
  uint8_t write_cmd[256];
  MODBUS_PARSE_CODE_Typedef_t state = MODBUS_OK;
  PORT_BUF_STRUCT_Typedef_t data;
  data.base_buf.buf = write_cmd;

  /*lock*/
  uv_mutex_lock(&modbus_mutex);

  /*组合写数据报文*/
  int len = master_write_frame_joint(write_cmd, addr, reg_s, reg_n, reg_d);
  data.base_buf.buf_len = len;

  /*发送写命令*/
  device_driver_send_data_port(&data, MODBUS_RTU_PROTO);

  /*判断缓冲区是否有新数据*/
  state = modbus_wait_new_data(addr, MODBUS_WRITE_COMMAND, reg_s, reg_n, NULL);

  /*unlock*/
  uv_mutex_unlock(&modbus_mutex);
  return state;
}

/**
  ******************************************************************
  * @brief   modbus主站初始化
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-12-05
  ******************************************************************
  */
void device_driver_modbus_master_stack_init(void)
{
  /*建立modbus锁*/
  uv_mutex_init(&modbus_mutex);

  /*建立缓冲区*/
  modbus_cb = cb_create(CQ_BUF_512B);

  /*注册modbus fd*/
  FD_FUNC_MAP_Typedef_t node;
  node.node_fd = device_driver_com_get_fd(MODBUS_RTU_PROTO);
  node.fd_type = SERIAL_ID_2;
  node.slot_read = modbus_master_read_port;
  node.slot_write = NULL;
  epoll_listen_register(SERIAL_FD ,&node);
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                