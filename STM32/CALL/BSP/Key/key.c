/**********************************************************************************
 * 文件名  ：key.c
 * 描述    ：按键扫描(系统滴答时钟SysTick实现扫描延时)
 * 实验平台：NiRen_TwoHeart系统板
 * 硬件连接：  PC10 -> KEY1     
 *             PC11 -> KEY2       
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "key.h" 
#include "SysTick.h"

/*******************************************************************************
* 函数名  : Key_GPIO_Config
* 描述    : KEY IO配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : KEY(1~2)的IO口分别是:PC10,PC11
*******************************************************************************/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(KEY_RCC, ENABLE);			//使能GPIOA的外设时钟	
	
	GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2;			//选择要初始化的GPIOA引脚(PA15,PA14)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置引脚工作模式为上拉输入 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);			//调用库函数中的GPIO初始化函数，初始化GPIOA中的PA15,PA14引脚
}

/*******************************************************************************
* 函数名  : Key_Detection
* 描述    : 按键按下检测
* 输入    : GPIOx：按键对应的GPIO，GPIO_Pin：对应按键端口
* 输出    : 无
* 返回    : KEY_DOWN(0):对应按键按下，KEY_UP(1):对应按键没按下
* 说明    : KEY(1~2)的IO口分别是:PC10,PC11
*******************************************************************************/
u8 Key_Down_Detection(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{			
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN)	//检测是否有按键按下 
	{	   
		Delay_nMs(10);	//延时消抖		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN)	//检测是否有按键按下   
		{	 
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN);	//等待按键释放  
			return KEY_DOWN;	 
		}
		else
		{
			return KEY_UP;
		}
	}
	else
	{
		return KEY_UP;
	}
}

/*******************************************************************************
* 函数名  : Key_Down_Scan
* 描述    : 按键按下扫描
* 输入    : 无
* 输出    : 0：没有按键按下，1：按键按下
* 返回    : 无 
* 说明    : KEY(1~2)的IO口分别是:PC10,PC11
*******************************************************************************/
u8 Key_Down_Scan(void)
{
	u16 downflag=0;
	u8 value=0;

  	downflag=GPIO_ReadInputData(KEY_PORT);
	if((downflag & 0x0c00) != 0x0c00)	//检测是否有按键按下 
	{	   
		Delay_nMs(10);	//延时消抖	
		downflag = GPIO_ReadInputData(KEY_PORT) & 0x0c00;	
		if(downflag != 0x0c00 )	//检测是否有按键按下   
		{	 
			if((downflag & 0x0400) == 0) value |= 0x01;
			if((downflag & 0x0800) == 0) value |= 0x02; 	 
		}		
	}	

	return value;
}
