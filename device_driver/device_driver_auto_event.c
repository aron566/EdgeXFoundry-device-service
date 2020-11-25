/**                                                                             
 *  @file device_driver_auto_event.c                                                   
 *                                                                              
 *  @date 2020年11月24日 11:33:30 星期二
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 主动上报数据-事件的轮询处理.
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
#include "device_driver_auto_event.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/

/** Private function prototypes ----------------------------------------------*/
/*事件检测轮询*/
static void *polling_event_loop(void *par);
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
  * @brief   事件检测轮询
  * @param   [in]par
  * @retval  NULL
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
static void *polling_event_loop(void *par)
{
    for(;;)
    {

    }
    return NULL;
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
  * @brief   事件检测轮询启动
  * @param   [in]par
  * @retval  返回0成功，-1失败
  * @author  aron566
  * @version V1.0
  * @date    2020-11-24
  ******************************************************************
  */
int device_driver_event_report_handler_start(void)
{

    return 0;
}                                                                           
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
/******************************** End of file *********************************/
                                                                                