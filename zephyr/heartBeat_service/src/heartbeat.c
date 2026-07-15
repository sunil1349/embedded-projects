#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#include "heartbeat.h"

static struct k_timer heartbeat_timer;
static struct k_work heartbeat_work;

static uint32_t heartbeat_count;
static bool heartbeat_initialized;

static void heartbeat_work_handler(struct k_work *work)
{
    ARG_UNUSED(work);

    heartbeat_count++;

    printk("[HEARTBEAT] count=%u uptime=%lld ms\n",
           heartbeat_count,
           k_uptime_get());
}

static void heartbeat_timer_expiry(struct k_timer *timer)
{
    ARG_UNUSED(timer);

    /*
     * Timer callback remains short.
     * Actual heartbeat processing is deferred to thread context.
     */
    (void)k_work_submit(&heartbeat_work);
}

static void heartbeat_timer_stopped(struct k_timer *timer)
{
    ARG_UNUSED(timer);

    /*
     * Keep this callback short as well.
     */
}

int heartbeat_init(void)
{
    heartbeat_count = 0;

    k_work_init(&heartbeat_work, heartbeat_work_handler);

    k_timer_init(&heartbeat_timer,
                 heartbeat_timer_expiry,
                 heartbeat_timer_stopped);

    heartbeat_initialized = true;

    return 0;
}

int heartbeat_start(uint32_t initial_delay_ms, uint32_t period_ms)
{
    if (!heartbeat_initialized) {
        return -EACCES;
    }

    if ((initial_delay_ms == 0U) || (period_ms == 0U)) {
        return -EINVAL;
    }

    k_timer_start(&heartbeat_timer,
                  K_MSEC(initial_delay_ms),
                  K_MSEC(period_ms));

    return 0;
}

void heartbeat_stop(void)
{
    if (!heartbeat_initialized) {
        return;
    }

    k_timer_stop(&heartbeat_timer);
}

uint32_t heartbeat_get_count(void)
{
    return heartbeat_count;
}