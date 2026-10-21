#pragma once
#include <Arduino.h>
#include <cmath>
#include "config.h"

struct RoomState {
    // Giá trị cảm biến (NAN nếu chưa có dữ liệu hoặc lỗi)
    float nhietDo   = NAN;
    float doAm      = NAN;
    float doSang    = NAN;
    float nguongToi = 100.0f;
    int   giaTriADC = 0;

    // Trạng thái hệ thống
    RoomMode cheDo       = MODE_AUTO;
    bool     phongToi    = false;
    bool     coChuyenDong= false;
    bool     denBat      = false;
    bool     canhBao     = false;

    // Cờ trạng thái phần cứng
    bool daCoDuLieu  = false;
    bool pirSanSang  = false;
    bool oledOK      = false;
    bool timerOK     = false;

    // Thời gian
    uint32_t lucBatDau = 0;
    uint32_t mauMs     = 0;

    const char* getModeName() const {
        switch (cheDo) {
            case MODE_MANUAL_ON:  return "ON";
            case MODE_MANUAL_OFF: return "OFF";
            default:              return "AUTO";
        }
    }
};
