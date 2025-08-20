#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include "lpc214x.h"
#include "mongoose_custom_config.h"
#include "mongoose.h"
//#include "mongoose_custom_impl.c"
#include "ENC28J60.h"
#include "sensorData.h"
//#include "curl.h" // For HTTP POST requests
//#include "netinet_in.h"
//#include "arpa_inet.h"
//#include "winsock2.h"
//#include "sys_socket.h"

// SPI and ENC28J60 initialization functions (as in your original code)
//extern void spi_init();
extern double generate_random(double min, double max);
extern char* sensor_data_to_json(SensorData *data);
extern void Getdata();
extern void ENC28J60_init();

// Additional functions for SPI communication
void spi_write_byte(uint8_t data) {
      // Send a byte of data over SPI
        SSPDR = data;
    while (!(S0SPSR & (1 << 1)));
}

// Function prototypes for exporting to DLL
__declspec(dllexport) void set_ip_address(const char* ip_address);

// Function definitions
__declspec(dllexport) void set_ip_address(const char* ip_address) {
    printf("Setting IP address to: %s\n", ip_address);
    // Logic to set the IP address can be added here
}


uint8_t spi_read_byte() {
    // Read a byte of data from SPI
        SSPDR = 0xFF;
    while (!(S0SPSR & (1 << 1))); // Wait for transmission complete
    return SSPDR;
}

// ... other functions for sending commands and receiving responses

// Mongoose event handler
bool mg_http_match_uri(struct mg_http_message *hm, const char *uri);
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/api/sensor")) {
             char json[100];
            snprintf(json, sizeof(json), 
                    "{\"temperature\":%.2f,\"humidity\":%.2f}",
                    generate_random(20.0, 30.0),    // Replace with your actual sensor data
                    generate_random(40.0, 60.0));   // Replace with your actual sensor data
            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n", json);
        } 
        else if (mg_http_match_uri(hm, "/")) {
            // Simple HTML page that updates sensor data
            mg_http_reply(c, 200, "Content-Type: text/html\r\n",
                         "<!DOCTYPE html>"
                         "<html><body>"
                         "<h1>Sensor Data</h1>"
                         "<div id='sensor-data'>Loading...</div>"
                         "<script>"
                         "setInterval(() => {"
                         "  fetch('/api/sensor')"
                         "    .then(r => r.json())"
                         "    .then(data => {"
                         "      document.getElementById('sensor-data').innerHTML ="
                         "        `Temperature: ${data.temperature}°C<br>"
                         "         Humidity: ${data.humidity}%`;"
                         "    });"
                         "}, 1000);"
                         "</script>"
                         "</body></html>");
        } 
        else {
            mg_http_reply(c, 404, "", "Not Found");
        }
    }
}

// Mongoose timer callback
static void timer_fn(void *arg) {
    // Perform periodic tasks here
}
char x = 'Y';
int main() {
    struct mg_mgr mgr;
    struct mg_connection {
	struct mg_connection *c;
    struct mg_connection *next;
    struct mg_mgr *mgr;
    struct mg_addr peer;
    struct mg_addr loc;
    void *fd;
    unsigned long id;
    struct mg_iobuf recv;
    struct mg_iobuf send;
    mg_event_handler_t fn;
    void *fn_data;
    mg_event_handler_t pfn;
    void *pfn_data;
    uint32_t is_listening : 1;
    uint32_t is_client : 1;
    uint32_t is_accepted : 1;
    uint32_t is_resolving : 1;
    uint32_t is_connecting : 1;
    uint32_t is_tls : 1;
    uint32_t is_tls_hs : 1;
    uint32_t is_udp : 1;
    uint32_t is_websocket : 1;
    uint32_t is_hexdumping : 1;
    uint32_t is_draining : 1;
    uint32_t is_closing : 1;
    uint32_t is_full : 1;
    uint32_t is_resp : 1;
    uint32_t is_readable : 1;
    uint32_t is_writable : 1;
    void *tls;
    unsigned long oldlc;
    size_t recv_mbuf_limit;
};
    struct mg_timer timer;
	struct mg_timer *timer_ptr = &timer;
    // Initialize hardware
    //spi_init();
    //enc28j60_init();

    // Initialize Mongoose
    mg_mgr_init(&mgr);

    // Create listening connection
     /*struct mg_connection *c = mg_http_listen(&mgr, "0.0.0.0:80", fn, NULL);
    if (c == NULL) {
        printf("Failed to create listener\n");
        return 1;
    }*/ //(COMMENTED)

    // Set up timer
    mg_timer_init(NULL,&timer, 1000, MG_TIMER_REPEAT,timer_fn,&mgr);

    // Main event loop
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
        // Perform any other necessary tasks
    }

    // Cleanup
    mg_mgr_free(&mgr);
// Initialize the ENC28J60 Ethernet module
    ENC28J60_init();

    /* Set a MAC address (example: 00:1A:2B:3C:4D:5E)
    unsigned char mac_address[6] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E};
    set_mac_address(mac_address);
*/
    // Set a static IP address (example: 192.168.1.100)
    set_ip_address("192.168.1.100");

    SensorData sensor_data;
    int generation_count = 0;

    while (1) {
        // Simulate sensor data generation
        printf("Generating sensor data...\n");
        Getdata(&sensor_data);

        // Print the generated sensor data
        printf("Temperature: %.2f °C\n", sensor_data.temperature);
        printf("Humidity: %.2f %%\n", sensor_data.humidity);
        printf("Pressure: %.2f hPa\n", sensor_data.pressure);
        printf("Light Intensity: %.2f lux\n", sensor_data.light);
        printf("---------------------------\n");

        // Convert the sensor data to JSON
        char* json_data = sensor_data_to_json(&sensor_data);
        printf("Sensor data in JSON format: %s\n", json_data);
        free(json_data);

        // Delay for 1 seconds to simulate real-time data generation
        Sleep(1000);

        generation_count++;

        // After every 5 data generations, prompt the user for input
        if (generation_count % 5 == 0) {
            char x;
            printf("Generate more Data (Y/N): ");
            scanf(" %c", &x);

            // Check if the user wants to stop
            if (x == 'N' || x == 'n') {
                break;
            }
        }
    }
    return 0;
}