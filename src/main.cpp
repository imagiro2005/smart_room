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

    // 1. Initialize shared I2C bus before peripherals (BH1750, OLED, RTC DS1307)
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
    bool shouldRefreshDisplay = false;

    // 1. Check hardware interrupt flags (Timer alarm & RTC 1Hz square wave)
    sensorMgr.checkInterruptFlags(shouldSample, shouldRefreshDisplay);

    // 2. Check physical wall push button event (External Interrupt)
    if (sensorMgr.checkButtonPress()) {
        roomCtrl.handleButtonPress(roomState);
        shouldRefreshDisplay = true;
    }

    // 3. Process UART commands ('A' = Auto, '1' = Lamp ON, '0' = Lamp OFF)
    if (roomCtrl.processSerial(roomState)) {
        shouldRefreshDisplay = true;
    }

    // 4. Timer fallback in case hardware timer fails
    if (!roomState.isTimerReady && (millis() - roomState.sampleTimeMs >= SENSOR_SAMPLE_INTERVAL_MS)) {
        shouldSample = true;
    }

    // 5. Periodic sensor sampling & RTC time update
    if (shouldSample) {
        sensorMgr.readSensors(roomState);
        shouldRefreshDisplay = true;
    }

    // 6. Update system outputs if state changed
    if (shouldRefreshDisplay) {
        updateAllOutputs();
    }

    // 7. Handle WebServer HTTP requests & maintain BLE advertising
    netMgr.handleClients();

    delay(1); // Yield CPU to FreeRTOS background tasks (WiFi/BLE)
}
