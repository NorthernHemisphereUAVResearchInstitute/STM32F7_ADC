#include "em.h"
#include "spi.h"
#include "delay.h"
#include "led.h"
//#include "fir.h"

extern float32_t em_data_m1[200];
extern float32_t em_data_m2[200];
extern float32_t em_data_m3[200];
extern float32_t em_data_m4[200];

void ADS131A0X_Init(void)
{
	
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();			//����GPIOGʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOGʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_10; //PG10 ce����
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);     //��ʼ��
	
	GPIO_Initure.Pin=GPIO_PIN_10; //PG10 reset
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     //��ʼ��
    
    SPI2_Init();    		                //��ʼ��SPI2  
	
    __HAL_SPI_DISABLE(&SPI2_Handler);               //�ȹر�SPI2
    SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_LOW; //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;     //����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
    HAL_SPI_Init(&SPI2_Handler);
    __HAL_SPI_ENABLE(&SPI2_Handler);                //ʹ��SPI2

	ADS_CS(1);			            		//SPIƬѡȡ��	
	ADS_RESET(0);
	ADS_RESET(1);
}

 


/***************************************************************************************** 
* Function Name: ADS13_SPI 
* Description  : ͨ��SPI������ADS131ͨ�� 
* Arguments    : com:��д������� 
* Return Value : ���ض�ȡ������ 
******************************************************************************************/  
unsigned char ADS13_SPI(unsigned char com)  
{
	u8 rx;
	HAL_SPI_TransmitReceive(&SPI2_Handler,&com,&rx,1, 1000);
	return rx;
}  


/* write a command to ADS131a02 */  
u16 ADS13_WRITE_CMD(u16 command)  
{  
    u16 receive=0x0000;  
    
    ADS_CS(0);  
	delay_us(10);  
	
    receive = ADS13_SPI((u8)(command>>8)); 
    receive <<= 8;  
    receive |= ADS13_SPI((u8)(command)); 
    ADS13_SPI(0X00);//����3���ֽ�   24bit
    ADS13_SPI(0X00);//����4���ֽ�   32bit
    ADS_CS(1);   
	delay_us(10);  
    return receive; //���ؽ��յ�����  
}

/***************************************************************************************** 
* Function Name: ADS13_REG 
* Description  : ��ADS131�ڲ��Ĵ������в���  
* Arguments    : com:��д�Ĵ�����ַ��data����д������� 
* Return Value : ���ض�ȡ������ 
******************************************************************************************/  
u16 ADS13_REG(unsigned char com, unsigned data)  
{  
    u16 temp=0X0000;   
    if((com&0x20)==0x20)//�ж��Ƿ�Ϊ���Ĵ���ָ�� if����Ϊ��  
    {
        ADS13_WRITE_CMD(com);  
			  temp= ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
    }  
    else //д�Ĵ���  
    {
        ADS_CS(0);
		delay_us(10);  
        ADS13_SPI(com);  
        ADS13_SPI(data);  
        ADS13_SPI(0X00); // ����0  24bit
        ADS13_SPI(0X00); // ����0  32bit
        delay_us(10);   
        ADS_CS(1);  
    }  
    delay_us(10);   
    return temp;  
}    
  
/***************************************************************************************** 
* Function Name: ADS13_PowerOnInit 
* Description  : ADS13�ϵ縴λ 
* Arguments    : NONE 
* Return Value : NONE 
******************************************************************************************/  
int adcenable_flag=0;  
void ADS13_PowerOnInit(void)  
{
	u8 RREG=0X20;//��
	u8 WREG=0X40;//д
	u16 RECEIVE=0X0000; //����spi���ص��ַ�  
	u16 ID_M,ID_L;
    ADS131A0X_Init(); 	
	
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_8);
    delay_ms(20); 

    do  
    {  
		ADS13_WRITE_CMD(ADS131A04_RESET_COMMAND);  //RESET ADS131
        RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);  
        printf("receive data is : %X...\n", RECEIVE);  
        if(RECEIVE == ADS131A04_READY) //ADS131A02 is ready.break loop  
		{
			printf("ADS131A02 is ready!\n  Configuring Registers...\n"); 
		}			
        delay_ms(5);  
		
    }while(RECEIVE != ADS131A04_READY); //��ʼ���ɹ�������ѭ��
		
	ADS13_WRITE_CMD(ADS131A04_UNCLOCK_COMMAND);//�����Ĵ���
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);  
	
	ADS13_WRITE_CMD(0x2000);//read id
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
	
	ADS13_WRITE_CMD(0x2100);//read id
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
	
	ADS13_REG(WREG|A_SYS_CFG, 0X78); //enable internal reference volatage (4.0v)  
	ADS13_REG(WREG|D_SYS_CFG, 0X3E); //Fixed six device words per frame for the ADS131A04 
	ADS13_REG(WREG|CLK1, 0X02); //ADC CLK1  fICLK = fCLKIN(16mhz) / 2   
	ADS13_REG(WREG|CLK2, 0X2d); //fMOD = fICLK / 2  fICLK = fCLKIN / 48 ****** now is 8.333khz *****0x21  
	ADS13_REG(WREG|ADC_ENA, 0X0F); //ADC CHANNEL ENABLE ALL  
	
	ADS13_WRITE_CMD(0x2B00);//read A_SYS_CFG
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
	
	ADS13_WRITE_CMD(0x2C00);//read D_SYS_CFG
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
	
	ADS13_WRITE_CMD(ADS131A04_WAKEUP_COMMAND);//WAKEUP ADS131  
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);  
	
	ADS13_WRITE_CMD(ADS131A04_LOCK_COMMAND);//WAKEUP ADS131  
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);
  
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_2);
	adcenable_flag=1; //��ʼ����ϱ�־λ  
}  
  
u8 ADC_READY(void)
{
	while(AD_RDY_PIN==0)
	{
		//LED(0);
		//LED(1);
	}
	
	while(AD_RDY_PIN==1)
	{
		//LED(1);
	}
	

	return 1;
}

u8 dummy_send[24]={0};
u8 recBuffer[24];
void Read_ADS131A0X_BUFFER_DATA(void)
{
	u8 i=0;
	ADS_CS(0); 
	for(i = 0; i < 24; i++)
	{
		recBuffer[i]=ADS13_SPI(dummy_send[i]);
	}
	ADS_CS(1);
}
	

u8 rx[200][24]={0};
void Read_ADS131A0X_Value(u8 cnt)//��ȡADC //�Ȱ����ݻ������ʱ�䲻��
{
	TEST_D5(0);

	static u8 tx[24]={0};
	
	if(ADC_READY())//�ȴ�ad����
	{
		
		ADS_CS(0);
		// ��ȡ�������� 24bit
		HAL_SPI_TransmitReceive(&SPI2_Handler,tx,&rx[cnt][0],24, 1000);
		ADS_CS(1);
		TEST_D5(1);
	}
	
	TEST_D5(1);
	
}

void deal_with_AD_rawdata()
{
	TEST_D5(0);
	
	uint32_t channel[4];
	
	for(u8 i = 0;	i<200;	i++)
	{
		channel[0] = rx[i][4];
		channel[0] = channel[0]<<8|rx[i][5];
		channel[0] = channel[0]<<8|rx[i][6];
		
		channel[1] = rx[i][8];
		channel[1] = channel[1]<<8|rx[i][9];
		channel[1] = channel[1]<<8|rx[i][10];
		
		channel[2] = rx[i][12];
		channel[2] = channel[2]<<8|rx[i][13];
		channel[2] = channel[2]<<8|rx[i][14];
		
		channel[3] = rx[i][16];
		channel[3] = channel[3]<<8|rx[i][17];
		channel[3] = channel[3]<<8|rx[i][18];
		
		em_data_m1[i] = (float32_t)channel[0];
		em_data_m2[i] = (float32_t)channel[1];
		em_data_m3[i] = (float32_t)channel[2];
		em_data_m4[i] = (float32_t)channel[3];
	}
}

void get_data()
{
	for(u8 i = 0; i<200; i++)
	{
		Read_ADS131A0X_Value(i);
	}
}

