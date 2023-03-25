#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "soft_timer.h"
#include "button.h"
#include "debug.h"

BUTTON_T button [] = {
    {0, RELEASED, false, PRESS_DEBOUNCE_TIME},
    {1, RELEASED, false, PRESS_DEBOUNCE_TIME},
    {2, RELEASED, false, PRESS_DEBOUNCE_TIME}
};

int control_button_pressed = NONE;
int patient_button_pressed = NONE;

GPIO_PinState button_raw_read(uint8_t button_id)
{
    GPIO_PinState button_raw_state = GPIO_PIN_RESET;

    switch (button_id)
    {
        case 0:
            button_raw_state = HAL_GPIO_ReadPin (TRIGGER_BUTTON_GPIO_Port, TRIGGER_BUTTON_Pin);
            break;

        case 1:
            button_raw_state = HAL_GPIO_ReadPin (CONTROL_BUTTON_GPIO_Port, CONTROL_BUTTON_Pin);
            break;

        case 2:
            button_raw_state = HAL_GPIO_ReadPin (PATIENT_BUTTON_GPIO_Port, PATIENT_BUTTON_Pin);
            break;
    }

    return button_raw_state;
}

void button_check(uint8_t button_id)
{
    bool raw_state;

    button[button_id].button_changed = false;

    raw_state = button_raw_read(button_id);
    if (raw_state == button[button_id].button_state) 
    {
        // Set the timer which will allow a change from the current state.
        if (button[button_id].button_state) {
            button[button_id].debounce_time = RELEASE_DEBOUNCE_TIME;
        } else {
            button[button_id].debounce_time = PRESS_DEBOUNCE_TIME;
        }
    } 
    else {
        // Key has changed – wait for new state to become stable.
        button[button_id].debounce_time -= BUTTON_POLL_TIME;
        if (button[button_id].debounce_time == 0) {
            // Timer expired – accept the change.
            button[button_id].button_state = raw_state;
            button[button_id].button_changed = true;
            // PRINT("STATE CHANGED\r\n");

            // And reset the timer.
            if (button[button_id].button_state) {
                button[button_id].debounce_time = RELEASE_DEBOUNCE_TIME;
            } else {
                button[button_id].debounce_time = PRESS_DEBOUNCE_TIME;
            }
        }
    }
}

uint32_t button_get_press_time(uint8_t button_id)
{
    return button[button_id].button_time;
}

void button_calc_press_time(uint8_t button_id)
{
    if (button[button_id].release_time > button[button_id].press_time) {
        button[button_id].button_time = button[button_id].release_time - button[button_id].press_time;
    } else {
        // Timer roll over (overflow)
        button[button_id].button_time = (soft_timer_get_period() - button[button_id].press_time) + \
            button[button_id].release_time + 1;
    }
}

void button_poll(void)
{
	int button_id = 1;
//	for (int i = 1; i<=2;i++)
//	{
		button_id = 1;
		
		button_check(button_id);
		if (button[button_id].button_changed) {

			button[button_id].button_changed = false;

			if (PRESSED == button[button_id].button_state)
			{
				button[button_id].press_time = xTaskGetTickCount();
				//PRINT("PRESSED === \r\n");
				button[button_id].button_time = 0;
			}
			else if (RELEASED == button[button_id].button_state)
			{
				button[button_id].release_time = xTaskGetTickCount();
				//PRINT("RELEASED === \r\n");
				button_calc_press_time(button_id);
			}
		}
//	}


	if(button[button_id].button_time){
		PRINT("CONTROL Button Pressed: %u \r\n", button[1].button_time);
		if ((button[button_id].button_time) > LONG_PRESS_TIME){
			control_button_pressed = LONG_PRESSED;
		} else if ((button[button_id].button_time) > NORMAL_PRESS_TIME){
			control_button_pressed = NORMAL_PRESSED;
		} else if ((button[button_id].button_time) > SHORT_PRESS_TIME){
			control_button_pressed = SHORT_PRESSED;
		}
		button[button_id].button_time = 0;
	}
//	if(button[2].button_time){
//		PRINT("GREEN Button PRessed: %u \r\n", button_get_press_time(2));
//		button[2].button_time = 0;
//	}
}

void button_patient_poll()
{
	int button_id = 2;

		button_check(button_id);
		if (button[button_id].button_changed) {

			button[button_id].button_changed = false;

			if (PRESSED == button[button_id].button_state)
			{
				button[button_id].press_time = xTaskGetTickCount();//soft_timer_get_count();
				//PRINT("PRESSED === \r\n");
				button[button_id].button_time = 0;
			}
			else if (RELEASED == button[button_id].button_state)
			{
				button[button_id].release_time = xTaskGetTickCount();//soft_timer_get_count();
				//PRINT("RELEASED === \r\n");
				button_calc_press_time(button_id);
			}
		}

//	if(button[button_id].button_time){
////		PRINT("BLUE Button pressed : %u \r\n", button_get_press_time(1));
//		if((button[button_id].button_time) > 3000){
//			patient_button_pressed = LONG_PRESSED;
//		} else {
//			patient_button_pressed = SHORT_PRESSED;
//		}
//		button[button_id].button_time = 0;
//	}

	if(button[button_id].button_time){
		//PRINT("CONTROL Button Pressed: %u \r\n", button[1].button_time);
		if ((button[button_id].button_time) > LONG_PRESS_TIME){
			patient_button_pressed = LONG_PRESSED;
		} else if ((button[button_id].button_time) > NORMAL_PRESS_TIME){
			patient_button_pressed = NORMAL_PRESSED;
		} else if ((button[button_id].button_time) > SHORT_PRESS_TIME){
			patient_button_pressed = SHORT_PRESSED;
		}
		button[button_id].button_time = 0;
	}

}

/**
 *  
 */
void button_init(void)
{
    
}