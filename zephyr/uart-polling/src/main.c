#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

int main(void)
{
    unsigned char rx_char;

    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return 0;
    }

    printk("UART polling example started\n");
    printk("Type any character. ESP32 will echo it back.\n");

    while (1) {
        if (uart_poll_in(uart_dev, &rx_char) == 0) {
            //uart_poll_out(uart_dev, rx_char);
            uart_poll_out(uart_dev, '[');
            uart_poll_out(uart_dev, rx_char);
            uart_poll_out(uart_dev, ']');
            if (rx_char == '\r') {
                uart_poll_out(uart_dev, '\n');
            }
        }

        k_msleep(10);
    }

    return 0;
}