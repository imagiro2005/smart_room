#include "sensor_manager.h"

volatile bool SensorManager::_flagKyDoc = false;
volatile bool SensorManager::_flagPIRThayDoi = false;

void IRAM_ATTR SensorManager::isrTimer() {
    _flagKyDoc = true;
}

void IRAM_ATTR SensorManager::isrPIR() {
    _flagPIRThayDoi = true;
}

SensorManager::SensorManager() : _dht(PIN_DHT, SENSOR_DHT_TYPE) {}

void SensorManager::triggerBH1750() {
    Wire.beginTransmission(ADDR_BH);
    Wire.write(0x10); // Lệnh 0x10: Đo độ sáng liên tục, độ phân giải cao 1 lx
    Wire.endTransmission();
}

void SensorManager::init(RoomState& state) {
    // Cấu hình PIR
    pinMode(PIN_PIR, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PIN_PIR), isrPIR, CHANGE);

    // Cấu hình ADC cho biến trở
    analogReadResolution(12);
    analogSetPinAttenuation(PIN_POT, ADC_11db);

    // Cấu hình DHT11
    _dht.begin();

    // Cấu hình I2C cho cảm biến ánh sáng BH1750
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setTimeOut(25);
    triggerBH1750();

    // Khởi tạo phần cứng Timer ngắt định kỳ
    _timerDoc = timerBegin(0, 80, true); // Prescaler 80 -> 1 tick = 1 micro giây
    if (_timerDoc) {
        timerAttachInterrupt(_timerDoc, isrTimer, true);
        timerAlarmWrite(_timerDoc, static_cast<uint64_t>(CHU_KY_DOC_MS) * 1000, true);
        timerAlarmEnable(_timerDoc);
        state.timerOK = true;
    } else {
        Serial.println("[SensorManager] LOI TIMER: su dung millis du phong!");
        state.timerOK = false;
    }
}

bool SensorManager::checkInterruptFlags(bool& canDoc, bool& canCapNhat) {
    noInterrupts();
    canDoc = _flagKyDoc;
    canCapNhat = _flagPIRThayDoi;
    _flagKyDoc = false;
    _flagPIRThayDoi = false;
    interrupts();
    return canDoc || canCapNhat;
}

void SensorManager::updateMotionState(RoomState& state) {
    state.coChuyenDong = state.pirSanSang && (digitalRead(PIN_PIR) == HIGH);
}

void SensorManager::readSensors(RoomState& state) {
    // 1. Đọc biến trở ADC (lấy mẫu trung bình 8 lần để chống nhiễu)
    long tongADC = 0;
    for (int i = 0; i < 8; i++) {
        tongADC += analogRead(PIN_POT);
    }
    state.giaTriADC = tongADC / 8;
    state.nguongToi = 20.0f + (state.giaTriADC * 480.0f / 4095.0f); // 20 .. 500 lux

    // 2. Đọc cảm biến ánh sáng BH1750
    state.doSang = NAN;
    if (Wire.requestFrom(static_cast<uint8_t>(ADDR_BH), static_cast<uint8_t>(2)) == 2) {
        int byteCao = Wire.read();
        int byteThap = Wire.read();
        int giaTriTho = (byteCao << 8) | byteThap;
        state.doSang = giaTriTho / 1.2f; // Quy đổi lux theo datasheet
    }
    triggerBH1750(); // Kích hoạt chu kỳ đo kế tiếp

    // 3. Đọc nhiệt độ và độ ẩm từ DHT11
    float t = _dht.readTemperature();
    float h = _dht.readHumidity();

    if (!std::isfinite(t) || t < -40.0f || t > 80.0f) {
        state.nhietDo = NAN;
    } else {
        state.nhietDo = t;
    }

    if (!std::isfinite(h) || h < 0.0f || h > 100.0f) {
        state.doAm = NAN;
    } else {
        state.doAm = h;
    }

    // 4. Cập nhật chuyển động
    updateMotionState(state);

    state.daCoDuLieu = true;
    state.mauMs = millis();
}
