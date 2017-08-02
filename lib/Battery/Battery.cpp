#include "Battery.h"

Battery::Battery(uint8_t pin, uint16_t calibration6V, uint16_t calibration12V)
  : pin(pin), calibration6V(calibration6V), calibration12V(calibration12V) {
  pinMode(pin, INPUT);
}

uint16_t Battery::getRawValue() {
  return analogRead(this->pin);
}

float Battery::getValue() {
  uint16_t battery = this->getRawValue();
  return this->mapfloat(battery, this->calibration6V, this->calibration12V, 0.0, 12.0);
}

float Battery::mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
