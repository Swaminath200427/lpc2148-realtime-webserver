#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HTML_FILE_PATH "index.html"

// Function to generate random sensor data
void generate_random_sensor_data(char *buffer, size_t size) {
    float temperature = ((float) rand() / RAND_MAX) * 100;
    float humidity = ((float) rand() / RAND_MAX) * 100;
    float pressure = ((float) rand() / RAND_MAX) * 1000;

    snprintf(buffer, size, "{\"temperature\": %.2f %s, \"humidity\": %.2f %s, \"pressure\": %.2f %s  }", 
             temperature,"deg C", humidity,"e", pressure,"hPa");
}

// Function to serve the HTML file
void serve_html(struct mg_connection *c) {
    FILE *fp = fopen(HTML_FILE_PATH, "r");
    if (fp == NULL) {
        mg_http_reply(c, 500, "Content-Type: text/plain\r\n", "Error: Could not open HTML file.\n");
        return;
    }

    // Read the HTML file into a buffer
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *html_content = malloc(file_size + 1);
    fread(html_content, 1, file_size, fp);
    html_content[file_size] = '\0';

    fclose(fp);

    // Serve the HTML file content
    mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", html_content);
    free(html_content);
}

// HTTP event handler for processing incoming requests
static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        // Serve HTML at the root "/"
        if (mg_strcmp(hm->uri, mg_str("/")) == 0) {
            serve_html(c);
        }
        // Handle the "/data" route for sensor data
        else if (mg_strcmp(hm->uri, mg_str("/data")) == 0) {
            char sensor_data[256];
            generate_random_sensor_data(sensor_data, sizeof(sensor_data));
            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", sensor_data);
        }
        // Return 404 for any other routes
        else {
            mg_http_reply(c, 404, "", "Not Found\n");
        }
    }
}

int main(void) {
    // Seed the random number generator
    srand(time(NULL));

    // Mongoose event manager
    struct mg_mgr mgr;

    // Initialize the Mongoose manager
    mg_mgr_init(&mgr);

    // Start a server on localhost:8000
    mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);

    printf("Server running on http://localhost:8000\n");

    // Event loop to handle incoming requests
    while (1) {
        mg_mgr_poll(&mgr, 1000);  // Polling every second
    }

    // Free the manager resources
    mg_mgr_free(&mgr);

    return 0;
}
