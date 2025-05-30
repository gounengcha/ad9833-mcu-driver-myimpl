/**
 * @file AD9833.c
 * @author Gounengcha 
 * @brief 
 * @version 0.1
 * @date 2025-05-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "AD9833.h"
#include <stdint.h>

//不包含粗调代码（粗调：只改变FREQ的MSB，精调：改变FREQ的MSB和LSB），
// 也不包括相位代码，channel只选择了channel 0

#define AD9833_WriteCtlReg AD9833_API.AD9833_WriteHalfWord //写控制寄存器

//写频率寄存器
//data:频率寄存器值
//channel:频率寄存器选择，0或1
void AD9833_WriteFreqReg(uint32_t data, uint8_t channel)    
{
    AD9833_API.AD9833_WriteHalfWord((data & 0x3FFF) | (1<<(channel + 14)));
    AD9833_API.AD9833_WriteHalfWord(((data & 0xFFFC000) >> 14) | (1<<(channel + 14)));    
} 

//写相位寄存器
//data:相位寄存器值
//channel:相位寄存器选择，0或1
void AD9833_WritePhaseReg(uint16_t data, uint8_t channel)
{
    AD9833_API.AD9833_WriteHalfWord((data & 0xFFF) | 0xC000 |(channel << 13));    
}

//默认配置为50Hz左右正弦波，但是DAC关闭不输出
void AD9833_Init(void)
{
    AD9833_WriteCtlReg(0x2050); //Reset = 1，并关闭DAC
    AD9833_WriteFreqReg(537, 0); //50Hz sine wave
    AD9833_WritePhaseReg(0, 0); //0 degree phase
    AD9833_WriteCtlReg(0x2040); //Reset = 0，并关闭DAC
}

//设置正弦波频率，并输出
//freq:频率，单位Hz
void AD9833_SetSinOuput(float freq)
{
    if (freq > 12500000 || freq < 0)
        return;
    uint32_t temp = (uint32_t)((freq/25000000.f)*(float)(1<<28));
    AD9833_WriteCtlReg(0x2000); // sine wave
    AD9833_WriteFreqReg(temp, 0);
}

//设置三角波频率，并输出
//freq:频率，单位Hz
void AD9833_SetTriOuput(float freq)
{
    if (freq > 12500000 || freq < 0)
        return;
    uint32_t temp = (uint32_t)((freq/25000000.f)*(float)(1<<28));
    AD9833_WriteCtlReg(0x2002); // triangle wave
    AD9833_WriteFreqReg(temp, 0);
}

// //设置方波频率，并输出其1分频
// //freq:频率，单位Hz
// void AD9833_SetSquareOuputDiv2(float freq)
// {
//     if (freq > 12500000 || freq < 0)
//         return;
//     uint32_t temp = (uint32_t)((freq/25000000.f)*(float)(1<<28));
//     AD9833_WriteCtlReg(0x2020); // square wave
//     AD9833_WriteFreqReg(temp, 0);
// }

//设置方波频率，并输出
//freq:频率，单位Hz
void AD9833_SetSquareOuput(float freq)
{
    if (freq > 12500000 || freq < 0)
        return;
    uint32_t temp = (uint32_t)((freq/25000000.f)*(float)(1<<28));
    AD9833_WriteCtlReg(0x2028); // square wave
    AD9833_WriteFreqReg(temp, 0);
}

//关闭输出
void AD9833_TurnOff(void)
{
    AD9833_WriteCtlReg(0x00C0);
}

//API结构体实例，AD9833_WriteHalfWord是与单片机SPI发送函数的接口
AD9833_API_Struct AD9833_API = {
    .Init = AD9833_Init,
    .AD9833_WriteHalfWord = NULL,       //需要自己编写
    .SetSinOuput = AD9833_SetSinOuput,
    .SetTriangleOuput = AD9833_SetTriOuput,
    .SetSquareOuput = AD9833_SetSquareOuput,
    .Off = AD9833_TurnOff
};




