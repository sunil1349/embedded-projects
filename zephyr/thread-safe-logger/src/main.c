#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "logger.h"

#define STACK_SIZE 1024

#define HEARTBEAT_PRIORITY 5
#define SENSOR_PRIORITY    5
#define CLI_PRIORITY       5

static void heartbeat_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    while (1) {
        logger_info("Heartbeat thread alive");
        k_msleep(1000);
    }
}

static void sensor_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    while (1) {
        logger_info("Sensor thread: temperature sample captured");
        k_msleep(1500);
    }
}

static void cli_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    while (1) {
        logger_warn("CLI thread waiting for command");
        k_msleep(2300);
    }
}

K_THREAD_DEFINE(heartbeat_tid,
                STACK_SIZE,
                heartbeat_thread,
                NULL, NULL, NULL,
                HEARTBEAT_PRIORITY,
                0,
                0);

K_THREAD_DEFINE(sensor_tid,
                STACK_SIZE,
                sensor_thread,
                NULL, NULL, NULL,
                SENSOR_PRIORITY,
                0,
                0);

K_THREAD_DEFINE(cli_tid,
                STACK_SIZE,
                cli_thread,
                NULL, NULL, NULL,
                CLI_PRIORITY,
                0,
                0);

int main(void)
{
    logger_init();

    printk("\nThread-safe logger demo started\n");

    while (1) {
        k_msleep(5000);
    }

    return 0;
}