#include <LPC21xx.h>

// SPI Pin Definitions according to LPC2148 Datasheet
#define SCK_PIN   4   // P0.4 as SCK
#define MISO_PIN  5   // P0.5 as MISO
#define MOSI_PIN  6   // P0.6 as MOSI
#define CS_PIN    7   // P0.7 as Chip Select (CS)

// ENC28J60 Opcodes
#define ENC28J60_READ_CTRL_REG  0x00
#define ENC28J60_WRITE_CTRL_REG 0x40
#define ENC28J60_SOFT_RESET     0xFF

// Function to initialize the SPI interface
void SPI_Init(void) {
    // Set SCK, MOSI, and CS as output, MISO as input
    PINSEL0 |= (1 << SCK_PIN) | (1 << MOSI_PIN); // Configure P0.4 as SCK, P0.6 as MOSI
    PINSEL0 &= ~(1 << MISO_PIN); // Ensure P0.5 is GPIO (MISO)
    IODIR0 |= (1 << SCK_PIN) | (1 << MOSI_PIN) | (1 << CS_PIN); // Set SCK, MOSI, CS as output
    IODIR0 &= ~(1 << MISO_PIN); // Set MISO as input

    // Enable SPI0, master mode, SCK idle low, MSB first
    S0SPCR = (1 << 5) | (1 << 3); // Master mode, CPOL = 0, CPHA = 0

    // Set SPI Clock: PCLK/8 (default PCLK = 60 MHz, so SPI clock = 7.5 MHz)
    S0SPCCR = 8;

    // De-assert the CS line
    IOSET0 = (1 << CS_PIN);
}

// Function to transfer data over SPI
uint8_t SPI_Transfer(uint8_t data) {
    S0SPDR = data;  // Load data to be transmitted
    while (!(S0SPSR & (1 << 7)));  // Wait for the transfer to finish (SPIF bit)
    return S0SPDR;  // Return received data
}

// Function to initialize the ENC28J60 Ethernet module
void ENC28J60_Init(void) {
    // Bring CS low to start communication
    IOCLR0 = (1 << CS_PIN);

    // Send soft reset command to ENC28J60
    SPI_Transfer(ENC28J60_SOFT_RESET);

    // Wait for the ENC28J60 to reset (add appropriate delay if needed)
    for (volatile int i = 0; i < 10000; i++);

    // Bring CS high to end communication
    IOSET0 = (1 << CS_PIN);
}

// Function to read a register from ENC28J60
uint8_t ENC28J60_Read(uint8_t address) {
    IOCLR0 = (1 << CS_PIN); // Select ENC28J60

    SPI_Transfer(ENC28J60_READ_CTRL_REG | (address & 0x1F)); // Send read command and address
    uint8_t result = SPI_Transfer(0x00); // Read the register value

    IOSET0 = (1 << CS_PIN); // Deselect ENC28J60
    return result;
}

// Function to write to a register of ENC28J60
void ENC28J60_Write(uint8_t address, uint8_t data) {
    IOCLR0 = (1 << CS_PIN); // Select ENC28J60

    SPI_Transfer(ENC28J60_WRITE_CTRL_REG | (address & 0x1F)); // Send write command and address
    SPI_Transfer(data); // Write the data to the register

    IOSET0 = (1 << CS_PIN); // Deselect ENC28J60
}

int main(void) {
    // Initialize SPI
    SPI_Init();

    // Initialize ENC28J60
    ENC28J60_Init();

    // Example: Write to a register and then read it back
    ENC28J60_Write(0x1F, 0xAA);  // Write 0xAA to register 0x1F
    uint8_t data = ENC28J60_Read(0x1F); // Read back the value from register 0x1F

    while (1) {
        // Your application code
    }

    return 0;
}
