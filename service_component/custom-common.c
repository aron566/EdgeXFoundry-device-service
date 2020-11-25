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
 * @brief   过滤字符串中指定字符
 * @param   [in]ch 过滤字符
 * @param   [in]str 原字符串
 * @param   [in]out_str 过滤后的字符串
 * @param   [in]size 存储区大小
 * @retval  字符串长度，-1失败
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
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                