#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define UART_BUF_SIZE 128

struct ring_buffer
{
    uint8_t data[UART_BUF_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint32_t overflow_count;
};

void ring_buffer_init(struct ring_buffer *rb);
bool ring_buffer_push(struct ring_buffer *rb, uint8_t data);
bool ring_buffer_pop(struct ring_buffer *rb, uint8_t *data);
bool ring_buffer_is_empty(struct ring_buffer *rb);
bool ring_buffer_is_full(struct ring_buffer *rb);

#endif