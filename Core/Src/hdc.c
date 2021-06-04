#include "hdc.h"
#include "i2c.h"
#include "timeouts.h"
#include "uart.h"

#define HDC_ADDRESS					(0x40<<1)

#define HEAT_INTERVAL				600000		//10 min
#define HEAT_INTERVAL_TINY 	10000			//10 s
#define HUMIDITY_THRESHOLD	255				//change in order to set heating threshold (feature disabled if val > 100)

static bool HDC_IsInitialized = false;
EvTim_stamp_t heatTimer;
uint8_t heaterCnt;

/*
 *	 HDC Initialization.
 */
bool HDC_Init(void) {
	
	int ret = 0;
	
	if (I2C_Init() != true)
	{
		LOG(" UNABLE initialize i2c\r\n");
		return false;
	}
	
	/* 'tx/rx' data buffer */
	uint8_t data[2] = {0xFF, 0xFF};

	/* Write 'read ID register' request */
	ret = I2C_WriteData(HDC_ADDRESS, data, 1);
	
	if (ret == -1)
	{
		LOG(" UNABLE write init data\r\n");
		return false;
	}
	
	ret = 0;
	data[0] = 0;
	
	/* Read 'ID register' */
	ret = I2C_ReadData(HDC_ADDRESS, data, 2);
	
	if (ret == -1)
	{
		LOG(" UNABLE read id reg\r\n");
	}
	
	/* Compare received data with expected value: 0x1050 */
	if (data[0] == 0x10 && data[1] == 0x50)
	{
		HDC_IsInitialized = true;
	}

	return HDC_IsInitialized;
}


/*
 *	 HDC get temperature and humidity.
 */
bool HDC_GetData( struct hdc_data_s * data ) {

	uint8_t cmd = 0x00;
	uint8_t buffer[4] = {0};
	int ret = 0;
	
	if (HDC_IsInitialized)
	{
			/* write 'measuremenet request' command */
			ret = I2C_WriteData(HDC_ADDRESS, &cmd, 1);
			
			if (ret == -1)
			{
				LOG(" UNABLE 'I2C_WriteData'\r\n");
			}
			
			/* wait for conversion to be completed - 18 ms delay */
			for (int t=250000; t>0; t--) {asm("nop");}
			
			if (I2C_ReadData( HDC_ADDRESS, buffer, 4) != 4)
			{
					if (ret < 0)
					{
							LOG(" UNABLE 'I2C_ReadData'\r\n");
					}
					
					data->temperature = 100;
					data->humidity = 0.0;
					return false;
			}

			/* calculate temperature and humidity according to HDC1080 datasheet, page 14 */
			data->temperature = ( ( (float)( (uint16_t)(buffer[0]<<8) + (uint16_t)buffer[1] ) ) / 65536.0 * 165.0 ) - 40.0;
			data->humidity = ( (float)( (uint16_t)(buffer[2]<<8) + (uint16_t)buffer[3] ) ) / 65536.0 * 100.0 ;
			
			return true;
	}

	else
	{
			data->temperature = 100.0;
			data->humidity = 0.0;
			HDC_Init();
	}
	
	return false;
}


/*
 *	 HDC enables heating to prevent incorrect readings in case of high humidity conditions.
 */
bool HDC_HeatHandler(uint8_t humidity)
{
	uint8_t buffer[4] = {0x02, 0x00, 0x00};
	
	if(humidity > HUMIDITY_THRESHOLD)	
	{
		
		switch(EvTim_getState(&heatTimer))		//set up timer for idling operations
		{
			case EVTIM_STOP:
				EvTim_ActivateMs(&heatTimer, HEAT_INTERVAL);
				break;
			
			case EVTIM_IN_PROGRESS:
				if(heaterCnt <= 0 || heaterCnt > 6) heaterCnt = false;
				break;
			
			case EVTIM_TIMES_UP:
				heaterCnt++;
				EvTim_Deactivate(&heatTimer);
				EvTim_ActivateMs(&heatTimer, HEAT_INTERVAL);
				break;
			
			default:
				heaterCnt = false;
				return false;
				break;
		};
		
		//inc every 10 min => if 60 minutes passed proceed to heating
		if(heaterCnt == 6)
		{
			buffer[1] = 0x30;		//if heating then set 13th and 12th bit
		}
		else
		{
			buffer[1] = 0x10;		//if no heating then set only 12th bit
		}
		
		Delay_ms(15);
		
#if DEBUG == 1
		TERMINALx("buffer to send:  | %d %d %d | \r\n", buffer[0], buffer[1], buffer[2]);
#endif
		
		if(I2C_WriteData(HDC_ADDRESS, buffer, 3))
		{
			if(buffer[1] == 0x30)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		EvTim_Deactivate(&heatTimer);
		return false;
	}
}



/*
 *	 HDC perform software reset operation.
 */
bool HDC_ResetDevice(void) {
	
	//TODO: send reg value to proper register
	Delay_ms(100);
	return false;
}
