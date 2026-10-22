#include "sensor_manager.h"

volatile bool SensorManager::_flagSampleTimer = false;
volatile bool SensorManager::_flagClockTick = false;
volatile bool SensorManager::_flagButtonPressed = false;
volatile uint32_t SensorManager::_lastButtonDebounceTimeMs = 0;

void IRAM_ATTR SensorManager::isrSampleTimer() {
    _flagSampleTimer = true;
}

void IRAM_ATTR SensorManager::isrClockTick() {
    _flagClockTick = true;
}

void IRAM_ATTR SensorManager::isrButton() {
    uint32_t nowMs = millis();
    // 250ms software debounce
    if (nowMs - _lastButtonDebounceTimeMs > 250) {
        _flagButtonPressed = true;
        _lastButtonDebounceTimeMs = nowMs;
    }
}

SensorManager::SensorManager() : _dht(PIN_DHT_DATA, SENSOR_DHT_TYPE) {}

void SensorManager::triggerBH1750() {
    Wire.beginTransmission(I2C_ADDR_BH);
    Wire.write(0x10); // 0x10: Continuously H-Resolution Mode (1 lx resolution)
    Wire.endTransmission();
}

void SensorManager::init(RoomState& state) {
    // 1. Configure wall push button (INPUT_PULLUP, active LOW interrupt)
    pinMode(PIN_PUSH_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_PUSH_BUTTON), isrButton, FALLING);

    // 2. Configure RTC DS1307 SQW 1Hz square wave interrupt
    pinMode(PIN_RTC_SQW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_RTC_SQW), isrClockTick, FALLING);

    // 3. Initialize RTC DS1307
    if (_rtc.begin()) {
        state.isRtcReady = true;
        if (!_rtc.isrunning()) {
            Serial.println("[SensorManager] RTC DS1307 is not running, setting compile time...");
            _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        // Enable 1Hz square wave output on SQW pin
        _rtc.writeSqwPinMode(DS1307_SquareWave1HZ);
        Serial.println("[SensorManager] RTC DS1307 initialized successfully (SQW: 1Hz)!");
    } else {
        Serial.println("[SensorManager] ERROR: RTC DS1307 not found on I2C bus!");
        state.isRtcReady = false;
    }

    // 4. Configure ADC for threshold potentiometer
    analogReadResolution(12);
    analogSetPinAttenuation(PIN_POT_ADC, ADC_11db);

    // 5. Initialize DHT sensor
    _dht.begin();

    // 6. Trigger initial BH1750 measurement
    triggerBH1750();

    // 7. Hardware Timer 0 periodic interrupt (2s)
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

bool SensorManager::checkInterruptFlags(bool& shouldSample, bool& shouldRefreshDisplay) {
    noInterrupts();
    shouldSample = _flagSampleTimer;
    shouldRefreshDisplay = _flagClockTick;
    _flagSampleTimer = false;
    _flagClockTick = false;
    interrupts();
    return shouldSample || shouldRefreshDisplay;
}

bool SensorManager::checkButtonPress() {
    if (_flagButtonPressed) {
        _flagButtonPressed = false;
        return true;
    }
    return false;
}

void SensorManager::readSensors(RoomState& state) {
    // 1. Read real-time clock from RTC DS1307
    if (state.isRtcReady) {
        DateTime now = _rtc.now();
        state.hour    = now.hour();
        state.minute  = now.minute();
        state.second  = now.second();
        state.day     = now.day();
        state.month   = now.month();
        state.year    = now.year();
        // Nighttime is defined from 18:00 to 05:59
        state.isNight = (state.hour >= NIGHT_START_HOUR || state.hour < NIGHT_END_HOUR);
    }

    // 2. Read potentiometer ADC (8-sample average oversampling)
    long sumAdc = 0;
    for (int i = 0; i < 8; i++) {
        sumAdc += analogRead(PIN_POT_ADC);
    }
    state.potAdcValue = sumAdc / 8;
    state.luxThreshold = 20.0f + (state.potAdcValue * 480.0f / 4095.0f); // 20 .. 500 lux

    // 3. Read BH1750 ambient light sensor
    state.lightLux = NAN;
    if (Wire.requestFrom(static_cast<uint8_t>(I2C_ADDR_BH), static_cast<uint8_t>(2)) == 2) {
        int msb = Wire.read();
        int lsb = Wire.read();
        int rawValue = (msb << 8) | lsb;
        state.lightLux = rawValue / 1.2f; // Convert raw to Lux per datasheet
    }
    triggerBH1750(); // Trigger next cycle

    // 4. Read DHT temperature and humidity
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

    state.isDataReady = true;
    state.sampleTimeMs = millis();
}
