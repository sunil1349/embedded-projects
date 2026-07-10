#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE  DT_ALIAS(sw0)

static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static const struct gpio_dt_spec button =
    GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_cb_data;
static struct k_work button_work;

static void button_work_handler(struct k_work *work)
{
    ARG_UNUSED(work);

    gpio_pin_toggle_dt(&led);
    printk("Workqueue: button event processed, LED toggled\n");
}

static void button_isr_callback(const struct device *dev,
                                struct gpio_callback *cb,
                                uint32_t pins)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(cb);
    ARG_UNUSED(pins);

    k_work_submit(&button_work);
}

int main(void)
{
    int ret;

    printk("\nWorkqueue demo started\n");

    if (!gpio_is_ready_dt(&led)) {
        printk("LED GPIO not ready\n");
        return 0;
    }

    if (!gpio_is_ready_dt(&button)) {
        printk("Button GPIO not ready\n");
        return 0;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure LED\n");
        return 0;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        printk("Failed to configure button\n");
        return 0;
    }

    k_work_init(&button_work, button_work_handler);

    gpio_init_callback(&button_cb_data,
                       button_isr_callback,
                       BIT(button.pin));

    gpio_add_callback(button.port, &button_cb_data);

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        printk("Failed to configure button interrupt\n");
        return 0;
    }

    printk("Press button on GPIO18\n");

    while (1) {
        k_msleep(5000);
    }

    return 0;
}