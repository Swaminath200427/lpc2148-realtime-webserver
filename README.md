# lpc2148-realtime-webserverLPC2148 Real-Time Web Server
A comprehensive embedded systems project implementing a real-time web server on the ARM7 LPC2148 microcontroller with Ethernet connectivity and sensor data monitoring capabilities.

🎯 Project Overview
This project demonstrates the implementation of a real-time web server on the LPC2148 ARM7 microcontroller, featuring Ethernet communication via the ENC28J60 module, sensor data collection, and a web-based dashboard for remote monitoring.

🏗️ System Architecture
The system consists of several key components working together:

LPC2148 ARM7 Microcontroller: Core processing unit running the embedded web server

ENC28J60 Ethernet Module: Provides TCP/IP network connectivity via SPI interface

Mongoose Web Server: Lightweight embedded web server framework

Sensor Simulation: Real-time generation of environmental sensor data

Flask Web Application: Frontend dashboard for data visualization

MySQL Database: Data storage and historical analysis

🔧 Hardware Components
LPC2148 ARM7 Microcontroller
CPU: ARM7TDMI-S core running at up to 60MHz

Memory: 512KB Flash, 32KB RAM

Peripherals: SPI, UART, GPIO, Timers

Package: 64-pin LQFP

ENC28J60 Ethernet Controller
Interface: SPI communication with LPC2148

Network: 10BASE-T Ethernet connectivity

Features: Hardware TCP/IP stack, integrated MAC and PHY

Buffer: 8KB internal buffer for packet storage

Pin Configuration
text
LPC2148 → ENC28J60
P0.4   → SCK  (SPI Clock)
P0.5   → MOSI (SPI Data Out)
P0.6   → MISO (SPI Data In)
P0.10  → CS   (Chip Select)
💻 Software Architecture
Embedded System (LPC2148)
Real-time sensor data generation with configurable sampling rates

SPI communication driver for ENC28J60 Ethernet controller

Mongoose web server integration for HTTP request handling

JSON data formatting for API responses

MAC address configuration and network initialization

Web Application (Flask)
Real-time data visualization with automatic refresh

Historical data storage in MySQL database

REST API endpoints for current and historical data

Responsive web interface with embedded systems styling

Background data collection via multithreading

🚀 Features
Real-Time Capabilities
Live sensor monitoring: Temperature, humidity, pressure, and light intensity

Automatic data refresh: Updates every 2 seconds

Network connectivity: Access via standard web browser

JSON API: RESTful endpoints for data access

Sensor Data Simulation
c
typedef struct {
    double temperature;  // Range: 20.0 - 50.0 °C
    double humidity;     // Range: 0.0 - 100.0 %
    double pressure;     // Range: 950.0 - 1050.0 hPa
    double light;        // Range: 0.0 - 1000.0 lux
} SensorData;
Web Server Endpoints
GET / - Main dashboard interface

GET /api/sensor - Current sensor data (JSON)

GET /api/current - Real-time data endpoint

GET /api/historical - Historical data with timestamp

🛠️ Build Instructions
Prerequisites
ARM Cross-Compiler: arm-none-eabi-gcc

CMake: Version 3.10 or higher

Python 3.x: For Flask web application

MySQL Server: For data storage

Keil μVision (optional): For IDE-based development

Building the Embedded Code
bash
cd webSERVER_1_2
mkdir build && cd build
cmake ..
make
Alternative Build with Makefile
bash
cd webSERVER_1_2
make clean
make all
Setting up the Web Application
bash
cd FRONTEND
pip install flask mysql-connector-python requests
python app.py
📋 Configuration
Network Configuration
c
// Set static IP address for LPC2148
set_ip_address("192.168.1.100");

// Configure MAC address
unsigned char mac_address = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E};
Database Configuration
python
DB_CONFIG = {
    "host": "localhost",
    "port": "3306",
    "user": "root",
    "password": "your_password",
    "database": "sensor_data_db"
}
🔌 Hardware Setup
Circuit Connections
Power Supply: 3.3V for both LPC2148 and ENC28J60

SPI Connections: Connect as per pin configuration above

Ethernet: Connect RJ45 jack to ENC28J60 module

Programming: Use JTAG or ISP interface for code upload

ENC28J60 Module Setup
Voltage: 3.3V operation

Crystal: 25MHz external crystal

Bypass Capacitors: 100nF ceramic capacitors

Ethernet Magnetics: Integrated transformer module

📊 API Documentation
Sensor Data Format
json
{
  "temperature": 25.42,
  "humidity": 65.18,
  "pressure": 1013.25,
  "light": 450.75
}
REST Endpoints
GET /api/sensor: Returns current sensor readings

GET /api/current: Real-time data with timestamp

GET /api/historical: Array of historical readings

🐛 Troubleshooting
Common Issues
ENC28J60 Not Responding

Check SPI connections and power supply

Verify chip select pin configuration

Ensure proper crystal oscillator operation

Network Connectivity Issues

Verify Ethernet cable connection

Check IP address configuration

Ensure router/switch compatibility

Compilation Errors

Verify ARM toolchain installation

Check include path configuration

Ensure all dependencies are available

Debug Features
Status LED indicators for network and data activity

UART debugging output for troubleshooting

SPI communication monitoring via oscilloscope

📈 Performance Specifications
Data Update Rate: 1-5 seconds configurable

Network Throughput: Up to 10 Mbps

Concurrent Connections: Up to 4 simultaneous clients

Memory Usage: <16KB RAM, <64KB Flash

Power Consumption: <200mA at 3.3V

🎓 Educational Value
This project demonstrates:

Embedded systems programming with real-time constraints

Network programming and TCP/IP stack implementation

Hardware interfacing via SPI communication

Web server development for embedded platforms

Database integration and data persistence

Cross-platform development (embedded + web)

🔮 Future Enhancements
Planned Features
Wireless connectivity via ESP8266/ESP32 module

SSL/TLS security for encrypted communication

Data logging to SD card for offline storage

Mobile application for remote monitoring

Real sensor integration replacing simulation

IoT cloud connectivity (AWS, Azure, etc.)

Possible Improvements
Multi-threading for better concurrent handling

Dynamic IP configuration via DHCP

Web-based configuration interface

Alarm/notification system for threshold monitoring

Data analytics and trending capabilities

📚 Learning Resources
Recommended Reading
LPC2148 User Manual: NXP Semiconductors

ENC28J60 Datasheet: Microchip Technology

ARM7 Architecture Reference: ARM Holdings

Embedded TCP/IP Programming: Network programming concepts

Development Tools
Keil μVision: Professional ARM development environment

GNU ARM Toolchain: Open-source cross-compiler

OpenOCD: On-chip debugging and programming

Wireshark: Network packet analysis

🤝 Contributing
Development Guidelines
Code Style: Follow embedded C best practices

Documentation: Comment complex algorithms and hardware interfaces

Testing: Verify on actual hardware before committing

Pull Requests: Include detailed description of changes

Issue Reporting
Hardware Issues: Include schematic and connection details

Software Bugs: Provide compilation environment and error logs

Feature Requests: Describe use case and implementation approach


🙏 Acknowledgments
Mongoose Web Server: Cesanta Software for the embedded web server framework

ARM Community: For ARM7 development resources and support

Open Source Libraries: Various contributors to embedded systems development

Educational Institutions: For providing learning opportunities in embedded systems

