#include "enc28j60_driver.h"
#include "lpc214x.h" // Include LPC2148 register definitions
#include <stdint.h>

// ENC28J60 Opcodes
#define ENC28J60_READ_CTRL_REG  0x00
#define ENC28J60_WRITE_CTRL_REG 0x40
#define ENC28J60_BIT_FIELD_SET  0x80
#define ENC28J60_BIT_FIELD_CLR  0xA0
#define ENC28J60_SOFT_RESET     0xFF

// ENC28J60 SPI Control
#define ENC28J60_CS_LOW()   IOCLR0 = (1 << 10) // Example: chip select pin (P0.10)
#define ENC28J60_CS_HIGH()  IOSET0 = (1 << 10)

// ENC28J60 buffer boundaries
#define RXSTART_INIT    0x0000 // start of RX buffer, room for 6-byte Ethernet header
#define RXSTOP_INIT     0x0BFF // end of RX buffer
#define TXSTART_INIT    0x0C00 // start of TX buffer
#define TXSTOP_INIT     0x11FF // end of TX buffer

// ENC28J60 register addresses
#define ECON1          0x1F
#define ECON2          0x1E
#define ERDPTL         0x00
#define EWRPTL         0x02
#define ETXSTL         0x04
#define ERXSTL         0x08
#define MACON1         0x40
#define MACON3         0x42
#define MIREGADR       0x14
#define MICMD          0x12
#define MISTAT         0x0A

// Function Prototypes (for internal use)
static void enc28j60_write_op(uint8_t op, uint8_t address, uint8_t data);
static uint8_t enc28j60_read_op(uint8_t op, uint8_t address);
static void enc28j60_set_bank(uint8_t address);
static void enc28j60_write(uint8_t address, uint8_t data);
static uint8_t enc28j60_read(uint8_t address);
static void enc28j60_soft_reset();
static void spi_transfer(uint8_t data);

// Initialize SPI for LPC2148
void spi_init()
{
    // Set P0.10 as GPIO for ENC28J60 CS
    IODIR0 |= (1 << 10); // Set P0.10 as output (CS)
    ENC28J60_CS_HIGH(); // Deselect ENC28J60

    // Set P0.4 (SCK), P0.5 (MISO), P0.6 (MOSI) as SPI pins
    PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12); // Select SPI for P0.4, P0.5, P0.6

    // Configure SPI
    S0SPCR = (1 << 5) | (1 << 6); // Master mode, 8-bit data
    S0SPCCR = 8; // Set SPI clock speed (PCLK / 8)
}

// Send and receive a byte over SPI
static uint8_t spi_transfer(uint8_t data)
{
    S0SPDR = data; // Load data into SPI data register
    while (!(S0SPSR & (1 << 7))) // Wait for transfer to complete
        ;
    return S0SPDR; // Return received data
}

// ENC28J60 read operation
static uint8_t enc28j60_read_op(uint8_t op, uint8_t address)
{
    ENC28J60_CS_LOW();
    spi_transfer(op | (address & 0x1F));
    uint8_t result = spi_transfer(0x00); // Dummy write to receive data
    if (address & 0x80) result = spi_transfer(0x00); // Discard dummy byte for MAC/MII registers
    ENC28J60_CS_HIGH();
    return result;
}

// ENC28J60 write operation
static void enc28j60_write_op(uint8_t op, uint8_t address, uint8_t data)
{
    ENC28J60_CS_LOW();
    spi_transfer(op | (address & 0x1F));
    spi_transfer(data);
    ENC28J60_CS_HIGH();
}

// Set ENC28J60 bank
static void enc28j60_set_bank(uint8_t address)
{
    uint8_t bank = (address & 0x60) >> 5;
    if ((enc28j60_read(ECON1) & 0x03) != bank)
    {
        enc28j60_write_op(ENC28J60_BIT_FIELD_CLR, ECON1, 0x03);
        enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, bank);
    }
}

// Read from ENC28J60 register
static uint8_t enc28j60_read(uint8_t address)
{
    enc28j60_set_bank(address);
    return enc28j60_read_op(ENC28J60_READ_CTRL_REG, address);
}

// Write to ENC28J60 register
static void enc28j60_write(uint8_t address, uint8_t data)
{
    enc28j60_set_bank(address);
    enc28j60_write_op(ENC28J60_WRITE_CTRL_REG, address, data);
}

// Soft reset the ENC28J60
static void enc28j60_soft_reset()
{
    ENC28J60_CS_LOW();
    spi_transfer(ENC28J60_SOFT_RESET);
    ENC28J60_CS_HIGH();
    // Wait for the reset to complete
    for (int i = 0; i < 10000; i++) { } // Delay for soft reset
}

// Initialize ENC28J60 Ethernet module
void enc28j60_init()
{
    // Initialize SPI
    spi_init();

    // Perform a soft reset of the ENC28J60
    enc28j60_soft_reset();

    // Wait for the reset to complete (use a delay or poll ESTAT.CLKRDY)
    while (!(enc28j60_read(ESTAT) & 0x01)); // Wait for oscillator start

    // Initialize RX buffer
    enc28j60_write(ERXSTL, RXSTART_INIT & 0xFF);
    enc28j60_write(ERXSTH, RXSTART_INIT >> 8);
    enc28j60_write(ERXNDL, RXSTOP_INIT & 0xFF);
    enc28j60_write(ERXNDH, RXSTOP_INIT >> 8);

    // Enable the MAC and enable reception
    enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, 0x04); // Set RXEN

    // Configure MAC settings (for example: MACON1, MACON3)
    enc28j60_write(MACON1, 0x0D); // Enable MAC
    enc28j60_write(MACON3, 0x31); // Enable automatic padding, CRC, and frame length check
}

// Set MAC address
void set_mac_address(unsigned char mac[6])
{
    // Write MAC address to ENC28J60 registers
    enc28j60_write(MAADR5, mac[0]);
    enc28j60_write(MAADR4, mac[1]);
    enc28j60_write(MAADR3, mac[2]);
    enc28j60_write(MAADR2, mac[3]);
    enc28j60_write(MAADR1, mac[4]);
    enc28j60_write(MAADR0, mac[5]);
}

// Set IP address (this is typically done with a higher-level network stack)
void set_ip_address(const char *ip)
{
    // This will be handled by a network stack (e.g., lwIP) in actual usage
}
