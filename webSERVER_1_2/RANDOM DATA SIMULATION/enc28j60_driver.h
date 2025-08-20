#ifndef ENC28J60_DRIVER_H
#define ENC28J60_DRIVER_H

void spi_init();
void enc28j60_init();
void set_mac_address(unsigned char mac[6]);
void set_ip_address(const char *ip);

#endif // ENC28J60_DRIVER_H
