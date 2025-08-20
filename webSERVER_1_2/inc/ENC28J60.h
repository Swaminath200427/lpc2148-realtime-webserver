/*#ifndef ENC28J60_H
#define ENC28J60_H

#include <stdint.h>

// Define constants and data structures
#define ENC28J60_MAC_ADDRESS {0x00, 0x12, 0x34, 0x56, 0x78, 0x9A}

 Function prototypes
void enc28j60_init(void);
void enc28j60_reset(void);
void enc28j60_read_op(uint8_t op, uint16_t len, uint8_t *data);
void enc28j60_write_op(uint8_t op, uint16_t len, uint8_t *data);
uint16_t enc28j60_read_op_len(uint8_t op);
uint16_t enc28j60_read_memif(uint16_t len, uint8_t *data);
void enc28j60_write_memif(uint16_t len, uint8_t *data);
uint16_t enc28j60_read_reg(uint8_t reg);
void enc28j60_write_reg(uint8_t reg, uint8_t data);

// Add more functions as needed

#endif // ENC28J60_H
*/
#ifndef ENC28J60_H
#define ENC28J60_H

#include <stdint.h>
#include "lpc214x.h"

// LPC2148 Pin Definitions
#define P0_4    4
#define P0_5    5
#define P0_6    6
#define P0_10   10

// Pin Configurations
#define ENC28J60_CS_PIN     P0_10
#define ENC28J60_SCK_PIN    P0_4
#define ENC28J60_MOSI_PIN   P0_5
#define ENC28J60_MISO_PIN   P0_6

/* Register Definitions for Port Operations
#define IOSET0          (*(volatile unsigned long *)(0xE0028004))
#define IOCLR0          (*(volatile unsigned long *)(0xE0028008))
#define IODIR0          (*(volatile unsigned long *)(0xE0028008))
#define PINSEL0         (*(volatile unsigned long *)(0xE002C000))
#define PINSEL1         (*(volatile unsigned long *)(0xE002C004))
*/
// Register Definitions for GPIO Port 0
#define IODIR0          (*(volatile unsigned long *)(0xE0028000))
#define IOSET0          (*(volatile unsigned long *)(0xE0028004))
#define IOCLR0          (*(volatile unsigned long *)(0xE0028008))

// PIN Select registers
#define PINSEL0         (*(volatile unsigned long *)(0xE002C000))
#define PINSEL1         (*(volatile unsigned long *)(0xE002C004))

// Power Control for Peripherals (PCONP) register
#define PCONP           (*(volatile unsigned long *)(0xE01FC0C4))

/* ENC28J60 Initialization function
void ENC28J60_init() {
    // Enable power to GPIO
    PCONP |= (1 << 15);

    // Configure the necessary PINSEL registers (uncomment if confirmed not causing access violation)
    // PINSEL0 |= (1 << X); // Set specific bits as needed for ENC28J60 SPI configuration
    // PINSEL1 |= (1 << Y); // Same as above
}*/

// Macros for pin operations
#define IOSET           IOSET0
#define IOCLR           IOCLR0

// ENC28J60 Operation Codes
#define ENC28J60_READ_CTRL_REG   0x00
#define ENC28J60_READ_BUF_MEM    0x3A
#define ENC28J60_WRITE_CTRL_REG  0x40
#define ENC28J60_WRITE_BUF_MEM   0x7A
#define ENC28J60_BIT_FIELD_SET   0x80
#define ENC28J60_BIT_FIELD_CLR   0xA0
#define ENC28J60_SOFT_RESET      0xFF

// ENC28J60 Register Definitions
#define ECON1    0x1F
#define ECON2    0x1E
#define ESTAT    0x1D
#define EIR      0x1C
#define EIE      0x1B
#define ETXST    0x04
#define ETXND    0x05
#define ERXST    0x08
#define ERXND    0x09
#define ERXRDPT  0x0A
#define ERXWRPT  0x0C
#define EDMAST   0x10
#define EDMAND   0x11
#define EDMADST  0x12
#define EDMACS   0x13

// MAC Address Definition
#define ENC28J60_MAC_ADDRESS {0x00, 0x12, 0x34, 0x56, 0x78, 0x9A}
extern void ENC28J60_init(void);
extern void ENC28J60_init_pins(void);

// SPI communication functions
extern void SPI_Init(void);
extern uint8_t SPI_Transfer(uint8_t data);

// Register access functions
extern uint8_t ENC28J60_ReadReg(uint8_t addr);
extern void ENC28J60_WriteReg(uint8_t addr, uint8_t data);
extern void ENC28J60_SetBits(uint8_t addr, uint8_t mask);
extern void ENC28J60_ClearBits(uint8_t addr, uint8_t mask);

// Packet handling functions
extern void ENC28J60_SendPacket(uint8_t* data, uint16_t len);
extern uint16_t ENC28J60_ReceivePacket(uint8_t* data, uint16_t maxlen);

/* Chip select control functions
extern void ENC28J60_CS_LOW(void);
extern void ENC28J60_CS_HIGH(void);*/
// Function Prototypes
// Initialization and Reset
void enc28j60_init(void);
void enc28j60_reset(void);
void enc28j60_init_pins(void);

// Basic Operations
void enc28j60_read_op(uint8_t op, uint16_t len, uint8_t *data);
void enc28j60_write_op(uint8_t op, uint16_t len, uint8_t *data);
uint16_t enc28j60_read_op_len(uint8_t op);

// Memory Interface Operations
uint16_t enc28j60_read_memif(uint16_t len, uint8_t *data);
void enc28j60_write_memif(uint16_t len, uint8_t *data);

// Register Operations
uint16_t enc28j60_read_reg(uint8_t reg);
void enc28j60_write_reg(uint8_t reg, uint8_t data);

// Chip Select Operations
static inline void enc28j60_cs_low(void) {
    IOCLR = (1 << ENC28J60_CS_PIN);
}

static inline void enc28j60_cs_high(void) {
    IOSET = (1 << ENC28J60_CS_PIN);
}

// SPI Operations
void enc28j60_spi_init(void);
uint8_t enc28j60_spi_transfer(uint8_t data);

// Packet Operations
void enc28j60_packet_send(uint8_t* packet, uint16_t len);
uint16_t enc28j60_packet_receive(uint8_t* packet, uint16_t maxlen);

// Buffer Management
void enc28j60_set_rx_buffer(uint16_t start, uint16_t end);
void enc28j60_set_tx_buffer(uint16_t start, uint16_t end);

// Interrupt Management
void enc28j60_enable_interrupts(uint8_t mask);
void enc28j60_disable_interrupts(uint8_t mask);
uint8_t enc28j60_get_interrupt_status(void);

#endif // ENC28J60_H