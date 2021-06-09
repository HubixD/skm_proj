/*
 * hdc1080.c
 *
 *  Created on: Jun 4, 2021
 *      Author: msliwinski
 */

#include "hdc1080.h"

#define HDC_ADDRESS (0x40 << 1)


extern I2C_HandleTypeDef hi2c1;
static bool HDC_IsInitialized = false;


bool HDC1080_Init(void)
{
	/* 'tx/rx' data buffer */
	uint8_t data[2] = {0xFF, 0xFF};

	/* Write 'read ID register' request */
	HAL_I2C_Master_Transmit(&hi2c1, HDC_ADDRESS, data, 1, 1);

	data[0] = 0;

	/* Read 'ID register' */
	HAL_I2C_Master_Receive(&hi2c1, HDC_ADDRESS, data, 2, 1);

	/* Compare received data with expected value: 0x1050 */
	if (data[0] == 0x10 && data[1] == 0x50)
	{
		HDC_IsInitialized = true;
	}

	return HDC_IsInitialized;
}


bool HDC1080_TriggerData(void)
{
	uint8_t cmd = 0x00;
	HAL_I2C_Master_Transmit(&hi2c1, HDC_ADDRESS, (uint8_t*)&cmd, 1, 1);
	return true;
}


bool HDC1080_GetData( struct hdc_data_s * data )
{
	uint8_t buffer[4] = {0};

	if (HDC_IsInitialized)
	{
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)HDC_ADDRESS, (uint8_t*)buffer, 4, 100);

		/* calculate temperature and humidity according to HDC1080 datasheet, page 14 */
		data->temperature = ( ( (float)( (uint16_t)(buffer[0]<<8) + (uint16_t)buffer[1] ) ) / 65536.0 * 165.0 ) - 40.0;
		data->humidity = ( (float)( (uint16_t)(buffer[2]<<8) + (uint16_t)buffer[3] ) ) / 65536.0 * 100.0 ;

		return true;
	}

	else
	{
			data->temperature = 100.0;
			data->humidity = 0.0;
			HDC1080_Init();
	}

	return false;
}
