#ifndef HDC_H_
#define HDC_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "evTim.h"

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
bool HDC_Init(void);


#include "stm32f3xx_hal.h"
#include "stdbool.h"

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
bool HDC_Init(void);


/**
 * Retrieve temperature ad humidity data from HDC sensor.
 *
 * @param pointer to /ref structure
 * @return true if data retrieved successfully, false otherwise
*/
bool HDC_GetData( struct hdc_data_s * data );


/**
 * Handle heat operations depending on humidity level.
 *
 * @param pointer to /ref structure
 * @return true if operation handled successfuly
*/
bool HDC_HeatHandler(uint8_t humidity);


/**
 * Reset HDC sensor.
 *
 * @return true if reset was successfull, false otherwise
*/
bool HDC_ResetDevice(void);

#endif /* INC_HDC1080_H_ */


/**
 * Reset HDC sensor.
 *
 * @return true if reset was successfull, false otherwise
*/
bool HDC_ResetDevice(void);


#endif
