#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "sensorData.h"

/*typedef struct {
    double temperature;
    double humidity;
    double pressure;
    double light;
} SensorData;
*/
// Function to generate a random value within a specified range
double generate_random(double min, double max) {
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

// Function to simulate sensor data generation
void Getdata(SensorData *data) {
    data->temperature = generate_random(20.0, 50.0);
    data->humidity = generate_random(0.0, 100.0);
    data->pressure = generate_random(950.0, 1050.0);
    data->light = generate_random(0.0, 1000.0);
}

// Function to convert SensorData struct to JSON
char* sensor_data_to_json(SensorData *data) {
    char* json_string = (char*)malloc(256 * sizeof(char));
    sprintf(json_string, "{\"temperature\":%.2f,\"humidity\":%.2f,\"pressure\":%.2f,\"light\":%.2f}",
            data->temperature, data->humidity, data->pressure, data->light);
    return json_string;
}

/*int main() {
    // Seed the random number generator
    srand(time(NULL));

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
        sleep(1);

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
}*/