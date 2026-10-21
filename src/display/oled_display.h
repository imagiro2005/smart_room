#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "config.h"
#include "app_state.h"

class OledDisplay {
public:
    OledDisplay();
    void init(RoomState& state);
    void update(const RoomState& state);

private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C _oled;
    String formatValue(float value, int decimals = 1);
};
