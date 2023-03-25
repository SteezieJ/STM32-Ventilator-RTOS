#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "debug.h"
#include "soft_timer.h"
#include "knob.h"
#include "button.h"
#include "ventilator.h"

uint32_t tidal_volume_Vt;
uint32_t respiration_rate_RR;
uint32_t minute_ventialtion_Ve;
uint32_t peep;
uint32_t peak_flow_rate_Qmax;
uint32_t total_cycle_time_TCT;

void ventilator_disp_setings(void)
{
    PRINT ("Tidal Volume (Vt): %u mL/breath\r\n", tidal_volume_Vt);
    PRINT ("Respiratory Rate (RR): %u breaths/min\r\n", respiration_rate_RR);
    PRINT ("Flow Rate (Qmax): %uL/min\r\n", peak_flow_rate_Qmax);
}

void ventilator_calc_vt(uint32_t knob_value)
{
    /*!
        ADC to Vt
        0 to 3.3V == VT_MIN to VT_MAX
        ADC data register is RIGHT aligned. So the data mask is 0x0FFF i.e. 4095
        VT Max == Max ADC i.e. 3.3V == Value 4095
        VT Min == Min ADC i.e. 0V == Value 0
        So,
        rpm = ((knob value + 1) * VT_MAX) / 4095
        Note: We add a +1 to the adc value as a correction factor
     */
    tidal_volume_Vt = ((knob_value + 1) * VT_MAX) / ADC_MAX;

    PRINT ("Tidal Volume (Vt): %u mL/breath\r\n", tidal_volume_Vt);
}

void ventilator_calc_rr(uint32_t knob_value)
{
    /*!
        ADC to Vt
        0 to 3.3V == RR_MIN to RR_MAX
        ADC data register is RIGHT aligned. So the data mask is 0x0FFF i.e. 4095
        RR Max == Max ADC i.e. 3.3V == Value 4095
        RR Min == Min ADC i.e. 0V == Value 0
        So,
        rpm = ((knob value + 1) * RR_MAX) / 4095
        Note: We add a +1 to the adc value as a correction factor
     */
    respiration_rate_RR = ((knob_value + 1) * RR_MAX) / ADC_MAX;

    PRINT ("Respiratory Rate (RR): %u breaths/min\r\n", respiration_rate_RR);
}

void ventilator_calc_qmax(uint32_t knob_value)
{
    /*!
        ADC to Vt
        0 to 3.3V == FLOW_RATE_MIN to FLOW_RATE_MAX
        ADC data register is RIGHT aligned. So the data mask is 0x0FFF i.e. 4095
        Flow Rate Max == Max ADC i.e. 3.3V == Value 4095
        Flow Rate Min == Min ADC i.e. 0V == Value 0
        So,
        rpm = ((knob value + 1) * FLOW_RATE_MAX) / 4095
        Note: We add a +1 to the adc value as a correction factor
     */
    peak_flow_rate_Qmax = ((knob_value + 1) * FLOW_RATE_MAX) / ADC_MAX;

    PRINT ("Flow Rate (Qmax): %uL/min\r\n", peak_flow_rate_Qmax);
}