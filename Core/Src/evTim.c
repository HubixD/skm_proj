#include "stm32f3xx_hal.h"
#include "evTim.h"

#define EVTIM_GET_COUNTER()     HAL_GetTick()


void EvTim_ActivateMs(EvTim_stamp_t *timeEvent_p, uint32_t delayMs)
{
  if(NULL != timeEvent_p)
  {
    timeEvent_p->timestamp = (EVTIM_GET_COUNTER() + delayMs);
		timeEvent_p->isRunning = true;
  }
}


void EvTim_Deactivate(EvTim_stamp_t *timeEvent_p)
{
  if(NULL != timeEvent_p)
  {
    timeEvent_p->timestamp = 0;
		timeEvent_p->isRunning = false;
  }
}


EvTim_State_t EvTim_getState(const EvTim_stamp_t *timeEvent_p)
{
	if (timeEvent_p != NULL)
	{
		if (!timeEvent_p->isRunning)
		{
			return EVTIM_STOP;
		}
		else
		{
			if(EVTIM_GET_COUNTER() > timeEvent_p->timestamp)
			{
				return EVTIM_TIMES_UP;
			}
			else
			{
				return EVTIM_IN_PROGRESS;
			}
		}		
	}
	else
	{
		return EVTIM_ERROR;
	}
}


void EvTim_DelayMs(uint32_t timeMs)
{
  EvTim_stamp_t myTimer = {0,false};
  EvTim_ActivateMs(&myTimer, timeMs);

  while(EvTim_getState(&myTimer) == EVTIM_IN_PROGRESS)
  {

  }
}
