#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include "main.h"
#include "cmsis_os.h"
#include "debug.h"
#include "soft_timer.h"
#include "knob.h"
#include "button.h"
#include "ventilator.h"

extern UART_HandleTypeDef huart1;

uint8_t debug_command_index;
char debug_command [MAX_DEBUG_COMMAND_LENGTH];
bool is_debug_command_received;

/**
 *  Print Debug Messages
 */
void debug_print(const char *format, ...)
{
  va_list arg;
  char sMessage[MAX_LOG_MESSAGE_SIZE];

  va_start (arg, format);
  if (vsnprintf(sMessage, MAX_LOG_MESSAGE_SIZE, format, arg) > 0) {
        HAL_UART_Transmit(&huart1, (uint8_t *)sMessage, strlen(sMessage), 500);
  }
  va_end (arg);
  
}

/**
 *  Clear Debug Command Variables
 */
void debug_clear_variables(void)
{
    memset (debug_command, 0x00, MAX_DEBUG_COMMAND_LENGTH);
    debug_command_index = 0;
    is_debug_command_received = false;
}

/**
 *  Debug help menu
 */
void debug_help(void)
{
    PRINT("\r\nDEBUG HELP MENU \r\n");
    PRINT("help - Display the help menu \r\n");
    PRINT("knob - Read the current Knob value \r\n");
    PRINT("gtimer - Get the current soft timer Count \r\n");
    PRINT("ttrigger - Read the state of the trigger button \r\n");
    PRINT("tcontrol - Read the state of the control button \r\n");
    PRINT("tpatient - Read the state of the patient button \r\n");
    PRINT("settings - Display the current Ventilator settings \r\n");
}

/**
 *  Handle Debug Commands
 */
void debug_process(void)
{
    if (!is_debug_command_received) {
        return;
    }

    if (!stricmp (debug_command, "help")) 
    {
        PRINT ("Welcome to Debug Test\r\n");
        debug_help();
    } 
    else if (!stricmp (debug_command, "knob")) 
    {
        knob_start();
        // Conversion takes 3 cycles. So use a crude wait
        osDelay(1000);
        
        PRINT ("Knob value: %u\r\n", knob_get());
    } 
    else if (!stricmp (debug_command, "gtimer")) 
    {
        PRINT ("Current Timer Count: %u\r\n", soft_timer_get_count());
    }
    else if (!stricmp (debug_command, "ttrigger")) 
    {
        PRINT ("Trigger Button State: %u\r\n", button_raw_read(0));
    }
    else if (!stricmp (debug_command, "tcontrol")) 
    {
        PRINT ("Control Button State: %u\r\n", button_raw_read(1));
    }
    else if (!stricmp (debug_command, "tpatient")) 
    {
        PRINT ("Patient Button State: %u\r\n", button_raw_read(2));
    }
    else if (!stricmp (debug_command, "settings")) 
    {
        PRINT ("Current Ventilator Settings\r\n");
        ventilator_disp_setings();
    } 
    else 
    {
        PRINT ("Unknown Debug Command\r\n");
        PRINT(debug_command);
    }

    debug_clear_variables();
}

/**
 *  Initialize the Debug Module
 */
void debug_init(void)
{
    debug_clear_variables();
}

/**
 *  Debug Port Receive Interrupt Handler
 */
void debug_rx_handler(uint8_t data)
{
    if (is_debug_command_received) {
        // A command is already in progress. Ignore this one
        return;
    }

    debug_command[debug_command_index ++] = (char) data;

    // Check EOL
    if ('.' == data) {
        debug_command[debug_command_index - 1] = '\0';

        // Complete command received. Trigger the handler
        is_debug_command_received = true;
    }
}