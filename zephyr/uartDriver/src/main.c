#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

static const struct device *uart_dev =
    DEVICE_DT_GET(UART_DEVICE_NODE);

static void uart_cb(const struct device *dev, void *user_data)
{
    uint8_t c;

    ARG_UNUSED(user_data);

    if (!uart_irq_update(dev)) {
        return;
    }

    if (uart_irq_rx_ready(dev)) {
        while (uart_fifo_read(dev, &c, 1) == 1) {
            uart_poll_out(dev, '[');
uart_poll_out(dev, c);
uart_poll_out(dev, ']');
        }
    }
}

int main(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return 0;
    }

    printk("UART interrupt example started\n");
    printk("Type characters. ESP32 will echo from interrupt callback.\n");

    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);

    uart_irq_rx_enable(uart_dev);

    while (1) {
        k_msleep(1000);
    }

    return 0;
}