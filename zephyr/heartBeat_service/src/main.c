#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "heartbeat.h"

int main(void)
{
    int ret;

    printk("\nHeartbeat service started\n");

    ret = heartbeat_init();
    if (ret != 0) {
        printk("Heartbeat initialization failed: %d\n", ret);
        return 0;
    }

    ret = heartbeat_start(1000U, 1000U);
    if (ret != 0) {
        printk("Heartbeat start failed: %d\n", ret);
        return 0;
    }

    printk("Heartbeat period: 1000 ms\n");

    while (1) {
        k_msleep(5000);

        printk("[MAIN] processed heartbeats=%u\n",
               heartbeat_get_count());
    }

    return 0;
}