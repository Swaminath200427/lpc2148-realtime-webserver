#include "mongoose.h"

// Function to match URI in HTTP messages
bool mg_http_match_uri(struct mg_http_message *hm, const char *uri) {
    return mg_strcmp(hm->uri, mg_str(uri)) == 0;
}

// Function to handle WebSocket messages
static void handle_websocket(struct mg_connection *c, struct mg_ws_message *wm) {
    // Here you can handle incoming WebSocket messages and respond
    char response[256];

    // Use wm->data.p if your mg_ws_message struct has a pointer to the data
    snprintf(response, sizeof(response), "Echo: %.*s", (int)wm->data.len, wm->data); // Use wm->data directly if it’s a char array
    mg_ws_send(c, response, strlen(response), WEBSOCKET_OP_TEXT);
}

// Event handler function for HTTP and WebSocket events
void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    switch (ev) {
        case MG_EV_HTTP_MSG: {
            struct mg_http_message *hm = (struct mg_http_message *)ev_data;
            if (mg_http_match_uri(hm, "/api/sensor")) {
                char json[100];
                snprintf(json, sizeof(json),
                         "{\"temperature\":%.2f,\"humidity\":%.2f}",
                         generate_random(20.0, 30.0), // Replace with your actual sensor data
                         generate_random(40.0, 60.0)); // Corrected here
                mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s\n", json);
            } else if (mg_http_match_uri(hm, "/")) {
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
            } else {
                mg_http_reply(c, 404, "", "Not Found");
            }
            break;
        }
        case MG_EV_WS_MSG: {
            // Handle WebSocket message
            struct mg_ws_message *wm = (struct mg_ws_message *)ev_data;
            handle_websocket(c, wm);
            break;
        }
        default:
            break;
    }
}
