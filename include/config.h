#pragma once
#include <Arduino.h>
#include <DHT.h>

// ==========================================
// GPIO PIN CONFIGURATION (ESP32-C3 DevKit)
// ==========================================
constexpr int PIN_POT_ADC       = 3;   // Potentiometer: Threshold adjustment (ADC1_CH3)
constexpr int PIN_LDR_ADC       = 1;   // LDR Photoresistor: Light intensity (ADC1_CH1)
constexpr int PIN_PIR_MOTION    = 0;   // PIR Sensor: Motion detection (External Interrupt)
constexpr int PIN_DHT_DATA      = 6;   // DHT22 Sensor: Temperature & Humidity
constexpr int PIN_LED_GREEN     = 7;   // Green LED: Safe / Comfortable environment
constexpr int PIN_LED_RED       = 10;  // Red LED: Threshold violated / Hazard alert
constexpr int PIN_LED_WHITE     = 2;   // White LED: Main room lighting lamp
constexpr int PIN_I2C_SDA       = 4;   // Shared I2C bus SDA (OLED SSD1306)
constexpr int PIN_I2C_SCL       = 5;   // Shared I2C bus SCL

// Sensor Type
#define SENSOR_DHT_TYPE DHT22

// ==========================================
// I2C DEVICE ADDRESS
// ==========================================
constexpr uint8_t I2C_ADDR_OLED = 0x3C;

// ==========================================
// SAFE ENVIRONMENT THRESHOLDS (DHT22)
// ==========================================
constexpr float TEMP_SAFE_MIN = 18.0f; // Safe temperature range: 18°C - 30°C
constexpr float TEMP_SAFE_MAX = 30.0f;

constexpr float HUM_SAFE_MIN  = 40.0f; // Safe humidity range: 40% - 75%
constexpr float HUM_SAFE_MAX  = 75.0f;

// ==========================================
// SMART LIGHTING TIMING & HYSTERESIS
// ==========================================
// Off-delay timer: Keep room lamp on for 30 seconds after the last detected motion
constexpr uint32_t LAMP_OFF_DELAY_MS = 30000; 

// Initial PIR sensor warm-up stabilization delay (15 seconds)
constexpr uint32_t PIR_WARMUP_MS = 15000;

// Sensor sampling interval via Hardware Timer (2 seconds)
constexpr uint32_t SENSOR_SAMPLE_INTERVAL_MS = 2000;

// Light hysteresis (percentage) to prevent rapid flickering
constexpr float LIGHT_HYSTERESIS_PERCENT = 3.0f;

// ==========================================
// ROOM LIGHTING CONTROL MODES
// ==========================================
enum RoomMode {
    MODE_AUTO       = 0, // Auto on when dark AND motion detected (with off-delay)
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
