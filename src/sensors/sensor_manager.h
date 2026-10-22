#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include "config.h"
#include "app_state.h"

class SensorManager {
public:
    SensorManager();
    void init(RoomState& state);
    void readSensors(RoomState& state);
    bool checkInterruptFlags(bool& shouldSample, bool& shouldRefreshDisplay);
    bool checkButtonPress();
    void triggerBH1750();

private:
    DHT _dht;
    RTC_DS1307 _rtc;
    hw_timer_t* _sampleTimer = nullptr;

    static void IRAM_ATTR isrSampleTimer();
    static void IRAM_ATTR isrClockTick();
    static void IRAM_ATTR isrButton();

    static volatile bool _flagSampleTimer;
    static volatile bool _flagClockTick;
    static volatile bool _flagButtonPressed;
    static volatile uint32_t _lastButtonDebounceTimeMs;
};
