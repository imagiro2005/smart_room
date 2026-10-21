#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_vcc;
  pin_t pin_gnd;
  pin_t pin_sda;
  pin_t pin_scl;
  pin_t pin_addr;
  uint32_t attr_lux;
  uint16_t raw_value;
  uint8_t read_index;
} chip_state_t;

static bool on_i2c_connect(void *user_data, uint32_t address, bool connect) {
  chip_state_t *chip = (chip_state_t *)user_data;
  if (connect) {
    float lux = attr_read_float(chip->attr_lux);
    if (lux < 0.0f) lux = 0.0f;
    // Theo datasheet BH1750: lux = raw / 1.2 => raw = lux * 1.2
    chip->raw_value = (uint16_t)(lux * 1.2f);
    chip->read_index = 0;
  }
  return true; // ACK
}

static uint8_t on_i2c_read(void *user_data) {
  chip_state_t *chip = (chip_state_t *)user_data;
  uint8_t byte_to_return = 0;
  if (chip->read_index == 0) {
    byte_to_return = (chip->raw_value >> 8) & 0xFF; // High byte
    chip->read_index++;
  } else if (chip->read_index == 1) {
    byte_to_return = chip->raw_value & 0xFF;        // Low byte
    chip->read_index++;
  }
  return byte_to_return;
}

static bool on_i2c_write(void *user_data, uint8_t data) {
  // Nhận lệnh từ Master (ví dụ 0x10 H-Resolution Mode)
  return true; // ACK
}

static void on_i2c_disconnect(void *user_data) {
  chip_state_t *chip = (chip_state_t *)user_data;
  chip->read_index = 0;
}

void chip_init(void) {
  chip_state_t *chip = (chip_state_t *)malloc(sizeof(chip_state_t));
  chip->pin_vcc  = pin_init("VCC", INPUT);
  chip->pin_gnd  = pin_init("GND", INPUT);
  chip->pin_sda  = pin_init("SDA", INPUT);
  chip->pin_scl  = pin_init("SCL", INPUT);
  chip->pin_addr = pin_init("ADDR", INPUT);

  chip->attr_lux = attr_init_float("lux", 150.0f);
  chip->read_index = 0;
  chip->raw_value = (uint16_t)(150.0f * 1.2f);

  const i2c_config_t i2c_config = {
    .user_data = chip,
    .address = 0x23,
    .scl = chip->pin_scl,
    .sda = chip->pin_sda,
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = on_i2c_disconnect,
  };
  i2c_init(&i2c_config);
}
