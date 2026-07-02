#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <errno.h>

#include "led_driver.h"

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int led_driver_init(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led)) {
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

void led_driver_on(void)
{
    gpio_pin_set_dt(&led, 1);
}

void led_driver_off(void)
{
    gpio_pin_set_dt(&led, 0);
}

void led_driver_toggle(void)
{
    gpio_pin_toggle_dt(&led);
}