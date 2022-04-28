/*
 * Functions for testing various system components.
 *
 * @date 04/11/2022
 */

#include <msp430.h>

void flash_led(void);




void flash_led(void){
    P2DIR |= BIT5;
    volatile int i;

    while(1){
        P2OUT ^= BIT5; //toggle P2.5 (RED LED)
        for(i = 0; i < 100000; i++);
    }

    //THESE LINES TEST SPI
    /**
    spi_init();

    spi_clk_passive_low(); //for lsm9ds1

    uint8_t data_send_1[1] = {0b01101010}; //configuration register
    uint8_t data_receive_1[1] = {0x00};

    uint8_t data_send_2[1] = {0b10000110};
    uint8_t data_receive_2[1] = {0b00000000};

    uint8_t data_send_3[2] = {0x00, 0x00};
    uint8_t data_receive_3[2] = {0x00, 0x00};

    P2OUT &= ~BIT0; //toggle CS

    while(!spi_free()); //wait until SPI module free

    spi_start_asynch_transmission(data_send_1, data_receive_1, 1);

    while(!spi_free()); //wait until SPI module free

    P2OUT |= BIT0; //toggle CS
    volatile int i;
    for(i = 0; i < 20000; i++); //delay

    P2OUT &= ~BIT0; //toggle CS

    while(!spi_free()); //wait until SPI module free

    spi_start_asynch_transmission(data_send_2, data_receive_2, 1);

    while(!spi_free()); //wait until SPI module free

    P2OUT |= BIT0; //toggle CS

    for(i = 0; i < 20000; i++); //delay

    P2OUT &= ~BIT0; //toggle CS

    spi_start_asynch_transmission(data_send_3, data_receive_3, 2);

    while(!spi_free()); //wait until SPI module free

    P2OUT |= BIT0; //toggle CS

    while(1);
    **/


    /**THESE LINES TEST UART

    uint8_t uart_send_data[7] = { 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x10};
    uint8_t uart_receive_data[2] = {0x00, 0x00};
    volatile int test_send;
    volatile int test_receive;

    uart_init();

    test_send = uart_send_bytes(uart_send_data, 7);

    test_receive = uart_receive_bytes(uart_receive_data, 2);

    while(1); //loop infinitely
    **/



}


