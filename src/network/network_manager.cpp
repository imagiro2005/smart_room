#include "network_manager.h"
#include "web_page.h"

std::atomic<bool> NetworkManager::_canQuangBaBLE{false};

NetworkManager::NetworkManager() : _server(80) {}

String NetworkManager::formatJsonNumber(float val) {
    if (isnan(val)) {
        return "null";
    }
    return String(val, 1);
}

void NetworkManager::handleRoot() {
    _server.sendHeader("Cache-Control", "no-cache");
    _server.send_P(200, "text/html; charset=utf-8", SMARTROOM_PAGE);
}

void NetworkManager::handleApiStatus() {
    if (!_statePtr) return;
    const RoomState& state = *_statePtr;

    int soMayBLE = _bleServer ? _bleServer->getConnectedCount() : 0;

    String json;
    json.reserve(560);
    json = "{\"t\":" + formatJsonNumber(state.nhietDo);
    json += ",\"h\":" + formatJsonNumber(state.doAm);
    json += ",\"lux\":" + formatJsonNumber(state.doSang);
    json += ",\"threshold\":" + String(state.nguongToi, 1);
    json += ",\"adc\":" + String(state.giaTriADC);
    json += ",\"pir\":" + String(state.coChuyenDong ? 1 : 0);
    json += ",\"lamp\":" + String(state.denBat ? 1 : 0);
    json += ",\"alert\":" + String(state.canhBao ? 1 : 0);
    json += ",\"dark\":" + String(state.phongToi ? 1 : 0);
    json += ",\"mode\":" + String(static_cast<int>(state.cheDo));
    json += ",\"ready\":" + String(state.daCoDuLieu ? 1 : 0);
    json += ",\"pirReady\":" + String(state.pirSanSang ? 1 : 0);
    json += ",\"timer\":" + String(state.timerOK ? 1 : 0);
    json += ",\"oled\":" + String(state.oledOK ? 1 : 0);
    json += ",\"bleClients\":" + String(soMayBLE);
    json += ",\"tMax\":" + String(NGUONG_NHIET_CAO, 1);
    json += ",\"hMax\":" + String(NGUONG_AM_CAO, 1);
    json += ",\"uptime\":" + String(millis() / 1000);
    json += ",\"sampleMs\":" + String(state.mauMs) + "}";

    _server.sendHeader("Cache-Control", "no-store");
    _server.send(200, "application/json", json);
}

void NetworkManager::handleApiMode() {
    if (!_statePtr) return;

    String modeArg = _server.arg("mode");
    if (modeArg != "0" && modeArg != "1" && modeArg != "2") {
        _server.send(400, "application/json", "{\"error\":\"mode\"}");
        return;
    }

    _statePtr->cheDo = static_cast<RoomMode>(modeArg.toInt());

    if (_onModeChanged) {
        _onModeChanged();
    }

    handleApiStatus();
}

void NetworkManager::initBLE() {
    BLEDevice::init(BLE_DEVICE_NAME);
    _bleServer = BLEDevice::createServer();
    _bleServer->setCallbacks(new BleServerCallbacks());

    BLEService* dichVu = _bleServer->createService(BLE_SERVICE_UUID);
    _bleDataChar = dichVu->createCharacteristic(
        BLE_DATA_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    _bleDataChar->addDescriptor(new BLE2902());
    _bleDataChar->setValue("CHO DU LIEU");
    dichVu->start();

    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(BLE_SERVICE_UUID);
    adv->setScanResponse(true);
    BLEDevice::startAdvertising();
}

void NetworkManager::init(RoomState& state, UpdateCallback onModeChanged) {
    _statePtr = &state;
    _onModeChanged = onModeChanged;

    // 1. Khởi tạo WiFi SoftAP
    WiFi.mode(WIFI_AP);
    _wifiOK = WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    if (_wifiOK) {
        Serial.printf("[Network] Wi-Fi: %s | Pass: %s | URL: http://%s\n",
                      WIFI_SSID, WIFI_PASSWORD, WiFi.softAPIP().toString().c_str());

        _server.on("/", HTTP_GET, [this]() { handleRoot(); });
        _server.on("/api/status", HTTP_GET, [this]() { handleApiStatus(); });
        _server.on("/api/mode", HTTP_POST, [this]() { handleApiMode(); });
        _server.begin();
    } else {
        Serial.println("[Network] LOI KHOI TAO WIFI SOFTAP!");
    }

    // 2. Khởi tạo BLE
    initBLE();
    Serial.println("[Network] BLE da khoi dong. UART dieu khien: A=AUTO, 1=ON, 0=OFF");
}

void NetworkManager::handleClients() {
    if (_wifiOK) {
        _server.handleClient();
    }
    if (_canQuangBaBLE.exchange(false)) {
        BLEDevice::startAdvertising();
    }
}

void NetworkManager::sendTelemetry(const RoomState& state) {
    int nhietGui = isnan(state.nhietDo) ? -99 : static_cast<int>(round(state.nhietDo));
    int amGui    = isnan(state.doAm)    ? -1  : static_cast<int>(round(state.doAm));
    int sangGui  = isnan(state.doSang)  ? -1  : static_cast<int>(round(state.doSang));

    // 1. Dữ liệu gọn nhẹ gửi qua BLE GATT Notify (tối đa 20 byte)
    char text[24];
    snprintf(text, sizeof(text), "%d,%d,%d,%d,%d,%d",
             nhietGui, amGui, sangGui,
             static_cast<int>(state.coChuyenDong),
             static_cast<int>(state.denBat),
             static_cast<int>(state.canhBao));

    if (_bleDataChar && _bleServer) {
        _bleDataChar->setValue(text);
        if (_bleServer->getConnectedCount() > 0) {
            _bleDataChar->notify();
        }
    }

    // 2. In Serial trực quan, định dạng căn lề bảng thẳng tắp, dùng \r\n chống thụt dòng
    String sTemp = isnan(state.nhietDo) ? "--" : String(state.nhietDo, 1);
    String sHum  = isnan(state.doAm)    ? "--" : String(state.doAm, 1);
    String sLux  = isnan(state.doSang)  ? "--" : String(state.doSang, 0);

    Serial.printf("[ROOM] Mode: %-4s | Temp: %5s C | Hum: %5s %% | Lux: %4s lx | Person: %-3s | Lamp: %-3s | Alert: %-3s\r\n",
                  state.getModeName(),
                  sTemp.c_str(),
                  sHum.c_str(),
                  sLux.c_str(),
                  state.coChuyenDong ? "YES" : "NO",
                  state.denBat ? "ON" : "OFF",
                  state.canhBao ? "YES" : "NO");
}
