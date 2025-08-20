#ifndef MONGOOSE_CUSTOM_IMPL_H
#define MONGOOSE_CUSTOM_IMPL_H

#include "mongoose_custom_config.h"
#include "mongoose.h"
#include <stdint.h>
// Timer functions
void timer0_init(void);
void FIQ_Handler(void);
unsigned long mg_millis(void);
//__irq void timer0_ISR(void);
/*#ifdef __arm__
    void timer0_ISR(void) __attribute__((interrupt("IRQ")));
#else
    void timer0_ISR(void);
#endif
*/
//Network functions
unsigned int mg_random(void);
uint32_t ntohl(uint32_t x);
void __sync_synchronize(void);         // Add prototype for __sync_synchronize

//unsigned long custom_get_millis(void);
//unsigned long mg_millis(void);       // Add prototype for mg_millis
//uint32_t mg_random(void);             // Add prototype for mg_random
//uint32_t ntohl(uint32_t netlong);     // Add prototype for ntohl

// Your existing TCP function declarations
//typedef void (*mg_pm_t)(void *fn_data);
int mg_tcpconnect(const char *url, mg_pm_t fn, void *fn_data);
int mg_tcplisten(const char *url, mg_pm_t fn, void *fn_data);
int mg_tcpsend(int fd, const void *buf, size_t len);
int mg_tcprecv(int fd, void *buf, size_t len);
int mg_tcpclose(int fd);

#endif // MONGOOSE_CUSTOM_IMPL_H
