#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

#define UART_BUF_SIZE 128

K_SEM_DEFINE(uart_rx_sem, 0, 1);
static const struct device *uart_dev =
    DEVICE_DT_GET(UART_DEVICE_NODE);

struct ring_buffer rx_rb;

static void uart_cb(const struct device *dev, void *user_data)
{
    uint8_t c;
    bool data_received = false;

    ARG_UNUSED(user_data);

    if (!uart_irq_update(dev)) {
        return;
    }

    if (uart_irq_rx_ready(dev)) {
        while (uart_fifo_read(dev, &c, 1) == 1) {
            ring_buffer_push(&rx_rb, c);
            data_received = true;
        }
    }
    if (data_received) {
        k_sem_give(&uart_rx_sem);
    }
}

int main(void)
{
    uint8_t c;

    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return 0;
    }

    printk("UART interrupt + ring buffer example started\n");
    printk("ISR stores bytes, main loop echoes them back\n");

    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
    uart_irq_rx_enable(uart_dev);

    while (1) {
        k_sem_take(&uart_rx_sem, K_FOREVER);
        while (ring_buffer_pop(&rx_rb, &c)) {
            uart_poll_out(uart_dev, '<');
            uart_poll_out(uart_dev, c);
            uart_poll_out(uart_dev, '>');
        }

        k_msleep(10);
    }

    return 0;
}