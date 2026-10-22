#include "room_controller.h"

void RoomController::init() {
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_WHITE, OUTPUT);

    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_WHITE, LOW);
}

void RoomController::handleButtonPress(RoomState& state) {
    // Cyclic mode toggle via physical wall button: AUTO -> MANUAL_ON -> MANUAL_OFF -> AUTO
    if (state.mode == MODE_AUTO) {
        state.mode = state.isWhiteLampOn ? MODE_MANUAL_OFF : MODE_MANUAL_ON;
    } else if (state.mode == MODE_MANUAL_ON) {
        state.mode = MODE_MANUAL_OFF;
    } else {
        state.mode = MODE_AUTO;
    }

    Serial.printf("[BUTTON] Physical button pressed! New mode: %s\r\n", state.getModeName());
}

void RoomController::update(RoomState& state) {
    if (!state.isDataReady) {
        return;
    }

    // ========================================================
    // 1. ENVIRONMENTAL COMFORT EVALUATION
    // ========================================================
    // Temperature comfort check (20°C - 28°C)
    state.isTempComfortable = (!isnan(state.temperature) && 
                               state.temperature >= TEMP_COMFORT_MIN && 
                               state.temperature <= TEMP_COMFORT_MAX);

    // Humidity comfort check (40% - 70%)
    state.isHumidityComfortable = (!isnan(state.humidity) && 
                                   state.humidity >= HUM_COMFORT_MIN && 
                                   state.humidity <= HUM_COMFORT_MAX);

    // Ambient light comfort check (Different thresholds for Day vs. Night)
    if (state.isNight) {
        // Nighttime (18:00 - 05:59): Soft ambient lighting (20 - 300 lux)
        state.isLightComfortable = (!isnan(state.lightLux) && 
                                    state.lightLux >= LUX_NIGHT_COMFORT_MIN && 
                                    state.lightLux <= LUX_NIGHT_COMFORT_MAX);
    } else {
        // Daytime (06:00 - 17:59): Sufficient work/study lighting (150 - 1000 lux)
        state.isLightComfortable = (!isnan(state.lightLux) && 
                                    state.lightLux >= LUX_DAY_COMFORT_MIN && 
                                    state.lightLux <= LUX_DAY_COMFORT_MAX);
    }

    // Environment is COMFORTABLE only if ALL THREE criteria are met
    state.isComfortable = (state.isTempComfortable && 
                           state.isHumidityComfortable && 
                           state.isLightComfortable);

    // Green LED and Red LED control (mutually exclusive)
    if (state.isComfortable) {
        digitalWrite(PIN_LED_GREEN, HIGH); // Ideal environment: Green LED ON
        digitalWrite(PIN_LED_RED, LOW);     // Red LED OFF
    } else {
        digitalWrite(PIN_LED_GREEN, LOW);  // Threshold violation: Green LED OFF
        digitalWrite(PIN_LED_RED, HIGH);   // Red LED ON (Alert)
    }

    // ========================================================
    // 2. ROOM LIGHTING CONTROL (WHITE LED)
    // ========================================================
    if (state.mode == MODE_MANUAL_ON) {
        // Manual ON: Lamp is ON, BUT if daytime ambient light is extremely bright
        // (>= 800 lux), turn OFF automatically to prevent glare & save energy.
        if (!state.isNight && !isnan(state.lightLux) && state.lightLux >= LUX_DAY_OVER_BRIGHT) {
            state.isWhiteLampOn = false;
            state.isDayTooBright = true;
        } else {
            state.isWhiteLampOn = true;
            state.isDayTooBright = false;
        }
    } else if (state.mode == MODE_MANUAL_OFF) {
        state.isWhiteLampOn = false;
    } else {
        // AUTOMATIC MODE (MODE_AUTO):
        if (!state.isNight) {
            // --- DAYTIME (06:00 - 17:59) ---
            // 1. Dark compensation: if ambient drops below 120 lux -> Turn ON lamp
            if (!isnan(state.lightLux)) {
                if (state.lightLux < LUX_DAY_DARK_ON) {
                    state.isDayTooDark = true;
                } else if (state.lightLux >= LUX_DAY_DARK_OFF) {
                    state.isDayTooDark = false;
                }

                // 2. Excessive daylight shutoff: if ambient light becomes excessively bright (>= 800 lux),
                // turn off the white lamp immediately if it is currently ON.
                if (state.lightLux >= LUX_DAY_OVER_BRIGHT) {
                    state.isDayTooBright = true;
                    state.isDayTooDark = false;
                } else {
                    state.isDayTooBright = false;
                }
            }
            state.isWhiteLampOn = state.isDayTooDark && !state.isDayTooBright;
        } else {
            // --- NIGHTTIME (18:00 - 05:59) ---
            // 1. Normal evening behavior: lamp is ON.
            // 2. Excessive brightness shutoff: if night ambient light > 350 lux -> Turn OFF lamp.
            if (!isnan(state.lightLux)) {
                if (state.lightLux > LUX_NIGHT_BRIGHT_OFF) {
                    state.isNightTooBright = true;
                } else if (state.lightLux <= LUX_NIGHT_BRIGHT_ON) {
                    state.isNightTooBright = false;
                }
            }
            state.isWhiteLampOn = !state.isNightTooBright;
        }
    }

    // Output drive for White LED
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
