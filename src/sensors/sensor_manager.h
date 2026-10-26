#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include "config.h"
#include "app_state.h"

class SensorManager {
public:
    SensorManager();
    void init(RoomState& state);
    void readSensors(RoomState& state);
    bool checkInterruptFlags(bool& shouldSample, bool& shouldUpdateState);
    void updateMotionState(RoomState& state);

private:
    DHT _dht;
    hw_timer_t* _sampleTimer = nullptr;

    static void IRAM_ATTR isrSampleTimer();
    static void IRAM_ATTR isrPIR();

    static volatile bool _flagSampleTimer;
    static volatile bool _flagMotionChanged;
};
