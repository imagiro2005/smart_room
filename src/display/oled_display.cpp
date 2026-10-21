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
    _oled.setI2CAddress(ADDR_OLED * 2);
    _oled.begin();
    state.oledOK = true;
    update(state);
}

void OledDisplay::update(const RoomState& state) {
    if (!state.oledOK) {
        return;
    }

    _oled.clearBuffer();
    // Font 7x14 to rõ, dễ nhìn, cân đối trên màn hình 128x64
    _oled.setFont(u8g2_font_7x14_tf);

    // Dòng 1: Nhiệt độ
    String sTemp = "TEMP  : " + formatValue(state.nhietDo) + " C";
    _oled.drawStr(2, 14, sTemp.c_str());

    // Dòng 2: Độ ẩm
    String sHum = "HUM   : " + formatValue(state.doAm) + " %";
    _oled.drawStr(2, 30, sHum.c_str());

    // Dòng 3: Ánh sáng
    String sLux = "LUX   : " + formatValue(state.doSang, 0) + " lx";
    _oled.drawStr(2, 46, sLux.c_str());

    // Dòng 4: Phát hiện người
    String sMotion;
    if (!state.pirSanSang) {
        sMotion = "PERSON: INIT...";
    } else {
        sMotion = state.coChuyenDong ? "PERSON: YES" : "PERSON: NO";
    }
    _oled.drawStr(2, 62, sMotion.c_str());

    _oled.sendBuffer();
}
