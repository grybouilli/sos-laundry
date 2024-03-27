#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RB_SIZE 32

struct ring_buffer {
    char buffer[RB_SIZE];

    uint8_t read;
    uint8_t write;
};

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

int main(void) {
    struct ring_buffer test;

    //  ---------------  rb_init test  ----------------------
    rb_init(&test);
    printf("rb_init test : %s\n", !test.read && !test.write ? "OK" : "not OK");

    //  ---------------  rb_is_empty test  ----------------------
    printf("rb_is_empty test 1 : %s\n", rb_is_empty(&test) ? "OK" : "not OK");

    //  ---------------  rb_put test  ----------------------
    char *strtest = "hello";
    size_t sz = strlen(strtest);
    for (int i = 0; i < sz; ++i) {
        rb_put(&test, strtest[i]);
        printf("rb_is_empty test %d : %s\n", i + 2,
               !rb_is_empty(&test) ? "OK" : "not OK");
    }

    uint8_t puttest = 1;

    for (int i = 0; i < sz; ++i) {
        puttest = puttest && (strtest[i] == test.buffer[i]);
    }

    puttest = puttest && test.read == 0 && test.write == sz;

    printf("rb_put test 1 : %s\n", puttest ? "OK" : "not OK");

    //  ---------------  rb_read test  ----------------------
    uint8_t readtest = 1;
    uint8_t i = 0;
    while (!rb_is_empty(&test)) {
        readtest = readtest && rb_read(&test) == strtest[i++];
        readtest = readtest && test.read == i;
    }

    printf("rb_read test : %s\n", readtest ? "OK" : "not OK");

    //  ---------------  rb_is_full test  ----------------------
    sz = RB_SIZE;

    for (int i = 0; i < sz; ++i) {
        rb_put(&test, 'c');
    }
    printf("rb_is_full test : %s\n", rb_is_full(&test) ? "OK" : "not OK");

    printf("rb_put test 2 : %s\n", !rb_put(&test, 'c') ? "OK" : "not OK");

    while (!rb_is_empty(&test)) {
        rb_read(&test);
    }

    printf("rb_read test 2 : %s\n",
           (void *)rb_read(&test) == NULL ? "OK" : "not OK");
    return 0;
}
