#ifndef ENC28J60_H
#define ENC28J60_H

#include <stdint.h>

// Define constants and data structures
#define ENC28J60_MAC_ADDRESS {0x00, 0x12, 0x34, 0x56, 0x78, 0x9A}

// Function prototypes
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