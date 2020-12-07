/**                                                                             
 *  @file custom-common.c                                                   
 *                                                                              
 *  @date 2020年11月09日 10:40:26 星期一
 *
 *  @author aron566
 *
 *  @copyright None
 *
 *  @brief None.
 *
 *  @details Common custom components.
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <errno.h>
/* Private includes ----------------------------------------------------------*/
#include "custom-common.h"
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
 * @brief   xxx
 * @param   [in]xxx
 * @return  xxx
 * @author  aron566
 * @version V1.0
 * @date    2020-11-15
 ******************************************************************
 */

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/  
/**
 ******************************************************************
 * @brief   过滤字符串中指定字符
 * @param   [in]ch 过滤字符
 * @param   [in]str 原字符串
 * @param   [in]out_str 过滤后的字符串
 * @param   [in]size 存储区大小
 * @return  字符串长度，-1失败
 * @author  aron566
 * @version V1.0
 * @date    2020-11-24
 ******************************************************************
 */
int common_filter_special_char(char ch, const char *str, char *out_str, int size)
{
    if(str == NULL || out_str == NULL || size <= 1)
    {
        return -1;
    }
    int len = 0;
    for(int index = 0; str[index] != '\0'; index++)
    {
        if(str[index] != ch)
        {
            out_str[len] = str[index];
            ++len;
            if(len >= size)
            {
                out_str[len-1] = '\0';
                break;
            }
        }
    }
    return len;
}

/**
  ******************************************************************
  * @brief   解析浮点数数据
  * @param   [in]data 完整报文，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  float
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
float common_get_float_data(uint8_t *data ,int start_index)
{
    static float result_float = 0.0;
    if(data == NULL)
    {
        return result_float;
    }
    uint32_t result_temp = data[start_index];
    result_temp |= (((uint32_t)data[start_index+1])<<8);
    result_temp |= (((uint32_t)data[start_index+2])<<16);
    result_temp |= (((uint32_t)data[start_index+3])<<24);
    result_float = *(float*)&result_temp;
    return result_float;
}

/**
  ******************************************************************
  * @brief   解析整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint16_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint16_t common_get_u16_data(uint8_t *data ,int start_index)
{
    static uint16_t result_u16 = 0;
    if(data == NULL)
    {
        return result_u16;
    }
    result_u16 = data[start_index];
    result_u16 |= (((uint16_t)data[start_index+1])<<8);
    return result_u16;
}

/**
  ******************************************************************
  * @brief   解析整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint32_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint32_t common_get_u32_data(uint8_t *data ,int start_index)
{
    static uint32_t result_u32 = 0;
    if(data == NULL)
    {
        return result_u32;
    }
    result_u32 = data[start_index];
    result_u32 |= (((uint32_t)data[start_index+1])<<8);
    result_u32 |= (((uint32_t)data[start_index+2])<<16);
    result_u32 |= (((uint32_t)data[start_index+3])<<24);
    return result_u32;
}

/**
  ******************************************************************
  * @brief   解析32位整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint32_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint32_t common_get_modbus_u32_data(uint8_t *data ,int start_index)
{
    static uint32_t result_u32 = 0;
    if(data == NULL)
    {
        return result_u32;
    }
    result_u32 = (((uint32_t)data[start_index])<<24);
    result_u32 |= (((uint32_t)data[start_index+1])<<16);
    result_u32 |= (((uint32_t)data[start_index+2])<<8);
    result_u32 |= (((uint32_t)data[start_index+3]));
    return result_u32;
}

/**
  ******************************************************************
  * @brief   解析16位整形数据
  * @param   [in]data 完整报文 ，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  uint16_t
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
uint16_t common_get_modbus_u16_data(uint8_t *data ,int start_index)
{
    static uint16_t result_u16 = 0;
    if(data == NULL)
    {
        return result_u16;
    }
    result_u16 = (((uint16_t)data[start_index])<<8);
    result_u16 |= (((uint16_t)data[start_index+1]));
    return result_u16;
}

/**
  ******************************************************************
  * @brief   解析浮点数数据
  * @param   [in]data 完整报文，当为NULL时直接返回，非NULL时解析完成后返回
  * @return  float
  * @author  aron566
  * @version V1.0
  * @date    2020-12-03
  ******************************************************************
  */
float common_get_modbus_float_data(uint8_t *data ,int start_index)
{
    static float result_float = 0.0;
    if(data == NULL)
    {
        return result_float;
    }
    uint32_t result_temp = (((uint32_t)data[start_index])<<24);
    result_temp |= (((uint32_t)data[start_index+1])<<16);
    result_temp |= (((uint32_t)data[start_index+2])<<8);
    result_temp |= (((uint32_t)data[start_index+3]));
    result_float = *(float*)&result_temp;
    return result_float;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                