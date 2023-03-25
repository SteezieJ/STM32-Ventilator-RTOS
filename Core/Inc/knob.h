#ifndef __KNOB_H
#define __KNOB_H

    void knob_init(void);
    void knob_start(void);
    void knob_stop(void);
    uint32_t knob_get(void);

    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
    void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);

#endif /* __KNOB_H */
