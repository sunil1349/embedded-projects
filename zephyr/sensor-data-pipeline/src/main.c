#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

#define SENSOR_PRIORITY     5
#define PROCESSOR_PRIORITY  5

struct sensor_sample {
    int temperature;
    int humidity;
    uint32_t sample_id;
};

static struct sensor_sample latest_sample;

K_SEM_DEFINE(sample_ready_sem, 0, 1);

static void sensor_thread(void *arg1, void *arg2, void *arg3)
{
    uint32_t id = 0;

    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        latest_sample.temperature = 25 + (id % 5);
        latest_sample.humidity = 60 + (id % 10);
        latest_sample.sample_id = id;

        printk("Sensor: sample %u captured\n", id);

        k_sem_give(&sample_ready_sem);

        id++;
        k_msleep(1000);
    }
}

static void processor_thread(void *arg1, void *arg2, void *arg3)
{
    struct sensor_sample sample;

    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        k_sem_take(&sample_ready_sem, K_FOREVER);

        sample = latest_sample;

        printk("Processor: sample=%u temp=%dC humidity=%d%%\n",
               sample.sample_id,
               sample.temperature,
               sample.humidity);
    }
}

K_THREAD_DEFINE(sensor_tid,
                STACK_SIZE,
                sensor_thread,
                NULL, NULL, NULL,
                SENSOR_PRIORITY,
                0,
                0);

K_THREAD_DEFINE(processor_tid,
                STACK_SIZE,
                processor_thread,
                NULL, NULL, NULL,
                PROCESSOR_PRIORITY,
                0,
                0);

int main(void)
{
    printk("\nSensor data pipeline started\n");

    while (1) {
        k_msleep(5000);
    }

    return 0;
}