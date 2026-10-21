#pragma once
#include <Arduino.h>
#include "config.h"
#include "app_state.h"

class RoomController {
public:
    void init();
    void update(RoomState& state);
    bool processSerial(RoomState& state);
};
