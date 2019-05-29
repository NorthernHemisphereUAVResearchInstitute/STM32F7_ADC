#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
//#include "sdram.h"
#include "em.h"

/************************************************
 ALIENTEK 阿波罗STM32F7开发板 实验36
 NRF24L01无线通信实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
    
int main(void)
{
		u8 i;
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	  uart_init(115200);		        //串口初始化
    LED_Init();                     //初始化LED
		ADS13_PowerOnInit();
		while(1)
		{
			get_data();
			deal_with_AD_rawdata();
			for(i=0;i<200;i++)
			{
				printf("%f\r\n",em_data_m1[i]);
			}
			//delay_ms(100);
		} 
}
