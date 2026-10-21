#include "room_controller.h"

void RoomController::init() {
    pinMode(PIN_LED_ROOM, OUTPUT);
    pinMode(PIN_LED_WARN, OUTPUT);
    digitalWrite(PIN_LED_ROOM, LOW);
    digitalWrite(PIN_LED_WARN, LOW);
}

void RoomController::update(RoomState& state) {
    // 1. Xác định phòng tối hay sáng (dùng Hysteresis chống chớp giật đèn)
    if (isnan(state.doSang)) {
        state.phongToi = false; // Mất cảm biến ánh sáng -> không tự bật đèn
    } else if (state.doSang <= state.nguongToi) {
        state.phongToi = true;
    } else if (state.doSang >= state.nguongToi + LUX_HYSTERESIS) {
        state.phongToi = false;
    }

    // 2. Quyết định trạng thái đèn phòng
    if (state.cheDo == MODE_MANUAL_ON) {
        state.denBat = true;
    } else if (state.cheDo == MODE_MANUAL_OFF) {
        state.denBat = false;
    } else {
        // Chế độ Tự động (AUTO): chỉ bật khi đã có dữ liệu, phòng tối và có chuyển động
        state.denBat = state.daCoDuLieu && state.phongToi && state.coChuyenDong;
    }

    // 3. Quyết định cảnh báo (LED đỏ)
    bool loiCamBien = isnan(state.nhietDo) || isnan(state.doAm) || isnan(state.doSang);
    bool vuotNguong = (state.nhietDo >= NGUONG_NHIET_CAO) || (state.doAm >= NGUONG_AM_CAO);
    state.canhBao = state.daCoDuLieu && (loiCamBien || vuotNguong);

    // 4. Xuất tín hiệu ra chân GPIO
    digitalWrite(PIN_LED_ROOM, state.denBat ? HIGH : LOW);
    digitalWrite(PIN_LED_WARN, state.canhBao ? HIGH : LOW);
}

bool RoomController::processSerial(RoomState& state) {
    if (!Serial.available()) {
        return false;
    }

    char c = static_cast<char>(Serial.read());
    if (c == 'A' || c == 'a') {
        state.cheDo = MODE_AUTO;
    } else if (c == '1') {
        state.cheDo = MODE_MANUAL_ON;
    } else if (c == '0') {
        state.cheDo = MODE_MANUAL_OFF;
    } else {
        return false;
    }

    return true;
}
