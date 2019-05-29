#ifndef __em_H
#define __em_H
#include "sys.h"
//#include "arm_math.h"

typedef  float float32_t;
extern float32_t em_data_m1[200];
extern float32_t em_data_m2[200];
extern float32_t em_data_m3[200];
extern float32_t em_data_m4[200];

#define ADS_CS(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET))//SPI片选信号	   
#define ADS_RESET(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET))//SPI片选信号	   
#define LED(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET))	//PB1置1 
	


#ifndef _ads131a04_h_
#define _ads131a04_h_

#define FPGA_RDY        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8) 
#define AD_RDY_PIN        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8) 

/*ADS131A04 Definitions*/


//Read/Write Registers OPCODES
#define ADS131A04_READ_REGISTER_COMMAND_OPCODE							0x2000
#define ADS131A04_READ_MULTIPLE_REGISTERS_COMMAND_OPCODE				0x2000
#define ADS131A04_WRITE_REGISTER_COMMAND_OPCODE							0x4000
#define ADS131A04_WRITE_MULTIPLE_REGISTERS_COMMAND_OPCODE				0x6000

#define ADS131A04_READ_REGISTER_RESPONSE_OPCODE							0x2000
#define ADS131A04_READ_MULTIPLE_REGISTERS_RESPONSE_OPCODE				0x6000
#define ADS131A04_WRITE_REGISTER_RESPONSE_OPCODE						0x2000
#define ADS131A04_WRITE_MULTIPLE_REGISTERS_RESPONSE_OPCODE				0x4000

//System Commands
#define ADS131A04_NULL_COMMAND											0X0000
#define ADS131A04_RESET_COMMAND											0X0011
#define ADS131A04_STANDBY_COMMAND										0X0022
#define ADS131A04_WAKEUP_COMMAND										0X0033
#define ADS131A04_LOCK_COMMAND											0X0555
#define ADS131A04_UNCLOCK_COMMAND										0X0655

#define READ_REGISTER_COMMAND(address, register_count) 					(ADS131A04_READ_REGISTER_COMMAND_OPCODE | (address<<8) | register_count)
//Please note that register_count tells A04 device register_count+1 registers to read
#define READ_MULTIPLE_REGISTERS_COMMAND(address, register_count) 		(ADS131A04_READ_MULTIPLE_REGISTERS_COMMAND_OPCODE | (address<<8) | register_count)
#define WRITE_REGISTER_COMMAND(address, data) 							(ADS131A04_WRITE_REGISTER_COMMAND_OPCODE | (address<<8) | data)
#define WRITE_MULTIPLE_REGISTERS_COMMAND(address, register_count) 		(ADS131A04_WRITE_MULTIPLE_REGISTERS_COMMAND_OPCODE | (address<<8) | register_count)
#define WRITE_MULTIPLE_REGISTERS_ADDITIONAL_WORD(data1, data2)			((data1<<8) | data2)
//data1 is written to a register at adress N.  data2 is written to next register at address N+1.


//system responses
#define ADS131A04_READY													0xFF04
#define ADS131A04_STANDBY_ACK											0X0022
#define ADS131A04_WAKEUP_ACK											0X0033
#define ADS131A04_LOCK_ACK												0X0555
#define ADS131A04_UNCLOCK_ACK											0X0655

#define WRITE_MULTIPLE_REGISTERS_ACK(address, register_count) 			(ADS131A04_WRITE_MULTIPLE_REGISTERS_RESPONSE_OPCODE | (address<<8) | register_count)
#define WRITE_REGISTER_ACK(address, data) 								(ADS131A04_WRITE_REGISTER_RESPONSE_OPCODE | (address<<8) | data)
#define READ_REGISTER_RESPONSE_PARSE(response, op_code, address, data) \
{ \
	op_code=(response & 0xE000); \
	address = ((response & 0x1F00)>>8); \
	data=(response & 0x00FF);\
}
#define READ_MULTIPLE_REGISTERS_ACK(address, register_count) 			(ADS131A04_READ_MULTIPLE_REGISTERS_RESPONSE_OPCODE | (address<<8) | register_count)
#define READ_MULTIPLE_REGISTER_RESPONSE_PARSE(response, data1, data2) \
{ \
	data1=((response & 0xFF00)>>8);\
	data2=(response & 0x00FF);\
}

/* ADS131A0x Register Settings Export */
/******************************************************************************/
/* This file contains the register map settings stub */
// General defines
#define ADS131A0x_REGISTER_COUNT 17
/* Register #define values (register address and value) */
/******************************************************************************/
/* This section contains the defines for register address and register settings */
//Register Addresses
#define ID_MSB															0x00 //ID Control Register MSB
#define ID_LSB															0x01 //ID Control Register LSB
#define STAT_1															0x02 //Status 1 Register
#define STAT_P															0x03 //Positive Input Fault Detect Status
#define STAT_N															0x04 //Negative Input Fault Detect Status
#define STAT_S															0x05 //SPI Status Register
#define ERROR_CNT														0x06 //Error Count Register
#define STAT_M2															0x07 //Hardware Mode Pin Status Register
//#define RESERVED														0x08
//#define RESERVED														0x09
//#define RESERVED														0x0A
#define A_SYS_CFG														0x0B //Analog System Configuration Register
#define D_SYS_CFG														0x0C //Digital System Configuration Register
#define CLK1															0x0D //Clock Configuration 1 Register
#define CLK2															0x0E //Clock Configuration 2 Register
#define ADC_ENA															0x0F //ADC Channel Enable Register
//#define RESERVED														0x10
#define ADS131_ADC1															0x11 //ADC Channel 1 Digital Gain Configuration
#define ADS131_ADC2															0x12 //ADC Channel 2 Digital Gain Configuration
#define ADS131_ADC3															0x13 //ADC Channel 3 Digital Gain Configuration
#define ADS131_ADC4															0x14 //ADC Channel 4 Digital Gain Configuration

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*****************************************************
* Local Prototypes
*****************************************************/
void ADS13_PowerOnInit(void);
void Read_ADS131A0X_Value(u8 cnt);
#endif

void ADS131A0X_Init(void);
void AD_FPGA_read_once (float32_t* m1,float32_t* m2,float32_t* m3);
void get_data(void);
void deal_with_AD_rawdata(void);
#endif
