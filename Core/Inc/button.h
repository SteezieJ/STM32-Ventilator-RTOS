#ifndef __BUTTON_H
#define __BUTTON_H

    #define BUTTON_POLL_TIME                5
    #define PRESS_DEBOUNCE_TIME             10
    #define RELEASE_DEBOUNCE_TIME           100

    #define LONG_PRESS_TIME                 4000 // Greater than this is considered long press
    #define NORMAL_PRESS_TIME               1000 // Greater than this 
    #define SHORT_PRESS_TIME                500  // Greater than this and less than normal press is considered short press

    typedef enum {RELEASED = 1, PRESSED = 0} BSTATE_T;

    typedef enum {NONE = 0, SHORT_PRESSED, NORMAL_PRESSED, LONG_PRESSED} BPRESSSTATE_T;

    typedef struct {
        uint8_t id;
        BSTATE_T button_state;
        bool button_changed;
        uint16_t debounce_time;
        uint32_t press_time;
        uint32_t release_time;
        uint32_t button_time;
    } BUTTON_T;

    extern int control_button_pressed;
    extern int patient_button_pressed;


    GPIO_PinState button_raw_read(uint8_t button_id);
    uint32_t button_get_press_time(uint8_t button_id);
    void button_test(uint8_t button_id);
    void button_poll();
    void button_patient_poll();
    void button_init(void);

#endif /* __BUTTON_H */
