/**********************************************************************************
 * 文件名  ：SysTick.c
 * 描述    ：通过系统滴答时钟SysTick中断实现nMs(n毫秒)、nS(n秒)的延时        
 * 实验平台：NiRen_TwoHeart系统板
 * 硬件连接：无须外接电路       
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "SysTick.h"

static u32 SysTickDelayTime;

/*******************************************************************************
* 函数名  : SysTick_Init_Config
* 描述    : 初始化系统滴答时钟SysTick
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 1)、SystemFrequency / 1000		1ms中断一次
*			2)、SystemFrequency / 100000	10us中断一次
*			3)、SystemFrequency / 1000000	1us中断一次
*			计算方法:(SystemFrequency / Value)个系统时钟节拍中断一次
*******************************************************************************/
void SysTick_Init_Config(void)
{

	while(SysTick_Config(SystemCoreClock / 1000));	//初始化并使能系统滴答时钟,返回1表示计数设置太大			 
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;		//失能滴答定时器 
}

/*******************************************************************************
* 函数名  : Delay_nMs
* 描述    : 延时程序(n毫秒)
* 输入    : nm：延时时间(n毫秒)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Delay_nMs(u32 nms)
{ 
	SysTickDelayTime = nms;		 
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//使能滴答定时器 
	while(SysTickDelayTime != 0);					//等待延时时间到
}

/*******************************************************************************
* 函数名  : Delay_S
* 描述    : 延时程序(n秒)
* 输入    : nS：延时时间(n秒)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Delay_nS(u32 ns)
{ 
	SysTickDelayTime = ns*1000;		 
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//使能滴答定时器 
	while(SysTickDelayTime != 0);					//等待延时时间到
}


/*******************************************************************************
* 函数名  : SysTickDelayTime_Counter
* 描述    : 获取节拍程序
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 在SysTick中断程序SysTick_Handler()调用(stm32f10x_it.c)
*******************************************************************************/ 
void SysTickDelayTime_Counter(void)
{
	if (SysTickDelayTime > 0)
	{ 
		SysTickDelayTime--;
	}
}
