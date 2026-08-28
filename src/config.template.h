#ifndef _CONFIG_H_
#define _CONFIG_H_

/**
  * Configurations
  */

// 2.4 GHz WiFi Configurations
#define WIFI_SSID "your WiFi SSID"
#define WIFI_PASSWORD "your WiFi password"

// OpenWeatherMap API Configurations
#define OPENWEATHERMAP_API_KEY "your OpenWeatherMap API key"
#define LATITUDE 35.68130      // Latitude (e.g., Tokyo)
#define LONGITUDE 139.76707    // Longitude (e.g., Tokyo)
#define LOCATION_NAME ""       // Optional display label (falls back to API timezone if empty)
#define TIMEZONE_OFFSET 9      // Offset from UTC (in hours)
#define TEMPERATURE_UNIT 0     // 0 = Celsius, 1 = Fahrenheit
#define PRESSURE_UNIT 0        // 0 = hPa, 1 = inHg

// Interval Configurations (minutes)
#define INTERVAL_IN_MINUTES 60 // 1 hour

// Display color behavior
#define INVERT_DISPLAY_DURING_DAY 1 // 1 = black background between sunrise and sunset

// CrowPanel side controls
#define SIDE_BUTTON_MENU_PIN 2
#define SIDE_BUTTON_EXIT_PIN 1
#define SIDE_ROCKER_DOWN_PIN 4
#define SIDE_ROCKER_CONFIRM_PIN 5
#define SIDE_ROCKER_UP_PIN 6
#define SIDE_CONTROLS_ENABLED 1

#endif
