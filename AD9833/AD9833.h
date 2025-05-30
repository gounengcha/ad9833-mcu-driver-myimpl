/**
 * @file AD9833.h
 * @author Gounengcha 
 * @brief 
 * @version 0.1
 * @date 2025-05-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __AD9833_H__
#define __AD9833_H__
#include "stdint.h"

typedef struct
{
    void    (*AD9833_WriteHalfWord)(uint16_t data); // SPI底层接口，写入16位数据函数，需要在最开始手动赋值(//SPI low-level function to write 16-bit data. Must be manually assigned first.)

    void 	(*Init)(void);                          //若上面的函数指针已经赋值，那么请调用调用该指针指向的初始化函数，包括复位等操作(//Call this initialization function (includes reset) after assigning all function pointers)
    void    (*SetSinOuput)(float freq);             //开启正弦输出，并设置频率（电压较低，比如0.6Vpp）(// Enable sine wave output at specified frequency (low amplitude, e.g. 0.6 Vpp))
    void    (*SetSquareOuput)(float freq);          //开启三角波输出，并设置频率（电压较低，比如0.6Vpp）(// Enable triangle wave output at specified frequency (low amplitude, e.g. 0.6 Vpp))
    void    (*SetTriangleOuput)(float freq);        //开启方波输出，并设置频率（电压较高，具体值与供电电压相关，试验中5V供电时的实测值高于3.6V）(// Enable square wave output at specified frequency (high amplitude, supply-dependent; >3.6V at 5V supply))
    void    (*Off)(void);                           //关闭波形输出( // Disable waveform output)
} AD9833_API_Struct;

extern AD9833_API_Struct AD9833_API;

// Usage example:
// for stm32 hal driver
#ifdef ReadExample // Don't define ReadExample ​​during compilation​​.
//in spi.c:  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;

//stm32 spi send func
void __AD9833_WriteHalfWord(uint16_t data)
{   
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 1, 1000);
}

// in main()
  AD9833_API.AD9833_WriteHalfWord = __AD9833_WriteHalfWord;
  AD9833_API.Init();
  AD9833_API.SetSinOuput(100000.f);
#endif


#endif
