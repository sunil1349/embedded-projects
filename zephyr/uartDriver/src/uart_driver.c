#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <errno.h>

#include "uart_driver.h"
#include "ring_buffer.h"

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

static const struct device *uart_dev =
    DEVICE_DT_GET(UART_DEVICE_NODE);

static struct ring_buffer rx_rb;

K_SEM_DEFINE(uart_rx_sem, 0, 1);

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

int uart_driver_init(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return -ENODEV;
    }

    ring_buffer_init(&rx_rb);

    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
    uart_irq_rx_enable(uart_dev);

    return 0;
}

bool uart_driver_get_char(uint8_t *ch)
{
    if (ch == NULL) {
        return false;
    }

    while (!ring_buffer_pop(&rx_rb, ch)) {
        k_sem_take(&uart_rx_sem, K_FOREVER);
    }

    return true;
}

void uart_driver_send_char(uint8_t ch)
{
    uart_poll_out(uart_dev, ch);
}

void uart_driver_send_string(const char *str)
{
    if (str == NULL) {
        return;
    }

    while (*str != '\0') {
        uart_driver_send_char((uint8_t)*str);
        str++;
    }
}