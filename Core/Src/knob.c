#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "knob.h"
#include "debug.h"

extern ADC_HandleTypeDef hadc1;

uint32_t knob_value;
bool is_knob_value_obtained;

/**
 *  ADC Conversion Error Handler
 */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    PRINT ("Error in reading Knob value !!! \r\n");
}

/**
 *  ADC Conversion Callback
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // Store the ADC value
	// knob_value = HAL_ADC_GetValue(hadc);

    // Stop the conversion
    knob_stop();
    is_knob_value_obtained = true;
}

/**
 *  Get the current Knob value
 */
uint32_t knob_get(void)
{
    uint8_t timeout = 20;

    do {
        if (is_knob_value_obtained)
        {
            // We are intentionally not clearing the flag
            // The flag will be cleared when the next knob read is intiated
            // Until then the last read value will be returned
            break;
        }

        osDelay(5);

    } while (-- timeout);

    return knob_value;
}

/**
 *  Stop ADC Conversion
 */
void knob_stop(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
}

/**
 *  Start ADC Conversion
 */
void knob_start(void)
{
    is_knob_value_obtained = false;
    knob_value = 0;
    HAL_ADC_Start_DMA(&hadc1, &knob_value, 1);
}

/**
 *  Initialize the Potentiometer knob
 */
void knob_init(void)
{
    knob_value = 0;
    is_knob_value_obtained = false;
}