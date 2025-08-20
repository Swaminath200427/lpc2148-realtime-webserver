#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include "lpc214x.h"
#include "mongoose_custom_config.h"
#include "mongoose.h"
#include "mongoose_custom_impl.c"
#include "ENC28J60.h"

#define SCK_PIN   4   // P0.4 as SCK
#define MISO_PIN  5   // P0.5 as MISO
#define MOSI_PIN  6   // P0.6 as MOSI
#define CS_PIN    7   // P0.7 as Chip Select (CS)

// SPI and ENC28J60 initialization functions (as in your original code)
void spi_init() {
    // Configure SPI pins, clock speed, mode, etc.
      PINSEL0 |= (1 << 2) | (1 << 4) | (1 << 6);             // Enable P0.1, P0.3, P0.7 for SPI
    PINSEL1 |= (1 << 2);                                                         // Enable P0.2 for SPI
                        // Set SPI clock speed (adjust as needed)
    S0SPCCR = 0x0F;                                                                     // Set clock prescaler for a frequency of 1/16th of the system clock
                        // Configure SPI mode (adjust as needed)
    ;                                                                     // Master mode, CPOL = 0, CPHA = 0
                        // Enable SPI
    S0SPCR = (1 << 5) | (1 << 3);
}

/*void enc28j60_init() {
      // Reset ENC28J60, configure MAC address, network parameters
        enc28j60_write_op(ENC28J60_OP_SOFT_RESET, 0, NULL);
    while (enc28j60_read_op(ENC28J60_OP_READ_STATUS, 0, NULL) & ENC28J60_STATUS_MII_BUSY);
                        // Set MAC address
    uint8_t mac_addr[] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x9A};
    enc28j60_write_op(ENC28J60_OP_WRITE_MACADDR, 6, mac_addr);
                        // Configure bank 0
    enc28j60_write_op(ENC28J60_OP_READ_BANK, 0, NULL);
    enc28j60_write_op(ENC28J60_OP_WRITE_MEMIF, 4, (uint8_t[]){0x77, 0x00, 0x00, 0x00});
                        // Configure bank 1
    enc28j60_write_op(ENC28J60_OP_READ_BANK, 1, NULL);
    enc28j60_write_op(ENC28J60_OP_WRITE_MEMIF, 16, (uint8_t[]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
                        // Enable Ethernet interface
    enc28j60_write_op(ENC28J60_OP_READ_BANK, 0, NULL);
    enc28j60_write_op(ENC28J60_OP_WRITE_MEMIF, 1, (uint8_t[]){0x01});
}*/

// Additional functions for SPI communication
void spi_write_byte(uint8_t data) {
      // Send a byte of data over SPI
        SPDR = data;
    while (!(SPSR & (1 << 1)));
}

uint8_t spi_read_byte() {
    // Read a byte of data from SPI
        SSPDR = 0xFF;
    while (!(SPSR & (1 << 1))); // Wait for transmission complete
    return SPDR;
}

// ... other functions for sending commands and receiving responses

// Mongoose event handler
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/")) {
            mg_http_reply(c, 200, "Content-Type: text/html\r\n", "<h1>Hello, World!</h1>");
        } else {
            mg_http_reply(c, 404, "", "Not Found");
        }
    }
}

// Mongoose timer callback
static void timer_fn(void *arg) {
    // Perform periodic tasks here
}

int main() {
    struct mg_mgr mgr;
    struct mg_connection *c;
    struct mg_timer timer;

    // Initialize hardware
    spi_init();
    enc28j60_init();

    // Initialize Mongoose
    mg_mgr_init(&mgr);

    // Create listening connection
    c = mg_http_listen(&mgr, "0.0.0.0:80", fn, NULL);
    if (c == NULL) {
        printf("Failed to create listener\n");
        return 1;
    }

    // Set up timer
    mg_timer_init(&timer, 1000, MG_TIMER_REPEAT, timer_fn, &mgr);

    // Main event loop
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
        // Perform any other necessary tasks
    }

    // Cleanup
    mg_mgr_free(&mgr);

    return 0;
}