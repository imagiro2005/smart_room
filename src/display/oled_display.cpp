#include "oled_display.h"

OledDisplay::OledDisplay() 
    : _oled(U8G2_R0, U8X8_PIN_NONE, PIN_I2C_SCL, PIN_I2C_SDA) {}

String OledDisplay::formatValue(float value, int decimals) {
    if (isnan(value)) {
        return "--";
    }
    return String(value, decimals);
}

void OledDisplay::init(RoomState& state) {
    _oled.setI2CAddress(I2C_ADDR_OLED * 2);
    _oled.begin();
    state.isOledReady = true;
    update(state);
}

void OledDisplay::update(const RoomState& state) {
    if (!state.isOledReady) {
        return;
    }

    _oled.clearBuffer();
    // 7x14 font: clear, modern and balanced on 128x64 display
    _oled.setFont(u8g2_font_7x14_tf);

    // Line 1: Temperature
    String sTemp = "TEMP  : " + formatValue(state.temperature) + " C";
    _oled.drawStr(2, 14, sTemp.c_str());

    // Line 2: Humidity
    String sHum = "HUM   : " + formatValue(state.humidity) + " %";
    _oled.drawStr(2, 30, sHum.c_str());

    // Line 3: Ambient Light
    String sLux = "LUX   : " + formatValue(state.lightLux, 0) + " lx";
    _oled.drawStr(2, 46, sLux.c_str());

    // Line 4: Real-time Clock from RTC DS1307
    String sTime;
    if (state.isRtcReady) {
        sTime = "TIME  : " + state.getTimeString();
    } else {
        sTime = "TIME  : NO RTC";
    }
    _oled.drawStr(2, 62, sTime.c_str());

    _oled.sendBuffer();
}
