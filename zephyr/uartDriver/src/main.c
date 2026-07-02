#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "uart_driver.h"
#include "command_parser.h"
#include "led_driver.h"

int main(void)
{
    uint8_t c;

    if (uart_driver_init() != 0) {
        printk("UART driver init failed\n");
        return 0;
    }

    if (led_driver_init() != 0) {
        printk("LED driver init failed\n");
    }

    command_parser_init();

    while (1) {
        if (uart_driver_get_char(&c)) {
            command_parser_process_char((char)c);
        }
    }

    return 0;
}