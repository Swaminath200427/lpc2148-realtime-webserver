#ifndef MONGOOSE_CUSTOM_CONFIG
#define MONGOOSE_CUSTOM_CONFIG

#define MG_ARCH MG_ARCH_WIN32

// Disable features that are not applicable or too heavy for LPC2148
#define MG_ENABLE_DIRECTORY_LISTING 0
#define MG_ENABLE_FILESYSTEM 0
#define MG_ENABLE_IPV6 0
#define MG_ENABLE_MQTT 0
#define MG_ENABLE_SOCKETPAIR 0
#define MG_ENABLE_LINES 0
#define MG_ENABLE_HTTP 1


// Define custom timing function (you'll need to implement this)
#define MG_MILLIS() custom_get_millis()

// Use custom TCP functions
#define MG_ENABLE_CUSTOM_TCP 1

// Disable features that require POSIX APIs
#define MG_ENABLE_POLL 0

// Add other necessary configurations here

#endif // MONGOOSE_CUSTOM_CONFIG_H
