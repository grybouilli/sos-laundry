#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ring_buffer.h>

#define RB_SIZE 32

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
