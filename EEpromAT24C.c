#include <EEpromAT24C.h>
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>
//***********************************//
#define EEPROMADDRESS  	(uint16_t)(0x51<<1)
uint8_t PageSize= 127 ;
char EEpromWriteBuffer[127];
ReporterInfo SomObj;
ReporterInfo *Mystruct2;
//***********************************//
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
//***********************************//
void WriteEEprom(uint16_t position, uint8_t dt)
{
	HAL_I2C_Mem_Write(&hi2c1,EEPROMADDRESS, position, 0xFF, &dt, 1,1000);
	HAL_Delay(11);
}
//***********************************//
uint8_t ReadEEprom(uint16_t position)
{
	uint8_t dt;
	HAL_I2C_Mem_Read(&hi2c1,EEPROMADDRESS , position, 0xFF, &dt ,1 ,1000);
	return dt;
}
//***********************************//
void WriteString(char *WriteString)
{
	HAL_UART_Transmit(&huart1,( uint8_t *) "ready to Write\n" , strlen("ready to Write\n"),1000);	
	for(int step=0 ; step<=PageSize ;step++  )
	{
		WriteEEprom(step , WriteString[step]);
	}
	HAL_UART_Transmit(&huart1,( uint8_t *)WriteString , strlen(WriteString),1000);
	HAL_UART_Transmit(&huart1,( uint8_t *) "\nEnd Write\n" , strlen("\nEnd Write\n"),1000);
}
//***********************************//
void ReadString(char *ReadString)
{
	HAL_UART_Transmit(&huart1,( uint8_t *) "\nBegin Read\n" , strlen("\nBegin Read\n"),1000);
	char OutPutChar[PageSize];
	memset(OutPutChar, 0x00 , sizeof(PageSize));
	
	
	for(int step=0 ; step<=PageSize ;step++  )
	{
		OutPutChar[step] = ReadEEprom(step);
	}
	strcpy(ReadString,OutPutChar);
 	HAL_UART_Transmit(&huart1,( uint8_t *) "\nEnd Read\n" , strlen("\nEnd Read\n"),1000);
}
//***********************************//
void WriteStruct(ReporterInfo *WriteStruct)
{
	
	snprintf(EEpromWriteBuffer, 100, "%.20s %.15s %.15s", WriteStruct->contact ,WriteStruct->SpError1 ,WriteStruct->SpError2 );
	HAL_UART_Transmit(&huart1,( uint8_t *)EEpromWriteBuffer, strlen(EEpromWriteBuffer),1000);
 
}

//***********************************//
void ReadStruct(ReporterInfo *ReadStruct)
{
	char EEpromWriteBuffer[PageSize];
	ReadString(EEpromWriteBuffer);
	strcpy ( (char *)ReadStruct  ,EEpromWriteBuffer );
}
//***********************************//

void EEpromAT24IsAvailable( I2C_HandleTypeDef *hi2c,UART_HandleTypeDef *huart)
{
    if(HAL_I2C_IsDeviceReady(hi2c ,   EEPROMADDRESS   , 2, HAL_MAX_DELAY) != HAL_OK)
    {
		HAL_UART_Transmit(huart,( uint8_t *) "Not Found" , strlen("Not Found"),1000);
		HAL_UART_Transmit(huart ,( uint8_t *) "\n" , strlen("\n"),1000);
	  while (1)
	  {
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		  HAL_Delay(100);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		  HAL_Delay(100);
	  }
    }
}

//***********************************//
void I2cBusScanner( I2C_HandleTypeDef *hi2c,UART_HandleTypeDef *huart)
{ 
    HAL_UART_Transmit(huart, (uint8_t *)"Connected to UART\r\n",strlen("Connected to UART\r\n"), 0xFFFF);
	HAL_UART_Transmit(huart, (uint8_t *)"Scanning I2C bus:\r\n",strlen("Scanning I2C bus:\r\n"), 0xFFFF);
	HAL_StatusTypeDef result;
 	uint8_t i;
 	for (i=1; i<128; i++)
 	{
 	  result = HAL_I2C_IsDeviceReady(hi2c , (uint16_t)(i<<1), 2, 2);
 	  if (result != HAL_OK) // HAL_ERROR or HAL_BUSY or HAL_TIMEOUT
 	  {
		  HAL_UART_Transmit(huart, (uint8_t *)"*",strlen("*"), 0xFFFF);
 	  }
 	  if (result == HAL_OK)
 	  {
		  char c[10];
		  HAL_UART_Transmit(huart, (uint8_t *)"\n",strlen("\n"), 0xFFFF);
		  sprintf(c, "%d",i);
		  HAL_UART_Transmit(huart, (uint8_t *)c,strlen(c), 0xFFFF);
		  HAL_UART_Transmit(huart, (uint8_t *)"\n",strlen("\n"), 0xFFFF);
 	  }
 	}
		  HAL_UART_Transmit(huart, (uint8_t *)"\r\n",strlen("\r\n"), 0xFFFF);
}
//***********************************//
