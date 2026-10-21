#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "app_state.h"
#include "sensors/sensor_manager.h"
#include "controller/room_controller.h"
#include "display/oled_display.h"
#include "network/network_manager.h"

// Đối tượng trạng thái & các module
RoomState      roomState;
SensorManager  sensorMgr;
RoomController roomCtrl;
OledDisplay    oledDisp;
NetworkManager netMgr;

// Hàm đồng bộ tất cả đầu ra (LEDs, OLED, BLE, Serial)
void updateAllOutputs() {
    roomCtrl.update(roomState);
    oledDisp.update(roomState);
    netMgr.sendTelemetry(roomState);
}

void setup() {
    Serial.begin(115200);
    roomState.lucBatDau = millis();

    // 1. Khởi tạo bus I2C chung trước cho OLED và Cảm biến ánh sáng
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setTimeOut(25);

    // 2. Khởi tạo các module
    roomCtrl.init();
    oledDisp.init(roomState);
    sensorMgr.init(roomState);
    netMgr.init(roomState, updateAllOutputs);

    updateAllOutputs();
}

void loop() {
    bool canDoc = false;
    bool canCapNhat = false;

    // 1. Kiểm tra cờ ngắt từ Timer và PIR
    sensorMgr.checkInterruptFlags(canDoc, canCapNhat);

    // 2. Kiểm tra PIR đã qua thời gian ổn định chưa (60s mặc định)
    if (!roomState.pirSanSang && (millis() - roomState.lucBatDau >= PIR_WARMUP_MS)) {
        roomState.pirSanSang = true;
        canCapNhat = true;
    }

    // 3. Nếu PIR có thay đổi hoặc vừa ổn định -> cập nhật trạng thái chuyển động
    if (canCapNhat) {
        sensorMgr.updateMotionState(roomState);
    }

    // 4. Kiểm tra lệnh điều khiển từ UART ('A', '1', '0')
    if (roomCtrl.processSerial(roomState)) {
        canCapNhat = true;
    }

    // 5. Dự phòng nếu phần cứng timer không khả dụng
    if (!roomState.timerOK && (millis() - roomState.mauMs >= CHU_KY_DOC_MS)) {
        canDoc = true;
    }

    // 6. Đọc cảm biến định kỳ (mỗi 2 giây)
    if (canDoc) {
        sensorMgr.readSensors(roomState);
        canCapNhat = true;
    }

    // 7. Cập nhật đầu ra khi có thay đổi trạng thái
    if (canCapNhat) {
        updateAllOutputs();
    }

    // 8. Phục vụ WebServer và duy trì BLE Advertising
    netMgr.handleClients();

    delay(1); // Nhường CPU cho WiFi/BLE background tasks
}
