#pragma once
#include <Arduino.h>
#include <cmath>
#include "config.h"

struct RoomState {
    // Sensor readings from DHT22 (NAN if disconnected or error)
    float temperature = NAN;
    float humidity    = NAN;

    // ADC readings: Potentiometer (Threshold) & LDR (Light Intensity)
    int   potRawAdc        = 0;
    int   ldrRawAdc        = 0;
    float thresholdPercent = 50.0f; // Adjusted via potentiometer (0% - 100%)
    float lightPercent     = 0.0f;  // Light level from LDR (0% = Dark, 100% = Bright)
    bool  isRoomDark       = false; // true if lightPercent <= thresholdPercent

    // Motion status from PIR
    bool     isMotionDetected   = false;
    uint32_t lastMotionTimeMs   = 0;
    bool     isMotionHoldActive = false; // true while within the 30-second off-delay window

    // Environmental safety status (DHT22)
    bool isEnvironmentSafe = false; // true: Safe (Green LED ON), false: Alert (Red LED ON)
    bool isTempSafe        = false;
    bool isHumSafe         = false;

    // Room lighting (White LED)
    RoomMode mode          = MODE_AUTO;
    bool     isWhiteLampOn = false;

    // Hardware status flags
    bool isDataReady  = false;
    bool isPirReady   = false;
    bool isOledReady  = false;
    bool isTimerReady = false;

    // System timestamps
    uint32_t bootTimeMs   = 0;
    uint32_t sampleTimeMs = 0;

    const char* getModeName() const {
        switch (mode) {
            case MODE_MANUAL_ON:  return "ON";
            case MODE_MANUAL_OFF: return "OFF";
            default:              return "AUTO";
        }
    }
};
