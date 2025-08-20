#include <lpc214x.h>  // Include the LPC2148 header file
#include "ENC28J60.h" // Include your ENC28J60 driver

#define SPI_CS_PIN    (1 << 7)  // Chip Select pin on P0.7
#define SPI_RESET_PIN (1 << 8)  // Reset pin on P0.8

void delay(unsigned int count) {
    while (count--);
}

void spi_init(void) {
    // Configure P0.4, P0.5, P0.6 for SPI functionality
    PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12);  // Set P0.4, P0.5, P0.6 for SCK0, MISO0, MOSI0

    // Initialize SPI0
    S0SPCR = (1 << 5) | (1 << 6); // SPI0: Master mode, CPOL=0, CPHA=0, 8-bit data transfer
    S0SPCCR = 8;                  // Set clock rate for SPI (depends on PCLK and ENC28J60 timing requirements)
}

void spi_cs_low(void) {
    IO0CLR = SPI_CS_PIN;  // Set CS pin low
}

void spi_cs_high(void) {
    IO0SET = SPI_CS_PIN;  // Set CS pin high
}

unsigned char spi_transfer(unsigned char data) {
    S0SPDR = data;                    // Load data into SPI data register
    while (!(S0SPSR & (1 << 7)));     // Wait for transfer to complete
    return S0SPDR;                    // Return received data
}

void enc28j60_init(void) {
    // Set CS and RESET pins as GPIO output
    IO0DIR |= SPI_CS_PIN | SPI_RESET_PIN; 

    // Reset the ENC28J60
    IO0CLR = SPI_RESET_PIN;   // Pull RESET pin low
    delay(10000);             // Hold RESET for some time
    IO0SET = SPI_RESET_PIN;   // Release RESET pin

    // Initialize SPI
    spi_init();

    // Start ENC28J60 initialization
    spi_cs_low();
    enc28j60_soft_reset();     // Soft reset the ENC28J60
    spi_cs_high();

    // Further initialization steps (configuring buffers, MAC, etc.)
    enc28j60_write_op(ENC28J60_WRITE_CTRL_REG, ECON1, 0x00); // Example operation to disable transmit

    // Setup MAC and PHY registers
    // Add further initialization code here based on your application needs
}

int main(void) {
    // Initialize ENC28J60
    enc28j60_init();

    while(1) {
        // Main loop
    }
}
