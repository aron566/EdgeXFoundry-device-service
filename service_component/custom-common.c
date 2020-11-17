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
 *  @details custom commponent component.
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
* @retval  xxx
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
* @brief   解析设备名包含的信息
* @param   [in]dev_name 设备完整名
* @param   [out]dev_info 设备解析结构信息存储区
* @retval  设备解析结构信息存储区地址
* @author  aron566
* @version V1.0
* @date    2020-11-09
******************************************************************
*/       
DEV_INFO_Typedef_t *common_parse_dev_name(const char *dev_name ,DEV_INFO_Typedef_t *dev_info)
{
    // （协议-位置信息(应包含上级网关信息+地理位置)-设备名(可重复)-地址号或者序号(同名设备不可重复)）
    /*解析设备通讯协议*/
    int ret = sscanf(dev_name, "%[^-]-%[^-]-%[^-]-%s", dev_info->protocol_str, 
                    dev_info->location_str, dev_info->dev_type_name, dev_info->dev_address);
    if(ret != 4)
    {
        printf("parse dev_name error.\r\n");
        return NULL;
    }
    else
    {
        return dev_info;
    }
}                                                  
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                