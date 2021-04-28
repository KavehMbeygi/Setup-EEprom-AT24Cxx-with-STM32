/*
 * at24_hal_i2c.h
 *
 *  Created on: ap,11,2020
 *      Author: kaveh mohammadbeygi
 */
 
#ifndef DRIVERS_MYLIB_EEPROMAT24C_H_
#define DRIVERS_MYLIB_EEPROMAT24C_H_

#include "stm32f1xx_hal.h"
 

typedef struct ReporterInfo 
{
	char contact[20];
	char SpError1[15],
		 SpError2[15],
		 SpError3[15];			
}ReporterInfo;


void I2cBusScanner( I2C_HandleTypeDef *hi2c,UART_HandleTypeDef *huart);
void EEpromAT24IsAvailable( I2C_HandleTypeDef *hi2c,UART_HandleTypeDef *huart);

void WriteEEprom(uint16_t position, uint8_t dt);
uint8_t ReadEEprom(uint16_t position);

void WriteString(char *SomeString);
void ReadString(char *ReadString);


void ReadStruct(ReporterInfo *ReadedObj);
void WriteStruct(ReporterInfo *WriteStruct);
	
//#include "stm32f1xx_hal.h"
#endif
