#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "led.h"
#include "debug.h"
#include "soft_timer.h"
#include "knob.h"
#include "button.h"
#include "ventilator.h"

int config_state = 1;
int patient_config_state = 1;
bool pwm_enable = false;
static bool stop_config_task = false;
/**
 *  
 */
void app_init(void)
{
    led_init();
    debug_init();
    soft_timer_init();
    knob_init();
    button_init();
}

/**
 *  
 */
void app_debug_task(void)
{
    debug_process();
}

/**
 *
 */
void app_button_task(void)
{
	uint32_t value;

	if(stop_config_task){
		// When Patient thread is active, configuration is not supported
		config_state = 1; // reset control state to init
		return;
	}
	button_poll();

	if(control_button_pressed == NONE)
	{
		return;
	}

    if (control_button_pressed == LONG_PRESSED) {
        if (config_state !=1) {
            // It should be possible to power off the device from any state
            config_state = 7;
        }
    }

	switch(config_state)
	{
	case 1:// Long Press, Switch on
		if(control_button_pressed == LONG_PRESSED)
		{
			PRINT ("==========================\r\n");
			PRINT ("Power ON\r\n");
			PRINT ("==========================\r\n");
            led_on();

			// Move to Next state
			config_state++;
			control_button_pressed = NONE;
		}

	break;

    case 2:// Normal Press, Enter settings mode
		if(control_button_pressed == NORMAL_PRESSED)
		{
			PRINT ("==========================\r\n");
			PRINT ("Entered Configuration Mode\r\n");
			PRINT ("==========================\r\n");
			// Move to Next state
			config_state++;
			control_button_pressed = NONE;
		}

	break;

	case 3: // Short press, read Knob and store Vt
		if(control_button_pressed == SHORT_PRESSED)
		{
			// Read Knob
			knob_start();
			osDelay(5);
            value = knob_get();
            ventilator_calc_vt(value);
			// Move to Next state
			config_state++;
			control_button_pressed = NONE;
		}

	break;

	case 4: // Short Press, read knob and store RR
		if(control_button_pressed == SHORT_PRESSED)
		{
			// Read Knob
			knob_start();
			osDelay(5);
            value = knob_get();
            ventilator_calc_rr(value);
			// Move to Next state
			config_state++;
			control_button_pressed = NONE;
		}

	break;


	case 5: // short Press, Read Knob and store Qmax
		if(control_button_pressed == SHORT_PRESSED)
		{
			// Read Knob
			knob_start();
			osDelay(5);
            value = knob_get();
            ventilator_calc_qmax(value);
			// Move to Next state
			config_state++;
			control_button_pressed = NONE;
		}

	break;

	case 6: // Normal Press, Exit settings mode
		if(control_button_pressed == NORMAL_PRESSED)
		{
			PRINT ("Stored all Configuration \r\n");
			PRINT ("==========================\r\n");
			PRINT ("Exit Configuration Mode\r\n");
			PRINT ("==========================\r\n");
			// Move to Next state
			config_state ++;
			control_button_pressed = NONE;
		}

	break;

    case 7: // Long Press, Switch OFF
		if(control_button_pressed == LONG_PRESSED)
		{
			PRINT ("==========================\r\n");
			PRINT ("Power OFF\r\n");
			PRINT ("==========================\r\n");
            led_off();

			// Move to initial state
			config_state = 1;
			control_button_pressed = NONE;
		}

	break;

	default:
		config_state = 1;
	break;
	}
}

/**
 *
 */
void app_patient_task(void)
{
	button_patient_poll();

	if(patient_button_pressed == NONE)
	{
		return;
	}

	switch(patient_config_state)
	{
	case 1: // Patient button Pressed
		if((patient_button_pressed == LONG_PRESSED) || (patient_button_pressed == SHORT_PRESSED))
		{
			PRINT ("===========\r\n");
			PRINT ("  Start PWM\r\n");
			PRINT ("===========\r\n");
			// Move to Next state
			patient_config_state++;
			patient_button_pressed = NONE;
			stop_config_task = true;

			// Enable PWM
//			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);

		}
		break;
	case 2:
		if((patient_button_pressed == LONG_PRESSED) || (patient_button_pressed == SHORT_PRESSED))
		{
			PRINT ("===========\r\n");
			PRINT ("  STOP PWM\r\n");
			PRINT ("===========\r\n");
			// Move to Init State
			patient_config_state = 1;
			patient_button_pressed = NONE;
			stop_config_task = false;

			// Disable PWM
//			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
			HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_1);

		}

	}

	if(pwm_enable){

	}
}

// TODO
// Define RTOS Tasks
// Replace the Soft timer with OS Ticks
