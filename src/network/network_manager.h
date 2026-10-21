#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <atomic>
#include <functional>
#include "config.h"
#include "app_state.h"

class NetworkManager {
public:
    using UpdateCallback = std::function<void()>;

    NetworkManager();
    void init(RoomState& state, UpdateCallback onModeChanged);
    void handleClients();
    void sendTelemetry(const RoomState& state);

private:
    WebServer _server;
    BLEServer* _bleServer = nullptr;
    BLECharacteristic* _bleDataChar = nullptr;
    bool _wifiOK = false;
    RoomState* _statePtr = nullptr;
    UpdateCallback _onModeChanged;

    static std::atomic<bool> _canQuangBaBLE;

    void handleRoot();
    void handleApiStatus();
    void handleApiMode();
    void initBLE();
    String formatJsonNumber(float val);

    class BleServerCallbacks : public BLEServerCallbacks {
        void onDisconnect(BLEServer*) override {
            _canQuangBaBLE.store(true);
        }
    };
};
