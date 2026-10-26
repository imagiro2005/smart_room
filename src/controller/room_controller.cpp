#include "room_controller.h"

void RoomController::init() {
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_WHITE, OUTPUT);

    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_WHITE, LOW);
}

void RoomController::update(RoomState& state) {
    if (!state.isDataReady) {
        return;
    }

    uint32_t nowMs = millis();

    // ========================================================
    // 1. ENVIRONMENTAL SAFETY MONITORING (GREEN & RED LED)
    // ========================================================
    // Temperature safety check (18°C - 30°C)
    state.isTempSafe = (!isnan(state.temperature) && 
                        state.temperature >= TEMP_SAFE_MIN && 
                        state.temperature <= TEMP_SAFE_MAX);

    // Humidity safety check (40% - 75%)
    state.isHumSafe = (!isnan(state.humidity) && 
                       state.humidity >= HUM_SAFE_MIN && 
                       state.humidity <= HUM_SAFE_MAX);

    // Both conditions must be satisfied for a SAFE environment
    state.isEnvironmentSafe = (state.isTempSafe && state.isHumSafe);

    // Green LED and Red LED drive (mutually exclusive)
    if (state.isEnvironmentSafe) {
        digitalWrite(PIN_LED_GREEN, HIGH); // Safe: Green LED ON
        digitalWrite(PIN_LED_RED, LOW);     // Red LED OFF
    } else {
        digitalWrite(PIN_LED_GREEN, LOW);  // Threshold violation or sensor fault: Green LED OFF
        digitalWrite(PIN_LED_RED, HIGH);   // Alert: Red LED ON
    }

    // ========================================================
    // 2. SMART ROOM LIGHTING CONTROL (WHITE LED)
    // ========================================================
    // Evaluate if room is DARK by comparing LDR light level against Potentiometer threshold
    if (state.lightPercent <= state.thresholdPercent) {
        state.isRoomDark = true;
    } else if (state.lightPercent >= state.thresholdPercent + LIGHT_HYSTERESIS_PERCENT) {
        state.isRoomDark = false;
    }

    // Off-delay hold window check (30 seconds retention after last detected motion)
    bool isWithinHoldTime = (nowMs - state.lastMotionTimeMs < LAMP_OFF_DELAY_MS);
    state.isMotionHoldActive = state.isPirReady && isWithinHoldTime && (state.lastMotionTimeMs > 0);

    // Lighting decision based on active mode
    if (state.mode == MODE_MANUAL_ON) {
        // Manual override: Force ON
        state.isWhiteLampOn = true;
    } else if (state.mode == MODE_MANUAL_OFF) {
        // Manual override: Force OFF
        state.isWhiteLampOn = false;
    } else {
        // AUTOMATIC MODE (MODE_AUTO):
        // Turn ON if room is DARK AND (motion is currently active OR within 30s off-delay hold window)
        bool hasOccupant = (state.isMotionDetected || state.isMotionHoldActive);
        state.isWhiteLampOn = state.isRoomDark && hasOccupant;
    }

    // Drive White LED output
    digitalWrite(PIN_LED_WHITE, state.isWhiteLampOn ? HIGH : LOW);
}

bool RoomController::processSerial(RoomState& state) {
    if (!Serial.available()) {
        return false;
    }

    char c = static_cast<char>(Serial.read());
    if (c == 'A' || c == 'a') {
        state.mode = MODE_AUTO;
    } else if (c == '1') {
        state.mode = MODE_MANUAL_ON;
    } else if (c == '0') {
        state.mode = MODE_MANUAL_OFF;
    } else {
        return false;
    }

    return true;
}
