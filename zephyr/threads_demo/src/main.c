#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

#define HEARTBEAT_PRIORITY 5
#define COUNTER_PRIORITY   5

void heartbeat_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        printk("Heartbeat thread running\n");
        k_msleep(1000);
    }
}

void counter_thread(void *arg1, void *arg2, void *arg3)
{
    int count = 0;

    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        printk("Counter thread: %d\n", count++);
        k_msleep(2000);
    }
}

K_THREAD_DEFINE(heartbeat_tid,
                STACK_SIZE,
                heartbeat_thread,
                NULL, NULL, NULL,
                HEARTBEAT_PRIORITY,
                0,
                0);

K_THREAD_DEFINE(counter_tid,
                STACK_SIZE,
                counter_thread,
                NULL, NULL, NULL,
                COUNTER_PRIORITY,
                0,
                0);

int main(void)
{
    printk("Zephyr threads demo started\n");

    while (1) {
        printk("Main thread monitor\n");
        k_msleep(5000);
    }

    return 0;
}