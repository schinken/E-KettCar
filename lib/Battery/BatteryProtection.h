#ifndef __BATTERY_PROTECTION_H__
#define __BATTERY_PROTECTION_H__

#include <Arduino.h>
#include <Battery.h>
#include <ExponentialSmoothing.h>

const float  LOW_POWER_VOLTAGE = 10.8;
const unsigned long LOW_POWER_TIME_MS = (1L * 60 * 1000);

const float BATTERY_PROTECTION_VOLTAGE = 10.5;
const unsigned long BATTERY_PROTECTION_MS = (1L * 60 * 1000);

class BatteryProtection {

    public:
      BatteryProtection(Battery* battery);

      void update();
      bool isLowPower();
      bool isBatteryProtection();

    private:
      Battery* battery;
      ExponentialSmoothing* smoothBattery;

      unsigned long lowPowerStartMs = 0;
      unsigned long batteryProtectionStartMs = 0;

      bool lowPower = false;
      bool batteryProtection = false;
};

#endif
