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
iot_data_t *common_value2iot_data(void *data, VALUE_Type_t type)
{
    if(data == NULL || type == VALUE_TYPE_MAX)
    {
        return NULL;
    }
    iot_data_t *value = NULL;
    switch(type)
    {
        case INT8:
            value = iot_data_alloc_i8(*(int8_t*)data);
            break;
        case INT16:
            value = iot_data_alloc_i16(*(int16_t*)data);
            break;
        case INT32:
            value = iot_data_alloc_i32(*(int32_t*)data);
            break;
        case UINT8:
            value = iot_data_alloc_ui8(*(uint8_t*)data);
            break;
        case UINT16:
            value = iot_data_alloc_ui16(*(uint16_t*)data);
            break;
        case UINT32:
            value = iot_data_alloc_ui32(*(uint32_t*)data);
            break;
        case FLOAT32:
            value = iot_data_alloc_f32(*(float*)data);
            break;
        case DOUBLE:      
            value = iot_data_alloc_f64(*(double*)data);
            break;
        case STRING:
            value = iot_data_alloc_string((const char*)data, IOT_DATA_COPY);
            break;
        default:
            break;
    }
    return value;
}

#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                