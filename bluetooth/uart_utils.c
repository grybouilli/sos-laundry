/**
 * @file uart_utils.c
 * @author grybouilli (grybouilli@outlook.fr)
 * @brief A collection of functions to use HC Bluetooth components. UART Communication relies on a queue / ring buffer to send messages.
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "uart_utils.h"
#include "ring_buffer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

struct ring_buffer receiver;
struct ring_buffer sender;

uint16_t commands = 0;

/**
 * @brief Setup function for UART and Bluetooth communication using the HC Modules. The Module should be connected to RX/TX pins.
 * 
 *
 * @param br Baudrate for the UART communication with the BT module.
 */
void setup_bt_com(uint32_t br) {

    uint16_t ubrr = UBRR(br);

    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    UCSR0A = (1 << U2X0); // asynchronous double speed mode

    UCSR0B = (1 << RXCIE0) | (1 << TXEN0) | (1 << RXEN0);

    // Set one stop bit
    UCSR0C = (0 << USBS0);

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    rb_init(&receiver);
    rb_init(&sender);
}

/**
 * @brief Sends a byte.
 * 
 *
 * @param c The byte to send.
 */
void send_byte(uint8_t c) { rb_put(&sender, c); }

/**
 * @brief Send a message via UART / BT.
 * 
 * 
 * @param msg The message to send.
 */
void send_message(char *msg) {
    uint8_t l = strlen(msg);
    for (uint8_t c = 0; c < l; ++c)
        send_byte(msg[c]);
}

/**
 * @brief Encode message with an ending character defined by END_CHAR_STR before sending it.
 *
 * @param msg The message to encode and send.
 */
void send_encoded_message(char * msg)
{
    char encoded_msg [RB_SIZE];
    memset( encoded_msg, '\0', sizeof(char)*RB_SIZE );
    send_message(encode_message(encoded_msg, msg));
}

/**
 * @brief Encode message by adding an ending character defined by END_CHAR_STR.
 * 
 *
 * @param dst The buffer to store the encoded message. It should hold enough space for the message + the size of the ending character(s).
 * @param msg The message to encode.
 * @return A copy of dst.
 */
char * encode_message(char * dst, char * msg)
{
    strcat(dst, msg);
    strcat(dst, END_CHAR_STR);
    return dst;
}

/**
 * @brief Decode message by removing ending character(s).
 * 
 *
 * @param dst The buffer to store the decoded message. It should hold enough space for the message.
 * @param msg The encoded message.
 * @return A copy of dst.
 */
char * decode_message(char * dst, char * msg)
{
    uint8_t l = strlen(msg);
    strncpy(dst, msg, l-1);
    dst[l] = '\0';
    return dst;
}


/**
 * @brief Send all pending message stored in the ring buffer. This function is blocking and should be used precautiously.
 * 
 */
void send_pending_messsages() {
    while (!rb_is_empty(&sender)) {
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        UDR0 = rb_read(&sender);
    }
}

/**
 * @brief Read all received messages stored in dedicated ring buffer. The message are handled according to the policy given by the handler.
 * 
 * @param handler Function that treats messages.
 */
void read_pending_messages(void (*handler)(char *)) {
    cli();
    char buffer[RB_SIZE];
    memset(buffer, '\0', RB_SIZE);

    uint8_t c = 0;
    while (commands) {
        buffer[c++] = rb_read(&receiver);
        if(buffer[c-1] == END_CHAR)
        {
            handler(buffer);
            commands--;
            c = 0;
            memset(buffer, '\0', RB_SIZE);
        }
    }

    sei();
}

/**
 * @brief Reception of BT messages is done using interrupts
 * 
 */
ISR(USART_RX_vect) { 
    char c = UDR0;
    rb_put(&receiver, c); 
    if(c == END_CHAR)
    {
        commands++;
    }
}
