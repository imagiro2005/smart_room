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
    json += ",\"lightLux\":" + formatJsonNumber(state.lightLux);
    json += ",\"luxThreshold\":" + String(state.luxThreshold, 1);
    json += ",\"potAdcValue\":" + String(state.potAdcValue);
    json += ",\"time\":\"" + state.getTimeString() + "\"";
    json += ",\"date\":\"" + state.getDateString() + "\"";
    json += ",\"isComfortable\":" + String(state.isComfortable ? 1 : 0);
    json += ",\"greenLed\":" + String(state.isComfortable ? 1 : 0);
    json += ",\"redLed\":" + String(state.isComfortable ? 0 : 1);
    json += ",\"isWhiteLampOn\":" + String(state.isWhiteLampOn ? 1 : 0);
    json += ",\"isNight\":" + String(state.isNight ? 1 : 0);
    json += ",\"isDayTooDark\":" + String(state.isDayTooDark ? 1 : 0);
    json += ",\"isDayTooBright\":" + String(state.isDayTooBright ? 1 : 0);
    json += ",\"isNightTooBright\":" + String(state.isNightTooBright ? 1 : 0);
    json += ",\"mode\":" + String(static_cast<int>(state.mode));
    json += ",\"isDataReady\":" + String(state.isDataReady ? 1 : 0);
    json += ",\"isRtcReady\":" + String(state.isRtcReady ? 1 : 0);
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
    int tVal   = isnan(state.temperature) ? -99 : static_cast<int>(round(state.temperature));
    int hVal   = isnan(state.humidity)    ? -1  : static_cast<int>(round(state.humidity));
    int luxVal = isnan(state.lightLux)    ? -1  : static_cast<int>(round(state.lightLux));

    // 1. Compact payload for BLE GATT Notify (under 20 bytes)
    char bleText[28];
    snprintf(bleText, sizeof(bleText), "%d,%d,%d,%d,%d,%d",
             tVal, hVal, luxVal,
             static_cast<int>(state.isComfortable),
             static_cast<int>(state.isWhiteLampOn),
             static_cast<int>(state.isNight));

    if (_bleDataChar && _bleServer) {
        _bleDataChar->setValue(bleText);
        if (_bleServer->getConnectedCount() > 0) {
            _bleDataChar->notify();
        }
    }

    // 2. Clear, tabular, aligned Serial logging with CRLF (\r\n)
    String sTemp = isnan(state.temperature) ? "--" : String(state.temperature, 1);
    String sHum  = isnan(state.humidity)    ? "--" : String(state.humidity, 1);
    String sLux  = isnan(state.lightLux)    ? "--" : String(state.lightLux, 0);

    const char* sComfort = state.isComfortable ? "YES [GREEN]" : "ALERT [RED]";
    const char* sPeriod  = state.isNight ? "NIGHT" : "DAY";

    // Extra status note for white lamp shutoff/activation
    const char* sLampReason = "";
    if (state.isWhiteLampOn) {
        if (!state.isNight && state.isDayTooDark) sLampReason = "(Day Dark Comp.)";
        else if (state.isNight) sLampReason = "(Night Auto)";
        else sLampReason = "(Manual)";
    } else {
        if (!state.isNight && state.isDayTooBright) sLampReason = "(Day Overbright OFF)";
        else if (state.isNight && state.isNightTooBright) sLampReason = "(Night Bright OFF)";
    }

    Serial.printf("[ROOM %s | %-4s] Temp: %5s C | Hum: %5s %% | Lux: %4s lx (%-5s) | Comfort: %-13s | Lamp: %-3s %s\r\n",
                  state.getTimeString().c_str(),
                  state.getModeName(),
                  sTemp.c_str(),
                  sHum.c_str(),
                  sLux.c_str(),
                  sPeriod,
                  sComfort,
                  state.isWhiteLampOn ? "ON" : "OFF",
                  sLampReason);
}
