#include <ring_buffer.h>






void ring_buffer_init(struct ring_buffer *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->overflow_count = 0;
}

bool ring_buffer_is_full(struct ring_buffer *rb)
{
    uint16_t next_head = (rb->head + 1) % UART_BUF_SIZE;
    return next_head == rb->tail;
}

bool ring_buffer_is_empty(struct ring_buffer *rb)
{
    return rb->head == rb->tail;
}

bool ring_buffer_push(struct ring_buffer *rb, uint8_t data)
{
    uint16_t next_head = (rb->head + 1) % UART_BUF_SIZE;

    if (next_head == rb->tail) {
        rb->overflow_count++;
        return false;
    }

    rb->data[rb->head] = data;
    rb->head = next_head;

    return true;
}

bool ring_buffer_pop(struct ring_buffer *rb, uint8_t *data)
{
    if (ring_buffer_is_empty(rb)) {
        return false;
    }

    *data = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % UART_BUF_SIZE;

    return true;
}