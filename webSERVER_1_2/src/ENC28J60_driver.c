#include "lpc214x.h"
#include "ENC28J60.h"
#include <windows.h>
#include <stdio.h>  // Include for printf

// Define pin connections
#define ENC28J60_CS   P0_10
#define ENC28J60_SCK  P0_4
#define ENC28J60_MOSI P0_5
#define ENC28J60_MISO P0_6

// Define ENC28J60 opcodes
#define ENC28J60_SOFT_RESET  0xFF
#define ENC28J60_READ_CTRL   0x00
#define ENC28J60_WRITE_CTRL  0x40
#define ENC28J60_BIT_FIELD   0x80
#define ENC28J60_BIT_SET     0xA0
#define ENC28J60_BIT_CLR     0xC0

// Define ENC28J60 registers
#define ECON1    0x1F
#define ECON2    0x1E
#define ESTAT    0x1D
#define MICMD    0x12
#define MIREGADR 0x14
#define MIWRL    0x16
#define MIWRH    0x17
#define MIRDL    0x18
#define MIRDH    0x19
#define MAADR1   0x04
#define MAADR2   0x05
#define MAADR3   0x02
#define MAADR4   0x03
#define MAADR5   0x00
#define MAADR6   0x01
#define ERXST    0x08
#define ERXND    0x09
#define ERXWRPT  0x0C
#define ERXRDPT  0x0A
#define ETXST    0x04
#define ETXND    0x05
#define EIE      0x1B
#define EIR      0x1C

// Utility functions for chip select
static inline void ENC28J60_CS_LOW(void) {
    IOCLR = (1 << ENC28J60_CS);
}

static inline void ENC28J60_CS_HIGH(void) {
    IOSET = (1 << ENC28J60_CS);
}

// SPI functions
void SPI_Init(void) {
    // Configure SPI pins
    PINSEL0 |= (1 << 8) | (1 << 10) | (1 << 12); // Configure P0.4, P0.5, P0.6 as SPI
    
    // Configure SPI
    S0SPCR = (1 << 5) |  // Master mode
             (0 << 3) |  // CPHA = 0
             (0 << 4);   // CPOL = 0
    
    // Set SPI clock rate (PCLK / 8)
    S0SPCCR = 8;
}

uint8_t SPI_Transfer(uint8_t data) {
    S0SPDR = data;
    while (!(S0SPSR & (1 << 7))); // Wait for transfer complete
    return S0SPDR;
}

// ENC28J60 low level functions
uint8_t ENC28J60_ReadReg(uint8_t addr) {
    uint8_t data;
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_READ_CTRL | (addr & 0x1F));
    data = SPI_Transfer(0xFF);
    ENC28J60_CS_HIGH();
    return data;
}

void ENC28J60_WriteReg(uint8_t addr, uint8_t data) {
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_WRITE_CTRL | (addr & 0x1F));
    SPI_Transfer(data);
    ENC28J60_CS_HIGH();
}

void ENC28J60_SetBits(uint8_t addr, uint8_t mask) {
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_BIT_SET | (addr & 0x1F));
    SPI_Transfer(mask);
    ENC28J60_CS_HIGH();
}

void ENC28J60_ClearBits(uint8_t addr, uint8_t mask) {
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_BIT_CLR | (addr & 0x1F));
    SPI_Transfer(mask);
    ENC28J60_CS_HIGH();
}

// Initialize GPIO pins for ENC28J60
void ENC28J60_init_pins() {
    // Configure CS as GPIO output
    PINSEL0 &= ~(3 << 20);  // P0.10 as GPIO
    IODIR0 |= (1 << ENC28J60_CS);  // Set as output
    ENC28J60_CS_HIGH();  // Deselect chip
}

uint8_t ENC28J60_CheckStatus() {
    return ENC28J60_ReadReg(ESTAT);  // Check if ESTAT returns expected values
}

// Initialize ENC28J60
__declspec(dllexport) void ENC28J60_init() {
    // Initialize pins and SPI
    ENC28J60_init_pins();
    SPI_Init();
    
    // Perform soft reset
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_SOFT_RESET);
    ENC28J60_CS_HIGH();
    
    // Wait for reset to complete
    for(volatile uint32_t i = 0; i < 50000; i++);
    
    uint8_t status = ENC28J60_CheckStatus();
    if (status == 0xFF || status == 0x00) {
        printf("Memory mapping error detected, status: 0x%02X\n", status);
        return;
    }

    // Initialize receive buffer
    ENC28J60_WriteReg(ERXST, 0x00);    // Receive buffer start
    ENC28J60_WriteReg(ERXND, 0x0F);    // Receive buffer end
    ENC28J60_WriteReg(ERXRDPT, 0x00);  // Read pointer
    ENC28J60_WriteReg(ERXWRPT, 0x00);  // Write pointer
    
    // Initialize MAC address
    ENC28J60_WriteReg(MAADR1, 0x00);
    ENC28J60_WriteReg(MAADR2, 0x12);
    ENC28J60_WriteReg(MAADR3, 0x34);
    ENC28J60_WriteReg(MAADR4, 0x56);
    ENC28J60_WriteReg(MAADR5, 0x78);
    ENC28J60_WriteReg(MAADR6, 0x9A);
    
    // Configure receive filters
    // Enable unicast and broadcast
    ENC28J60_WriteReg(0x00, 0x80);
    
    // Initialize PHY
    // Enable auto-negotiation
    ENC28J60_WriteReg(MIREGADR, 0x00);
    ENC28J60_WriteReg(MIWRL, 0x01);
    ENC28J60_WriteReg(MIWRH, 0x00);
    
    // Enable interrupts
    ENC28J60_WriteReg(EIE, 0xC0);  // Enable receive and link status change interrupts
    
    // Enable packet reception
    ENC28J60_SetBits(ECON1, 0x04);
}

// Example packet transmission function
void ENC28J60_SendPacket(uint8_t* data, uint16_t len) {
    // Wait for any previous transmission to complete
    while(ENC28J60_ReadReg(ECON1) & 0x08);
    
    // Set the write pointer to start of transmit buffer
    ENC28J60_WriteReg(ETXST, 0x10);
    ENC28J60_WriteReg(ETXND, 0x10 + len);
    
    // Write per-packet control byte
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_WRITE_CTRL | 0x00);
    SPI_Transfer(0x00);  // No special control flags
    
    // Copy the packet into the transmit buffer
    for(uint16_t i = 0; i < len; i++) {
        SPI_Transfer(data[i]);
    }
    ENC28J60_CS_HIGH();
    
    // Start transmission
    ENC28J60_SetBits(ECON1, 0x08);
}

// Example packet reception function
uint16_t ENC28J60_ReceivePacket(uint8_t* data, uint16_t maxlen) {
    uint16_t len = 0;
    uint8_t header[2];
    
    // Check if packet received
    if(!(ENC28J60_ReadReg(EIR) & 0x40)) {
        return 0;
    }
    
    // Read packet header
    ENC28J60_CS_LOW();
    SPI_Transfer(ENC28J60_READ_CTRL | 0x00);
    header[0] = SPI_Transfer(0xFF);
    header[1] = SPI_Transfer(0xFF);
    len = (header[1] << 8) | header[0];
    
    // Read packet data
    if(len > maxlen) len = maxlen;
    for(uint16_t i = 0; i < len; i++) {
        data[i] = SPI_Transfer(0xFF);
    }
    ENC28J60_CS_HIGH();
    
    // Clear the packet received bit
    ENC28J60_ClearBits(EIR, 0x40);
    
    return len;
}