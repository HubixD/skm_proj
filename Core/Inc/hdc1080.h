#ifndef HDC_H_
#define HDC_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "evTim.h"
#include "stm32f3xx_hal.h"


/*
 * Structure containing HDC data.
*/
struct hdc_data_s {

	uint8_t status;
	float temperature;
	float humidity;
};


/**
 * Initialize HDC sensor.
 *
 * @return true if initialization successfull, false otherwise
*/
bool HDC1080_Init(void);


/**
 * Retrieve temperature ad humidity data from HDC sensor.
 *
 * @param pointer to /ref structure
 * @return true if data retrieved successfully, false otherwise
*/
bool HDC1080_GetData( struct hdc_data_s * data );


bool HDC1080_TriggerData(void);

#endif /* INC_HDC1080_H_ */
