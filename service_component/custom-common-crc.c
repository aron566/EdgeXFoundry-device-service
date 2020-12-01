/**                                                                             
 *  @file custom-common-crc.c                                                   
 *                                                                              
 *  @date 2020年09月10日 17:36:42 星期四
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief None.
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
#include "custom-common-crc.h"
/** Private typedef ----------------------------------------------------------*/
                                                                                
/** Private macros -----------------------------------------------------------*/
                                                                                
/** Private constants --------------------------------------------------------*/
const uint8_t auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

const uint8_t auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
                                                                                
/** Private function prototypes ----------------------------------------------*/
static uint16_t crc_cal(uint16_t Data ,uint16_t GenPoly ,uint16_t CrcData);                                                                                
/** Private user code --------------------------------------------------------*/
                                                                     
/** Private application code -------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Static code                                                             
*                                                                               
********************************************************************************
*/                                                                              
/**
 * @brief CRC计算
 * 
 * @param Data 
 * @param GenPoly 
 * @param CrcData 
 * @return uint16_t 
 */
static uint16_t crc_cal(uint16_t Data ,uint16_t GenPoly ,uint16_t CrcData)
{
	uint16_t TmpI;
	Data *= 2;
	for (TmpI = 8; TmpI > 0; TmpI--) {
		Data = Data / 2;
		if ((Data ^ CrcData) & 1)
			CrcData = (CrcData / 2) ^ GenPoly;
		else
			CrcData /= 2;
	}
	return CrcData;
}
                                                                           
/** Public application code --------------------------------------------------*/
/*******************************************************************************
*                                                                               
*       Public code                                                             
*                                                                               
********************************************************************************
*/
 /**
  * @brief 查表计算CRC
  * 
  * @param puchMsg 数据
  * @param usDataLen 数据长度
  * @return uint16_t 
  */
uint16_t get_crc(uint8_t *puchMsg ,uint16_t usDataLen)
{
    uint8_t uchCRCHi = 0xFF;  /* 高CRC字节初始化 */
    uint8_t uchCRCLo = 0xFF;  /* 低CRC 字节初始化 */
    int uIndex = 0;           /* CRC循环中的索引 */
    while(usDataLen--)
    {
        uIndex      = uchCRCHi^*puchMsg++;
        uchCRCHi    = uchCRCLo^auchCRCHi[uIndex];
        uchCRCLo    = auchCRCLo[uIndex];
    }
    return (uchCRCHi<<8|uchCRCLo); 
}  

/**
 * @brief Get the crc result object
 * 
 * @param puchMsg 
 * @param usDataLen 
 * @return true 
 * @return false 
 */
bool get_crc_result(uint8_t *puchMsg ,uint16_t usDataLen)
{
    if(puchMsg == NULL)
    {
        return false;
    }
    uint16_t crc_ret = get_crc(puchMsg ,usDataLen);
    uint8_t CRC_value_L_temp = *(puchMsg + usDataLen);
	uint8_t CRC_value_H_temp = *(puchMsg + usDataLen+1);
	uint8_t CRC_value_L = (uint8_t)(crc_ret &0x00FF);//有无符号重要！
	uint8_t CRC_value_H = (uint8_t)((crc_ret>>8)&0x00FF);

	if(CRC_value_L == CRC_value_L_temp && CRC_value_H == CRC_value_H_temp)
	{
		return true;
	}
	return false;
}


/**
 * @brief modbusCRC计算
 * 
 * @param data 带入CRC计算的数据起始
 * @param data_len 带入CRC计算的数据长度
 * @return uint16_t 
 */
uint16_t modbus_crc_return(uint8_t *data ,uint16_t data_len)
{
    uint16_t temp;
    uint16_t crc_ret = 0xFFFF;
    for (temp = 0; temp < data_len; temp++)
    {
        crc_ret = crc_cal(data[temp], 0xA001, crc_ret);
    }
    return crc_ret;
}

/**
 * @brief 校验CRC
 * 
 * @param msg 带入CRC计算的数据起始
 * @param len 带入CRC计算的数据长度
 * @return uint8_t 0错误 1校验正确 
 */
uint8_t return_check_crc(uint8_t *msg ,uint16_t len)
{
	uint8_t CRC_value_L,CRC_value_H,CRC_value_L_temp,CRC_value_H_temp;
	uint16_t crc_ret = 0;
	crc_ret = modbus_crc_return(msg,len);
	CRC_value_L_temp = *(msg+len);
	CRC_value_H_temp = *(msg+len+1);
	CRC_value_L =(uint8_t)(crc_ret &0x00FF);//有无符号重要！
	CRC_value_H = (uint8_t)((crc_ret>>8)&0x00FF);
	if(CRC_value_L == CRC_value_L_temp && CRC_value_H == CRC_value_H_temp)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#ifdef __cplusplus ///<end extern c                                             
}
#endif                                                                          
/******************************** End of file *********************************/
                                                                                