cmake_minimum_required(VERSION 3.10)
project(WEBSERVER_1_2)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories for headers
include_directories("F:\\PROJECTS & RESEARCH\\MP_PROJECT\\webSERVER_1_2\\inc")

# Specify the sources including mongoose and any other necessary files
set(SOURCES src/main.c src/mongoose.c src/mongoose_custom_impl.c src/Sensor_rand.c src/mongoose_ws.c src/mongoose_ws_custom.c src/ENC28J60_driver.c)

# Define necessary macros for mongoose (adjust based on your usage)
add_definitions(-DMG_ENABLE_HTTP -DMG_ENABLE_WEBSOCKET -DMG_ENABLE_MQTT)

# Add the shared library target
add_library(mylibrary SHARED ${SOURCES})

# Windows-specific settings
if(WIN32)
    target_link_options(mylibrary PRIVATE -lws2_32 -lwsock32 -static)
    target_link_libraries(mylibrary PRIVATE ws2_32 wsock32)
    target_compile_definitions(mylibrary PRIVATE WIN32_LEAN_AND_MEAN)
endif()

# Specify the output directory for the DLL
set_target_properties(mylibrary PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")