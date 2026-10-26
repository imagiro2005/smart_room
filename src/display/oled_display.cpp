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

    // Line 1: Temperature from DHT22
    String sTemp = "TEMP : " + formatValue(state.temperature) + " C";
    _oled.drawStr(2, 14, sTemp.c_str());

    // Line 2: Humidity from DHT22
    String sHum = "HUM  : " + formatValue(state.humidity) + " %";
    _oled.drawStr(2, 30, sHum.c_str());

    // Line 3: LDR light intensity vs Potentiometer threshold
    String sLight = "LDR  : " + String(state.isRoomDark ? "DARK" : "BRIGHT") + 
                    " (" + String(static_cast<int>(state.thresholdPercent)) + "%)";
    _oled.drawStr(2, 46, sLight.c_str());

    // Line 4: System safety state and White Room Lamp status
    String sState;
    if (!state.isDataReady) {
        sState = "STATE: INIT...";
    } else {
        const char* safeStr = state.isEnvironmentSafe ? "SAFE" : "ALERT";
        const char* lampStr = state.isWhiteLampOn ? "ON" : "OFF";
        sState = String("STATE:") + safeStr + " | L:" + lampStr;
    }
    _oled.drawStr(2, 62, sState.c_str());

    _oled.sendBuffer();
}
