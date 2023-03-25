#ifndef __VENTILATOR_H
#define __VENTILATOR_H

    #define VT_MIN          500
    #define VT_MAX          750
    #define RR_MIN          12
    #define RR_MAX          20
    #define FLOW_RATE_MIN   60
    #define FLOW_RATE_MAX   120
    #define ADC_MAX         4095
    #define ADC_MIN         0


    void ventilator_disp_setings(void);
    void ventilator_calc_vt(uint32_t knob_value);
    void ventilator_calc_rr(uint32_t knob_value);
    void ventilator_calc_qmax(uint32_t knob_value);

#endif /* __VENTILATOR_H */
