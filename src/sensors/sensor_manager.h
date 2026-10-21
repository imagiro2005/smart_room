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
    bool checkInterruptFlags(bool& canDoc, bool& canCapNhat);
    void updateMotionState(RoomState& state);
    void triggerBH1750();

private:
    DHT _dht;
    hw_timer_t* _timerDoc = nullptr;

    static void IRAM_ATTR isrTimer();
    static void IRAM_ATTR isrPIR();

    static volatile bool _flagKyDoc;
    static volatile bool _flagPIRThayDoi;
};
