#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

#define SENSOR_PRIORITY     5
#define PROCESSOR_PRIORITY  5

#define MSGQ_MAX_MESSAGES   10

struct sensor_sample {
    uint32_t sample_id;
    int temperature;
    int humidity;
};

K_MSGQ_DEFINE(sensor_msgq,
              sizeof(struct sensor_sample),
              MSGQ_MAX_MESSAGES,
              4);

static void sensor_thread(void *arg1, void *arg2, void *arg3)
{
    uint32_t id = 0;

    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        struct sensor_sample sample = {
            .sample_id = id,
            .temperature = 25 + (id % 5),
            .humidity = 60 + (id % 10),
        };

        if (k_msgq_put(&sensor_msgq, &sample, K_NO_WAIT) == 0) {
            printk("Sensor: queued sample %u\n", sample.sample_id);
        } else {
            printk("Sensor: queue full, dropped sample %u\n", sample.sample_id);
        }

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
        k_msgq_get(&sensor_msgq, &sample, K_FOREVER);

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
    printk("\nMessage queue demo started\n");

    while (1) {
        k_msleep(5000);
    }

    return 0;
}