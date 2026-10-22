#pragma once
#include <Arduino.h>
#include <DHT.h>

// ==========================================
// GPIO PIN CONFIGURATION (ESP32-C3 DevKit)
// ==========================================
constexpr int PIN_POT_ADC       = 3;   // Potentiometer for threshold adjustment (ADC1_CH3)
constexpr int PIN_DHT_DATA      = 6;   // DHT sensor data line
constexpr int PIN_PUSH_BUTTON   = 2;   // Physical wall button (External Interrupt, active LOW)
constexpr int PIN_RTC_SQW       = 0;   // RTC DS1307 1Hz square wave (External Interrupt)
constexpr int PIN_LED_GREEN     = 7;   // Green LED: Comfortable environment indicator
constexpr int PIN_LED_RED       = 10;  // Red LED: Environment threshold violation / Alert
constexpr int PIN_LED_WHITE     = 1;   // White LED: Main room lighting lamp
constexpr int PIN_I2C_SDA       = 4;   // Shared I2C bus SDA (BH1750, OLED, RTC DS1307)
constexpr int PIN_I2C_SCL       = 5;   // Shared I2C bus SCL

// DHT Sensor Type
#define SENSOR_DHT_TYPE DHT22

// ==========================================
// I2C DEVICE ADDRESSES
// ==========================================
constexpr uint8_t I2C_ADDR_OLED = 0x3C;
constexpr uint8_t I2C_ADDR_BH   = 0x23;
constexpr uint8_t I2C_ADDR_RTC  = 0x68;

// ==========================================
// COMFORTABLE ENVIRONMENT THRESHOLDS
// ==========================================
constexpr float TEMP_COMFORT_MIN = 20.0f; // Comfortable temperature: 20°C - 28°C
constexpr float TEMP_COMFORT_MAX = 28.0f;

constexpr float HUM_COMFORT_MIN  = 40.0f; // Comfortable humidity: 40% - 70%
constexpr float HUM_COMFORT_MAX  = 70.0f;

// Daytime comfortable lighting (06:00 - 17:59): Sufficient light for working/studying
constexpr float LUX_DAY_COMFORT_MIN = 150.0f;
constexpr float LUX_DAY_COMFORT_MAX = 1000.0f;

// Nighttime comfortable lighting (18:00 - 05:59): Soft ambient light for resting
constexpr float LUX_NIGHT_COMFORT_MIN = 20.0f;
constexpr float LUX_NIGHT_COMFORT_MAX = 300.0f;

// ==========================================
// AUTOMATIC LIGHTING LOGIC THRESHOLDS
// ==========================================
// Daytime dark compensation (rainy, cloudy, curtains closed)
constexpr float LUX_DAY_DARK_ON       = 120.0f; // Below 120 lux -> Turn ON white lamp
constexpr float LUX_DAY_DARK_OFF      = 160.0f; // Recovered above 160 lux -> Turn OFF white lamp

// Daytime excessive brightness safety shutoff:
// If daytime ambient light is extremely bright (> 800 lux), turn OFF white lamp immediately
constexpr float LUX_DAY_OVER_BRIGHT   = 800.0f;

// Nighttime excessive brightness shutoff
constexpr float LUX_NIGHT_BRIGHT_OFF  = 350.0f; // Above 350 lux at night -> Turn OFF lamp
constexpr float LUX_NIGHT_BRIGHT_ON   = 280.0f; // Ambient dim below 280 lux -> Turn ON lamp

// RTC Day / Night transition boundaries
constexpr uint8_t NIGHT_START_HOUR = 18; // 18:00 (Night begins)
constexpr uint8_t NIGHT_END_HOUR   = 6;  // 06:00 (Day begins)

// Sampling Interval
constexpr uint32_t SENSOR_SAMPLE_INTERVAL_MS = 2000; // Sample every 2000 ms

// ==========================================
// ROOM LIGHTING CONTROL MODES
// ==========================================
enum RoomMode {
    MODE_AUTO       = 0, // Automated schedule & extreme lighting response
    MODE_MANUAL_ON  = 1, // Manually forced ON
    MODE_MANUAL_OFF = 2  // Manually forced OFF
};

// ==========================================
// NETWORK & BLE CONFIGURATION
// ==========================================
constexpr const char* WIFI_SSID        = "SmartRoom-C3";
constexpr const char* WIFI_PASSWORD    = "smart1234";
constexpr const char* BLE_DEVICE_NAME  = "SmartRoom-C3";
constexpr const char* BLE_SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
constexpr const char* BLE_DATA_UUID    = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
