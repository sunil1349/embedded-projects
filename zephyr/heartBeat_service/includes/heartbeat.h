#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdint.h>

/**
 * @brief Initialize the heartbeat service.
 *
 * Initializes the timer and work item used by the service.
 *
 * @return 0 on success.
 */
int heartbeat_init(void);

/**
 * @brief Start periodic heartbeat generation.
 *
 * @param initial_delay_ms Delay before the first heartbeat.
 * @param period_ms Period between heartbeat events.
 *
 * @return 0 on success, otherwise a negative error code.
 */
int heartbeat_start(uint32_t initial_delay_ms, uint32_t period_ms);

/**
 * @brief Stop heartbeat generation.
 */
void heartbeat_stop(void);

/**
 * @brief Get the number of processed heartbeat events.
 *
 * @return Current heartbeat count.
 */
uint32_t heartbeat_get_count(void);

#endif