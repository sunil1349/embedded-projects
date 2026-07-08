#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "logger.h"

static struct k_mutex logger_mutex;

int logger_init(void)
{
    k_mutex_init(&logger_mutex);
    return 0;
}

static void logger_print(const char *level, const char *msg)
{
    k_mutex_lock(&logger_mutex, K_FOREVER);

    printk("[%s] %s\n", level, msg);

    k_mutex_unlock(&logger_mutex);
}

void logger_info(const char *msg)
{
    logger_print("INFO", msg);
}

void logger_warn(const char *msg)
{
    logger_print("WARN", msg);
}

void logger_error(const char *msg)
{
    logger_print("ERROR", msg);
}