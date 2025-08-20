#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 

void Getdata(){
while (i <= 1) {
        printf("I value: %d", i);
        // Simulate random sensor readings
        double temperature = generate_random(20.0, 50.0); // Temperature range: -20 to 50 degrees Celsius
        double humidity = generate_random(0.0, 100.0);     // Humidity range: 0 to 100 percent
        double pressure = generate_random(950.0, 1050.0);  // Pressure range: 950 to 1050 hPa
        double light = generate_random(0.0, 1000.0);       // Light intensity range: 0 to 1000 lux

        // Introduce random errors in sensor readings
        /*temperature = introduce_error(temperature, 50.0, 10.0); // Max temperature threshold: 50.0°C, Error margin: 10.0°C
        humidity = introduce_error(humidity, 100.0, 20.0);      // Max humidity threshold: 100.0%, Error margin: 20.0%
        pressure = introduce_error(pressure, 1050.0, 50.0);     // Max pressure threshold: 1050.0 hPa, Error margin: 50.0 hPa
        light = introduce_error(light, 1000.0, 200.0);*/          // Max light intensity threshold: 1000.0 lux, Error margin: 200.0 lux

        // Write the sensor readings to the file
        fprintf(file, "Temperature: %.2f °C\n", temperature);
        fprintf(file, "Humidity: %.2f %%\n", humidity);
        fprintf(file, "Pressure: %.2f hPa\n", pressure);
        fprintf(file, "Light Intensity: %.2f lux\n", light);
        fprintf(file, "---------------------------\n");

        // Flush the output to ensure the data is written immediately
        fflush(file);

        // Print the values to the console as well (optional)
        printf("Temperature: %.2f °C\n", temperature);
        printf("Humidity: %.2f %%\n", humidity);
        printf("Pressure: %.2f hPa\n", pressure);
        printf("Light Intensity: %.2f lux\n", light);
        printf("---------------------------\n");

        fflush(stdout);

        // Introduce a delay to simulate real-time data (e.g., 1 second)
        //sleep(1); // Use Sleep(1000) if you're on Windows
       i = i+1; 
        
    }
}