#pragma once
#include <Arduino.h>
#include <DHT.h>

// ==========================================
// CẤU HÌNH CHÂN GPIO (ESP32-C3 DevKit)
// ==========================================
constexpr int PIN_POT      = 3;   // Biến trở: chỉnh ngưỡng tối (ADC)
constexpr int PIN_DHT      = 6;   // DHT: Nhiệt độ & Độ ẩm
constexpr int PIN_PIR      = 0;   // PIR HC-SR501: Chuyển động (Ngắt ngoài)
constexpr int PIN_LED_ROOM = 7;   // LED xanh: Đèn chiếu sáng phòng
constexpr int PIN_LED_WARN = 10;  // LED đỏ: Cảnh báo nhiệt/ẩm/lỗi cảm biến
constexpr int PIN_I2C_SDA  = 4;   // Bus I2C chung (BH1750 & OLED SSD1306)
constexpr int PIN_I2C_SCL  = 5;

// Loại cảm biến DHT (DHT22 tương thích hoàn hảo với mô phỏng Wokwi)
#define SENSOR_DHT_TYPE DHT22

// ==========================================
// ĐỊA CHỈ I2C
// ==========================================
constexpr uint8_t ADDR_OLED = 0x3C;
constexpr uint8_t ADDR_BH   = 0x23;

// ==========================================
// THAM SỐ NGƯỠNG VÀ THỜI GIAN
// ==========================================
constexpr float   NGUONG_NHIET_CAO = 32.0f;     // °C
constexpr float   NGUONG_AM_CAO    = 80.0f;     // %
constexpr float   LUX_HYSTERESIS   = 20.0f;     // Trễ chống rung đèn
constexpr uint32_t CHU_KY_DOC_MS   = 2000;      // Chu kỳ đọc cảm biến (ms)
constexpr uint32_t PIR_WARMUP_MS   = 60000;     // Thời gian chờ PIR ổn định (ms)

// ==========================================
// CHẾ ĐỘ ĐIỀU KHIỂN
// ==========================================
enum RoomMode {
    MODE_AUTO       = 0,
    MODE_MANUAL_ON  = 1,
    MODE_MANUAL_OFF = 2
};

// ==========================================
// CẤU HÌNH MẠNG & BLE
// ==========================================
constexpr const char* WIFI_SSID       = "SmartRoom-C3";
constexpr const char* WIFI_PASSWORD   = "smart1234";
constexpr const char* BLE_DEVICE_NAME = "SmartRoom-C3";
constexpr const char* BLE_SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
constexpr const char* BLE_DATA_UUID    = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
