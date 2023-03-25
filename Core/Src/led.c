#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "led.h"
#include "debug.h"
#include "soft_timer.h"


/**
 *  
 */
void led_on(void)
{
    HAL_GPIO_WritePin(POWER_LED_GPIO_Port, POWER_LED_Pin, GPIO_PIN_RESET);
}

/**
 *  
 */
void led_off(void)
{
    HAL_GPIO_WritePin(POWER_LED_GPIO_Port, POWER_LED_Pin, GPIO_PIN_SET);
}

/**
 *
 */
void led_init(void)
{
    led_off();
}
