#include "stm32f10x.h"
#include "usart.h"
#include "Led.h"
#include "SysTick.h"
#include "timer.h"
#include "string.h"



#define Buf2_Max 200 					  //串口2缓存长度

char Uart2_Buf[Buf2_Max];
u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;
vu8 Timer0_start;	//定时器0延时启动计数器
vu8 Heart_beat;		//发送心跳帧标志位

int ATCMD_number=4;//需要发送的指令条数
char *AT_Command[]={
  "AT+CPIN?",
	"AT+CSQ",
  "AT+CREG?",
	"ATD10086;",
	//"AT+CMGF=1",
  //"AT+CMGS=\"10086\"",
};
void CLR_Buf2(void);
void UART2_Send(float *a)
{
	while(*a)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*a++);//发送当前字符
	}
}


/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 注意   : 串口2负责与CP2102模块通信，串口1用于串口调试，可以避免在下载程序时数据
					 还发送到模块
*******************************************************************************/
u16 data=0x1a;
int main(void)
{
  int cmd_count;
	SysTick_Init_Config();
  GPIO_Config();
  Timer2_Init_Config();
  USART1_Init_Config(115200);
  USART2_Init_Config(115200);
  UART1_SendString("SIM7600X call 10086 Demo\r\n");
  for(cmd_count=0;cmd_count<ATCMD_number;cmd_count++){
  UART2_SendString(AT_Command[cmd_count]);
    UART2_SendLR();
		Delay_nMs(5000); 
    UART1_SendString(Uart2_Buf);
    UART1_SendLR();
    CLR_Buf2();
  }
		Delay_nMs(8000); 

}

/*******************************************************************************
* 函数名  : USART2_IRQHandler
* 描述    : 串口1中断服务程序
* 输入    : 无
* 返回    : 无 
* 说明    : 
*******************************************************************************/
void USART2_IRQHandler(void)                	
{
			u8 Res=0;
			Res =USART_ReceiveData(USART2);
			Uart2_Buf[First_Int] = Res;  	  //将接收到的字符串存到缓存中
      
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}    
} 	

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM3中断
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		if(Timer0_start)
		Times++;
		if(Times > shijian)
		{
			Timer0_start = 0;
			Times = 0;
		}
		
		Heartbeat++;
		if(Heartbeat>9)//每10秒发送心跳帧
		{
			Heartbeat=0;
			Heart_beat=1;
		}
		if(flag)
		{
			LED4_ON(); 
			flag=0;
		}
		else
		{
			LED4_OFF(); 
			flag=1;
		}
	}	
}

void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}
