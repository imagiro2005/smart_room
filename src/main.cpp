#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "app_state.h"
#include "sensors/sensor_manager.h"
#include "controller/room_controller.h"
#include "display/oled_display.h"
#include "network/network_manager.h"

// System state & module instances
RoomState      roomState;
SensorManager  sensorMgr;
RoomController roomCtrl;
OledDisplay    oledDisp;
NetworkManager netMgr;

// Synchronize all hardware outputs (3 LEDs, OLED, BLE, Serial)
void updateAllOutputs() {
    roomCtrl.update(roomState);
    oledDisp.update(roomState);
    netMgr.sendTelemetry(roomState);
}

void setup() {
    Serial.begin(115200);
    roomState.bootTimeMs = millis();

    // 1. Initialize shared I2C bus for OLED SSD1306
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setTimeOut(25);

    // 2. Initialize all modules
    roomCtrl.init();
    oledDisp.init(roomState);
    sensorMgr.init(roomState);
    netMgr.init(roomState, updateAllOutputs);

    updateAllOutputs();
}

void loop() {
    bool shouldSample = false;
    bool shouldUpdateState = false;

    // 1. Check hardware interrupt flags (Timer alarm & PIR motion interrupt)
    sensorMgr.checkInterruptFlags(shouldSample, shouldUpdateState);

    // 2. Check initial PIR sensor warm-up stabilization (15 seconds)
    if (!roomState.isPirReady && (millis() - roomState.bootTimeMs >= PIR_WARMUP_MS)) {
        roomState.isPirReady = true;
        shouldUpdateState = true;
    }

    // 3. Update motion state if PIR triggered or warm-up complete
    if (shouldUpdateState) {
        sensorMgr.updateMotionState(roomState);
    }

    // 4. Process UART serial commands ('A' = Auto, '1' = Lamp ON, '0' = Lamp OFF)
    if (roomCtrl.processSerial(roomState)) {
        shouldUpdateState = true;
    }

    // 5. Timer fallback in case hardware timer fails
    if (!roomState.isTimerReady && (millis() - roomState.sampleTimeMs >= SENSOR_SAMPLE_INTERVAL_MS)) {
        shouldSample = true;
    }

    // 6. Periodic sensor sampling (DHT22, Potentiometer, LDR)
    if (shouldSample) {
        sensorMgr.readSensors(roomState);
        shouldUpdateState = true;
    }

    // 7. Synchronize outputs if state changed or off-delay hold window is active
    if (shouldUpdateState || roomState.isMotionHoldActive) {
        updateAllOutputs();
    }

    // 8. Handle WebServer HTTP requests & maintain BLE advertising
    netMgr.handleClients();

    delay(1); // Yield CPU to FreeRTOS background tasks (WiFi/BLE)
}
