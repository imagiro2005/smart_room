#pragma once
#include <Arduino.h>
#include <cmath>
#include "config.h"

struct RoomState {
    // Sensor readings (NAN if sensor disconnected or read error)
    float temperature   = NAN;
    float humidity      = NAN;
    float lightLux      = NAN;
    float luxThreshold  = 150.0f;
    int   potAdcValue   = 0;

    // Real-Time Clock (RTC DS1307)
    uint8_t  hour    = 0;
    uint8_t  minute  = 0;
    uint8_t  second  = 0;
    uint8_t  day     = 1;
    uint8_t  month   = 1;
    uint16_t year    = 2026;
    bool     isNight = false;

    // Environmental comfort assessment
    bool isComfortable         = false; // true: Green LED ON, false: Red LED ON
    bool isTempComfortable     = false;
    bool isHumidityComfortable = false;
    bool isLightComfortable    = false;

    // Room lighting (White LED)
    RoomMode mode             = MODE_AUTO;
    bool     isWhiteLampOn    = false;
    bool     isDayTooDark     = false; // Auto ON: Daytime too dark (under 120 lux)
    bool     isDayTooBright   = false; // Auto OFF: Daytime too bright (over 800 lux)
    bool     isNightTooBright = false; // Auto OFF: Nighttime too bright (over 350 lux)

    // Hardware status flags
    bool isDataReady  = false;
    bool isRtcReady   = false;
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

    String getTimeString() const {
        char buf[12];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour, minute, second);
        return String(buf);
    }

    String getDateString() const {
        char buf[12];
        snprintf(buf, sizeof(buf), "%02d/%02d/%04d", day, month, year);
        return String(buf);
    }
};
