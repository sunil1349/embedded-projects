#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

int uart_driver_init(void);

bool uart_driver_get_char(uint8_t *ch);

void uart_driver_send_char(uint8_t ch);

void uart_driver_send_string(const char *str);

#endif