#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <avr/io.h>

#define RB_SIZE 32
struct ring_buffer {
    char buffer[RB_SIZE];

    uint8_t read;
    uint8_t write;
};

void rb_init(struct ring_buffer *rb);

uint8_t rb_is_full(struct ring_buffer *rb);

uint8_t rb_is_empty(struct ring_buffer *rb);

uint8_t rb_put(struct ring_buffer *rb, char c);

char rb_read(struct ring_buffer *rb);

#endif // !_RING_BUFFER_H_
