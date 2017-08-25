#include "ExponentialSmoothing.h"

ExponentialSmoothing::ExponentialSmoothing()
  : smoothValue(0), alpha(ExponentialSmoothing::DEFAULT_ALPHA) {
}

ExponentialSmoothing::ExponentialSmoothing(float alpha)
  : smoothValue(0), alpha(alpha) {
}

float ExponentialSmoothing::getValue() {
  return this->smoothValue;
}

void ExponentialSmoothing::setValue(float value) {
  this->smoothValue = value;
}

void ExponentialSmoothing::update(float newValue) {
  this->smoothValue = this->alpha * newValue + (1.0 - this->alpha) * this->smoothValue;
}
