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

    int bleClientCount = _bleServer ? _bleServer->getConnectedCount() : 0;

    String json;
    json.reserve(580);
    json = "{\"temperature\":" + formatJsonNumber(state.temperature);
    json += ",\"humidity\":" + formatJsonNumber(state.humidity);
    json += ",\"lightPercent\":" + String(state.lightPercent, 1);
    json += ",\"thresholdPercent\":" + String(state.thresholdPercent, 1);
    json += ",\"potRawAdc\":" + String(state.potRawAdc);
    json += ",\"ldrRawAdc\":" + String(state.ldrRawAdc);
    json += ",\"isRoomDark\":" + String(state.isRoomDark ? 1 : 0);
    json += ",\"isMotionDetected\":" + String(state.isMotionDetected ? 1 : 0);
    json += ",\"isEnvironmentSafe\":" + String(state.isEnvironmentSafe ? 1 : 0);
    json += ",\"greenLed\":" + String(state.isEnvironmentSafe ? 1 : 0);
    json += ",\"redLed\":" + String(state.isEnvironmentSafe ? 0 : 1);
    json += ",\"whiteLamp\":" + String(state.isWhiteLampOn ? 1 : 0);
    json += ",\"mode\":" + String(static_cast<int>(state.mode));
    json += ",\"isDataReady\":" + String(state.isDataReady ? 1 : 0);
    json += ",\"isPirReady\":" + String(state.isPirReady ? 1 : 0);
    json += ",\"isTimerReady\":" + String(state.isTimerReady ? 1 : 0);
    json += ",\"isOledReady\":" + String(state.isOledReady ? 1 : 0);
    json += ",\"bleClients\":" + String(bleClientCount);
    json += ",\"uptimeSec\":" + String(millis() / 1000);
    json += ",\"sampleTimeMs\":" + String(state.sampleTimeMs) + "}";

    _server.sendHeader("Cache-Control", "no-store");
    _server.send(200, "application/json", json);
}

void NetworkManager::handleApiMode() {
    if (!_statePtr) return;

    String modeArg = _server.arg("mode");
    if (modeArg != "0" && modeArg != "1" && modeArg != "2") {
        _server.send(400, "application/json", "{\"error\":\"invalid_mode\"}");
        return;
    }

    _statePtr->mode = static_cast<RoomMode>(modeArg.toInt());

    if (_onModeChanged) {
        _onModeChanged();
    }

    handleApiStatus();
}

void NetworkManager::initBLE() {
    BLEDevice::init(BLE_DEVICE_NAME);
    _bleServer = BLEDevice::createServer();
    _bleServer->setCallbacks(new BleServerCallbacks());

    BLEService* service = _bleServer->createService(BLE_SERVICE_UUID);
    _bleDataChar = service->createCharacteristic(
        BLE_DATA_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    _bleDataChar->addDescriptor(new BLE2902());
    _bleDataChar->setValue("WAITING DATA");
    service->start();

    BLEAdvertising* adv = BLEDevice::getAdvertising();
    adv->addServiceUUID(BLE_SERVICE_UUID);
    adv->setScanResponse(true);
    BLEDevice::startAdvertising();
}

void NetworkManager::init(RoomState& state, UpdateCallback onModeChanged) {
    _statePtr = &state;
    _onModeChanged = onModeChanged;

    // 1. Initialize WiFi SoftAP
    WiFi.mode(WIFI_AP);
    _wifiOK = WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    if (_wifiOK) {
        Serial.printf("[Network] Wi-Fi AP: %s | Pass: %s | URL: http://%s\r\n",
                      WIFI_SSID, WIFI_PASSWORD, WiFi.softAPIP().toString().c_str());

        _server.on("/", HTTP_GET, [this]() { handleRoot(); });
        _server.on("/api/status", HTTP_GET, [this]() { handleApiStatus(); });
        _server.on("/api/mode", HTTP_POST, [this]() { handleApiMode(); });
        _server.begin();
    } else {
        Serial.println("[Network] ERROR: SoftAP initialization failed!");
    }

    // 2. Initialize BLE
    initBLE();
    Serial.println("[Network] BLE initialized. UART control: A=AUTO, 1=ON, 0=OFF");
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
    int tVal = isnan(state.temperature) ? -99 : static_cast<int>(round(state.temperature));
    int hVal = isnan(state.humidity)    ? -1  : static_cast<int>(round(state.humidity));
    int lVal = static_cast<int>(round(state.lightPercent));

    // 1. Compact payload for BLE GATT Notify (under 20 bytes)
    char bleText[28];
    snprintf(bleText, sizeof(bleText), "%d,%d,%d,%d,%d,%d",
             tVal, hVal, lVal,
             static_cast<int>(state.isMotionDetected),
             static_cast<int>(state.isEnvironmentSafe),
             static_cast<int>(state.isWhiteLampOn));

    if (_bleDataChar && _bleServer) {
        _bleDataChar->setValue(bleText);
        if (_bleServer->getConnectedCount() > 0) {
            _bleDataChar->notify();
        }
    }

    // 2. Clear, tabular, aligned Serial logging with CRLF (\r\n)
    String sTemp = isnan(state.temperature) ? "--" : String(state.temperature, 1);
    String sHum  = isnan(state.humidity)    ? "--" : String(state.humidity, 1);

    const char* sEnvState = state.isEnvironmentSafe ? "SAFE [GREEN]" : "ALERT [RED]";
    const char* sDarkText = state.isRoomDark ? "DARK" : "BRIGHT";

    Serial.printf("[ROOM | %-4s] Temp: %5s C | Hum: %5s %% | Light: %3d%% (Set:%3d%% - %-6s) | PIR: %-3s | State: %-12s | Lamp: %-3s\r\n",
                  state.getModeName(),
                  sTemp.c_str(),
                  sHum.c_str(),
                  static_cast<int>(state.lightPercent),
                  static_cast<int>(state.thresholdPercent),
                  sDarkText,
                  state.isMotionDetected ? "YES" : "NO",
                  sEnvState,
                  state.isWhiteLampOn ? "ON" : "OFF");
}
