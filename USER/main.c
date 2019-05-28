#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "em.h"

/************************************************
 ALIENTEK ������STM32F7������ ʵ��36
 NRF24L01����ͨ��ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
    
int main(void)
{
	u8 key,mode;
	u16 t=0;			 
	u8 tmp_buf[33];	 

    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	  uart_init(115200);		        //���ڳ�ʼ��
    //LED_Init();                     //��ʼ��LED
		ADS13_PowerOnInit();
	while(1);
         
}
