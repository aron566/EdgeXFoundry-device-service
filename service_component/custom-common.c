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

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/
/**
 ******************************************************************
 * @brief   数值转为iot_data格式
 * @param   [in]data 数值
 * @param   [in]type 数值类型
 * @return  iot_data_t *
 * @author  aron566
 * @version V1.0
 * @date    2020-12-09
 ******************************************************************
 */
iot_data_t *common_value2iot_data(const void *data, VALUE_Type_t type)
{
    if(data == NULL || type == VALUE_TYPE_MAX)
    {
        return NULL;
    }
    iot_data_t *value = NULL;
    switch(type)
    {
        case INT8:
            value = iot_data_alloc_i8(*(const int8_t*)data);
            break;
        case INT16:
            value = iot_data_alloc_i16(*(const int16_t*)data);
            break;
        case INT32:
            value = iot_data_alloc_i32(*(const int32_t*)data);
            break;
        case UINT8:
            value = iot_data_alloc_ui8(*(const uint8_t*)data);
            break;
        case UINT16:
            value = iot_data_alloc_ui16(*(const uint16_t*)data);
            break;
        case UINT32:
            value = iot_data_alloc_ui32(*(const uint32_t*)data);
            break;
        case UINT64:
            value = iot_data_alloc_ui64(*(const uint64_t*)data);
            break;
        case FLOAT32:
            value = iot_data_alloc_f32(*(const float*)data);
            break;
        case DOUBLE:      
            value = iot_data_alloc_f64(*(const double*)data);
            break;
        case STRING:
            value = iot_data_alloc_string((const char*)data, IOT_DATA_COPY);
            break;
        default:
            printf("\n\n\nunknow value type\n\n\n");
            // value = iot_data_alloc_ui32(*(const uint32_t*)data);
            break;
    }
    return value;
}

/**
 ******************************************************************
 * @brief   iot_data转为uint64_t格式，字符串将给出申请的地址
 * @param   [in]data 数值
 * @param   [in]type data的数值类型
 * @return  uint64 数值
 * @author  aron566
 * @version V1.0
 * @date    2020-12-09
 ******************************************************************
 */
uint64_t common_iot_data2u64(const iot_data_t *data, VALUE_Type_t type)
{
    if(data == NULL || type == VALUE_TYPE_MAX)
    {
        return 0;
    }
    uint64_t value = 0;
    switch(type)
    {
        case INT8:
            value = (uint64_t)iot_data_i8(data);
            break;
        case INT16:
            value = (uint64_t)iot_data_i16(data);
            break;
        case INT32:
            value = (uint64_t)iot_data_i32(data);
            break;
        case INT64:
            value = (uint64_t)iot_data_i64(data);
            break;
        case UINT8:
            value = (uint64_t)iot_data_ui8(data);
            break;
        case UINT16:
            value = (uint64_t)iot_data_ui16(data);
            break;
        case UINT32:
            value = (uint64_t)iot_data_ui32(data);
            break;
        case UINT64:
            value = (uint64_t)iot_data_ui64(data);
            break;
        case FLOAT32:
            value = (uint64_t)iot_data_f32(data);
            break;
        case DOUBLE:      
            value = (uint64_t)iot_data_f64(data);
            break;
        case STRING:
            /*获取字符串*/
            {
                const char *str = iot_data_string(data);
                if(str != NULL)
                {
                    size_t len = strlen(str);
                    char *str_buf = (char*)calloc(len+1, sizeof(char));
                    if(str_buf != NULL)
                    {
                        strncopy(str_buf, str, len+1);
                        value = (uint64_t)str_buf;
                    }
                }
            }
            break;
        default:
            break;
    }
    return value;
}


/**
 ******************************************************************
 * @brief   array转为base64,使用完需释放
 * @param   [in]str 原始数据
 * @return  解码后数据
 * @author  aron566
 * @version V1.0
 * @date    2020-12-14
 ******************************************************************
 */
uint8_t *base64_encode(const uint8_t *buf, uint64_t size)
{
    size_t str_len = common_b64_encodesize((size_t)size);
    uint8_t *str = (uint8_t *)calloc(str_len, sizeof(uint8_t));
    if(str == NULL)
    {
        return NULL;
    }
    bool ret = common_b64_encode(buf, (size_t)size, (char *)str, str_len);
    if(ret == false)
    {
        return NULL;
    }
    return str;
}

/**
 ******************************************************************
 * @brief   base64转为array,使用完需释放
 * @param   [in]base64数据
 * @param   [out]size 解码字节长度
 * @return  解码后数据
 * @author  aron566
 * @version V1.0
 * @date    2020-12-14
 ******************************************************************
 */
uint8_t *base64_decode(const char *base64str, uint64_t *size)
{
    size_t data_len = common_b64_maxdecodesize(base64str);
    uint8_t *binary_data = (uint8_t *)calloc(data_len, sizeof(uint8_t));
    if(binary_data == NULL)
    {
        return NULL;
    }

    bool ret = common_b64_decode(base64str, binary_data, &data_len);
    if(ret == false)
    {
        return NULL;
    }
    *size = (uint64_t)data_len;
    return binary_data;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                