/**
 * @file ring_buffer.c
 * @author grybouilli (grybouilli@outlook.fr)
 * @brief Implementation of a ring buffer for BT messages reception and sending.
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "ring_buffer.h"
#include "string.h"

/**
 * @brief Initializes the ring buffer.
 * 
 * @param rb Pointer to the ring buffer to initialize.
 */
void rb_init(struct ring_buffer *rb) {
    rb->read = 0;
    rb->write = 0;
}

/**
 * @brief Check is ring buffer is full.
 * 
 * @param rb Pointer to the ring buffer to check.
 * @return 1 if full, 0 otherwise.
 */
uint8_t rb_is_full(struct ring_buffer *rb) {
    return rb->write >= rb->read ? rb->write - rb->read == RB_SIZE - 1
                                 : rb->read - rb->write == 1;
}

/**
 * @brief Check is ring buffer is empty.
 * 
 * @param rb Pointer to the ring buffer to check.
 * @return 1 if empty, 0 otherwise.
 */
uint8_t rb_is_empty(struct ring_buffer *rb) { return rb->read == rb->write; }

/**
 * @brief Put a character into the ring buffer. If the ring buffer is full, doesn't do anything.
 * 
 * @param rb Pointer to the ring buffer to fill.
 * @param c Character to write.
 * @return 1 if c was put in rb, 0 if not.
 */
uint8_t rb_put(struct ring_buffer *rb, char c) {
    if (rb_is_full(rb))
        return 0;

    rb->buffer[rb->write] = c;
    rb->write++;
    rb->write %= RB_SIZE;

    return 1;
}

/**
 * @brief Reads a byte from a ring buffer.
 * 
 * @param rb Pointer to the ring buffer to read from.
 * @return The character read if non-empty, NULL otherwise.
 */
char rb_read(struct ring_buffer *rb) {
    if (rb_is_empty(rb))
        return NULL;
    char c = rb->buffer[rb->read++];
    rb->read %= RB_SIZE;
    return c;
}
