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
uint8_t *base64_encode(const char *str)
{
    long len = 0;
    long str_len = 0;
    uint8_t *res = NULL;
    int i,j;
    /*定义base64编码表*/
    const char *base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
    /*计算经过base64编码后的字符串长度*/
    str_len = strlen(str);
    if(str_len % 3 == 0)
    {
        len = str_len/ 3*4;
    }
    else
    {
        len = (str_len/3+1)*4;
    }
 
    res = malloc(sizeof(uint8_t)*len+1);
    if(res == NULL)
    {
        return NULL;
    }
    res[len] = '\0';
 
    /*以3个8位字符为一组进行编码*/
    for(i = 0, j = 0; i < len - 2; j += 3, i += 4)
    {
        /*取出第一个字符的前6位并找出对应的结果字符*/
        res[i] = base64_table[str[j]>>2];
        /*将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符*/
        res[i+1] = base64_table[(str[j]&0x3)<<4 | (str[j+1]>>4)];
        /*将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符*/
        res[i+2] = base64_table[(str[j+1]&0xf)<<2 | (str[j+2]>>6)];
        /*取出第三个字符的后6位并找出结果字符*/
        res[i+3] = base64_table[str[j+2]&0x3f];
    }
 
    switch(str_len % 3)
    {
        case 1:
            res[i-2]='=';
            res[i-1]='=';
            break;
        case 2:
            res[i-1]='=';
            break;
    }
    return res;
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
uint8_t *base64_decode(const char *code, uint16_t *size)
{
    /*根据base64表，以字符找到对应的十进制数据*/
    int table[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
    long len = 0;
    long str_len = 0;
    uint8_t *res = NULL;
    int i, j;
 
    /*计算解码后的字符串长度*/
    len = strlen(code);
    /*判断编码后的字符串后是否有=*/
    if(strstr(code, "=="))
        str_len = len/4*3-2;
    else if(strstr(code, "="))
        str_len = len / 4*3-1;
    else
        str_len = len / 4;
 
    res = malloc(sizeof(uint8_t)*str_len+1);
    if(res == NULL)
    {
        return NULL;
    }
    res[str_len] = '\0';
 
    /*以4个字符为一位进行解码*/
    for(i = 0, j = 0; i < len - 2; j += 3, i += 4)
    {
        /*取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合*/
        res[j] = ((uint8_t)table[(uint8_t)code[i]])<<2 | (((uint8_t)table[(uint8_t)code[i+1]])>>4);
        /*取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合*/
        res[j+1] = (((uint8_t)table[(uint8_t)code[i+1]])<<4) | (((uint8_t)table[(uint8_t)code[i+2]])>>2);
        /*取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合*/
        res[j+2] = (((uint8_t)table[(uint8_t)code[i+2]])<<6) | ((uint8_t)table[(uint8_t)code[i+3]]);
    }
    return res;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                