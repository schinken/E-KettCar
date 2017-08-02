#include "GasPedal.h"

GasPedal::GasPedal(uint8_t pin) : pin(pin) {
  pinMode(pin, INPUT);
}

GasPedal::GasPedal(uint8_t pin, uint16_t minValue, uint16_t maxValue)
  : pin(pin), minValue(minValue), maxValue(maxValue) {
  pinMode(pin, INPUT);
}

uint16_t GasPedal::getRawValue() {
  return analogRead(this->pin);
}

uint8_t GasPedal::getValue() {
  uint16_t gas = this->getRawValue();

  gas = constrain(gas, this->minValue, this->maxValue);
  gas = map(gas, this->minValue, this->maxValue, 0, 255);

  return gas;
}

void GasPedal::setMinimum(uint16_t value) {
  this->minValue = value;
}

void GasPedal::setMaximum(uint16_t value) {
  this->maxValue = value;
}
