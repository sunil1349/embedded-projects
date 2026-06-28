#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define BUTTON_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec button =
    GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

int main(void)
{
    int last_state = -1;

    if (!gpio_is_ready_dt(&button)) {
        printk("Button GPIO device is not ready\n");
        return 0;
    }

    gpio_pin_configure_dt(&button, GPIO_INPUT);

    printk("Button polling started\n");

    while (1) {
        int state = gpio_pin_get_dt(&button);

        if (state != last_state) {
            if (state == 1) {
                printk("Button Pressed\n");
            } else {
                printk("Button Released\n");
            }

            last_state = state;
        }

        k_msleep(50);
    }

    return 0;
}