/**********************************************************************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库         
 * 实验平台：NiRen_TwoHeart系统板
 * 硬件连接：  PB5 -> LED1     
 *             PB6 -> LED2     
 *             PB7 -> LED3    
 *             PB8 -> LED3    
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "Led.h"
	   
/*******************************************************************************
* 函数名  : GPIO_Config
* 描述    : LED 和PWR_MG323 IO配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : LED(1~4)的IO口分别是:PB5,PB6,PB7,PB8  PWR_MG323:PB9
*******************************************************************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);			//使能GPIOB的外设时钟	
	
	GPIO_InitStructure.GPIO_Pin = LED_ALL|PWR_MG323;	//选择要初始化的GPIOB引脚(PB5,PB6,PB7,PB8,PB9)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置引脚工作模式为通用推挽输出 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);			//调用库函数中的GPIO初始化函数，初始化GPIOB中的PB5,PB6,PB7,PB8,PB9引脚

	LED_ALL_OFF();										//关闭ALL_LED  					 
}

/*点亮LED1.PB5*/
void LED1_ON(void) 
{
	GPIO_SetBits(LED_PORT,LED1);
}

/*关闭LED1.PB5*/
void LED1_OFF(void)
{
	GPIO_ResetBits(LED_PORT,LED1);
}

/*点亮LED2.PB6*/
void LED2_ON(void)  
{
	GPIO_SetBits(LED_PORT,LED2);
}

/*关闭LED2.PB6*/
void LED2_OFF(void)
{
	GPIO_ResetBits(LED_PORT,LED2);
}

/*点亮LED3.PB7*/
void LED3_ON(void)   
{
	GPIO_SetBits(LED_PORT,LED3);
}

/*关闭LED3.PB7*/
void LED3_OFF(void)  
{
	GPIO_ResetBits(LED_PORT,LED3); 
}

/*点亮LED4.PB8*/
void LED4_ON(void) 
{
	GPIO_SetBits(LED_PORT,LED4);
}

/*关闭LED4.PB8*/
void LED4_OFF(void) 
{
	GPIO_ResetBits(LED_PORT,LED4); 
}

/*点亮ALL_LED*/
void LED_ALL_ON(void)
{
	GPIO_SetBits(LED_PORT,LED_ALL);
}
/*关闭ALL_LED*/
void LED_ALL_OFF(void)  
{
	GPIO_ResetBits(LED_PORT,LED_ALL); 
}

