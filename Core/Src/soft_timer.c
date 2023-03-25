#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "soft_timer.h"

extern TIM_HandleTypeDef htim1;

/**
 *  
 */
uint32_t soft_timer_get_period(void)
{
    return htim1.Init.Period;
}

/**
 *  
 */
uint32_t soft_timer_get_count(void)
{
    return __HAL_TIM_GET_COUNTER(&htim1);
}

/**
 *  
 */
void soft_timer_init(void)
{
    // Note: Prescaler is set to 99
    // So, one tick of the timer is equal to 1μs

    HAL_TIM_Base_Start(&htim1);
}
