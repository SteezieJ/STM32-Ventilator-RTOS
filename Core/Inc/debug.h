#ifndef __DEBUG_H
#define __DEBUG_H

#define MAX_DEBUG_COMMAND_LENGTH        64
#define MAX_LOG_MESSAGE_SIZE            1024
#define PRINT(x,...)                    debug_print(x,##__VA_ARGS__)


void debug_print(const char *format, ...);
void debug_clear_variables(void);
void debug_process(void);
void debug_init(void);
void debug_rx_handler(uint8_t data);

#endif /* __DEBUG_H */
