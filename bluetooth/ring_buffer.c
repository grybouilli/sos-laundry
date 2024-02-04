#include "ring_buffer.h"
#include "string.h"

void rb_init(struct ring_buffer *rb) {
    rb->read = 0;
    rb->write = 0;
}

uint8_t rb_is_full(struct ring_buffer *rb) {
    return rb->write >= rb->read ? rb->write - rb->read == RB_SIZE - 1
                                 : rb->read - rb->write == 1;
}

uint8_t rb_is_empty(struct ring_buffer *rb) { return rb->read == rb->write; }

uint8_t rb_put(struct ring_buffer *rb, char c) {
    if (rb_is_full(rb))
        return 0;

    rb->buffer[rb->write] = c;
    rb->write++;
    rb->write %= RB_SIZE;

    return 1;
}

char rb_read(struct ring_buffer *rb) {
    if (rb_is_empty(rb))
        return NULL;
    char c = rb->buffer[rb->read++];
    rb->read %= RB_SIZE;
    return c;
}
