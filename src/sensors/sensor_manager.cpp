#include "sensor_manager.h"

volatile bool SensorManager::_flagSampleTimer = false;
volatile bool SensorManager::_flagMotionChanged = false;

void IRAM_ATTR SensorManager::isrSampleTimer() {
    _flagSampleTimer = true;
}

void IRAM_ATTR SensorManager::isrPIR() {
    _flagMotionChanged = true;
}

SensorManager::SensorManager() : _dht(PIN_DHT_DATA, SENSOR_DHT_TYPE) {}

void SensorManager::init(RoomState& state) {
    // 1. Configure PIR motion sensor with External Interrupt
    pinMode(PIN_PIR_MOTION, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PIN_PIR_MOTION), isrPIR, CHANGE);

    // 2. Configure 12-bit ADC for Potentiometer and LDR photoresistor
    analogReadResolution(12);
    analogSetPinAttenuation(PIN_POT_ADC, ADC_11db);
    analogSetPinAttenuation(PIN_LDR_ADC, ADC_11db);

    // 3. Initialize DHT22
    _dht.begin();

    // 4. Hardware Timer 0 periodic interrupt (every 2 seconds)
    _sampleTimer = timerBegin(0, 80, true); // Prescaler 80 -> 1 tick = 1 microsecond
    if (_sampleTimer) {
        timerAttachInterrupt(_sampleTimer, isrSampleTimer, true);
        timerAlarmWrite(_sampleTimer, static_cast<uint64_t>(SENSOR_SAMPLE_INTERVAL_MS) * 1000, true);
        timerAlarmEnable(_sampleTimer);
        state.isTimerReady = true;
    } else {
        Serial.println("[SensorManager] TIMER ERROR: falling back to millis()!");
        state.isTimerReady = false;
    }
}

bool SensorManager::checkInterruptFlags(bool& shouldSample, bool& shouldUpdateState) {
    noInterrupts();
    shouldSample = _flagSampleTimer;
    shouldUpdateState = _flagMotionChanged;
    _flagSampleTimer = false;
    _flagMotionChanged = false;
    interrupts();
    return shouldSample || shouldUpdateState;
}

void SensorManager::updateMotionState(RoomState& state) {
    bool rawPir = (digitalRead(PIN_PIR_MOTION) == HIGH);
    state.isMotionDetected = state.isPirReady && rawPir;

    if (state.isMotionDetected) {
        state.lastMotionTimeMs = millis();
    }
}

void SensorManager::readSensors(RoomState& state) {
    // 1. Oversampling (8-sample average) for Potentiometer ADC
    long sumPot = 0;
    for (int i = 0; i < 8; i++) {
        sumPot += analogRead(PIN_POT_ADC);
    }
    state.potRawAdc = sumPot / 8;
    // Map Potentiometer 0..4095 to Threshold Percentage 0%..100%
    state.thresholdPercent = state.potRawAdc * 100.0f / 4095.0f;

    // 2. Oversampling (8-sample average) for LDR Photoresistor ADC
    long sumLdr = 0;
    for (int i = 0; i < 8; i++) {
        sumLdr += analogRead(PIN_LDR_ADC);
    }
    state.ldrRawAdc = sumLdr / 8;
    // LDR Module AO output: lower voltage = brighter light, higher voltage = darker.
    // Convert to Light Percentage (0% = Pitch Black, 100% = Maximum Brightness)
    float calculatedLight = 100.0f - (state.ldrRawAdc * 100.0f / 4095.0f);
    if (calculatedLight < 0.0f) calculatedLight = 0.0f;
    if (calculatedLight > 100.0f) calculatedLight = 100.0f;
    state.lightPercent = calculatedLight;

    // 3. Read DHT22 temperature and humidity
    float t = _dht.readTemperature();
    float h = _dht.readHumidity();

    if (!std::isfinite(t) || t < -40.0f || t > 80.0f) {
        state.temperature = NAN;
    } else {
        state.temperature = t;
    }

    if (!std::isfinite(h) || h < 0.0f || h > 100.0f) {
        state.humidity = NAN;
    } else {
        state.humidity = h;
    }

    // 4. Update motion state
    updateMotionState(state);

    state.isDataReady = true;
    state.sampleTimeMs = millis();
}
