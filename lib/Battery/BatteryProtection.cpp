#include "BatteryProtection.h"

BatteryProtection::BatteryProtection(Battery *battery) {
  this->battery = battery;

  this->smoothBattery = new ExponentialSmoothing(0.03);
  this->smoothBattery->setValue(battery->getValue());
}

bool BatteryProtection::isLowPower() {
  return this->lowPower;
}

bool BatteryProtection::isBatteryProtection() {
  return this->batteryProtection;
}

void BatteryProtection::update() {
  float battery = this->smoothBattery->getValue();

  Serial.print("BP: ");
  Serial.println(battery);

  if (battery < BATTERY_PROTECTION_VOLTAGE) {
    if (this->batteryProtectionStartMs == 0) {
      Serial.println("Battery protection start ms!");
      this->batteryProtectionStartMs = millis();
    }

    if (millis() - this->batteryProtectionStartMs > BATTERY_PROTECTION_MS) {
      Serial.println("Battery protection since 1 min!");
      this->lowPower = true;
      this->batteryProtection = true;
    }

  } else if (battery < LOW_POWER_VOLTAGE) {

    // It's defenitely not battery protection!
    this->batteryProtection = false;

    if (this->lowPowerStartMs == 0) {
      Serial.println("low power start ms!");
      this->lowPowerStartMs = millis();
    }

    if (millis() - this->lowPowerStartMs > LOW_POWER_TIME_MS) {
      Serial.println("low power since 1 min!");
      this->lowPower = true;
    }

  } else {
    Serial.println("LOW POWER / BATTERY PROTECT disabled");

    this->lowPower = false;
    this->batteryProtection = false;

    this->lowPowerStartMs = 0;
    this->batteryProtectionStartMs = 0;
  }

  this->smoothBattery->update(this->battery->getValue());
}
