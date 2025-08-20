#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

// Struct to hold the sensor data
typedef struct {
    double temperature;
    double humidity;
    double pressure;
    double light;
} SensorData;

#endif // SENSOR_DATA_H