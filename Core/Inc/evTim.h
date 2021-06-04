
#ifndef  EVTIM_H
#define  EVTIM_H

#include "stdint.h"
#include "stdbool.h"

typedef enum EvTim_State_t
{
  EVTIM_STOP = 0,
  EVTIM_IN_PROGRESS,
  EVTIM_TIMES_UP,
  EVTIM_ERROR
} EvTim_State_t;


typedef struct{
	uint32_t timestamp;
	bool isRunning;
}EvTim_stamp_t;


/** @brief Activate timer for @ref delayMs ms*/
void EvTim_ActivateMs(EvTim_stamp_t *timeEvent_p, uint32_t delayMs);


/** @brief Deactivate timer*/
void EvTim_Deactivate(EvTim_stamp_t *timeEvent_p);


/** @brief Wait @ref timeMs ms, function refresh watchdog*/
void EvTim_DelayMs(uint32_t timeMs);


/** @brief Check the timer status
		@return EVTIM_ERROR, pointer error
						EVTIM_STOP - event is deactivated
						EVTIM_IN_PROGRESS - event not occurred yet
						EVTIM_TIMES_UP - time event occurred*/
EvTim_State_t EvTim_getState(const EvTim_stamp_t *timeEvent_p);

#endif /* EVTIM_H */
