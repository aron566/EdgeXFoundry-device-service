/**                                                                             
 *  @file custom-com-circularqueue.h                                                    
 *                                                                              
 *  @date 2020年12月02日 12:43:42 星期三
 *                                                                              
 *  @author aron566                                                             
 *                                                                              
 *  @brief None.                                                                
 *                                                                              
 *  @version V1.0                                                               
 */                                                                             
#ifndef CUSTOM_COM_CIRCULARQUEUE_H                                                          
#define CUSTOM_COM_CIRCULARQUEUE_H                                                          
#ifdef __cplusplus ///<use C compiler                                           
extern "C" {                                                                   
#endif                                                                          
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */                          
#include <stddef.h> /**< need definition of NULL    */                          
#include <stdbool.h>/**< need definition of BOOL    */                        
#include <stdio.h>  /**< if need printf             */                          
#include <stdlib.h>                                                             
#include <string.h>                                                             
/** Private includes ---------------------------------------------------------*/
                                                                                
/** Private defines ----------------------------------------------------------*/
                                                                    
/** Exported typedefines -----------------------------------------------------*/
/** 数据结构体*/
typedef struct
{
	uint8_t *dataBufer;		/**< for 8bit buffer*/
	uint16_t *data16Bufer;	/**< for 16bit buffer*/
	uint32_t *data32Bufer;	/**< for 32bit buffer*/
	uint32_t size;
	uint32_t entrance;
	uint32_t exit;
}CQ_handleTypeDef;
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
#define GET_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define IS_POWER_OF_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*============================= Common ================================*/
bool CQ_isEmpty(CQ_handleTypeDef *CircularQueue);
bool CQ_isFull(CQ_handleTypeDef *CircularQueue);
void CQ_emptyData(CQ_handleTypeDef*CircularQueue);
uint32_t CQ_getLength(CQ_handleTypeDef *CircularQueue);
/*手动缩减缓冲区长度--用作：错误帧偏移-正确帧读取后剔除*/
void CQ_ManualOffsetInc(CQ_handleTypeDef *CircularQueue ,uint32_t len);

/*===========================8 Bit Option==============================*/
bool CQ_init(CQ_handleTypeDef *CircularQueue ,uint8_t *memAdd ,uint32_t len);
/*分配一个缓冲区并进行初始化--替代--CQ_init*/
CQ_handleTypeDef *cb_create(uint32_t buffsize);

uint32_t CQ_getData(CQ_handleTypeDef *CircularQueue ,uint8_t *targetBuf ,uint32_t len);
uint32_t CQ_putData(CQ_handleTypeDef *CircularQueue ,uint8_t *sourceBuf ,uint32_t len);

/*修改后的加入数据操作--数据长度作为帧头先存入缓冲区*/
uint32_t DQ_putData(CQ_handleTypeDef *CircularQueue ,uint8_t *sourceBuf ,uint32_t len);
/*修改后的取数据操作--会直接读取帧长度信息，依据当前一包长度加入缓冲区*/
uint32_t DQ_getData(CQ_handleTypeDef *CircularQueue ,uint8_t *targetBuf);

/*修改后的获取数据操作--数据读取后不会减小缓冲区长度，需手动减小,目的为了分步取出完整数据*/
uint32_t CQ_ManualGetData(CQ_handleTypeDef *CircularQueue ,uint8_t *targetBuf ,uint32_t len);
/*修改后的获取数据操作--读取指定偏移的数据，不会减小缓冲区长度,目的为了验证数据，判断帧头等*/
uint8_t CQ_ManualGet_Offset_Data(uint32_t index ,CQ_handleTypeDef *CircularQueue);

/*===========================16 Bit Option==============================*/
/*16bit环形缓冲区初始化*/
bool CQ_16_init(CQ_handleTypeDef *CircularQueue ,uint16_t *memAdd ,uint32_t len);
/*分配一个缓冲区并进行初始化--替代--CQ_16_init*/
CQ_handleTypeDef *cb_16create(uint32_t buffsize);
/*加入16bit类型数据*/
uint32_t CQ_16putData(CQ_handleTypeDef *CircularQueue ,uint16_t * sourceBuf ,uint32_t len);
/*取出16bit类型数据*/
uint32_t CQ_16getData(CQ_handleTypeDef *CircularQueue ,uint16_t *targetBuf ,uint32_t len);

/*===========================32 Bit Option==============================*/
/*32bit环形缓冲区初始化*/
bool CQ_32_init(CQ_handleTypeDef *CircularQueue, uint32_t *memAdd, uint32_t len);
/*分配一个缓冲区并进行初始化--替代--CQ_32_init*/
CQ_handleTypeDef *cb_32create(uint32_t buffsize);
/*加入32bit类型数据*/
uint32_t CQ_32putData(CQ_handleTypeDef *CircularQueue ,uint32_t * sourceBuf ,uint32_t len);
/*取出32bit类型数据*/
uint32_t CQ_32getData(CQ_handleTypeDef *CircularQueue ,uint32_t *targetBuf ,uint32_t len);
                                                                        
#ifdef __cplusplus ///<end extern c                                             
}                                                                               
#endif                                                                          
#endif                                                                          
/******************************** End of file *********************************/
                                                                                